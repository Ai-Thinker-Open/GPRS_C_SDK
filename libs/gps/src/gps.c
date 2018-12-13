/*
 * @File  gps.c
 * @Brief 
 * 
 * @Author: Neucrack 
 * @Date: 2018-06-14 18:13:36 
 * @Last Modified by: Neucrack
 * @Last Modified time: 2018-06-15 18:56:01
 */

#include "gps.h"
#include "api_hal_uart.h"
#include "assert.h"
#include "buffer.h"
#include "api_debug.h"
#include "gps_parse.h"
#include "api_fs.h"

#include "api_socket.h"
#include "api_os.h"



#define GPS_CMD_HEADER "$PGKC"
const uint8_t GPS_CMD_BINARY_HEADER[2] = {0xaa,0xf0};
const unsigned char Hex_Str[16]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};


static bool isCmdSendTimeOut = false;
static HANDLE semCmdSending = NULL;
static uint8_t tmp[GPS_NMEA_FRAME_BUFFER_LENGTH+1];
static Buffer_t gpsNmeaBuffer;
static uint8_t  gpsDataBuffer[GPS_DATA_BUFFER_MAX_LENGTH];
static char*  gpsAckMsg = NULL;
static bool isSaveLog = false;
static const char* gpsLogPath = NULL;


void GPS_Init()
{
    //Initialize buffer to cache nmea message
    Buffer_Init(&gpsNmeaBuffer,gpsDataBuffer,GPS_DATA_BUFFER_MAX_LENGTH);
}

void GPS_SaveLog( bool save, const char* path)
{
    isSaveLog = save;
    gpsLogPath = path;
}

bool GPS_IsSaveLog()
{
    return isSaveLog;
}

bool GPS_ClearLog()
{
    int32_t fd;
    
    uint8_t *path = (uint8_t*)gpsLogPath;
    fd = API_FS_Open(path, FS_O_RDWR | FS_O_CREAT, 0);
	if ( fd < 0)
	{
        GPS_DEBUG_I("Open file failed:%d",fd);
		return false;
	}
    API_FS_Close(fd);
    return true;
}

void GPS_Send(char* cmd,uint16_t len)
{
    UART_Write(UART2,cmd,len);
}

bool SaveToTFCard(char* str)
{
    int32_t fd;
    int32_t ret;
    uint8_t *path = (uint8_t*)gpsLogPath;

    fd = API_FS_Open(path, FS_O_RDWR|FS_O_APPEND | FS_O_CREAT, 0);
	if ( fd < 0)
	{
        GPS_DEBUG_I("Open file failed:%d",fd);
		return false;
	}
    ret = API_FS_Write(fd, (uint8_t*)str, strlen(str));
    API_FS_Close(fd);
    if(ret <= 0)
        return false;
	return true;
}

