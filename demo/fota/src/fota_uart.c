

#include "stdint.h"
#include "stdbool.h"
#include "string.h"
#include "stdio.h"
#include "api_hal_uart.h"
#include "api_fota.h"

#include "api_os.h"
#include "api_event.h"
#include "api_debug.h"

#define MAIN_TASK_STACK_SIZE    (1024 * 2)
#define MAIN_TASK_PRIORITY      0 
#define MAIN_TASK_NAME         "FOTA UART Test Task"

#define FOTA_UART_HEADER         "fsize"
#define FOTA_UART_SIZE        sizeof(FOTA_UART_HEADER)-1

static HANDLE mainTaskHandle = NULL;
// static void FOTA_ReceivedFileData(UINT8 *data, UINT16 len)
// {
//     static int fotasize=0;
//     int pos = strstr(data, "\r\n");
//     Trace(1,"uart received data,length:%d,data: %d, pos: ", len, (pos));
//     MEMBLOCK_Trace(1, data, len, 16);
//     if(fotasize == 0 && memcmp(data, FOTA_UART_HEADER, FOTA_UART_SIZE) == 0)
//     {
//         fotasize = atoi(data + FOTA_UART_SIZE);
//         Trace(1,"uart fotasize:%d len:%d", fotasize, len);
//         if(fotasize)
//         {  
//             if(!API_FotaInit(fotasize))
//                 goto upgrade_faile;
//         }
//     }
//     else if(fotasize)
//     {
//         Trace(1,"uart fota,fotasize:%d len:%d", fotasize, len);
//         if(API_FotaReceiveData(data, len) == 0)
//             goto upgrade_faile;
//     }

// upgrade_faile:
//     Trace(1,"uart fota false");
//     API_FotaClean();
//     return 0;
// }

static void FOTA_ReceivedData(UINT8 *data, UINT16 len)
{
    static int fotasize=0;
    Trace(1,"uart received data,length:%d,data:%s %d", len, data, FOTA_UART_SIZE);
    MEMBLOCK_Trace(1, data, len, 16);
    if(fotasize == 0 && memcmp(data, FOTA_UART_HEADER, FOTA_UART_SIZE) == 0)
    {
        fotasize = atoi(data + FOTA_UART_SIZE);
        Trace(1,"uart fotasize:%d len:%d", fotasize, len);
        if(fotasize)
        {  
            if(!API_FotaInit(fotasize))
                goto upgrade_faile;
        }
    }
    else if(fotasize)
    {
        Trace(1,"uart fota,fotasize:%d len:%d", fotasize, len);
        if(API_FotaReceiveData(data, len) == 0)
            goto upgrade_faile;
    }
    return ;

upgrade_faile:
    Trace(1,"uart fota false");
    // API_FotaClean();
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
                // FOTA_ReceivedFileData(data,pEvent->param2);
            }
            break;
        default:
            break;
    }
}
static void FOTAUART_TestTask()
{
    UART_Config_t config = {
        .baudRate = UART_BAUD_RATE_115200,
        .dataBits = UART_DATA_BITS_8,
        .stopBits = UART_STOP_BITS_1,
        .parity   = UART_PARITY_NONE,
        .rxCallback = NULL,
        .useEvent = true,
    };
    OS_Sleep(1000);//no task
    API_Event_t* event=NULL;
    Trace(1,"uart2 init is master");
    UART_Init(UART1,config);

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

void FOTA_UartTest(void)
{
    mainTaskHandle = OS_CreateTask(FOTAUART_TestTask ,
        NULL, NULL, MAIN_TASK_STACK_SIZE, MAIN_TASK_PRIORITY, 0, 0, MAIN_TASK_NAME);
    OS_SetUserMainHandle(&mainTaskHandle);
}

