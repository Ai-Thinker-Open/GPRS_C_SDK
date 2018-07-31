#ifndef ___TIME_H__
#define ___TIME_H__

#include <sdk_init.h>

#define gettimeofday                  CSDK_FUNC(gettimeofday)
#define TIME_SetIsAutoUpdateRtcTime   CSDK_FUNC(TIME_SetIsAutoUpdateRtcTime)
#define TIME_IsAutoUpdateRtcTime      CSDK_FUNC(TIME_IsAutoUpdateRtcTime)
#define TIME_SetRtcTime               CSDK_FUNC(TIME_SetRtcTime)
#define TIME_GetRtcTIme               CSDK_FUNC(TIME_GetRtcTIme)
#define clock                         CSDK_FUNC(clock)

//return time unit:s
// uint32_t TIME_GetIime();  
#define TIME_GetIime                              CSDK_FUNC(TIME_GetIime)
//the same as TIME_GetRtcTIme but no timezone
// bool TIME_GetSystemTime(TIME_System_t* sysTime);
#define TIME_GetSystemTime                        CSDK_FUNC(TIME_GetSystemTime)
//the same as TIME_GetRtcTIme but no timezone
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
#define TIME_SystemTime2TimeStamp                 CSDK_FUNC(TIME_SystemTime2TimeStamp)
// bool     TIME_TimeStamp2SystemTime(uint32_t stamp, TIME_System_t* sysTime);
#define TIME_TimeStamp2SystemTime                 CSDK_FUNC(TIME_TimeStamp2SystemTime)

///////standard///
// time_t time(time_t* timer);
#define TIME_GetTime           CSDK_FUNC(time)
#define time(x)                TIME_GetTime(x)

// time_t timelocal(struct tm* tm);
#define timelocal              CSDK_FUNC(timelocal)
// time_t mktime(struct tm* timer);
#define mktime                 CSDK_FUNC(mktime)

#define timegm(x) mktime(x)


#endif