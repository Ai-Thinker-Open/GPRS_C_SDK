
#include "stdbool.h"
#include "stdint.h"
#include "stdio.h"
#include "string.h"

#include "api_os.h"
#include "api_debug.h"
#include "api_event.h"

#include <APDS9930.h>

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

void SecondTask(void *pData)
{
    OS_Sleep(3000);

    // ported from https://github.com/Depau/APDS9930/blob/master/examples/AmbientLightSensor/AmbientLightSensor.ino
    APDS9930 apds;
    float ambient_light = 0;
    uint16_t ch0 = 0;
    uint16_t ch1 = 1;

    // Initialize APDS-9930 (configure I2C and initial values)
    if ( apds.init(I2C2) ) {
        Trace(2, "APDS-9930 initialization complete");
    } else {
        Trace(2, "Something went wrong during APDS-9930 init!");
    }

    // Start running the APDS-9930 light sensor (no interrupts)
    if ( apds.enableLightSensor(false) ) {
        Trace(2, "Light sensor is now running");
    } else {
        Trace(2, "Something went wrong during light sensor init!");
    }

    while (1)
    {
        // Read the light levels (ambient, red, green, blue)
        if (!apds.readAmbientLightLux(ambient_light) ||
            !apds.readCh0Light(ch0) ||
            !apds.readCh1Light(ch1))
        {
            Trace(2, "Error reading light values");
        }
        else
        {
            Trace(2, "Ambient: %02.f", ambient_light);
            Trace(2, "    CH0: %d", ch0);
            Trace(2, "    CH1: %d", ch1);
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

void Depau_APDS9930_Main(void)
{
    mainTaskHandle = OS_CreateTask(MainTask,
        NULL, NULL, MAIN_TASK_STACK_SIZE, MAIN_TASK_PRIORITY, 0, 0, MAIN_TASK_NAME);
    OS_SetUserMainHandle(&mainTaskHandle);
}

}
