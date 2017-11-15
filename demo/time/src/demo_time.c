
#include <time.h>
#include <api_os.h>


#define MAIN_TASK_STACK_SIZE    (1024 * 2)
#define MAIN_TASK_PRIORITY      0 
#define MAIN_TASK_NAME         "Test Task"

static HANDLE timeTaskHandle = NULL;

void TestTime()
{
    struct timeval tv;
    struct timezone tz; 
    int ret = gettimeofday(&tv,&tz);
    int sec = tv.tv_sec;
    int usec = tv.tv_usec;
    Trace(2,"%d %d %d %d",tv.tv_sec,20,tv.tv_usec,10);
    Trace(2,"%d %d %d %d",5,20,30,10);
    Trace(2,"time now:%d %d %d %d",sec,20,usec,10);
    clock_t timeStart = clock();
    OS_Sleep(500);
    clock_t timeEnd   = clock();
    Trace(2,"%d,%d %d time interval:%d",timeStart,timeEnd,timeEnd-timeStart,(int)((timeEnd-timeStart)/CLOCKS_PER_MSEC));
    // Trace(2,"%d,%d %d time interval:%d",timeStart,timeEnd,timeEnd-timeStart,(int)((timeEnd-timeStart)));
}


void time_MainTask()
{
    
    while(1)
    {
        TestTime();
        OS_Sleep(5000);
    }
}

void time_Main(void)
{
    timeTaskHandle = OS_CreateTask(time_MainTask ,
        NULL, NULL, MAIN_TASK_STACK_SIZE, MAIN_TASK_PRIORITY, 0, 0, MAIN_TASK_NAME);
    OS_SetUserMainHandle(&timeTaskHandle);
}

