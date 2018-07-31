




#include "stdbool.h"
#include "stdint.h"
#include "stdio.h"
#include "string.h"

#include "api_os.h"
#include "api_debug.h"
#include "api_event.h"
#include "api_network.h"
#include "api_ssl.h"


///////////////////////////////////////////////////
#define SSL_SERVER_ADDRESS    "ssl.neucrack.com"
#define SSL_SERVER_PORT       "3456"
#define SSL_SERVER_HOST_NAME  "ssl.neucrack.com"

const char* ca_cert = "-----BEGIN CERTIFICATE-----\n\
MIID9zCCAt+gAwIBAgIJAJ/FqkkWhVVtMA0GCSqGSIb3DQEBCwUAMIGRMQswCQYD\n\
VQQGEwJDTjESMBAGA1UECAwJR1VBTkdET05HMREwDwYDVQQHDAhTSE5FWkhFTjEU\n\
MBIGA1UECgwLTkVVQ1JBQ0tfQ0ExCzAJBgNVBAsMAkNBMRQwEgYDVQQDDAtuZXVj\n\
cmFja19jYTEiMCAGCSqGSIb3DQEJARYTY3pkNjY2NjY2QGdtYWlsLmNvbTAeFw0x\n\
ODAxMTgwNzEyMDFaFw0xOTAxMTgwNzEyMDFaMIGRMQswCQYDVQQGEwJDTjESMBAG\n\
A1UECAwJR1VBTkdET05HMREwDwYDVQQHDAhTSE5FWkhFTjEUMBIGA1UECgwLTkVV\n\
Q1JBQ0tfQ0ExCzAJBgNVBAsMAkNBMRQwEgYDVQQDDAtuZXVjcmFja19jYTEiMCAG\n\
CSqGSIb3DQEJARYTY3pkNjY2NjY2QGdtYWlsLmNvbTCCASIwDQYJKoZIhvcNAQEB\n\
BQADggEPADCCAQoCggEBAOFA7NEQvrCpV67exKjyru6Vc2KYFc+B/nQyJmT31nhO\n\
f8De2mUr1HPlZY9u1pJeMCkY2c2ToKXdaXNTUznI3PVTtxc5z2U5u1L2elg7G7kV\n\
zyrAAjz4SKD/hZTL4gUFyO3sfSUBgvDHVwDcB1wqcQPJXmPDRb5aT69LkmHTKMpB\n\
OjYQ3Sw2fQo4EzTjb4lfaAU30MdO1AgfVT0LP82G/7HfUEt2IdZrpjkprHvUo/2m\n\
gDYbq9uOJTCi5HUMSqXDM/wpEnRP4ATSrcL6Xccjryv6wQRroW3da+hXxcWARqZR\n\
i3OfXdCMrSaw8nMNxLPss4UrpQ02P+2yqzWfdeYZ6jkCAwEAAaNQME4wHQYDVR0O\n\
BBYEFJD9fZdjm+lPykElhBviFcYvK3m9MB8GA1UdIwQYMBaAFJD9fZdjm+lPykEl\n\
hBviFcYvK3m9MAwGA1UdEwQFMAMBAf8wDQYJKoZIhvcNAQELBQADggEBAA6o/esx\n\
Ls/gftzuWt3b0Wej2GVRk4snx1DAYMnQA3MgAXw1Y3cGZ7y8cwMyDWy7//K2l2kQ\n\
HT6sithRUCBLK3ZkXAAZz5valBdwHSRYWk40oi+WXX0ZzVzsrcf4ISMvk57yl0yV\n\
d91KhrYTFGIksWJR6UMEeVRRQKcHcsjjqsAL0i1rWHHtp3G3EX4LCK91aVKfUWZa\n\
evTKVXCux8nhKo3fDUQTDwNEGd+XDUBUJ/sANhvRJ1zrtT8NbUF5XTIB07r28BsY\n\
DHFqwQjorWeg4Z6t9/yFiffyVmF72MZG7cG24JN+VwEfzD1vz+5OSY0ZeoLUmYv7\n\
NYYZjpkEaGH9eCs=\n\
-----END CERTIFICATE-----";

