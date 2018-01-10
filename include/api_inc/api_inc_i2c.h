#ifndef _API_INC_I2C_H
#define _API_INC_I2C_H

// <CmdMask> list
#define I2C_MASTER_ACK              (1<<0)
#define I2C_MASTER_RD               (1<<4)
#define I2C_MASTER_STO              (1<<8)
#define I2C_MASTER_WR               (1<<12)
#define I2C_MASTER_STA              (1<<16)

typedef enum
{
    I2C1 = 1 ,
    I2C2 = 2 ,
    I2C3 = 3 ,
    I2C_ID_MAX
} I2C_ID_t;

typedef enum
{
    I2C_FREQ_100K,
    I2C_FREQ_400K,
    I2C_FREQ_MAX
} I2C_FREQ_t;

typedef struct{
    I2C_FREQ_t freq;
} I2C_Config_t;

typedef enum{
    I2C_ERROR_NONE = 0     ,
    /// A resource reset is required
    I2C_ERROR_RESOURCE_RESET,
    /// An attempt to access a busy resource failed
    I2C_ERROR_RESOURCE_BUSY,
    /// Timeout while trying to access the resource
    I2C_ERROR_RESOURCE_TIMEOUT,
    /// An attempt to access a resource that is not enabled
    I2C_ERROR_RESOURCE_NOT_ENABLED,
    /// Invalid parameter
    I2C_ERROR_BAD_PARAMETER,
    /// Communication failure
    I2C_ERROR_COMMUNICATION_FAILED  = 14,

    I2C_ERROR_MAX
} I2C_Error_t;

#endif
