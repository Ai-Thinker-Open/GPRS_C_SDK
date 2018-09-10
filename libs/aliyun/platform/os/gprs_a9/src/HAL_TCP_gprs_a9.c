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
#include <string.h>
#include <unistd.h>

#include "iot_import.h"

#include "time.h"
#include "api_socket.h"
#include "errno.h"


#define PLATFORM_GPRS_A9_SOCK_LOG(format, ...) \
    do { \
        HAL_Printf("GPRS_A9_SOCK %u %s() | "format"\n", __LINE__, __FUNCTION__, ##__VA_ARGS__);\
        fflush(stdout);\
    } while(0);


static uint64_t _gprs_a9_get_time_ms(void)
{
    struct timeval tv = { 0 };
    uint64_t time_ms;

    gettimeofday(&tv, NULL);

    time_ms = tv.tv_sec * 1000 + tv.tv_usec / 1000;

    return time_ms;
}

static uint64_t _gprs_a9_time_left(uint64_t t_end, uint64_t t_now)
{
    uint64_t t_left;

    if (t_end > t_now) {
        t_left = t_end - t_now;
    } else {
        t_left = 0;
    }

    return t_left;
}

uintptr_t HAL_TCP_Establish(const char *host, uint16_t port)
{
    int fd = 0;
    char tmp[16];
    struct sockaddr_in sockaddr;
    int ret = 0;

    PLATFORM_GPRS_A9_SOCK_LOG("establish tcp connection with server(host=%s port=%u)", host, port);

    ret = DNS_GetHostByName2(host,tmp);
    if(ret <0 )
    {
        perror("get ip by hostname fail");
        return 0;   
    }
    fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (fd < 0) {
        perror("create socket error");
        return 0;
    }


    memset(&sockaddr,0,sizeof(sockaddr));
    sockaddr.sin_family = AF_INET;
    sockaddr.sin_port = htons(port);
    inet_pton(AF_INET,tmp,&sockaddr.sin_addr);

    ret = connect(fd, (struct sockaddr*)&sockaddr, sizeof(struct sockaddr_in));
    if(ret == 0)
    {
        PLATFORM_GPRS_A9_SOCK_LOG("success to establish tcp connection,fd=%d",fd);
        return fd;
    }
    close(fd);
    perror("connect error");
    PLATFORM_GPRS_A9_SOCK_LOG("fail to establish tcp connection");
    return 0;
}


int HAL_TCP_Destroy(uintptr_t fd)
{
    int rc;

    /* Shutdown both send and receive operations. */
    rc = shutdown((int) fd, 2);
    if (0 != rc) {
        perror("shutdown error");
        return -1;
    }

    rc = close((int) fd);
    if (0 != rc) {
        perror("closesocket error");
        return -1;
    }

    return 0;
}


int32_t HAL_TCP_Write(uintptr_t fd, const char *buf, uint32_t len, uint32_t timeout_ms)
{
    int ret;
    uint32_t len_sent;
    uint64_t t_end, t_left;
    fd_set sets;

    t_end = _gprs_a9_get_time_ms() + timeout_ms;
    len_sent = 0;
    ret = 1; /* send one time if timeout_ms is value 0 */

    do {
        t_left = _gprs_a9_time_left(t_end, _gprs_a9_get_time_ms());

        if (0 != t_left) {
            struct timeval timeout;

            FD_ZERO(&sets);
            FD_SET(fd, &sets);

            timeout.tv_sec = t_left / 1000;
            timeout.tv_usec = (t_left % 1000) * 1000;

            ret = select(fd + 1, NULL, &sets, NULL, &timeout);
            if (ret > 0) {
                if (0 == FD_ISSET(fd, &sets)) {
                    PLATFORM_GPRS_A9_SOCK_LOG("Should NOT arrive");
                    /* If timeout in next loop, it will not sent any data */
                    ret = 0;
                    continue;
                }
            } else if (0 == ret) {
                PLATFORM_GPRS_A9_SOCK_LOG("select-write timeout %d", (int)fd);
                break;
            } else {
                if (EINTR == errno) {
                    PLATFORM_GPRS_A9_SOCK_LOG("EINTR be caught");
                    continue;
                }

                perror("select-write fail");
                break;
            }
        }

        if (ret > 0) {
            ret = send(fd, buf + len_sent, len - len_sent, 0);
            if (ret > 0) {
                len_sent += ret;
            } else if (0 == ret) {
                PLATFORM_GPRS_A9_SOCK_LOG("No data be sent");
            } else {
                if (EINTR == errno) {
                    PLATFORM_GPRS_A9_SOCK_LOG("EINTR be caught");
                    continue;
                }

                perror("send fail");
                break;
            }
        }
    } while ((len_sent < len) && (_gprs_a9_time_left(t_end, _gprs_a9_get_time_ms()) > 0));

    return len_sent;
}


int32_t HAL_TCP_Read(uintptr_t fd, char *buf, uint32_t len, uint32_t timeout_ms)
{
    int ret, err_code;
    uint32_t len_recv;
    uint64_t t_end, t_left;
    fd_set sets;
    struct timeval timeout;

    t_end = _gprs_a9_get_time_ms() + timeout_ms;
    len_recv = 0;
    err_code = 0;

    do {
        t_left = _gprs_a9_time_left(t_end, _gprs_a9_get_time_ms());
        if (0 == t_left) {
            break;
        }
        FD_ZERO(&sets);
        FD_SET(fd, &sets);

        timeout.tv_sec = t_left / 1000;
        timeout.tv_usec = (t_left % 1000) * 1000;

        ret = select(fd + 1, &sets, NULL, NULL, &timeout);
        if (ret > 0) {
            ret = recv(fd, buf + len_recv, len - len_recv, 0);
            if (ret > 0) {
                len_recv += ret;
            } else if (0 == ret) {
                perror("connection is closed");
                err_code = -1;
                break;
            } else {
                if (EINTR == errno) {
                    PLATFORM_GPRS_A9_SOCK_LOG("EINTR be caught");
                    continue;
                }
                perror("recv fail");
                err_code = -2;
                break;
            }
        } else if (0 == ret) {
            break;
        } else {
            perror("select-recv fail");
            err_code = -2;
            break;
        }
    } while ((len_recv < len));

    /* priority to return data bytes if any data be received from TCP connection. */
    /* It will get error code on next calling */
    return (0 != len_recv) ? len_recv : err_code;
}
