#ifndef __API_INC_TIME_H__
#define __API_INC_TIME_H__

#include <cs_types.h>


#ifndef _TIME_T_DEFINED
#define _TIME_T_DEFINED
    typedef long         time_t;
#endif

typedef long         clock_t;

typedef struct tm{
    int tm_sec;         /* 秒，范围从 0 到 59        */
    int tm_min;         /* 分，范围从 0 到 59        */
    int tm_hour;        /* 小时，范围从 0 到 23        */
    int tm_mday;        /* 一月中的第几天，范围从 1 到 31    */
    int tm_mon;         /* 月，范围从 0 到 11        */
    int tm_year;        /* 自 1900 年起的年数        */
    int tm_wday;        /* 一周中的第几天，范围从 0 到 6    */
    int tm_yday;        /* 一年中的第几天，范围从 0 到 365    */
    int tm_isdst;       /* 夏令时                */
 }tm_t;

 typedef struct
{
    uint16_t year;
    uint8_t  month;
    uint8_t  dayOfWeek;
    uint8_t  day;
    uint8_t  hour;
    uint8_t  minute;
    uint8_t  second;
    uint16_t milliseconds;
} TIME_System_t;

typedef struct timeval {
    long long    tv_sec;         /* seconds */
    long         tv_usec;        /* and microseconds!!!but millisecond instead here(实际数据是ms)!! */
}timeval_t;

struct timespec {
    time_t tv_sec;
    long tv_nsec;
};

typedef struct timezone {
    int tz_minuteswest;
    int tz_dsttime;
}timezone_t;

typedef struct{
    uint16_t year;
    uint8_t  month;
    uint8_t  day;
    uint8_t  hour;
    uint8_t  minute;
    uint8_t  second;
    int8_t   timeZone;
    int8_t   timeZoneMinutes;
}RTC_Time_t;

#define CLOCKS_PER_SEC   (16384.0)
#define CLOCKS_PER_MSEC  (16.384)


#endif
