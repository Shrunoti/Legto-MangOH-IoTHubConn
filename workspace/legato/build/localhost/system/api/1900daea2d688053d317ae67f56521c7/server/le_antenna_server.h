

/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */


#ifndef LE_ANTENNA_INTERFACE_H_INCLUDE_GUARD
#define LE_ANTENNA_INTERFACE_H_INCLUDE_GUARD


#include "legato.h"


//--------------------------------------------------------------------------------------------------
/**
 * Get the server service reference
 */
//--------------------------------------------------------------------------------------------------
le_msg_ServiceRef_t le_antenna_GetServiceRef
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * Get the client session reference for the current message
 */
//--------------------------------------------------------------------------------------------------
le_msg_SessionRef_t le_antenna_GetClientSessionRef
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * Initialize the server and advertise the service.
 */
//--------------------------------------------------------------------------------------------------
void le_antenna_AdvertiseService
(
    void
);


//--------------------------------------------------------------------------------------------------
/**
 *  Reference type for an antenna diagnostic.
 */
//--------------------------------------------------------------------------------------------------
typedef struct le_antenna_Obj* le_antenna_ObjRef_t;


//--------------------------------------------------------------------------------------------------
/**
 * Antenna type.
 */
//--------------------------------------------------------------------------------------------------
typedef enum
{
    LE_ANTENNA_PRIMARY_CELLULAR = 0,
        ///< Cellular antenna type
    LE_ANTENNA_DIVERSITY_CELLULAR = 1,
        ///< Diversity antenna type
    LE_ANTENNA_GNSS = 2,
        ///< GNSS positioning antenna type
    LE_ANTENNA_MAX = 3
        ///<
}
le_antenna_Type_t;


//--------------------------------------------------------------------------------------------------
/**
 * Antenna status
 */
//--------------------------------------------------------------------------------------------------
typedef enum
{
    LE_ANTENNA_SHORT_CIRCUIT = 0,
        ///< antenna in short circuit: unknown state.
    LE_ANTENNA_CLOSE_CIRCUIT = 1,
        ///< antenna in closed circuit: presence of antenna.
    LE_ANTENNA_OPEN_CIRCUIT = 2,
        ///< antenna in open circuit: there is no antenna.
    LE_ANTENNA_OVER_CURRENT = 3,
        ///< antenna in short circuit and current HW protection circuitry has
        ///< tripped. Only applicable for GNSS antenna.
    LE_ANTENNA_INACTIVE = 4,
        ///< Antenna diagnosis feature is inactive.
    LE_ANTENNA_LAST_STATUS = 5
        ///< last status value.
}
le_antenna_Status_t;


//--------------------------------------------------------------------------------------------------
/**
 * Reference type used by Add/Remove functions for EVENT 'le_antenna_StatusEvent'
 */
//--------------------------------------------------------------------------------------------------
typedef struct le_antenna_StatusEventHandler* le_antenna_StatusEventHandlerRef_t;


//--------------------------------------------------------------------------------------------------
/**
 * Handler for antenna status.
 *
 */
//--------------------------------------------------------------------------------------------------
typedef void (*le_antenna_StatusHandlerFunc_t)
(
    le_antenna_ObjRef_t antennaRef,
        ///< antenna reference
    le_antenna_Status_t status,
        ///< antenna status
    void* contextPtr
        ///<
);


//--------------------------------------------------------------------------------------------------
/**
 * Requested the antenna monitoring.
 *
 * @return
 *      - Reference to the antenna object.
 *      - NULL on failure.
 */
//--------------------------------------------------------------------------------------------------
le_antenna_ObjRef_t le_antenna_Request
(
    le_antenna_Type_t antennaType
        ///< [IN] antenna to be monitored
);



