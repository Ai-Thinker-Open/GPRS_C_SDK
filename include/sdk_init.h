#ifndef SDK_INIT_H
#define SDK_INIT_H

#include <string.h>
// #include <cs_types.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <sdk_init.h>
#include <api_inc_fs.h>
#include <api_inc_gpio.h>
#include <api_inc_uart.h>
#include <api_inc_network.h>
#include <api_inc_socket.h>
#include <api_inc_os.h>
#include <api_inc_sim.h>
#include <api_inc_sms.h>
#include <api_inc_time.h>
#include <api_inc_pm.h>
#include <api_inc_call.h>
#include <api_inc_adc.h>
#include <api_inc_mqtt.h>
#include <api_inc_charset.h>
#include <api_inc_i2c.h>
#include <api_inc_ssl.h>
#include <api_inc_spi.h>
#include <api_inc_fota.h>
#include <api_inc_ss.h>
#include <stdarg.h>
#include <api_inc_gizwits.h>
#include <api_inc_audio.h>
#include <setjmp_inc.h>
#include <api_hal_gouda_inc.h>
#include <api_inc_flash.h>





typedef struct T_INTERFACE_VTBL_TAG
{
    //debug
    bool                (*Trace)(uint16_t nIndex,const char* fmt, ...) __attribute__((format(printf, 2, 3)));
    void                (*MEMBLOCK_Trace)(uint16_t nIndex, uint8_t *buffer, uint16_t len, uint8_t radix);
    void                (*__assert)(const char* fmt);

    //power
    bool                (*PM_PowerEnable)(Power_Type_t powerType, bool isOn);
    void                (*PM_SleepMode)(bool isSleepMode);
    uint16_t            (*PM_Voltage)(uint8_t* percent);
    void                (*PM_ShutDown)();
    void                (*PM_Restart)();
    void                (*PM_SetSysMinFreq)(PM_Sys_Freq_t freq);

    /*api_os*/
    void                (*OS_SetUserMainHandle)(HANDLE* UserHandle);
    HANDLE              (*OS_GetUserMainHandle)();
    HANDLE              (*OS_CreateTask)(PTASK_FUNC_T pTaskEntry,void* pParameter,void* pStackAddr,uint16_t nStackSize
                                            ,uint8_t nPriority,uint16_t nCreationFlags,uint16_t nTimeSlice,const char* pTaskName);
    void                (*OS_StartTask)(HANDLE hTask,void* pParameter);
    void                (*OS_StopTask)(HANDLE hTask);
    bool                (*OS_DeleteTask)(HANDLE hTask);
    bool                (*OS_SuspendTask)(HANDLE hTask);
    bool                (*OS_ResumeTask)(HANDLE hTask);
    bool                (*OS_Sleep)(uint32_t nMillisecondes);
    void                (*OS_SleepUs)(uint32_t us);
    bool                (*OS_WaitEvent)(HANDLE hTask,void** pEvent,uint32_t nTimeOut);
    bool                (*OS_SendEvent)(HANDLE hTask,void* pEvent,uint32_t nTimeOut,uint16_t nOption);
    bool                (*OS_ResetEventQueue)(HANDLE hTask);
    bool                (*OS_IsEventAvailable)(HANDLE hTask);
    void*               (*OS_Malloc)(uint32_t nSize);
    void*               (*OS_Realloc)(void* ptr,uint32_t nSize);
    bool                (*OS_Free)(void* pMemBlock);
    bool                (*OS_GetHeapUsageStatus)(OS_Heap_Status_t *pOsHeapStatus);
    HANDLE              (*OS_CreateSemaphore)(uint32_t nInitCount);
    bool                (*OS_DeleteSemaphore)(HANDLE hSem);
    bool                (*OS_WaitForSemaphore)(HANDLE hSem,uint32_t nTimeOut);
    bool                (*OS_ReleaseSemaphore)(HANDLE hSem);
    HANDLE              (*OS_CreateMutex)(void);
    void                (*OS_DeleteMutex)(HANDLE mutex);
    void                (*OS_LockMutex)(HANDLE mutex);
    void                (*OS_UnlockMutex)(HANDLE mutex);
    bool                (*OS_StartCallbackTimer)(HANDLE hTask, uint32_t ms, OS_CALLBACK_FUNC_T callback, void* param);
    bool                (*OS_StopCallbackTimer)(HANDLE hTask, OS_CALLBACK_FUNC_T callback, void *param);
    uint32_t            (*OS_QueryCallbackTimer)(HANDLE hTask, OS_CALLBACK_FUNC_T callback, void *param);
    bool                (*OS_GetTaskInfo)(HANDLE pHTask, OS_Task_Info_t* taskInfo);


    /*api_hal_gpio*/
    bool                (*GPIO_Init)(GPIO_config_t config);
    void                (*GPIO_GetConfig)(GPIO_PIN pin,GPIO_config_t* config); 
    bool                (*GPIO_SetLevel)(GPIO_config_t gpioConf, GPIO_LEVEL  level);
    bool                (*GPIO_Set)(GPIO_PIN pin, GPIO_LEVEL  level);
    bool                (*GPIO_GetLevel)(GPIO_config_t gpioConf, GPIO_LEVEL* level);
    bool                (*GPIO_Get)(GPIO_PIN pin, GPIO_LEVEL* level);
    bool                (*GPIO_ChangeMode)(GPIO_PIN pin, GPIO_MODE mode);
    bool                (*GPIO_Close)(GPIO_PIN pin);

    /*api_hal_uart*/
    bool                (*UART_Init)(UART_Port_t uartN, UART_Config_t config);
    uint32_t            (*UART_Write)(UART_Port_t uartN, uint8_t* dataToWrite, uint32_t length);
    uint32_t            (*UART_Read)(UART_Port_t uartN, uint8_t* dataToRead, uint32_t length, uint32_t timeOutMs);
    bool                (*UART_Close)(UART_Port_t uartN);
    
    /*api_gps*/     
    bool                (*GPS_Open)(UART_Callback_t);
    bool                (*GPS_Close)(void);
    bool                (*GPS_IsOpen)();
    
    /*api_network*/
    void                (*Network_SetStatusChangedCallback)(Network_Callback_Func_t callback);
    bool                (*Network_StartAttach)(void);
    bool                (*Network_StartDetach)(void);
    bool                (*Network_StartActive)(Network_PDP_Context_t context);
    bool                (*Network_StartDeactive)(uint8_t contextID);
    bool                (*Network_GetIp)(char* ip, uint8_t size);
    bool                (*Network_GetCellInfoRequst)();
    bool                (*Network_GetAttachStatus)(uint8_t* status);
    bool                (*Network_GetActiveStatus)(uint8_t* status);
    bool                (*Network_GetSignalQuality)(Network_Signal_Quality_t* sq);
    bool                (*Network_GetStatusInfo)(Network_Status_Info_t* status);
    bool                (*Network_SetFlightMode)(bool enable);
    bool                (*Network_GetFlightMode)(bool* enable);
    bool                (*Network_Register)(uint8_t* operatorID, Network_Register_Mode_t mode);
    bool                (*Network_DeRegister)();
    bool                (*Network_SetFrequencyBand)(uint8_t freqBand);
    bool                (*Network_GetCurrentOperator)(uint8_t operatorId[6], Network_Register_Mode_t* mode );
    bool                (*Network_GetOperatorInfo)(uint8_t index, uint8_t** operatorId, uint8_t** operatorName);
    bool                (*Network_GetOperatorNameById)(uint8_t* operatorId, uint8_t** operatorName);
    bool                (*Network_GetOperatorIdByName)(uint8_t* operatorName,uint8_t** operatorId);
    bool                (*Network_GetAvailableOperatorReq)();
    
    /*api_socket*/
    int                 (*Socket_TcpipConnect)(TCP_UDP_t tcpOrUdp, const char* ip,uint16_t port);
    int                 (*Socket_TcpipWrite)(int socketFd, uint8_t* data, uint16_t length);
    int                 (*Socket_TcpipRead)( int socketFd, uint8_t* data, uint16_t length);
    bool                (*Socket_TcpipClose)(int socketFd);
    DNS_Status_t        (*DNS_GetHostByName)(const char* domain, char* ip);
    DNS_Status_t        (*DNS_GetHostByNameEX)(const char *hostname, char* ip, DNS_CALLBACK_FUNC_T func, void* param);
    int32_t             (*DNS_GetHostByName2)(const uint8_t* domain, uint8_t* ip);
    int                 (*lwip_accept)(int s, struct sockaddr *addr, socklen_t *addrlen);
    int                 (*lwip_bind)(int s, const struct sockaddr *name, socklen_t namelen);
    int                 (*lwip_shutdown)(int s, int how);
    int                 (*lwip_getpeername) (int s, struct sockaddr *name, socklen_t *namelen);
    int                 (*lwip_getsockname) (int s, struct sockaddr *name, socklen_t *namelen);
    int                 (*lwip_getsockopt) (int s, int level, int optname, void *optval, socklen_t *optlen);
    int                 (*lwip_setsockopt) (int s, int level, int optname, const void *optval, socklen_t optlen);
    int                 (*lwip_close)(int s);
    int                 (*lwip_connect)(int s, const struct sockaddr *name, socklen_t namelen);
    int                 (*lwip_listen)(int s, int backlog);
    ssize_t             (*lwip_recv)(int s, void *mem, size_t len, int flags);
    ssize_t             (*lwip_read)(int s, void *mem, size_t len);
    ssize_t             (*lwip_recvfrom)(int s, void *mem, size_t len, int flags, struct sockaddr *from, socklen_t *fromlen);
    ssize_t             (*lwip_recvmsg)(int s, struct msghdr *message, int flags);
    ssize_t             (*lwip_send)(int s, const void *dataptr, size_t size, int flags);
    ssize_t             (*lwip_sendmsg)(int s, const struct msghdr *message, int flags);
    ssize_t             (*lwip_sendto)(int s, const void *dataptr, size_t size, int flags, const struct sockaddr *to, socklen_t tolen);
    int                 (*lwip_socket)(int domain, int type, int protocol);
    ssize_t             (*lwip_write)(int s, const void *dataptr, size_t size);
    ssize_t             (*lwip_writev)(int s, const struct iovec *iov, int iovcnt);
    int                 (*lwip_select)(int maxfdp1, fd_set *readset, fd_set *writeset, fd_set *exceptset,struct timeval *timeout);
    int                 (*lwip_ioctl)(int s, long cmd, void *argp);
    int                 (*lwip_fcntl)(int s, int cmd, int val);
    char*               (*ip6addr_ntoa_r)(const ip6_addr_t *addr, char *buf, int buflen);
    char*               (*ip4addr_ntoa_r)(const ip4_addr_t *addr, char *buf, int buflen);
    char*               (*ip6addr_ntoa)(const ip6_addr_t *addr);
    char*               (*ip4addr_ntoa)(const ip4_addr_t *addr);
    int                 (*ip6addr_aton)(const char *cp, ip6_addr_t *addr);
    int                 (*ip4addr_aton)(const char *cp, ip4_addr_t *addr);
    uint16_t            (*lwip_htons)(uint16_t x);
    uint32_t            (*lwip_htonl)(uint32_t x);
    const char*         (*lwip_strerr)(err_t err);
    int                 (*err_to_errno)(err_t err);
    int                 (*Socket_GetLastError)(void);
    

    
    /*time*/
    int                 (*gettimeofday)(timeval_t *tv, timezone_t *tz);
    void                (*TIME_SetIsAutoUpdateRtcTime)(bool isAutoUpdate);
    bool                (*TIME_IsAutoUpdateRtcTime)();
    bool                (*TIME_SetRtcTime)(RTC_Time_t*);
    bool                (*TIME_GetRtcTime)(RTC_Time_t*);
    long                (*clock)(void);
    uint32_t            (*TIME_GetTime)();
    bool                (*TIME_GetSystemTime)(TIME_System_t* sysTime);
    bool                (*TIME_SetSystemTime)(TIME_System_t* sysTime);
    int8_t              (*TIME_GetTimeZone)();
    bool                (*TIME_SetTimeZone)(int8_t timeZone);
    bool                (*TIME_GetLocalTime)(TIME_System_t* localTime);
    bool                (*TIME_SetLocalTime)(TIME_System_t* localTime);
    uint32_t            (*TIME_SystemTime2TimeStamp)(TIME_System_t* sysTime);
    bool                (*TIME_TimeStamp2SystemTime)(uint32_t stamp, TIME_System_t* sysTime);
    time_t              (*time)(time_t* timer);
    time_t              (*timelocal)(struct tm* tm);
    time_t              (*mktime)(struct tm* timer);
    
    /*sms*/
    bool                (*SMS_SetFormat)(SMS_Format_t format, SIM_ID_t simID);
    bool                (*SMS_SetParameter)(SMS_Parameter_t* smsParameter,SIM_ID_t simID);
    bool                (*SMS_SendMessage)(const char* phoneNumber, const uint8_t* message,  uint8_t length, SIM_ID_t simID);
    bool                (*SMS_SetServerCenterInfo)(SMS_Server_Center_Info_t* serverCenterInfo);
    bool                (*SMS_GetServerCenterInfo)(SMS_Server_Center_Info_t* serverCenterInfo);
    bool                (*SMS_ListMessageRequst)(SMS_Status_t smsStatus,SMS_Storage_t storage);
    bool                (*SMS_DeleteMessage)(uint8_t index,SMS_Status_t status,SMS_Storage_t storage);
    bool                (*SMS_GetStorageInfo)(SMS_Storage_Info_t* storageInfo, SMS_Storage_t storage);
    bool                (*SMS_SetNewMessageStorage)(SMS_Storage_t storage);
    const char*         (*SMS_GetCharset)(Charset_t);
    bool                (*SMS_Unicode2LocalLanguage)(uint8_t* unicodeIn, uint16_t unicodeLenIn, Charset_t localLanguage, uint8_t** localOut, uint32_t* localLenOut);
    bool                (*SMS_LocalLanguage2Unicode)(uint8_t* localIn, uint16_t localLenIn, Charset_t localLanguage, uint8_t** unicodeOut, uint32_t* unicodeLenOut);
    
    //charset
    const char*         (*GetCharset)(Charset_t index);
    bool                (*Unicode2LocalLanguage)(uint8_t* unicodeIn, uint16_t unicodeLenIn, Charset_t localLanguage, uint8_t** localOut, uint32_t* localLenOut);
    bool                (*LocalLanguage2Unicode)(uint8_t* localIn, uint16_t localLenIn, Charset_t localLanguage, uint8_t** unicodeOut, uint32_t* unicodeLenOut);
    bool                (*Unicode2LocalLanguageBigEndian)(uint8_t* unicodeIn, uint16_t unicodeLenIn, Charset_t localLanguage, uint8_t** localOut, uint32_t* localLenOut);
    bool                (*LocalLanguage2UnicodeBigEndian)(uint8_t* localIn, uint16_t localLenIn, Charset_t localLanguage, uint8_t** unicodeOut, uint32_t* unicodeLenOut);
    int32_t             (*GSM_8BitTo7Bit)(const uint8_t* pSrc, uint8_t* pDest, uint16_t nSrcSize );
    int32_t             (*GSM_7BitTo8Bit)(const uint8_t* pSrc, uint8_t* pDest, uint16_t nSrcSize );

    /*fs*/
    void                (*API_FS_SetUseOldVersion)(bool useOldVersion);
    int32_t             (*API_FS_Open)(const char*    fileName,uint32_t operationFlag,uint32_t mode);
    int32_t             (*API_FS_Close)(int32_t fd);
    int32_t             (*API_FS_Read)(int32_t  fd,uint8_t* pBuffer,uint32_t length);
    int32_t             (*API_FS_Write)(int32_t  fd,uint8_t* pBuffer,uint32_t length);
    uint32_t            (*API_FS_Flush)(int32_t fd);
    int32_t             (*API_FS_Create)(const char* fileName,uint32_t mode);
    int32_t             (*API_FS_Delete)(const char* fileName);
    int64_t             (*API_FS_Seek)( int32_t  fd, int64_t  offset, uint8_t  origin);
    int32_t             (*API_FS_IsEndOfFile)(int32_t fd);
    int32_t             (*API_FS_Rename)(const char* oldName,const char* newName);
    int32_t             (*API_FS_GetFileName)(int32_t  fd, int32_t  nameBufferLen, uint8_t* fileName);
    int64_t             (*API_FS_GetFileSize)(int32_t fd);
    int64_t             (*API_FS_GetDirSize)(const char* fileName, uint64_t* size);
    int32_t             (*API_FS_GetCurDir)(uint32_t size, char* pCurDir);
    int32_t             (*API_FS_ChangeDir)(const char* pDirName);
    int32_t             (*API_FS_Mkdir)(const char* fileName,uint32_t mode);
    int32_t             (*API_FS_Rmdir)(const char* fileName);
    int32_t             (*API_FS_GetFSInfo)(const char* pDevName, API_FS_INFO* pFsInfo);
    Dir_t*              (*API_FS_OpenDir)(const char* name);
    Dirent_t*           (*API_FS_ReadDir)(Dir_t* pDir);
    int                 (*API_FS_ReadDir_r)(Dir_t *pDir, Dirent_t *entry, Dirent_t **outDirent);
    long                (*API_FS_TellDir)(Dir_t *pDir);
    void                (*API_FS_SeekDir)(Dir_t *pDir, long loc);
    void                (*API_FS_RewindDir)(Dir_t *pDir);
    int                 (*API_FS_CloseDir)(Dir_t *pDir);
    char*               (*API_FS_RealPath)(const char* path, char* resolvedPath);

    /*info*/
    bool                (*INFO_GetIMEI)(uint8_t* pImei);

    //audio
    void                (*AUDIO_MicOpen)();
    void                (*AUDIO_MicClose)();
    bool                (*AUDIO_MicSetMute)(bool isMute);
    void                (*AUDIO_SetMode)(AUDIO_Mode_t mode);
    void                (*AUDIO_SpeakerOpen)();
    void                (*AUDIO_SpeakerClose)();
    bool                (*AUDIO_SpeakerSetMute)(bool isMute);
    bool                (*AUDIO_SpeakerSetVolume)(uint8_t volume);
    AUDIO_Error_t       (*AUDIO_Play)(const char* fileName,AUDIO_Type_t type, AUDIO_PLAY_CALLBACK_t callback);
    AUDIO_Error_t       (*AUDIO_SetEQ)(AUDIO_EQ_t eq);
    AUDIO_Error_t       (*AUDIO_Pause)(void);
    AUDIO_Error_t       (*AUDIO_Resume)(HANDLE fileFd);
    AUDIO_Error_t       (*AUDIO_Stop)(void);
    AUDIO_Error_t       (*AUDIO_GetPlayInfo)(AUDIO_Play_Info_t* playInfo);
    bool                (*AUDIO_LoopTestStart)(AUDIO_Mode_t mode);
    bool                (*AUDIO_LoopTestEnd)();
    AUDIO_Error_t       (*AUDIO_RecordStart)(AUDIO_Type_t format, AUDIO_Record_Mode_t mode, int fd, AUDIO_FILE_RECORD_CALLBACK_t callback, AUDIO_BUFFER_DATA_CALLBACK_t dataCallback);
    bool                (*AUDIO_RecordStop)(void);


    //call
    bool                (*CALL_Dial)(const char* number);
    bool                (*CALL_HangUp)();
    bool                (*CALL_Answer)();
    bool                (*CALL_DTMF)(char dtmf, CALL_DTMF_Gain_t attenuation, uint8_t duration, uint8_t speakerGain, bool send);
    bool                (*CALL_Status)(CALL_Status_t** callStatus, uint8_t* count);

    //adc
    void                (*ADC_Init)(ADC_Config_t adcConfig);
    void                (*ADC_Close)(ADC_Channel_t channel);
    bool                (*ADC_Read)(ADC_Channel_t channel, uint16_t* value, uint16_t* mV);

    //mqtt
    MQTT_Client_t*      (*MQTT_ClientNew)();
    void                (*MQTT_ClientDestroy)(MQTT_Client_t* client);
    MQTT_Error_t        (*MQTT_Connect)(MQTT_Client_t* client, const char* domainOrIp,uint16_t port, 
                                    MQTT_Connection_Callback_t callback, void* arg, const MQTT_Connect_Info_t* connectInfo);
    MQTT_Error_t        (*MQTT_Disconnect)(MQTT_Client_t* client);
    void                (*MQTT_SetInPubCallback)(MQTT_Client_t* client, MQTT_InPub_Callback_t inpubCallback, MQTT_InPub_Data_Callback_t dataInpubCallback, void* arg);
    MQTT_Error_t        (*MQTT_Subscribe)(MQTT_Client_t* client, const char* topic, uint8_t qos, MQTT_Request_Callback_t callback, void* arg);
    MQTT_Error_t        (*MQTT_Unsubscribe)(MQTT_Client_t* client, const char* topic, MQTT_Request_Callback_t cb, void* arg);
    MQTT_Error_t        (*MQTT_Publish)(MQTT_Client_t* client, const char* topic, const void* payload, uint16_t payloadLen, uint8_t dup, uint8_t qos, uint8_t retain,
                                    MQTT_Request_Callback_t callback, void* arg);
    uint8_t             (*MQTT_IsConnected)(MQTT_Client_t* client);

    //sim
    bool                (*SIM_GetICCID)(uint8_t* iccid);
    bool                (*SIM_GetIMSI)(uint8_t* pImsi);

    //i2c
    bool                (*I2C_Init)(I2C_ID_t i2c, I2C_Config_t config);
    I2C_Error_t         (*I2C_Transmit)(I2C_ID_t i2c, uint16_t slaveAddr, uint8_t* pData, uint16_t length, uint32_t timeOut);
    I2C_Error_t         (*I2C_Receive)(I2C_ID_t i2c, uint16_t slaveAddr, uint8_t* pData, uint16_t length, uint32_t timeOut);
    I2C_Error_t         (*I2C_WriteMem)(I2C_ID_t i2c, uint16_t slaveAddr, uint32_t memAddr, uint8_t memSize, uint8_t* pData, uint16_t length, uint32_t timeOut);
    I2C_Error_t         (*I2C_ReadMem)(I2C_ID_t i2c, uint16_t slaveAddr, uint32_t memAddr, uint8_t memSize, uint8_t* pData, uint16_t length, uint32_t timeOut);
    I2C_Error_t         (*I2C_WriteRawByte)(I2C_ID_t i2c, uint8_t SendByte, I2C_CMD_Mask_t CmdMask, uint32_t timeOut);
    uint8_t             (*I2C_ReadRawByte)(I2C_ID_t i2c, I2C_CMD_Mask_t CmdMask, uint32_t timeOut);
    bool                (*I2C_Close)(I2C_ID_t i2c);

    //std
    int                 (*sprintf)(char * buf, const char *fmt, ...);
    int                 (*snprintf)(char * buf, size_t len, const char *fmt, ...);
    int                 (*sscanf)(const char * buf, const char * fmt, ...);
    int                 (*vsprintf)(char *buf, const char *fmt, va_list ap);
    int                 (*vsnprintf)(char *buf, size_t size, const char *fmt, va_list ap);
    int                 (*vsscanf) (const char *fp, const char *fmt0, va_list ap);
    long                (*atol)(const char *s);
    long long           (*atoll)(const char *s);
    int                 (*atox)(const char *s, int len);
    double              (*atof)(const char *s);
    unsigned char*      (*itoa)(int value, char *str, int radix);
    char*               (*gcvt)(double value, int ndigit, char *buf);
    int                 (*rand) (void);
    void                (*srand)(unsigned int seed);
    double              (*acos)(double);
    double              (*asin)(double);
    double              (*atan)(double);
    double              (*atan2)(double, double);
    double              (*cos)(double);
    double              (*sin)(double);
    double              (*tan)(double);
    double              (*cosh)(double);
    double              (*sinh)(double);
    double              (*tanh)(double);
    double              (*exp)(double);
    double              (*frexp)(double, int *);
    double              (*ldexp)(double, int);
    double              (*log)(double);
    double              (*log10)(double);
    double              (*modf)(double, double *);
    double              (*pow)(double, double);
    double              (*sqrt)(double);
    double              (*ceil)(double);
    double              (*fabs)(double);
    double              (*floor)(double);
    double              (*fmod)(double, double);
    double              (*erf)(double);
    double              (*erfc)(double);
    double              (*gamma)(double);
    double              (*hypot)(double, double);
    int                 (*finite)(double);
    double              (*lgamma)(double);
    double              (*acosh)(double);
    double              (*asinh)(double);
    double              (*atanh)(double);
    double              (*cbrt)(double);
    double              (*logb)(double);
    double              (*nextafter)(double, double);
    double              (*remainder)(double, double);
    double              (*significand)(double);
    double              (*copysign)(double, double);
    int                 (*ilogb)(double);
    double              (*rint)(double);
    double              (*scalbn)(double, int);
    double              (*expm1)(double);
    double              (*log1p)(double);

    //setjmp
    volatile void       (*longjmp)(jmp_buf env,  int value);
    int                 (*setjmp)(jmp_buf env);

    //ssl
    SSL_Error_t         (*SSL_Init)(SSL_Config_t* sslConfig);
    SSL_Error_t         (*SSL_Connect)(SSL_Config_t* sslConfig, const char* server, const char* port);
    int                 (*SSL_Write)(SSL_Config_t* sslConfig, uint8_t* data, int length, int timeoutMs);
    int                 (*SSL_Read)(SSL_Config_t* sslConfig, uint8_t* data, int length, int timeoutMs);
    SSL_Error_t         (*SSL_Close)(SSL_Config_t* sslConfig);
    SSL_Error_t         (*SSL_Destroy)(SSL_Config_t* sslConfig);

    //spi
    bool                (*SPI_Init)(SPI_ID_t spiN, SPI_Config_t spiConfig);
    bool                (*SPI_Close)(SPI_ID_t spiN);
    uint32_t            (*SPI_Write)(SPI_ID_t spiN, const uint8_t *data, uint32_t length);
    uint32_t            (*SPI_Read)(SPI_ID_t spiN, uint8_t *data, uint32_t length);
    bool                (*SPI_IsTxDone)(SPI_ID_t spiN);
    bool                (*SPI_IsTxDmaDone)(SPI_ID_t spiN);
    bool                (*SPI_IsRxDmaDone)(SPI_ID_t spiN);
    void                (*SPI_ClearTxDmaDone)(SPI_ID_t spiN);
    void                (*SPI_ClearRxDmaDone)(SPI_ID_t spiN);
    void                (*SPI_FlushFIFOs)(SPI_ID_t spiN);
    void                (*SPI_SetIrqHandler)(SPI_ID_t spiN, SPI_Irq_Handler_t handler);
    void                (*SPI_SetIrqMask)(SPI_ID_t spiN, SPI_Irq_Flags_t irqMask);

    //fota
    bool                (*API_FotaInit)(int size);
    int                 (*API_FotaReceiveData)(unsigned char *data, int len);
    void                (*API_FotaClean)(void);
    int                 (*API_FotaByServer)(char *url, fota_handler_t data_process);

    //sys
    uint32_t            (*SYS_EnterCriticalSection)(void);
    void                (*SYS_ExitCriticalSection)(uint32_t status);

    //ussd
    uint32_t            (*SS_SendUSSD)(USSD_Type_t );

    //lbs
    bool                (*LBS_GetLocation)(Network_Location_t* bsInfo, uint8_t bsNumber,int timeoutS, float* longitude, float* latitude);

    //gizwits
    bool                (*GIZWITS_GetConfig)(Gizwits_Config_t* config, const char* path);
    int                 (*GIZWITS_Connect)(Gizwits_t* gizwits, Gizwits_Config_t* config, const char* configPath);
    int                 (*GIZWITS_Send)(Gizwits_t* gizwits, Gizwits_Action_t action, uint8_t* data, int len);
    int                 (*GIZWITS_DoReceive)(Gizwits_t* gizwits, OnReceivedOnePacket_Callback_t onOnePacket);
    bool                (*GIZWITS_Ping)(Gizwits_t* gizwits);
    int                 (*GIZWITS_GetSocket)(Gizwits_t* gizwits);
    bool                (*GIZWITS_Close)(Gizwits_t* gizwits);

    //watchdog
    void                (*WatchDog_Open)(uint32_t delay);
    void                (*WatchDogSetPeriod)(uint32_t delay);
    void                (*WatchDog_KeepAlive)(void);
    void                (*WatchDog_Close)(void);

    //gouda hal driver
    void                (*hal_GoudaSerialOpen)(HAL_GOUDA_SPI_LINE_T spiLineType, uint32_t spiFreq, const HAL_GOUDA_LCD_CONFIG_T* config, uint32_t ebcCsAddr);
    void                (*hal_GoudaClose)(void);
    HAL_ERR_T           (*hal_GoudaWriteCmd)(uint16_t addr);
    HAL_ERR_T           (*hal_GoudaWriteData)(uint16_t data);
    HAL_ERR_T           (*hal_GoudaWriteReg)(uint16_t addr, uint16_t data);
    HAL_ERR_T           (*hal_GoudaReadData)(uint16_t addr, uint8_t *pData, uint32_t len);
    HAL_ERR_T           (*hal_GoudaReadReg)(uint16_t addr, uint16_t *pData);
    HAL_ERR_T           (*hal_GoudaBlitRoi)(const HAL_GOUDA_WINDOW_T* pRoi, const uint32_t nbCmd, const HAL_GOUDA_LCD_CMD_T* pCmds, HAL_GOUDA_IRQ_HANDLER_T handler);
    HAL_ERR_T           (*hal_GoudaBlitRoi2Ram)(uint32_t* pBuffer, const uint16_t width, const HAL_GOUDA_WINDOW_T* pRoi, HAL_GOUDA_IRQ_HANDLER_T handler);
    bool                (*hal_GoudaIsActive)(void);
    void                (*hal_GoudaSetBgColor)(uint16_t color);
    uint16_t            (*hal_GoudaGetBgColor)(void);
    HAL_ERR_T           (*hal_GoudaOvlLayerOpen)(HAL_GOUDA_OVL_LAYER_ID_T id, HAL_GOUDA_OVL_LAYER_DEF_T* def);
    void                (*hal_GoudaOvlLayerClose)(HAL_GOUDA_OVL_LAYER_ID_T id);
    void                (*hal_GoudaSetBlock)(uint32_t block);

    //spi flash hal
    void                (*hal_SpiFlashLock)(void);
    void                (*hal_SpiFlashUnlock)(void);
    void                (*hal_SpiFlashOpen)(void);
    void                (*hal_SpiFlashClose)();
    uint32_t            (*hal_SpiFlashGetID)(void);
    void                (*hal_SpiFlashSetLockCallback)(HAL_FLASH_CALLBACK_FUNCTION_T lock, void *param);
    void                (*hal_SpiFlashSetUnlockCallback)(HAL_FLASH_CALLBACK_FUNCTION_T unlock, void *param);
    void                (*hal_SpiFlashSetCheckIrq)(bool checkirq);
    void*               (*hal_SpiFlashMapUncacheAddress)(uint32_t flashAddress);
    uint32_t            (*hal_SpiFlashOffset)(void *address);
    bool                (*hal_SpiFlashWrite)(uint32_t flashAddress, const void *data, unsigned size);
    bool                (*hal_SpiFlashErase)(uint32_t flashAddress, unsigned size);
    void                (*hal_SpiFlashChipErase)(void);
    void                (*hal_SpiFlashSleep)(void);
    void                (*hal_SpiFlashWakeup)(void);
    void                (*hal_SpiFlashReset)(void);
    unsigned            (*hal_SpiFlashGetPageSize)(void);
    unsigned            (*hal_SpiFlashGetSectorSize)(void);
    unsigned            (*hal_SpiFlashGetBlockSize)(void);
    unsigned            (*hal_SpiFlashGetSize)(void);
    uint16_t            (*hal_SpiFlashReadStatusRegister)(void);
    void                (*hal_SpiFlashWriteStatusRegister)(uint16_t val, uint16_t mask);
    uint8_t             (*hal_SpiFlashReadStatusRegisterEx)(uint8_t n);
    void                (*hal_SpiFlashWriteStatusRegisterEx)(uint8_t n, uint8_t val);
    void                (*hal_SpiFlashWriteVolatileStatusRegisterEx)(uint8_t n, uint8_t val);
    void                (*hal_SpiFlashSecurityErase)(uint32_t address);
    void                (*hal_SpiFlashSecurityWrite)(uint32_t address, const void *data, unsigned size);
    void                (*hal_SpiFlashSecurityRead)(uint32_t address, void *data, unsigned size);

} T_INTERFACE_VTBL_TAG;
extern T_INTERFACE_VTBL_TAG *g_InterfaceVtbl;
#define CSDK_FUNC(name) (g_InterfaceVtbl->name)


#endif