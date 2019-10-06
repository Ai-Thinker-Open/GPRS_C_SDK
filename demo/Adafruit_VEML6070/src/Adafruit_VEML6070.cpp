
#include "stdbool.h"
#include "stdint.h"
#include "stdio.h"
#include "string.h"

#include "api_os.h"
#include "api_debug.h"
#include "api_event.h"

#include <Adafruit_VEML6070.h>

#define MAIN_TASK_STACK_SIZE    (2048 * 2)
#define MAIN_TASK_PRIORITY      0
#define MAIN_TASK_NAME          "Main Test Task"

#define SECOND_TASK_STACK_SIZE    (2048 * 2)
#define SECOND_TASK_PRIORITY      1
#define SECOND_TASK_NAME          "Second Test Task"

static HANDLE mainTaskHandle = NULL;
static HANDLE secondTaskHandle = NULL;

void EventDispatch(API_Event_t* pEvent)
{
    switch(pEvent->id)
    {
        default:
            break;
    }
}

uint8_t toRiskLevel(uint16_t uv)
{
    // Mapping table for VEML6070_1_T, Rset = 270K
    // http://www.vishay.com/docs/84310/designingveml6070.pdf page 5
    //uint16_t mapping[4] = {561, 1121, 1495, 2205};

    // Mapping table for VEML6070_4_T, Rset = 270K
    uint16_t mapping[4] = {2241, 4482, 5976, 8217};

    uint8_t i;
    for (i = 4; i > 0; i--)
    {
        if (uv >= mapping[i - 1])
        {
            return i;
        }
    }
    return 0;
}

void SecondTask(void *pData)
{
    OS_Sleep(3000);

    // Get the UV reading the VEML6070 UVA Light Sensor
    Adafruit_VEML6070 veml6070 = Adafruit_VEML6070();
    veml6070.begin(I2C2, VEML6070_4_T);

    while (1)
    {
        uint16_t uv = veml6070.readUV();
        Trace(2, "UV: %d", uv);

        uint8_t risk = toRiskLevel(uv);
        switch (risk) {
            case 0:
                Trace(2, "Risk Level: low");
                break;
            case 1:
                Trace(2, "Risk Level: moderate");
                break;
            case 2:
                Trace(2, "Risk Level: high");
                break;
            case 3:
                Trace(2, "Risk Level: very high");
                break;
            case 4:
                Trace(2, "Risk Level: extreme");
                break;
        }

        OS_Sleep(3000);
    }
}

void MainTask(void *pData)
{
    API_Event_t* event=NULL;

    secondTaskHandle = OS_CreateTask(SecondTask,
        NULL, NULL, SECOND_TASK_STACK_SIZE, SECOND_TASK_PRIORITY, 0, 0, SECOND_TASK_NAME);

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


extern "C"
{

void Adafruit_VEML6070_Main(void)
{
    mainTaskHandle = OS_CreateTask(MainTask,
        NULL, NULL, MAIN_TASK_STACK_SIZE, MAIN_TASK_PRIORITY, 0, 0, MAIN_TASK_NAME);
    OS_SetUserMainHandle(&mainTaskHandle);
}

}
