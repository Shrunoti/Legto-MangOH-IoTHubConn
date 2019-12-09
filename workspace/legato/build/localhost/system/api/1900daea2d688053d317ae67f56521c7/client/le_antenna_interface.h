

/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */

/**
 * @page c_antenna Antenna Monitoring
 *
 * @ref le_antenna_interface.h "API Reference"
 *
 * <HR>
 *
 * This file contains reference definitions for the antenna diagnostic APIs.
 *
 * @section le_antenna_binding IPC interfaces binding
 *
 * All functions of this API are provided by the @b modemService.
 *
 * Here's a code sample binding to modem services:
 * @verbatim
   bindings:
   {
      clientExe.clientComponent.le_antenna -> modemService.le_antenna
   }
   @endverbatim
 *
 * @section le_antenna_cell_antenna Cellular or Diversity antenna
 *
 * @subsection le_antenna_cell_antenna_diag Diagnostics principle
 *
 * Diagnostic functionality detects antenna presence and/or defective
 * antennas.
 *
 * Antenna diagnostics are based on the following principles:
 *
 * A small DC current passes through the coaxial cable to the antenna. A resistive bridge is
 * used to provide different voltage depending on the antenna state.
 *
 * @note Only antennas with a resistor between the radiating element and ground are supported.
 *
 * These are the diagnostic states:
 * - open circuit: there's no antenna but RF open circuit
 * - closed circuit: antenna is present
 * - short circuit: with or without antenna, short circuit for RF path.
 *
 * @subsection le_antenna_cell_thres_subsection Thresholds
 *
 * Configurable thresholds are compared to the ADC (Analog to Digital Converter) reading from the
 * antenna diagnostic voltage to determine the antenna state.
 *
 * Short and Open threshold limits determine the antenna's state:
 *
 * <b>Short Threshold Limit</b>
 * - short circuit: unknown state
 * - close circuit: presence of antenna
 *
 * A short event is reported for the Cellular or Diversity antenna if the ADC value is lower than
 * the corresponding short limit.
 *
 * <b>Open Threshold Limit</b>
 * - open circuit: no antenna
 *
 * An open event is reported for the Cellular or Diversity antenna
 * if the ADC value is higher than the corresponding open limit.
 *
 * @note The open threshold is always HIGHER than the short threshold.
 *
 * @section le_antenna_gnss GNSS antenna
 *
 * @warning Be sure check the supported antenna diagnostic limit for your specific platform.
 *
 * @subpage platformConstraintsAntenna Platform Constraints
 *
 * @subsection le_antenna_gnss_diag Diagnostics principle
 *
 * This functionality detects (or not) the presence of an antenna or a defective
 * antenna.
 *
 * Antenna diagnostics measure the current consumption for a GNSS active
 * antenna, and provides current protection circuitry to protect the active
 * antenna's power supply.
 *
 * These are the diagnostic states:
 * - short circuit: with or without antenna, short circuit for RF path.
 * - open circuit: there is no antenna.
 * - closed circuit: presence of antenna.
 * - over current : with or without antenna, short circuit for RF path and current
 * hardware protection circuitry has tripped.
 *
 * @subsection le_antenna_gnss_thresh Thresholds
 *
 * GNSS configurable thresholds are compared to the ADC reading from the
 * antenna diagnostic hardware design to measure the current to determine the antenna state.
 *
 * These are the status values reported for a GNSS antenna:
 * - SHORT_CIRCUIT: ADC value > short limit, but over current HW not tripped.
 * - CLOSE_CIRCUIT: short limit >= ADC value >= open limit.
 * - OPEN_CIRCUIT: ADC value < open limit.
 * - OVER_CURRENT: antenna is shorted and current HW protection circuitry has tripped.
 *
 * @note The open threshold is always LOWER than the short threshold.
 *
 * @section le_antenna_gnss_diag_adc_selection Antenna diagnostic ADC selection
 *
 * @warning Ensure to check the supported antenna diagnosis for your specific platform.
 *
 * By default, antenna diagnostics use an internal ADC to read the voltage from the
 * integrated antenna diagnostic circuit, if any.
 *
 * An antenna design using an external antenna diagnostic circuit can still take
 * advantage of the antenna monitoring service. Using the function le_antenna_SetExternalAdc(),
 * the module can monitor one of the external ADC’s to read the voltage from an
 * external antenna diagnosis circuit, rather than the internal ADC.
 * le_antenna_GetExternalAdc() function reads the external ADC used to monitor
 * the requested antenna.
 *
 * @section API_desc API description
 *
 * le_antenna_Request() API allows the application to monitor the requested antenna.
 *
 * le_antenna_GetType() API retrieves the antenna type from an antenna reference.
 *
 * le_antenna_SetShortLimit() API sets the ADC value used to detect a short circuit.
 *
 * le_antenna_GetShortLimit() API gets the ADC value used to detect a short circuit.
 *
 * le_antenna_SetOpenLimit() API sets the ADC value used to detect an open circuit.
 *
 * le_antenna_GetOpenLimit() API gets the ADC value used to detect an open circuit.
 *
 * le_antenna_AddStatusEventHandler() API adds a handler to be notified when the requested antenna
 * status changed.
 *
 * le_antenna_RemoveStatusEventHandler() removes the antenna status handler.
 *
 * le_antenna_GetStatus() API gets the current antenna status.
 *
 * le_antenna_SetExternalAdc() API sets the external ADC used to monitor the requested antenna.
 *
 * le_antenna_GetExternalAdc() API gets the external ADC used to monitor the requested antenna.
 *
 *
 * Copyright (C) Sierra Wireless Inc.
 */
/**
 * @file le_antenna_interface.h
 *
 * Legato @ref c_antenna include file.
 *
 * Copyright (C) Sierra Wireless Inc.
 */

#ifndef LE_ANTENNA_INTERFACE_H_INCLUDE_GUARD
#define LE_ANTENNA_INTERFACE_H_INCLUDE_GUARD


#include "legato.h"


//--------------------------------------------------------------------------------------------------
/**
 * Type for handler called when a server disconnects.
 */
//--------------------------------------------------------------------------------------------------
typedef void (*le_antenna_DisconnectHandler_t)(void *);

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
void le_antenna_ConnectService
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
le_result_t le_antenna_TryConnectService
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
void le_antenna_SetServerDisconnectHandler
(
    le_antenna_DisconnectHandler_t disconnectHandler,
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
void le_antenna_DisconnectService
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