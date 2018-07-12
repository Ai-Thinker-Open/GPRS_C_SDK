
#include <string.h>
#include <stdio.h>
#include <api_os.h>
#include <api_gps.h>
#include <api_event.h>
#include <api_hal_uart.h>
#include <api_debug.h>
#include "buffer.h"
#include "gps_parse.h"
#include "math.h"
#include "gps.h"
#include "api_hal_pm.h"
#include "time.h"
#include "api_info.h"
#include "assert.h"
#include "api_socket.h"
#include "api_network.h"
#include "api_lbs.h"




/**
 * gps tracker, use an open source tracker server traccar:https://www.traccar.org/
 * the server in the code(`#define SERVER_IP   "ss.neucrack.com"`) may invalid someday, you can download the server and deploy youself
 * How to use: 
 *          compile and download to A9G dev board, open browser, access http://ss.neucrack.com:8082 ,
 *          then register and login, add devices and the number is IMEI e.g. `867959033006999`, finally the position of your device will be show in the map
 *          
 * @attention The code below just a demo, please read and check the code carefully before copy to your project directly(DO NOT copy directly)!!!!!!
 * 
 * 
 */
#define SERVER_IP   "ss.neucrack.com"
#define SERVER_PORT  8082

#define GPS_NMEA_LOG_FILE_PATH "/t/gps_nmea.log"



#define MAIN_TASK_STACK_SIZE    (2048 * 2)
#define MAIN_TASK_PRIORITY      0
#define MAIN_TASK_NAME          "GPS Test Task"

static HANDLE gpsTaskHandle = NULL;
bool isGpsOn = true;
bool networkFlag = false;
HANDLE semGetCellInfo = NULL;

float latitudeLbs  = 0.0;
float longitudeLbs = 0.0;


// const uint8_t nmea[]="$GNGGA,000021.263,2228.7216,N,11345.5625,E,0,0,,153.3,M,-3.3,M,,*4E\r\n$GPGSA,A,1,,,,,,,,,,,,,,,*1E\r\n$BDGSA,A,1,,,,,,,,,,,,,,,*0F\r\n$GPGSV,1,1,00*79\r\n$BDGSV,1,1,00*68\r\n$GNRMC,000021.263,V,2228.7216,N,11345.5625,E,0.000,0.00,060180,,,N*5D\r\n$GNVTG,0.00,T,,M,0.000,N,0.000,K,N*2C\r\n";

