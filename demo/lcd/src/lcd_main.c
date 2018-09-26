#include "stdint.h"
#include "stdbool.h"
#include "api_os.h"
#include "api_event.h"
#include "api_debug.h"
#include "api_hal_gpio.h"
#include "api_hal_pm.h"

#include "lcd.h"



#define AppMain_TASK_STACK_SIZE    (1024 * 2)
#define AppMain_TASK_PRIORITY      0
HANDLE mainTaskHandle  = NULL;


void EventDispatch(API_Event_t* pEvent)
{
    switch(pEvent->id)
    {
        case API_EVENT_ID_POWER_ON:
            break;
        case API_EVENT_ID_NO_SIMCARD:
            break;
        case API_EVENT_ID_NETWORK_REGISTERED_HOME:
        case API_EVENT_ID_NETWORK_REGISTERED_ROAMING:
            break;
        default:
            break;
    }
}



LCD_ROI_t roi = {
    .x = 0,
    .y = 0,
    .width = 100,
    .height = 200
};

LCD_OP_t lcd = {
    .Open              = NULL,
    .Close             = NULL,
    .SetContrast       = NULL,
    .SetBrightness     = NULL,
    .SetStandbyMode    = NULL,
    .Sleep             = NULL,
    .PartialOn         = NULL,
    .PartialOff        = NULL,
    .WakeUp            = NULL,
    .GetScreenInfo     = NULL,
    .SetPixel16        = NULL,
    .FillRect16        = NULL,
    .Blit16            = NULL,
    .Busy              = NULL,
    .SetDirRotation    = NULL,
    .SetDirDefault     = NULL,
    .GetStringId       = NULL,
    .GetLcdId          = NULL,
    .ChangeLcdSpiFreq  = NULL,
    .GoudaBltHdl       = NULL,
};
    
void Init_Interface()
{
    LCD_ili9341_Register(&lcd);
    lcd.Open();
	
}



void SSD1306_Task(void* param)
{
    // OS_Sleep(2000);
    Init_Interface();
    uint16_t color = 0x0000;
    while(1)
    {
        lcd.FillRect16(&roi,color);
        OS_Sleep(100);
        color += 20;
        
    }
}

void AppMainTask(void *pData)
{
    API_Event_t* event=NULL;
            
    OS_CreateTask(SSD1306_Task ,
        NULL, NULL, AppMain_TASK_STACK_SIZE, AppMain_TASK_PRIORITY+1, 0, 0, "lcd Task");
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
void lcd_Main(void)
{
    mainTaskHandle = OS_CreateTask(AppMainTask ,
        NULL, NULL, AppMain_TASK_STACK_SIZE, AppMain_TASK_PRIORITY, 0, 0, "main Task");
    OS_SetUserMainHandle(&mainTaskHandle);
}

