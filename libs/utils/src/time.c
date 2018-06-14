#include "time.h"

time_t time(time_t* timer)
{
    return TIME_GetTime(timer);
}