void EventDispatch(API_Event_t* pEvent)
{
    static uint8_t lbsCount = 0;
    switch(pEvent->id)
    {
        case API_EVENT_ID_NO_SIMCARD:
            Trace(10,"!!NO SIM CARD%d!!!!",pEvent->param1);
            networkFlag = false;
            break;
        case API_EVENT_ID_NETWORK_REGISTER_SEARCHING:
            Trace(2,"network register searching");
            networkFlag = false;
            break;
        case API_EVENT_ID_NETWORK_REGISTER_DENIED:
            Trace(2,"network register denied");
        case API_EVENT_ID_NETWORK_REGISTER_NO:
            Trace(2,"network register no");
            break;
        case API_EVENT_ID_GPS_UART_RECEIVED:
            // Trace(1,"received GPS data,length:%d, data:%s,flag:%d",pEvent->param1,pEvent->pParam1,flag);
            GPS_Update(pEvent->pParam1,pEvent->param1);
            break;
        case API_EVENT_ID_NETWORK_REGISTERED_HOME:
        case API_EVENT_ID_NETWORK_REGISTERED_ROAMING:
        {
            uint8_t status;
            Trace(2,"network register success");
            bool ret = Network_GetAttachStatus(&status);
            if(!ret)
                Trace(1,"get attach staus fail");
            Trace(1,"attach status:%d",status);
            if(status == 0)
            {
                ret = Network_StartAttach();
                if(!ret)
                {
                    Trace(1,"network attach fail");
                }
            }
            else
            {
                Network_PDP_Context_t context = {
                    .apn        ="cmnet",
                    .userName   = ""    ,
                    .userPasswd = ""
                };
                Network_StartActive(context);
            }
            break;
        }
        case API_EVENT_ID_NETWORK_ATTACHED:
            Trace(2,"network attach success");
            Network_PDP_Context_t context = {
                .apn        ="cmnet",
                .userName   = ""    ,
                .userPasswd = ""
            };
            Network_StartActive(context);
            break;

        case API_EVENT_ID_NETWORK_ACTIVATED:
            Trace(2,"network activate success");
            networkFlag = true;
            break;
        
        case API_EVENT_ID_UART_RECEIVED:
            if(pEvent->param1 == UART1)
            {
                uint8_t data[pEvent->param2+1];
                data[pEvent->param2] = 0;
                memcpy(data,pEvent->pParam1,pEvent->param2);
                Trace(1,"uart received data,length:%d,data:%s",pEvent->param2,data);
                if(strcmp(data,"close") == 0)
                {
                    Trace(1,"close gps");
                    GPS_Close();
                    isGpsOn = false;
                }
                else if(strcmp(data,"open") == 0)
                {
                    Trace(1,"open gps");
                    GPS_Open(NULL);
                    isGpsOn = true;
                }
            }
            break;
        case API_EVENT_ID_NETWORK_CELL_INFO:
        {
            uint8_t number = pEvent->param1;
            Network_Location_t* location = (Network_Location_t*)pEvent->pParam1;
            Trace(2,"network cell infomation,serving cell number:1, neighbor cell number:%d",number-1);
            
            for(int i=0;i<number;++i)
            {
                Trace(2,"cell %d info:%d%d%d,%d%d%d,%d,%d,%d,%d,%d,%d",i,
				location[i].sMcc[0], location[i].sMcc[1], location[i].sMcc[2], 
				location[i].sMnc[0], location[i].sMnc[1], location[i].sMnc[2],
				location[i].sLac, location[i].sCellID, location[i].iBsic,
                location[i].iRxLev, location[i].iRxLevSub, location[i].nArfcn);
            }

            if(!LBS_GetLocation(location,number,15,&longitudeLbs,&latitudeLbs))
                Trace(1,"===LBS get location fail===");
            else
                Trace(1,"===LBS get location success, latitude:%f,longitude:%f===",latitudeLbs,longitudeLbs);
            if((latitudeLbs == 0) &&(longitudeLbs == 0))//not get location from server, try again
            {
                if(++lbsCount>6)
                {
                    lbsCount = 0;
                    Trace(1,"try 6 times to get location from lbs but fail!!");
                    OS_ReleaseSemaphore(semGetCellInfo);
                    break;
                }
                if(!Network_GetCellInfoRequst())
                {
                    Trace(1,"network get cell info fail");
                    OS_ReleaseSemaphore(semGetCellInfo);
                }
                break;
            }
            OS_ReleaseSemaphore(semGetCellInfo);
            lbsCount = 0;
            break;
        }

        default:
            break;
    }
}





//http post with no header
int Http_Post(const char* domain, int port,const char* path,uint8_t* body, uint16_t bodyLen, char* retBuffer, int bufferLen)
{
    uint8_t ip[16];
    bool flag = false;
    uint16_t recvLen = 0;

    //connect server
    memset(ip,0,sizeof(ip));
    if(DNS_GetHostByName2(domain,ip) != 0)
    {
        Trace(2,"get ip error");
        return -1;
    }
    // Trace(2,"get ip success:%s -> %s",domain,ip);
    char* servInetAddr = ip;
    char* temp = OS_Malloc(2048);
    if(!temp)
    {
        Trace(2,"malloc fail");
        return -1;
    }
    snprintf(temp,2048,"POST %s HTTP/1.1\r\nContent-Type: application/x-www-form-urlencoded\r\nConnection: Keep-Alive\r\nHost: %s\r\nContent-Length: %d\r\n\r\n",
                            path,domain,bodyLen);
    char* pData = temp;
    int fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(fd < 0){
        Trace(2,"socket fail");
        OS_Free(temp);
        return -1;
    }
    // Trace(2,"fd:%d",fd);

    struct sockaddr_in sockaddr;
    memset(&sockaddr,0,sizeof(sockaddr));
    sockaddr.sin_family = AF_INET;
    sockaddr.sin_port = htons(port);
    inet_pton(AF_INET,servInetAddr,&sockaddr.sin_addr);

    int ret = connect(fd, (struct sockaddr*)&sockaddr, sizeof(struct sockaddr_in));
    if(ret < 0){
        Trace(2,"socket connect fail");
        OS_Free(temp);
        return -1;
    }
    // Trace(2,"socket connect success");
    Trace(2,"send request:%s",pData);
    ret = send(fd, pData, strlen(pData), 0);
    if(ret < 0){
        Trace(2,"socket send fail");
        OS_Free(temp);
        return -1;
    }
    ret = send(fd, body, bodyLen, 0);
    if(ret < 0){
        Trace(2,"socket send fail");
        OS_Free(temp);
        return -1;
    }
    // Trace(2,"socket send success");

    struct fd_set fds;
    struct timeval timeout={12,0};
    FD_ZERO(&fds);
    FD_SET(fd,&fds);
    while(!flag)
    {
        ret = select(fd+1,&fds,NULL,NULL,&timeout);
        // Trace(2,"select return:%d",ret);
        switch(ret)
        {
            case -1:
                Trace(2,"select error");
                flag = true;
                break;
            case 0:
                Trace(2,"select timeout");
                flag = true;
                break;
            default:
                if(FD_ISSET(fd,&fds))
                {
                    memset(retBuffer,0,bufferLen);
                    ret = recv(fd,retBuffer,bufferLen,0);
                    recvLen += ret;
                    if(ret < 0)
                    {
                        Trace(2,"recv error");
                        flag = true;
                        break;
                    }
                    else if(ret == 0)
                    {
                        Trace(2,"ret == 0");
                        break;
                    }
                    else if(ret < 1352)
                    {
                        GPS_DEBUG_I("recv len:%d,data:%s",recvLen,retBuffer);
                        close(fd);
                        OS_Free(temp);
                        return recvLen;
                    }
                }
                break;
        }
    }
    close(fd);
    OS_Free(temp);
    return -1;
}

