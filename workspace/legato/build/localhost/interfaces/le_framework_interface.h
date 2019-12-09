

/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */

/**
 * @page c_framework Framework Control API
 *
 * @ref le_framework_interface.h "API Reference"
 *
 * API for controlling the framework. This API should only be made available to privileged users.
 *
 * @section legatoServicesSupervisor_binding Binding
 *
 * You can use a definition @c .adef file to bind your client-side app and component to the
 * server-side Framework Control service.
 *
 * This code sample shows how to bind an app to this service:
 * @code
 * bindings
 * {
 *     myExe.myComp.le_framework -> <root>.le_framework
 * }
 * @endcode
 *
 * See @ref defFilesAdef for details.
 *
 * <HR>
 *
 * Copyright (C) Sierra Wireless Inc.
 */
/**
 * @file le_framework_interface.h
 *
 * Legato @ref c_framework include file.
 *
 * Copyright (C) Sierra Wireless Inc.
 */

#ifndef LE_FRAMEWORK_INTERFACE_H_INCLUDE_GUARD
#define LE_FRAMEWORK_INTERFACE_H_INCLUDE_GUARD


#include "legato.h"


//--------------------------------------------------------------------------------------------------
/**
 * Type for handler called when a server disconnects.
 */
//--------------------------------------------------------------------------------------------------
typedef void (*le_framework_DisconnectHandler_t)(void *);

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
void le_framework_ConnectService
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
le_result_t le_framework_TryConnectService
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
void le_framework_SetServerDisconnectHandler
(
    le_framework_DisconnectHandler_t disconnectHandler,
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
void le_framework_DisconnectService
(
    void
);


//--------------------------------------------------------------------------------------------------
/**
 * Stops the Legato framework.
 *
 * @return
 *      LE_OK if successful.
 *      LE_DUPLICATE if the framework is in the process of shutting down (perhaps someone else has
 *                   already requested the framework be stopped or restarted).
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_framework_Stop
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * Restarts the Legato framework.
 *
 * @return
 *      LE_OK if the request was accepted and the restart procedure has begun.
 *      LE_DUPLICATE if the framework is already in the process of shutting down (perhaps someone
 *                   else has already requested the framework be stopped or restarted).
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_framework_Restart
(
    bool manualRestart
        ///< [IN] Was the restart manually triggered e.g. "legato restart"
);

//--------------------------------------------------------------------------------------------------
/**
 * Reports if the Legato framework is stopping.
 *
 * @return
 *     true if the framework is stopping or rebooting
 *     false otherwise
 */
//--------------------------------------------------------------------------------------------------
bool le_framework_IsStopping
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * Checks whether legato framework is Read-Only or not.
 *
 * @return
 *     true if the framework is Read-Only
 *     false otherwise
 */
//--------------------------------------------------------------------------------------------------
bool le_framework_IsReadOnly
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * Mark the next reboot as expected. Should be called by short lived app that shutdown platform
 * after a small wakeup. This prevents system not to rollback on expected reboot.
 */
//--------------------------------------------------------------------------------------------------
void le_framework_NotifyExpectedReboot
(
    void
);

#endif // LE_FRAMEWORK_INTERFACE_H_INCLUDE_GUARD