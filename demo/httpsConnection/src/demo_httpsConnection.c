/*
 * @File  demo_httpsConnection.c
 * @Brief HTTPs connection with chip A9G. POST and GET functions.
 * 
 * @Author: Ricx8 
 * @Date: 2019-01-10 23:27:26 
 * @Last Modified by: Ricx8
 * @Last Modified time: 2019-01-16 01:38:55
 */

#include <string.h>
#include <stdio.h>
#include <api_os.h>
#include <api_event.h>
#include <api_socket.h>
#include <api_network.h>
#include <api_debug.h>
#include "api_ssl.h"

/*******************************************************************/
/////////////////////////socket configuration////////////////////////
#define SERVER_IP           "caracascoder.com" // Replace this with the host to connect
#define SERVER_PORT         443     // Replace this with the port to connect
#define SSL_SERVER_PORT     "443"   // Replace this with the port to connect
#define SERVER_PATH         "/test/setLocation.php?location=A9G:443_v23" // [GET] Replace this with your path
#define SERVER_PATH_POST    "/test/setLocationPOST.php" // [POST] Replace this with your path
#define SERVER_DATA         "\r\nlocation=A9G_POST_v23" // [POST] Replace this with POST data
/*******************************************************************/

// Replace the next certificate with the certificate of the page you will connet to.
const char* ca_cert = "-----BEGIN CERTIFICATE-----\n\
MIIFbDCCBFSgAwIBAgISA0gXQ0cnsv7p31+BZWAULSL1MA0GCSqGSIb3DQEBCwUA\n\
MEoxCzAJBgNVBAYTAlVTMRYwFAYDVQQKEw1MZXQncyBFbmNyeXB0MSMwIQYDVQQD\n\
ExpMZXQncyBFbmNyeXB0IEF1dGhvcml0eSBYMzAeFw0xODEyMDUwNTQyMjRaFw0x\n\
OTAzMDUwNTQyMjRaMBsxGTAXBgNVBAMTEGNhcmFjYXNjb2Rlci5jb20wggEiMA0G\n\
CSqGSIb3DQEBAQUAA4IBDwAwggEKAoIBAQC65kt6A41pEicBD1xzlHRmZU3DfZcB\n\
lp8D/xLI2ye0uEKX4qexrsuUycyUnAu6RTUHs6zb5dnXdYHUyQj6cUhq/bDn1td8\n\
rpj3+m1tSAXSIFEoxG+oT9w+HbjGO6chitI07k7pR5UVwXNVuzEO1Oq3PqBrYK6I\n\
5EEjmW3ABS2NZlgAF/xaDHGaLswrOnbouT+tkrnGN74FRc6prNw5e+dBcdhJpu16\n\
9OqfrZo1/e8qLC3knuGBPJtY1m09vYlyMq/acW2G4b+6bLsdWHohPOijDxVvxy/X\n\
NG7W6/EkebsImS3UfiMCqnredmiI2TZs10dRHE2j7FBO3zvNfMaCmuzDAgMBAAGj\n\
ggJ5MIICdTAOBgNVHQ8BAf8EBAMCBaAwHQYDVR0lBBYwFAYIKwYBBQUHAwEGCCsG\n\
AQUFBwMCMAwGA1UdEwEB/wQCMAAwHQYDVR0OBBYEFFzYUb1zmdjpUc7mi4+pxv83\n\
Nl+zMB8GA1UdIwQYMBaAFKhKamMEfd265tE5t6ZFZe/zqOyhMG8GCCsGAQUFBwEB\n\
BGMwYTAuBggrBgEFBQcwAYYiaHR0cDovL29jc3AuaW50LXgzLmxldHNlbmNyeXB0\n\
Lm9yZzAvBggrBgEFBQcwAoYjaHR0cDovL2NlcnQuaW50LXgzLmxldHNlbmNyeXB0\n\
Lm9yZy8wMQYDVR0RBCowKIIQY2FyYWNhc2NvZGVyLmNvbYIUd3d3LmNhcmFjYXNj\n\
b2Rlci5jb20wTAYDVR0gBEUwQzAIBgZngQwBAgEwNwYLKwYBBAGC3xMBAQEwKDAm\n\
BggrBgEFBQcCARYaaHR0cDovL2Nwcy5sZXRzZW5jcnlwdC5vcmcwggECBgorBgEE\n\
AdZ5AgQCBIHzBIHwAO4AdQB0ftqDMa0zEJEhnM4lT0Jwwr/9XkIgCMY3NXnmEHvM\n\
VgAAAWd9GnQfAAAEAwBGMEQCIFHSIub5jcIkvtboWzMNDnC7mNJDoO4dhu3FfoMn\n\
gD2ZAiAGNaEo2olXcLYY3NnMMFfoTSvigsFvpFBUVt1g8l692AB1ACk8UZZUyDll\n\
uqpQ/FgH1Ldvv1h6KXLcpMMM9OVFR/R4AAABZ30adF4AAAQDAEYwRAIge+whROY5\n\
Z6nHBJPdofSd5zPpu5qSz8Vu75kyg78g7hICIBud81Z8D8dQXe9oJN2leOU/gfLv\n\
XwwI3NXcLx3A6QGGMA0GCSqGSIb3DQEBCwUAA4IBAQB48k0mw9GaR/YOazNCYGfl\n\
ZA5QhGPANVmCSHgE+6kXPWJXwMmPg1/IP8WL+PNdeid8S48aHt1hO+pEqUjdVF+Z\n\
FO2r397GDJ42CQVUIfPUx8C1s+ZLcsNaukqhyMgzzUc8jw9Fu1z6sPxalJpbLvje\n\
54XeL++s3jm4exnPQc8mkJyugCZUIWrlg2GrzCGAgAe7NJYlGW3UDlCpdHKrDZTp\n\
bS+2G9dRIhOWfVsUJ67bGfyE1xb2eq9aNrogc7kKFoeFM/Ye6dxWBKET1EAVtBuR\n\
tB5nq0N8j20gka7hpYoS99GrHXNyFO5rRJH0Of1jzEfT+S4hCDFG7tTbA5IDejVP\n\
-----END CERTIFICATE-----";

