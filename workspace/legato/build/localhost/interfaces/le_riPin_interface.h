

/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */

/**
 * @page c_riPin Ring Indicator Signal
 *
 * @ref le_riPin_interface.h "API Reference"
 *
 * <HR>
 *
 * The Ring Indicator pin is used to allow the module to wake up a host device.
 *
 * @section c_riPin_binding IPC interfaces binding
 *
 * All the functions of this API are provided by the @b modemService app.
 *
 * Here's a code sample binding to modem services:
 * @verbatim
   bindings:
   {
      clientExe.clientComponent.le_riPin -> modemService.le_riPin
   }
   @endverbatim
 *
 * @section c_riPin_usage Ring Indication signal
 *
 *
 * The RING pin can be configured to notify a host device with different timing of pulse signals for
 * different module activities.
 *
 * - le_riPin_AmIOwnerOfRingSignal() function checks whether the application core is the current
 *   owner of the Ring Indicator signal.
 * - le_riPin_TakeRingSignal() function takes control of the Ring Indicator signal.
 * - le_riPin_ReleaseRingSignal() function releases control of the Ring Indicator signal.
 *
 * - le_riPin_PulseRingSignal() function sets the ring signal high for configurable duration of time
 *   before lowering it.
 *
 * <HR>
 * Copyright (C) Sierra Wireless Inc.
 */
/**
 * @file le_riPin_interface.h
 *
 * Legato @ref c_riPin include file.
 *
 * Copyright (C) Sierra Wireless Inc.
 */

#ifndef LE_RIPIN_INTERFACE_H_INCLUDE_GUARD
#define LE_RIPIN_INTERFACE_H_INCLUDE_GUARD


#include "legato.h"


//--------------------------------------------------------------------------------------------------
/**
 * Type for handler called when a server disconnects.
 */
//--------------------------------------------------------------------------------------------------
typedef void (*le_riPin_DisconnectHandler_t)(void *);

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
void le_riPin_ConnectService
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
le_result_t le_riPin_TryConnectService
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
void le_riPin_SetServerDisconnectHandler
(
    le_riPin_DisconnectHandler_t disconnectHandler,
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
void le_riPin_DisconnectService
(
    void
);


//--------------------------------------------------------------------------------------------------
/**
 * Check whether the application core is the current owner of the Ring Indicator signal.
 *
 * @return
 *      - LE_OK              The function succeeded.
 *      - LE_FAULT           The function failed.
 *      - LE_BAD_PARAMETER   Bad input parameter.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_riPin_AmIOwnerOfRingSignal
(
    bool* amIOwnerPtr
        ///< [OUT] true when application core is the owner of the Ring Indicator signal,
);

//--------------------------------------------------------------------------------------------------
/**
 * Take control of the Ring Indicator signal.
 *
 * @return
 *      - LE_OK           The function succeeded.
 *      - LE_FAULT        The function failed.
 *      - LE_UNSUPPORTED  The platform does not support this operation.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_riPin_TakeRingSignal
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * Release control of the Ring Indicator signal.
 *
 * @return
 *      - LE_OK           The function succeeded.
 *      - LE_FAULT        The function failed.
 *      - LE_UNSUPPORTED  The platform does not support this operation.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_riPin_ReleaseRingSignal
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * Set the Ring Indicator signal to high for configurable duration of time before lowering it.
 *
 */
//--------------------------------------------------------------------------------------------------
void le_riPin_PulseRingSignal
(
    uint32_t duration
        ///< [IN] [IN] Duration in ms
);

#endif // LE_RIPIN_INTERFACE_H_INCLUDE_GUARD