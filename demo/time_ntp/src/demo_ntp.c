

#include <string.h>
#include <stdio.h>
#include <api_os.h>
#include <api_event.h>
#include <api_network.h>
#include <api_debug.h>
#include "ntp.h"


#define NTP_SERVER "cn.ntp.org.cn"



#define MAIN_TASK_STACK_SIZE    (2048 * 2)
#define MAIN_TASK_PRIORITY      0
#define MAIN_TASK_NAME          "Socket Test Task"

#define TEST_TASK_STACK_SIZE    (2048 * 2)
#define TEST_TASK_PRIORITY      1
#define TEST_TASK_NAME          "Test Task"

static HANDLE networkTaskHandle = NULL;
static HANDLE testTaskHandle = NULL;
bool network_flag = false;



void EventDispatch(API_Event_t* pEvent)
{
    switch(pEvent->id)
    {
        case API_EVENT_ID_NO_SIMCARD:
            Trace(10,"!!NO SIM CARD%d!!!!",pEvent->param1);
            break;
        
        case API_EVENT_ID_SIMCARD_DROP:
            Trace(2,"SIM CARD%d DROP",pEvent->param1);
            break;
        
        case API_EVENT_ID_SIGNAL_QUALITY:
            Trace(2,"signal quality:%d",pEvent->param1);
            break;
        

        case API_EVENT_ID_NETWORK_REGISTERED_HOME:
        case API_EVENT_ID_NETWORK_REGISTERED_ROAMING:
        {
            uint8_t status;
            Trace(2,"network register success");
            bool ret = Network_GetAttachStatus(&status);
            if(!ret)
                Trace(1,"get attach staus fail");
            Trace(1,"attach status:%d",status);
            if(status == 0)
            {
                ret = Network_StartAttach();
                if(!ret)
                {
                    Trace(1,"network attach fail");
                }
            }
            else
            {
                Network_PDP_Context_t context = {
                    .apn        ="cmnet",
                    .userName   = ""    ,
                    .userPasswd = ""
                };
                Network_StartActive(context);
            }

            break;
        }
        case API_EVENT_ID_NETWORK_REGISTER_SEARCHING:
            Trace(2,"network register searching");
            break;
        
        case API_EVENT_ID_NETWORK_REGISTER_DENIED:
            Trace(2,"network register denied");
            break;

        case API_EVENT_ID_NETWORK_REGISTER_NO:
            Trace(2,"network register no");
            break;
        
        case API_EVENT_ID_NETWORK_DETACHED:
            Trace(2,"network detached");
            break;
        
        case API_EVENT_ID_NETWORK_ATTACH_FAILED:
            Trace(2,"network attach failed");
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

        case API_EVENT_ID_NETWORK_DEACTIVED:
            Trace(2,"network deactived");
            break;
        
        case API_EVENT_ID_NETWORK_ACTIVATE_FAILED:
            Trace(2,"network activate failed");
            break;
        
        case API_EVENT_ID_NETWORK_ACTIVATED:
            Trace(2,"network activate success");
            network_flag = true;
            break;

        case API_EVENT_ID_NETWORK_GOT_TIME:
            Trace(2,"network got time");
            break;

        default:
            break;
    }
}



void Test_MainTask(void* param)
{
    time_t timeNTP = 0;
    time_t timeNow;
    
    while(!network_flag)
        OS_Sleep(200);

    while(1)
    {
      if(timeNTP <= 0)
      {
        if(NTP_Update(NTP_SERVER,5,&timeNTP,true) != 0)
            timeNTP = 0;
        if( timeNTP > 0)
        {
          Trace(1,"ntp get time success,time:%u",timeNTP);
          Trace(1, "timestamp:%d Time: %s",timeNTP, ctime( ( const time_t* ) &timeNTP ));
        }
          
      }
      timeNow = time(NULL);
      Trace(1,"time rtc now:%s",ctime((const time_t*)&timeNow) );
      OS_Sleep(5000);
    }
}


void network_MainTask(void *pData)
{
    API_Event_t* event=NULL;

    // Network_SetStatusChangedCallback(OnNetworkStatusChanged);
    testTaskHandle = OS_CreateTask(Test_MainTask ,
        NULL, NULL, TEST_TASK_STACK_SIZE, TEST_TASK_PRIORITY, 0, 0, TEST_TASK_NAME);

    while(1)
    {
        if(OS_WaitEvent(networkTaskHandle, (void**)&event, OS_TIME_OUT_WAIT_FOREVER))
        {
            EventDispatch(event);
            OS_Free(event->pParam1);
            OS_Free(event->pParam2);
            OS_Free(event);
        }
    }
}


void time_ntp_Main(void)
{
    networkTaskHandle = OS_CreateTask(network_MainTask ,
        NULL, NULL, MAIN_TASK_STACK_SIZE, MAIN_TASK_PRIORITY, 0, 0, MAIN_TASK_NAME);
    OS_SetUserMainHandle(&networkTaskHandle);
}


