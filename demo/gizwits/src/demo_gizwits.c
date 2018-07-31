/*
 * @File  demo_gizwits.c
 * @Brief gizwits usage
 * 
 * @Author: Neucrack 
 * @Date: 2018-06-12 18:04:08 
 * @Last Modified by: Neucrack
 * @Last Modified time: 2018-06-22 19:15:45
 */


#include <string.h>
#include <stdio.h>
#include <api_os.h>
#include <api_event.h>
#include <api_socket.h>
#include <api_network.h>
#include <api_debug.h>
#include <api_gizwits.h>
#include <time.h>
#include <api_info.h>
#include <assert.h>

/*******************************************************************/
/////////////////////////    configuration   ////////////////////////
const char* GIZWITS_CONFIG_FILE_PATH = "gizwits_config.conf";
#define GIZWITS_TRACKER_PK "c2df95705871421d8d4d8cfdb673ec39"
#define GIZWITS_TRACKER_PK_SECRET "a039314fda***********6eb033cb218"
/*******************************************************************/


#define MAIN_TASK_STACK_SIZE    (2048 * 2)
#define MAIN_TASK_PRIORITY      0
#define MAIN_TASK_NAME          "Socket Test Task"

#define TEST_TASK_STACK_SIZE    (2048 * 2)
#define TEST_TASK_PRIORITY      0
#define TEST_TASK_NAME          "Test Task"

static HANDLE socketTaskHandle = NULL;
static HANDLE testTaskHandle = NULL;
static HANDLE semStart = NULL;


void EventDispatch(API_Event_t* pEvent)
{
    switch(pEvent->id)
    {
        case API_EVENT_ID_NO_SIMCARD:
            Trace(10,"!!NO SIM CARD%d!!!!",pEvent->param1);
            break;

        case API_EVENT_ID_NETWORK_REGISTER_SEARCHING:
            Trace(2,"network register searching");
            break;
        
        case API_EVENT_ID_NETWORK_REGISTER_DENIED:
            Trace(2,"network register denied");
            break;
        
        case API_EVENT_ID_NETWORK_REGISTER_NO:
            Trace(2,"network register no");
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
            OS_ReleaseSemaphore(semStart);
            break;

        default:
            break;
    }
}

void OnReceivedOnePacket(Gizwits_t* gizwits, Gizwits_Action_t action,uint8_t* data, int len)
{
    Trace(1,"this is one packet from mqtt, action: %d", action);

	switch(action){
		case GIZWITS_VARIABLE_LENGTH_ACTION_CONTROL:
			Trace(1,"please control your device here, and then report the new status, both app and mqtt");
			// GIZWITS_Send();
			break;
		
		case GIZWITS_VARIABLE_LENGTH_ACTION_READ_STATUS:
			Trace(1,"please update your device status into data buf and send back to mqtt");		
			// GIZWITS_Send();
			break;

		case GIZWITS_VARIABLE_LENGTH_ACTION_TRANS_RECV:
			Trace(1,"this is your raw data from mqtt");
			break;

		case GIZWITS_VARIABLE_LENGTH_ACTION_PUSH_OTA:
			Trace(1,"get your ota file url: %s", gizwits->otaUrl);
            Trace(1,"did:%s",data);
			break;
        default:
            Trace(1,"unkown action:%d",action);
            break;
	}
}


