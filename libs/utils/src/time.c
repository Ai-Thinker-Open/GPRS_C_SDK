#include "time.h"
#include "stdint.h"
#include "stdbool.h"
#include "stdio.h"

char *ctime(const time_t *time)
{
    TIME_System_t sysTime;
    static char temp[20];

    bool ret = TIME_TimeStamp2SystemTime((uint32_t)*time-TIME_2000_1970_S,&sysTime);
    if(!ret)
        return NULL;
    snprintf(temp,sizeof(temp),"%04d-%02d-%02d %02d:%02d:%02d",
                sysTime.year,sysTime.month,sysTime.day,
                sysTime.hour,sysTime.minute,sysTime.second);
    return temp;
}


