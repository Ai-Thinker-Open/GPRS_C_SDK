#ifndef __API_HAL_I2C_H__
#define __API_HAL_I2C_H__

#include <sdk_init.h>
 

/// \brief  bool I2C_Init(I2C_ID_t i2c, I2C_Config_t config);
/// \param  I2C_Bus_ID_t i2c, ID of the I2C bus
/// \param  I2C_Bps_t Bpsx, the speed of I2C
/// \return BOOL  
///         TRUE: init success
///			FALSE: init failed
/// \note   when I2C1 is used, the i2c1 pin must be pulled up
#define I2C_Init            CSDK_FUNC(I2C_Init)

// I2C_Error_t         (*I2C_Transmit)(I2C_ID_t i2c, uint16_t slaveAddr, uint8_t* pData, uint16_t length, uint32_t timeOut);
// I2C_Error_t         (*I2C_Receive)(I2C_ID_t i2c, uint16_t slaveAddr, uint8_t* pData, uint16_t length, uint32_t timeOut);
// I2C_Error_t         (*I2C_WriteMem)(I2C_ID_t i2c, uint16_t slaveAddr, uint32_t memAddr, uint8_t memSize, uint8_t* pData, uint16_t length, uint32_t timeOut);
// I2C_Error_t         (*I2C_ReadMem)(I2C_ID_t i2c, uint16_t slaveAddr, uint32_t memAddr, uint8_t memSize, uint8_t* pData, uint16_t length, uint32_t timeOut);
// I2C_Error_t         (*I2C_WriteRawByte)(I2C_ID_t i2c, uint8_t SendByte, I2C_CMD_Mask_t CmdMask, uint32_t timeOut);
// uint8_t             (*I2C_ReadRawByte)(I2C_ID_t i2c, I2C_CMD_Mask_t CmdMask, uint32_t timeOut);

#define I2C_Transmit           CSDK_FUNC(I2C_Transmit)
#define I2C_Receive            CSDK_FUNC(I2C_Receive)
#define I2C_WriteMem           CSDK_FUNC(I2C_WriteMem)
#define I2C_ReadMem            CSDK_FUNC(I2C_ReadMem)
#define I2C_WriteRawByte       CSDK_FUNC(I2C_WriteRawByte)
#define I2C_ReadRawByte        CSDK_FUNC(I2C_ReadRawByte)


/// \brief  bool I2C_Close(I2C_ID_t i2c);
/// \param  I2C_Bus_ID_t i2c, ID of the I2C bus
/// \return BOOL
///         TRUE: close success
///			FALSE: close failed
#define I2C_Close            CSDK_FUNC(I2C_Close)


#endif 
