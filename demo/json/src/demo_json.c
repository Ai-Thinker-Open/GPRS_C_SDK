#include "stdint.h"
#include "stdbool.h"
#include "stdlib.h"
#include "stdio.h"
#include "api_debug.h"
#include "api_os.h"
#include "api_event.h"
#include "sdk_init.h"

#include "cJSON.h"
#include "assert.h"


#define MAIN_TASK_STACK_SIZE    (1024 * 2)
#define MAIN_TASK_PRIORITY      0 
#define MAIN_TASK_NAME         "MAIN Test Task"

#define TEST_TASK_STACK_SIZE    (1024 * 2)
#define TEST_TASK_PRIORITY      1
#define TEST_TASK_NAME         "Test Task"

static HANDLE mainTaskHandle = NULL;
static HANDLE secondTaskHandle = NULL;


static int print_preallocated(cJSON *root)
{
    /* declarations */
    char *out = NULL;
    char *buf = NULL;
    char *buf_fail = NULL;
    size_t len = 0;
    size_t len_fail = 0;

    /* formatted print */
    out = cJSON_Print(root);

    /* create buffer to succeed */
    /* the extra 5 bytes are because of inaccuracies when reserving memory */
    len = strlen(out) + 5;
    buf = (char*)malloc(len);
    if (buf == NULL)
    {
        Trace(1,"Failed to allocate memory.\n");
        Assert(false,"malloc fail");
    }

    /* create buffer to fail */
    len_fail = strlen(out);
    buf_fail = (char*)malloc(len_fail);
    if (buf_fail == NULL)
    {
        Trace(1,"Failed to allocate memory.\n");
        Assert(false,"malloc fail");
    }

    /* Print to buffer */
    if (!cJSON_PrintPreallocated(root, buf, (int)len, 1)) {
        Trace(1,"cJSON_PrintPreallocated failed!\n");
        if (strcmp(out, buf) != 0) {
            Trace(1,"cJSON_PrintPreallocated not the same as cJSON_Print!\n");
            Trace(1,"cJSON_Print result:\n%s\n", out);
            Trace(1,"cJSON_PrintPreallocated result:\n%s\n", buf);
        }
        free(out);
        free(buf_fail);
        free(buf);
        return -1;
    }

    /* success */
    Trace(1,"%s\n", buf);

    /* force it to fail */
    if (cJSON_PrintPreallocated(root, buf_fail, (int)len_fail, 1)) {
        Trace(1,"cJSON_PrintPreallocated failed to show error with insufficient memory!\n");
        Trace(1,"cJSON_Print result:\n%s\n", out);
        Trace(1,"cJSON_PrintPreallocated result:\n%s\n", buf_fail);
        free(out);
        free(buf_fail);
        free(buf);
        return -1;
    }

    free(out);
    free(buf_fail);
    free(buf);
    return 0;
}


void STD_TestTask()
{
    OS_Sleep(3000);

    while(1)
    {
        cJSON *root = NULL;
        cJSON *fmt = NULL;

        root = cJSON_CreateObject();
        cJSON_AddItemToObject(root, "name", cJSON_CreateString("Jack (\"Bee\") Nimble"));
        cJSON_AddItemToObject(root, "format", fmt = cJSON_CreateObject());
        cJSON_AddStringToObject(fmt, "type", "rect");
        cJSON_AddNumberToObject(fmt, "width", 1920);
        cJSON_AddNumberToObject(fmt, "height", 1080);
        cJSON_AddFalseToObject (fmt, "interlace");
        cJSON_AddNumberToObject(fmt, "frame rate", 24);
        if (print_preallocated(root) != 0) {
            Trace(1,"print fail");
        }
        cJSON_Delete(root);

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


void json_Main()
{
    mainTaskHandle = OS_CreateTask(MainTask ,
        NULL, NULL, MAIN_TASK_STACK_SIZE, MAIN_TASK_PRIORITY, 0, 0, MAIN_TASK_NAME);
    OS_SetUserMainHandle(&mainTaskHandle);
}
