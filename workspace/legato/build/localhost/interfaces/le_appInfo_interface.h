

/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */

/**
 * @page c_appInfo Application Information API
 *
 * @ref le_appInfo_interface.h "API Reference"
 *
 * This API provides information about applications.
 *
 * All the functions in this API are provided by the @b Supervisor.
 *
 * Here's a code sample binding to this service:
 * @verbatim
   bindings:
   {
      clientExe.clientComponent.le_appInfo -> <root>.le_appInfo
   }
   @endverbatim
 *
 * <HR>
 *
 * Copyright (C) Sierra Wireless Inc.
 */
/**
 * @file le_appInfo_interface.h
 *
 * Legato @ref c_appInfo include file.
 *
 * Copyright (C) Sierra Wireless Inc.
 */

#ifndef LE_APPINFO_INTERFACE_H_INCLUDE_GUARD
#define LE_APPINFO_INTERFACE_H_INCLUDE_GUARD


#include "legato.h"

// Interface specific includes
#include "le_limit_interface.h"


//--------------------------------------------------------------------------------------------------
/**
 * Type for handler called when a server disconnects.
 */
//--------------------------------------------------------------------------------------------------
typedef void (*le_appInfo_DisconnectHandler_t)(void *);

//--------------------------------------------------------------------------------------------------
/**
 *
 * Connect the current client thread to the service providing this API. Block until the service is
 * available.
 *
 * For each thread that wants to use this API, either ConnectService or TryConnectService must be
 * called before any other functions in this API.  Normally, ConnectService is automatically called
 * for the main thread, but not for any other thread. For details, see @ref apiFilesC_client.
 *
 * This function is created automatically.
 */
//--------------------------------------------------------------------------------------------------
void le_appInfo_ConnectService
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 *
 * Try to connect the current client thread to the service providing this API. Return with an error
 * if the service is not available.
 *
 * For each thread that wants to use this API, either ConnectService or TryConnectService must be
 * called before any other functions in this API.  Normally, ConnectService is automatically called
 * for the main thread, but not for any other thread. For details, see @ref apiFilesC_client.
 *
 * This function is created automatically.
 *
 * @return
 *  - LE_OK if the client connected successfully to the service.
 *  - LE_UNAVAILABLE if the server is not currently offering the service to which the client is
 *    bound.
 *  - LE_NOT_PERMITTED if the client interface is not bound to any service (doesn't have a binding).
 *  - LE_COMM_ERROR if the Service Directory cannot be reached.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_appInfo_TryConnectService
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * Set handler called when server disconnection is detected.
 *
 * When a server connection is lost, call this handler then exit with LE_FATAL.  If a program wants
 * to continue without exiting, it should call longjmp() from inside the handler.
 */
//--------------------------------------------------------------------------------------------------
void le_appInfo_SetServerDisconnectHandler
(
    le_appInfo_DisconnectHandler_t disconnectHandler,
    void *contextPtr
);

//--------------------------------------------------------------------------------------------------
/**
 *
 * Disconnect the current client thread from the service providing this API.
 *
 * Normally, this function doesn't need to be called. After this function is called, there's no
 * longer a connection to the service, and the functions in this API can't be used. For details, see
 * @ref apiFilesC_client.
 *
 * This function is created automatically.
 */
//--------------------------------------------------------------------------------------------------
void le_appInfo_DisconnectService
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