/***************************************************
  This is a library for the MAX44009 Ambient Light Sensor

 ****************************************************/

#include "api_hal_i2c.h"

#define MAX44009_I2CADDR 0x4A

// Registers
#define MAX44009_INTSTATUS 0x00
#define MAX44009_INTENABLE 0x01
#define MAX44009_CONFIG 0x02
#define MAX44009_LUXHIGH 0x03
#define MAX44009_LUXLOW 0x04
#define MAX44009_UPPERTHRESHOLD 0x05
#define MAX44009_LOWERTHRESHOLD 0x06
#define MAX44009_THRESHOLDTIMER 0x07

class MikeNZ_MAX44009  {
 public:
  MikeNZ_MAX44009(uint8_t addr = MAX44009_I2CADDR);
  void begin(I2C_ID_t i2c);

  float readHighLimit(void);
  float readLowLimit(void);
  float readLux(void);

private:
  uint8_t _addr;
  I2C_ID_t _i2c;
  I2C_Config_t _i2cConfig;

  void writeRegister(const uint8_t reg, const uint8_t value);
  uint8_t readRegister(const uint8_t reg);
};
