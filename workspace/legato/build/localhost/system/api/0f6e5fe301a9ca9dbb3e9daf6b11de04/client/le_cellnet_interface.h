

/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */

/**
 * @page c_le_cellnet Cellular Network
 *
 * @ref le_cellnet_interface.h "API Reference"
 *
 * <HR>
 *
 * The Cellular Network service ensures that the modem is registered on the network when an user
 * application makes a request for network access. This includes:
 * - ensuring that the radio is turned on.
 * - ensuring that there is a valid SIM, and it is unlocked.
 * - ensuring that the modem is registered on the network.
 *
 * If all of the above conditions are met, then the service indicates that the network is available.
 *
 * @section le_cellnet_binding IPC interfaces binding
 *
 * All the functions of this API are provided by the @b cellNetService application service.
 *
 * Here's a code sample binding to Cellular Network services:
 * @verbatim
   bindings:
   {
      clientExe.clientComponent.le_cellnet -> cellNetService.le_cellnet
   }
   @endverbatim
 *
 * @section c_le_cellnet_requesting Requesting the Cellular Network
 *
 * The Cellular Network can be requested using le_cellnet_Request(). This function will turn on the
 * radio if it is switched off and it will unlock the SIM if a PIN code is required (the necessary
 * information will be retrieved from the secure storage, cf. @ref c_le_cellnet_config).
 *
 * Before the cellular network is requested, an application should register a network state
 * handler using le_cellnet_AddStateEventHandler(). Once the cellular network becomes available,
 * the handler will be called to indicate that the modem is now registered on the network.
 *
 * If the state of the network changes, then the handler will be called with the new state.
 * The current cellular network state can also be retrieved with le_cellnet_GetNetworkState().
 * @note There might be a discrepancy between the state notified by the handler and the state
 * retrieved through le_cellnet_GetNetworkState(): the state can have changed between the event
 * notification and its treatment by the application.
 *
 * To release the cellular network, an application can use le_cellnet_Release(). Once all user
 * applications release the cellular network access, then the service will turn off the radio.
 *
 * The application can release the network state handler by calling
 * le_cellnet_RemoveStateEventHandler() when it is not needed anymore.
 *
 * All configuration data required for a network registration, such as the PIN code of the SIM,
 * will be stored in the secure storage by the use of the le_cellnet_SetSimPinCode() function.
 *
 *
 * @section c_le_cellnet_config Cellular Network configuration
 *
 * Setting the SIM PIN code in the secure storage and retrieving it is performed by two functions:
 * le_cellnet_SetSimPinCode() and le_cellnet_GetSimPinCode().
 *
 * The le_cellnet_SetSimPinCode() function stores the PIN code in the secure storage and takes two
 * parameters: the SIM identifier and the SIM card PIN code.
 *
 * @note The function erases the previously stored SIM card PIN code.
 * @note It is possible to set and retry a void entry ("") corresponding to an unlocked SIM.
 *
 * The le_cellnet_GetSimPinCode() function retrieves the SIM card PIN code from the secure storage
 * and takes one parameter: the SIM identifier.
 *
 * @note The action carried out by the Cellular Network Service when a new SIM is inserted depends
 * on the SIM state:
 *   - if the SIM is locked, it will automatically read the secure storage in order to try to enter
 *     the PIN code for the SIM card. If the PIN code is not set or too long (max 8 digits), an
 *     error will be logged and the SIM will not be unlocked.
 *   - is the SIM is blocked, it just logs an error and does not try to enter the PUK code.
 *
 * Copyright (C) Sierra Wireless Inc.
 */
/**
 * @file le_cellnet_interface.h
 *
 * Legato @ref c_le_cellnet include file.
 *
 * Copyright (C) Sierra Wireless Inc.
 */

#ifndef LE_CELLNET_INTERFACE_H_INCLUDE_GUARD
#define LE_CELLNET_INTERFACE_H_INCLUDE_GUARD


#include "legato.h"

// Interface specific includes
#include "le_sim_interface.h"


//--------------------------------------------------------------------------------------------------
/**
 * Type for handler called when a server disconnects.
 */
