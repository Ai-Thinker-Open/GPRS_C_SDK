
#include "stdbool.h"
#include "stdint.h"
#include "stdio.h"
#include "string.h"

#include "api_os.h"
#include "api_debug.h"
#include "api_event.h"
#include "api_audio.h"
#include "api_fs.h"


#define RECORD_FILE_NAME "/t/record0.amr"

#define MAIN_TASK_STACK_SIZE    (2048 * 2)
#define MAIN_TASK_PRIORITY      0
#define MAIN_TASK_NAME          "Main Test Task"

#define SECOND_TASK_STACK_SIZE    (2048 * 2)
#define SECOND_TASK_PRIORITY      1
#define SECOND_TASK_NAME          "Second Test Task"

static HANDLE mainTaskHandle = NULL;
static HANDLE secondTaskHandle = NULL;

static int recordFd = -1;
static bool playEnd = true;

void RecordTest();


void EventDispatch(API_Event_t* pEvent)
{
    switch(pEvent->id)
    {
        case API_EVENT_ID_NO_SIMCARD:
            Trace(10,"!!NO SIM CARD%d!!!!",pEvent->param1);
            break;

        case API_EVENT_ID_SYSTEM_READY:
            Trace(1,"system initialize complete00");
            break;

        case API_EVENT_ID_NETWORK_REGISTERED_HOME:
        case API_EVENT_ID_NETWORK_REGISTERED_ROAMING:
            Trace(2,"network register success00");
            break;

        default:
            break;
    }
}

void PlayCallback(AUDIO_Error_t result)
{
    Trace(1,"play callback result:%d",result);
    if(AUDIO_ERROR_END_OF_FILE == result)
        Trace(1,"play music file end");
    playEnd = true;
    AUDIO_Stop();
}

void Music()
{
    const uint8_t* path = "/t/music.mp3";
    Trace(1,"play music %s now",path);

    AUDIO_Error_t ret = AUDIO_Play(path,AUDIO_TYPE_MP3,PlayCallback);
    if( ret != AUDIO_ERROR_NO)
    {
        Trace(1,"play music fail:%d",ret);
        return;
    }
    else
    {
        playEnd = false;
        Trace(1,"play music success");
    }
    OS_Sleep(200);
    AUDIO_SetMode(AUDIO_MODE_LOUDSPEAKER);
    AUDIO_SpeakerOpen();
    AUDIO_SpeakerSetVolume(4);

    uint8_t i = AUDIO_EQ_NORMAL;
    while(!playEnd)
    {
        AUDIO_SetEQ(i++);
        if(i==AUDIO_EQ_MAX)
            i = AUDIO_EQ_NORMAL;
        Trace(1,"EQ mode:%d",i);
        OS_Sleep(5000);
    }
}

void LoopTest()
{
    Trace(1,"start audio loop test now");

    AUDIO_SetMode(AUDIO_MODE_LOUDSPEAKER);
    AUDIO_SpeakerOpen();
    AUDIO_SpeakerSetVolume(15);
    AUDIO_LoopTestStart(AUDIO_MODE_LOUDSPEAKER);
    OS_Sleep(30000);
    AUDIO_LoopTestEnd();
}

void CallbackResult(AUDIO_Error_t result)
{
    if(result != AUDIO_ERROR_NO)
    {
        Trace(1,"audio record error:%d",result);
    }
}

/*
void CallbackData(unsigned char* data, unsigned int len, unsigned int* recordLen)
{
    if (recordFd == -1)
    {
        Trace(1,"file not open");
        //open here
        return;
    }
    int32_t wlen = API_FS_Write(recordFd, data, len);
    if (wlen < 0)
    {
        Trace(1, "Write record failed");
        return;
    }
    else
        *recordLen = wlen;
    Trace(1, "record write len=%d", wlen);
}
*/

void RecordTest()
{
    Trace(1,"record start format amr");
    recordFd = API_FS_Open(RECORD_FILE_NAME,FS_O_WRONLY|FS_O_CREAT|FS_O_TRUNC,0);
    if(recordFd < 0)
    {
        Trace(1,"open file %s error:%d",RECORD_FILE_NAME,recordFd);
        return ;
    }
    AUDIO_SetMode(AUDIO_MODE_LOUDSPEAKER);
    AUDIO_Error_t result =  AUDIO_RecordStart(AUDIO_TYPE_AMR,AUDIO_RECORD_MODE_AMR122,recordFd,CallbackResult,NULL);
    if(result != AUDIO_ERROR_NO)
    {
        Trace(1,"record start fail:%d",result);
        API_FS_Close(recordFd);
        recordFd = -1;
        return ;
    }
    Trace(1,"recording now...");
    int i = 10 ;
    while(--i)
    {
        Trace(1,"recording...");
        OS_Sleep(2000);
    }
    Trace(1,"record stop now");
    AUDIO_RecordStop();
    API_FS_Close(recordFd);
    recordFd = -1;
    Trace(1,"record end");
}

void SecondTask(void *pData)
{
    
    OS_Sleep(5000);

    RecordTest();
    LoopTest();
    Music();

    while(1)
        OS_Sleep(1000);
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

void audio_Main()
{
    mainTaskHandle = OS_CreateTask(MainTask,
        NULL, NULL, MAIN_TASK_STACK_SIZE, MAIN_TASK_PRIORITY, 0, 0, MAIN_TASK_NAME);
}

