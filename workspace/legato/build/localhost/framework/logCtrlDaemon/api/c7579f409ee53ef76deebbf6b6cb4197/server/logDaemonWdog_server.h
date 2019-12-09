

/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */


#ifndef LOGDAEMONWDOG_INTERFACE_H_INCLUDE_GUARD
#define LOGDAEMONWDOG_INTERFACE_H_INCLUDE_GUARD


#include "legato.h"


//--------------------------------------------------------------------------------------------------
/**
 * Get the server service reference
 */
//--------------------------------------------------------------------------------------------------
le_msg_ServiceRef_t logDaemonWdog_GetServiceRef
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * Get the client session reference for the current message
 */
//--------------------------------------------------------------------------------------------------
le_msg_SessionRef_t logDaemonWdog_GetClientSessionRef
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * Initialize the server and advertise the service.
 */
//--------------------------------------------------------------------------------------------------
void logDaemonWdog_AdvertiseService
(
    void
);


//--------------------------------------------------------------------------------------------------
/**
 * Reference type used by Add/Remove functions for EVENT 'logDaemonWdog_KickEvent'
 */
//--------------------------------------------------------------------------------------------------
typedef struct logDaemonWdog_KickEventHandler* logDaemonWdog_KickEventHandlerRef_t;


//--------------------------------------------------------------------------------------------------
/**
 * Handler for framework daemon kick.  Called periodically by a framework daemon to notify
 * the watchdogDaemon it's still alive.
 */
//--------------------------------------------------------------------------------------------------
typedef void (*logDaemonWdog_KickHandlerFunc_t)
(
    void* contextPtr
        ///<
);


//--------------------------------------------------------------------------------------------------
/**
 * Add handler function for EVENT 'logDaemonWdog_KickEvent'
 *
 * This event is fired by a framework daemon periodically in its event loop to notify
 * the watchdogDaemon it's still alive.
 */
//--------------------------------------------------------------------------------------------------
logDaemonWdog_KickEventHandlerRef_t logDaemonWdog_AddKickEventHandler
(
    uint32_t interval,
        ///< [IN] [IN] Interval to kick the timer, in ms.
    logDaemonWdog_KickHandlerFunc_t handlerPtr,
        ///< [IN] [IN] Handler for watchdog kick.
    void* contextPtr
        ///< [IN]
);



//--------------------------------------------------------------------------------------------------
/**
 * Remove handler function for EVENT 'logDaemonWdog_KickEvent'
 */
//--------------------------------------------------------------------------------------------------
void logDaemonWdog_RemoveKickEventHandler
(
    logDaemonWdog_KickEventHandlerRef_t handlerRef
        ///< [IN]
);


#endif // LOGDAEMONWDOG_INTERFACE_H_INCLUDE_GUARD