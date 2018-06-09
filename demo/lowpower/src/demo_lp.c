
#include "stdbool.h"
#include "stdint.h"
#include "stdio.h"
#include "string.h"

#include "api_os.h"
#include "api_debug.h"
#include "api_event.h"
#include "api_hal_pm.h"
#include "api_key.h"
#include "time.h"


#define MAIN_TASK_STACK_SIZE    (2048 * 2)
#define MAIN_TASK_PRIORITY      0
#define MAIN_TASK_NAME          "Main Test Task"

#define SECOND_TASK_STACK_SIZE    (2048 * 2)
#define SECOND_TASK_PRIORITY      1
#define SECOND_TASK_NAME          "Second Test Task"

static HANDLE mainTaskHandle = NULL;


void LoopTest()
{
    uint32_t i = 0;
    volatile uint32_t j = 0;

    clock_t timeStart = clock();
    for(;i<0x00ff0000;++i)
    {
        j = i + 10;
    }
    clock_t timeEnd   = clock();
    Trace(2,"loop test spent time:%d",(int)((timeEnd-timeStart)/CLOCKS_PER_MSEC));
}

void OnTimer(void* param)
{
    static int i=0;

    if(i < 4)
        OS_StartCallbackTimer(mainTaskHandle,20000,OnTimer,NULL);

    if(i == 0)
    {
        Trace(1,"High Performance");
        PM_SetSysMinFreq(PM_SYS_FREQ_312M);
        LoopTest();
    }
    else if(i == 1)
    {
        Trace(1,"Performance 178M");
        PM_SetSysMinFreq(PM_SYS_FREQ_178M);
        LoopTest();
    }
    else if(i == 2)
    {
        Trace(1,"Performance 78M");
        PM_SetSysMinFreq(PM_SYS_FREQ_78M);
        LoopTest();
    }
    else if(i == 3)
    {
        Trace(1,"Performance 13M");
        PM_SetSysMinFreq(PM_SYS_FREQ_13M);
        LoopTest();
    }
    else if( i == 4 )
    {
        Trace(1,"sleep mode");
        PM_SleepMode(true);
        LoopTest();
    }
    ++i;
}



void EventDispatch(API_Event_t* pEvent)
{
    switch(pEvent->id)
    {
        case API_EVENT_ID_NO_SIMCARD:
            Trace(10,"!!NO SIM CARD%d!!!!",pEvent->param1);
            break;

        case API_EVENT_ID_SYSTEM_READY:
            Trace(1,"system initialize complete");
            // PM_Test__();
            break;

        case API_EVENT_ID_NETWORK_REGISTERED_HOME:
        case API_EVENT_ID_NETWORK_REGISTERED_ROAMING:
            Trace(2,"network register success");
            OnTimer(NULL);
            break;

        case API_EVENT_ID_POWER_ON:
            Trace(1,"Power on ,cause:0x%02x",pEvent->param1);
            break;

        case API_EVENT_ID_KEY_DOWN:
            Trace(1,"key down, key:0x%02x",pEvent->param1);
            if(pEvent->param1 == KEY_POWER)
            {
                Trace(1,"power key press down now");
            }
            break;
        case API_EVENT_ID_KEY_UP:
            Trace(1,"key release, key:0x%02x",pEvent->param1);
            if(pEvent->param1 == KEY_POWER)
            {
                Trace(1,"power key release now");
            }
            break;
        default:
            break;
    }
}


void MainTask(void *pData)
{
    API_Event_t* event=NULL;

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

void lowpower_Main()
{
    mainTaskHandle = OS_CreateTask(MainTask,
        NULL, NULL, MAIN_TASK_STACK_SIZE, MAIN_TASK_PRIORITY, 0, 0, MAIN_TASK_NAME);
    OS_SetUserMainHandle(&mainTaskHandle);
}
