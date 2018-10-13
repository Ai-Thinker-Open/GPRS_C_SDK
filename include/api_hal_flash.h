#ifndef __API_HAL_FLASH_H
#define __API_HAL_FLASH_H


#include "sdk_init.h"


#define HAL_SPI_FLASH_OFFSET(address) ((uint32_t)(address)&0x00ffffff)
#define HAL_SPI_FLASH_UNCACHE_ADDRESS(offset) ((void *)(HAL_SPI_FLASH_OFFSET(offset) | 0xa8000000))


// =============================================================================
// hal_SpiFlashLock/Unlock
// -----------------------------------------------------------------------------
/// Apply/Release access to spi flash.
// =============================================================================
// void hal_SpiFlashLock(void);
#define hal_SpiFlashLock CSDK_FUNC(hal_SpiFlashLock)
// void hal_SpiFlashUnlock(void);
#define hal_SpiFlashUnlock CSDK_FUNC(hal_SpiFlashUnlock)

// =============================================================================
// hal_SpiFlashOpen
// -----------------------------------------------------------------------------
/// Initialize SPI FLASH (configure FLASH clock, read ID and etc). Will
/// configure clock, SPI controller and reset FLASH.
// =============================================================================
// void hal_SpiFlashOpen(void);
#define hal_SpiFlashOpen CSDK_FUNC(hal_SpiFlashOpen)
// void hal_SpiFlashClose();
#define hal_SpiFlashClose CSDK_FUNC(hal_SpiFlashClose)

// =============================================================================
// hal_SpiFlashGetID
// -----------------------------------------------------------------------------
/// Get FLASH ID (it will be read once at Init).
/// id[23:16]: capacity
/// id[15:8]: memory type
/// id[7:0]: manufacturer
// =============================================================================
// uint32_t hal_SpiFlashGetID(void);
#define hal_SpiFlashGetID CSDK_FUNC(hal_SpiFlashGetID)

// =============================================================================
// hal_SpiFlashSetLockCallback/hal_SpiFlashSetUnlockCallback/hal_SpiFlashSetCheckIrq
// -----------------------------------------------------------------------------
/// Set FLASH multi-task lock/unlock callback, and whether to check pending IRQ
/// during wait WIP finish. Usually, they will be called once after OS is
/// started and before multi-task FLASH operations.
// =============================================================================
// void hal_SpiFlashSetLockCallback(HAL_FLASH_CALLBACK_FUNCTION_T lock, void *param);
#define hal_SpiFlashSetLockCallback CSDK_FUNC(hal_SpiFlashSetLockCallback)
// void hal_SpiFlashSetUnlockCallback(HAL_FLASH_CALLBACK_FUNCTION_T unlock, void *param);
#define hal_SpiFlashSetUnlockCallback CSDK_FUNC(hal_SpiFlashSetUnlockCallback)
// void hal_SpiFlashSetCheckIrq(bool checkirq);
#define hal_SpiFlashSetCheckIrq CSDK_FUNC(hal_SpiFlashSetCheckIrq)

// =============================================================================
// hal_SpiFlashMapUncacheAddress/hal_SpiFlashOffset
// -----------------------------------------------------------------------------
/// Map between FLASH offset and CPU address.
// =============================================================================
// void *hal_SpiFlashMapUncacheAddress(uint32_t flashAddress);
#define hal_SpiFlashMapUncacheAddress CSDK_FUNC(hal_SpiFlashMapUncacheAddress)
// uint32_t hal_SpiFlashOffset(void *address);
#define hal_SpiFlashOffset CSDK_FUNC(hal_SpiFlashOffset)

// =============================================================================
// hal_SpiFlashWrite
// -----------------------------------------------------------------------------
/// Write data to FLASH. To get correct result, it is needed to erase FLASH
/// before write.
///
/// Optionally, it can check IRQ during wait. When there are pending IRQ,
/// SUSPEND Will be called before enable IRQ, and when the task of function
/// is scheduled again, it will call RESUME.
///
/// It will return false only when parameters are invalid.
// =============================================================================
// bool hal_SpiFlashWrite(uint32_t flashAddress, const void *data, unsigned size);
#define hal_SpiFlashWrite CSDK_FUNC(hal_SpiFlashWrite)

// =============================================================================
// hal_SpiFlashErase
// -----------------------------------------------------------------------------
/// Erase the FLASH range. The range should be sector aligned. It will return
/// false only when parameters are invalid.
// =============================================================================
// bool hal_SpiFlashErase(uint32_t flashAddress, unsigned size);
#define hal_SpiFlashErase CSDK_FUNC(hal_SpiFlashErase)

