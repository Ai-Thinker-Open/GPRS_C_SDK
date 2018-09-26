
#ifndef __API_HAL_GOUDA_INC_H
#define __API_HAL_GOUDA_INC_H

#include "stdint.h"
#include "stdbool.h"

typedef enum
{
    HAL_GOUDA_SPI_LINE_4 = 0,
    HAL_GOUDA_SPI_LINE_3 = 1,
    HAL_GOUDA_SPI_LINE_4_START_BYTE = 2,
    HAL_GOUDA_SPI_LINE_3_2_LANE =3 ,
} HAL_GOUDA_SPI_LINE_T;

typedef enum
{
    HAL_GOUDA_LCD_CS_0 = 0,
    HAL_GOUDA_LCD_CS_1 = 1,

    HAL_GOUDA_LCD_CS_QTY,

    HAL_GOUDA_LCD_MEMORY_IF = 2,
    HAL_GOUDA_LCD_IN_RAM = 3

} HAL_GOUDA_LCD_CS_T;


typedef enum
{
    /// 8-bit - RGB3:3:2 - 1cycle/1pixel - RRRGGGBB
    HAL_GOUDA_LCD_OUTPUT_FORMAT_8_BIT_RGB332    = 0,
    /// 8-bit - RGB4:4:4 - 3cycle/2pixel - RRRRGGGG/BBBBRRRR/GGGGBBBB
    HAL_GOUDA_LCD_OUTPUT_FORMAT_8_bit_RGB444    = 1,
    /// 8-bit - RGB5:6:5 - 2cycle/1pixel - RRRRRGGG/GGGBBBBB
    HAL_GOUDA_LCD_OUTPUT_FORMAT_8_bit_RGB565    = 2,
    /// 16-bit - RGB3:3:2 - 1cycle/2pixel - RRRGGGBBRRRGGGBB
    HAL_GOUDA_LCD_OUTPUT_FORMAT_16_bit_RGB332   = 4,
    /// 16-bit - RGB4:4:4 - 1cycle/1pixel - XXXXRRRRGGGGBBBB
    HAL_GOUDA_LCD_OUTPUT_FORMAT_16_bit_RGB444   = 5,
    /// 16-bit - RGB5:6:5 - 1cycle/1pixel - RRRRRGGGGGGBBBBB
    HAL_GOUDA_LCD_OUTPUT_FORMAT_16_bit_RGB565   = 6

} HAL_GOUDA_LCD_OUTPUT_FORMAT_T;

typedef union
{
    struct
    {
        HAL_GOUDA_LCD_CS_T              cs:2;
        uint32_t :2;
        HAL_GOUDA_LCD_OUTPUT_FORMAT_T   outputFormat:3;
        bool                            highByte:1;
        /// \c FALSE is active low, \c TRUE is active high.
        bool                            cs0Polarity:1;
        /// \c FALSE is active low, \c TRUE is active high.
        bool                            cs1Polarity:1;
        /// \c FALSE is active low, \c TRUE is active high.
        bool                            rsPolarity:1;
        /// \c FALSE is active low, \c TRUE is active high.
        bool                            wrPolarity:1;
        /// \c FALSE is active low, \c TRUE is active high.
        bool                            rdPolarity:1;
        uint32_t :3;
        /// Number of command to be send to the LCD command (up to 31)
        uint32_t                          nbCmd:5;
        uint32_t :3;
        /// Start command transfer only. Autoreset
        bool                            startCmdOnly:1;
        bool                            resetb:1;
        uint32_t :6;
    };

    /// Use this 'reg' field of the union
    /// to access this timing configuration as
    /// a 32 bits value than can be written to
    /// a register.
    uint32_t reg;
} HAL_GOUDA_LCD_CONFIG_T;


typedef struct
{
    /// top left point
    uint16_t  tlPX;
    uint16_t  tlPY;
    /// bottom right point
    uint16_t  brPX;
    uint16_t  brPY;

} HAL_GOUDA_WINDOW_T;

typedef struct
{
    /// \FALSE for a command, \TRUE for a data
    bool    isData;
    /// value of data or command
    uint16_t  value;
} HAL_GOUDA_LCD_CMD_T;

typedef enum
{
    HAL_GOUDA_OVL_LAYER_ID0,
    HAL_GOUDA_OVL_LAYER_ID1,
    HAL_GOUDA_OVL_LAYER_ID2,

    HAL_GOUDA_OVL_LAYER_ID_QTY
} HAL_GOUDA_OVL_LAYER_ID_T;


typedef enum
{
    HAL_GOUDA_IMG_FORMAT_RGB565,
    HAL_GOUDA_IMG_FORMAT_RGBA,
    HAL_GOUDA_IMG_FORMAT_UYVY,
    HAL_GOUDA_IMG_FORMAT_YUYV,
    HAL_GOUDA_IMG_FORMAT_IYUV
} HAL_GOUDA_IMG_FORMAT_T;

typedef enum
{
    /// exact color match
    HAL_GOUDA_CKEY_MASK_OFF = 0,
    /// disregard 1 LSBit of each color component for matching
    HAL_GOUDA_CKEY_MASK_1LSB = 1,
    /// disregard 2 LSBit of each color component for matching
    HAL_GOUDA_CKEY_MASK_2LSB = 3,
    /// disregard 3 LSBit of each color component for matching
    HAL_GOUDA_CKEY_MASK_3LSB = 7
} HAL_GOUDA_CKEY_MASK_T;


typedef struct
{
    ///@todo check with uint16_t* (?)
    uint32_t* addr;
    HAL_GOUDA_IMG_FORMAT_T fmt;
    /// buffer width in bytes including optional padding, when 0 will be calculated
    uint16_t  stride;
    HAL_GOUDA_WINDOW_T pos;
    uint8_t   alpha;
    bool    cKeyEn;
    uint16_t  cKeyColor;
    HAL_GOUDA_CKEY_MASK_T cKeyMask;
} HAL_GOUDA_OVL_LAYER_DEF_T;


typedef enum
{
    /// No error occured
    HAL_ERR_NO,

    /// A resource reset is required
    HAL_ERR_RESOURCE_RESET,

    /// An attempt to access a busy resource failed
    HAL_ERR_RESOURCE_BUSY,

    /// Timeout while trying to access the resource
    HAL_ERR_RESOURCE_TIMEOUT,

    /// An attempt to access a resource that is not enabled
    HAL_ERR_RESOURCE_NOT_ENABLED,

    /// Invalid parameter
    HAL_ERR_BAD_PARAMETER,

    /// Uart RX FIFO overflowed
    HAL_ERR_UART_RX_OVERFLOW,

    /// Uart TX FIFO overflowed
    HAL_ERR_UART_TX_OVERFLOW,
    HAL_ERR_UART_PARITY,
    HAL_ERR_UART_FRAMING,
    HAL_ERR_UART_BREAK_INT,
    HAL_ERR_TIM_RTC_NOT_VALID,
    HAL_ERR_TIM_RTC_ALARM_NOT_ENABLED,
    HAL_ERR_TIM_RTC_ALARM_NOT_DISABLED,

    /// Communication failure
    HAL_ERR_COMMUNICATION_FAILED,

    HAL_ERR_QTY
} HAL_ERR_T;


typedef void (*HAL_GOUDA_IRQ_HANDLER_T)(void);


#endif
