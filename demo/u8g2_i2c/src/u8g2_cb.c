#include "u8g2_cb.h"

#define I2C_OLED I2C2
#define I2C_TIMEOUT 10000

static I2C_Config_t config;

uint8_t u8g2_byte_cb(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
    static uint8_t buffer[32];
    static uint8_t buf_idx;
    uint8_t *data;

    switch (msg)
    {
    case U8X8_MSG_BYTE_SEND: // 发送数据
        data = (uint8_t *)arg_ptr;
        while (arg_int > 0)
        {
            buffer[buf_idx++] = *data;
            data++;
            arg_int--;
        }
        break;
    case U8X8_MSG_BYTE_INIT: // 初始化I2C
        config.freq = I2C_FREQ_400K;
        I2C_Init(I2C_OLED, config);
        break;
    case U8X8_MSG_BYTE_SET_DC: // D/C选择
        break;
    case U8X8_MSG_BYTE_START_TRANSFER: // 开始传送
        buf_idx = 0;
        break;
    case U8X8_MSG_BYTE_END_TRANSFER: // 结束传送
        I2C_Transmit(I2C_OLED, u8x8_GetI2CAddress(u8x8) >> 1, buffer, buf_idx, I2C_TIMEOUT);
        break;
    default:
        return 0;
    }
    return 1;
}

uint8_t u8g2_gpio_and_delay_cb(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
    return 1;
}
