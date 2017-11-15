/*
 * @File  api_hal_gpio.h
 * @Brief 
 * 
 * @Author: Neucrack 
 * @Date: 2017-10-25 15:20:15 
 * @Last Modified by: Neucrack
 * @Last Modified time: 2017-10-28 10:59:37
 */

#ifndef __API_HAL_GPIO_H_
#define __API_HAL_GPIO_H_

#include <sdk_init.h>

/*
typedef enum{
  GPIO_PIN0 = 0,       //IO interruptible
  GPIO_PIN1,           //IO interruptible
  GPIO_PIN2,           //IO interruptible
  GPIO_PIN3,           //IO interruptible
  GPIO_PIN4,           //IO interruptible
  GPIO_PIN5,           //IO interruptible
  GPIO_PIN6,           //IO interruptible
  GPIO_PIN7,           //IO interruptible
  GPIO_PIN8,
  GPIO_PIN9,
  GPIO_PIN10,
  GPIO_PIN11,
  GPIO_PIN12,
  GPIO_PIN13,
  GPIO_PIN14,
  GPIO_PIN15,
  GPIO_PIN16,
  GPIO_PIN17,
  GPIO_PIN18,
  GPIO_PIN19,
  GPIO_PIN20,
  GPIO_PIN21,
  GPIO_PIN22,
  GPIO_PIN23,
  GPIO_PIN24,
  GPIO_PIN25,
  GPIO_PIN26,
  GPIO_PIN27,
  GPIO_PIN28,
  GPIO_PIN29,
  GPIO_PIN30,
  GPIO_PIN31,
  GPIO_PIN32,
  GPIO_PIN33,
  GPIO_PIN34,
  GPIO_PIN_MAX
}GPIO_PIN;

typedef enum{
  GPIO_MODE_OUTPUT = 0,   // output    mode
  GPIO_MODE_INPUT,        // input     mode
  GPIO_MODE_INPUT_INT,    // interrupt mode
  GPIO_MODE_MAX
}GPIO_MODE;

typedef enum{
  GPIO_LEVEL_LOW  = 0,
  GPIO_LEVEL_HIGH = 1
}GPIO_LEVEL;

typedef enum {
  GPIO_INT_TYPE_HIGH_LEVEL = 0,
  GPIO_INT_TYPE_LOW_LEVEL,
  GPIO_INT_TYPE_RISING_EDGE,
  GPIO_INT_TYPE_FALLING_EDGE,
  GPIO_INT_TYPE_RISING_FALLING_EDGE,
  GPIO_INT_TYPE_MAX
}GPIO_INT_TYPE;

typedef struct{
  GPIO_PIN pin;
}GPIO_INT_callback_param_t;

typedef void (*PCallbackINT)(GPIO_INT_callback_param_t* param);

typedef struct{
  uint16_t        debounce; //ms
  GPIO_INT_TYPE   type    ;
  PCallbackINT    callback;
}GPIO_INT_config_t;

typedef struct{
  GPIO_PIN          pin         ;
  GPIO_MODE         mode        ;
  GPIO_LEVEL        defaultLevel;
  GPIO_INT_config_t intConfig   ;
}GPIO_config_t;
*/

/**
  *
  *@attention only gpio0~7 ar interruptible
  *
  */ 

  // bool GPIO_Init(GPIO_config_t config);
  // void GPIO_GetConfig(GPIO_PIN pin,GPIO_config_t* config); 
  // bool GPIO_SetLevel(GPIO_config_t gpioConf, GPIO_LEVEL  level);
  // bool GPIO_Set(GPIO_PIN pin, GPIO_LEVEL  level);
  // bool GPIO_GetLevel(GPIO_config_t gpioConf, GPIO_LEVEL* level);
  // bool GPIO_Get(GPIO_PIN pin, GPIO_LEVEL* level);
  // bool GPIO_Close(GPIO_PIN pin);

/// \brief  bool GPIO_Init(GPIO_config_t config);
/// \param  GPIO_config_t config, set the GPIO_config_t struct
/// \return bool
#define GPIO_Init                                   CSDK_FUNC(GPIO_Init)

/// \brief  void GPIO_GetConfig(GPIO_PIN pin,GPIO_config_t* config);
/// \param  GPIO_PIN pin,  set the number of gpio
/// \param  GPIO_config_t* config, set the GPIO_config_t struct
/// \return void
#define GPIO_GetConfig                              CSDK_FUNC(GPIO_GetConfig)

/// \brief  bool GPIO_SetLevel(GPIO_config_t gpioConf, GPIO_LEVEL  level);
/// \param  GPIO_config_t gpioConf, set the GPIO_config_t struct
/// \param  GPIO_LEVEL  level,  set the  gpio level high or low
/// \return bool
#define GPIO_SetLevel                               CSDK_FUNC(GPIO_SetLevel)

/// \brief  bool GPIO_Set(GPIO_PIN pin, GPIO_LEVEL  level);
/// \param  GPIO_PIN pin,  set the number of gpio
/// \param  GPIO_LEVEL  level,  set the  gpio level high or low
/// \return bool
#define GPIO_Set                                    CSDK_FUNC(GPIO_Set)

/// \brief  bool GPIO_GetLevel(GPIO_config_t gpioConf, GPIO_LEVEL* level);
/// \param  GPIO_config_t gpioConf, set the GPIO_config_t struct
/// \param  GPIO_LEVEL*  level,  get the  gpio level high or low
/// \return bool
#define GPIO_GetLevel                               CSDK_FUNC(GPIO_GetLevel)

/// \brief  bool GPIO_Get(GPIO_PIN pin, GPIO_LEVEL* level);
/// \param  GPIO_PIN pin,  set the number of gpio
/// \param  GPIO_LEVEL*  level,  get the  gpio level high or low
/// \return bool
#define GPIO_Get                                    CSDK_FUNC(GPIO_Get)

/// \brief  bool GPIO_Close(GPIO_PIN pin);
/// \param  GPIO_PIN pin,  set the number of gpio
/// \return bool
#define GPIO_Close                                  CSDK_FUNC(GPIO_Close)

#endif