const char* client_cert = "-----BEGIN CERTIFICATE-----\n\
MIIDpDCCAowCAQEwDQYJKoZIhvcNAQELBQAwgZExCzAJBgNVBAYTAkNOMRIwEAYD\n\
VQQIDAlHVUFOR0RPTkcxETAPBgNVBAcMCFNITkVaSEVOMRQwEgYDVQQKDAtORVVD\n\
UkFDS19DQTELMAkGA1UECwwCQ0ExFDASBgNVBAMMC25ldWNyYWNrX2NhMSIwIAYJ\n\
KoZIhvcNAQkBFhNjemQ2NjY2NjZAZ21haWwuY29tMB4XDTE4MDExODA3MTMyM1oX\n\
DTE5MDExODA3MTMyM1owgZ0xCzAJBgNVBAYTAkNOMRIwEAYDVQQIDAlHVUFOR0RP\n\
TkcxETAPBgNVBAcMCFNIRU5aSEVOMRgwFgYDVQQKDA9ORVVDUkFDS19DTElFTlQx\n\
DzANBgNVBAsMBkNMSUVOVDEYMBYGA1UEAwwPbmV1Y3JhY2tfY2xpZW50MSIwIAYJ\n\
KoZIhvcNAQkBFhNjemQ2NjY2NjZAZ21haWwuY29tMIIBIjANBgkqhkiG9w0BAQEF\n\
AAOCAQ8AMIIBCgKCAQEArx3otIKLYnS3/eMPPV/RqYNIV6MQ9QYZn+WMleeVi+sr\n\
ANGo2eyGRcmHHTofZPA+bFQnunh+CwX2maQ2DxBxK0jSwUS4IFtXm2z0/eYS7NbV\n\
ehlEnJ/Zryklkuohy9tItNVGlnODHQqrqzO4UEcAd+tGxC4shuHtB+ghCt/qyTcm\n\
lrY3/2sDBp//J52I8wi0TRz6hYb1mbzavL2BMaGlmq9TITQ9XSYa+YTHREwvxYKQ\n\
HEFB8fcvrtkb5NZdtqWRsbLCoRUU1ZwDq85G1PRP763KUzgiRiPCKBY8GM05ePft\n\
xzPwDz9gTg2x2MUtBKMz9wsSKODxuhgtrxjoYIMcgwIDAQABMA0GCSqGSIb3DQEB\n\
CwUAA4IBAQBAehFa7NdP0CFf6hEZ5JvcdKYbyGFhcmufduTRfx7JFPL8IgYt56NA\n\
do02CnBxN5145G+I79bQoXcbw/QK9u12AvTHJ3twow9/4pQZiS+tKLao/UAuZem3\n\
EOxFNGHApOmOhDj/ypbU3nox4SdbZGzO0HXK4T/mCIMKTMevQAoBYtR8rPJY3V10\n\
McUpgtqWJGo2WqSB84+4RzzEP3ySHkAYh83xELv1imyevoNl128tjfagHYMLf1Yy\n\
q5gzlewghsN0T0D32ysswWHNI8abR4Gkq8pTsS6gGQf/6MV7ih6k0I9Lk3+nubFP\n\
R3/EQfUl+7lP5rg5hyi9ryPLgcPyjdNZ\n\
-----END CERTIFICATE-----";

const char* client_key = "-----BEGIN RSA PRIVATE KEY-----\n\
MIIEpQIBAAKCAQEArx3otIKLYnS3/eMPPV/RqYNIV6MQ9QYZn+WMleeVi+srANGo\n\
2eyGRcmHHTofZPA+bFQnunh+CwX2maQ2DxBxK0jSwUS4IFtXm2z0/eYS7NbVehlE\n\
nJ/Zryklkuohy9tItNVGlnODHQqrqzO4UEcAd+tGxC4shuHtB+ghCt/qyTcmlrY3\n\
/2sDBp//J52I8wi0TRz6hYb1mbzavL2BMaGlmq9TITQ9XSYa+YTHREwvxYKQHEFB\n\
8fcvrtkb5NZdtqWRsbLCoRUU1ZwDq85G1PRP763KUzgiRiPCKBY8GM05ePftxzPw\n\
Dz9gTg2x2MUtBKMz9wsSKODxuhgtrxjoYIMcgwIDAQABAoIBAQCW6+38dG3wYquX\n\
8jLWMFigAXc9oFoi8ijvd4ScRUTLm9gqEoe1mXV3e+IovEkGU09ZSYYb3XCzy2Xm\n\
sHGutfry1QZebv1JVa/BUjwq/jjdSaIeYtyRJ+sSXSH25jZOXSTiRNVbIR6kzud4\n\
21BET40FzQ3mUe0qKliAcfzYRHczBhmmTT/Nqdlwg3gPJMw9ub6ZPO1bpoIbTMjY\n\
x+yL0ybUzRajy4ol9q82pYg+OrhtlOZHxu8CKNrlSmsXB3WeJoii2GbW1CA2lynX\n\
UbDGF0zHf5Sq3ioWUSlPVJA04/IMEoIcGgG9w/3OHsrSpkWRo1sJ646wzfHaws64\n\
yvg0yXYBAoGBAN5CVKgVUn3EQ/X7O63CsoXIPi7E4+b/G1vERZbU8wJrQq3BjMOH\n\
DIrUaqMUaR1c/9kZv19YLpSCOU0kuGHYMhATtOYHdCUobLycS1o1e7Ftz+xZZn2A\n\
nhfJqRwybsSjGx+uIxhgn1y//ak//h9Upat63q4GR+2sctARdFkOMmwDAoGBAMmz\n\
fDr9rWoeEaEK9yOBdTowqP5QQsMtKOyHt21AFYCRHdIax13DyP1OhBZNAgjOpatl\n\
CV+pzUK8tC8DL3Lo/i+HYRY6j7VU/JXHBaXa0XyjL/dv24B5Sk+HJWWMJ7EESkMa\n\
oiuv3j70z1DBYg+nqibXiQD5KNOSRUM9Ksvk1OWBAoGBAIC/0+J6V4rbQrMfkfaa\n\
LmgpEzxseci6RtLl45n4Sb1A/SNCV5R8ittxKUTG8g08bkkJDT7iCjmlJiR1/QlQ\n\
/88oW0ZVUwajYDnyIARlrv2dKzEyqJhTBUGqY/gdjuUBxxiW+2WHs0k7Pmno1AC6\n\
PC4VwyD0Kn6Xwvqc4dUk0DiZAoGAEJUdBYyNkUffkynfn5+pD5uoiR/4HSu9TZTq\n\
06SNIADOPtPMyDysbYbi/SJfpHG41Rp9ZswDABWbirHsWLonC6lJphYycmoGShoO\n\
sNAIoQKZkvIzTaCrJ1apEGwxZzA/Gr05z/Wpg+uEj9MWSMs0BfV6YgYaASrpXtnM\n\
hu7NQgECgYEAg6s/toJ8fwGlU+o2GBJdU0d206OWZ+OvYf87OISVX9xnVkvoBPz6\n\
2Vo31PURShDzjd4QIJQq8Pw+4WGczoeWrtqhfV5V+52b3v8ii69fFc8daVBa8rke\n\
OLjDMAYBm2U0rEVJRGhH3G7zP31fe4v4LzpCOasozf1Vm0YXapQ6KlI=\n\
-----END RSA PRIVATE KEY-----";


