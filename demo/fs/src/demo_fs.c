
#include "stdbool.h"
#include "stdint.h"
#include "stdio.h"
#include "string.h"

#include "api_os.h"
#include "api_debug.h"
#include "api_event.h"
#include "api_fs.h"

#define CONFIG_FILE_NAME "/test.txt"

#define MAIN_TASK_STACK_SIZE    (2048 * 2)
#define MAIN_TASK_PRIORITY      0
#define MAIN_TASK_NAME          "Fs Test Task"
#define FS_TASK_NAME            "Fs  Task"

static HANDLE mainTaskHandle = NULL;
static HANDLE fsTaskHandle = NULL;

typedef struct{
    int a;
    int b;
    int c;
}Data_t;

bool SaveData(Data_t* data)
{
    int32_t fd;
    int32_t ret;
    // UINT8 *unicodeName = NULL;
    // UINT32 outlen = 0;

    // ret = Ascii2Unicode(CONFIG_FILE_NAME, strlen(CONFIG_FILE_NAME), &unicodeName, &outlen, NULL);
    // if(ret != 0)
        // return false;

    fd = API_FS_Open(CONFIG_FILE_NAME, FS_O_RDWR | FS_O_CREAT, 0);
	if ( fd < 0)
	{
        Trace(1,"Open file failed:%d",fd);
        // OS_Free(unicodeName);
		return false;
	}
    ret = API_FS_Write(fd, (uint8_t*)data, sizeof(Data_t));
    // API_FS_Flush(fd);
    API_FS_Close(fd);
    // OS_Free(unicodeName);
    if(ret <= 0)
        return false;
	return true;
}
bool ReadData(Data_t* data)
{
    int32_t fd;
    int32_t ret;
    // UINT8 *unicodeName = NULL;
    // UINT32 outlen = 0;


    // ret = Ascii2Unicode(CONFIG_FILE_NAME, strlen(CONFIG_FILE_NAME), &unicodeName, &outlen, NULL);
    // if(ret != 0)
    //     return false;
    fd = API_FS_Open(CONFIG_FILE_NAME, (FS_O_RDONLY|FS_O_CREAT), 0);
	if ( fd < 0)
	{
		return false;
    }
    ret = API_FS_Read(fd, (uint8_t*)data, sizeof(Data_t)) ;
    Trace(1,"read ret:%d,sizeof(Data_t):%d",ret,sizeof(Data_t));
    API_FS_Close(fd);
    // OS_Free(unicodeName);
    if(ret <= 0)
        return false;
	return true;
}

void FsTestCase()
{
    Data_t data;

    Trace(1,"Start Fs test");
    //read
    if(!ReadData(&data))
    {
        data.a = 0;
        data.b = 1;
        data.c = 2;
        Trace(1,"Read data fail");
    }
    else
    {
        if(data.a < 0)
            data.a = 0;
        if(data.b < 0)
            data.b = 1;
        if(data.c < 0)
            data.c = 2;
        Trace(1,"Read data success, a:%d, b:%d, c:%d",data.a,data.b,data.c);
    }
    //write
    ++data.a;
    ++data.b;
    ++data.c;
    if(!SaveData(&data))
        Trace(1,"Save Data fail");
    else
        Trace(1,"Save data success, a:%d, b:%d, c:%d",data.a,data.b,data.c);
    //read
    memset(&data,0,sizeof(Data_t));
    if(!ReadData(&data))
        Trace(1,"Read data fail");
    else
    {
        Trace(1,"Read data success, a:%d, b:%d, c:%d",data.a,data.b,data.c);
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
            FsTestCase();
            break;
        default:
            break;
    }
}


void Init()
{

}

void FsTest(void *pData)
{
    API_Event_t* event=NULL;

    Init();

    while(1)
    {
        if(OS_WaitEvent(mainTaskHandle, &event, OS_TIME_OUT_WAIT_FOREVER))
        {
            EventDispatch(event);
            OS_Free(event->pParam1);
            OS_Free(event);
        }
    }
}

void fs_Main()
{
    mainTaskHandle = OS_CreateTask(FsTest,
        NULL, NULL, MAIN_TASK_STACK_SIZE, MAIN_TASK_PRIORITY, 0, 0, MAIN_TASK_NAME);
    OS_SetUserMainHandle(&mainTaskHandle);
}


