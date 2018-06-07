
#include "api_network.h"
#include "api_os.h"
#include "api_debug.h"
#include "api_fota.h"



#define MAIN_TASK_STACK_SIZE    (1024 * 2)
#define MAIN_TASK_PRIORITY      0 
#define MAIN_TASK_NAME         "FOTA SERVER Test Task"


#define SOFT_VERSION          "V3.1"
#define FOTA_HTTP_SERVER      "http://test.ai-thinker.com/csdk/fota/%stonew.pack"



static HANDLE mainTaskHandle = NULL;
static int fotasize = 0; 

static int processFotaUpgradeData(const unsigned char *data, size_t len)
{
    if(len)
    {
        Trace(1,"fota len:%d data:%s fotasize:%d ", len, data, fotasize);
        if(fotasize == 0)
        {
            if(!API_FotaInit(len))
                goto upgrade_faile;
            if(API_FotaReceiveData(data, len) == 0)
                goto upgrade_faile;
        }
        else if(fotasize)
        {
            if(API_FotaReceiveData(data, len) == 0)
                goto upgrade_faile;
        }
    }else{//error
        Trace(1,"fota len:%d data:%s fotasize:%d ", len, data, fotasize);
        goto upgrade_faile;
    }
    return;

upgrade_faile:
    Trace(1,"server fota false");
    // API_FotaClean();
    return 0;
}

static void OnNetworkStatusChanged(Network_Status_t status)
{
    switch(status)
    {
        case NETWORK_STATUS_REGISTERED:
            Trace(1,"fota network register success");
            Network_StartAttach();
            break;
        case NETWORK_STATUS_ATTACHED:
            Trace(1,"fota network attach success");
            Network_PDP_Context_t context = {
                .apn        ="cmnet",
                .userName   = ""    ,
                .userPasswd = ""
            };
            Network_StartActive(context);
            break;
        case NETWORK_STATUS_ACTIVATED:
        {
            char url[256];
            memset(url, 0, sizeof(url));
            sprintf(url, FOTA_HTTP_SERVER, SOFT_VERSION);
            Trace(1,"fota network activate success url %s", url);
            if(API_FotaByServer(url, processFotaUpgradeData) == 0)
                Trace(1,"fota wait");
            else
                Trace(1,"fota fail");
            break;
        }
        default:
            break;
    }
}

static void FOTASERVER_TestTask()
{
    Network_SetStatusChangedCallback(OnNetworkStatusChanged);
    while(1)
    {
        static int times = 0;
        Trace(1,"fota server is new %s:%d", SOFT_VERSION, ++times);
        OS_Sleep(5000);
    }
}

void FOTASERVER_Test(void)
{
    mainTaskHandle = OS_CreateTask(FOTASERVER_TestTask ,
        NULL, NULL, MAIN_TASK_STACK_SIZE, MAIN_TASK_PRIORITY, 0, 0, MAIN_TASK_NAME);
    OS_SetUserMainHandle(&mainTaskHandle);
}

