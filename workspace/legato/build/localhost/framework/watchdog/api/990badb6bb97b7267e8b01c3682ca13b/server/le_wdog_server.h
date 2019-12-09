

/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */


#ifndef LE_WDOG_INTERFACE_H_INCLUDE_GUARD
#define LE_WDOG_INTERFACE_H_INCLUDE_GUARD


#include "legato.h"


//--------------------------------------------------------------------------------------------------
/**
 * Get the server service reference
 */
//--------------------------------------------------------------------------------------------------
le_msg_ServiceRef_t le_wdog_GetServiceRef
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * Get the client session reference for the current message
 */
//--------------------------------------------------------------------------------------------------
le_msg_SessionRef_t le_wdog_GetClientSessionRef
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * Initialize the server and advertise the service.
 */
//--------------------------------------------------------------------------------------------------
void le_wdog_AdvertiseService
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