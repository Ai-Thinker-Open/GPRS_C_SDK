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


/// \brief  I2C_Error_t I2C_ReadByte(I2C_ID_t i2c, uint32_t slaveAddr, uint32_t memAddr, uint8_t* data);
/// \param  I2C_Bus_ID_t i2c, ID of the I2C bus
/// \param  uint32_t slaveAddr, Address of the slave from which we read a byte 
/// \param  uint32_t memAddr, memAddr Address in the memory map where to get the byte.
/// \param  uint8_t* pData, pData pointer to save one byte data
/// \return I2C_Error_t
#define I2C_ReadByte            CSDK_FUNC(I2C_ReadByte)


/// \brief  I2C_Error_t I2C_WriteByte(I2C_ID_t i2c, uint32_t slaveAddr, uint32_t memAddr, uint8_t data);
/// \param  I2C_Bus_ID_t i2c, ID of the I2C bus
/// \param  uint32_t slaveAdd, Address of the slave to which we write a byte 
/// \param  uint8_t* pData, one byte data that you want to write
/// \return I2C_Error_t
#define I2C_WriteByte            CSDK_FUNC(I2C_WriteByte)


/// \brief  I2C_Error_t I2C_ReadBytes(I2C_ID_t i2c, uint32_t slaveAddr, uint32_t memAddr, uint8_t* pData, uint32_t length);
/// \param  I2C_Bus_ID_t i2c, ID of the I2C bus
/// \param  uint32_t slaveAddr, Address of the slave from which we read data
/// \param  uint32_t memAddr, memAddr Address in the memory map where to get data.
/// \param  uint8_t* pData, buffer pointer to read the data
/// \param  uint32_t length, length of the data that you want to read
/// \return I2C_Error_t
#define I2C_ReadBytes            CSDK_FUNC(I2C_ReadBytes)


/// \brief  I2C_Error_t I2C_WriteBytes(I2C_ID_t i2c, uint32_t slaveAddr, uint32_t memAddr, CONST uint8_t* pData, uint32_t length);
/// \param  I2C_Bus_ID_t i2c, ID of the I2C bus
/// \param  uint32_t slaveAdd, Address of the slave to which we write a byte 
/// \param  uint8_t* pData, the pointer of buffer that you want to write
/// \param  uint32_t length, length of the data that you want to write
/// \return I2C_Error_t
#define I2C_WriteBytes            CSDK_FUNC(I2C_WriteBytes)


/// \brief  I2C_Error_t I2C_ReadRawByte(I2C_ID_t i2c, uint32_t CmdMask);
/// \param  I2C_Bus_ID_t i2c, ID of the I2C bus
/// \param  uint32_t CmdMask, can be I2C_MASTER_ACK|I2C_MASTER_RD|I2C_MASTER_STO|I2C_MASTER_WR|I2C_MASTER_STA
/// \return I2C_Error_t
/// \note   This function completes the 2 phase read cycle as defined in the SCCB
///         spec.  The actual commands for the I2C module must be:  Stop, Read, and
///         NACK. (The SCCB requires a NACK at the end of the read byte)
#define I2C_ReadRawByte            CSDK_FUNC(I2C_ReadRawByte)


/// \brief  I2C_Error_t I2C_WriteRawByte(I2C_ID_t i2c, uint8_t SendByte, uint32_t CmdMask);
/// \param  I2C_Bus_ID_t i2c, ID of the I2C bus
/// \param  uint8_t SendByte, one byte what you want to send
/// \param  uint32_t CmdMask, this is the command associated with this byte.  It must
///         be sent at the same time so the two actions (write to the data fifo and
///         issue the command) are done together, can be I2C_MASTER_ACK|I2C_MASTER_RD|
///         I2C_MASTER_STO|I2C_MASTER_WR|I2C_MASTER_STA
/// \return I2C_Error_t
/// \note   This function sends a single byte on the I2C interface
///         It is not designed for use in the real I2C protocol, but allows access
///         for non-standard usages such as SCCB for the Omnivision Camera control
#define I2C_WriteRawByte            CSDK_FUNC(I2C_WriteRawByte)


/// \brief  bool I2C_Close(I2C_ID_t i2c);
/// \param  I2C_Bus_ID_t i2c, ID of the I2C bus
/// \return BOOL
///         TRUE: close success
///			FALSE: close failed
#define I2C_Close            CSDK_FUNC(I2C_Close)


#endif 
