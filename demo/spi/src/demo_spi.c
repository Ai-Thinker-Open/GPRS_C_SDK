#include "stdint.h"
#include "stdbool.h"
#include "time.h"
#include "api_os.h"
#include "api_debug.h"
#include "api_hal_spi.h"
#include "api_hal_uart.h"
#include "string.h"

/// Only one DEMO can be selected for each compilation !!!
// #define SPI_DERECT_DEMO
// #define SPI_DMA_DEMO
#define SPI_IRQ_DEMO

#define MAIN_TASK_STACK_SIZE    (1024 * 2)
#define MAIN_TASK_PRIORITY      0
#define MAIN_TASK_NAME          "Spi Test Task"

static HANDLE mainTaskHandle = NULL;

uint8_t SPI_ReadOneByte(SPI_ID_t SPIx)
{
	uint8_t r_byte,temp = 0xff;
	SPI_Write(SPIx, &temp, 1);
	while(!SPI_IsTxDone(SPIx));
	SPI_Read(SPIx, &r_byte, 1);
	return r_byte;
}

uint32_t SPI_ReadWriteData(SPI_ID_t SPIx, uint8_t *s_data, uint8_t *r_data, uint32_t length)
{
	uint8_t *r_data_ptr = r_data;
	for(uint32_t i=0; i<length; i++)
	{
		SPI_Write(SPIx, s_data++, 1);
		while(!SPI_IsTxDone(SPIx));
		SPI_Read(SPIx, r_data_ptr++, 1);
	}
	return length;
}

uint32_t SPI_DmaReadWriteData(SPI_ID_t SPIx, uint8_t *s_addr, uint8_t *r_addr, uint32_t length)
{
	if(NULL != r_addr)
		SPI_Read(SPIx, r_addr, length);
	SPI_Write(SPIx, s_addr, length);
}

/// SPI Derect Polling Demo
#if defined(SPI_DERECT_DEMO)
void Spi_MainTask()
{
	uint8_t *send_buff,*rev_buff;
    SPI_Config_t SPI_CFG =
    {
	    .cs = SPI_CS_0,
        .txMode = SPI_MODE_DIRECT_POLLING,
        .rxMode = SPI_MODE_DIRECT_POLLING,
        .freq = 1000000,
        .line = SPI_LINE_4,
        .txOnly = false,
        .cpol = 0,
        .cpha = 0,
        .csActiveLow = true,
        .dataBits = SPI_DATA_BITS_8,
        .irqHandler = NULL,
        .irqMask = {0,0,0,0,0}
    };

    SPI_Init(SPI1, SPI_CFG);

	send_buff = OS_Malloc(10);
	rev_buff = OS_Malloc(10);
	memset(send_buff, 0, sizeof(send_buff));
	memset(rev_buff, 0, sizeof(rev_buff));

	strcpy(send_buff, "123456789");

	while(1)
	{
		SPI_ReadWriteData(SPI1, send_buff, rev_buff, 9);
		OS_Sleep(100);
	}
}

/// SPI DMA Polling Demo
#elif defined(SPI_DMA_DEMO)
void Spi_MainTask()
{
	uint8_t *send_buff,*rev_buff;
	SPI_Config_t SPI_CFG =
    {
	    .cs = SPI_CS_0,
        .txMode = SPI_MODE_DMA_POLLING,
        .rxMode = SPI_MODE_DMA_POLLING,
        .freq = 1000000,
        .line = SPI_LINE_4,
        .txOnly = false,
        .cpol = 0,
        .cpha = 0,
        .csActiveLow = true,
        .dataBits = SPI_DATA_BITS_8,
        .irqHandler = NULL,
        .irqMask = {0,0,0,0,0}
    };

    SPI_Init(SPI1, SPI_CFG);

	send_buff = OS_Malloc(4000);
	rev_buff = OS_Malloc(4000);
	memset(send_buff, 0xaa, sizeof(send_buff));
    memset(rev_buff, 0, sizeof(rev_buff));
    
	while(1)
	{
		SPI_DmaReadWriteData(SPI1, send_buff, rev_buff, sizeof(send_buff));
		while((!SPI_IsTxDmaDone(SPI1))&&(!SPI_IsRxDmaDone(SPI1)));
		SPI_ClearTxDmaDone(SPI1);
		SPI_ClearRxDmaDone(SPI1);
		OS_Sleep(100);
	}
}

/// SPI IRQ DEMO
#elif defined(SPI_IRQ_DEMO)
uint8_t spi_finish_flag = 0;
#define SPI_START "Spi Irq Test Start"
#define SPI_FINISH "Spi Irq Tx Finished"
void Spi_Handler(SPI_Irq_Flags_t Flags)
{
	if(Flags.txDmaDone)
	{
		SPI_ClearTxDmaDone(SPI1);
		spi_finish_flag = 1;
	}
}

void Spi_MainTask()
{
	uint8_t *send_buff;
	send_buff = OS_Malloc(4000);
	memset(send_buff, 0xab, 4000);

	OS_Sleep(5000);

	SPI_Config_t SPI_CFG =
    {
	    .cs = SPI_CS_0,
        .txMode = SPI_MODE_DMA_IRQ,
        .rxMode = SPI_MODE_DMA_IRQ,
        .freq = 1000000,
        .line = SPI_LINE_4,
        .txOnly = false,
        .cpol = 0,
        .cpha = 0,
        .csActiveLow = true,
        .dataBits = SPI_DATA_BITS_8,
        .irqHandler = Spi_Handler,
        .irqMask = {0,0,1,0,0}
    };
	SPI_Init(SPI1, SPI_CFG);

    Trace(2, SPI_START);
	SPI_Write(SPI1, send_buff, 4000);

	while(!spi_finish_flag)
	{
		OS_Sleep(100); //must sleep for a period of time,otherwise other tasks can't run
	}

	OS_Free(send_buff);
    SPI_Close(SPI1);
    Trace(2,SPI_FINISH);
    
	while(1)
	{
		OS_Sleep(3000);
	}
}
#endif

void spi_Main()
{
    mainTaskHandle = OS_CreateTask(Spi_MainTask ,
        NULL, NULL, MAIN_TASK_STACK_SIZE, MAIN_TASK_PRIORITY, 0, 0, MAIN_TASK_NAME);
    OS_SetUserMainHandle(&mainTaskHandle);
}


