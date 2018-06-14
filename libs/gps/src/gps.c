/*
 * @File  gps.c
 * @Brief 
 * 
 * @Author: Neucrack 
 * @Date: 2018-06-14 18:13:36 
 * @Last Modified by: Neucrack
 * @Last Modified time: 2018-06-14 19:59:14
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
HANDLE semCmdSending = NULL;
uint8_t tmp[GPS_NMEA_FRAME_BUFFER_LENGTH+1];
Buffer_t gpsNmeaBuffer;
uint8_t  gpsDataBuffer[GPS_DATA_BUFFER_MAX_LENGTH];


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
    
    Buffer_Puts(&gpsNmeaBuffer,data,length);
    
    if(semCmdSending == NULL)
    {
        index = Buffer_Query(&gpsNmeaBuffer,"$GNVTG",strlen("$GNVTG"),Buffer_StartPostion(&gpsNmeaBuffer));
        if(index >= 0)
        {
            // Trace(1,"find $GNVTG");
            index = Buffer_Query(&gpsNmeaBuffer,"\r\n",strlen("\r\n"),index);
            if(index >= 0)
            {
                // Trace(1,"find complete GPS frame");
                
                memset(tmp,0,sizeof(tmp));
                uint32_t len = Buffer_Size2(&gpsNmeaBuffer,index)+1;
                // Trace(1,"frame len:%d",len);
                if(!Buffer_Gets(&gpsNmeaBuffer,tmp,len>GPS_NMEA_FRAME_BUFFER_LENGTH?GPS_NMEA_FRAME_BUFFER_LENGTH:len))
                {
                    Trace(1,"get data from buffer fail");
                    return;
                }
                GPS_Parse(tmp);
            }
        }
    }
    else//sending command
    {
        index = Buffer_Query(&gpsNmeaBuffer,"$PGKC001",strlen("$PGKC001"),Buffer_StartPostion(&gpsNmeaBuffer));
        if(index >= 0)
        {
            index2 = Buffer_Query(&gpsNmeaBuffer,"\r\n",strlen("\r\n"),index);
            if(index2 >= 0)
            {
                Trace(LIBS_DEBUG_I,"GPS find ack message");
                memset(tmp,0,sizeof(tmp));
                uint32_t len = Buffer_Size2(&gpsNmeaBuffer,index2)+1;
                if(!Buffer_Gets(&gpsNmeaBuffer,tmp,len>GPS_NMEA_FRAME_BUFFER_LENGTH?GPS_NMEA_FRAME_BUFFER_LENGTH:len))
                {
                    Trace(1,"get data from buffer fail");
                    return;
                }
                OS_ReleaseSemaphore(semCmdSending);
            }
        }
    }
}

/**
 * Add parity tail and send to gps
 */
void Gps_CMDSend(unsigned char* str)
{
    // char a[] = "$GPGGA,085014.955,2839.2050,n,11549.5721,E,104,03.6,76.6,M,-6.2,M,,*4C";
	int i;
    unsigned char result = *(str+1);
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
	GPS_Send(str,strlen(str));
}

void OnCmdAckFail(void* param)
{
    isCmdSendTimeOut = true;
    OS_ReleaseSemaphore(semCmdSending);
}


bool GPS_SetOutputInterval(uint16_t intervalMs)
{
    char temp[GPS_BUFFER_MAX_LENGTH+6];
    snprintf(temp,GPS_BUFFER_MAX_LENGTH,"%s%03d%d,",GPS_CMD_HEADER,GPS_CMD_NMEA_OUTPUT_INTERVAL,intervalMs);
    semCmdSending = OS_CreateSemaphore(0);
    Gps_CMDSend(temp);
    OS_StartCallbackTimer(OS_GetUserMainHandle(),GPS_TIME_OUT_CMD,OnCmdAckFail,NULL);
    OS_WaitForSemaphore(semCmdSending,OS_TIME_OUT_WAIT_FOREVER);
    OS_DeleteSemaphore(semCmdSending);
    if(isCmdSendTimeOut)
    {
        Trace(LIBS_DEBUG_E,"GPS exec command fail");
    }
    else
    {
        OS_StopCallbackTimer(OS_GetUserMainHandle(),OnCmdAckFail,NULL);
        Trace(LIBS_DEBUG_I,"GPS ack:%s",tmp);
        Trace(LIBS_DEBUG_I,"GPS exec command success");
    }
    return true;
}

