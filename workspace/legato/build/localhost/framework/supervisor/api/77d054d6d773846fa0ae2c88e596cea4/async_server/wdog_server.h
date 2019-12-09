

/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */


#ifndef WDOG_INTERFACE_H_INCLUDE_GUARD
#define WDOG_INTERFACE_H_INCLUDE_GUARD


#include "legato.h"

//--------------------------------------------------------------------------------------------------
/**
 * Command reference for async server-side function support.  The interface function receives the
 * reference, and must pass it to the corresponding respond function.
 */
//--------------------------------------------------------------------------------------------------
typedef struct wdog_ServerCmd* wdog_ServerCmdRef_t;


//--------------------------------------------------------------------------------------------------
/**
 * Get the server service reference
 */
//--------------------------------------------------------------------------------------------------
le_msg_ServiceRef_t wdog_GetServiceRef
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * Get the client session reference for the current message
 */
//--------------------------------------------------------------------------------------------------
le_msg_SessionRef_t wdog_GetClientSessionRef
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * Initialize the server and advertise the service.
 */
//--------------------------------------------------------------------------------------------------
void wdog_AdvertiseService
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * Server-side respond function for wdog_WatchdogTimedOut
 */
//--------------------------------------------------------------------------------------------------
void wdog_WatchdogTimedOutRespond
(
    wdog_ServerCmdRef_t _cmdRef
);

//--------------------------------------------------------------------------------------------------
/**
 * Prototype for server-side async interface function
 */
//--------------------------------------------------------------------------------------------------
void wdog_WatchdogTimedOut
(
    wdog_ServerCmdRef_t _cmdRef,
    uint32_t procId
);


#endif // WDOG_INTERFACE_H_INCLUDE_GUARD