uint8_t buffer[1024],buffer2[400];

void gps_testTask(void *pData)
{
    GPS_Info_t* gpsInfo = Gps_GetInfo();
    

    //wait for gprs register complete
    //The process of GPRS registration network may cause the power supply voltage of GPS to drop,
    //which resulting in GPS restart.
    while(!networkFlag)
    {
        Trace(1,"wait for gprs regiter complete");
        OS_Sleep(2000);
    }

    //open GPS hardware(UART2 open either)
    GPS_Init();
    GPS_SaveLog(true,GPS_NMEA_LOG_FILE_PATH);
    // if(!GPS_ClearLog())
    //     Trace(1,"open file error, please check tf card");
    GPS_Open(NULL);

    //wait for gps start up, or gps will not response command
    while(gpsInfo->rmc.latitude.value == 0)
        OS_Sleep(1000);
    

    // set gps nmea output interval as 10s
    for(uint8_t i = 0;i<5;++i)
    {
        bool ret = GPS_SetOutputInterval(10000);
        Trace(1,"set gps ret:%d",ret);
        if(ret)
            break;
        OS_Sleep(1000);
    }
    //get version of gps firmware
    if(!GPS_GetVersion(buffer,150))
        Trace(1,"get gps firmware version fail");
    else
        Trace(1,"gps firmware version:%s",buffer);

    
    //get location through LBS
    semGetCellInfo = OS_CreateSemaphore(0);
    if(!Network_GetCellInfoRequst())
    {
        Trace(1,"network get cell info fail");
    }
    OS_WaitForSemaphore(semGetCellInfo,OS_TIME_OUT_WAIT_FOREVER);
    OS_DeleteSemaphore(semGetCellInfo);
    semGetCellInfo = NULL;

    //send location to GPS and update brdc GPD file
    Trace(1,"do AGPS now");
    if(!GPS_AGPS(latitudeLbs,longitudeLbs,0,true))
    {
        Trace(1,"agps fail");
    }
    else
    {
        Trace(1,"do AGPS success");
    }

    //set nmea output interval as 1s
    if(!GPS_SetOutputInterval(1000))
        Trace(1,"set nmea output interval fail");
    
    Trace(1,"init ok");

    
    while(1)
    {
        if(isGpsOn)
        {
            //show fix info
            uint8_t isFixed = gpsInfo->gsa[0].fix_type > gpsInfo->gsa[1].fix_type ?gpsInfo->gsa[0].fix_type:gpsInfo->gsa[1].fix_type;
            char* isFixedStr = NULL; 
            if(isFixed == 2)
                isFixedStr = "2D fix";
            else if(isFixed == 3)
            {
                if(gpsInfo->gga.fix_quality == 1)
                    isFixedStr = "3D fix";
                else if(gpsInfo->gga.fix_quality == 2)
                    isFixedStr = "3D/DGPS fix";
            }
            else
                isFixedStr = "no fix";

            //convert unit ddmm.mmmm to degree(°) 
            int temp = (int)(gpsInfo->rmc.latitude.value/gpsInfo->rmc.latitude.scale/100);
            double latitude = temp+(double)(gpsInfo->rmc.latitude.value - temp*gpsInfo->rmc.latitude.scale*100)/gpsInfo->rmc.latitude.scale/60.0;
            temp = (int)(gpsInfo->rmc.longitude.value/gpsInfo->rmc.longitude.scale/100);
            double longitude = temp+(double)(gpsInfo->rmc.longitude.value - temp*gpsInfo->rmc.longitude.scale*100)/gpsInfo->rmc.longitude.scale/60.0;

            
            //you can copy ` latitude,longitude ` to http://www.gpsspg.com/maps.htm check location on map

            snprintf(buffer,sizeof(buffer),"GPS fix mode:%d, BDS fix mode:%d, fix quality:%d, satellites tracked:%d, gps sates total:%d, is fixed:%s, coordinate:WGS84, Latitude:%f, Longitude:%f, unit:degree,altitude:%f",gpsInfo->gsa[0].fix_type, gpsInfo->gsa[1].fix_type,
                                                                gpsInfo->gga.fix_quality,gpsInfo->gga.satellites_tracked, gpsInfo->gsv[0].total_sats, isFixedStr, latitude,longitude,gpsInfo->gga.altitude);
            //show in tracer
            Trace(1,buffer);
            
            //send to UART1
            UART_Write(UART1,buffer,strlen(buffer));
            UART_Write(UART1,"\r\n\r\n",4);

            //send to server
            char* requestPath = buffer2;
            uint8_t percent;
            uint16_t v = PM_Voltage(&percent);
            Trace(1,"power:%d %d",v,percent);
            memset(buffer,0,sizeof(buffer));
            if(!INFO_GetIMEI(buffer))
                Assert(false,"NO IMEI");
            Trace(1,"device name:%s",buffer);
            snprintf(requestPath,sizeof(buffer2),"/?id=%s&timestamp=%d&lat=%f&lon=%f&speed=%f&bearing=%.1f&altitude=%f&accuracy=%.1f&batt=%.1f",
                                                    buffer,time(NULL),latitude,longitude,isFixed*1.0,0.0,gpsInfo->gga.altitude,0.0,percent*1.0);
            if(Http_Post(SERVER_IP,SERVER_PORT,requestPath,NULL,0,buffer,sizeof(buffer)) < 0)
                Trace(1,"send location to server fail");
            else
            {
                Trace(1,"send location to server success");
                Trace(1,"response:%s",buffer);
            }
        }

        OS_Sleep(10000);
    }
}


