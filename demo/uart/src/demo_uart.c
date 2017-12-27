
#include <api_os.h>
#include <api_hal_uart.h>
#include <api_debug.h>
// #include <string.h>
// #include <stdio.h>


#define MAIN_TASK_STACK_SIZE    (1024 * 2)
#define MAIN_TASK_PRIORITY      0 
#define MAIN_TASK_NAME         "UART Test Task"

static HANDLE uartTaskHandle = NULL;

uint8_t rxbuffer[100];
void OnUart1ReceivedData(UART_Callback_Param_t param)
{
    memset(rxbuffer,0,sizeof(rxbuffer));
    uint32_t len = UART_Read(UART1,rxbuffer,param.length,200);
    Trace(1,"uart1 received data,length:%d,read:%d,data:%s",param.length,len,rxbuffer);
}

void uart_MainTask()
{
    UART_Config_t config = {
        .baudRate = UART_BAUD_RATE_115200,
        .dataBits = UART_DATA_BITS_8,
        .stopBits = UART_STOP_BITS_1,
        .parity   = UART_PARITY_NONE,
        .rxCallback = OnUart1ReceivedData,
    };
    uint32_t times = 0;
    UART_Init(UART1,config);
    config.rxCallback = NULL;
    UART_Init(UART2,config);

    while(1)
    {
        uint8_t temp[20];
        uint8_t buffer[50];

        sprintf(temp,"hello:%d,1234567890 1234567890\n", ++times);
        UART_Write(UART1,temp,strlen(temp)+1);
        Trace(1,"UART_Write:%s %p",temp, &times);
        memset(buffer,0,sizeof(buffer));
        uint32_t readLen = UART_Read(UART2,buffer,10,3000);
        Trace(1,"UART_Read uart2,readLen:%d,data:%s",readLen,buffer);
        OS_Sleep(200);
    }
}

void uart_Main(void)
{
    uartTaskHandle = OS_CreateTask(uart_MainTask ,
        NULL, NULL, MAIN_TASK_STACK_SIZE, MAIN_TASK_PRIORITY, 0, 0, MAIN_TASK_NAME);
    OS_SetUserMainHandle(&uartTaskHandle);
}

