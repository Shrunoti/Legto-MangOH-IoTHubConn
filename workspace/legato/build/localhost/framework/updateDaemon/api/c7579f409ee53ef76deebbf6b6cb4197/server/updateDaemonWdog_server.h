

/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */


#ifndef UPDATEDAEMONWDOG_INTERFACE_H_INCLUDE_GUARD
#define UPDATEDAEMONWDOG_INTERFACE_H_INCLUDE_GUARD


#include "legato.h"


//--------------------------------------------------------------------------------------------------
/**
 * Get the server service reference
 */
//--------------------------------------------------------------------------------------------------
le_msg_ServiceRef_t updateDaemonWdog_GetServiceRef
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * Get the client session reference for the current message
 */
//--------------------------------------------------------------------------------------------------
le_msg_SessionRef_t updateDaemonWdog_GetClientSessionRef
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * Initialize the server and advertise the service.
 */
//--------------------------------------------------------------------------------------------------
void updateDaemonWdog_AdvertiseService
(
    void
);


//--------------------------------------------------------------------------------------------------
/**
 * Reference type used by Add/Remove functions for EVENT 'updateDaemonWdog_KickEvent'
 */
//--------------------------------------------------------------------------------------------------
typedef struct updateDaemonWdog_KickEventHandler* updateDaemonWdog_KickEventHandlerRef_t;


//--------------------------------------------------------------------------------------------------
/**
 * Handler for framework daemon kick.  Called periodically by a framework daemon to notify
 * the watchdogDaemon it's still alive.
 */
//--------------------------------------------------------------------------------------------------
typedef void (*updateDaemonWdog_KickHandlerFunc_t)
(
    void* contextPtr
        ///<
);


//--------------------------------------------------------------------------------------------------
/**
 * Add handler function for EVENT 'updateDaemonWdog_KickEvent'
 *
 * This event is fired by a framework daemon periodically in its event loop to notify
 * the watchdogDaemon it's still alive.
 */
//--------------------------------------------------------------------------------------------------
updateDaemonWdog_KickEventHandlerRef_t updateDaemonWdog_AddKickEventHandler
(
    uint32_t interval,
        ///< [IN] [IN] Interval to kick the timer, in ms.
    updateDaemonWdog_KickHandlerFunc_t handlerPtr,
        ///< [IN] [IN] Handler for watchdog kick.
    void* contextPtr
        ///< [IN]
);



//--------------------------------------------------------------------------------------------------
/**
 * Remove handler function for EVENT 'updateDaemonWdog_KickEvent'
 */
//--------------------------------------------------------------------------------------------------
void updateDaemonWdog_RemoveKickEventHandler
(
    updateDaemonWdog_KickEventHandlerRef_t handlerRef
        ///< [IN]
);


#endif // UPDATEDAEMONWDOG_INTERFACE_H_INCLUDE_GUARD