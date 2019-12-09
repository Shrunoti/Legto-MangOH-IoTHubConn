

/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */


#ifndef LE_APPINFO_INTERFACE_H_INCLUDE_GUARD
#define LE_APPINFO_INTERFACE_H_INCLUDE_GUARD


#include "legato.h"

// Interface specific includes
#include "le_limit_server.h"


//--------------------------------------------------------------------------------------------------
/**
 * Get the server service reference
 */
//--------------------------------------------------------------------------------------------------
le_msg_ServiceRef_t le_appInfo_GetServiceRef
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * Get the client session reference for the current message
 */
//--------------------------------------------------------------------------------------------------
le_msg_SessionRef_t le_appInfo_GetClientSessionRef
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * Initialize the server and advertise the service.
 */
//--------------------------------------------------------------------------------------------------
void le_appInfo_AdvertiseService
(
    void
);


//--------------------------------------------------------------------------------------------------
/**
 * Length of an MD5 hash string.
 */
//--------------------------------------------------------------------------------------------------
#define LE_APPINFO_MD5_STR_LEN 32

//--------------------------------------------------------------------------------------------------
/**
 * Application states.
 */
//--------------------------------------------------------------------------------------------------
typedef enum
{
    LE_APPINFO_STOPPED = 0,
        ///< Application has been stopped or never started.
    LE_APPINFO_RUNNING = 1
        ///< Application is running.
}
le_appInfo_State_t;


//--------------------------------------------------------------------------------------------------
/**
 * Process states.
 */
//--------------------------------------------------------------------------------------------------
typedef enum
{
    LE_APPINFO_PROC_STOPPED = 0,
        ///< Process has been stopped or never started.
    LE_APPINFO_PROC_RUNNING = 1
        ///< Process is running.
}
le_appInfo_ProcState_t;



//--------------------------------------------------------------------------------------------------
/**
 * Gets the state of the specified application.  The state of unknown applications is STOPPED.
 *
 * @return
 *      The state of the specified application.
 *
 * @note If the application name pointer is null or if its string is empty or of bad format it is a
 *       fatal error, the function will not return.
 */
//--------------------------------------------------------------------------------------------------
le_appInfo_State_t le_appInfo_GetState
(
    const char* LE_NONNULL appName
        ///< [IN] Application name.
);



//--------------------------------------------------------------------------------------------------
/**
 * Gets the state of the specified process in an application.  This function only works for
 * configured processes that the Supervisor starts directly.
 *
 * @return
 *      The state of the specified process.
 *
 * @note If the application or process names pointers are null or if their strings are empty or of
 *       bad format it is a fatal error, the function will not return.
 */
//--------------------------------------------------------------------------------------------------
le_appInfo_ProcState_t le_appInfo_GetProcState
(
    const char* LE_NONNULL appName,
        ///< [IN] Application name.
    const char* LE_NONNULL procName
        ///< [IN] Process name.
);



//--------------------------------------------------------------------------------------------------
/**
 * Gets the application name of the process with the specified PID.
 *
 * @return
 *      LE_OK if the application name was successfully found.
 *      LE_OVERFLOW if the application name could not fit in the provided buffer.
 *      LE_NOT_FOUND if the process is not part of an application.
 *      LE_FAULT if there was an error.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_appInfo_GetName
(
    int32_t pid,
        ///< [IN] PID of the process.
    char* appName,
        ///< [OUT] Application name.
    size_t appNameSize
        ///< [IN]
);



//--------------------------------------------------------------------------------------------------
/**
 * Gets the application hash as a hexidecimal string.  The application hash is a unique hash of the
 * current version of the application.
 *
 * @return
 *      LE_OK if the application has was successfully retrieved.
 *      LE_OVERFLOW if the application hash could not fit in the provided buffer.
 *      LE_NOT_FOUND if the application is not installed.
 *      LE_FAULT if there was an error.
 *
 * @note If the application name pointer is null or if its string is empty or of bad format it is a
 *       fatal error, the function will not return.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_appInfo_GetHash
(
    const char* LE_NONNULL appName,
        ///< [IN] Application name.
    char* hashStr,
        ///< [OUT] Hash string.
    size_t hashStrSize
        ///< [IN]
);


#endif // LE_APPINFO_INTERFACE_H_INCLUDE_GUARD