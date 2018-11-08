

#include <string.h>
#include <stdio.h>
#include <api_os.h>
#include <api_event.h>
#include <api_network.h>
#include <api_debug.h>
#include <api_socket.h>


#define PDP_CONTEXT_APN       "cmnet"
#define PDP_CONTEXT_USERNAME  ""
#define PDP_CONTEXT_PASSWD    ""


#define DOMAIN_NUMBER 8

#define MAIN_TASK_STACK_SIZE    (2048 * 2)
#define MAIN_TASK_PRIORITY      0
#define MAIN_TASK_NAME          "Socket Test Task"

#define TEST_TASK_STACK_SIZE    (2048 * 2)
#define TEST_TASK_PRIORITY      1
#define TEST_TASK_NAME          "Test Task"

static HANDLE networkTaskHandle = NULL;
static HANDLE testTaskHandle = NULL;

const char domains[DOMAIN_NUMBER][50] = {
                "www.neucrack.com",
                "mqtt.neucrack.com",
                "www.baidu.com",
                "www.ai-thinker.com",
                "iot.eclipse.org",
                "www.github.com",
                "www.qq.com",
                "www.taobao.com"
};


bool AttachActivate()
{
    uint8_t status;
    bool ret = Network_GetAttachStatus(&status);
    if(!ret)
    {
        Trace(2,"get attach staus fail");
        return false;
    }
    Trace(2,"attach status:%d",status);
    if(!status)
    {
        ret = Network_StartAttach();
        if(!ret)
        {
            Trace(2,"network attach fail");
            return false;
        }
    }
    else
    {
        ret = Network_GetActiveStatus(&status);
        if(!ret)
        {
            Trace(2,"get activate staus fail");
            return false;
        }
        Trace(2,"activate status:%d",status);
        if(!status)
        {
            Network_PDP_Context_t context = {
                .apn        = PDP_CONTEXT_APN,
                .userName   = PDP_CONTEXT_USERNAME,
                .userPasswd = PDP_CONTEXT_PASSWD
            };
            Network_StartActive(context);
        }
    }
    return true;
}

static void NetworkEventDispatch(API_Event_t* pEvent)
{
    switch(pEvent->id)
    {
        case API_EVENT_ID_NETWORK_REGISTER_DENIED:
            Trace(2,"network register denied");
            break;

        case API_EVENT_ID_NETWORK_REGISTER_NO:
            Trace(2,"network register no");
            break;
        
        case API_EVENT_ID_NETWORK_REGISTERED_HOME:
        case API_EVENT_ID_NETWORK_REGISTERED_ROAMING:
            Trace(2,"network register success");
            AttachActivate();
            break;

        case API_EVENT_ID_NETWORK_DETACHED:
            Trace(2,"network detached");
            AttachActivate();
            break;
        case API_EVENT_ID_NETWORK_ATTACH_FAILED:
            Trace(2,"network attach failed");
            AttachActivate();
            break;

        case API_EVENT_ID_NETWORK_ATTACHED:
            Trace(2,"network attach success");
            AttachActivate();
            break;

        case API_EVENT_ID_NETWORK_DEACTIVED:
            Trace(2,"network deactived");
            AttachActivate();
            break;

        case API_EVENT_ID_NETWORK_ACTIVATE_FAILED:
            Trace(2,"network activate failed");
            AttachActivate();
            break;

        case API_EVENT_ID_NETWORK_ACTIVATED:
            Trace(2,"network activate success..");
            break;

        case API_EVENT_ID_SIGNAL_QUALITY:
            Trace(2,"CSQ:%d",pEvent->param1);
            break;

        default:
            break;
    }
}

static void EventDispatch(API_Event_t* pEvent)
{
    switch(pEvent->id)
    {    
        case API_EVENT_ID_NO_SIMCARD:
            Trace(2,"!!NO SIM CARD%d!!!!",pEvent->param1);
            break;
        case API_EVENT_ID_SIMCARD_DROP:
            Trace(2,"!!SIM CARD%d DROP!!!!",pEvent->param1);
            break;
        case API_EVENT_ID_SYSTEM_READY:
            Trace(2,"system initialize complete");
            break;
        case API_EVENT_ID_NETWORK_REGISTER_DENIED:
        case API_EVENT_ID_NETWORK_REGISTER_NO:
        case API_EVENT_ID_NETWORK_REGISTERED_HOME:
        case API_EVENT_ID_NETWORK_REGISTERED_ROAMING:
        case API_EVENT_ID_NETWORK_DETACHED:
        case API_EVENT_ID_NETWORK_ATTACH_FAILED:
        case API_EVENT_ID_NETWORK_ATTACHED:
        case API_EVENT_ID_NETWORK_DEACTIVED:
        case API_EVENT_ID_NETWORK_ACTIVATE_FAILED:
        case API_EVENT_ID_NETWORK_ACTIVATED:
        case API_EVENT_ID_SIGNAL_QUALITY:
            NetworkEventDispatch(pEvent);
            break;

        default:
            break;
    }
}

void Test_MainTask(void* param)
{
    uint8_t buffer[16];
    int i = 0;
    
    OS_Sleep(5000);

    while(1)
    {
        memset(buffer,0,sizeof(buffer));
        int ret = DNS_GetHostByName2(domains[i],buffer);
        if(ret != 0)
        {
            Trace(1,"DNS get ip fail");
        }
        else
        {
            Trace(1,"DNS %s get ip success:%s",domains[i],buffer);
        }
        ++i;
        if(i>=DOMAIN_NUMBER)
            i = 0;
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


void network_Main(void)
{
    networkTaskHandle = OS_CreateTask(network_MainTask ,
        NULL, NULL, MAIN_TASK_STACK_SIZE, MAIN_TASK_PRIORITY, 0, 0, MAIN_TASK_NAME);
    OS_SetUserMainHandle(&networkTaskHandle);
}


