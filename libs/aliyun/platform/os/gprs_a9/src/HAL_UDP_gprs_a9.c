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

#include "api_socket.h"
#include "errno.h"


void *HAL_UDP_create(char *host, unsigned short port)
{
    int fd = 0;
    char tmp[16];
    struct sockaddr_in sockaddr;
    int ret = 0;

    if (NULL == host) {
        return (void *)(-1);
    }    

    ret = DNS_GetHostByName2(host,tmp);
    if(ret <0 )
    {
        perror("get ip by hostname fail");
        return (void *)(-1);
    }

    fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (fd < 0) {
        perror("create socket error");
        return (void *)(-1);
    }

    memset(&sockaddr,0,sizeof(sockaddr));
    sockaddr.sin_family = AF_INET;
    sockaddr.sin_port = htons(port);
    inet_pton(AF_INET,tmp,&sockaddr.sin_addr);

    ret = connect(fd, (struct sockaddr*)&sockaddr, sizeof(struct sockaddr_in));
    if (ret == 0) {
        return (void *)(fd);
    }
    close(fd);

    return (void *)(-1);
}

void HAL_UDP_close(void *p_socket)
{
    int            socket_id = -1;

    socket_id = (int)p_socket;
    close(socket_id);
}

int HAL_UDP_write(void *p_socket,
                  const unsigned char *p_data,
                  unsigned int datalen)
{
    int             rc = -1;
    int             socket_id = -1;

    socket_id = (int)p_socket;
    rc = send(socket_id, (char *)p_data, (int)datalen, 0);
    if (-1 == rc) {
        return -1;
    }

    return rc;
}

int HAL_UDP_read(void *p_socket,
                 unsigned char *p_data,
                 unsigned int datalen)
{
    long            socket_id = -1;
    int             count = -1;

    if (NULL == p_data || NULL == p_socket) {
        return -1;
    }

    socket_id = (long)p_socket;
    count = (int)read(socket_id, p_data, datalen);

    return count;
}

int HAL_UDP_readTimeout(void *p_socket,
                        unsigned char *p_data,
                        unsigned int datalen,
                        unsigned int timeout)
{
    int                 ret;
    struct timeval      tv;
    fd_set              read_fds;
    long                socket_id = -1;

    if (NULL == p_socket || NULL == p_data) {
        return -1;
    }
    socket_id = (long)p_socket;

    if (socket_id < 0) {
        return -1;
    }

    FD_ZERO(&read_fds);
    FD_SET(socket_id, &read_fds);

    tv.tv_sec  = timeout / 1000;
    tv.tv_usec = (timeout % 1000) * 1000;

    ret = select(socket_id + 1, &read_fds, NULL, NULL, timeout == 0 ? NULL : &tv);

    /* Zero fds ready means we timed out */
    if (ret == 0) {
        return -2;    /* receive timeout */
    }

    if (ret < 0) {
        if (errno == EINTR) {
            return -3;    /* want read */
        }

        return -4; /* receive failed */
    }

    /* This call will not block */
    return HAL_UDP_read(p_socket, p_data, datalen);
}

