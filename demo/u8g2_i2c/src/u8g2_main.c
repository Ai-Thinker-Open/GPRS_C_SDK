#include "stdint.h"
#include "stdbool.h"
#include "api_os.h"
#include "api_event.h"
#include "api_debug.h"

#include "u8g2_cb.h"

#define AppMain_TASK_STACK_SIZE (1024 * 2)
#define AppMain_TASK_PRIORITY 1

HANDLE mainTaskHandle = NULL;
static u8g2_t u8g2;

void EventDispatch(API_Event_t *pEvent)
{
    switch (pEvent->id)
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

void AppMainTask(void *pData)
{
    API_Event_t *event = NULL;

    u8g2_Setup_ssd1306_i2c_128x64_noname_1(&u8g2, U8G2_R0, u8g2_byte_cb, u8g2_gpio_and_delay_cb);
    u8g2_InitDisplay(&u8g2);
    u8g2_SetPowerSave(&u8g2, 0);
    u8g2_SetFont(&u8g2, u8g2_font_6x12_tr);

    u8g2_FirstPage(&u8g2);
    do
    {
        u8g2_DrawStr(&u8g2, 0, 12, "hello world!");
        u8g2_SendBuffer(&u8g2);
    } while (u8g2_NextPage(&u8g2));

    while (1)
    {
        if (OS_WaitEvent(mainTaskHandle, (void **)&event, OS_TIME_OUT_WAIT_FOREVER))
        {
            EventDispatch(event);
            OS_Free(event->pParam1);
            OS_Free(event->pParam2);
            OS_Free(event);
        }
    }
}
void u8g2_i2c_Main(void)
{
    mainTaskHandle = OS_CreateTask(AppMainTask,
                                   NULL, NULL, AppMain_TASK_STACK_SIZE, AppMain_TASK_PRIORITY, 0, 0, "init Task");
    OS_SetUserMainHandle(&mainTaskHandle);
}