void GPS_Update(uint8_t* data,uint32_t length)
{
    int32_t index;
    int32_t index2;
    bool ret = false;
    ret = Buffer_Puts(&gpsNmeaBuffer,data,length);
    if(!ret)
        GPS_DEBUG_I("buffer overflow");
    if(semCmdSending == NULL)
    {
        while(1)
        {
            index = Buffer_Query(&gpsNmeaBuffer,"VTG",strlen("VTG"),Buffer_StartPostion(&gpsNmeaBuffer));
            if(index >= 0)
            {
                // GPS_DEBUG_I("find $GNVTG");
                index = Buffer_Query(&gpsNmeaBuffer,"\r\n",strlen("\r\n"),index);
                if(index >= 0)
                {
                    // GPS_DEBUG_I("find complete GPS frame");
                    
                    memset(tmp,0,sizeof(tmp));
                    uint32_t len = Buffer_Size2(&gpsNmeaBuffer,index)+1;
                    // GPS_DEBUG_I("frame len:%d",len);
                    if(!Buffer_Gets(&gpsNmeaBuffer,tmp,len>GPS_NMEA_FRAME_BUFFER_LENGTH?GPS_NMEA_FRAME_BUFFER_LENGTH:len))
                    {
                        GPS_DEBUG_I("get data from buffer fail");
                        return;
                    }
                    GPS_DEBUG_I("parse nmea frame");
                    if(isSaveLog)
                        SaveToTFCard((char*)tmp);
                    GPS_Parse(tmp);
                }
            }
            else
                break;
        }
    }
    else//sending command
    {
        index = Buffer_Query(&gpsNmeaBuffer,GPS_CMD_HEADER,strlen(GPS_CMD_HEADER),Buffer_StartPostion(&gpsNmeaBuffer));
        if(index >= 0)//NMEA command
        {
            index2 = Buffer_Query(&gpsNmeaBuffer,"\r\n",strlen("\r\n"),index);
            if(index2 >= 0)//find one frame command
            {
                GPS_DEBUG_I("GPS find ack message");
                memset(tmp,0,sizeof(tmp));
                uint32_t len = Buffer_Size2(&gpsNmeaBuffer,index2)+1;
                len = len>GPS_NMEA_FRAME_BUFFER_LENGTH?GPS_NMEA_FRAME_BUFFER_LENGTH:len;
                if(!Buffer_Gets(&gpsNmeaBuffer,tmp,(uint16_t)len))
                {
                    GPS_DEBUG_I("get data from buffer fail");
                    return;
                }
                // GPS_DEBUG_I("tmp:%s,%d,%d,%d",tmp,index,index2,len);
                if(semCmdSending)
                {
                    // Assert(gpsAckMsg == NULL,"already malloced"); //test
                    char* index0 = strstr(tmp,GPS_CMD_HEADER);
                    // Assert(index0 != NULL,"index null"); //test
                    len = len - ((uint32_t)index0 - (uint32_t)tmp);
                    gpsAckMsg = OS_Malloc(len);
                    if(gpsAckMsg)
                    {
                        memcpy(gpsAckMsg,index0,len);
                        OS_ReleaseSemaphore(semCmdSending);
                    }
                }
            }
        }
        else//no NMEA command
        {
            index = Buffer_Query(&gpsNmeaBuffer,(uint8_t*)GPS_CMD_BINARY_HEADER,2,Buffer_StartPostion(&gpsNmeaBuffer));
            if(index >= 0)//find binary command
            {
                index2 = Buffer_Query(&gpsNmeaBuffer,"\r\n",strlen("\r\n"),index);
                if(index2 >= 0)//find one frame command
                {
                    GPS_DEBUG_I("GPS find binary ack message");
                    memset(tmp,0,sizeof(tmp));
                    uint32_t len = Buffer_Size2(&gpsNmeaBuffer,index2)+1;
                    len = len>GPS_NMEA_FRAME_BUFFER_LENGTH?GPS_NMEA_FRAME_BUFFER_LENGTH:len;
                    if(!Buffer_Gets(&gpsNmeaBuffer,tmp,(uint16_t)len))
                    {
                        GPS_DEBUG_I("get data from buffer fail");
                        return;
                    }
                    // GPS_DEBUG_I("tmp:%s,%d,%d,%d",tmp,index,index2,len);
                    if(semCmdSending)
                    {
                        // Assert(gpsAckMsg == NULL,"already malloced"); //test
                        char* index0 = NULL;
                        for(int i=1;i<len;++i)
                        {
                            if(tmp[i] == GPS_CMD_BINARY_HEADER[1] && tmp[i-1]==GPS_CMD_BINARY_HEADER[0])
                            {
                                index0 = tmp+i-1;
                                break;
                            }
                        }
                        // Assert(index0 != NULL,"index null"); //test
                        len = len - ((uint32_t)index0 - (uint32_t)tmp);
                        gpsAckMsg = OS_Malloc(len);
                        if(gpsAckMsg)
                        {
                            memcpy(gpsAckMsg,index0,len);
                            GPS_DEBUG_MEM(gpsAckMsg,len,16);
                            OS_ReleaseSemaphore(semCmdSending);
                        }
                    }
                }
            }
        }
    }
}

/**
 * 
 * @param str: string to check parity, must end with '\0'
 * 
 */
bool GPS_CheckParity(char* str)
{
    // char str[] = "$GPGGA,085014.955,2839.2050,n,11549.5721,E,104,03.6,76.6,M,-6.2,M,,*4C";
    int i;
    if(str == NULL)
        return false;
    if(strstr(str,"*") == NULL)
        return false;

    char result = *(str+1);
    for (i = 2;*(str+i) !='*'; i++)
    {
        result ^= *(str+i);
    }
    if(Hex_Str[result>>4] == str[i+1] && Hex_Str[result&0X0F] == str[i+2])
        return true;
    return false;
}


bool GPS_CheckParityBinary(uint8_t* data)
{
	uint8_t result = data[2];
	uint16_t len = ( (data[2]&0x00ff) | (data[3] << 8 & 0xff00) )- 3;
	for (int i = 3; i < len; ++i)
	{
		result ^= data[i];
	}
	return (result == data[len]);
}

bool GPS_IsCMDValid(uint16_t cmd)
{
    if( cmd == GPS_CMD_ACK                  ||
        cmd == GPS_CMD_ACK_GPD              ||
        cmd == GPS_CMD_REBOOT               ||
        cmd == GPS_CMD_ERASE_INFO_IN_FLASH  ||
        cmd == GPS_CMD_STANDBY_MODE         ||
        cmd == GPS_CMD_NMEA_OUTPUT_INTERVAL ||
        cmd == GPS_CMD_LP_MODE              ||
        cmd == GPS_CMD_NMEA_OUTPUT_QZSS     ||
        cmd == GPS_CMD_QZSS                 ||
        cmd == GPS_CMD_SEARCH_MODE          ||
        cmd == GPS_CMD_FORMAT               ||
        cmd == GPS_CMD_SBAS                 ||
        cmd == GPS_CMD_NMEA_OUTPUT_FREQ     ||
        cmd == GPS_CMD_SET_RTC_TIME         ||
        cmd == GPS_CMD_GET_VERSION          ||
        cmd == GPS_CMD_ACK_VERSION          ||
        cmd == GPS_CMD_GPD_PACK             ||
        cmd == GPS_CMD_SET_LOCATION_TIME    ||
        cmd == GPS_CMD_FIX_MODE             
        )
        return true;
    return false;
}

