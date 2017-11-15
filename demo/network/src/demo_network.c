
#include <api_network.h>
#include <api_os.h>
#include <api_debug.h>



#define MAIN_TASK_STACK_SIZE    (1024 * 2)
#define MAIN_TASK_PRIORITY      0 
#define MAIN_TASK_NAME         "Network Test Task"

static HANDLE networkTaskHandle = NULL;


void OnNetworkStatusChanged(Network_Status_t status)
{
    switch(status)
    {
        case NETWORK_STATUS_REGISTERED:
            Trace(1,"network register success");
            Network_StartAttach();
            break;
        case NETWORK_STATUS_ATTACHED:
            Trace(1,"network attach success");
            Network_PDP_Context_t context = {
                .apn        ="cmnet",
                .userName   = ""    ,
                .userPasswd = ""
            };
            Network_StartActive(context);
            break;
        case NETWORK_STATUS_ACTIVATED:
            Trace(1,"network activate success");
            break;
        default:
            break;
    }
}

void network_MainTask()
{
    Network_SetStatusChangedCallback(OnNetworkStatusChanged);
    while(1)
    {
        static int times = 0;
        Trace(1,"idle:%d",++times);
        OS_Sleep(5000);
    }
}

void network_Main(void)
{
    networkTaskHandle = OS_CreateTask(network_MainTask ,
        NULL, NULL, MAIN_TASK_STACK_SIZE, MAIN_TASK_PRIORITY, 0, 0, MAIN_TASK_NAME);
    OS_SetUserMainHandle(&networkTaskHandle);
}


