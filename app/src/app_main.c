/*
 * @File  app_main.c
 * @Brief An example of SDK's mini system
 * 
 * @Author: Neucrack 
 * @Date: 2017-11-11 16:45:17 
 * @Last Modified by: Neucrack
 * @Last Modified time: 2017-11-11 18:24:56
 */


#include "stdint.h"
#include "stdbool.h"
#include "api_os.h"
#include "api_event.h"
#include "api_debug.h"
#include "api_hal_pm.h"

#define AppMain_TASK_STACK_SIZE    (2048 * 2)
#define AppMain_TASK_PRIORITY      0
#define OTHERE_TASK_STACK_SIZE     (2048 * 2)
#define OTHERE_TASK_PRIORITY       1
HANDLE mainTaskHandle  = NULL;
HANDLE otherTaskHandle = NULL;


void LoopTask(void *pData)
{
    int count = 0;
    //wait for trace initilize complete
    OS_Sleep(3000);
    while(1)
    {
        Trace(1, "Test Test%d",++count);

        //Use event drive or sleep some time here
        OS_Sleep(2000);
    }
}
void EventDispatch(API_Event_t* pEvent)
{
    switch(pEvent->id)
    {
        case API_EVENT_ID_POWER_ON:
            break;
        case API_EVENT_ID_NO_SIMCARD:
            break;
        case API_EVENT_ID_NETWORK_REGISTERED_HOME:
        case API_EVENT_ID_NETWORK_REGISTERED_ROAMING:
            break;
        default:
            break;
    }
}


void AppMainTask(void *pData)
{
    API_Event_t* event=NULL;

    otherTaskHandle = OS_CreateTask(LoopTask,
                                    NULL, NULL, OTHERE_TASK_STACK_SIZE, OTHERE_TASK_PRIORITY, 0, 0, "ohter Task");
    while(1)
    {
        if(OS_WaitEvent(mainTaskHandle, (void**)&event, OS_TIME_OUT_WAIT_FOREVER))
        {
            PM_SetSysMinFreq(PM_SYS_FREQ_178M);//set back system min frequency to 178M or higher(/lower) value
            EventDispatch(event);
            OS_Free(event->pParam1);
            OS_Free(event->pParam2);
            OS_Free(event);
            PM_SetSysMinFreq(PM_SYS_FREQ_32K);//release system freq to enter sleep mode to save power,
                                              //system remain runable but slower, and close eripheral not using
        }
    }
}
void app_Main(void)
{
    mainTaskHandle = OS_CreateTask(AppMainTask ,
                                   NULL, NULL, OTHERE_TASK_PRIORITY, AppMain_TASK_PRIORITY, 0, 0, "init Task");
    OS_SetUserMainHandle(&mainTaskHandle);
}