void gps_MainTask(void *pData)
{
    API_Event_t* event=NULL;
    
    TIME_SetIsAutoUpdateRtcTime(true);
    
    //open UART1 to print NMEA infomation
    UART_Config_t config = {
        .baudRate = UART_BAUD_RATE_115200,
        .dataBits = UART_DATA_BITS_8,
        .stopBits = UART_STOP_BITS_1,
        .parity   = UART_PARITY_NONE,
        .rxCallback = NULL,
        .useEvent   = true
    };
    UART_Init(UART1,config);

    //Create UART1 send task and location print task
    OS_CreateTask(gps_testTask,
            NULL, NULL, MAIN_TASK_STACK_SIZE, MAIN_TASK_PRIORITY, 0, 0, MAIN_TASK_NAME);

    //Wait event
    while(1)
    {
        if(OS_WaitEvent(gpsTaskHandle, (void**)&event, OS_TIME_OUT_WAIT_FOREVER))
        {
            EventDispatch(event);
            OS_Free(event->pParam1);
            OS_Free(event->pParam2);
            OS_Free(event);
        }
    }
}


void gps_agps_Main(void)
{
    gpsTaskHandle = OS_CreateTask(gps_MainTask,
        NULL, NULL, MAIN_TASK_STACK_SIZE, MAIN_TASK_PRIORITY, 0, 0, MAIN_TASK_NAME);
    OS_SetUserMainHandle(&gpsTaskHandle);
}

