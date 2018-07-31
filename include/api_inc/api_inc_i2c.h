#ifndef _API_INC_I2C_H
#define _API_INC_I2C_H

#define I2C_DEFAULT_TIME_OUT 10  //10ms


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

typedef enum{
    I2C_CMD_MASK_MASTER_NACK   =     (1<<0)  ,
    I2C_CMD_MASK_MASTER_READ   =     (1<<4)  ,
    I2C_CMD_MASK_MASTER_STOP   =     (1<<8)  ,
    I2C_CMD_MASK_MASTER_WRITE  =     (1<<12) ,
    I2C_CMD_MASK_MASTER_START  =     (1<<16) ,
    I2C_CMD_MASK_MAX
} I2C_CMD_Mask_t;
 

#endif
