
#include "stdbool.h"
#include "stdint.h"
#include "stdio.h"
#include "string.h"

#include "api_os.h"
#include "api_debug.h"
#include "api_event.h"

#include <ClosedCube_OPT3001.h>

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

    // Get the lux from the OPT3001 sensor
    ClosedCube_OPT3001 opt3001;
    opt3001.begin(I2C2);

    Trace(2, "OPT3001 Manufacturer ID: %04x", opt3001.readManufacturerID());
    Trace(2, "OPT3001 Device ID: %04x", opt3001.readDeviceID());

    OPT3001_Config newConfig;
 	newConfig.RangeNumber = 0xC;
 	newConfig.ConvertionTime = 0x0;
 	newConfig.Latch = 0x1;
 	newConfig.ModeOfConversionOperation = 0x3;

 	OPT3001_ErrorCode errorConfig = opt3001.writeConfig(newConfig);
 	if (errorConfig != NO_ERROR) {
        Trace(2, "OPT3001 configuration error");
        return;
    } else {
        OPT3001_Config sensorConfig = opt3001.readConfig();
        Trace(2, "OPT3001 Current Config");
        Trace(2, "Conversion ready (R): 0x%02x", sensorConfig.ConversionReady);
        Trace(2, "Conversion time (R/W): 0x%02x", sensorConfig.ConvertionTime);
        Trace(2, "Fault count field (R/W): 0x%02x", sensorConfig.FaultCount);
        Trace(2, "Flag high field (R-only): 0x%02x", sensorConfig.FlagHigh);
        Trace(2, "Flag low field (R-only): 0x%02x", sensorConfig.FlagLow);
        Trace(2, "Latch field (R/W): 0x%02x", sensorConfig.Latch);
        Trace(2, "Mask exponent field (R/W): 0x%02x", sensorConfig.MaskExponent);
        Trace(2, "Mode of conversion operation (R/W): 0x%02x", sensorConfig.ModeOfConversionOperation);
        Trace(2, "Polarity field (R/W): 0x%02x", sensorConfig.Polarity);
        Trace(2, "Overflow flag (R-only): 0x%02x", sensorConfig.OverflowFlag);
        Trace(2, "Range number (R/W): 0x%02x", sensorConfig.RangeNumber);
    }

    OPT3001 result = opt3001.readHighLimit();
    if (result.error == NO_ERROR) {
        Trace(2, "High-Limit: %f lux", result.lux);
    } else {
        Trace(2, "High-Limit: error");
    }

    result = opt3001.readLowLimit();
    if (result.error == NO_ERROR)
    {
        Trace(2, "Low-Limit: %f lux", result.lux);
    }
    else
    {
        Trace(2, "Low-Limit: error");
    }

    while (1)
    {
        result = opt3001.readResult();
        if (result.error == NO_ERROR)
        {
            Trace(2, "Lux: %f lux", result.lux);
        }
        else
        {
            Trace(2, "Lux: error");
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

void ClosedCube_OPT3001_Main(void)
{
    mainTaskHandle = OS_CreateTask(MainTask,
        NULL, NULL, MAIN_TASK_STACK_SIZE, MAIN_TASK_PRIORITY, 0, 0, MAIN_TASK_NAME);
    OS_SetUserMainHandle(&mainTaskHandle);
}

}
