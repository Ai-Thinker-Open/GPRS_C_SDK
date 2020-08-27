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
#include <stdlib.h>
#include <stdarg.h>

#include <unistd.h>

#include "iot_import.h"
#ifdef MQTT_ID2_AUTH
#include "tfs.h"
#endif /**< MQTT_ID2_AUTH*/

#include "api_os.h"
#include "time.h"
#include "api_fs.h"




char _product_key[PRODUCT_KEY_LEN + 1];
char _product_secret[PRODUCT_SECRET_LEN + 1];
char _device_name[DEVICE_NAME_LEN + 1];
char _device_secret[DEVICE_SECRET_LEN + 1];
#define UNUSED(expr) do { (void)(expr); } while (0)

void *HAL_MutexCreate(void)
{
    HANDLE mutex = OS_CreateMutex();
    return (void*)mutex;
}

void HAL_MutexDestroy(_IN_ void *mutex)
{
    OS_DeleteMutex((HANDLE)mutex);
}

void HAL_MutexLock(_IN_ void *mutex)
{
    OS_LockMutex((HANDLE)mutex);
}

void HAL_MutexUnlock(_IN_ void *mutex)
{
    OS_UnlockMutex((HANDLE)mutex);
}

void *HAL_Malloc(_IN_ uint32_t size)
{
    return OS_Malloc(size);
}

void HAL_Free(_IN_ void *ptr)
{
    OS_Free(ptr);
}


uint64_t HAL_UptimeMs(void)
{
    uint64_t time_ms = (uint64_t)(clock()/CLOCKS_PER_MSEC);
    return time_ms;
}

void HAL_SleepMs(_IN_ uint32_t ms)
{
    OS_Sleep(ms);
}

void HAL_Srandom(uint32_t seed)
{
    srand(seed);
}

uint32_t HAL_Random(uint32_t region)
{
    return (region > 0) ? (rand() % region) : 0;
}

int HAL_Snprintf(_IN_ char *str, const int len, const char *fmt, ...)
{
    va_list args;
    int     rc;

    va_start(args, fmt);
    rc = vsnprintf(str, len, fmt, args);
    va_end(args);

    return rc;
}

int HAL_Vsnprintf(_IN_ char *str, _IN_ const int len, _IN_ const char *format, va_list ap)
{
    return vsnprintf(str, len, format, ap);
}

void HAL_Printf(_IN_ const char *fmt, ...)
{
    va_list args;
    char uart_buf[256];

    memset(uart_buf,0,sizeof(uart_buf));
    va_start(args, fmt);
    vsnprintf(uart_buf, sizeof(uart_buf), fmt, args);
    va_end(args);

    printf(uart_buf);
}

int HAL_GetPartnerID(char* pid_str)
{
    memset(pid_str, 0x0, PID_STRLEN_MAX);
    strcpy(pid_str, "Ai-Thinker");
    return strlen(pid_str);
}

int HAL_GetModuleID(char* mid_str)
{
    memset(mid_str, 0x0, MID_STRLEN_MAX);
    strcpy(mid_str, "GPRS_A9");
    return strlen(mid_str);
}


char *HAL_GetChipID(_OU_ char* cid_str)
{
    memset(cid_str, 0x0, HAL_CID_LEN);
    strncpy(cid_str, "RDA8955", HAL_CID_LEN);
    cid_str[HAL_CID_LEN - 1] = '\0';
    return cid_str;
}


int HAL_GetDeviceID(_OU_ char* device_id)
{
    memset(device_id, 0x0, DEVICE_ID_LEN);
    HAL_Snprintf(device_id, DEVICE_ID_LEN, "%s.%s", _product_key, _device_name);
    device_id[DEVICE_ID_LEN - 1] = '\0';

    return strlen(device_id);
}

