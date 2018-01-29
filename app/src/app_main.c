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

#define AppMain_TASK_STACK_SIZE    (1024 * 2)
#define AppMain_TASK_PRIORITY      1 
HANDLE mainTaskHandle  = NULL;
HANDLE otherTaskHandle = NULL;


void LoopTask(VOID *pData)
{
    uint64_t count = 0;
    while(1)
    {
        ++count;
        if(count == 3000)
        {
            count = 0;
            Trace(1,"Test Test");
            OS_Sleep(1000);
            Trace(1,"Test Test2");
        }
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


void AppMainTask(VOID *pData)
{
    API_Event_t* event=NULL;
    
    otherTaskHandle = OS_CreateTask(LoopTask ,
        NULL, NULL, AppMain_TASK_STACK_SIZE, AppMain_TASK_PRIORITY, 0, 0, "ohter Task");
        
    while(1)
    {
        if(OS_WaitEvent(mainTaskHandle, &event, OS_TIME_OUT_WAIT_FOREVER))
        {
            EventDispatch(event);
            OS_Free(event->pParam1);
            OS_Free(event->pParam2);
            OS_Free(event);
        }
    }
}
void app_Main(void)
{
    mainTaskHandle = OS_CreateTask(AppMainTask ,
        NULL, NULL, AppMain_TASK_STACK_SIZE, AppMain_TASK_PRIORITY, 0, 0, "init Task");
    OS_SetUserMainHandle(&mainTaskHandle);
}