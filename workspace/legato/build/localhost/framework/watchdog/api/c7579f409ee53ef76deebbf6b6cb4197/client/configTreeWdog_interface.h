

/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */

/**
 * @page c_frameworkWdog Framework Watchdog API
 *
 * @ref frameworkWdog.h "API Reference"
 *
 * The framework Watchdog API is implemented by framework daemons to connect to the watchdog.
 * Since the framework relies on many of the framework daemons (specifically supervisor, log
 * and configTree), these daemons cannot call into the watchdog or they risk a deadlock.
 *
 * Instead each framework daemon should implement frameworkWdog.api, and the watchdog will
 * call this function to give the framework daemon the kick function to call.
 *
 * <HR>
 *
 * Copyright (C) Sierra Wireless Inc.
 */
/**
 * @file frameworkWdog_interface.h
 *
 * Legato @ref c_frameworkWdog include file.
 */

#ifndef CONFIGTREEWDOG_INTERFACE_H_INCLUDE_GUARD
#define CONFIGTREEWDOG_INTERFACE_H_INCLUDE_GUARD


#include "legato.h"


//--------------------------------------------------------------------------------------------------
/**
 * Type for handler called when a server disconnects.
 */
//--------------------------------------------------------------------------------------------------
typedef void (*configTreeWdog_DisconnectHandler_t)(void *);

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
void configTreeWdog_ConnectService
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
le_result_t configTreeWdog_TryConnectService
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
void configTreeWdog_SetServerDisconnectHandler
(
    configTreeWdog_DisconnectHandler_t disconnectHandler,
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
void configTreeWdog_DisconnectService
(
    void
);


//--------------------------------------------------------------------------------------------------
/**
 * Reference type used by Add/Remove functions for EVENT 'configTreeWdog_KickEvent'
 */
//--------------------------------------------------------------------------------------------------
typedef struct configTreeWdog_KickEventHandler* configTreeWdog_KickEventHandlerRef_t;


//--------------------------------------------------------------------------------------------------
/**
 * Handler for framework daemon kick.  Called periodically by a framework daemon to notify
 * the watchdogDaemon it's still alive.
 */
//--------------------------------------------------------------------------------------------------
typedef void (*configTreeWdog_KickHandlerFunc_t)
(
    void* contextPtr
        ///<
);

//--------------------------------------------------------------------------------------------------
/**
 * Add handler function for EVENT 'configTreeWdog_KickEvent'
 *
 * This event is fired by a framework daemon periodically in its event loop to notify
 * the watchdogDaemon it's still alive.
 */
//--------------------------------------------------------------------------------------------------
configTreeWdog_KickEventHandlerRef_t configTreeWdog_AddKickEventHandler
(
    uint32_t interval,
        ///< [IN] [IN] Interval to kick the timer, in ms.
    configTreeWdog_KickHandlerFunc_t handlerPtr,
        ///< [IN] [IN] Handler for watchdog kick.
    void* contextPtr
        ///< [IN]
);

//--------------------------------------------------------------------------------------------------
/**
 * Remove handler function for EVENT 'configTreeWdog_KickEvent'
 */
//--------------------------------------------------------------------------------------------------
void configTreeWdog_RemoveKickEventHandler
(
    configTreeWdog_KickEventHandlerRef_t handlerRef
        ///< [IN]
);

#endif // CONFIGTREEWDOG_INTERFACE_H_INCLUDE_GUARD