#define MAIN_TASK_STACK_SIZE    (2048 * 2)
#define MAIN_TASK_PRIORITY      0
#define MAIN_TASK_NAME          "Socket Test Task"

#define TEST_TASK_STACK_SIZE    (2048 * 2)
#define TEST_TASK_PRIORITY      0
#define TEST_TASK_NAME          "Test Task"

static HANDLE socketTaskHandle = NULL;
static HANDLE testTaskHandle = NULL;
static HANDLE semStart = NULL;


void EventDispatch(API_Event_t* pEvent)
{
    switch(pEvent->id)
    {
        case API_EVENT_ID_NO_SIMCARD:
            Trace(10,"!!NO SIM CARD%d!!!!",pEvent->param1);
            break;

        case API_EVENT_ID_NETWORK_REGISTER_SEARCHING:
            Trace(2,"network register searching");
            break;
        
        case API_EVENT_ID_NETWORK_REGISTER_DENIED:
            Trace(2,"network register denied");
            break;
        
        case API_EVENT_ID_NETWORK_REGISTER_NO:
            Trace(2,"network register no");
            break;
        
        case API_EVENT_ID_NETWORK_REGISTERED_HOME:
        case API_EVENT_ID_NETWORK_REGISTERED_ROAMING:
        {
            uint8_t status;
            Trace(2,"network register success");
            bool ret = Network_GetAttachStatus(&status);
            if(!ret)
                Trace(1,"get attach staus fail");
            Trace(1,"attach status:%d",status);
            if(status == 0)
            {
                ret = Network_StartAttach();
                if(!ret)
                {
                    Trace(1,"network attach fail");
                }
            }
            else
            {
                Network_PDP_Context_t context = {
                    .apn        ="wholesale", // Replace this with your APN name
                    .userName   = ""    ,
                    .userPasswd = ""
                };
                Network_StartActive(context);
            }
            break;
        }
        case API_EVENT_ID_NETWORK_ATTACHED:
            Trace(2,"network attach success");
            Network_PDP_Context_t context = {
                .apn        ="wholesale", // Replace this with your APN name
                .userName   = ""    ,
                .userPasswd = ""
            };
            Network_StartActive(context);
            break;

        case API_EVENT_ID_NETWORK_ACTIVATED:
            Trace(2,"network activate success");
            OS_ReleaseSemaphore(semStart);
            break;

        default:
            break;
    }
}


