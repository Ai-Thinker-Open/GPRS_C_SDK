
#include "stdbool.h"
#include "stdint.h"
#include "stdio.h"
#include "string.h"

#include "api_network.h"
#include "api_os.h"
#include "api_socket.h"
#include "api_debug.h"
#include "api_event.h"
#include "time.h"


/*******************************************************************/
#define RECEIVE_BUFFER_MAX_LENGTH 200
/*******************************************************************/


#define MAIN_TASK_STACK_SIZE    (2048 * 2)
#define MAIN_TASK_PRIORITY      0
#define MAIN_TASK_NAME          "Main Test Task"

#define DNS_TASK_STACK_SIZE    (2048 * 2)
#define DNS_TASK_PRIORITY      1
#define DNS_TASK_NAME          "DNS Test Task"

static HANDLE mainTaskHandle = NULL;
static HANDLE dnsTestHandle = NULL;


bool isNetworkOk = false;
int socketFd = -1;

uint8_t buffer[RECEIVE_BUFFER_MAX_LENGTH];


void EventDispatch(API_Event_t* pEvent)
{
    switch(pEvent->id)
    {
        case API_EVENT_ID_NO_SIMCARD:
            Trace(10,"!!NO SIM CARD%d!!!!",pEvent->param1);
            break;

        case API_EVENT_ID_NETWORK_REGISTERED_HOME:
        case API_EVENT_ID_NETWORK_REGISTERED_ROAMING:
            Trace(1,"network register success");
            Network_StartAttach();
            break;

        case API_EVENT_ID_NETWORK_ATTACHED:
            Trace(1,"network attach success");
            Network_PDP_Context_t context = {
                .apn        ="cmnet",
                .userName   = ""    ,
                .userPasswd = ""
            };
            Network_StartActive(context);
            break;

        case API_EVENT_ID_NETWORK_ACTIVATED:
            Trace(1,"network activate success");
            isNetworkOk = true;
            break;
        
        case API_EVENT_ID_DNS_SUCCESS:
            Trace(1,"DNS get ip address from domain success(event),domain:%s,ip:%s",pEvent->pParam2,pEvent->pParam1);
            break;

        case API_EVENT_ID_DNS_ERROR:
            Trace(1,"DNS get ip address error(event)!!!");
            break;

        default:
            break;
    }
}

uint8_t buffer[200];
uint8_t domains[10][50]={
    "www.baidu.com",
    "www.neucrack.com",
    "neucrack.com",
    "www.jianshu.com",
    "www.w3school.com.cn",
    "www.google.com",
    "www.zhihu.com",
    "iot.eclipse.org",
    "www.cqut.edu.cn",
    "ssl.neucrack.com"
};

void DnsTestCase(void *pData)
{
    int ret = -1;
    uint8_t count = 0;

    while(!isNetworkOk)//wait for network register complete
        OS_Sleep(100);
    
    // API_Socket_Test();
    for(uint8_t i=0;i<10;++i)
    {
        memset(buffer,0,sizeof(buffer));
        clock_t startTime = clock();
        int ret = DNS_GetHostByName2(domains[i],buffer);
        clock_t endTime = clock();
        Trace(1,"DNS len:%d, domain:%s, ip:%s",ret,domains[i],buffer);
        Trace(1,"DNS_GetHostByName2 time last:%dms", (int)((endTime - startTime)/CLOCKS_PER_MSEC ));
    }

    while(1)
    {
        OS_Sleep(1000);
    }
}

void DnsTest(void *pData)
{
    API_Event_t* event=NULL;

    dnsTestHandle =  OS_CreateTask(DnsTestCase,
        NULL, NULL, DNS_TASK_STACK_SIZE, DNS_TASK_PRIORITY, 0, 0, DNS_TASK_NAME);

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

void dns_Main(void)
{
    mainTaskHandle = OS_CreateTask(DnsTest,
        NULL, NULL, MAIN_TASK_STACK_SIZE, MAIN_TASK_PRIORITY, 0, 0, MAIN_TASK_NAME);
    OS_SetUserMainHandle(&mainTaskHandle);
}

