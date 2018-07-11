/*
 * @File  gps.h
 * @Brief gps operation releated
 * 
 * @Author: Neucrack 
 * @Date: 2018-06-14 18:04:49 
 * @Last Modified by: Neucrack
 * @Last Modified time: 2018-06-15 18:48:24
 */


#ifndef __GPS_H
#define __GPS_H

#include "api_gps.h"

#ifdef __cplusplus
extern "C"{
#endif

/////////////////////////////////////////
////////////////config//////////////////
#define GPS_BUFFER_MAX_LENGTH 530  //max:agps gpd file pack need 512+8+3=523(Bytes)
#define GPS_TIME_OUT_CMD      1500
#define GPS_NMEA_FRAME_BUFFER_LENGTH 1024
#define GPS_DATA_BUFFER_MAX_LENGTH 2048

#define GPS_DEBUG 0

#define GPS_AGPS_GPD_FILE_SERVER       "www.goke-agps.com"
#define GPS_AGPS_GPD_FILE_SERVER_PORT   7777
#define GPS_AGPS_GPD_FILE_PATH         "/brdcGPD.dat"

/////////////////////////////////////////

#if (GPS_DEBUG == 1)
#define GPS_DEBUG_I(fmt,...)  \
    do \
    { \
        Trace(LIBS_DEBUG_I,"[GPS][%s][%s][%d]>>"#fmt,__FILE__,__FUNCTION__,__LINE__,##__VA_ARGS__); \
    } while(0)

#define GPS_DEBUG_MEM(a,b,c) \
    do \
    { \
        Trace(LIBS_DEBUG_I,"[GPS][%s][%s][%d]>>Trace Memory",__FILE__,__FUNCTION__,__LINE__); \
        Trace_MemBlock(LIBS_DEBUG_I,a,b,c); \
    } while(0)
#else
#define GPS_DEBUG_I(fmt,...)   
#define GPS_DEBUG_MEM(a,b,c)  
#endif

typedef enum{
    GPS_CMD_FAIL                 = 0  ,
    GPS_CMD_ACK                  = 1,
    GPS_CMD_ACK_GPD              = 3,
    GPS_CMD_REBOOT               = 30,
    GPS_CMD_ERASE_INFO_IN_FLASH  = 40,
    GPS_CMD_STANDBY_MODE         = 51,
    GPS_CMD_NMEA_OUTPUT_INTERVAL = 101,
    GPS_CMD_LP_MODE              = 105,
    GPS_CMD_NMEA_OUTPUT_QZSS     = 113,
    GPS_CMD_QZSS                 = 114,
    GPS_CMD_SEARCH_MODE          = 115,
    GPS_CMD_FORMAT               = 149,
    GPS_CMD_SBAS                 = 239,
    GPS_CMD_NMEA_OUTPUT_FREQ     = 242,
    GPS_CMD_SET_RTC_TIME         = 278,
    GPS_CMD_GET_VERSION          = 462,
    GPS_CMD_ACK_VERSION          = 463,
    GPS_CMD_GPD_PACK             = 614,
    GPS_CMD_SET_LOCATION_TIME    = 639,
    GPS_CMD_FIX_MODE             = 786,
    GPS_CMD_MAX
}GPS_CMD_t;

typedef enum{
    GPS_CMD_ACK_FAIL        = 0,
    GPS_CMD_ACK_NOT_SUPPORT = 1, //gps not support the command
    GPS_CMD_ACK_EXEC_FAIL   = 2, //gps execute fail, maybe the command error
    GPS_CMD_ACK_EXEC_SUCCESS= 3, //gps execute success
    GPS_CMD_ACK_MAX
}GPS_CMD_Ack_t;

typedef enum{
    GPS_REBOOT_MODE_HOT  = 1,
    GPS_REBOOT_MODE_WARM = 2,
    GPS_REBOOT_MODE_COLD = 3,
    GPS_REBOOT_MODE_MAX
}GPS_Reboot_Mode_t;

typedef enum{
    GPS_STANDBY_MODE_STOP  = 0,
    GPS_STANDBY_MODE_SLEEP = 1,
    GPS_STANDBY_MODE_MAX
}GPS_STANDBY_Mode_t;            // standby mode

typedef enum{
    GPS_LP_MODE_NORMAL         = 0,
    GPS_LP_MODE_LP             = 8,
    GPS_LP_MODE_SUPPER_LP      = 9,
    GPS_LP_MODE_MAX
}GPS_LP_Mode_t;                 // low power mode

typedef enum{
    GPS_FORMAT_NMEA = 0,
    GPS_FORMAT_BINARY = 1,
    GPS_FORMAT_MAX
}GPS_Format_t;

typedef struct{
    uint8_t gll;
    uint8_t rmc;
    uint8_t vtg;
    uint8_t gga;
    uint8_t gsa;
    uint8_t gsv;
    uint8_t grs;
    uint8_t gst;
}GPS_NMEA_Output_Freq_t;

typedef enum{
    GPS_FIX_MODE_NORMAL = 0,
    GPS_FIX_MODE_LOW_SPEED  = 1,
    GPS_FIX_MODE_HIGH_SPEED = 2,
    GPS_FIX_MODE_ELEVATION  = 3,
    GPS_FIX_MODE_MAX
}GPS_Fix_Mode_t;


void GPS_Init();
/*
bool GPS_Open(UART_Callback_t gpsReceivedCallback);
bool GPS_Close(void);
bool GPS_IsOpen()
*/
void GPS_Update(uint8_t* data,uint32_t length);
bool GPS_Reboot(GPS_Reboot_Mode_t mode);
bool GPS_ClearInfoInFlash();
bool GPS_SetStandbyMode(GPS_STANDBY_Mode_t mode);
/**
 * 
 * @param intervalMs: nmea infomation output interval, 200~10000 ms
 */
bool GPS_SetOutputInterval(uint16_t intervalMs);
bool GPS_SetLpMode(GPS_LP_Mode_t mode);
bool GPS_SetQzssOutput(bool openOutput);
bool GPS_SetQzssEnable(bool enable);
bool GPS_SetSearchMode(bool gps, bool glonass, bool beidou, bool galieo);
bool GPS_SetFormat(GPS_Format_t format);
bool GPS_SetSBASEnable(bool enable);
bool GPS_SetNmeaOutputFreq(GPS_NMEA_Output_Freq_t* config);
bool GPS_SetRtcTime(RTC_Time_t* time);
//GOKE9501_1.3_17101100
bool GPS_GetVersion(char* version, uint8_t len);
/**
 * 
 * @param latitude: unit: degree(°)
 */
bool GPS_SetLocationTime(float latitude, float longitude, float altitude, RTC_Time_t* time);
bool GPS_SetFixMode(GPS_Fix_Mode_t mode);

bool GPS_SetBinaryMode();
bool GPS_SetNMEAMode();

void GPS_SaveLog(bool save, const char* logPath);
bool GPS_IsSaveLog();
bool GPS_ClearLog();

/**
 * do AGPS process, to accelerate GPS fix( download brdc GPD file and upload to GPS, and set location and time)
 * @param latitude:  latitude got from lbs
 * @param longitude: longitude got from lbs
 * @param altitude:  altitude, you can get from internet or set to zero
 * @param downloadGPD: the GPD file size about 3.4k, file change in every 2 hours, and file is valid in 6 hours,
 *                     you can only update onece in 2 hours or 6 hours to save money
 * @return execute agps proccess success or not
 */
bool GPS_AGPS(float latitude, float longitude, float altitude, bool downloadGPD);

#ifdef __cplusplus
}
#endif



#endif

