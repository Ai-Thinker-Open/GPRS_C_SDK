#ifndef ___TIME_H__
#define ___TIME_H__

#include <sdk_init.h>

#define TIME_2000_1970_S    946684800UL

#define gettimeofday                  CSDK_FUNC(gettimeofday)
/**
  * Sync time from GSM/GPRS network when attach success
  * 
  * @attention  Some base station may not publish time, so the NTP is the best time sync way
  * 
  */
#define TIME_SetIsAutoUpdateRtcTime   CSDK_FUNC(TIME_SetIsAutoUpdateRtcTime)
#define TIME_IsAutoUpdateRtcTime      CSDK_FUNC(TIME_IsAutoUpdateRtcTime)
#define TIME_SetRtcTime               CSDK_FUNC(TIME_SetRtcTime)
#define TIME_GetRtcTime               CSDK_FUNC(TIME_GetRtcTime)
#define clock                         CSDK_FUNC(clock)

//return time from 2000-1-1 00:00:00(UTC) unit:s
// uint32_t TIME_GetTime();  
#define TIME_GetTime                              CSDK_FUNC(TIME_GetTime)
//the same as TIME_GetRtcTime but no timezone
// bool TIME_GetSystemTime(TIME_System_t* sysTime);
#define TIME_GetSystemTime                        CSDK_FUNC(TIME_GetSystemTime)
//the same as TIME_GetRtcTime but no timezone
// bool TIME_SetSystemTime(TIME_System_t* sysTime);
#define TIME_SetSystemTime                        CSDK_FUNC(TIME_SetSystemTime)
//get time zone, unit:15minutes,e.g. if value is 2, means +30minutes
// int8_t TIME_GetTimeZone();
#define TIME_GetTimeZone                          CSDK_FUNC(TIME_GetTimeZone)
// bool TIME_SetTimeZone(int8_t timeZone);
#define TIME_SetTimeZone                          CSDK_FUNC(TIME_SetTimeZone)

//get local time (takes the local timezone into account)
// bool TIME_GetLocalTime(TIME_System_t* localTime);
#define TIME_GetLocalTime                         CSDK_FUNC(TIME_GetLocalTime)
//set local time (takes the local timezone into account)
// bool TIME_SetLocalTime(TIME_System_t* localTime);
#define TIME_SetLocalTime                         CSDK_FUNC(TIME_SetLocalTime)

// uint32_t TIME_SystemTime2TimeStamp(TIME_System_t* sysTime);
//return time from 2000-1-1 00:00:00(UTC) unit:s
#define TIME_SystemTime2TimeStamp                 CSDK_FUNC(TIME_SystemTime2TimeStamp)

// bool     TIME_TimeStamp2SystemTime(uint32_t stamp, TIME_System_t* sysTime);
//param stamp: time from 2000-1-1 00:00:00(UTC) unit:s, if unix timestamp, should - TIME_2000_1970_S
#define TIME_TimeStamp2SystemTime                 CSDK_FUNC(TIME_TimeStamp2SystemTime)


///////////////////////////standard/////////////////////////////////
//unix time

// time_t time(time_t* timer);
#define TIME_GetTime2           CSDK_FUNC(time)
#define time(x)                TIME_GetTime2(x)

// time_t timelocal(struct tm* tm);
#define timelocal              CSDK_FUNC(timelocal)
// time_t mktime(struct tm* timer);
#define mktime                 CSDK_FUNC(mktime)

#define timegm(x) mktime(x)

/**
  * 
  * @attention not safe for multi thread!! 
  */  
char *ctime(const time_t *time);

#endif