#ifdef MQTT_ID2_AUTH
int HAL_GetID2(_OU_ char* id2_str)
{
	int rc;
    uint8_t                 id2[TFS_ID2_LEN + 1] = {0};
	uint32_t                id2_len = TFS_ID2_LEN + 1;
    memset(id2_str, 0x0, TFS_ID2_LEN + 1);
    rc = tfs_get_ID2(id2, &id2_len);
    if (rc < 0) return rc;
    strncpy(id2_str, (const char*)id2, TFS_ID2_LEN);
    return strlen(id2_str);
}
#endif /**< MQTT_ID2_AUTH*/

int HAL_SetProductKey(_IN_ char* product_key)
{
    int len = strlen(product_key);
    if (len > PRODUCT_KEY_LEN) return -1;
    memset(_product_key, 0x0, PRODUCT_KEY_LEN + 1);
    strncpy(_product_key, product_key, len);
    return len;
}


int HAL_SetDeviceName(_IN_ char* device_name)
{
    int len = strlen(device_name);
    if (len > DEVICE_NAME_LEN) return -1;
    memset(_device_name, 0x0, DEVICE_NAME_LEN + 1);
    strncpy(_device_name, device_name, len);
    return len;
}


int HAL_SetDeviceSecret(_IN_ char* device_secret)
{
    int len = strlen(device_secret);
    if (len > DEVICE_SECRET_LEN) return -1;
    memset(_device_secret, 0x0, DEVICE_SECRET_LEN + 1);
    strncpy(_device_secret, device_secret, len);
    return len; 
}


int HAL_SetProductSecret(_IN_ char* product_secret)
{
    int len = strlen(product_secret);
    if (len > PRODUCT_SECRET_LEN) return -1;
    memset(_product_secret, 0x0, PRODUCT_SECRET_LEN + 1);
    strncpy(_product_secret, product_secret, len);
    return len;
}

int HAL_GetProductKey(_OU_ char* product_key)
{
    memset(product_key, 0x0, PRODUCT_KEY_LEN);
    strncpy(product_key, _product_key, PRODUCT_KEY_LEN);
    return strlen(product_key);
}

int HAL_GetProductSecret(_OU_ char* product_secret)
{
    memset(product_secret, 0x0, PRODUCT_SECRET_LEN);
    strncpy(product_secret, _product_secret, PRODUCT_SECRET_LEN);
    return strlen(product_secret);
}

int HAL_GetDeviceName(_OU_ char* device_name)
{
    memset(device_name, 0x0, DEVICE_NAME_LEN);
    strncpy(device_name, _device_name, DEVICE_NAME_LEN);
    return strlen(device_name);
}


int HAL_GetDeviceSecret(_OU_ char* device_secret)
{
    memset(device_secret, 0x0, DEVICE_SECRET_LEN);
    strncpy(device_secret, _device_secret, DEVICE_SECRET_LEN);
    return strlen(device_secret);
}

int HAL_GetFirmwareVesion(_OU_ char* version)
{
    char *ver = "V1.0.0";
    int len = strlen(ver);
    if (len > FIRMWARE_VERSION_MAXLEN)
        return 0;
    memset(version, 0x0, FIRMWARE_VERSION_MAXLEN);
    strncpy(version, ver, FIRMWARE_VERSION_MAXLEN);
    return len;
}

static uint32_t fd;

#define otafilename "/tmp/alinkota.bin"

void HAL_Firmware_Persistence_Start(void)
{
    fd = API_FS_Open(otafilename, FS_O_WRONLY|FS_O_CREAT,0);
//    assert(fp);
}

int HAL_Firmware_Persistence_Write(_IN_ char *buffer, _IN_ uint32_t length)
{
    int32_t written_len = 0;

    written_len = API_FS_Write(fd, (uint8_t*)buffer, length);

    if (written_len != length) {
        return -1;
    }
    return 0;
}

int HAL_Firmware_Persistence_Stop(void)
{
    if (fd != NULL) {
        API_FS_Close(fd);
    }

    /* check file md5, and burning it to flash ... finally reboot system */

    return 0;
}


