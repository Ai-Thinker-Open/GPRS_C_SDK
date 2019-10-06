/***************************************************
  This is a library for the MAX44009 Ambient Light Sensor

  BSD license, all text above must be included in any redistribution
 ****************************************************/

#include "MikeNZ_MAX44009.h"
#include "api_debug.h"
#include "api_os.h"

MikeNZ_MAX44009::MikeNZ_MAX44009(uint8_t i2caddr) : _addr(i2caddr) {
}

void MikeNZ_MAX44009::begin(I2C_ID_t i2c)
{
    _i2c = i2c;
    _i2cConfig.freq = I2C_FREQ_100K;
    I2C_Init(_i2c, _i2cConfig);
    writeRegister(MAX44009_CONFIG, 0x00);
}

float MikeNZ_MAX44009::readHighLimit()
{
    uint8_t highByte = readRegister(MAX44009_UPPERTHRESHOLD);
    uint8_t exponent = highByte >> 4; // Upper 4 bits
    uint8_t mantissa = ((highByte & 0x0F) << 4) | 0x0F;

    return (float)(((0x00000001 << exponent) * (float)mantissa) * 0.045);
}

float MikeNZ_MAX44009::readLowLimit()
{
    uint8_t highByte = readRegister(MAX44009_LOWERTHRESHOLD);
    uint8_t exponent = highByte >> 4; // Upper 4 bits
    uint8_t mantissa = ((highByte & 0x0F) << 4) | 0x0F;

    return (float)(((0x00000001 << exponent) * (float)mantissa) * 0.045);
}

float MikeNZ_MAX44009::readLux()
{
    uint8_t highByte = readRegister(MAX44009_LUXHIGH);
    uint8_t lowByte = readRegister(MAX44009_LUXLOW);

    uint8_t exponent = highByte >> 4; // Upper 4 bits
    uint8_t mantissa = ((highByte & 0x0F) << 4) | (lowByte & 0x0F);

    return (float)(((0x00000001 << exponent) * (float)mantissa) * 0.045);
}

void MikeNZ_MAX44009::writeRegister(const uint8_t reg, const uint8_t value)
{
    uint8_t data[2] = {
        reg,                 // Register
        value & 0xFF         // Value
    };
    I2C_Error_t error = I2C_Transmit(_i2c, _addr, &data[0], 2, I2C_DEFAULT_TIME_OUT);
    if (error != I2C_ERROR_NONE)
    {
        Trace(1, "MikeNZ_MAX44009::writeRegister transmit error: 0X%02x", error);
    }
}

uint8_t MikeNZ_MAX44009::readRegister(uint8_t reg) {
    I2C_Error_t error = I2C_Transmit(_i2c, _addr, &reg, 1, I2C_DEFAULT_TIME_OUT);
    if (error != I2C_ERROR_NONE)
    {
        Trace(1, "MikeNZ_MAX44009::readRegister transmit error: 0X%02x", error);
    }

    uint8_t res = 0;
    error = I2C_Receive(_i2c, _addr, &res, 1, I2C_DEFAULT_TIME_OUT);
    if (error != I2C_ERROR_NONE)
    {
        Trace(1, "Adafruit_INA219::wireReadRegister recieve error: 0X%02x", error);
    }

    return res;
}
