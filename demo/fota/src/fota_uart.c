

#include "stdint.h"
#include "stdbool.h"
#include "string.h"
#include "stdlib.h"
#include "stdio.h"
#include "api_hal_uart.h"
#include "api_fota.h"

#include "api_os.h"
#include "api_event.h"
#include "api_debug.h"
#include "demo_fota.h"



////////////////////////////////////////////////////
//   change comment in old version and new version
#define VERSION "V1.0.0"
// #define VERSION "V2.0.0"
///////////////////////////////////////////////////






#define MAIN_TASK_STACK_SIZE    (1024 * 2)
#define MAIN_TASK_PRIORITY      0 
#define MAIN_TASK_NAME         "FOTA UART Test Task"

#define FOTA_UART_HEADER         "fsize"
#define FOTA_UART_SIZE           strlen(FOTA_UART_HEADER)


static HANDLE mainTaskHandle = NULL;

static void FOTA_ReceivedData(uint8_t *data, uint16_t len)
{
    static int fotasize=0;
    Trace(1,"uart received data,length:%d,data:%s %d", len, data, FOTA_UART_SIZE);
    MEMBLOCK_Trace(1, data, len, 16);
    if(fotasize == 0 && memcmp(data, FOTA_UART_HEADER, FOTA_UART_SIZE) == 0)
    {
        fotasize = atoi(data + FOTA_UART_SIZE);
        Trace(1,"need init, uart fota pack total size:%d", fotasize);
        if(fotasize)
        {  
            if(!API_FotaInit(fotasize))
                goto upgrade_faile;
        }
    }
    else if(fotasize)
    {
        Trace(1,"uart fota pack total size:%d", fotasize);
        if(API_FotaReceiveData(data, len) == 0)
            goto upgrade_faile;
    }
    return ;

upgrade_faile:
    Trace(1,"uart fota fail!!!");
    fotasize = 0;
    API_FotaClean();
    return ;
}

static void EventDispatch(API_Event_t* pEvent)
{
    switch(pEvent->id)
    {
        case API_EVENT_ID_UART_RECEIVED:
            if(pEvent->param1 == UART1)
            {
                uint8_t data[pEvent->param2+1];
                data[pEvent->param2] = 0;
                memcpy(data,pEvent->pParam1,pEvent->param2);
                FOTA_ReceivedData(data,pEvent->param2); 
            }
            break;
        default:
            break;
    }
}

void EchoTask(void* param)
{
    while(1)
    {
        Trace(1,"hello, I'm version %s",VERSION);
        OS_Sleep(3000);
    }
}

static void FOTAUART_TestTask()
{
    API_Event_t* event=NULL;
    UART_Config_t config = {
        .baudRate = UART_BAUD_RATE_115200,
        .dataBits = UART_DATA_BITS_8,
        .stopBits = UART_STOP_BITS_1,
        .parity   = UART_PARITY_NONE,
        .rxCallback = NULL,
        .useEvent = true,
    };

    UART_Init(UART1,config);
    OS_CreateTask(EchoTask ,
        NULL, NULL, MAIN_TASK_STACK_SIZE, MAIN_TASK_PRIORITY+1, 0, 0, "echo task");
    
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

void FOTA_UartTest(void)
{
    mainTaskHandle = OS_CreateTask(FOTAUART_TestTask ,
        NULL, NULL, MAIN_TASK_STACK_SIZE, MAIN_TASK_PRIORITY, 0, 0, MAIN_TASK_NAME);
    OS_SetUserMainHandle(&mainTaskHandle);
}

