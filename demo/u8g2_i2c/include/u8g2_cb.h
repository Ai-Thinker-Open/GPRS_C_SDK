#ifndef __U8G2_CB_H__
#define __U8G2_CB_H__

#include "stdint.h"
#include "api_hal_i2c.h"
#include "u8g2.h"

uint8_t u8g2_byte_cb(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr);
uint8_t u8g2_gpio_and_delay_cb(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr);

#endif