//https GET 
int Https_Get(const char* domain, int port,const char* path, char* retBuffer, int* bufferLen){
    uint8_t ip[16];
    uint8_t buffer[2048];
    int retBufferLen = *bufferLen;
    int ret;
    SSL_Error_t error;
    SSL_Config_t config = {
        .caCert = ca_cert,
        .caCrl  = NULL,
        .clientCert = NULL,
        .clientKey  = NULL,
        .clientKeyPasswd = NULL,
        .hostName   = SERVER_IP,
        .minVersion = SSL_VERSION_SSLv3,
        .maxVersion = SSL_VERSION_TLSv1_2,
        .verifyMode = SSL_VERIFY_MODE_OPTIONAL,
        .entropyCustom = "GPRS"
    };

    // Get ip from DNS server.
    memset(ip,0,sizeof(ip));
    if(DNS_GetHostByName2(domain,ip) != 0){
        Trace(1,"get ip error");
        return -1;
    }
    Trace(1,"get ip success:%s -> %s",domain,ip);

    // Build the package
    snprintf(retBuffer,retBufferLen,"GET %s HTTP/1.1\r\nHost: %s\r\n\r\n", path, domain);
    char* pData = retBuffer;

    error = SSL_Init(&config);
    if(error != SSL_ERROR_NONE){
        Trace(1,"ssl init error:%d",error);
        return -1;
    }

    // Connect to server
    error = SSL_Connect(&config, SERVER_IP, SSL_SERVER_PORT);
    if(error != SSL_ERROR_NONE){
        Trace(1,"ssl connect error:%d",error);
        Trace(1,"ssl destroy");
        SSL_Destroy(&config);
        return -1;
    }

    // Send package
    Trace(1,"write len:%d data:%s" ,strlen(pData),pData);
    ret = SSL_Write(&config, pData, strlen(pData),5000);
    if(ret <= 0){
        error = ret;
        Trace(1,"ssl write fail:%d",error);
        Trace(1,"ssl close");
        SSL_Close(&config);
        return -1;
    }

    // Read response
    memset(buffer,0,sizeof(buffer));
    ret = SSL_Read(&config, buffer, strlen(pData), 2000);
    if(ret <= 0){
        error = ret;
        Trace(1,"ssl read fail:%d",error);
        Trace(1,"ssl close");
        SSL_Close(&config);
        return -1;
    }
    Trace(1,"read len:%d, data:%s",ret,buffer);

    SSL_Close(&config);
    return 0;
}

