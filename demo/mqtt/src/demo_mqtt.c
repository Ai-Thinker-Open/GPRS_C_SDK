#include "stdbool.h"
#include "stdint.h"
#include "stdio.h"
#include "string.h"

#include "api_os.h"
#include "api_debug.h"
#include "api_event.h"
#include "api_mqtt.h"
#include "api_network.h"
#include "api_socket.h"

#include "demo_mqtt.h"



#define MAIN_TASK_STACK_SIZE    (2048 * 2)
#define MAIN_TASK_PRIORITY      0
#define MAIN_TASK_NAME          "Main Test Task"

#define SECOND_TASK_STACK_SIZE    (2048 * 2)
#define SECOND_TASK_PRIORITY      1
#define SECOND_TASK_NAME          "MQTT Test Task"

static HANDLE mainTaskHandle = NULL;
static HANDLE secondTaskHandle = NULL;

static HANDLE semMqttStart = NULL;


typedef enum{
    MQTT_EVENT_CONNECTED = 0,
    MQTT_EVENT_DISCONNECTED ,
    MQTT_EVENT_MAX
}MQTT_Event_ID_t;

typedef struct {
    MQTT_Event_ID_t id;
    MQTT_Client_t* client;
}MQTT_Event_t;

typedef enum{
    MQTT_STATUS_DISCONNECTED = 0,
    MQTT_STATUS_CONNECTED       ,
    MQTT_STATUS_MAX
}MQTT_Status_t;

MQTT_Status_t mqttStatus = MQTT_STATUS_DISCONNECTED;


static void EventDispatch(API_Event_t* pEvent)
{
    switch(pEvent->id)
    {
        case API_EVENT_ID_NO_SIMCARD:
            Trace(1,"!!NO SIM CARD%d!!!!",pEvent->param1);
            break;

        case API_EVENT_ID_SYSTEM_READY:
            Trace(1,"system initialize complete");
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
            Trace(1,"network activate success.."); 
            OS_ReleaseSemaphore(semMqttStart);
            break;
        
        case API_EVENT_ID_SOCKET_CONNECTED:
            Trace(1,"socket connected");
            break;
        
        case API_EVENT_ID_SOCKET_CLOSED:
            Trace(1,"socket closed");
                      
            break;

        case API_EVENT_ID_SIGNAL_QUALITY:
            Trace(1,"CSQ:%d",pEvent->param1);
            break;

        default:
            break;
    }
}

void OnMqttReceived(void* arg, const char* topic, uint32_t payloadLen)
{
    Trace(1,"MQTT received publish data request, topic:%s, payload length:%d",topic,payloadLen);
}

void OnMqttReceiedData(void* arg, const uint8_t* data, uint16_t len, MQTT_Flags_t flags)
{
    Trace(1,"MQTT recieved publish data,  length:%d,data:%s",len,data);
    if(flags == MQTT_FLAG_DATA_LAST)
        Trace(1,"MQTT data is last frame");
}

 void OnMqttSubscribed(void* arg, MQTT_Error_t err)
 {
     if(err != MQTT_ERROR_NONE)
        Trace(1,"MQTT subscribe fail,error code:%d",err);
     else
        Trace(1,"MQTT subscribe success,topic:%s",(const char*)arg);
 }

void OnMqttConnection(MQTT_Client_t *client, void *arg, MQTT_Connection_Status_t status)
{
    Trace(1,"MQTT connection status:%d",status);
    MQTT_Event_t* event = (MQTT_Event_t*)OS_Malloc(sizeof(MQTT_Event_t));
    if(!event)
    {
        Trace(1,"MQTT no memory");
        return ;
    }
    if(status == MQTT_CONNECTION_ACCEPTED)
    {
        Trace(1,"MQTT succeed connect to broker");
        //!!! DO NOT suscribe here(interrupt function), do MQTT suscribe in task, or it will not excute
        event->id = MQTT_EVENT_CONNECTED;
        event->client = client;
        OS_SendEvent(secondTaskHandle,event,OS_TIME_OUT_WAIT_FOREVER,OS_EVENT_PRI_NORMAL);
    }
    else
    {
        event->id = MQTT_EVENT_DISCONNECTED;
        event->client = client;
        OS_SendEvent(secondTaskHandle,event,OS_TIME_OUT_WAIT_FOREVER,OS_EVENT_PRI_NORMAL);
        Trace(1,"MQTT connect to broker fail,error code:%d",status);
    }
    Trace(1,"MQTT OnMqttConnection() end");
}

