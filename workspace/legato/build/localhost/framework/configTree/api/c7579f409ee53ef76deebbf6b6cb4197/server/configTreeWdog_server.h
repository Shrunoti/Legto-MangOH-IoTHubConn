

/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */


#ifndef CONFIGTREEWDOG_INTERFACE_H_INCLUDE_GUARD
#define CONFIGTREEWDOG_INTERFACE_H_INCLUDE_GUARD


#include "legato.h"


//--------------------------------------------------------------------------------------------------
/**
 * Get the server service reference
 */
//--------------------------------------------------------------------------------------------------
le_msg_ServiceRef_t configTreeWdog_GetServiceRef
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * Get the client session reference for the current message
 */
//--------------------------------------------------------------------------------------------------
le_msg_SessionRef_t configTreeWdog_GetClientSessionRef
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * Initialize the server and advertise the service.
 */
//--------------------------------------------------------------------------------------------------
void configTreeWdog_AdvertiseService
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