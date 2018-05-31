
#include <time.h>
#include <api_os.h>
#include <api_debug.h>


#define MAIN_TASK_STACK_SIZE    (1024 * 2)
#define MAIN_TASK_PRIORITY      0 
#define MAIN_TASK_NAME         "Test Task"

static HANDLE timeTaskHandle = NULL;

void TestTime()
{
    clock_t timeStart = clock();
    OS_Sleep(500);
    clock_t timeEnd   = clock();
    Trace(2,"%d,%d %d time interval:%d",(int)timeStart,(int)timeEnd,(int)(timeEnd-timeStart),(int)((timeEnd-timeStart)/CLOCKS_PER_MSEC));
    // Trace(2,"%d,%d %d time interval:%d",timeStart,timeEnd,timeEnd-timeStart,(int)((timeEnd-timeStart)));
}

void TestTime2()
{
    RTC_Time_t time;
    struct timeval tv;

    bool ret = TIME_GetRtcTIme(&time);
    if(time.timeZone>0)
        Trace(2,"return:%d,time:%d-%d-%d %d:%d:%d +%d",ret,time.year,time.month,time.day,time.hour,time.minute,time.second,time.timeZone);
    else
        Trace(2,"return:%d,time:%d-%d-%d %d:%d:%d %d",ret,time.year,time.month,time.day,time.hour,time.minute,time.second,time.timeZone);
    
    int ret2 = gettimeofday(&tv,NULL);
    int sec = tv.tv_sec;
    int msec = tv.tv_usec/1000;
    Trace(2,"return2:%d,time now:%d s,%d ms",ret2,sec,msec);
}


void time_MainTask()
{
    TIME_SetIsAutoUpdateRtcTime(true);

    while(1)
    {
        TestTime();
        TestTime2();
        OS_Sleep(5000);
    }
}

void time_Main(void)
{
    timeTaskHandle = OS_CreateTask(time_MainTask ,
        NULL, NULL, MAIN_TASK_STACK_SIZE, MAIN_TASK_PRIORITY, 0, 0, MAIN_TASK_NAME);
    OS_SetUserMainHandle(&timeTaskHandle);
}

