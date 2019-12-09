

/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */

/**
 * @page c_logFd File Descriptor Logging API
 *
 * @ref logFd_interface.h "API Reference"
 *
 * This API provides a method for logging messages coming from a file descriptor such as a pipe or
 * socket.  This API MUST only be used by the Supervisor.
 *
 * <HR>
 *
 * Copyright (C) Sierra Wireless Inc.
 */
/**
 * @file logFd_interface.h
 *
 * Legato @ref c_logFd include file.
 *
 * Copyright (C) Sierra Wireless Inc.
 */

#ifndef LOGFD_INTERFACE_H_INCLUDE_GUARD
#define LOGFD_INTERFACE_H_INCLUDE_GUARD


#include "legato.h"

// Interface specific includes
#include "le_limit_interface.h"


//--------------------------------------------------------------------------------------------------
/**
 * Type for handler called when a server disconnects.
 */
//--------------------------------------------------------------------------------------------------
typedef void (*logFd_DisconnectHandler_t)(void *);

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
void logFd_ConnectService
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
le_result_t logFd_TryConnectService
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
void logFd_SetServerDisconnectHandler
(
    logFd_DisconnectHandler_t disconnectHandler,
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
void logFd_DisconnectService
(
    void
);


//--------------------------------------------------------------------------------------------------
/**
 * Registers an application process' standard error for logging.  Messages from this file descriptor
 * will be logged at LE_LOG_ERR level.
 *
 * @return
 *      LE_OK if successful.
 *      LE_FAULT if there was an error.
 */
//--------------------------------------------------------------------------------------------------
void logFd_StdErr
(
    int fd,
        ///< [IN] stderr file descriptor.
    const char* LE_NONNULL appName,
        ///< [IN] Name of the application.
    const char* LE_NONNULL procName,
        ///< [IN] Name of the process.
    int32_t pid
        ///< [IN] PID of the process.
);

//--------------------------------------------------------------------------------------------------
/**
 * Registers an application process' standard out for logging.  Messages from this file descriptor
 * will be logged at LE_LOG_INFO level.
 *
 * @return
 *      LE_OK if successful.
 *      LE_FAULT if there was an error.
 */
//--------------------------------------------------------------------------------------------------
void logFd_StdOut
(
    int fd,
        ///< [IN] stdout file descriptor.
    const char* LE_NONNULL appName,
        ///< [IN] Name of the application.
    const char* LE_NONNULL procName,
        ///< [IN] Name of the process.
    int32_t pid
        ///< [IN] PID of the process.
);

#endif // LOGFD_INTERFACE_H_INCLUDE_GUARD