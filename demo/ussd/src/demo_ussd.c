

#include <string.h>
#include <stdio.h>
#include <api_os.h>
#include <api_event.h>
#include <api_network.h>
#include <api_debug.h>
#include <api_ss.h>
#include <api_charset.h>


#define DOMAIN_NUMBER 8

#define MAIN_TASK_STACK_SIZE    (2048 * 2)
#define MAIN_TASK_PRIORITY      0
#define MAIN_TASK_NAME          "Socket Test Task"

#define TEST_TASK_STACK_SIZE    (2048 * 2)
#define TEST_TASK_PRIORITY      1
#define TEST_TASK_NAME          "Test Task"

static HANDLE networkTaskHandle = NULL;
static HANDLE testTaskHandle = NULL;
HANDLE sem = NULL;


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
            Trace(2,"network register success");
            OS_ReleaseSemaphore(sem);
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
            break;

        case API_EVENT_ID_NETWORK_DEACTIVED:
            Trace(2,"network deactived");
            break;
        
        case API_EVENT_ID_NETWORK_ACTIVATE_FAILED:
            Trace(2,"network activate failed");
            break;
        
        case API_EVENT_ID_NETWORK_ACTIVATED:
            Trace(2,"network activate success");
            break;

        case API_EVENT_ID_NETWORK_GOT_TIME:
            Trace(2,"network got time");
            break;
        
        case API_EVENT_ID_USSD_SEND_SUCCESS:
        {
            uint8_t buffer[50];
            Trace(1,"ussd execute success");
            USSD_Type_t* result = (USSD_Type_t*)pEvent->pParam1;
            int len = GSM_8BitTo7Bit(result->usdString,buffer,result->usdStringSize);
            Trace(1,"string:%s,size:%d,option:%d,dcs:%d",buffer,len,result->dcs,result->option);
            break;
        }
        case API_EVENT_ID_USSD_SEND_FAIL:
            Trace(1,"ussd exec fail,error code:%x,%d",pEvent->param1,pEvent->param2);
            break;

        case API_EVENT_ID_USSD_IND:
        {
            uint8_t buffer[50];
            Trace(1,"ussd recieved");
            USSD_Type_t* result = (USSD_Type_t*)pEvent->pParam1;
            int len = GSM_7BitTo8Bit(result->usdString,buffer,result->usdStringSize);
            Trace(1,"string:%s,size:%d,option:%d,dcs:%d",buffer,len,result->dcs,result->option);
            break;
        }
        default:
            break;
    }
}

#define USSD_STRING "*#21#"

void Test_MainTask(void* param)
{
    uint8_t buffer[50];
    USSD_Type_t usd;

    sem = OS_CreateSemaphore(0);
    OS_WaitForSemaphore(sem,OS_TIME_OUT_WAIT_FOREVER);
    
    int encodeLen = GSM_8BitTo7Bit(USSD_STRING,buffer,strlen(USSD_STRING));
    usd.usdString = buffer;
    usd.usdStringSize = encodeLen;
    usd.option = 3;
    usd.dcs = 0x0f;
    uint32_t ret = SS_SendUSSD(usd);
    Trace(1,"ussd ret:0x%x",ret);
    while(1)
    {
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


void ussd_Main()
{
    networkTaskHandle = OS_CreateTask(network_MainTask ,
        NULL, NULL, MAIN_TASK_STACK_SIZE, MAIN_TASK_PRIORITY, 0, 0, MAIN_TASK_NAME);
    OS_SetUserMainHandle(&networkTaskHandle);
}


