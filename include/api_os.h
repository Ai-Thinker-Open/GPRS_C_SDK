/*
 * @File  api_os.h
 * @Brief 
 * 
 * @Author: Neucrack 
 * @Date: 2017-10-26 10:45:00 
 * @Last Modified by: Neucrack
 * @Last Modified time: 2018-06-14 19:51:56
 */


#ifndef _API_OS_H__
#define _API_OS_H__

#include <sdk_init.h>

/*
#define OS_WAIT_FOREVER 0xFFFFFFFF
#define OS_NO_WAIT 0x0
#define OS_EVENT_PRI_NORMAL 0
#define OS_EVENT_PRI_URGENT 1
typedef void (*PTASK_FUNC_T)(PVOID pParameter);
void OS_SetUserMainHandle(HANDLE* UserHandle);
HANDLE OS_CreateTask(
    PTASK_FUNC_T pTaskEntry,
    PVOID pParameter,
    PVOID pStackAddr,
    UINT16 nStackSize,
    UINT8 nPriority,
    UINT16 nCreationFlags,
    UINT16 nTimeSlice,
    PCSTR pTaskName);
void OS_StartTask(
    HANDLE pHTask,
    PVOID pParameter);
void OS_StopTask(
    HANDLE pHTask);
bool OS_DeleteTask(
    HANDLE hTask);
UINT32 OS_SuspendTask(
    HANDLE hTask);
bool OS_ResumeTask(
    HANDLE hTask);
bool OS_Sleep(UINT32 nMillisecondes);
#define OS_TIME_OUT_WAIT_FOREVER 0xFFFFFFFF
#define OS_TIME_OUT_NO_WAIT      0x0
bool OS_WaitEvent(
    HANDLE   hTask,
    PVOID*   pEvent,
    UINT32   nTimeOut);
bool OS_SendEvent(
    HANDLE hTask,
    PVOID  pEvent,
    UINT32 nTimeOut,
    UINT16 nOption);
bool OS_ResetEventQueue(
    HANDLE hTask);
bool OS_IsEventAvailable(
    HANDLE hTask);
PVOID OS_Malloc (UINT32 nSize);
PVOID OS_Realloc(VOID *ptr, UINT32 nSize);
bool  OS_Free   (PVOID pMemBlock);
typedef struct _OS_HEAP_STATUS
{
    UINT32 nUsedSize;
    UINT32 nTotalSize;
}OS_Heap_Status_t;
bool OS_GetHeapUsageStatus(OS_Heap_Status_t* pOsHeapStatus);
HANDLE OS_CreateSemaphore(
    UINT32 nInitCount // Specify the initial count of the semaphore
    );
bool OS_DeleteSemaphore(
    HANDLE hSem);
bool OS_WaitForSemaphore(
    HANDLE hSem,    // Specify the handle to a counting semaphore
    UINT32 nTimeOut // the time-out value
    );
bool OS_ReleaseSemaphore(
    HANDLE hSem //Specify the counting semaphore
    );
HANDLE OS_CreateMutex(void);
void OS_DeleteMutex(HANDLE mutex);
void OS_LockMutex(HANDLE mutex);
void OS_UnlockMutex(HANDLE mutex);
*/

// void                (*OS_SetUserMainHandle)(HANDLE* UserHandle);
// HANDLE              (*OS_CreateTask)(PTASK_FUNC_T pTaskEntry,PVOID pParameter,PVOID pStackAddr,UINT16 nStackSize
//                                         ,UINT8 nPriority,UINT16 nCreationFlags,UINT16 nTimeSlice,PCSTR pTaskName);
// void                (*OS_StartTask)(HANDLE hTask,PVOID pParameter);
// void                (*OS_StopTask)(HANDLE hTask);
// bool                (*OS_DeleteTask)(HANDLE hTask);
// UINT32              (*OS_SuspendTask)(HANDLE hTask);
// bool                (*OS_ResumeTask)(HANDLE hTask);
// bool                (*OS_Sleep)(UINT32 nMillisecondes);
// bool                (*OS_WaitEvent)(HANDLE hTask,PVOID *pEvent,UINT32 nTimeOut);
// bool                (*OS_SendEvent)(HANDLE hTask,PVOID pEvent,UINT32 nTimeOut,UINT16 nOption);
// bool                (*OS_ResetEventQueue)(HANDLE hTask);
// bool                (*OS_IsEventAvailable)(HANDLE hTask);
// PVOID               (*OS_Malloc)(UINT32 nSize);
// PVOID               (*OS_Realloc)(VOID *ptr,UINT32 nSize);
// bool                (*OS_Free)(PVOID pMemBlock);
// bool                (*OS_GetHeapUsageStatus)(OS_Heap_Status_t *pOsHeapStatus);
// HANDLE              (*OS_CreateSemaphore)(UINT32 nInitCount);
// bool                (*OS_DeleteSemaphore)(HANDLE hSem);
// bool                (*OS_WaitForSemaphore)(HANDLE hSem,UINT32 nTimeOut);
// bool                (*OS_ReleaseSemaphore)(HANDLE hSem);
// HANDLE              (*OS_CreateMutex)(void);
// void                (*OS_DeleteMutex)(HANDLE mutex);
// void                (*OS_LockMutex)(HANDLE mutex);
// void                (*OS_UnlockMutex)(HANDLE mutex);
/**/




