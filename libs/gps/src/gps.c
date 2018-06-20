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



#define GPS_CMD_HEADER "$PGKC"
const unsigned char Hex_Str[16]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};


static bool isCmdSendTimeOut = false;
static HANDLE semCmdSending = NULL;
static uint8_t tmp[GPS_NMEA_FRAME_BUFFER_LENGTH+1];
static Buffer_t gpsNmeaBuffer;
static uint8_t  gpsDataBuffer[GPS_DATA_BUFFER_MAX_LENGTH];
static char*  gpsAckMsg = NULL;


void GPS_Init()
{
    //Initialize buffer to cache nmea message
    Buffer_Init(&gpsNmeaBuffer,gpsDataBuffer,GPS_DATA_BUFFER_MAX_LENGTH);
}

void GPS_Send(char* cmd,uint8_t len)
{
    UART_Write(UART2,cmd,len);
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
                GPS_Parse(tmp);
            }
        }
    }
    else//sending command
    {
        index = Buffer_Query(&gpsNmeaBuffer,GPS_CMD_HEADER,strlen(GPS_CMD_HEADER),Buffer_StartPostion(&gpsNmeaBuffer));
        if(index >= 0)
        {
            index2 = Buffer_Query(&gpsNmeaBuffer,"\r\n",strlen("\r\n"),index);
            if(index2 >= 0)
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

bool GPS_IsCMDValid(uint16_t cmd)
{
    if( cmd == GPS_CMD_ACK                  ||
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

GPS_CMD_Ack_t GPS_AckCheck(char* ackStr, GPS_CMD_t cmdSend)
{
    //$PGKC001,32,1*1E
    //$PGKC001,101,3*2D
    Assert(!(ackStr == NULL),"param error");

    uint16_t ackCmd = 0;

    //sscanf();
    char* index = strstr(ackStr,GPS_CMD_HEADER);
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

    GPS_DEBUG_I("ack cmd:%d",ackCmd);
    if(!GPS_IsCMDValid(ackCmd))
        return GPS_CMD_ACK_FAIL;
    if(ackCmd != cmdSend)
        return GPS_CMD_ACK_FAIL;    
    uint8_t result = index2[2]-'0';
    GPS_DEBUG_I("result:%d",result);
    if(!GPS_IsCMDACKValid(result))
        return GPS_CMD_ACK_FAIL;
    return (GPS_CMD_Ack_t)result;
}

/**
 * Add parity tail and send to gps
 */
void GPS_CMDSend(char* str)
{
    // char a[] = "$GPGGA,085014.955,2839.2050,n,11549.5721,E,104,03.6,76.6,M,-6.2,M,,*4C";
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
    GPS_DEBUG_I("gps send cmd:%s,len:%d",str,strlen(str));
    Buffer_Clear(&gpsNmeaBuffer);
	GPS_Send(str,strlen(str));
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
GPS_CMD_t GPS_SendCMDWaitAck(GPS_CMD_t cmdSend, char* cmdStr, char** ackStr, uint16_t timeout)
{

    isCmdSendTimeOut = false;
    semCmdSending = OS_CreateSemaphore(0);
    GPS_CMDSend(cmdStr);
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
        char* ack = strstr(gpsAckMsg,GPS_CMD_HEADER);
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


static bool GPS_SendWaiteNormalAck(GPS_CMD_t cmdSend, char* cmdStr,uint16_t timeout)
{
    char* ackStr = NULL;
    GPS_CMD_t ackCmd;
    GPS_CMD_Ack_t result;
    
    ackCmd = GPS_SendCMDWaitAck(cmdSend,cmdStr,&ackStr,timeout);
    if(ackCmd != GPS_CMD_ACK)
    {
        GPS_DEBUG_I("ack cmd check fail, wish:%d, actual:%d",GPS_CMD_ACK,ackCmd);
        goto fail;
    }
    result = GPS_AckCheck(ackStr,cmdSend);
    if(result != GPS_CMD_ACK_EXEC_SUCCESS)
    {
        GPS_DEBUG_I("ack result:%d",result);
        goto fail;
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
    return GPS_SendWaiteNormalAck(cmdSend,temp,GPS_TIME_OUT_CMD);
}

bool GPS_ClearInfoInFlash()
{
    GPS_CMD_t  cmdSend;
    char temp[GPS_BUFFER_MAX_LENGTH+6];

    cmdSend = GPS_CMD_ERASE_INFO_IN_FLASH;
    snprintf(temp,GPS_BUFFER_MAX_LENGTH,"%s%03d",GPS_CMD_HEADER,cmdSend);
    GPS_DEBUG_I("clear:%s",temp);
    return GPS_SendWaiteNormalAck(cmdSend,temp,5000);
}

bool GPS_SetStandbyMode(GPS_STANDBY_Mode_t mode)
{
    GPS_CMD_t cmdSend;
    char temp[GPS_BUFFER_MAX_LENGTH+6];

    cmdSend = GPS_CMD_STANDBY_MODE;

    snprintf(temp,GPS_BUFFER_MAX_LENGTH,"%s%03d,%d",GPS_CMD_HEADER,cmdSend,mode);
    return GPS_SendWaiteNormalAck(cmdSend,temp,GPS_TIME_OUT_CMD);
}

bool GPS_SetOutputInterval(uint16_t intervalMs)
{
    GPS_CMD_t cmdSend;
    char temp[GPS_BUFFER_MAX_LENGTH+6];

    cmdSend = GPS_CMD_NMEA_OUTPUT_INTERVAL;
    snprintf(temp,GPS_BUFFER_MAX_LENGTH,"%s%03d,%d",GPS_CMD_HEADER,cmdSend,intervalMs);
    return GPS_SendWaiteNormalAck(cmdSend,temp,GPS_TIME_OUT_CMD);
}

bool GPS_SetLpMode(GPS_LP_Mode_t mode)
{
    GPS_CMD_t cmdSend;
    char temp[GPS_BUFFER_MAX_LENGTH+6];

    cmdSend = GPS_CMD_LP_MODE;

    snprintf(temp,GPS_BUFFER_MAX_LENGTH,"%s%03d,%d",GPS_CMD_HEADER,cmdSend,mode);
    return GPS_SendWaiteNormalAck(cmdSend,temp,GPS_TIME_OUT_CMD);
}

bool GPS_SetQzssOutput(bool openOutput)
{
    GPS_CMD_t cmdSend;
    char temp[GPS_BUFFER_MAX_LENGTH+6];

    cmdSend = GPS_CMD_NMEA_OUTPUT_QZSS;

    snprintf(temp,GPS_BUFFER_MAX_LENGTH,"%s%03d,%d",GPS_CMD_HEADER,cmdSend,openOutput);
    return GPS_SendWaiteNormalAck(cmdSend,temp,GPS_TIME_OUT_CMD);
}



bool GPS_SetQzssEnable(bool enable)
{
    GPS_CMD_t cmdSend;
    char temp[GPS_BUFFER_MAX_LENGTH+6];

    cmdSend = GPS_CMD_QZSS;

    snprintf(temp,GPS_BUFFER_MAX_LENGTH,"%s%03d,%d",GPS_CMD_HEADER,cmdSend,enable);
    return GPS_SendWaiteNormalAck(cmdSend,temp,GPS_TIME_OUT_CMD);
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
    return GPS_SendWaiteNormalAck(cmdSend,temp,GPS_TIME_OUT_CMD);  
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
    return GPS_SendWaiteNormalAck(cmdSend,temp,GPS_TIME_OUT_CMD); 
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
    return GPS_SendWaiteNormalAck(cmdSend,temp,GPS_TIME_OUT_CMD);  
}
bool GPS_SetRtcTime(RTC_Time_t* t)
{
    GPS_CMD_t cmdSend;
    char temp[GPS_BUFFER_MAX_LENGTH+6];

    cmdSend = GPS_CMD_SET_RTC_TIME;

    snprintf(temp,GPS_BUFFER_MAX_LENGTH,"%s%03d,%d,%d,%d,%d,%d,%d",GPS_CMD_HEADER,cmdSend,
                    t->year,t->month,t->day,t->hour,t->minute,t->second);
    return GPS_SendWaiteNormalAck(cmdSend,temp,GPS_TIME_OUT_CMD); 
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

    
    ackCmd = GPS_SendCMDWaitAck(cmdSend,temp,&ackStr,GPS_TIME_OUT_CMD);
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
    return GPS_SendWaiteNormalAck(cmdSend,temp,GPS_TIME_OUT_CMD); 
}

bool GPS_SetFixMode(GPS_Fix_Mode_t mode)
{
    GPS_CMD_t cmdSend;
    char temp[GPS_BUFFER_MAX_LENGTH+6];

    cmdSend = GPS_CMD_FIX_MODE;

    snprintf(temp,GPS_BUFFER_MAX_LENGTH,"%s%03d,%d",GPS_CMD_HEADER,cmdSend,mode);
    return GPS_SendWaiteNormalAck(cmdSend,temp,GPS_TIME_OUT_CMD); 
}