bool GPS_IsCMDACKValid(uint8_t ack)
{
    if( ack == GPS_CMD_ACK_FAIL         || 
        ack == GPS_CMD_ACK_NOT_SUPPORT  ||
        ack == GPS_CMD_ACK_EXEC_FAIL    || 
        ack == GPS_CMD_ACK_EXEC_SUCCESS
        )
        return true;
    return false;
}

GPS_CMD_t GPS_GetAckCmd(char* str)
{
    Assert(!(str == NULL),"param error");

    char* index = strstr(str,GPS_CMD_HEADER);
    if(!index)
        return GPS_CMD_FAIL;
    uint16_t ackCmd0 = (index[5]-'0')*100+(index[6]-'0')*10+(index[7]-'0');

    if(!GPS_IsCMDValid(ackCmd0))
        return GPS_CMD_FAIL;
    return (GPS_CMD_t)ackCmd0;
}

GPS_CMD_t GPS_GetAckCmdBinary(uint8_t* data)
{
    Assert(!(data == NULL),"param error");

    if(data[0]!=GPS_CMD_BINARY_HEADER[0] || data[1]!=GPS_CMD_BINARY_HEADER[1])
        return GPS_CMD_FAIL;
    uint16_t ackCmd0 = ((data[4]&0x00ff) | (data[5] << 8 & 0xff00) );
    if(!GPS_IsCMDValid(ackCmd0))
        return GPS_CMD_FAIL;
    return (GPS_CMD_t)ackCmd0;
}

GPS_CMD_Ack_t GPS_AckCheck(char* ackStr, GPS_CMD_t cmdSend)
{
    //$PGKC001,32,1*1E
    //$PGKC001,101,3*2D
    Assert(!(ackStr == NULL),"param error");

    uint16_t ackCmd = 0;
    char* index = NULL;
    uint8_t result = 0;

    if((ackStr[0]&0xff) == GPS_CMD_BINARY_HEADER[0] && (ackStr[1]&0xff)  == GPS_CMD_BINARY_HEADER[1])//binary
    {
        ackCmd = ((ackStr[6]&0x00ff) | (ackStr[7] << 8 & 0xff00) );
        result = ackStr[8];
    }
    else
    {
        index = strstr(ackStr,GPS_CMD_HEADER);
        if(!index)
            return GPS_CMD_FAIL;
        index = strstr(index+strlen(GPS_CMD_HEADER),",");
        if(!index)
            return GPS_CMD_FAIL;
        char* index2 = strstr(index+1,",");
        if(!index2)
            return GPS_CMD_FAIL;
        index2 -= 1;
        while(index != index2)
        {
            index++;
            ackCmd = ackCmd*10 + (*index-'0');
        }
        result = index2[2]-'0';
    }


    GPS_DEBUG_I("ack cmd:%d",ackCmd);
    if(!GPS_IsCMDValid(ackCmd))
        return GPS_CMD_ACK_FAIL;
    if(ackCmd != cmdSend)
        return GPS_CMD_ACK_FAIL;    
    GPS_DEBUG_I("result:%d",result);
    if(!GPS_IsCMDACKValid(result))
        return GPS_CMD_ACK_FAIL;
    return (GPS_CMD_Ack_t)result;
}

/**
 * Add parity tail and send to gps
 */
void GPS_CMDSend(char* str,GPS_Format_t format)
{
    if(format == GPS_FORMAT_NMEA)
    {
        int i;
        char result = *(str+1);
        for (i = 2;*(str+i) != 0; i++)
        {
            result ^= *(str+i);
        }
        *(str+i++) = '*';
        *(str+i++) = Hex_Str[result>>4];
        *(str+i++) = Hex_Str[result&0X0F];
        *(str+i++) = '\r';
        *(str+i++) = '\n';
        *(str+i) = 0;
        GPS_DEBUG_I("gps send nmea cmd:%s,len:%d",str,strlen(str));
        Buffer_Clear(&gpsNmeaBuffer);
        GPS_Send(str,strlen(str));
    }
    else
    {
        uint8_t* data = str;
        uint8_t result = data[2];
        uint16_t len = ( (data[2]&0x00ff) | (data[3] << 8 & 0xff00) )- 3;
        for (int i = 3; i < len; ++i)
        {
            result ^= data[i];
        }
        data[len] = result;
        data[len+1] = 0x0D;
        data[len+2] = 0x0A;
        GPS_DEBUG_I("gsp send binary cmd,len:%d",len+3);
        GPS_DEBUG_MEM(data,len+3,16);
        GPS_Send(data,len+3);
    }
}

