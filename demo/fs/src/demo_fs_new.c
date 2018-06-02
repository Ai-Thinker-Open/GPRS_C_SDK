
#include "stdbool.h"
#include "stdint.h"
#include "stdio.h"
#include "string.h"

#include "api_os.h"
#include "api_debug.h"
#include "api_event.h"
#include "api_fs.h"
#include "api_charset.h"

#define CONFIG_FILE_NAME "/test0.conf"
#define TF_CARD_TEST_FILE_NAME "/t/test_TF_card.txt"
#define TF_CARD_TEST_STRING    "12346test string, please open with utf-8! 哈哈哈哈，请用UTF-8编码打开这个文件哦，不然看不到中文\r\n\r\n"

#define MAIN_TASK_STACK_SIZE    (2048 * 2)
#define MAIN_TASK_PRIORITY      0
#define MAIN_TASK_NAME          "Fs Test Task"

static HANDLE mainTaskHandle = NULL;

typedef struct{
    int a;
    int b;
    int c;
}Data_t;

void TraceBytes(uint16_t level, uint8_t* data, uint32_t len, uint8_t base, uint16_t lenRow)
{
    uint8_t buff[lenRow*4+1];
    uint32_t count = 0;

    memset(buff,0,sizeof(buff));
    while(count<len)
    {
        if(base == 10)
        {
            snprintf(buff+strlen(buff),sizeof(buff)-strlen(buff),"%d ",data[count++]);
        }
        else if(base == 16)
        {
            snprintf(buff+strlen(buff),sizeof(buff)-strlen(buff),"%02x ",data[count++]);
        }
        if(count%lenRow == 0 || count==len)
        {
            Trace(level,"%s",buff);
            memset(buff,0,sizeof(buff));
        }
    }
}

bool SaveData(Data_t* data)
{
    int32_t fd;
    int32_t ret;
    uint8_t *path = CONFIG_FILE_NAME;

    fd = API_FS_Open(path, FS_O_RDWR | FS_O_CREAT, 0);
	if ( fd < 0)
	{
        Trace(1,"Open file failed:%d",fd);
		return false;
	}
    ret = API_FS_Write(fd, (uint8_t*)data, sizeof(Data_t));
    API_FS_Close(fd);
    if(ret <= 0)
        return false;
	return true;
}
bool ReadData(Data_t* data)
{
    int32_t fd;
    int32_t ret;
    uint8_t *path = CONFIG_FILE_NAME;

    fd = API_FS_Open(path, (FS_O_RDONLY|FS_O_CREAT), 0);
	if ( fd < 0)
	{
        Trace(1,"Open file failed:%d",fd);
		return false;
    }
    ret = API_FS_Read(fd, (uint8_t*)data, sizeof(Data_t)) ;
    Trace(1,"read ret:%d,sizeof(Data_t):%d",ret,sizeof(Data_t));
    API_FS_Close(fd);
    if(ret <= 0)
        return false;
	return true;
}

void FsInfoTest()
{
    API_FS_INFO fsInfo;
    int sizeUsed = 0, sizeTotal = 0;

    Trace(1,"Start Fs info test!");
    
    if(API_FS_GetFSInfo(FS_DEVICE_NAME_FLASH,&fsInfo) < 0)
    {
        Trace(1,"Get FS Flash device info fail!");
    }
    sizeUsed  = fsInfo.usedSize;
    sizeTotal = fsInfo.totalSize;
    Trace(1,"flash used:%d Bytes, total size:%d Bytes",sizeUsed,sizeTotal);
    if(API_FS_GetFSInfo(FS_DEVICE_NAME_T_FLASH,&fsInfo) < 0)
    {
        Trace(1,"Get FS T Flash device info fail!");
    }
    sizeUsed  = fsInfo.usedSize;
    sizeTotal = fsInfo.totalSize;
    float mb = sizeTotal/1024.0/1024.0;
    Trace(1,"T Flash used:%d Bytes, total size:%d Bytes(%d.%03d MB)",sizeUsed,sizeTotal,(int)mb, (int)((mb-(int)mb)*1000)  );
}


void FsTestCase()
{
    Data_t data;
    Trace(1,"Start Fs read write test!");

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

bool FsTFTest()
{
    int32_t fd;
    int32_t ret;
    uint8_t *path = TF_CARD_TEST_FILE_NAME;

    Trace(1,"Start Fs  TF card read write test!");

    fd = API_FS_Open(path, FS_O_RDWR | FS_O_CREAT, 0);
	if ( fd < 0)
	{
        Trace(1,"Open file failed:%d",fd);
		return false;
	}
    ret = API_FS_Write(fd, (uint8_t*)TF_CARD_TEST_STRING, strlen(TF_CARD_TEST_STRING));
    API_FS_Close(fd);
    if(ret <= 0)
    {
        Trace(1,"write fail");
        return false;
    }
    Trace(1,"write success");
	return true;
}


void FsTestTask(void* param)
{
    OS_Sleep(3000);
    FsInfoTest();
    FsTestCase();
    FsTFTest();
    Trace(1,"fs test end");
    while(1)
    {
        OS_Sleep(5000);
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
            
            break;
        default:
            break;
    }
}


void FsTest(void *pData)
{
    API_Event_t* event=NULL;

    OS_CreateTask(FsTestTask,
        NULL, NULL, MAIN_TASK_STACK_SIZE, MAIN_TASK_PRIORITY, 0, 0, MAIN_TASK_NAME);

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

void FS_NewTest()
{
    mainTaskHandle = OS_CreateTask(FsTest,
        NULL, NULL, MAIN_TASK_STACK_SIZE, MAIN_TASK_PRIORITY, 0, 0, MAIN_TASK_NAME);
    OS_SetUserMainHandle(&mainTaskHandle);
}


