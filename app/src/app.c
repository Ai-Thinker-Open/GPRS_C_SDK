#include "api_hal_gpio.h"
#include "stdint.h"
#include "stdbool.h"
#include "api_debug.h"
#include "api_os.h"
#include "api_hal_pm.h"
#include "api_os.h"
#include "api_event.h"


#define MAIN_TASK_STACK_SIZE    (1024 * 2)
#define MAIN_TASK_PRIORITY      0 
#define MAIN_TASK_NAME         "MAIN Test Task"

#define TEST_TASK_STACK_SIZE    (1024 * 2)
#define TEST_TASK_PRIORITY      1
#define TEST_TASK_NAME         "GPIO Test Task"

static HANDLE mainTaskHandle = NULL;
static HANDLE secondTaskHandle = NULL;


#if 0

void OnPinFalling(GPIO_INT_callback_param_t* param)
{
    Trace(1,"OnPinFalling");
    switch(param->pin)
    {
        case GPIO_PIN2:
            Trace(1,"gpio2 detect falling edge!");
            GPIO_LEVEL statusNow;
            GPIO_Get(GPIO_PIN2,&statusNow);
            Trace(1,"gpio2 status now:%d",statusNow);
            break;
        default:
            break;
    }
}
void GPIO_TestTask()
{
    static GPIO_LEVEL ledBlueLevel = GPIO_LEVEL_LOW;
    GPIO_config_t gpioLedBlue;
    gpioLedBlue.mode         = GPIO_MODE_OUTPUT;
    gpioLedBlue.pin          = GPIO_PIN27;
    gpioLedBlue.defaultLevel = GPIO_LEVEL_LOW;
    
    GPIO_config_t gpioLedBlue2 = {
        .mode         = GPIO_MODE_OUTPUT,
        .pin          = GPIO_PIN28,
        .defaultLevel = GPIO_LEVEL_LOW
    };

    GPIO_config_t gpioINT = {
        .mode               = GPIO_MODE_INPUT_INT,
        .pin                = GPIO_PIN2,
        .defaultLevel       = GPIO_LEVEL_LOW,
        .intConfig.debounce = 50,
        .intConfig.type     = GPIO_INT_TYPE_FALLING_EDGE,
        .intConfig.callback = OnPinFalling
    };
    GPIO_config_t gpioInput = {
        .mode               = GPIO_MODE_INPUT,
        .pin                = GPIO_PIN30,
        .defaultLevel       = GPIO_LEVEL_HIGH,
        .intConfig.debounce = 0,
        .intConfig.type     = GPIO_INT_TYPE_MAX,
        .intConfig.callback = NULL
    };
    Trace(1,"GPIO Test main");
    GPIO_Init(gpioLedBlue);
    GPIO_Init(gpioLedBlue2);
    GPIO_Init(gpioInput);
    GPIO_Init(gpioINT);

    while(1)
    {
        GPIO_LEVEL status=0;
        ledBlueLevel = (ledBlueLevel==GPIO_LEVEL_HIGH)?GPIO_LEVEL_LOW:GPIO_LEVEL_HIGH;
        Trace(1,"ledBlueLevel toggle:%d",ledBlueLevel);
        GPIO_SetLevel(gpioLedBlue,ledBlueLevel);        //Set level
        GPIO_SetLevel(gpioLedBlue2,ledBlueLevel);        //Set level
        GPIO_GetLevel(gpioInput,&status);
        Trace(1,"GPIO30 status:%d",status);
        OS_Sleep(1000);                                  //Sleep 500 ms
    }
}

#endif