void StartTimerPublish(uint32_t interval,MQTT_Client_t* client);
void OnPublish(void* arg, MQTT_Error_t err)
{
    if(err == MQTT_ERROR_NONE)
        Trace(1,"MQTT publish success");
    else
        Trace(1,"MQTT publish error, error code:%d",err);
}

void OnTimerPublish(void* param)
{
    MQTT_Error_t err;
    MQTT_Client_t* client = (MQTT_Client_t*)param;
    if(mqttStatus != MQTT_STATUS_CONNECTED)
    {
        Trace(1,"MQTT not connected to broker! can not publish");
        return;
    }
    Trace(1,"MQTT OnTimerPublish");
    err = MQTT_Publish(client,PUBLISH_TOPIC,PUBLISH_PAYLOEAD,strlen(PUBLISH_PAYLOEAD),1,2,0,OnPublish,NULL);
    if(err != MQTT_ERROR_NONE)
        Trace(1,"MQTT publish error, error code:%d",err);
    StartTimerPublish(PUBLISH_INTERVAL,client);
}

void StartTimerPublish(uint32_t interval,MQTT_Client_t* client)
{
    OS_StartCallbackTimer(mainTaskHandle,interval,OnTimerPublish,(void*)client);
}

void SecondTaskEventDispatch(MQTT_Event_t* pEvent)
{
    switch(pEvent->id)
    {
        case MQTT_EVENT_CONNECTED:
            mqttStatus = MQTT_STATUS_CONNECTED;
            Trace(1,"MQTT connected, now subscribe topic:%s",SUBSCRIBE_TOPIC);
            MQTT_Error_t err;
            MQTT_SetInPubCallback(pEvent->client, OnMqttReceived, OnMqttReceiedData, NULL);
            err = MQTT_Subscribe(pEvent->client,SUBSCRIBE_TOPIC,2,OnMqttSubscribed,(void*)SUBSCRIBE_TOPIC);
            if(err != MQTT_ERROR_NONE)
                Trace(1,"MQTT subscribe error, error code:%d",err);
            StartTimerPublish(PUBLISH_INTERVAL,pEvent->client);
            break;
        case MQTT_EVENT_DISCONNECTED:
            mqttStatus = MQTT_STATUS_DISCONNECTED;
            break;
        default:
            break;
    }
}

void SecondTask(void *pData)
{
    MQTT_Event_t* event=NULL;
    semMqttStart = OS_CreateSemaphore(0);
    OS_WaitForSemaphore(semMqttStart,OS_WAIT_FOREVER);
    OS_DeleteSemaphore(semMqttStart);
    Trace(1,"start mqtt test");
    MQTT_Client_t* client = MQTT_ClientNew();
    MQTT_Connect_Info_t ci;
    MQTT_Error_t err;
    memset(&ci,0,sizeof(MQTT_Connect_Info_t));
    ci.client_id = CLIENT_ID;
    ci.client_user = CLIENT_USER;
    ci.client_pass = CLIENT_PASS;
    ci.keep_alive = 60;
    ci.clean_session = 1;
    ci.use_ssl = false;

    err = MQTT_Connect(client,BROKER_IP,BROKER_PORT,OnMqttConnection,NULL,&ci);
    if(err != MQTT_ERROR_NONE)
        Trace(1,"MQTT connect fail,error code:%d",err);
    
    while(1)
    {
        if(OS_WaitEvent(secondTaskHandle, (void**)&event, OS_TIME_OUT_WAIT_FOREVER))
        {
            SecondTaskEventDispatch(event);
            OS_Free(event);
        }
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

void mqtt_Main(void)
{
    mainTaskHandle = OS_CreateTask(MainTask,
        NULL, NULL, MAIN_TASK_STACK_SIZE, MAIN_TASK_PRIORITY, 0, 0, MAIN_TASK_NAME);
    OS_SetUserMainHandle(&mainTaskHandle);
}