void OnCmdAckFail(void* param)
{
    isCmdSendTimeOut = true;
    OS_ReleaseSemaphore(semCmdSending);    
    GPS_DEBUG_I("gps ack time out");
}

/**
 * Send command and wait for acknowledgement from gps
 * @return GPS_CMD_t: return GPS_CMD_FAIL if wait ack fail
 *                    return GPS_CMD_* command if get ack success, and the return value is the ack cmd
 * 
 */
GPS_CMD_t GPS_SendCMDWaitAck(GPS_CMD_t cmdSend, char* cmdStr,GPS_Format_t format, char** ackStr, uint16_t timeout)
{

    isCmdSendTimeOut = false;
    semCmdSending = OS_CreateSemaphore(0);
    GPS_CMDSend(cmdStr,format);
    OS_StartCallbackTimer(OS_GetUserMainHandle(),timeout,OnCmdAckFail,NULL);
    OS_WaitForSemaphore(semCmdSending,OS_TIME_OUT_WAIT_FOREVER);
    OS_DeleteSemaphore(semCmdSending);
    semCmdSending = NULL;
    if(isCmdSendTimeOut)
    {
        GPS_DEBUG_I("GPS exec command fail");
        return GPS_CMD_FAIL;
    }
    else
    {
        OS_StopCallbackTimer(OS_GetUserMainHandle(),OnCmdAckFail,NULL);
        char* ack = NULL;
        if(((gpsAckMsg[0]&0xff) == GPS_CMD_BINARY_HEADER[0]) && ((gpsAckMsg[1]&0xff) == GPS_CMD_BINARY_HEADER[1]))
        {//binary ack
            if(!GPS_CheckParityBinary(gpsAckMsg))
            {
                GPS_DEBUG_I("check parity binary fail");
                return GPS_CMD_FAIL;
            }
            *ackStr = gpsAckMsg;
            return GPS_GetAckCmdBinary(gpsAckMsg);
        }
        else
        {
            ack = strstr(gpsAckMsg,GPS_CMD_HEADER);
            //check parity 
            if(!GPS_CheckParity(ack))
            {
                GPS_DEBUG_I("check parity fail");
                return GPS_CMD_FAIL;
            }
            *ackStr = ack;
            GPS_DEBUG_I("ack string:%s",ack);
            return GPS_GetAckCmd(ack);
        }
    }
}


static bool GPS_SendWaiteNormalAck(GPS_CMD_t cmdSend, char* cmdStr, GPS_Format_t format, uint16_t timeout)
{
    char* ackStr = NULL;
    GPS_CMD_t ackCmd;
    GPS_CMD_Ack_t result;
    
    ackCmd = GPS_SendCMDWaitAck(cmdSend,cmdStr,format,&ackStr,timeout);
    if((ackCmd != GPS_CMD_ACK) && (ackCmd != GPS_CMD_ACK_GPD))
    {
        GPS_DEBUG_I("ack cmd check fail, wish:%d, actual:%d",GPS_CMD_ACK,ackCmd);
        goto fail;
    }
    if(ackCmd == GPS_CMD_ACK_GPD)
    {//0xaa,0xf0,0x0c,0x00,0x03,0x00,0x00,0x00,0x01,0x0e,0x0d,0x0a 
        if( (cmdStr[6]!= ackStr[6]) || (cmdStr[7]!= ackStr[7]) || (ackStr[8]!=1))
        {
            GPS_DEBUG_I("ack pack number error");
            goto fail;
        }
    }
    else
    {
        result = GPS_AckCheck(ackStr,cmdSend);
        if(result != GPS_CMD_ACK_EXEC_SUCCESS)
        {
            GPS_DEBUG_I("ack result:%d",result);
            goto fail;
        }
    }
    OS_Free(gpsAckMsg);
    gpsAckMsg = NULL;
    return true;

fail:
    OS_Free(gpsAckMsg);
    gpsAckMsg = NULL;
    return false;
}

bool GPS_Reboot(GPS_Reboot_Mode_t mode)
{
    GPS_CMD_t  cmdSend;
    char temp[GPS_BUFFER_MAX_LENGTH+6];

    cmdSend = GPS_CMD_REBOOT;
    snprintf(temp,GPS_BUFFER_MAX_LENGTH,"%s%03d,%d,%d",GPS_CMD_HEADER,cmdSend,mode,1);
    return GPS_SendWaiteNormalAck(cmdSend,temp,GPS_FORMAT_NMEA,GPS_TIME_OUT_CMD);
}

