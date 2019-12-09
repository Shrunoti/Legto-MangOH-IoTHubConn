

/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */


#ifndef LE_ADC_INTERFACE_H_INCLUDE_GUARD
#define LE_ADC_INTERFACE_H_INCLUDE_GUARD


#include "legato.h"


//--------------------------------------------------------------------------------------------------
/**
 * Get the server service reference
 */
//--------------------------------------------------------------------------------------------------
le_msg_ServiceRef_t le_adc_GetServiceRef
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * Get the client session reference for the current message
 */
//--------------------------------------------------------------------------------------------------
le_msg_SessionRef_t le_adc_GetClientSessionRef
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * Initialize the server and advertise the service.
 */
//--------------------------------------------------------------------------------------------------
void le_adc_AdvertiseService
(
    void
);


//--------------------------------------------------------------------------------------------------
/**
 * Maximum ADC's name string length.
 */
//--------------------------------------------------------------------------------------------------
#define LE_ADC_ADC_NAME_MAX_LEN 30

//--------------------------------------------------------------------------------------------------
/**
 * Maximum ADC's name string length.
 * One extra byte is added for the null character.
 */
//--------------------------------------------------------------------------------------------------
#define LE_ADC_ADC_NAME_MAX_BYTES 31


//--------------------------------------------------------------------------------------------------
/**
 * Get the value of an ADC input
 *
 * @return
 *      - LE_OK            The function succeeded.
 *      - LE_FAULT         The function failed.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_adc_ReadValue
(
    const char* LE_NONNULL adcName,
        ///< [IN] Name of the ADC to read.
    int32_t* adcValuePtr
        ///< [OUT] The adc value
);


#endif // LE_ADC_INTERFACE_H_INCLUDE_GUARD