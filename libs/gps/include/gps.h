/*
 * @File  gps.h
 * @Brief gps operation releated
 * 
 * @Author: Neucrack 
 * @Date: 2018-06-14 18:04:49 
 * @Last Modified by: Neucrack
 * @Last Modified time: 2018-06-15 17:37:26
 */


#ifndef __GPS_H
#define __GPS_H

#include "api_gps.h"

#ifdef __cplusplus
extern "C"{
#endif

/////////////////////////////////////////
////////////////config//////////////////
#define GPS_BUFFER_MAX_LENGTH 35
#define GPS_TIME_OUT_CMD      1500
#define GPS_NMEA_FRAME_BUFFER_LENGTH 1024
#define GPS_DATA_BUFFER_MAX_LENGTH 2048

#define GPS_DEBUG 0

/////////////////////////////////////////

#if (GPS_DEBUG == 1)
#define GPS_DEBUG_I(fmt,...)  \
    do \
    { \
        Trace(LIBS_DEBUG_I,"[GPS][%s][%s][%d]>>"#fmt,__FILE__,__FUNCTION__,__LINE__,##__VA_ARGS__); \
    } while(0)
#else
#define GPS_DEBUG_I(fmt,...)   
#endif

typedef enum{
    GPS_CMD_ACK = 001 ,
    GPS_CMD_NMEA_OUTPUT_INTERVAL = 101,
    GPS_CMD_MAX
}GPS_CMD_t;

typedef enum{
    GPS_CMD_ACK_NOT_SUPPORT = 1,
    GPS_CMD_ACK_EXEC_FAIL   = 2,
    GPS_CMD_ACK_EXEC_SUCCESS= 3,
    GPS_CMD_ACK_MAX
}GPS_CMD_Ack_t;

void GPS_Init();
void GPS_Update(uint8_t* data,uint32_t length);

bool GPS_SetOutputInterval(uint16_t intervalMs);


#ifdef __cplusplus
}
#endif



#endif

