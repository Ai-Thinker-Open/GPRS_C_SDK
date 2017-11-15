
#include <api_os.h>
#include <api_sms.h>
#include <api_event.h>
#include <api_debug.h>


#define TEST_PHONE_NUMBER "150....0062"

#define MAIN_TASK_STACK_SIZE    (2048 * 2)
#define MAIN_TASK_PRIORITY      0
#define MAIN_TASK_NAME          "SMS Test Task"

static HANDLE smsTaskHandle = NULL;


void SendSMS()
{
    Trace(1,"sms start send message");
    if(!SMS_SetFormat(SMS_FORMAT_TEXT,SIM0))
    {
        Trace(1,"sms set format error");
        return;
    }
    SMS_Parameter_t smsParam = {
        .fo = 17 ,
        .vp = 167,
        .pid= 0  ,
        .dcs=0
    };
    if(!SMS_SetParameter(&smsParam,SIM0))
    {
        Trace(1,"sms set parameter error");
        return;
    }
    if(!SMS_SendMessage(TEST_PHONE_NUMBER,"hello,sms test!",0,SIM0))
    {
        Trace(1,"sms send message fail");
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
            Trace(2,"network register success");
            SendSMS();
            break;
        case API_EVENT_ID_SMS_SENT:
            Trace(2,"Send Message Success");
            break;
        default:
            break;
    }
}



void sms_MainTask(void* pData)
{
    API_Event_t* event=NULL;

    while(1)
    {
        if(OS_WaitEvent(smsTaskHandle, &event, OS_TIME_OUT_WAIT_FOREVER))
        {
            EventDispatch(event);
            OS_Free(event->pParam1);
            OS_Free(event);
        }
    }
}

void sms_Main()
{
    smsTaskHandle = OS_CreateTask(sms_MainTask,
        NULL, NULL, MAIN_TASK_STACK_SIZE, MAIN_TASK_PRIORITY, 0, 0, MAIN_TASK_NAME);
    OS_SetUserMainHandle(&smsTaskHandle);
}

