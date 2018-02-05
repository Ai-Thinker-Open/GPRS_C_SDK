#ifndef __API_INC_UART_H__
#define __API_INC_UART_H__

#include <cs_types.h>

/*#################      uart      #################################*/
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
    char *buf;//the receive buf
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
#endif
