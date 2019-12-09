

/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */


#ifndef LE_BOOTREASON_INTERFACE_H_INCLUDE_GUARD
#define LE_BOOTREASON_INTERFACE_H_INCLUDE_GUARD


#include "legato.h"


//--------------------------------------------------------------------------------------------------
/**
 * Get the server service reference
 */
//--------------------------------------------------------------------------------------------------
le_msg_ServiceRef_t le_bootReason_GetServiceRef
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * Get the client session reference for the current message
 */
//--------------------------------------------------------------------------------------------------
le_msg_SessionRef_t le_bootReason_GetClientSessionRef
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * Initialize the server and advertise the service.
 */
//--------------------------------------------------------------------------------------------------
void le_bootReason_AdvertiseService
(
    void
);



//--------------------------------------------------------------------------------------------------
/**
 * Checks whether boot-reason was timer expiry.
 *
 * @return
 *      - TRUE if boot-reason was timer expiry.
 *      - FALSE otherwise.
 */
//--------------------------------------------------------------------------------------------------
bool le_bootReason_WasTimer
(
    void
);



//--------------------------------------------------------------------------------------------------
/**
 * Checks whether boot-reason was specific gpio change. GPIO number is specified in parameter.
 *
 * @return
 *      - TRUE if boot-reason was specified gpio change.
 *      - FALSE otherwise.
 *
 * @note The process exits if invalid gpio number is passed.  Check corresponding device documents
 * for valid list of gpio.
 */
//--------------------------------------------------------------------------------------------------
bool le_bootReason_WasGpio
(
    uint32_t gpioNum
        ///< [IN] GPIO number.
);



//--------------------------------------------------------------------------------------------------
/**
 * Checks whether boot reason was due to the specified ADC having a reading above or below the
 * configured limits.
 *
 * @return
 *      true if boot reason was due to the given ADC or false otherwise.
 *
 * @note
 *      The process exits if an invalid ADC number is passed. Check corresponding device documents
 *      for valid list of ADC numbers.
 */
//--------------------------------------------------------------------------------------------------
bool le_bootReason_WasAdc
(
    uint32_t adcNum
        ///< [IN] ADC number
);


#endif // LE_BOOTREASON_INTERFACE_H_INCLUDE_GUARD