/// \brief  void                OS_SetUserMainHandle(HANDLE* UserHandle);
/// \param  HANDLE UserHandle  , 
/// \return void , 
#define OS_SetUserMainHandle                          CSDK_FUNC(OS_SetUserMainHandle)

#define OS_GetUserMainHandle                          CSDK_FUNC(OS_GetUserMainHandle)


/// \brief  void                OS_CreateTask)(PTASK_FUNC_T pTaskEntry,PVOID pParameter,PVOID pStackAddr,UINT16 nStackSize
//                                         ,UINT8 nPriority,UINT16 nCreationFlags,UINT16 nTimeSlice,PCSTR pTaskName);
/// \param  PTASK_FUNC_T pTaskEntry , 
/// \param  PVOID pParameter  , the pEvent pParameter pointer
/// \param  PVOID pStackAddr  , the pEvent pStackAddr pointer,reserved, only NULL
/// \param  UINT16 nStackSize  , the stack size of task, unit: Word(4 Bytes), so the actual size is nStackSize*4 Bytes
/// \param  UINT8 nPriority  , the priority of task
/// \param  UINT16 nCreationFlags  , 
/// \param  UINT16 nTimeSlice  , 
/// \param  PCSTR pTaskName  , 
/// \return void , 
#define OS_CreateTask                               CSDK_FUNC(OS_CreateTask)

/// \brief  void                OS_StartTask)(HANDLE hTask,PVOID pParameter);
/// \param  HANDLE hTask  , 
/// \param  PVOID pParameter  , the pEvent pointer pointer
/// \return void , 
#define OS_StartTask                                CSDK_FUNC(OS_StartTask)

/// \brief  void                OS_StopTask(HANDLE hTask);
/// \param  HANDLE hTask  , 
/// \return void , 
#define OS_StopTask                                 CSDK_FUNC(OS_StopTask)

/// \brief  bool                OS_DeleteTask(HANDLE hTask);
/// \param  HANDLE hTask  , 
/// \return bool , 
#define OS_DeleteTask                               CSDK_FUNC(OS_DeleteTask)

/// \brief  UINT32              OS_SuspendTask(HANDLE hTask);
/// \param  HANDLE hTask  , 
/// \return UINT32 , 
#define OS_SuspendTask                              CSDK_FUNC(OS_SuspendTask)

/// \brief  bool                OS_ResumeTask(HANDLE hTask);
/// \param  HANDLE hTask  , 
/// \return bool , 
#define OS_ResumeTask                               CSDK_FUNC(OS_ResumeTask)

/// \brief  bool                OS_Sleep(UINT32 nMillisecondes);
/// \param  UINT32 nMillisecondes  , 
/// \return bool , 
#define OS_Sleep                                    CSDK_FUNC(OS_Sleep)

#define OS_SleepUs                                  CSDK_FUNC(OS_SleepUs)

/// \brief  bool                OS_WaitEvent(HANDLE hTask,PVOID *pEvent,UINT32 nTimeOut);
/// \param  HANDLE hTask  , the task to wait event 
/// \param  PVOID pEvent  , the pEvent pointer
/// \param  UINT32 nTimeOut  , 
/// \return bool , 
#define OS_WaitEvent                                CSDK_FUNC(OS_WaitEvent)

