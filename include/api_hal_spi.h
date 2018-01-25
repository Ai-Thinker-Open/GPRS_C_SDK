#ifndef __API_HAL_SPI_H
#define __API_HAL_SPI_H

#ifdef __cplusplus
extern "C"{
#endif

#include "sdk_init.h"


/// \brief  Init SPI
/// \param  spiConfig, configuration of SPI
/// \return bool  
///         TRUE: init success
///			FALSE: init failed
// bool SPI_Init(SPI_ID_t spiN, SPI_Config_t spiConfig);
#define SPI_Init           CSDK_FUNC(SPI_Init)

/// \brief  Close SPI
/// \param  spiN, SPI ID
/// \return bool  
///         TRUE: close success
///			FALSE: close failed
// bool SPI_Close(SPI_ID_t spiN);
#define SPI_Close          CSDK_FUNC(SPI_Close)


/// \brief  SPI Write Data
/// \param  spiN, SPI ID
/// \param  csN, Chip Select
/// \param  data, the starting address of data to be sent
/// \param  length, the length of data to be sent,
///         when in API_HAL_SPI_DIRECT_POLLING mode, it can only up to 16 bytes
/// \return the length of data to be sent 
// uint32_t SPI_Write(SPI_ID_t spiN, const uint8_t *data, uint32_t length);
#define SPI_Write          CSDK_FUNC(SPI_Write)

/// \brief  SPI Read Data From FIFO
/// \param  spiN, SPI ID
/// \param  csN, Chip Select
/// \param  data, the starting address of receive buffer
/// \param  length, the length of data to be got,
///         when in API_HAL_SPI_DIRECT_POLLING mode, it can only up to 16 bytes
/// \return the length of data to be got 
// uint32_t SPI_Read(SPI_ID_t spiN, uint8_t *data, uint32_t length);
#define SPI_Read           CSDK_FUNC(SPI_Read)

/// \brief  Check If SPI Finished Sendind Data
/// \param  spiN, SPI ID
/// \param  csN, Chip Select
/// \return bool  
///         TRUE: SPI send data finished
///			FALSE: SPI send data unfinished
// bool SPI_IsTxDone(SPI_ID_t spiN);
#define SPI_IsTxDone       CSDK_FUNC(SPI_IsTxDone)

/// \brief  Check If SPI DMA Finished Sendind Data
/// \param  spiN, SPI ID
/// \param  csN, Chip Select
/// \return bool  
///         TRUE: SPI DMA send data finished
///			FALSE: SPI DMA send data unfinished
// bool SPI_IsTxDmaDone(SPI_ID_t spiN);
#define SPI_IsTxDmaDone    CSDK_FUNC(SPI_IsTxDmaDone)

/// \brief  Check If SPI DMA Finished Getting Data
/// \param  spiN, SPI ID
/// \param  csN, Chip Select
/// \return bool  
///         TRUE: SPI get data finished
///			FALSE: SPI get data unfinished
// bool SPI_IsRxDmaDone(SPI_ID_t spiN);
#define SPI_IsRxDmaDone    CSDK_FUNC(SPI_IsRxDmaDone)

/// \brief  Clear The SPI DMA Send-finished Flag
/// \param  spiN, SPI ID
// void SPI_ClearTxDmaDone(SPI_ID_t spiN);
#define SPI_ClearTxDmaDone CSDK_FUNC(SPI_ClearTxDmaDone)

/// \brief  Clear The SPI DMA Get-finished Flag
/// \param  spiN, SPI ID
// void SPI_ClearRxDmaDone(SPI_ID_t spiN);
#define SPI_ClearRxDmaDone CSDK_FUNC(SPI_ClearRxDmaDone)

/// \brief  Flush both SPI FIFOs
/// \param  spiN, SPI ID
// void SPI_FlushFIFOs(SPI_ID_t spiN);
#define SPI_FlushFIFOs     CSDK_FUNC(SPI_FlushFIFOs)


/// \brief  SPI Set Irq handler
/// \param  spiN, SPI ID
/// \param  csN, Chip Select
/// \param  handler, SPI Irq handler
// void SPI_SetIrqHandler(SPI_ID_t spiN, SPI_Irq_Handler_t handler);
#define SPI_SetIrqHandler  CSDK_FUNC(SPI_SetIrqHandler)

/// \brief  SPI Set Irq Enable status
/// \param  spiN, SPI ID
/// \param  csN, Chip Select
/// \param  status, SPI Irq Enable status
// void SPI_SetIrqMask(SPI_ID_t spiN, SPI_Irq_Flags_t irqMask);
#define SPI_SetIrqMask     CSDK_FUNC(SPI_SetIrqMask)





#ifdef __cplusplus
}
#endif

#endif


