

#include <string.h>
#include <stdio.h>
#include <api_os.h>
#include <api_event.h>
#include <api_socket.h>
#include <api_network.h>
#include <api_debug.h>


/*******************************************************************/
/////////////////////////socket configuration////////////////////////

#define DNS_DOMAIN  "120.77.1.207"
#define SERVER_PORT 9530
#define RECEIVE_BUFFER_MAX_LENGTH 200
/*******************************************************************/


#define MAIN_TASK_STACK_SIZE    (2048 * 2)
#define MAIN_TASK_PRIORITY      0
#define MAIN_TASK_NAME          "Udp Test Main Task"

#define TEST_TASK_STACK_SIZE    (2048 * 2)
#define TEST_TASK_PRIORITY      1
#define TEST_TASK_NAME          "Udp Test Task"

static HANDLE socketTaskHandle = NULL;

int socketFd = -1;
uint8_t buffer[RECEIVE_BUFFER_MAX_LENGTH];
HANDLE sem = NULL;
int errorCode = 0;

void EventDispatch(API_Event_t* pEvent)
{
    switch(pEvent->id)
    {
        case API_EVENT_ID_NO_SIMCARD:
            Trace(10,"!!NO SIM CARD%d!!!!",pEvent->param1);
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
            // if(sem)
            //     OS_ReleaseSemaphore(sem);
            sem = 1;
            break;

        case API_EVENT_ID_SOCKET_SENT:
            // if(sem)
            //     OS_ReleaseSemaphore(sem);
            sem = 1;
            break;
        case API_EVENT_ID_SOCKET_RECEIVED:
        {
            int fd = pEvent->param1;
            int length = pEvent->param2>RECEIVE_BUFFER_MAX_LENGTH?RECEIVE_BUFFER_MAX_LENGTH:pEvent->param2;
            memset(buffer,0,sizeof(buffer));
            length = Socket_TcpipRead(fd,buffer,length);
            Trace(2,"udp %d received %d bytes data:%s",fd,length,buffer);
            break;
        }
        case API_EVENT_ID_SOCKET_CLOSED:
        {
            int fd = pEvent->param1;
            Trace(2,"udp %d closed",fd);
            // if(sem)
            //     OS_ReleaseSemaphore(sem);
            sem = 1;
            break;
        }
        case API_EVENT_ID_SOCKET_ERROR:
        {
            int fd = pEvent->param1;
            Trace(2,"socket %d error occurred,cause:%d",fd,pEvent->param2);
            errorCode = pEvent->param2;
            // if(sem)
            //     OS_ReleaseSemaphore(sem);
            sem = 1;
            break;
        }
        default:
            break;
    }
}

void CreateSem(HANDLE* sem_)
{
    *sem_ = 0;
}

void WaitSem(HANDLE* sem_)
{
    // *sem = OS_CreateSemaphore(0);
    // OS_WaitForSemaphore(*sem,OS_WAIT_FOREVER);
    // OS_DeleteSemaphore(*sem);
    // *sem = NULL;
    while(*sem_ == 0)
        OS_Sleep(1);
    *sem_ = 0;
}

bool Connect()
{
    memset(buffer,0,sizeof(buffer));
    if(DNS_GetHostByName2(DNS_DOMAIN,(char*)buffer) != 0)
        return false;
    Trace(2,"DNS,domain:%s,ip:%s,strlen(ip):%d",DNS_DOMAIN,buffer,strlen(buffer));
    socketFd = Socket_TcpipConnect(UDP,buffer,SERVER_PORT);

    if(errorCode != 0)
    {
        errorCode = 0;
        Trace(2,"error ocurred");
        return false;
    }
    return true;
}

bool Write(uint8_t* data, uint16_t len)
{
    Trace(2,"Write");
    CreateSem(&sem);
    int ret = Socket_TcpipWrite(socketFd,data,len);
    if(ret < 0)
    {
        Trace(2,"udp write fail:%d",ret);
        return false;
    }    
    Trace(2,"### udp %d send %d bytes data to server:%s,ret:%d",socketFd, len, data,ret);
    if(errorCode != 0)
    {
        errorCode = 0;
        Trace(2,"error ocurred");
        return false;
    }
    return true;
}

bool Close()
{
    CreateSem(&sem);
    Socket_TcpipClose(socketFd);
    WaitSem(&sem);
    return true;
}

void socketTestTask(void* param)
{
    int failCount = 0;
    int count = 0;
    WaitSem(&sem);
    Trace(2,"sem:%d,%p",(int)sem,(void*)sem);
    Connect();
    while(1)
    {
        if(failCount == 5)
        {
            Close();
        }
        if(failCount >= 5)
        {
            if(Connect())
                failCount = 0;
            else
                ++failCount;
        }
        else
        {
            if(!Write("sdfgss",strlen("sdfgss")))
            {
                ++failCount;
                Trace(2,"write fail");
            }
        }
        
        OS_Sleep(5000);
    }
}


void udp_MainTask(void *pData)
{
    API_Event_t* event=NULL;

    CreateSem(&sem);
    OS_CreateTask(socketTestTask,
        NULL, NULL, TEST_TASK_STACK_SIZE, TEST_TASK_PRIORITY, 0, 0, TEST_TASK_NAME);
    while(1)
    {
        if(OS_WaitEvent(socketTaskHandle, (void**)&event, OS_TIME_OUT_WAIT_FOREVER))
        {
            EventDispatch(event);
            OS_Free(event->pParam1);
            OS_Free(event->pParam2);
            OS_Free(event);
        }
    }
}
void udp_Main()
{
    socketTaskHandle = OS_CreateTask(udp_MainTask,
        NULL, NULL, MAIN_TASK_STACK_SIZE, MAIN_TASK_PRIORITY, 0, 0, MAIN_TASK_NAME);
    OS_SetUserMainHandle(&socketTaskHandle);
}

