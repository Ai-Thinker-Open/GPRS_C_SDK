
#include "api_hal_pm.h"
#include "api_hal_gpio.h"


void GPIO_EnablePower(GPIO_PIN pin, bool enable)
{
    if(pin>=GPIO_PIN0 && pin<=GPIO_PIN7)
        PM_PowerEnable(POWER_TYPE_VPAD,enable);
    else if(pin>=GPIO_PIN8 && pin<=GPIO_PIN13)
        PM_PowerEnable(POWER_TYPE_MMC,enable);
    else if(pin>=GPIO_PIN14 && pin<=GPIO_PIN18)
        PM_PowerEnable(POWER_TYPE_LCD,enable);
    else if(pin>=GPIO_PIN19 && pin<=GPIO_PIN24)
        PM_PowerEnable(POWER_TYPE_CAM,enable);
}


