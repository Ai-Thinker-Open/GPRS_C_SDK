
#include "stdbool.h"
#include "stdint.h"
#include "stdio.h"
#include "string.h"

#include "api_os.h"
#include "api_debug.h"
#include "api_event.h"
#include "api_hal_pm.h"
#include "api_key.h"



#define MAIN_TASK_STACK_SIZE    (2048 * 2)
#define MAIN_TASK_PRIORITY      0
#define MAIN_TASK_NAME          "Main Test Task"

#define SECOND_TASK_STACK_SIZE    (2048 * 2)
#define SECOND_TASK_PRIORITY      1
#define SECOND_TASK_NAME          "Second Test Task"

static HANDLE mainTaskHandle = NULL;
static HANDLE secondTaskHandle = NULL;

Power_On_Cause_t powerOnCause = POWER_ON_CAUSE_MAX;

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
            break;

        case API_EVENT_ID_POWER_ON:
            powerOnCause = pEvent->param1;
            break;

        case API_EVENT_ID_KEY_DOWN:
            // Trace(1,"key down, key:0x%02x",pEvent->param1);
            if(pEvent->param1 == KEY_POWER)
            {
                Trace(1,"power key press down now");
            }
            break;
        case API_EVENT_ID_KEY_UP:
            // Trace(1,"key release, key:0x%02x",pEvent->param1);
            if(pEvent->param1 == KEY_POWER)
            {
                Trace(1,"power key release now");
            }
            break;
        default:
            break;
    }
}

void SecondTask(void *pData)
{
    int i = 0;
    uint8_t percent;
    OS_Sleep(3000);
    Trace(1,"Power on ,cause:0x%02x",powerOnCause);
    while(1)
    {
        OS_Sleep(500);
        // uint16_t v = PM_Voltage(&percent);
        // Trace(1,"voltage:%dmV,%d percent last,times count:%d",v,percent,i);
        // ++i;
        // if(i==40)//20s
        // {
        //     Trace(1,"Sleep now");
        //     PM_SleepMode(true);
        // }
        // if(i==80)//40s
        // {
        //     Trace(1,"Exit sleep mode now");
        //     PM_SleepMode(false);
        // }
        // if(i == 200)
        //     PM_Restart();
    }
}

void MainTask(void *pData)
{
    API_Event_t* event=NULL;

    secondTaskHandle = OS_CreateTask(SecondTask,
        NULL, NULL, SECOND_TASK_STACK_SIZE, SECOND_TASK_PRIORITY, 0, 0, SECOND_TASK_NAME);

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

void pm_Main(void)
{
    mainTaskHandle = OS_CreateTask(MainTask,
        NULL, NULL, MAIN_TASK_STACK_SIZE, MAIN_TASK_PRIORITY, 0, 0, MAIN_TASK_NAME);
    OS_SetUserMainHandle(&mainTaskHandle);
}