#if 1
int aaaa=0,bbbb=0,cccc=123;
void GPIO_TestTask()
{
    static GPIO_LEVEL ledBlueLevel = GPIO_LEVEL_LOW;

    GPIO_config_t gpioLedBlue = {
        .mode         = GPIO_MODE_OUTPUT,
        .pin          = GPIO_PIN25,
        .defaultLevel = GPIO_LEVEL_LOW
    };

    for(uint8_t i=0;i<POWER_TYPE_MAX;++i)
        PM_PowerEnable(i,true);

    for(int i=0;i<GPIO_PIN_MAX;++i)
    {
        gpioLedBlue.pin = i;
        GPIO_Init(gpioLedBlue);
    }
    
    while(1)
    {
        ledBlueLevel = (ledBlueLevel==GPIO_LEVEL_HIGH)?GPIO_LEVEL_LOW:GPIO_LEVEL_HIGH;
        Trace(1,"ledBlueLevel toggle:%d",ledBlueLevel);
        Trace(1,"aaaa:%d",aaaa);
        Trace(1,"bbbb:%d",bbbb);
        Trace(1,"cccc:%d",cccc);
        
        for(int i=0;i<GPIO_PIN_MAX;++i)
        {
            gpioLedBlue.pin = i;
            GPIO_SetLevel(gpioLedBlue,ledBlueLevel);        //Set level
        }
        
        OS_Sleep(500);                                  //Sleep 500 ms
    }
}

#endif


#if 0

void OnPinFalling(GPIO_INT_callback_param_t* param)
{
    Trace(1,"OnPinFalling");
    Trace(1,"gpio detect falling edge!pin:%d",param->pin);
}
void GPIO_TestTask()
{
    static GPIO_LEVEL ledBlueLevel = GPIO_LEVEL_LOW;

    GPIO_config_t gpioInt = {
        .mode               = GPIO_MODE_INPUT_INT,
        .pin                = GPIO_PIN2,
        .defaultLevel       = GPIO_LEVEL_LOW,
        .intConfig.debounce = 50,
        .intConfig.type     = GPIO_INT_TYPE_FALLING_EDGE,
        .intConfig.callback = OnPinFalling
    };
    for(int i=0;i<=7;++i)
    {
        gpioInt.pin = i;
        GPIO_Init(gpioInt);
    }
    
    while(1)
    {
        Trace(1,"idle");
        OS_Sleep(2000);
    }
}

#endif

#if 0
void GPIO_TestTask()
{
    static GPIO_LEVEL ledBlueLevel = GPIO_LEVEL_LOW;

    GPIO_config_t gpioLedBlue = {
        .mode         = GPIO_MODE_OUTPUT,
        .pin          = GPIO_PIN25,
        .defaultLevel = GPIO_LEVEL_LOW
    };

    PM_PowerEnable(POWER_TYPE_LCD,true);

    for(int i=14;i<=18;++i)
    {
        gpioLedBlue.pin = i;
        GPIO_Init(gpioLedBlue);
    }
    
    while(1)
    {
        ledBlueLevel = (ledBlueLevel==GPIO_LEVEL_HIGH)?GPIO_LEVEL_LOW:GPIO_LEVEL_HIGH;
        Trace(1,"ledBlueLevel toggle:%d",ledBlueLevel);
        Trace(1,"aaaa:%d",aaaa);
        Trace(1,"bbbb:%d",bbbb);
        Trace(1,"cccc:%d",cccc);
        for(int i=14;i<=18;++i)
        {
            gpioLedBlue.pin = i;
            GPIO_SetLevel(gpioLedBlue,ledBlueLevel);        //Set level
        }
        OS_Sleep(500);                                  //Sleep 500 ms
    }
}
#endif


void EventDispatch(API_Event_t* pEvent)
{
    switch(pEvent->id)
    {
        default:
            break;
    }
}

void MainTask(void *pData)
{
    API_Event_t* event=NULL;

    secondTaskHandle = OS_CreateTask(GPIO_TestTask,
        NULL, NULL, TEST_TASK_STACK_SIZE, TEST_TASK_PRIORITY, 0, 0, TEST_TASK_NAME);

    while(1)
    {
        if(OS_WaitEvent(mainTaskHandle, (void**)&event, OS_TIME_OUT_WAIT_FOREVER))
        {
            EventDispatch(event);
            OS_Free(event->pParam1);
            OS_Free(event->pParam2);
            OS_Free(event);
        }
    }
}


void app_Main()
{
    mainTaskHandle = OS_CreateTask(MainTask ,
        NULL, NULL, MAIN_TASK_STACK_SIZE, MAIN_TASK_PRIORITY, 0, 0, MAIN_TASK_NAME);
    OS_SetUserMainHandle(&mainTaskHandle);
}