//https POST
int Https_Post(const char* domain, int port,const char* path, const char* data, uint16_t dataLen, char* retBuffer, int* bufferLen){
    uint8_t ip[16];
    uint8_t buffer[2048];
    int retBufferLen = *bufferLen;
    int ret;
    SSL_Error_t error;
    SSL_Config_t config = {
        .caCert = ca_cert,
        .caCrl  = NULL,
        .clientCert = NULL,
        .clientKey  = NULL,
        .clientKeyPasswd = NULL,
        .hostName   = SERVER_IP,
        .minVersion = SSL_VERSION_SSLv3,
        .maxVersion = SSL_VERSION_TLSv1_2,
        .verifyMode = SSL_VERIFY_MODE_OPTIONAL, // SSL_VERIFY_MODE_REQUIRED
        .entropyCustom = "GPRS"
    };

    // Get ip from DNS server.
    memset(ip,0,sizeof(ip));
    if(DNS_GetHostByName2(domain,ip) != 0){
        Trace(1,"get ip error");
        return -1;
    }
    Trace(1,"get ip success:%s -> %s",domain,ip);

    // Build the package
    snprintf(retBuffer,retBufferLen,"POST %s HTTP/1.1\r\nHost: %s\r\nContent-Type: application/x-www-form-urlencoded\r\nContent-Length: %d\r\n%s\r\n\r\n", path, domain, dataLen, data);
    char* pData = retBuffer;
    Trace(1,"Package: %s", pData);

    error = SSL_Init(&config);
    if(error != SSL_ERROR_NONE){
        Trace(1,"ssl init error:%d",error);
        return -1;
    }

    // Connect to server
    error = SSL_Connect(&config, SERVER_IP, SSL_SERVER_PORT);
    if(error != SSL_ERROR_NONE){
        Trace(1,"ssl connect error:%d",error);
        Trace(1,"ssl destroy");
        SSL_Destroy(&config);
        return -1;
    }

    // Send package
    Trace(1,"Write len:%d data:%s", strlen(pData), pData);
    ret = SSL_Write(&config, pData, strlen(pData),5000);
    if(ret <= 0){
        error = ret;
        Trace(1,"ssl write fail:%d",error);
        Trace(1,"ssl close");
        SSL_Close(&config);
        return -1;
    }

    // Read response
    memset(buffer,0,sizeof(buffer));
    ret = SSL_Read(&config, buffer, strlen(pData), 2000);
    if(ret <= 0){
        error = ret;
        Trace(1,"ssl read fail:%d",error);
        Trace(1,"ssl close");
        SSL_Close(&config);
        return -1;
    }
    Trace(1,"read len:%d, data:%s",ret,buffer);

    SSL_Close(&config);
    return 0;
}


void Socket_BIO_Test(){
    char buffer[2048];
    int len = sizeof(buffer);
    //wait for gprs network connection ok
    semStart = OS_CreateSemaphore(0);
    OS_WaitForSemaphore(semStart,OS_TIME_OUT_WAIT_FOREVER);
    OS_DeleteSemaphore(semStart);

    //perform http get
    //if(Https_Get(SERVER_IP,SERVER_PORT,SERVER_PATH,buffer,&len) < 0){
    if(Https_Post(SERVER_IP, SERVER_PORT, SERVER_PATH_POST, SERVER_DATA, 21, buffer, &len) < 0){
        Trace(1,"http get fail");
    }
    else{
        //show response message though tracer(pay attention:tracer can not show all the word if the body too long)
        Trace(1,"http get success,ret:%s",buffer);
        char* index0 = strstr(buffer,"\r\n\r\n");
        char temp = index0[4];
        index0[4] = '\0';
        Trace(1,"http response header:%s",buffer);
        index0[4] = temp;
        Trace(1,"http response body:%s",index0+4);
    }
}

void test_MainTask(void* param)
{
    API_Event_t* event=NULL;

    Socket_BIO_Test();
    
    while(1)
    {
        if(OS_WaitEvent(socketTaskHandle, (void**)&event, OS_TIME_OUT_WAIT_FOREVER))
        {
            // EventDispatch(event);
            OS_Free(event->pParam1);
            OS_Free(event->pParam2);
            OS_Free(event);
        }
    }
}


void socket_MainTask(void *pData)
{
    API_Event_t* event=NULL;

    testTaskHandle = OS_CreateTask(test_MainTask,
        NULL, NULL, TEST_TASK_STACK_SIZE, TEST_TASK_PRIORITY, 0, 0, TEST_TASK_NAME);

    while(1)
    {
        if(OS_WaitEvent(socketTaskHandle, (void**)&event, OS_TIME_OUT_WAIT_FOREVER))
        {
            EventDispatch(event);
            OS_Free(event->pParam1);
            OS_Free(event->pParam2);
            OS_Free(event);
        }
    }
}

void httpsConnection_Main()
{
    socketTaskHandle = OS_CreateTask(socket_MainTask,
        NULL, NULL, MAIN_TASK_STACK_SIZE, MAIN_TASK_PRIORITY, 0, 0, MAIN_TASK_NAME);
    OS_SetUserMainHandle(&socketTaskHandle);
}

