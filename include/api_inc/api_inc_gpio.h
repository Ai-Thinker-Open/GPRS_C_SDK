#ifndef __API_INC_GPIO_H__
#define __API_INC_GPIO_H__

#include <cs_types.h>

/*#################      gpio      #################################*/
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

#endif
