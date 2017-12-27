
#include "stdbool.h"
#include "stdint.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"

#include "api_os.h"
#include "api_debug.h"
#include "api_event.h"
#include "time.h"


#define MAIN_TASK_STACK_SIZE    (2048 * 2)
#define MAIN_TASK_PRIORITY      0
#define MAIN_TASK_NAME          "Socket Test Task"

static HANDLE mainTaskHandle = NULL;
static HANDLE testTaskHandle = NULL;
static HANDLE semStart = NULL;

void StartTimer1(uint32_t interval);
void StartTimer2(uint32_t interval);

void OnTimer1(void* param)
{
    Trace(1,"OnTimer1,time:%dms",(uint32_t)(clock()/CLOCKS_PER_MSEC));
    StartTimer1(2000);
}

void OnTimer2(void* param)
{
    Trace(1,"OnTimer2,time:%dms",(uint32_t)(clock()/CLOCKS_PER_MSEC));
    StartTimer2(3000);
}

void StartTimer1(uint32_t interval)
{
    // here first parameter must be mainTaskHandle, for task TestTest have no EventDispatch
    uint32_t retTime =OS_QueryCallbackTimer(mainTaskHandle,OnTimer1,NULL);
    Trace(1,"timer1 interval0:%d",retTime);
    OS_StartCallbackTimer(mainTaskHandle,interval,OnTimer1,NULL);
    retTime =OS_QueryCallbackTimer(mainTaskHandle,OnTimer1,NULL);
    Trace(1,"timer1 interval1:%d",retTime);
}
void StartTimer2(uint32_t interval)
{
    // here first parameter must be mainTaskHandle, for task TestTest have no EventDispatch
    OS_StartCallbackTimer(mainTaskHandle,interval,OnTimer2,NULL);
    uint32_t retTime =OS_QueryCallbackTimer(mainTaskHandle,OnTimer1,NULL);
    Trace(1,"timer1 interval2:%d",retTime);
}

void EventDispatch(API_Event_t* pEvent)
{
    switch(pEvent->id)
    {
        case API_EVENT_ID_NETWORK_REGISTERED_HOME:
        case API_EVENT_ID_NETWORK_REGISTERED_ROAMING:
            Trace(1,"network register success");
            OS_ReleaseSemaphore(semStart);
            break;
        default:
            break;
    }
}


void TestTest(void* pData)
{
    OS_Heap_Status_t heapStatus;
    semStart = OS_CreateSemaphore(0);
    OS_WaitForSemaphore(semStart,OS_WAIT_FOREVER);
    OS_DeleteSemaphore(semStart);
    Trace(1,"Wait semaphore success!");
    StartTimer1(2000);
    StartTimer2(3000);
    while(1)
    {
        if(OS_GetHeapUsageStatus(&heapStatus))
            Trace(1,"1 heap total:%d, used:%d",heapStatus.totalSize,heapStatus.usedSize);
        else
            Trace(1,"get heap status failed!!");
        uint8_t* p = OS_Malloc(10000);
        if(!p)
            Trace(1,"malloc fail");
        const char* text = "12345679";
        memset(p,0,sizeof(p));
        memcpy(p,text,9);
        Trace(1,"malloc content");
        OS_GetHeapUsageStatus(&heapStatus);
        Trace(1,"2 heap total:%d, used:%d",heapStatus.totalSize,heapStatus.usedSize);
        OS_Free(p);
        OS_GetHeapUsageStatus(&heapStatus);
        Trace(1,"3 heap total:%d, used:%d",heapStatus.totalSize,heapStatus.usedSize);
        OS_Sleep(5000);
    }
}


void OSTest(void* pData)
{
    API_Event_t* event=NULL;

    testTaskHandle = OS_CreateTask(TestTest,
        NULL, NULL, MAIN_TASK_STACK_SIZE, MAIN_TASK_PRIORITY, 0, 0, MAIN_TASK_NAME);
    while(1)
    {
        if(OS_WaitEvent(mainTaskHandle, (void**)&event, OS_TIME_OUT_WAIT_FOREVER))
        {
            EventDispatch(event);
            OS_Free(event->pParam1);
            OS_Free(event->pParam2);
            OS_Free(event);
        }
    }
}

void os_Main(void)
{
    mainTaskHandle = OS_CreateTask(OSTest,
        NULL, NULL, MAIN_TASK_STACK_SIZE, MAIN_TASK_PRIORITY, 0, 0, MAIN_TASK_NAME);
    OS_SetUserMainHandle(&mainTaskHandle);
}