bool GPS_ClearInfoInFlash()
{
    GPS_CMD_t  cmdSend;
    char temp[GPS_BUFFER_MAX_LENGTH+6];

    cmdSend = GPS_CMD_ERASE_INFO_IN_FLASH;
    snprintf(temp,GPS_BUFFER_MAX_LENGTH,"%s%03d",GPS_CMD_HEADER,cmdSend);
    GPS_DEBUG_I("clear:%s",temp);
    return GPS_SendWaiteNormalAck(cmdSend,temp,GPS_FORMAT_NMEA,5000);
}

bool GPS_SetStandbyMode(GPS_STANDBY_Mode_t mode)
{
    GPS_CMD_t cmdSend;
    char temp[GPS_BUFFER_MAX_LENGTH+6];

    cmdSend = GPS_CMD_STANDBY_MODE;

    snprintf(temp,GPS_BUFFER_MAX_LENGTH,"%s%03d,%d",GPS_CMD_HEADER,cmdSend,mode);
    return GPS_SendWaiteNormalAck(cmdSend,temp,GPS_FORMAT_NMEA,GPS_TIME_OUT_CMD);
}


/**
 * 
 * @param intervalMs: nmea infomation output interval, 200~10000 ms
 */
bool GPS_SetOutputInterval(uint16_t intervalMs)
{
    GPS_CMD_t cmdSend;
    char temp[GPS_BUFFER_MAX_LENGTH+6];

    cmdSend = GPS_CMD_NMEA_OUTPUT_INTERVAL;
    snprintf(temp,GPS_BUFFER_MAX_LENGTH,"%s%03d,%d",GPS_CMD_HEADER,cmdSend,intervalMs);
    return GPS_SendWaiteNormalAck(cmdSend,temp,GPS_FORMAT_NMEA,GPS_TIME_OUT_CMD);
}

bool GPS_SetLpMode(GPS_LP_Mode_t mode)
{
    GPS_CMD_t cmdSend;
    char temp[GPS_BUFFER_MAX_LENGTH+6];

    cmdSend = GPS_CMD_LP_MODE;

    snprintf(temp,GPS_BUFFER_MAX_LENGTH,"%s%03d,%d",GPS_CMD_HEADER,cmdSend,mode);
    return GPS_SendWaiteNormalAck(cmdSend,temp,GPS_FORMAT_NMEA,GPS_TIME_OUT_CMD);
}

bool GPS_SetQzssOutput(bool openOutput)
{
    GPS_CMD_t cmdSend;
    char temp[GPS_BUFFER_MAX_LENGTH+6];

    cmdSend = GPS_CMD_NMEA_OUTPUT_QZSS;

    snprintf(temp,GPS_BUFFER_MAX_LENGTH,"%s%03d,%d",GPS_CMD_HEADER,cmdSend,openOutput);
    return GPS_SendWaiteNormalAck(cmdSend,temp,GPS_FORMAT_NMEA,GPS_TIME_OUT_CMD);
}



bool GPS_SetQzssEnable(bool enable)
{
    GPS_CMD_t cmdSend;
    char temp[GPS_BUFFER_MAX_LENGTH+6];

    cmdSend = GPS_CMD_QZSS;

    snprintf(temp,GPS_BUFFER_MAX_LENGTH,"%s%03d,%d",GPS_CMD_HEADER,cmdSend,enable);
    return GPS_SendWaiteNormalAck(cmdSend,temp,GPS_FORMAT_NMEA,GPS_TIME_OUT_CMD);
}

/**
 * @brief Set search mode, parameters
 * 
 * 
 */
