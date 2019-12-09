

/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */

/**
 * @page c_wdog Watchdog Service
 *
 * @ref le_wdog_interface.h "Type/Function Reference"
 *
 * @section Watchdog Service
 *
 * Provides an API for monitoring critical applications and services for deadlocks and other
 * similar faults.  The watchdog for a process is started by calling @c le_wdog_Kick or
 * @c le_wdog_Timeout.  Once started, the watchdog must be kicked periodically by calling
 * @c le_wdog_Kick.  If the watchdog is not kicked (because it is deadlocked, for example),
 * the watchdog service will attempt to recover the service by killing the process and
 * executing the action specified in the process' watchdogAction (if specified)
 * otherwise its faultAction.
 *
 * Generally a service which uses the watchdog should set @c watchdogTimeout to give the
 * default timeout, and @c watchdogAction to give a recovery action.
 *
 * Critical services should also set @c maxWatchdogTimeout.  In this case the watchdog
 * will be started on system boot, and cannot be stopped or set
 * longer than the timeout given in @c maxWatchdogTimeout.  This ensures the service is
 * always running as long as the system is running.
 *
 * @note If maxWatchdogTimeout is not set, no more action is taken if performing the process'
 * @c watchdogAction doesn't recover the process.  If @c maxWatchdogTimeout is specified the
 * system will be rebooted if the process does not recover.
 *
 * Copyright (C) Sierra Wireless Inc.
 */
/**
 * @file le_wdog_interface.h
 *
 * Legato @ref c_wdog include file.
 *
 * Copyright (C) Sierra Wireless Inc.
 */
/**
 * Special values that have specific meaning when used as durations in Timeout():
 * - @ref LE_WDOG_TIMEOUT_NEVER
 * - @ref LE_WDOG_TIMEOUT_NOW
 */

#ifndef LE_WDOG_INTERFACE_H_INCLUDE_GUARD
#define LE_WDOG_INTERFACE_H_INCLUDE_GUARD


#include "legato.h"


//--------------------------------------------------------------------------------------------------
/**
 * Type for handler called when a server disconnects.
 */
//--------------------------------------------------------------------------------------------------
typedef void (*le_wdog_DisconnectHandler_t)(void *);

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
void le_wdog_ConnectService
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
le_result_t le_wdog_TryConnectService
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
void le_wdog_SetServerDisconnectHandler
(
    le_wdog_DisconnectHandler_t disconnectHandler,
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
void le_wdog_DisconnectService
(
    void
);


//--------------------------------------------------------------------------------------------------
/**
 * Suspend the watchdog so that it never times out.
 */
//--------------------------------------------------------------------------------------------------
#define LE_WDOG_TIMEOUT_NEVER -1

//--------------------------------------------------------------------------------------------------
/**
 * Timeout immediately
 */
//--------------------------------------------------------------------------------------------------
#define LE_WDOG_TIMEOUT_NOW 0

//--------------------------------------------------------------------------------------------------
/**
 * External watchdog kick handler
 */
//--------------------------------------------------------------------------------------------------
typedef void (*le_wdog_ExternalWatchdogHandlerFunc_t)
(
    void* contextPtr
        ///<
);

//--------------------------------------------------------------------------------------------------
/**
 * Kicks the watchdog timer.
 *
 * Once the watchdog has been kicked it must be kicked again before the expiration of the current
 * effective timeout else the configured WatchdogAction will be executed.
 */
//--------------------------------------------------------------------------------------------------
void le_wdog_Kick
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * Set a time out.
 *
 * The watchdog is kicked and a new effective timeout value is set. The new timeout will be
 * effective until the next kick at which point it will revert to the original value.
 */
//--------------------------------------------------------------------------------------------------
void le_wdog_Timeout
(
    int32_t milliseconds
        ///< [IN] The number of milliseconds until this timer expires
);

//--------------------------------------------------------------------------------------------------
/**
 * Get the watchdog timeout configured for this process
 *
 * @return
 *      - LE_OK            The watchdog timeout is configured and returned
 *      - LE_NOT_FOUND     The watchdog timeout is not set
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_wdog_GetWatchdogTimeout
(
    uint64_t* millisecondsPtr
        ///< [OUT] The watchdog timeout set for this process
);

//--------------------------------------------------------------------------------------------------
/**
 * Get the max watchdog timeout configured for this process
 *
 * @return
 *      - LE_OK            The max watchdog timeout is configured and returned
 *      - LE_NOT_FOUND     The max watchdog timeout is not set
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_wdog_GetMaxWatchdogTimeout
(
    uint64_t* millisecondsPtr
        ///< [OUT] The max watchdog timeout set for this process
);

#endif // LE_WDOG_INTERFACE_H_INCLUDE_GUARD