// =============================================================================
// hal_SpiFlashChipErase
// -----------------------------------------------------------------------------
/// May be it only needed for RAMRUN
// =============================================================================
// void hal_SpiFlashChipErase(void);
#define hal_SpiFlashChipErase CSDK_FUNC(hal_SpiFlashChipErase)

// =============================================================================
// hal_SpiFlashSleep/hal_SpiFlashWakeup/hal_SpiFlashReset
// -----------------------------------------------------------------------------
/// Sleep will save FLASH power consumption.
// =============================================================================
// void hal_SpiFlashSleep(void);
#define hal_SpiFlashSleep CSDK_FUNC(hal_SpiFlashSleep)
// void hal_SpiFlashWakeup(void);
#define hal_SpiFlashWakeup CSDK_FUNC(hal_SpiFlashWakeup)
// void hal_SpiFlashReset(void);
#define hal_SpiFlashReset CSDK_FUNC(hal_SpiFlashReset)

// =============================================================================
// hal_SpiFlashGetPageSize/Sector/Block/Capacity
// -----------------------------------------------------------------------------
/// Get size information
// =============================================================================
// unsigned hal_SpiFlashGetPageSize(void);
#define hal_SpiFlashGetPageSize CSDK_FUNC(hal_SpiFlashGetPageSize)
// unsigned hal_SpiFlashGetSectorSize(void);
#define hal_SpiFlashGetSectorSize CSDK_FUNC(hal_SpiFlashGetSectorSize)
// unsigned hal_SpiFlashGetBlockSize(void);
#define hal_SpiFlashGetBlockSize CSDK_FUNC(hal_SpiFlashGetBlockSize)
// unsigned hal_SpiFlashGetSize(void);
#define hal_SpiFlashGetSize CSDK_FUNC(hal_SpiFlashGetSize)

// =============================================================================
// hal_SpiFlashReadStatusRegister/hal_SpiFlashWriteStatusRegister
// -----------------------------------------------------------------------------
/// Read and Write SR1 and SR2.
// =============================================================================
// uint16_t hal_SpiFlashReadStatusRegister(void);
#define hal_SpiFlashReadStatusRegister CSDK_FUNC(hal_SpiFlashReadStatusRegister)
// void hal_SpiFlashWriteStatusRegister(uint16_t val, uint16_t mask);
#define hal_SpiFlashWriteStatusRegister CSDK_FUNC(hal_SpiFlashWriteStatusRegister)

// =============================================================================
// hal_SpiFlashReadStatusRegisterEx/hal_SpiFlashWriteStatusRegisterEx/hal_SpiFlashWriteVolatileStatusRegisterEx
// -----------------------------------------------------------------------------
/// Read and write SR1/SR2/SR3. Not all FLASH support this. 'n' should be 1/2/3.
// =============================================================================
// uint8_t hal_SpiFlashReadStatusRegisterEx(uint8_t n);
#define hal_SpiFlashReadStatusRegisterEx CSDK_FUNC(hal_SpiFlashReadStatusRegisterEx)
// void hal_SpiFlashWriteStatusRegisterEx(uint8_t n, uint8_t val);
#define hal_SpiFlashWriteStatusRegisterEx CSDK_FUNC(hal_SpiFlashWriteStatusRegisterEx)
// void hal_SpiFlashWriteVolatileStatusRegisterEx(uint8_t n, uint8_t val);
#define hal_SpiFlashWriteVolatileStatusRegisterEx CSDK_FUNC(hal_SpiFlashWriteVolatileStatusRegisterEx)

// =============================================================================
// hal_SpiFlashSecurityErase/Write/Read
// -----------------------------------------------------------------------------
/// Operations for security register. Security register feature won't be checked.
/// Caller should provide address[23:0] according to datasheet.
// =============================================================================
// void hal_SpiFlashSecurityErase(uint32_t address);
#define hal_SpiFlashSecurityErase CSDK_FUNC(hal_SpiFlashSecurityErase)
// void hal_SpiFlashSecurityWrite(uint32_t address, const void *data, unsigned size);
#define hal_SpiFlashSecurityWrite CSDK_FUNC(hal_SpiFlashSecurityWrite)
// void hal_SpiFlashSecurityRead(uint32_t address, void *data, unsigned size);
#define hal_SpiFlashSecurityRead CSDK_FUNC(hal_SpiFlashSecurityRead)


#endif

