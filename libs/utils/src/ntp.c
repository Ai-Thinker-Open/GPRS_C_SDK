#include <api_socket.h>
#include <ntp.h>
#include <api_debug.h>
#include <time.h>


#define NTP_TIMESTAMP_70_YEARS 2208988800UL

#define LI(packet)   (uint8_t) ((packet.li_vn_mode & 0xC0) >> 6) // (li   & 11 000 000) >> 6
#define VN(packet)   (uint8_t) ((packet.li_vn_mode & 0x38) >> 3) // (vn   & 00 111 000) >> 3
#define MODE(packet) (uint8_t) ((packet.li_vn_mode & 0x07) >> 0) // (mode & 00 000 111) >> 0

// Structure that defines the 48 byte NTP packet protocol.

typedef struct
{

    uint8_t li_vn_mode;      // Eight bits. li, vn, and mode.
                                // li.   Two bits.   Leap indicator.
                                // vn.   Three bits. Version number of the protocol.
                                // mode. Three bits. Client will pick mode 3 for client.

    uint8_t stratum;         // Eight bits. Stratum level of the local clock.
    uint8_t poll;            // Eight bits. Maximum interval between successive messages.
    uint8_t precision;       // Eight bits. Precision of the local clock.

    uint32_t rootDelay;      // 32 bits. Total round trip delay time.
    uint32_t rootDispersion; // 32 bits. Max error aloud from primary clock source.
    uint32_t refId;          // 32 bits. Reference clock identifier.

    uint32_t refTm_s;        // 32 bits. Reference time-stamp seconds.
    uint32_t refTm_f;        // 32 bits. Reference time-stamp fraction of a second.

    uint32_t origTm_s;       // 32 bits. Originate time-stamp seconds.
    uint32_t origTm_f;       // 32 bits. Originate time-stamp fraction of a second.

    uint32_t rxTm_s;         // 32 bits. Received time-stamp seconds.
    uint32_t rxTm_f;         // 32 bits. Received time-stamp fraction of a second.

    uint32_t txTm_s;         // 32 bits and the most important field the client cares about. Transmit time-stamp seconds.
    uint32_t txTm_f;         // 32 bits. Transmit time-stamp fraction of a second.

} ntp_packet;              // Total: 384 bits or 48 bytes.

int NTP_Update(const char* server, time_t timeoutS, time_t* timeNow, bool isSetRTC)
{
    int sockfd, n; // Socket file descriptor and the n return result from writing/reading from the socket.

    int portno = 123; // NTP UDP port number.
    uint8_t ip[16];
    int ret;
    bool flag = false;
    uint32_t t4;
    TIME_System_t sysTime;
    char* host_name = (char*)server; // NTP server host-name.
    ntp_packet packet;

    memset( &packet, 0, sizeof( ntp_packet ) );

    // Set the first byte's bits to 00,011,011 for li = 0, vn = 3, and mode = 3. The rest will be left set to zero.

    // Create a UDP socket, convert the host-name to an IP address, set the port number,
    // connect to the server, send the packet, and then read in the return packet.

    struct sockaddr_in serv_addr; // Server address data structure.

    sockfd = socket( AF_INET, SOCK_DGRAM, IPPROTO_UDP ); // Create a UDP socket.
    if ( sockfd < 0 )
    {
        Trace(1,"get socket error");
        return -1;
    }

    if(DNS_GetHostByName2( host_name, ip)!=0) // Convert URL to IP.
    {
        Trace(1,"get ip error");
        return -11;
    }

    memset(&serv_addr,0,sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(portno);
    inet_pton(AF_INET,ip,&serv_addr.sin_addr);

    if ( connect( sockfd, ( struct sockaddr * ) &serv_addr, sizeof( serv_addr) ) < 0 )
    {
        Trace(1,"connect error");
        close(sockfd);
        return -2;
    }


    packet.li_vn_mode = (0x00<<6)|(0x04<<3)|0x03; //not sync, version 4, client mode
    packet.txTm_s = ntohl( (uint32_t)time(NULL) + NTP_TIMESTAMP_70_YEARS );
    n = write( sockfd, ( char* ) &packet, sizeof( ntp_packet ) );
    if ( n < 0 )
    {
        Trace(1,"send request error");
        close(sockfd);
        return -3;
    }  

    struct fd_set fds;
    struct timeval timeout={timeoutS,0};
    uint16_t recvLen = 0;
    int retBufferLen = sizeof(ntp_packet);
    FD_ZERO(&fds);
    FD_SET(sockfd,&fds);
    while(!flag)
    {
        ret = select(sockfd+1,&fds,NULL,NULL,&timeout);
        switch(ret)
        {
            case -1:
                Trace(1,"select error");
                close(sockfd);
                return -4;
            case 0:
                Trace(1,"select timeout");
                close(sockfd);
                return -5;
            default:
                if(FD_ISSET(sockfd,&fds))
                {
                    Trace(1,"select return:%d",ret);
                    ret = read(sockfd,(char*)&packet+recvLen,retBufferLen-recvLen);
                    Trace(1,"ret:%d",ret);
                    recvLen += ret;
                    if(ret < 0)
                    {
                        Trace(1,"recv error");
                        close(sockfd);
                        return -6;
                    }
                    else if(ret == 0)
                    {
                        Trace(1,"ret == 0");
                        close(sockfd);
                        return -7;
                    }
                    else
                    {
                        if(recvLen==retBufferLen)
                        {
                        Trace(1,"recv len:%d",recvLen);
                        close(sockfd);
                        flag = true;
                        break;
                        }
                    }                  
                    
                }
                break;
        }
    }
    // These two fields contain the time-stamp seconds as the packet left the NTP server.
    // The number of seconds correspond to the seconds passed since 1900.
    // ntohl() converts the bit/byte order from the network's to host's "endianness".

    packet.refTm_s = ntohl( packet.refTm_s ); // Time-stamp seconds.
    // packet.refTm_f = ntohl( packet.refTm_f ); // Time-stamp fraction of a second.
    packet.origTm_s = ntohl( packet.origTm_s ); // Time-stamp seconds.
    // packet.origTm_f = ntohl( packet.origTm_f ); // Time-stamp fraction of a second.
    packet.rxTm_s = ntohl( packet.rxTm_s ); // Time-stamp seconds.
    // packet.rxTm_f = ntohl( packet.rxTm_f ); // Time-stamp fraction of a second.
    packet.txTm_s = ntohl( packet.txTm_s ); // Time-stamp seconds.
    // packet.txTm_f = ntohl( packet.txTm_f ); // Time-stamp fraction of a second.

    // Extract the 32 bits that represent the time-stamp seconds (since NTP epoch) from when the packet left the server.
    // Subtract 70 years worth of seconds from the seconds since 1900.
    // This leaves the seconds since the UNIX epoch of 1970.
    // (1900)------------------(1970)**************************************(Time Packet Left the Server)

    t4 = time(NULL) + NTP_TIMESTAMP_70_YEARS;//time from 1900
    int32_t delta = (int32_t)(( (packet.rxTm_s - packet.origTm_s)+(packet.txTm_s-t4) ) / 2);
    *timeNow = t4 + delta - NTP_TIMESTAMP_70_YEARS; //time utc(from 1970)

    if(isSetRTC)
    {
        TIME_TimeStamp2SystemTime(*timeNow - TIME_2000_1970_S, &sysTime);
        if(!TIME_SetSystemTime(&sysTime))
            return -8;
    }
    return 0;
}

