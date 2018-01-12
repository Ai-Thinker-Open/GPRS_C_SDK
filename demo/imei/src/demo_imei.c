
#include "stdbool.h"
#include "stdint.h"
#include "stdio.h"
#include "string.h"

#include "api_os.h"
#include "api_debug.h"
#include "api_event.h"
#include "api_info.h"



#define MAIN_TASK_STACK_SIZE    (2048 * 2)
#define MAIN_TASK_PRIORITY      0
#define MAIN_TASK_NAME          "IMEI Test Task"

static HANDLE mainTaskHandle = NULL;
static HANDLE imeiTaskHandle = NULL;



void ImeiRead(void* param)
{
    uint8_t imei[16];
    while(1)
    {
        memset(imei,0,sizeof(imei));
        INFO_GetIMEI(imei);
        Trace(1,"%s",imei);
        OS_Sleep(3000);
    }
}


void EventDispatch(API_Event_t* pEvent)
{
    switch(pEvent->id)
    {
        case API_EVENT_ID_NO_SIMCARD:
            Trace(10,"!!NO SIM CARD%d!!!!",pEvent->param1);
            break;

        case API_EVENT_ID_NETWORK_REGISTERED_HOME:
        case API_EVENT_ID_NETWORK_REGISTERED_ROAMING:
            break;
        default:
            break;
    }
}

void Init()
{

}

void ImeiTest(void *pData)
{
    API_Event_t* event=NULL;

    Init();

    imeiTaskHandle = OS_CreateTask(ImeiRead,
        NULL, NULL, MAIN_TASK_STACK_SIZE, MAIN_TASK_PRIORITY, 0, 0, MAIN_TASK_NAME);

    while(1)
    {
        if(OS_WaitEvent(mainTaskHandle, (void*)&event, OS_TIME_OUT_WAIT_FOREVER))
        {
            EventDispatch(event);
            OS_Free(event->pParam1);
            OS_Free(event->pParam2);
            OS_Free(event);
        }
    }
}

void imei_Main()
{
    mainTaskHandle = OS_CreateTask(ImeiTest,
        NULL, NULL, MAIN_TASK_STACK_SIZE, MAIN_TASK_PRIORITY, 0, 0, MAIN_TASK_NAME);
    OS_SetUserMainHandle(&mainTaskHandle);
}