uint8_t* dataToSend="data from GPRS";
uint8_t buffer[50];
/////////////////////////////////////////////////


#define MAIN_TASK_STACK_SIZE    (2048 * 2)
#define MAIN_TASK_PRIORITY      0
#define MAIN_TASK_NAME          "Main Test Task"

#define SECOND_TASK_STACK_SIZE    (2048 * 2)
#define SECOND_TASK_PRIORITY      1
#define SECOND_TASK_NAME          "Second Test Task"

static HANDLE mainTaskHandle = NULL;
static HANDLE secondTaskHandle = NULL;
static bool flag = false;


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
            Network_StartAttach();
            break;

        case API_EVENT_ID_NETWORK_ATTACHED:
            Trace(2,"network attach success");
            Network_PDP_Context_t context = {
                .apn        ="cmnet",
                .userName   = ""    ,
                .userPasswd = ""
            };
            Network_StartActive(context);
            break;

        case API_EVENT_ID_NETWORK_ACTIVATED:
            Trace(2,"network activate success");
            flag = 1;
            break;

        default:
            break;
    }
}


int SSL_Test_Client1( void )
{
    int ret;
    SSL_Error_t error;
    SSL_Config_t config = {
        .caCert = ca_cert,
        .caCrl  = NULL,
        .clientCert = client_cert,
        .clientKey  = client_key,
        .clientKeyPasswd = NULL,
        .hostName   = SSL_SERVER_HOST_NAME,
        .minVersion = SSL_VERSION_SSLv3,
        .maxVersion = SSL_VERSION_TLSv1_2,
        .verifyMode = SSL_VERIFY_MODE_REQUIRED,
        .entropyCustom = "GPRS"
    };
    error = SSL_Init(&config);
    if(error != SSL_ERROR_NONE)
    {
        Trace(1,"ssl init error:%d",error);
        return error;
    }
    error = SSL_Connect(&config,SSL_SERVER_ADDRESS,SSL_SERVER_PORT);
    if(error != SSL_ERROR_NONE)
    {
        Trace(1,"ssl connect error:%d",error);
        goto exit;
    }
    for(int i=0;i<10;++i)
    {
        Trace(1,"count:%d  write len:%d data:%s",i,strlen(dataToSend),dataToSend);
        ret = SSL_Write(&config,dataToSend,strlen(dataToSend),5000);
        if(ret <= 0)
        {
            error = ret;
            Trace(1,"ssl write fail:%d",error);
            goto exit0;
        }
        memset(buffer,0,sizeof(buffer));
        ret = SSL_Read(&config,buffer,strlen(dataToSend),2000);
        if(ret <= 0)
        {
            error = ret;
            Trace(1,"ssl read fail:%d",error);
            goto exit0;
        }
        Trace(1,"read len:%d, data:%s",ret,buffer);
    }
    
exit0:
    Trace(1,"ssl close");
    SSL_Close(&config);
exit:
    Trace(1,"ssl destroy");
    SSL_Destroy(&config);
    return error;
}


void SecondTask(void *pData)
{
    while(1)
    {
        if(flag)
        {
            Trace(1,"start ssl test");
            SSL_Test_Client1();
            Trace(1,"ssl test end");
            break;
        }
        OS_Sleep(1);
    }
    while(1)
        OS_Sleep(10000);
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

void ssl_Main()
{
    mainTaskHandle = OS_CreateTask(MainTask,
        NULL, NULL, MAIN_TASK_STACK_SIZE, MAIN_TASK_PRIORITY, 0, 0, MAIN_TASK_NAME);
    OS_SetUserMainHandle(&mainTaskHandle);
}

