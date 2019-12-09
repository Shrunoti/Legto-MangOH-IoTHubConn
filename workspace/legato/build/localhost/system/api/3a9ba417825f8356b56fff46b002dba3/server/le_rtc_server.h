

/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */


#ifndef LE_RTC_INTERFACE_H_INCLUDE_GUARD
#define LE_RTC_INTERFACE_H_INCLUDE_GUARD


#include "legato.h"


//--------------------------------------------------------------------------------------------------
/**
 * Get the server service reference
 */
//--------------------------------------------------------------------------------------------------
le_msg_ServiceRef_t le_rtc_GetServiceRef
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * Get the client session reference for the current message
 */
//--------------------------------------------------------------------------------------------------
le_msg_SessionRef_t le_rtc_GetClientSessionRef
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * Initialize the server and advertise the service.
 */
//--------------------------------------------------------------------------------------------------
void le_rtc_AdvertiseService
(
    void
);



//--------------------------------------------------------------------------------------------------
/**
 * Get the current time from the RTC
 *
 * @return
 *      - LE_OK            Function succeeded.
 *      - LE_FAULT         Function failed.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_rtc_GetUserTime
(
    uint64_t* millisecondsPastGpsEpochPtr
        ///< [OUT]
);



//--------------------------------------------------------------------------------------------------
/**
 * Set the RTC with the given time string
 *
 * @return
 *      - LE_OK            Function succeeded.
 *      - LE_FAULT         Function failed.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_rtc_SetUserTime
(
    uint64_t millisecondsPastGpsEpoch
        ///< [IN]
);


#endif // LE_RTC_INTERFACE_H_INCLUDE_GUARD