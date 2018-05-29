#include "stdint.h"
#include "stdbool.h"
#include "stdlib.h"
#include "stdio.h"
#include "api_debug.h"
#include "api_os.h"
#include "api_event.h"
#include "math.h"


#define MAIN_TASK_STACK_SIZE    (1024 * 2)
#define MAIN_TASK_PRIORITY      0 
#define MAIN_TASK_NAME         "MAIN Test Task"

#define TEST_TASK_STACK_SIZE    (1024 * 2)
#define TEST_TASK_PRIORITY      1
#define TEST_TASK_NAME         "Test Task"

static HANDLE mainTaskHandle = NULL;
static HANDLE secondTaskHandle = NULL;



int cmp ( const void *a , const void *b ) 
{ 
    return *(int *)a - *(int *)b; 
} 

void STD_TestTask()
{
    double value = 1.234567;
    char bufff[50];

    while(1)
    {
        gcvt(value, 3, bufff);
        Trace(1,"%s",bufff);

        int valueInt = 12345;
        memset(bufff,0,sizeof(bufff));
        itoa(valueInt,bufff,10);
        Trace(1,"%s",bufff);

        int a = atoi("12");
        Trace(1,"atoi:%d",a);

        long b = atol("12");
        Trace(1,"atol:%d",b);

        long c = atoll("12");
        Trace(1,"atoll:%d",c);

        long d = atox("1234",4);
        Trace(1,"atox:%d",d);

        float e = atof("123.456");
        Trace(1,"atof:%d.%d",(int)(e),((int)(e*1000)%1000));

        char* string = "-12345 stop here";
        char* stopString = NULL;
        long ret = strtol(string,&stopString,10);
        Trace(1,"ret:%d,string:%s,stopstring:%s",(int)ret,string,stopString);

        srand(123);
        int randValue = rand();
        Trace(1,"rand value:%d",randValue);

        int num[10]={2,1,4,5,8,2,4,65,34,23};
        qsort(num,10,sizeof(num[0]),cmp);
        memset(bufff,0,sizeof(bufff));
        for(int i=0;i<10;++i)
            snprintf(bufff+strlen(bufff),50,"%d ",num[i]);
        Trace(1,"qsort:%s",bufff);

        int key = 34;
        int* item = NULL;
        item = bsearch(&key,num,10,sizeof(int),cmp);
        Trace(1,"item:%d",*item);

        int read0,read1;
        // sscanf("123 456","%d %d",&read0,&read1);//error!!!
        sscanf("123","%d",&read0);
        sscanf("456","%d",&read1);
        Trace(1,"read0:%d, read1:%d",read0,read1);

        a = atan(1);
        gcvt(a, 3, bufff);
        Trace(1,"atan(1) = %s",bufff);


        
        OS_Sleep(3000);
    }
}


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

    secondTaskHandle = OS_CreateTask(STD_TestTask,
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


void std_Main()
{
    mainTaskHandle = OS_CreateTask(MainTask ,
        NULL, NULL, MAIN_TASK_STACK_SIZE, MAIN_TASK_PRIORITY, 0, 0, MAIN_TASK_NAME);
    OS_SetUserMainHandle(&mainTaskHandle);
}
