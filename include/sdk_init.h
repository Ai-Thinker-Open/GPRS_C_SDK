#ifndef SDK_INIT_H
#define SDK_INIT_H

#include <cs_types.h>
#include <sdk_init.h>
#include <api_inc_fs.h>
#include <api_inc_gpio.h>
#include <api_inc_uart.h>
#include <api_inc_network.h>
#include <api_inc_socket.h>
#include <api_inc_os.h>
#include <api_inc_sms.h>
#include <api_inc_time.h>
#include <api_sim.h>
#include <api_hal_power_manager.h>




typedef struct T_INTERFACE_VTBL_TAG
{
    bool                (*Trace)(uint16_t nIndex, PCSTR fmt, ...);
    bool                (*PowerEnable)(Power_Type_t powerType, bool isOn);

    /*api_os*/      
    void                (*OS_SetUserMainHandle)(HANDLE* UserHandle);
    HANDLE              (*OS_CreateTask)(PTASK_FUNC_T pTaskEntry,PVOID pParameter,PVOID pStackAddr,UINT16 nStackSize
                                            ,UINT8 nPriority,UINT16 nCreationFlags,UINT16 nTimeSlice,PCSTR pTaskName);
    void                (*OS_StartTask)(HANDLE hTask,PVOID pParameter);
    void                (*OS_StopTask)(HANDLE hTask);
    bool                (*OS_DeleteTask)(HANDLE hTask);
    UINT32              (*OS_SuspendTask)(HANDLE hTask);
    bool                (*OS_ResumeTask)(HANDLE hTask);
    bool                (*OS_Sleep)(UINT32 nMillisecondes);
    bool                (*OS_WaitEvent)(HANDLE hTask,PVOID *pEvent,UINT32 nTimeOut);
    bool                (*OS_SendEvent)(HANDLE hTask,PVOID pEvent,UINT32 nTimeOut,UINT16 nOption);
    bool                (*OS_ResetEventQueue)(HANDLE hTask);
    bool                (*OS_IsEventAvailable)(HANDLE hTask);
    PVOID               (*OS_Malloc)(UINT32 nSize);
    PVOID               (*OS_Realloc)(VOID *ptr,UINT32 nSize);
    bool                (*OS_Free)(PVOID pMemBlock);
    bool                (*OS_GetHeapUsageStatus)(OS_Heap_Status_t *pOsHeapStatus);
    HANDLE              (*OS_CreateSemaphore)(UINT32 nInitCount);
    bool                (*OS_DeleteSemaphore)(HANDLE hSem);
    bool                (*OS_WaitForSemaphore)(HANDLE hSem,UINT32 nTimeOut);
    bool                (*OS_ReleaseSemaphore)(HANDLE hSem);
    HANDLE              (*OS_CreateMutex)(void);
    void                (*OS_DeleteMutex)(HANDLE mutex);
    void                (*OS_LockMutex)(HANDLE mutex);
    void                (*OS_UnlockMutex)(HANDLE mutex);

    /*api_hal_gpio*/
    bool                (*GPIO_Init)(GPIO_config_t config);
    void                (*GPIO_GetConfig)(GPIO_PIN pin,GPIO_config_t* config); 
    bool                (*GPIO_SetLevel)(GPIO_config_t gpioConf, GPIO_LEVEL  level);
    bool                (*GPIO_Set)(GPIO_PIN pin, GPIO_LEVEL  level);
    bool                (*GPIO_GetLevel)(GPIO_config_t gpioConf, GPIO_LEVEL* level);
    bool                (*GPIO_Get)(GPIO_PIN pin, GPIO_LEVEL* level);
    bool                (*GPIO_Close)(GPIO_PIN pin);

    /*api_hal_gpio*/
    bool                (*UART_Init)(UART_Port_t uartN, UART_Config_t config);
    UINT32              (*UART_Write)(UART_Port_t uartN, UINT8* dataToWrite, uint32_t length);
    UINT32              (*UART_Read)(UART_Port_t uartN, UINT8* dataToRead, uint32_t length, uint32_t timeOutMs);
    bool                (*UART_Close)(UART_Port_t uartN);

    /*api_hal_gps*/
    bool                (*GPS_Open)(UART_Callback_t);
    bool                (*GPS_Close)(void);

    /*api_network*/
    void                (*Network_SetStatusChangedCallback)(Network_Callback_Func_t callback);
    bool                (*Network_StartAttach)(void);
    bool                (*Network_StartDetach)(void);
    bool                (*Network_StartActive)(Network_PDP_Context_t context);
    bool                (*Network_StartDeactive)(UINT8 contextID);
    
    /*api_socket*/
    int                 (*Socket_TcpipConnect)(TCP_UDP_t tcpOrUdp, const char* ip,uint16_t port);
    int                 (*Socket_TcpipWrite)(int socketFd, UINT8* data, uint16_t length);
    int                 (*Socket_TcpipRead)( int socketFd, UINT8* data, uint16_t length);
    bool                (*Socket_TcpipClose)(int socketFd);
    DNS_Status_t        (*DNS_GetHostByName)(UINT8* domain, UINT8* ip);
    
    /*time*/
    int                 (*gettimeofday)(timeval_t *tv, timezone_t *tz);
    clock_t             (*clock)(void);
    
    /*sms*/
    bool                (*SMS_SetFormat)(SMS_Format_t format, SIM_ID_t simID);
    bool                (*SMS_SetParameter)(SMS_Parameter_t* smsParameter,SIM_ID_t simID);
    bool                (*SMS_SendMessage)(const char* phoneNumber, UINT8* message,  UINT8 pduLength, SIM_ID_t simID);
    const char*         (*SMS_GetCharset)(Charset_t);
    
    /*fs*/
    INT32               (*API_FS_Open)(PCSTR    fileName,UINT32 operationFlag,UINT32 mode);
    INT32               (*API_FS_Close)(INT32 fd);
    INT32               (*API_FS_Read)(INT32  fd,UINT8* pBuffer,UINT32 length);
    INT32               (*API_FS_Write)(INT32  fd,UINT8* pBuffer,UINT32 length);
    UINT32              (*API_FS_Flush)(INT32 fd);
    INT32               (*API_FS_Create)(PCSTR fileName,UINT32 mode);
    INT32               (*API_FS_Delete)(PCSTR fileName);
    INT32               (*API_FS_Mkdir)(PCSTR fileName,UINT32 mode);
    INT32               (*API_FS_Rmdir)(PCSTR fileName);
    UINT32              (*Ascii2Unicode)(const UINT8 *in, UINT32 in_len, UINT8 **out, UINT32 *out_len, UINT8 nCharset[12]);

    /*info*/
    bool                (*INFO_GetIMEI)(uint8_t* pImei);

} T_INTERFACE_VTBL_TAG;
extern T_INTERFACE_VTBL_TAG *g_InterfaceVtbl;
#define CSDK_FUNC(name) (g_InterfaceVtbl->name)

// #define Trace                                   CSDK_FUNC(Trace)
#define PowerEnable                             CSDK_FUNC(PowerEnable)


#endif