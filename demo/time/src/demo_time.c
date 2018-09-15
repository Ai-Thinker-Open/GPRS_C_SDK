
#include <time.h>
#include <api_os.h>
#include <api_debug.h>
#include <stdio.h>
#include <api_event.h>

#define MAIN_TASK_STACK_SIZE    (1024 * 2)
#define MAIN_TASK_PRIORITY      0 
#define MAIN_TASK_NAME         "Test Task"

static HANDLE timeTaskHandle = NULL;
static HANDLE mainTaskHandle = NULL;

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

    bool ret = TIME_GetRtcTime(&time);
    if(time.timeZone>0)
        Trace(2,"return:%d,time:%d-%d-%d %02d:%02d:%02d +%d",ret,time.year,time.month,time.day,time.hour,time.minute,time.second,time.timeZone);
    else
        Trace(2,"return:%d,time:%d-%d-%d %02d:%02d:%02d %d",ret,time.year,time.month,time.day,time.hour,time.minute,time.second,time.timeZone);
    
    int ret2 = gettimeofday(&tv,NULL);
    int sec = tv.tv_sec;
    int msec = tv.tv_usec/1000;
    Trace(2,"return2:%d,time now:%d s,%d ms",ret2,sec,msec);
}


void TestTime3()
{
    uint32_t time = TIME_GetTime();
    Trace(1,"timestamp from 2000-1-1 00:00:00(UTC):%d",time);
    TIME_System_t localTime;
    TIME_GetLocalTime(&localTime);
    printf("localtime:%d-%d-%d %02d:%02d:%02d",localTime.year,localTime.month,localTime.day,localTime.hour,localTime.minute,localTime.second);
    time = time(NULL);
    printf("timestamp UTC:%d",time);
    
    TIME_TimeStamp2SystemTime(0,&localTime);
    printf("timestamp 0:%d-%d-%d %02d:%02d:%02d",localTime.year,localTime.month,localTime.day,localTime.hour,localTime.minute,localTime.second);

}

void time_MainTask()
{
    //sync time from GSM/GPRS network when attach success
    //@attention  (some base station may not publish time, so the NTP is the best time sync way)
    TIME_SetIsAutoUpdateRtcTime(true);

    while(1)
    {
        TestTime();
        TestTime2();
        TestTime3();
        OS_Sleep(500);
    }
}

void MainTask(void* param)
{
    API_Event_t* event;

    timeTaskHandle = OS_CreateTask(time_MainTask ,
        NULL, NULL, MAIN_TASK_STACK_SIZE, MAIN_TASK_PRIORITY+1, 0, 0, "time task");

    while(1)
    {
        if(OS_WaitEvent(mainTaskHandle, (void**)&event, OS_TIME_OUT_WAIT_FOREVER))
        {
            OS_Free(event->pParam1);
            OS_Free(event->pParam2);
            OS_Free(event);
        }
    }
}

void time_Main(void)
{
    mainTaskHandle = OS_CreateTask(MainTask ,
        NULL, NULL, MAIN_TASK_STACK_SIZE, MAIN_TASK_PRIORITY, 0, 0, MAIN_TASK_NAME);
    OS_SetUserMainHandle(&mainTaskHandle);
}

