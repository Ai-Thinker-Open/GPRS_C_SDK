
/*
 * @File  demo_socket_nbio.c
 * @Brief socket(TCP) communication with Non Block IO(event drive) example
 * 
 * @Author: Neucrack 
 * @Date: 2018-06-12 18:05:26 
 * @Last Modified by: Neucrack
 * @Last Modified time: 2018-06-12 18:05:56
 */





#include <string.h>
#include <stdio.h>
#include <api_os.h>
#include <api_event.h>
#include <api_socket.h>
#include <api_network.h>
#include <api_debug.h>


/*******************************************************************/
/////////////////////////socket configuration////////////////////////
// edit ip address and port
// (you can get ip and port from online tcp debug tool: http://tt.ai-thinker.com:8000/ttcloud)
#define SERVER_IP   "122.114.122.174"
#define SERVER_PORT 34404

#define DNS_DOMAIN  "www.neucrack.com"
#define RECEIVE_BUFFER_MAX_LENGTH 200
/*******************************************************************/


#define MAIN_TASK_STACK_SIZE    (2048 * 2)
#define MAIN_TASK_PRIORITY      0
#define MAIN_TASK_NAME          "Socket Test Task"

static HANDLE socketTaskHandle = NULL;

int socketFd = -1;
int socketFd2 = -1;
uint8_t buffer[RECEIVE_BUFFER_MAX_LENGTH];
int receivedDataCount = -1;

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
            //Start DNS test
            memset(buffer,0,sizeof(buffer));
            DNS_Status_t dnsRet = DNS_GetHostByName(DNS_DOMAIN,buffer);
            if(dnsRet == DNS_STATUS_OK)
            {
                Trace(2,"DNS get ip address from domain success(return),domain:%s,ip:%s",DNS_DOMAIN,buffer);
            }
            else if(dnsRet == DNS_STATUS_ERROR)
            {
                Trace(2,"DNS get ip address error(return)!!!");
            }
            //Start connect tcp server
            socketFd = Socket_TcpipConnect(TCP,SERVER_IP,SERVER_PORT);
            Trace(2,"connect tcp server,socketFd:%d",socketFd);
            break;

        case API_EVENT_ID_SOCKET_CONNECTED:
            if(pEvent->param1 == socketFd)
            {
                socketFd2 = Socket_TcpipConnect(TCP,SERVER_IP,SERVER_PORT);
                Trace(2,"connected tcp server,socketFd:%d",socketFd2);
            }
            Socket_TcpipWrite(pEvent->param1,"hello...test string\n",strlen("hello...test string\n"));
            Trace(2,"socket %d send %d bytes data to server:%s",pEvent->param1, strlen("hello...test string\n"),"hello...test string\n");
            break;

        case API_EVENT_ID_SOCKET_SENT:
        {
            int fd = pEvent->param1;
            Trace(2,"socket %d send data complete",fd);
            break;
        }
        case API_EVENT_ID_SOCKET_RECEIVED:
        {
            int fd = pEvent->param1;
            int length = pEvent->param2>RECEIVE_BUFFER_MAX_LENGTH?RECEIVE_BUFFER_MAX_LENGTH:pEvent->param2;
            memset(buffer,0,sizeof(buffer));
            length = Socket_TcpipRead(fd,buffer,length);
            Trace(2,"socket %d received %d bytes data:%s",fd,length,buffer);
            Socket_TcpipWrite(fd,buffer,length);
            Trace(2,"send received data to server");
            
            if(++receivedDataCount > 20)
            {
                Trace(2,"socket received %d times, now try to close socket",receivedDataCount);
                Socket_TcpipClose(socketFd);
                Socket_TcpipClose(socketFd2);
            }
            break;
        }
        case API_EVENT_ID_SOCKET_CLOSED:
        {
            int fd = pEvent->param1;
            Trace(2,"socket %d closed",fd);
            break;
        }
        case API_EVENT_ID_SOCKET_ERROR:
        {
            int fd = pEvent->param1;
            Trace(2,"socket %d error occurred,cause:%d",fd,pEvent->param2);
            break;
        }
        case API_EVENT_ID_DNS_SUCCESS:
            Trace(2,"DNS get ip address from domain success(event),domain:%s,ip:%s",pEvent->pParam1,pEvent->pParam2);
            break;

        case API_EVENT_ID_DNS_ERROR:
            Trace(2,"DNS get ip address error(event)!!!");
            break;

        default:
            break;
    }
}



void socket_MainTask(void *pData)
{
    API_Event_t* event=NULL;

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

void socket_nbio_Main(void)
{
    socketTaskHandle = OS_CreateTask(socket_MainTask,
        NULL, NULL, MAIN_TASK_STACK_SIZE, MAIN_TASK_PRIORITY, 0, 0, MAIN_TASK_NAME);
    OS_SetUserMainHandle(&socketTaskHandle);
}

