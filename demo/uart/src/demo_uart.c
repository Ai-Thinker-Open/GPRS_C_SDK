
#include <api_os.h>
#include <api_hal_uart.h>
#include <api_debug.h>
// #include <string.h>
// #include <stdio.h>
#include <api_os.h>
#include <api_event.h>


#define MAIN_TASK_STACK_SIZE    (2048 * 2)
#define MAIN_TASK_PRIORITY      0 
#define MAIN_TASK_NAME         "Main Test Task"

#define UART_TASK_STACK_SIZE    (2048 * 2)
#define UART_TASK_PRIORITY      0 
#define UART_TASK_NAME         "UART Test Task"

static HANDLE uartTaskHandle = NULL;
static HANDLE mainTaskHandle = NULL;

uint8_t rxbuffer[100];
void OnUart1ReceivedData(UART_Callback_Param_t param)
{
    memset(rxbuffer,0,sizeof(rxbuffer));
    uint32_t len = UART_Read(UART1,rxbuffer,param.length,200);
    Trace(1,"uart1 received data,length:%d,read:%d,data:%s",param.length,len,rxbuffer);
}

void uart_MainTask()
{
    uint32_t times = 0;
    uint8_t temp[500];
    UART_Config_t config = {
        .baudRate = UART_BAUD_RATE_115200,
        .dataBits = UART_DATA_BITS_8,
        .stopBits = UART_STOP_BITS_1,
        .parity   = UART_PARITY_NONE,
        .rxCallback = OnUart1ReceivedData,
    };

    UART_Init(UART1,config);
    // config.rxCallback = NULL;
    // UART_Init(UART2,config);

    while(1)
    {
        for(int i=0;i<500;++i)
        {
            temp[i] = i%5+'0';
        }
        sprintf(temp,"%d", ++times);
        temp[strlen(temp)] = ':';
        temp[498] = '\n';
        temp[499] = 0;
        UART_Write(UART1,temp,500);
        // Trace(1,"UART_Write:%s %p",temp, &times);
        // memset(temp,0,sizeof(temp));
        // uint32_t readLen = UART_Read(UART1,temp,240,5000);
        // Trace(1,"UART_Read uart2,readLen:%d,data:%s",readLen,temp);
        OS_Sleep(5000);
    }
}


void EventDispatch(API_Event_t* pEvent)
{
    switch(pEvent->id)
    {
        default:
            break;
    }
}

void MainTask(void *pData)
{
    API_Event_t* event=NULL;

    uartTaskHandle = OS_CreateTask(uart_MainTask,
        NULL, NULL, UART_TASK_STACK_SIZE, UART_TASK_PRIORITY, 0, 0, UART_TASK_NAME);

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

void uart_Main(void)
{
    mainTaskHandle = OS_CreateTask(MainTask,
        NULL, NULL, MAIN_TASK_STACK_SIZE, MAIN_TASK_PRIORITY, 0, 0, MAIN_TASK_NAME);
    OS_SetUserMainHandle(&mainTaskHandle);
}