//--------------------------------------------------------------------------------------------------
/**
 * Get the antenna type.
 *
 * @return
 *      - LE_OK on success
 *      - LE_NOT_FOUND if the antenna reference is unknown
 *      - LE_BAD_PARAMETER if an invalid reference provided.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_antenna_GetType
(
    le_antenna_ObjRef_t antennaRef,
        ///< [IN] antenna reference
    le_antenna_Type_t* antennaTypePtr
        ///< [OUT] allocated antenna type
);



//--------------------------------------------------------------------------------------------------
/**
 * Set the ADC value used to detect a short circuit.
 *
 * @return
 *      - LE_OK on success
 *      - LE_NOT_FOUND if the antenna reference is unknown
 *      - LE_FAULT on other failure
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_antenna_SetShortLimit
(
    le_antenna_ObjRef_t antennaRef,
        ///< [IN] antenna reference
    uint32_t shortLimit
        ///< [IN] ADC value used to detect a short circuit
);



//--------------------------------------------------------------------------------------------------
/**
 * Get the ADC value used to detect a short circuit.
 *
 * @return
 *      - LE_OK on success
 *      - LE_NOT_FOUND if the antenna reference is unknown
 *      - LE_FAULT on other failure
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_antenna_GetShortLimit
(
    le_antenna_ObjRef_t antennaRef,
        ///< [IN] antenna reference
    uint32_t* shortLimitPtr
        ///< [OUT] ADC value used to detect a short circuit
);



//--------------------------------------------------------------------------------------------------
/**
 * Set the ADC value used to detect an open circuit.
 *
 * @return
 *      - LE_OK on success
 *      - LE_NOT_FOUND if the antenna reference is unknown
 *      - LE_FAULT on other failure
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_antenna_SetOpenLimit
(
    le_antenna_ObjRef_t antennaRef,
        ///< [IN] antenna reference
    uint32_t openLimit
        ///< [IN] ADC value used to detect an open circuit
);



//--------------------------------------------------------------------------------------------------
/**
 * Get the ADC value used to detect an open circuit.
 *
 * @return
 *      - LE_OK on success
 *      - LE_NOT_FOUND if the antenna reference is unknown
 *      - LE_FAULT on other failure
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_antenna_GetOpenLimit
(
    le_antenna_ObjRef_t antennaRef,
        ///< [IN] antenna reference
    uint32_t* openLimitPtr
        ///< [OUT] ADC value used to detect an open circuit
);



//--------------------------------------------------------------------------------------------------
/**
 * Add handler function for EVENT 'le_antenna_StatusEvent'
 *
 * This event provides information on antenna status for the given antennaRef.
 *
 */
//--------------------------------------------------------------------------------------------------
le_antenna_StatusEventHandlerRef_t le_antenna_AddStatusEventHandler
(
    le_antenna_ObjRef_t antennaRef,
        ///< [IN] antenna reference
    le_antenna_StatusHandlerFunc_t handlerPtr,
        ///< [IN]
    void* contextPtr
        ///< [IN]
);



//--------------------------------------------------------------------------------------------------
/**
 * Remove handler function for EVENT 'le_antenna_StatusEvent'
 */
//--------------------------------------------------------------------------------------------------
void le_antenna_RemoveStatusEventHandler
(
    le_antenna_StatusEventHandlerRef_t handlerRef
        ///< [IN]
);



//--------------------------------------------------------------------------------------------------
/**
 * Get the antenna status.
 *
 * @return
 *      - LE_OK on success
 *      - LE_NOT_FOUND if the antenna reference is unknown
 *      - LE_UNSUPPORTED if the antenna detection is not supported
 *      - LE_FAULT on other failure
 *
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_antenna_GetStatus
(
    le_antenna_ObjRef_t antennaRef,
        ///< [IN] antenna reference
    le_antenna_Status_t* statusPtr
        ///< [OUT] antenna status
);



//--------------------------------------------------------------------------------------------------
/**
 * Set the external ADC used to monitor the requested antenna.
 *
 * @return
 *      - LE_OK on success
 *      - LE_NOT_FOUND if the antenna reference is unknown
 *      - LE_UNSUPPORTED request not supported
 *      - LE_FAULT on other failure
 *
 * @note The same external ADC may not be selected for both antennas at the same time.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_antenna_SetExternalAdc
(
    le_antenna_ObjRef_t antennaRef,
        ///< [IN] antenna reference
    int8_t adcId
        ///< [IN] ADC index used to monitor the requested antenna
);



//--------------------------------------------------------------------------------------------------
/**
 * Get the external ADC used to monitor the requested antenna.
 *
 * @return
 *      - LE_OK on success
 *      - LE_NOT_FOUND if the antenna reference is unknown
 *      - LE_UNSUPPORTED request not supported
 *      - LE_FAULT on other failure
 *
 * @note If the returned ADC index is "-1", it means no external ADC are used to monitor
 * the requested antenna.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_antenna_GetExternalAdc
(
    le_antenna_ObjRef_t antennaRef,
        ///< [IN] antenna reference
    int8_t* adcIdPtr
        ///< [OUT] ADC index used to monitor the requested antenna
);


#endif // LE_ANTENNA_INTERFACE_H_INCLUDE_GUARD