bool GIZWITS_TEST()
{
    Gizwits_Config_t config;
    Gizwits_t gizwits;
    int ret;
    
    GIZWITS_GetConfig(&config,GIZWITS_CONFIG_FILE_PATH);
    memcpy(config.pk,GIZWITS_TRACKER_PK,strlen(GIZWITS_TRACKER_PK));
    memcpy(config.pk_secret,GIZWITS_TRACKER_PK_SECRET,strlen(GIZWITS_TRACKER_PK_SECRET));
    config.alive = 300;
    INFO_GetIMEI(config.imei);

    Trace(1,"imei:%s",config.imei);
    Trace(1,"pk:%s",config.pk);
    Trace(1,"pk_secret:%s",config.pk_secret);
    Trace(1,"did:%s",config.did);
    Trace(1,"alive time:%d",config.alive);

    Assert(strstr(config.pk_secret,"*") == NULL,"!!!!!please config gizwits parameters first!!!!!!!");

    ret = GIZWITS_Connect(&gizwits,&config,GIZWITS_CONFIG_FILE_PATH);
    if(ret<0)
    {
        Trace(1,"connect gizwits fail");
        return false;
    }
    Trace(1,"connect gizwits success");

    time_t ping_timeOld, sendTime;
    ping_timeOld = time(NULL);
    sendTime = ping_timeOld;
    fd_set		readfds;
    struct 		timeval select_timeval;
    int fd;

	while(1){
        //heart beat in 310s
        if(time(NULL) - ping_timeOld > 280)
        {
            if(!GIZWITS_Ping(&gizwits))
                Trace(1,"gizwits ping fail");
            ping_timeOld = time(NULL);
        }

        //send every 20+5 s
        if(time(NULL) - sendTime > 20)
        {
            if(GIZWITS_Send(&gizwits,GIZWITS_VARIABLE_LENGTH_ACTION_READ_STATUS,"123456",6) < 0)
                Trace(1,"gizwits send fail");
            sendTime = time(NULL);
        }

        //receive
        select_timeval.tv_sec = 5;
		select_timeval.tv_usec = 0;
		FD_ZERO(&readfds);

        fd = GIZWITS_GetSocket(&gizwits);
        if(fd > 0)
        {
            FD_SET(fd, &readfds);
        }
        if(select(fd+1, &readfds, NULL, NULL, &select_timeval) <= 0){
			Trace(1,"server: no select");
			continue;
		}
		Trace(1,"server: get one select issue");
		if(FD_ISSET(fd, &readfds)){ 
			Trace(1,"can read");
			ret = GIZWITS_DoReceive(&gizwits,OnReceivedOnePacket);
            Trace(1,"recive return:%d",ret);
		}
	}

}



void Socket_BIO_Test()
{
    //wait for gprs network connection ok
    semStart = OS_CreateSemaphore(0);
    OS_WaitForSemaphore(semStart,OS_TIME_OUT_WAIT_FOREVER);
    OS_DeleteSemaphore(semStart);

    Trace(1,"start connect gizwits now");
    GIZWITS_TEST();
}

void test_MainTask(void* param)
{
    API_Event_t* event=NULL;

    Socket_BIO_Test();
    
    while(1)
    {
        if(OS_WaitEvent(socketTaskHandle, (void**)&event, OS_TIME_OUT_WAIT_FOREVER))
        {
            // EventDispatch(event);
            OS_Free(event->pParam1);
            OS_Free(event->pParam2);
            OS_Free(event);
        }
    }
}


void socket_MainTask(void *pData)
{
    API_Event_t* event=NULL;

    testTaskHandle = OS_CreateTask(test_MainTask,
        NULL, NULL, TEST_TASK_STACK_SIZE, TEST_TASK_PRIORITY, 0, 0, TEST_TASK_NAME);

    while(1)
    {
        if(OS_WaitEvent(socketTaskHandle, (void**)&event, OS_TIME_OUT_WAIT_FOREVER))
        {
            EventDispatch(event);
            OS_Free(event->pParam1);
            OS_Free(event->pParam2);
            OS_Free(event);
        }
    }
}

void gizwits_Main()
{
    socketTaskHandle = OS_CreateTask(socket_MainTask,
        NULL, NULL, MAIN_TASK_STACK_SIZE, MAIN_TASK_PRIORITY, 0, 0, MAIN_TASK_NAME);
    OS_SetUserMainHandle(&socketTaskHandle);
}