/// \brief  bool                OS_SendEvent(HANDLE hTask,PVOID pEvent,UINT32 nTimeOut,UINT16 nOption);
/// \param  HANDLE hTask  , the task to send event to 
/// \param  PVOID pEvent  , the pEvent pointer
/// \param  UINT32 nTimeOut  , 
/// \param  UINT16 nOption  , 
/// \return bool , 
#define OS_SendEvent                                CSDK_FUNC(OS_SendEvent)

/// \brief  bool                OS_ResetEventQueue(HANDLE hTask);
/// \param  HANDLE hTask  , 
/// \return bool , 
#define OS_ResetEventQueue                          CSDK_FUNC(OS_ResetEventQueue)

/// \brief  bool                OS_IsEventAvailable(HANDLE hTask);
/// \param  HANDLE hTask  , 
/// \return bool , 
#define OS_IsEventAvailable                         CSDK_FUNC(OS_IsEventAvailable)

/// \brief  PVOID               (*OS_Malloc)(UINT32 nSize);
/// \param  UINT32 nSize  , realloc data size
/// \return PVOID , 
#define OS_Malloc                                   CSDK_FUNC(OS_Malloc)

/// \brief  PVOID               (*OS_Realloc)(VOID *ptr,UINT32 nSize);
/// \param  VOID *ptr  , the buf pointer
/// \param  UINT32 nSize  , realloc data size
/// \return PVOID , 
#define OS_Realloc                                  CSDK_FUNC(OS_Realloc)

/// \brief  bool                OS_Free(PVOID pMemBlock);
/// \param  PVOID pMemBlocks  , 
/// \return bool , 
#define OS_Free                                     CSDK_FUNC(OS_Free)

/// \brief  bool                OS_GetHeapUsageStatus(OS_Heap_Status_t *pOsHeapStatus);
/// \param  OS_Heap_Status_t *pOsHeapStatus  , 
/// \return bool , 
#define OS_GetHeapUsageStatus                       CSDK_FUNC(OS_GetHeapUsageStatus)

/// \brief  HANDLE              OS_CreateSemaphore(UINT32 nInitCount);
/// \param  UINT32 nTimeOut  , 
/// \return HANDLE , 
#define OS_CreateSemaphore                          CSDK_FUNC(OS_CreateSemaphore)

/// \brief  bool                OS_DeleteSemaphore(HANDLE hSem);
/// \param  HANDLE hSem  , 
/// \return bool , 
#define OS_DeleteSemaphore                          CSDK_FUNC(OS_DeleteSemaphore)

/// \brief  bool                OS_WaitForSemaphore(HANDLE hSem,UINT32 nTimeOut);
/// \param  HANDLE hSem  , 
/// \param  UINT32 nTimeOut  , 
/// \return bool , 
#define OS_WaitForSemaphore                         CSDK_FUNC(OS_WaitForSemaphore)

/// \brief  bool                OS_ReleaseSemaphore(HANDLE hSem);
/// \param  HANDLE hSem  , 
/// \return bool , 
#define OS_ReleaseSemaphore                         CSDK_FUNC(OS_ReleaseSemaphore)

/// \brief  HANDLE              OS_CreateMutex(void);
/// \param  void  , 
/// \return HANDLE , 
#define OS_CreateMutex                              CSDK_FUNC(OS_CreateMutex)

/// \brief  void                OS_DeleteMutex(HANDLE mutex);
/// \param  HANDLE mutex , 
/// \return void , 
#define OS_DeleteMutex                              CSDK_FUNC(OS_DeleteMutex)

/// \brief  void                OS_LockMutex(HANDLE mutex);
/// \param  HANDLE mutex , 
/// \return void , 
#define OS_LockMutex                                CSDK_FUNC(OS_LockMutex)

/// \brief  void                OS_UnlockMutex(HANDLE mutex);
/// \param  HANDLE mutex , 
/// \return void , 
#define OS_UnlockMutex                              CSDK_FUNC(OS_UnlockMutex)

/// \brief  bool OS_GetTaskInfo(HANDLE pHTask, OS_Task_Info_t* taskInfo);
#define OS_GetTaskInfo                              CSDK_FUNC(OS_GetTaskInfo)

/**
  *
  * @param hTask Task handle that must have OS_WaitEvent(hTask.....)
  *
  */
#define OS_StartCallbackTimer                       CSDK_FUNC(OS_StartCallbackTimer)
#define OS_StopCallbackTimer                        CSDK_FUNC(OS_StopCallbackTimer)
#define OS_QueryCallbackTimer                       CSDK_FUNC(OS_QueryCallbackTimer)

#endif