bool GPS_SetSearchMode(bool gps, bool glonass, bool beidou, bool galieo)
{
    GPS_CMD_t cmdSend;
    char temp[GPS_BUFFER_MAX_LENGTH+6];

    cmdSend = GPS_CMD_SEARCH_MODE;

    snprintf(temp,GPS_BUFFER_MAX_LENGTH,"%s%03d,%d,%d,%d,%d",GPS_CMD_HEADER,cmdSend,gps,glonass,beidou,galieo);
    return GPS_SendWaiteNormalAck(cmdSend,temp,GPS_FORMAT_NMEA,GPS_TIME_OUT_CMD);  
}
bool GPS_SetFormat(GPS_Format_t format)
{
    return false;
}
bool GPS_SetSBASEnable(bool enable)
{
    GPS_CMD_t cmdSend;
    char temp[GPS_BUFFER_MAX_LENGTH+6];

    cmdSend = GPS_CMD_SBAS;

    snprintf(temp,GPS_BUFFER_MAX_LENGTH,"%s%03d,%d",GPS_CMD_HEADER,cmdSend,enable);
    return GPS_SendWaiteNormalAck(cmdSend,temp,GPS_FORMAT_NMEA,GPS_TIME_OUT_CMD); 
}
bool GPS_SetNmeaOutputFreq(GPS_NMEA_Output_Freq_t* config)
{
    Assert(config!=NULL,"param GPS_NMEA_Output_Freq_t error");

    GPS_CMD_t cmdSend;
    char temp[GPS_BUFFER_MAX_LENGTH+6];

    cmdSend = GPS_CMD_NMEA_OUTPUT_FREQ;

    snprintf(temp,GPS_BUFFER_MAX_LENGTH,"%s%03d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d",
                    GPS_CMD_HEADER,cmdSend,
                    config->gll,config->rmc,config->vtg,config->gga,config->gsa,config->gsv,
                    config->grs,config->gst,0,0,0,0,0,0,0,0,0,0);
    return GPS_SendWaiteNormalAck(cmdSend,temp,GPS_FORMAT_NMEA,GPS_TIME_OUT_CMD);  
}
bool GPS_SetRtcTime(RTC_Time_t* t)
{
    GPS_CMD_t cmdSend;
    char temp[GPS_BUFFER_MAX_LENGTH+6];

    cmdSend = GPS_CMD_SET_RTC_TIME;

    snprintf(temp,GPS_BUFFER_MAX_LENGTH,"%s%03d,%d,%d,%d,%d,%d,%d",GPS_CMD_HEADER,cmdSend,
                    t->year,t->month,t->day,t->hour,t->minute,t->second);
    return GPS_SendWaiteNormalAck(cmdSend,temp,GPS_FORMAT_NMEA,GPS_TIME_OUT_CMD); 
}
//GOKE9501_1.3_17101100
bool GPS_GetVersion(char* version, uint8_t len)
{
    char* ackStr = NULL;
    GPS_CMD_t ackCmd;
    GPS_CMD_t cmdSend;
    char temp[GPS_BUFFER_MAX_LENGTH+6];

    cmdSend = GPS_CMD_GET_VERSION;

    snprintf(temp,GPS_BUFFER_MAX_LENGTH,"%s%03d",GPS_CMD_HEADER,cmdSend);

    
    ackCmd = GPS_SendCMDWaitAck(cmdSend,temp,GPS_FORMAT_NMEA,&ackStr,GPS_TIME_OUT_CMD);
    if(ackCmd != GPS_CMD_ACK_VERSION)
    {
        GPS_DEBUG_I("ack cmd check fail, wish:%d, actual:%d",GPS_CMD_ACK,ackCmd);
        goto fail;
    }
    char* index = strstr(ackStr,",");
    char* index2 = strstr(index,"*");
    if(index == NULL || index2 == NULL)
        goto fail;
    *index2 = '\0';
    snprintf(version,len,"%s",index);

    OS_Free(gpsAckMsg);
    gpsAckMsg = NULL;
    return true;

fail:
    OS_Free(gpsAckMsg);
    gpsAckMsg = NULL;
    return false;
}
bool GPS_SetLocationTime(float latitude, float longitude, float altitude, RTC_Time_t* t)
{
    GPS_CMD_t cmdSend;
    char temp[GPS_BUFFER_MAX_LENGTH+6];
    char buff1[11],buff2[11],buff3[11];

    cmdSend = GPS_CMD_SET_LOCATION_TIME;

    gcvt(latitude,6,buff1);
    gcvt(longitude,6,buff2);
    gcvt(altitude,6,buff3);

    snprintf(temp,GPS_BUFFER_MAX_LENGTH,"%s%03d,%s,%s,%s,%d,%d,%d,%d,%d,%d",GPS_CMD_HEADER,cmdSend,
                    buff1,buff2,buff3,
                    t->year,t->month,t->day,t->hour,t->minute,t->second);
    return GPS_SendWaiteNormalAck(cmdSend,temp,GPS_FORMAT_NMEA,GPS_TIME_OUT_CMD); 
}

bool GPS_SetFixMode(GPS_Fix_Mode_t mode)
{
    GPS_CMD_t cmdSend;
    char temp[GPS_BUFFER_MAX_LENGTH+6];

    cmdSend = GPS_CMD_FIX_MODE;

    snprintf(temp,GPS_BUFFER_MAX_LENGTH,"%s%03d,%d",GPS_CMD_HEADER,cmdSend,mode);
    return GPS_SendWaiteNormalAck(cmdSend,temp,GPS_FORMAT_NMEA,GPS_TIME_OUT_CMD);
}


