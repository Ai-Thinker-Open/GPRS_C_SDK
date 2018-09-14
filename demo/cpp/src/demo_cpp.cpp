
#include "stdbool.h"
#include "stdint.h"
#include "stdio.h"
#include "string.h"

#include "api_os.h"
#include "api_debug.h"
#include "api_event.h"
#include "api_hal_adc.h"




#define MAIN_TASK_STACK_SIZE    (2048 * 2)
#define MAIN_TASK_PRIORITY      0
#define MAIN_TASK_NAME          "Main Test Task"

#define SECOND_TASK_STACK_SIZE    (2048 * 2)
#define SECOND_TASK_PRIORITY      1
#define SECOND_TASK_NAME          "Second Test Task"

static HANDLE mainTaskHandle = NULL;
static HANDLE secondTaskHandle = NULL;

class A{

public:
    virtual void Show() = 0;
    void ShowA()
    {
        printf("class A");
    }
};

class B: public A{

public:
    virtual void Show()
    {
        printf("Show in B");
    }

    void ShowB()
    {
        printf("class B");
    }
};


void EventDispatch(API_Event_t* pEvent)
{
    switch(pEvent->id)
    {
        case API_EVENT_ID_NO_SIMCARD:
            Trace(10,"!!NO SIM CARD%d!!!!",pEvent->param1);
            break;

        case API_EVENT_ID_SYSTEM_READY:
            Trace(1,"system initialize complete");
            break;

        case API_EVENT_ID_NETWORK_REGISTERED_HOME:
        case API_EVENT_ID_NETWORK_REGISTERED_ROAMING:
            Trace(2,"network register success");
            break;

        default:
            break;
    }
}

void SecondTask(void *pData)
{
    uint16_t value = 0, mV = 0;
    ADC_Config_t config;
    config.channel = ADC_CHANNEL_0;
    config.samplePeriod = ADC_SAMPLE_PERIOD_100MS;
    ADC_Init(config);
    
    B b = B();
    while(1)
    {
        if(ADC_Read(ADC_CHANNEL_0, &value, &mV))
            Trace(1,"ADC value:%d, %dmV",value,mV);
        b.ShowA();
        b.ShowB();
        b.Show();

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

void cpp_Main(void)
{
    mainTaskHandle = OS_CreateTask(MainTask,
        NULL, NULL, MAIN_TASK_STACK_SIZE, MAIN_TASK_PRIORITY, 0, 0, MAIN_TASK_NAME);
    OS_SetUserMainHandle(&mainTaskHandle);
}

}
