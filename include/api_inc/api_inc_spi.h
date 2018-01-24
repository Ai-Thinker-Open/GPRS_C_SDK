#ifndef __API_INC_SPI_H
#define __API_INC_SPI_H

#include "stdint.h"
#include "stdbool.h"


#define SPI_FREQ_MAX 13000000

/// SPI ID
typedef enum
{
    SPI1      = 1 ,
    SPI2      = 2 ,
} SPI_ID_t;

/// Chip Select
typedef enum
{
    SPI_CS_0  = 0 ,
    SPI_CS_1      ,
    SPI_CS_MAX
} SPI_CS_t;

/// SPI Transfer Mode
typedef enum
{
    /// Direct polling: The application sends/receives the data directly to/from
    /// the hardware module. The number of bytes actually sent/received is
    /// returned.
    SPI_MODE_DIRECT_POLLING                   ,

	/// Direct IRQ: The application sends/receives the data directly to/from
    /// the hardware module. The number of bytes actually sent/received is
    /// returned. An Irq can be generated when the Tx/Rx FIFO reaches the
    /// pre-programmed level.
	SPI_MODE_DIRECT_IRQ                       ,

    /// DMA polling: The application sends/receives the data through a DMA to
    /// the hardware module. The function returns 0 when no DMA channel is
    /// available. No bytes are sent. The function returns the number of bytes
    /// to send when a DMA resource is available. They will all be sent. A
    /// function allows to check if the previous DMA transfer is finished. No
    /// new DMA transfer in the same direction will be allowed before the end
    /// of the previous transfer.
    SPI_MODE_DMA_POLLING                      ,

	/// DMA IRQ: The application sends/receives the data through a DMA to the
    /// hardware module. The function returns 0 when no DMA channel is
    /// available. No bytes are sent. The function returns the number of bytes
    /// to send when a DMA resource is available. They will all be sent. An
    /// IRQ is generated when the current transfer is finished. No new DMA
    /// transfer in the same direction will be allowed before the end of the
    /// previous transfer.
	SPI_MODE_DMA_IRQ                          ,

	SPI_MODE_MAX
} SPI_Mode_t;

typedef enum{
	
	SPI_LINE_3  = 3    ,   ///  Half-Duplex Mode
	SPI_LINE_4  = 4    ,	///  Full-Duplex Mode
} SPI_Line_t;

typedef enum{
	SPI_DATA_BITS_8 = 8 ,
	SPI_DATA_BITS_16 = 16,
} SPI_Data_Bits_t;

/// Irq Enable status
typedef struct
{
	/// Rx FIFO overflow
	bool rxOverflow;
	/// Tx send finish
	bool txFinish;
	/// Tx DMA send finish
	bool txDmaDone;
	/// Rx send finish
	bool rxFinish;
	/// Rx DMA send finish
	bool rxDmaDone;
} SPI_Irq_Flags_t;

/// Irq handler
typedef void (*SPI_Irq_Handler_t)(SPI_Irq_Flags_t);

/// SPI Init Parameters
typedef struct
{
	/// Select the Chip Select
	SPI_CS_t cs;

	/// Emission transfer mode
	SPI_Mode_t txMode;

	/// Reception transfer mode
	SPI_Mode_t rxMode;

	/// SPI clock frequency
	/// up to 13Mbits/s (SPI_FREQ_MAX)
	uint32_t   freq;

	/// SPI transfer line mode
	SPI_Line_t line;

	/// SPI tx only
	/// false: tx only disable
	/// true:  tx only enable
	bool txOnly;

	/// SPI Clk Polarity
	/// 0 when SPI is free,clk line is in low level
	/// 1 when SPI is free,clk line is in high level
	uint8_t cpol;

	/// SPI Clk Phase
	/// 0 SPI sampling data at the first edge
	/// 1 SPI sampling data at the second edge
	uint8_t cpha;

	/// SPI Cs Active Polarity
	/// true: Cs Active Low
	/// false:  Cs Active High
	bool csActiveLow;

	/// SPI framesize
	SPI_Data_Bits_t dataBits;


	SPI_Irq_Handler_t irqHandler;

	SPI_Irq_Flags_t   irqMask;

} SPI_Config_t;


#endif

