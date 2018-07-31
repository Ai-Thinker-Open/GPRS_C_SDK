
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




#define MAIN_TASK_STACK_SIZE    (2048 * 2)
#define MAIN_TASK_PRIORITY      0
#define MAIN_TASK_NAME          "Main Test Task"

#define SECOND_TASK_STACK_SIZE    (2048 * 2)
#define SECOND_TASK_PRIORITY      1
#define SECOND_TASK_NAME          "Second Test Task"

static HANDLE mainTaskHandle = NULL;
static HANDLE secondTaskHandle = NULL;
static bool flag = false, flag2 = true;


void EventDispatch(API_Event_t* pEvent)
{
    switch(pEvent->id)
    {
        case API_EVENT_ID_NO_SIMCARD:
            Trace(10,"!!NO SIM CARD%d!!!!",pEvent->param1);
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
        OS_Sleep(10000);
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

void lbs_Main()
{
    mainTaskHandle = OS_CreateTask(MainTask ,
        NULL, NULL, MAIN_TASK_STACK_SIZE, MAIN_TASK_PRIORITY, 0, 0, MAIN_TASK_NAME);
    OS_SetUserMainHandle(&mainTaskHandle);
}


