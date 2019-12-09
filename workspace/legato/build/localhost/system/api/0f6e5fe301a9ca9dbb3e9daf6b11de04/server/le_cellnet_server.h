

/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */


#ifndef LE_CELLNET_INTERFACE_H_INCLUDE_GUARD
#define LE_CELLNET_INTERFACE_H_INCLUDE_GUARD


#include "legato.h"

// Interface specific includes
#include "le_sim_server.h"


//--------------------------------------------------------------------------------------------------
/**
 * Get the server service reference
 */
//--------------------------------------------------------------------------------------------------
le_msg_ServiceRef_t le_cellnet_GetServiceRef
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * Get the client session reference for the current message
 */
//--------------------------------------------------------------------------------------------------
le_msg_SessionRef_t le_cellnet_GetClientSessionRef
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * Initialize the server and advertise the service.
 */
//--------------------------------------------------------------------------------------------------
void le_cellnet_AdvertiseService
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