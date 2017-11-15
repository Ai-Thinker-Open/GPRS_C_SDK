#ifndef ___TIME_H__
#define ___TIME_H__

#include <sdk_init.h>
typedef long         time_t;

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
 }tm;




#define CLOCKS_PER_SEC   (16384.0)
#define CLOCKS_PER_MSEC  (16.384)
// int gettimeofday(timeval_t *tv, timezone_t *tz);
// clock_t clock(void);
#define gettimeofday                                    CSDK_FUNC(gettimeofday)
#define clock                                           CSDK_FUNC(clock)

#endif