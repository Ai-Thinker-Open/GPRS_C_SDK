/*
 * Copyright (c) 2014-2016 Alibaba Group. All rights reserved.
 * License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>
#include "unistd.h"

#include "iot_import.h"
#include "api_ssl.h"
#include "api_os.h"
#include "api_socket.h"


#define SSL_LOG(format, ...) \
    do { \
        HAL_Printf("[inf] %s(%d): "format"\n", __FUNCTION__, __LINE__, ##__VA_ARGS__);\
        fflush(stdout);\
    }while(0);

int HAL_SSL_Read(uintptr_t handle, char *buf, int len, int timeout_ms)
{
    int ret;
    // SSL_LOG("HAL_SSL_Read timeout:%dms",timeout_ms);
    ret = SSL_Read((SSL_Config_t*)handle,buf,len,timeout_ms);
    return ret;
}

int HAL_SSL_Write(uintptr_t handle, const char *buf, int len, int timeout_ms)
{
    int ret;

    // SSL_LOG("write len:%d data:%s",strlen(buf),buf);
    ret = SSL_Write((SSL_Config_t*)handle,(uint8_t*)buf,len,timeout_ms);
    return ret;
}

int32_t HAL_SSL_Destroy(uintptr_t handle)
{
    if ((uintptr_t)NULL == handle) {
        SSL_LOG("handle is NULL");
        return 0;
    }
    SSL_Destroy((SSL_Config_t*)handle);
    return 0;
}

uintptr_t HAL_SSL_Establish(const char *host,
                            uint16_t port,
                            const char *ca_crt,
                            size_t ca_crt_len)
{
    SSL_Error_t error;
    SSL_Config_t* config = OS_Malloc(sizeof(SSL_Config_t));
    char tmp[10],ip[16];
    int ret = 0;


    if(config == NULL)
    {
        return (uintptr_t)(NULL);
    }
    config->caCert = ca_crt;
    config->caCrl  = NULL;
    config->clientCert = NULL;
    config->clientKey  = NULL;
    config->clientKeyPasswd = NULL;
    config->hostName   = host;
    config->minVersion = SSL_VERSION_TLSv1_2;
    config->maxVersion = SSL_VERSION_TLSv1_2;
    config->verifyMode = SSL_VERIFY_MODE_REQUIRED;
    config->entropyCustom = "GPRS";
    SSL_LOG("start init SSL");
    error = SSL_Init(config);
    if(error != SSL_ERROR_NONE)
    {
        SSL_LOG("ssl init error:%d",error);
        goto fail;
    }
    SSL_LOG("SSL Connect: %s:%d",host,port);

    ret = DNS_GetHostByName2(host,ip);
    if(ret != 0 )
    {
        perror("get ip by hostname fail");
        goto fail;
    }
    itoa(port,tmp,10);
    SSL_LOG("ip:%s,port:%s",ip,tmp);
    error = SSL_Connect(config,ip,tmp);
    if(error == SSL_ERROR_NONE)
    {
        SSL_LOG("ssl connect success");
        return (uintptr_t)(config);
    }
fail:    
    SSL_LOG("ssl connect fail:%d, destroy now",error);
    SSL_Destroy(config);
    OS_Free(config);
    return (uintptr_t)NULL;
}