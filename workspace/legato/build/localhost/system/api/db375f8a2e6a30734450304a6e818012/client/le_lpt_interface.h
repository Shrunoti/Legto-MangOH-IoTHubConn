

/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */

/**
 * @page c_lpt Low Power Technologies
 *
 * @ref le_lpt_interface.h "API Reference"
 *
 * <HR>
 *
 * The Low Power Technologies (LPT) APIs are used to control the low power features of the modem.
 *
 * @section le_lpt_binding IPC interfaces binding
 *
 * All the functions of this API are provided by the @b modemService service.
 *
 * Here's a code sample binding to modem services:
 * @verbatim
   bindings:
   {
      clientExe.clientComponent.le_lpt -> modemService.le_lpt
   }
   @endverbatim
 *
 * @section le_lpt_eDRX eDRX
 *
 * The extended idle-mode discontinuous reception (eDRX) is a mechanism that reduces power
 * consumption by extending the sleeping cycle in idle mode. It allows the device to turn part of
 * its circuitry off during the extended DRX period to save power. During the extended DRX period,
 * the device is not listening for paging or downlink control channels, so the network should not
 * try to contact the device.
 *
 * @warning Enabling eDRX introduces a longer latency in reaching the device and should therefore
 * not be used by systems that cannot handle it, e.g. systems supporting mobile-terminated voice.
 *
 * The use of eDRX for a given radio access technology (@ref le_lpt_EDrxRat_t) can be enabled and
 * disabled with le_lpt_SetEDrxState().
 *
 * @snippet "apps/test/modemServices/lpt/lptIntegrationTest/lptTest/lptTest.c" Set state
 *
 * The eDRX feature is controlled by two parameters, which are defined in the standard 3GPP
 * TS 24.008 Release 13 Section 10.5.5.32: the eDRX value, defining the eDRX cycle length, and
 * the Paging Time Window. These parameters are negotiated between the device and the network during
 * an attach or routing area updating procedure.
 *
 * The requested eDRX value can be set with le_lpt_SetRequestedEDrxValue() and retrieved with
 * le_lpt_GetRequestedEDrxValue().
 *
 * @snippet "apps/test/modemServices/lpt/lptIntegrationTest/lptTest/lptTest.c" eDRX value
 *
 * The eDRX value provided by the network can be retrieved with
 * le_lpt_GetNetworkProvidedEDrxValue().
 *
 * @snippet "apps/test/modemServices/lpt/lptIntegrationTest/lptTest/lptTest.c" NP eDRX value
 *
 * The requested Paging Time Window cannot be set, but the network-provided value can be retrieved
 * with le_lpt_GetNetworkProvidedPagingTimeWindow().
 *
 * @snippet "apps/test/modemServices/lpt/lptIntegrationTest/lptTest/lptTest.c" NP PTW
 *
 * A handler can also be registered with le_lpt_AddEDrxParamsChangeHandler() in order to be notified
 * of the changes in the network-provided eDRX parameters.
 *
 * @snippet "apps/test/modemServices/lpt/lptIntegrationTest/lptTest/lptTest.c" eDRX handler
 * @snippet "apps/test/modemServices/lpt/lptIntegrationTest/lptTest/lptTest.c" Add eDRX handler
 *
 * The handler can be removed with le_lpt_RemoveEDrxParamsChangeHandler().
 *
 * <HR>
 *
 * Copyright (C) Sierra Wireless Inc.
 */
/**
 * @file le_lpt_interface.h
 *
 * Legato @ref c_lpt include file.
 *
 * Copyright (C) Sierra Wireless Inc.
 */

#ifndef LE_LPT_INTERFACE_H_INCLUDE_GUARD
#define LE_LPT_INTERFACE_H_INCLUDE_GUARD


#include "legato.h"


//--------------------------------------------------------------------------------------------------
/**
 * Type for handler called when a server disconnects.
 */