//http get with no header
static int Http_Get(const char* domain, int port,const char* path, char* retBuffer, int* bufferLen)
{
    bool flag = false;
    uint16_t recvLen = 0;
    uint8_t ip[16];
    int retBufferLen = *bufferLen;
    //connect server
    memset(ip,0,sizeof(ip));
    if(DNS_GetHostByName2(domain,ip) != 0)
    {
        GPS_DEBUG_I("get ip error");
        return -1;
    }
    GPS_DEBUG_I("get ip success:%s -> %s",domain,ip);
    char* servInetAddr = ip;
    snprintf(retBuffer,retBufferLen,"GET %s HTTP/1.1\r\nHost: %s\r\n\r\n",path,domain);
    char* pData = retBuffer;
    int fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(fd < 0){
        GPS_DEBUG_I("socket fail");
        return -1;
    }
    GPS_DEBUG_I("fd:%d",fd);

    struct sockaddr_in sockaddr;
    memset(&sockaddr,0,sizeof(sockaddr));
    sockaddr.sin_family = AF_INET;
    sockaddr.sin_port = htons(port);
    inet_pton(AF_INET,servInetAddr,&sockaddr.sin_addr);

    int ret = connect(fd, (struct sockaddr*)&sockaddr, sizeof(struct sockaddr_in));
    if(ret < 0){
        GPS_DEBUG_I("socket connect fail");
        close(fd);
        return -1;
    }
    GPS_DEBUG_I("socket connect success");
    GPS_DEBUG_I("send request:%s",pData);
    ret = send(fd, pData, strlen(pData), 0);
    if(ret < 0){
        GPS_DEBUG_I("socket send fail");
        close(fd);
        return -1;
    }
    GPS_DEBUG_I("socket send success");

    struct fd_set fds;
    struct timeval timeout={12,0};
    FD_ZERO(&fds);
    FD_SET(fd,&fds);
    while(!flag)
    {
        ret = select(fd+1,&fds,NULL,NULL,&timeout);
        switch(ret)
        {
            case -1:
                GPS_DEBUG_I("select error");
                flag = true;
                break;
            case 0:
                GPS_DEBUG_I("select timeout");
                flag = true;
                break;
            default:
                if(FD_ISSET(fd,&fds))
                {
                    GPS_DEBUG_I("select return:%d",ret);
                    memset(retBuffer+recvLen,0,retBufferLen-recvLen);
                    ret = recv(fd,retBuffer+recvLen,retBufferLen-recvLen,0);
                    GPS_DEBUG_I("ret:%d",ret);
                    recvLen += ret;
                    if(ret < 0)
                    {
                        GPS_DEBUG_I("recv error");
                        flag = true;
                        break;
                    }
                    else if(ret == 0)
                    {
                        GPS_DEBUG_I("ret == 0");
                        flag = true;
                        break;
                    }
                    else if(ret < 1352)
                    {
                        GPS_DEBUG_I("recv len:%d,data:%s",recvLen,retBuffer);
                        *bufferLen = recvLen;
                        close(fd);
                        return recvLen;
                    }                  
                    
                }
                break;
        }
    }
    close(fd);
    return -1;
}



bool GPS_SetBinaryMode()
{
    GPS_CMD_t  cmdSend;
    char temp[GPS_BUFFER_MAX_LENGTH+6];

    cmdSend = GPS_CMD_FORMAT;
    snprintf(temp,GPS_BUFFER_MAX_LENGTH,"%s%03d,%d,%d",GPS_CMD_HEADER,cmdSend,1,9600);
    return GPS_SendWaiteNormalAck(cmdSend,temp,GPS_FORMAT_NMEA,GPS_TIME_OUT_CMD);
}

bool GPS_SetNMEAMode()
{
    GPS_CMD_t  cmdSend;
    uint8_t temp[GPS_BUFFER_MAX_LENGTH+6];
    uint32_t baudrate = 9600;
    uint16_t len = 14;

    cmdSend = GPS_CMD_FORMAT;
    temp[0] = GPS_CMD_BINARY_HEADER[0];
    temp[1] = GPS_CMD_BINARY_HEADER[1];
    temp[4] = (cmdSend&0xff);
    temp[5] = (cmdSend>>8&0x00ff);
    temp[6] = 0;//nmea mode
    memcpy(temp+7,(uint8_t*)&baudrate,4);//little edian
    temp[2] = len&0xff;
    temp[3] = (len>>8) & 0x00ff;
    return GPS_SendWaiteNormalAck(cmdSend,temp,GPS_FORMAT_BINARY,GPS_TIME_OUT_CMD);
}

//send 512 bytes pack  data, padding 0 if less than 512 bytes
bool GPS_SendGPDPack(uint16_t index, uint8_t* pack)
{
    GPS_CMD_t  cmdSend;
    uint8_t temp[GPS_BUFFER_MAX_LENGTH+6];
    uint16_t len = 523;//512+8+3

    if(index == 0xffff)
    {
        len = 0x000b;
    }

    cmdSend = GPS_CMD_GPD_PACK;
    temp[0] = GPS_CMD_BINARY_HEADER[0];
    temp[1] = GPS_CMD_BINARY_HEADER[1];
    temp[2] = len&0xff;
    temp[3] = (len>>8) & 0x00ff;
    temp[4] = (cmdSend&0xff);
    temp[5] = (cmdSend>>8&0x00ff);
    temp[6] = index&0xff;
    temp[7] = (index>>8) & 0x00ff;
    if(pack)
        memcpy(temp+8,pack,512);
    return GPS_SendWaiteNormalAck(cmdSend,temp,GPS_FORMAT_BINARY,GPS_TIME_OUT_CMD);
}


