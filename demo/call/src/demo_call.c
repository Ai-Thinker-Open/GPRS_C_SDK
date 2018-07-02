





#include "stdbool.h"
#include "stdint.h"
#include "stdio.h"
#include "string.h"

#include "api_os.h"
#include "api_debug.h"
#include "api_event.h"
#include "api_call.h"
#include "api_audio.h"

#include "demo_call.h"


#define MAIN_TASK_STACK_SIZE    (2048 * 2)
#define MAIN_TASK_PRIORITY      0
#define MAIN_TASK_NAME          "Main Test Task"

#define SECOND_TASK_STACK_SIZE    (2048 * 2)
#define SECOND_TASK_PRIORITY      1
#define SECOND_TASK_NAME          "Second Test Task"

static HANDLE mainTaskHandle = NULL;
static HANDLE secondTaskHandle = NULL;

uint8_t flag = 0;
bool isDialSuccess = false;

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
            Trace(1,"network register success");
            flag = 1;
            break;
        case API_EVENT_ID_CALL_DIAL://param1: isSuccess, param2:error code(CALL_Error_t)
            Trace(1,"Is dial success:%d, error code:%d",pEvent->param1,pEvent->param2);
            if(pEvent->param1)
                isDialSuccess = true;
            break;
        case API_EVENT_ID_CALL_HANGUP:  //param1: is remote release call, param2:error code(CALL_Error_t)
            Trace(1,"Hang up,is remote hang up:%d, error code:%d",pEvent->param1,pEvent->param2);
            break;
        case API_EVENT_ID_CALL_INCOMING:   //param1: number type, pParam1:number
            Trace(1,"Receive a call, number:%s, number type:%d",pEvent->pParam1,pEvent->param1);
            OS_Sleep(5000);
            if(!CALL_Answer())
                Trace(1,"answer fail");
            break;
        case API_EVENT_ID_CALL_ANSWER  :  
            Trace(1,"answer success");
            break;
        case API_EVENT_ID_CALL_DTMF    :  //param1: key
            Trace(1,"received DTMF tone:%c",pEvent->param1);
            break;
        default:
            break;
    }
}

void CallTest()
{
    //initialize
    Trace(1,"start Call test, initialize audio first");
    AUDIO_MicOpen();
    AUDIO_SpeakerOpen();
    AUDIO_MicSetMute(false);
    AUDIO_SpeakerSetVolume(15);

    //make a call
    Trace(1,"start make a call");
    if(!CALL_Dial(DIAL_NUMBER))
    {
        Trace(1,"make a call failed");
        return;
    }
    while(!isDialSuccess)
        OS_Sleep(100);

    
    CALL_Status_t* callStatus = NULL;
    uint8_t count;
    bool ret = CALL_Status(&callStatus,count);
    uint8_t i;
    if(ret)
    {
        for(i=0;i<count;++i)
        {
            Trace(1,"index:%d,direction:%d,status:%d,mode:%d,mpty:%d,number:%s,number type:%d",
                                                    callStatus[i].index,callStatus[i].direction,callStatus[i].status,callStatus[i].mode,
                                                    callStatus[i].multiparty,callStatus[i].number,callStatus[i].numberType);
        }
        OS_Free(callStatus);
    }
    
    
    uint8_t dtmf = '0';
    for(int i=0;i<10;++i,++dtmf)
    {
        Trace(1,"make a DTMF:%c",dtmf);
        if(!CALL_DTMF(dtmf,CALL_DTMF_GAIN_m3dB,5,15,true))
        {
            Trace(1,"DTMF fail");
            break;
        }
        OS_Sleep(3000);
    }
    Trace(1,"sleep start");
    OS_Sleep(10000);
    Trace(1,"sleep end");
    if(!CALL_HangUp())
        Trace(1,"hang up fail");
}

void SecondTask(void *pData)
{
    while(1)
    {
        if(flag == 1)
        {
            CallTest();
            flag = 2;
        }
        OS_Sleep(100);
    }
}

void MainTask(void *pData)
{
    API_Event_t* event=NULL;
    flag = 0;
    isDialSuccess = false;

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

void call_Main(void)
{
    mainTaskHandle = OS_CreateTask(MainTask,
        NULL, NULL, MAIN_TASK_STACK_SIZE, MAIN_TASK_PRIORITY, 0, 0, MAIN_TASK_NAME);
    OS_SetUserMainHandle(&mainTaskHandle);    
}