//--------------------------------------------------------------------------------------------------
typedef void (*le_cellnet_DisconnectHandler_t)(void *);

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
void le_cellnet_ConnectService
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
le_result_t le_cellnet_TryConnectService
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
void le_cellnet_SetServerDisconnectHandler
(
    le_cellnet_DisconnectHandler_t disconnectHandler,
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
void le_cellnet_DisconnectService
(
    void
);


//--------------------------------------------------------------------------------------------------
/**
 * Reference returned by Request function and used by Release function
 */
//--------------------------------------------------------------------------------------------------
typedef struct le_cellnet_RequestObj* le_cellnet_RequestObjRef_t;


//--------------------------------------------------------------------------------------------------
/**
 * Cellular Network states.
 *
 */
//--------------------------------------------------------------------------------------------------
typedef enum
{
    LE_CELLNET_RADIO_OFF = 0,
        ///< The radio is switched-off.
    LE_CELLNET_REG_EMERGENCY = 1,
        ///< Only emergency calls are allowed.
    LE_CELLNET_REG_HOME = 2,
        ///< Registered, home network.
    LE_CELLNET_REG_ROAMING = 3,
        ///< Registered to a roaming network.
    LE_CELLNET_REG_UNKNOWN = 4,
        ///< Unknown state.
    LE_CELLNET_SIM_ABSENT = 5
        ///< No SIM card available.
}
le_cellnet_State_t;


//--------------------------------------------------------------------------------------------------
/**
 * Reference type used by Add/Remove functions for EVENT 'le_cellnet_StateEvent'
 */
//--------------------------------------------------------------------------------------------------
typedef struct le_cellnet_StateEventHandler* le_cellnet_StateEventHandlerRef_t;


//--------------------------------------------------------------------------------------------------
/**
 * Handler for network state changes
 */
//--------------------------------------------------------------------------------------------------
typedef void (*le_cellnet_StateHandlerFunc_t)
(
    le_cellnet_State_t state,
        ///< The cellular network state
    void* contextPtr
        ///<
);

//--------------------------------------------------------------------------------------------------
/**
 * Add handler function for EVENT 'le_cellnet_StateEvent'
 *
 * This event provides information on network state changes
 */
//--------------------------------------------------------------------------------------------------
le_cellnet_StateEventHandlerRef_t le_cellnet_AddStateEventHandler
(
    le_cellnet_StateHandlerFunc_t handlerPtr,
        ///< [IN]
    void* contextPtr
        ///< [IN]
);

//--------------------------------------------------------------------------------------------------
/**
 * Remove handler function for EVENT 'le_cellnet_StateEvent'
 */
//--------------------------------------------------------------------------------------------------
void le_cellnet_RemoveStateEventHandler
(
    le_cellnet_StateEventHandlerRef_t handlerRef
        ///< [IN]
);

//--------------------------------------------------------------------------------------------------
/**
 * Request a cellular network
 *
 * @return
 *      - A reference to the cellular network
 *      - 0 (zero) if the network requested could not be processed
 */
//--------------------------------------------------------------------------------------------------
le_cellnet_RequestObjRef_t le_cellnet_Request
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * Release a cellular network
 */
//--------------------------------------------------------------------------------------------------
void le_cellnet_Release
(
    le_cellnet_RequestObjRef_t cellNetRef
        ///< [IN] Reference to a cellular network request.
);

//--------------------------------------------------------------------------------------------------
/**
 * Set the PIN code in the secure storage.
 *
 * @return
 *  - LE_OUT_OF_RANGE    Invalid simId
 *  - LE_FORMAT_ERROR    PIN code is not in string format.
 *  - LE_UNDERFLOW       The PIN code is not long enough (min 4 digits).
 *  - LE_OK              The function succeeded.
 *  - LE_FAULT           The function failed on any other errors
 *
 * @note If PIN code is too long (max 8 digits), it is a fatal error, the
 *       function will not return.
 *
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_cellnet_SetSimPinCode
(
    le_sim_Id_t simId,
        ///< [IN] SIM identifier.
    const char* LE_NONNULL pinCode
        ///< [IN] PIN code to insert in the secure storage.
);

//--------------------------------------------------------------------------------------------------
/**
 * Retrieve the PIN code from the secure storage.
 *
 * @return
 *  - LE_OUT_OF_RANGE    Invalid simId
 *  - LE_NOT_FOUND       PIN code is not found in the secure storage.
 *  - LE_OVERFLOW        PIN code exceeds the maximum length of 8 digits.
 *  - LE_UNDERFLOW       The PIN code is not long enough (min 4 digits).
 *  - LE_OK              The function succeeded.
 *  - LE_FAULT           If there are some other errors.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_cellnet_GetSimPinCode
(
    le_sim_Id_t simId,
        ///< [IN] SIM identifier.
    char* pinCode,
        ///< [OUT] Read the PIN code from the secure storage.
    size_t pinCodeSize
        ///< [IN]
);

//--------------------------------------------------------------------------------------------------
/**
 * Retrieve the current cellular network state.
 *
 * @return
 *  - LE_OK             The function succeeded.
 *  - LE_FAILED         The function failed
 *  - LE_BAD_PARAMETER  A bad parameter was passed.
 *
 * @note If the caller passes a null pointer to this function, this is a fatal error and the
 *       function will not return.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_cellnet_GetNetworkState
(
    le_cellnet_State_t* statePtr
        ///< [OUT] Cellular network state.
);

#endif // LE_CELLNET_INTERFACE_H_INCLUDE_GUARD