/**
 * do AGPS process, to accelerate GPS fix( download brdc GPD file and upload to GPS, and set location and time)
 * @param latitude:  latitude got from lbs
 * @param longitude: longitude got from lbs
 * @param altitude:  altitude, you can get from internet or set to zero
 * @param downloadGPD: the GPD file size about 3.4k, file change in every 2 hours, and file is valid in 6 hours,
 *                     you can only update onece in 2 hours or 6 hours to save money
 * @return execute agps proccess success or not
 */
bool GPS_AGPS(float latitude, float longitude, float altitude, bool downloadGPD)
{
    char* buffer = NULL;

    if(downloadGPD)
    {
        ///////////////////////////////////////////////////////////
        //1. get GPD file from server
        int bufferLen = 5120; // if(bufferLen%512 != 0) bufferLen >= gpd_file_len + (512-gpd_file_len%512)
        buffer = (char*)OS_Malloc(bufferLen);
        if(!buffer)
        {
            GPS_DEBUG_I("malloc fail");
            return false;
        }
        memset(buffer,0,bufferLen);
        int ret = Http_Get(GPS_AGPS_GPD_FILE_SERVER,GPS_AGPS_GPD_FILE_SERVER_PORT,
                GPS_AGPS_GPD_FILE_PATH,buffer,&bufferLen);
        if(ret < 0)
        {
            GPS_DEBUG_I("http get fail");
            OS_Free(buffer);
            return false;
        }
        char* indexResult = strstr(buffer,"200 OK");
        if(!indexResult)
        {
            GPS_DEBUG_I("http get response error:%s",buffer);
            OS_Free(buffer);
            return false; 
        }
        char* indexBody = strstr(indexResult,"\r\n\r\n");
        if(!indexBody)
        {
            GPS_DEBUG_I("http get response content error:%s",buffer);
            OS_Free(buffer);
            return false; 
        }
        indexBody+=4;
        uint8_t* gpdData = indexBody;
        uint16_t gpdLen  = ret-(indexBody-buffer);
        GPS_DEBUG_I("GPD file length:%d",gpdLen);
        if(gpdLen%512)//padding 0 
        {
            memset(gpdData+gpdLen,0,512 - gpdLen%512);
            gpdLen = gpdLen + (512 - gpdLen%512);
        }
        GPS_DEBUG_I("GPD file length(with padding 0):%d",gpdLen);
        GPS_DEBUG_MEM(gpdData,gpdLen,16);
        
        ///////////////////////////////////////////////////////////
        //2. set mode to binary mode
        if(!GPS_SetBinaryMode())
        {
            GPS_DEBUG_I("set binary mode fail");
            return false;
        }

        ///////////////////////////////////////////////////////////
        //3. send gpd file to gps chip
        //512 bytes evry time transmission(padding 0 if less than 512 bytes)
        uint16_t i=0;
        uint8_t sendFailTimes = 0;

        for(;;)
        {
            if(i*512 > gpdLen)
            {
                if(!GPS_SendGPDPack(0xffff,NULL))//end
                {
                    if(++sendFailTimes > 3)
                    {
                        GPS_DEBUG_I("send gpd file max retry");
                        if(!GPS_SetNMEAMode())
                            GPS_DEBUG_I("set nmea mode fail");
                        return false;
                    }
                    continue;
                }
                break;
            }
            if(!GPS_SendGPDPack(i,gpdData+i*512))//send fail
            {
                if(++sendFailTimes > 3)
                {
                    GPS_DEBUG_I("send gpd file max retry");
                    if(!GPS_SetNMEAMode())
                        GPS_DEBUG_I("set nmea mode fail");
                    return false;
                }
                continue;   
            }
            sendFailTimes = 0;
            ++i;
        }
        GPS_DEBUG_I("send gpd file to gps success");
        
        ///////////////////////////////////////////////////////////
        //4. set mode to nmea mode
        if(!GPS_SetNMEAMode())
        {
            GPS_DEBUG_I("set nmea mode fail");
            return false;
        }

    }

    ///////////////////////////////////////////////////////////
    //5. set gps rtc time
    RTC_Time_t time;
    TIME_GetRtcTime(&time);
    if(!GPS_SetRtcTime(&time))
        GPS_DEBUG_I("set rtc time fail");
    
    ///////////////////////////////////////////////////////////
    //6. send location and date time to gps
    TIME_GetRtcTime(&time);
    if(!GPS_SetLocationTime(latitude,longitude,altitude,&time))
        GPS_DEBUG_I("set location time fail");

    if(downloadGPD)
        OS_Free(buffer);
    return true;
}