//--------------------------------------------------------------------------------------------------
typedef void (*le_lpt_DisconnectHandler_t)(void *);

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
void le_lpt_ConnectService
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
le_result_t le_lpt_TryConnectService
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
void le_lpt_SetServerDisconnectHandler
(
    le_lpt_DisconnectHandler_t disconnectHandler,
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
void le_lpt_DisconnectService
(
    void
);


//--------------------------------------------------------------------------------------------------
/**
 * eDRX Radio Access Technology enum
 */
//--------------------------------------------------------------------------------------------------
typedef enum
{
    LE_LPT_EDRX_RAT_UNKNOWN = 0,
        ///< Unknown
    LE_LPT_EDRX_RAT_EC_GSM_IOT = 1,
        ///< EC-GSM-IoT (A/Gb mode)
    LE_LPT_EDRX_RAT_GSM = 2,
        ///< GSM (A/Gb mode)
    LE_LPT_EDRX_RAT_UTRAN = 3,
        ///< UTRAN (Iu mode)
    LE_LPT_EDRX_RAT_LTE_M1 = 4,
        ///< E-UTRAN (WB-S1 mode)
    LE_LPT_EDRX_RAT_LTE_NB1 = 5,
        ///< E-UTRAN (NB-S1 mode)
    LE_LPT_EDRX_RAT_MAX = 6
        ///< Invalid
}
le_lpt_EDrxRat_t;


//--------------------------------------------------------------------------------------------------
/**
 * Reference type used by Add/Remove functions for EVENT 'le_lpt_EDrxParamsChange'
 */
//--------------------------------------------------------------------------------------------------
typedef struct le_lpt_EDrxParamsChangeHandler* le_lpt_EDrxParamsChangeHandlerRef_t;


//--------------------------------------------------------------------------------------------------
/**
 * Handler to report a change in the network-provided eDRX parameters.
 */
//--------------------------------------------------------------------------------------------------
typedef void (*le_lpt_EDrxParamsChangeHandlerFunc_t)
(
    le_lpt_EDrxRat_t rat,
        ///< Radio Access Technology.
    le_onoff_t activation,
        ///< eDRX activation state.
    uint8_t eDrxValue,
        ///< eDRX cycle value, defined in 3GPP
        ///< TS 24.008 Rel-13 section 10.5.5.32.
    uint8_t pagingTimeWindow,
        ///< Paging Time Window, defined in 3GPP
        ///< TS 24.008 Rel-13 section 10.5.5.32.
    void* contextPtr
        ///<
);

//--------------------------------------------------------------------------------------------------
/**
 * Set the eDRX activation state for the given Radio Access Technology.
 *
 * @return
 *  - LE_OK             The function succeeded.
 *  - LE_BAD_PARAMETER  A parameter is invalid.
 *  - LE_UNSUPPORTED    eDRX is not supported by the platform.
 *  - LE_FAULT          The function failed.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_lpt_SetEDrxState
(
    le_lpt_EDrxRat_t rat,
        ///< [IN] Radio Access Technology.
    le_onoff_t activation
        ///< [IN] eDRX activation state.
);

//--------------------------------------------------------------------------------------------------
/**
 * Set the requested eDRX cycle value for the given Radio Access Technology.
 * The eDRX cycle value is defined in 3GPP TS 24.008 Release 13 section 10.5.5.32.
 *
 * @return
 *  - LE_OK             The function succeeded.
 *  - LE_BAD_PARAMETER  A parameter is invalid.
 *  - LE_UNSUPPORTED    eDRX is not supported by the platform.
 *  - LE_FAULT          The function failed.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_lpt_SetRequestedEDrxValue
(
    le_lpt_EDrxRat_t rat,
        ///< [IN] Radio Access Technology.
    uint8_t eDrxValue
        ///< [IN] Requested eDRX cycle value, defined in 3GPP
        ///< TS 24.008 Rel-13 section 10.5.5.32.
);

//--------------------------------------------------------------------------------------------------
/**
 * Get the requested eDRX cycle value for the given Radio Access Technology.
 * The eDRX cycle value is defined in 3GPP TS 24.008 Release 13 section 10.5.5.32.
 *
 * @return
 *  - LE_OK             The function succeeded.
 *  - LE_BAD_PARAMETER  A parameter is invalid.
 *  - LE_UNSUPPORTED    eDRX is not supported by the platform.
 *  - LE_UNAVAILABLE    No requested eDRX cycle value.
 *  - LE_FAULT          The function failed.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_lpt_GetRequestedEDrxValue
(
    le_lpt_EDrxRat_t rat,
        ///< [IN] Radio Access Technology.
    uint8_t* eDrxValuePtr
        ///< [OUT] Requested eDRX cycle value, defined in 3GPP
        ///< TS 24.008 Rel-13 section 10.5.5.32.
);

//--------------------------------------------------------------------------------------------------
/**
 * Get the network-provided eDRX cycle value for the given Radio Access Technology.
 * The eDRX cycle value is defined in 3GPP TS 24.008 Release 13 section 10.5.5.32.
 *
 * @return
 *  - LE_OK             The function succeeded.
 *  - LE_BAD_PARAMETER  A parameter is invalid.
 *  - LE_UNSUPPORTED    eDRX is not supported by the platform.
 *  - LE_UNAVAILABLE    No network-provided eDRX cycle value.
 *  - LE_FAULT          The function failed.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_lpt_GetNetworkProvidedEDrxValue
(
    le_lpt_EDrxRat_t rat,
        ///< [IN] Radio Access Technology.
    uint8_t* eDrxValuePtr
        ///< [OUT] Network-provided eDRX cycle value, defined in
        ///< 3GPP TS 24.008 Rel-13 section 10.5.5.32.
);

//--------------------------------------------------------------------------------------------------
/**
 * Get the network-provided Paging Time Window for the given Radio Access Technology.
 * The Paging Time Window is defined in 3GPP TS 24.008 Release 13 section 10.5.5.32.
 *
 * @return
 *  - LE_OK             The function succeeded.
 *  - LE_BAD_PARAMETER  A parameter is invalid.
 *  - LE_UNSUPPORTED    eDRX is not supported by the platform.
 *  - LE_UNAVAILABLE    No defined Paging Time Window.
 *  - LE_FAULT          The function failed.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_lpt_GetNetworkProvidedPagingTimeWindow
(
    le_lpt_EDrxRat_t rat,
        ///< [IN] Radio Access Technology.
    uint8_t* pagingTimeWindowPtr
        ///< [OUT] Network-provided Paging Time Window, defined
        ///< in 3GPP TS 24.008 Rel-13 section 10.5.5.32.
);

//--------------------------------------------------------------------------------------------------
/**
 * Add handler function for EVENT 'le_lpt_EDrxParamsChange'
 *
 * Event to report a change in the network-provided eDRX parameters.
 */
//--------------------------------------------------------------------------------------------------
le_lpt_EDrxParamsChangeHandlerRef_t le_lpt_AddEDrxParamsChangeHandler
(
    le_lpt_EDrxParamsChangeHandlerFunc_t handlerPtr,
        ///< [IN]
    void* contextPtr
        ///< [IN]
);

//--------------------------------------------------------------------------------------------------
/**
 * Remove handler function for EVENT 'le_lpt_EDrxParamsChange'
 */
//--------------------------------------------------------------------------------------------------
void le_lpt_RemoveEDrxParamsChangeHandler
(
    le_lpt_EDrxParamsChangeHandlerRef_t handlerRef
        ///< [IN]
);

#endif // LE_LPT_INTERFACE_H_INCLUDE_GUARD