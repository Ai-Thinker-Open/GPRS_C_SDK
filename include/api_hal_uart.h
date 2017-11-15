/*
 * @File  api_hal_uart.h
 * @Brief 
 * 
 * @Author: Neucrack 
 * @Date: 2017-10-27 18:45:16 
 * @Last Modified by: Neucrack
 * @Last Modified time: 2017-10-28 17:17:15
 */

#ifndef __API_HAL_UART_H__
#define __API_HAL_UART_H__

#include <sdk_init.h>

/*
typedef enum{
    UART1   = 1,
    UART2   = 2,
    UART_GPS = 2,
    UART_PORT_MAX
}UART_Port_t;

typedef enum{
    UART_BAUD_RATE_1200                     = 1200,
    UART_BAUD_RATE_2400                     = 2400,
    UART_BAUD_RATE_4800                     = 4800,
    UART_BAUD_RATE_9600                     = 9600,
    UART_BAUD_RATE_14400                    = 14400,
    UART_BAUD_RATE_19200                    = 19200,
    UART_BAUD_RATE_28800                    = 28800,
    UART_BAUD_RATE_33600                    = 33600,
    UART_BAUD_RATE_38400                    = 38400,
    UART_BAUD_RATE_57600                    = 57600,
    UART_BAUD_RATE_115200                   = 115200,
    UART_BAUD_RATE_230400                   = 230400,
    UART_BAUD_RATE_460800                   = 460800,
    UART_BAUD_RATE_921600                   = 921600,
    UART_BAUD_RATE_1300000                  = 1300000,
    UART_BAUD_RATE_1625000                  = 1625000,
    UART_BAUD_RATE_2166700                  = 2166700,
    UART_BAUD_RATE_3250000                  = 3250000,
    UART_BAUD_RATE_MAX                      = 3250001
}UART_Baud_Rate_t;

typedef enum{
    UART_DATA_BITS_7                        = 7,
    UART_DATA_BITS_8                        = 8,
    UART_DATA_BITS_MAX
}UART_Data_Bits_t;

typedef enum{
    UART_STOP_BITS_1                         = 1,
    UART_STOP_BITS_2                           ,
    UART_STOP_BITS_MAX
}UART_Stop_Bits_t;

typedef enum{
    UART_PARITY_NONE                          = 0,
    UART_PARITY_ODD                              ,
    UART_PARITY_EVEN                             ,
    UART_PARITY_SPACE                            ,
    UART_PARITY_MARK                             ,
    UART_PARITY_MAX
}UART_Parity_t;

typedef struct{
    UART_Port_t port;
    uint32_t    length;
}UART_Callback_Param_t;

typedef enum{
    UART_ERROR_RX_LINE_ERROR,
    UART_ERROR_RX_OVER_FLOW_ERROR,
    UART_ERROR_RX_PARITY_ERROR,
    UART_ERROR_RX_BREAK_INT_ERROR,
    UART_ERROR_RX_FRAMING_ERROR,
    UART_ERROR_TX_OVER_FLOW_ERROR,
    UART_ERROR_MAX
}UART_Error_t;

typedef void (*UART_Callback_t)(UART_Callback_Param_t param);
typedef void (*UART_Error_Callback_t)(UART_Error_t error);

typedef struct{
    UART_Baud_Rate_t      baudRate;
    UART_Data_Bits_t      dataBits;
    UART_Stop_Bits_t      stopBits;
    UART_Parity_t         parity;
    UART_Callback_t       rxCallback;
    UART_Error_Callback_t errorCallback;
    bool                  useEvent;
}UART_Config_t;
*/

// bool     UART_Init(UART_Port_t uartN, UART_Config_t config);
// uint32_t UART_Write(UART_Port_t uartN, uint8_t* dataToWrite, uint32_t length);
// uint32_t UART_Read(UART_Port_t uartN, uint8_t* dataToRead, uint32_t length, uint32_t timeOutMs);
// bool     UART_Close(UART_Port_t uartN);


/// \brief  bool     UART_Init(UART_Port_t uartN, UART_Config_t config);
/// \param  UART_Port_t uartN, set the uart port
/// \param  UART_Config_t config, set the uart config
/// \return bool
#define UART_Init                           CSDK_FUNC(UART_Init)

/// \brief  uint32_t UART_Write(UART_Port_t uartN, uint8_t* dataToWrite, uint32_t length);
/// \param  UART_Port_t uartN, set the uart port
/// \param  uint8_t* dataToWrite, the data you want to dataToWrite write
/// \param  uint32_t length, set the data length of dataToWrite
/// \return uint32_t
#define UART_Write                          CSDK_FUNC(UART_Write)

/// \brief  uint32_t UART_Read(UART_Port_t uartN, uint8_t* dataToRead, uint32_t length, uint32_t timeOutMs);
/// \param  UART_Port_t uartN, set the uart port
/// \param  uint8_t* dataToRead, the read data buf dataToRead 
/// \param  uint32_t length, set the max length of dataToRead
/// \param  uint32_t timeOutMs, set timeout in ms
/// \return uint32_t
#define UART_Read                           CSDK_FUNC(UART_Read)

/// \brief  bool     UART_Close(UART_Port_t uartN);
/// \param  UART_Port_t uartN, set the uart port
/// \return bool
#define UART_Close                          CSDK_FUNC(UART_Close)

#endif //__API_HAL_UART_H__
