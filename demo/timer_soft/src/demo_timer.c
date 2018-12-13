
#include "stdbool.h"
#include "stdint.h"
#include "stdio.h"
#include "string.h"

#include "api_os.h"
#include "api_debug.h"
#include "api_event.h"
#include "api_network.h"
#include "api_socket.h"
#include "api_lbs.h"
#include "time.h"


#define TIMER_TASK_STACK_SIZE    (1024)
#define TIMER_TASK_PRIORITY      0
#define TIMER_TASK_NAME          "Timer Task"

#define MAIN_TASK_STACK_SIZE    (2048 * 2)
#define MAIN_TASK_PRIORITY      1
#define MAIN_TASK_NAME          "Main Test Task"

#define SECOND_TASK_STACK_SIZE    (2048 * 2)
#define SECOND_TASK_PRIORITY      2
#define SECOND_TASK_NAME          "Second Test Task"



static HANDLE mainTaskHandle = NULL;
static HANDLE secondTaskHandle = NULL;
static HANDLE timerTask = NULL;
static bool flag = false, flag2 = true;


void EventDispatch(API_Event_t* pEvent)
{
    switch(pEvent->id)
    {
        case API_EVENT_ID_NO_SIMCARD:
            Trace(10,"!!NO SIM CARD%d!!!!",pEvent->param1);
            break;

        case API_EVENT_ID_SIGNAL_QUALITY:
            Trace(1,"SQ:%d",pEvent->param1);
            break;

        case API_EVENT_ID_SYSTEM_READY:
            Trace(1,"system initialize complete");
            break;

        case API_EVENT_ID_NETWORK_REGISTERED_HOME:
        case API_EVENT_ID_NETWORK_REGISTERED_ROAMING:
            Trace(2,"network register success");
            Network_StartAttach();
            break;

        case API_EVENT_ID_NETWORK_ATTACHED:
            Trace(2,"network attach success");
            Network_PDP_Context_t context = {
                .apn        ="cmnet",
                .userName   = ""    ,
                .userPasswd = ""
            };
            Network_StartActive(context);
            break;

        case API_EVENT_ID_NETWORK_ACTIVATED:
            Trace(2,"network activate success");
            flag = true;
            break;

        case API_EVENT_ID_NETWORK_CELL_INFO:
        {
            uint8_t number = pEvent->param1;
            Network_Location_t* location = pEvent->pParam1;
            Trace(2,"network cell infomation,serving cell number:1, neighbor cell number:%d",number-1);
            
            for(int i=0;i<number;++i)
            {
                Trace(2,"cell %d info:%d%d%d,%d%d%d,%d,%d,%d,%d,%d,%d",i,
				location[i].sMcc[0], location[i].sMcc[1], location[i].sMcc[2], 
				location[i].sMnc[0], location[i].sMnc[1], location[i].sMnc[2],
				location[i].sLac, location[i].sCellID, location[i].iBsic,
                location[i].iRxLev, location[i].iRxLevSub, location[i].nArfcn);
            }

            float longitude,latitude;
            if(!LBS_GetLocation(location,number,15,&longitude,&latitude))
            {
                Trace(1,"===LBS get location fail===");
            }
            else
            {
                Trace(1,"===LBS get location success,latitude:%d.%d,longitude:%d.%d===",(int)latitude, (int)((latitude-(int)latitude)*100000),
                                                                                 (int)longitude, (int)((longitude-(int)longitude)*100000) );
            }
            flag2 = true;
            break;
        }
        default:
            break;
    }
}


OS_Heap_Status_t heapStatus;


void SecondTask(void *pData)
{
    char ip[16];
    
    int count=0;

    while(!flag)
    {
        Trace(1,"wait for network register");
        OS_Sleep(3000);
    }

    while(1)
    {

        if(!Network_GetCellInfoRequst())
        {
            Trace(1,"network get cell info fail");
        }
        else
        {
            flag2 = false;
        }
        while(!flag2)
        {
            Trace(1,"wait for lbs result");
            OS_Sleep(2000);
        }
        Trace(1,"times count:%d",++count);
        OS_Sleep(5000);
    }
}


void StartTimer1(uint32_t interval);

void OnTimer1(void* param)
{
    Trace(1,"OnTimer1,time:%dms",(uint32_t)(clock()/CLOCKS_PER_MSEC));
    StartTimer1(100);
}

void StartTimer1(uint32_t interval)
{
    OS_StartCallbackTimer(timerTask,interval,OnTimer1,NULL);
}

void TimerTask(void* param)
{
    API_Event_t* event=NULL;
    while(1)
    {
        if(OS_WaitEvent(timerTask, (void**)&event, OS_TIME_OUT_WAIT_FOREVER))
        {
            OS_Free(event->pParam1);
            OS_Free(event->pParam2);
            OS_Free(event);
        }
    }
}

void MainTask(void *pData)
{
    API_Event_t* event=NULL;

    secondTaskHandle = OS_CreateTask(SecondTask,
        NULL, NULL, SECOND_TASK_STACK_SIZE, SECOND_TASK_PRIORITY, 0, 0, SECOND_TASK_NAME);
    
    StartTimer1(100);

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

void timer_soft_Main()
{
    mainTaskHandle = OS_CreateTask(MainTask ,
        NULL, NULL, MAIN_TASK_STACK_SIZE, MAIN_TASK_PRIORITY, 0, 0, MAIN_TASK_NAME);
    OS_SetUserMainHandle(&mainTaskHandle);

    // The timer is not a hardware timer interrupt, just a soft timer, it will generate a event when time up,
    // we need to receive this event by `OS_WaitEvent()`.
    // So we create a task to do nothing but receive timer event in OS_WaitEvent(), 
    // and timer's callback will be called in this function,
    // So Do Not spend too much time in timer callback.
    // And this task has highest priority so we can use block function in other task
    timerTask = OS_CreateTask(TimerTask,
        NULL, NULL, TIMER_TASK_STACK_SIZE, TIMER_TASK_PRIORITY, 0, 0, TIMER_TASK_NAME);

}


