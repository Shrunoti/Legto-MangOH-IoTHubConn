

/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */

/**
 * @page c_pm Power Manager
 *
 * @ref le_pm_interface.h "API Reference"
 *
 * <HR>
 *
 * Components need access to the Power Manager to control the system's wake-up state.
 * Operations that need fast response times (e.g., maintaining call state or playing/recording a
 * media stream) result in high interrupt rates; keeping the system awake results in better performance
 * and power efficiency.
 *
 * Power Manager uses kernel wakeup sources to keep the system awake when at least one of the
 * registered components requests a wakeup source to be held. When all wakeup sources are
 * released, the system may enter a suspend state depending on the status of other (unrelated) wakeup
 * sources.
 *
 * @section le_pm_binding IPC interfaces binding
 *
 * All the functions of this API are provided by the @b powerMgr service.
 *
 * Here's a code sample binding to Power Manager services:
 * @verbatim
   bindings:
   {
      clientExe.clientComponent.le_pm -> powerMgr.le_pm
   }
   @endverbatim
 *
 * @section le_pm_request Requesting and releasing a wakeup source
 *
 * The Power Manager service provides basic API for requesting and releasing a wakeup source.
 * Power Manager's clients call @c le_pm_NewWakeupSource() to create a wakeup source. This function
 * returns a @ref le_pm_WakeupSourceRef_t type that can later be used to acquire and release a
 * wakeup source through @c le_pm_StayAwake() and le_pm_Relax(), respectively. Wakeup sources
 * are not reference-counted, which means multiple calls to le_pm_StayAwake() can be canceled
 * by a single call to le_pm_Relax().
 *
 * To have a reference-counted wakeup-source, set the LE_PM_REF_COUNT bit in the opts argument.
 * When this bit is set, each le_pm_StayAwake() increments a counter, and multiple calls to
 * le_pm_Relax() is necessary to release the wakeup source.
 *
 * Power Manager service will automatically release and delete all wakeup sources held on behalf
 * of an exiting or disconnecting client.
 *
 * The service le_pm_ForceRelaxAndDestroyAllWakeupSource() will return LE_NOT_PERMITTED until a
 * call to le_pm_StayAwake() fails with LE_NO_MEMORY. This should be considered as an ultime
 * defense if no more wakeup sources may be acquired or relased. This service will kill all
 * clients and release and destroy all wakeup source currently managed.
 *
 * At startup, the Power Manager will release all wakeup sources matching the pattern for Legato
 * apps and will keep the others untouched.
 *
 * For deterministic behaviour, clients requesting services of Power Manager should have
 * CAP_EPOLLWAKEUP (or CAP_BLOCK_SUSPEND) capability assigned.
 *
 * The maximum number of wakeup sources managed at same time is fixed by the kernel configuration
 * option CONFIG_PM_WAKELOCKS_LIMIT.
 *
 * <HR>
 *
 * Copyright (C) Sierra Wireless Inc.
 */
/**
 * @file le_pm_interface.h
 *
 * Legato @ref c_pm include file.
 *
 * Copyright (C) Sierra Wireless Inc.
 */
/**
 * Some useful constants
 */

#ifndef LE_PM_INTERFACE_H_INCLUDE_GUARD
#define LE_PM_INTERFACE_H_INCLUDE_GUARD


#include "legato.h"


//--------------------------------------------------------------------------------------------------
/**
 * Type for handler called when a server disconnects.
 */
//--------------------------------------------------------------------------------------------------
typedef void (*le_pm_DisconnectHandler_t)(void *);

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
void le_pm_ConnectService
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
le_result_t le_pm_TryConnectService
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
void le_pm_SetServerDisconnectHandler
(
    le_pm_DisconnectHandler_t disconnectHandler,
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
void le_pm_DisconnectService
(
    void
);


//--------------------------------------------------------------------------------------------------
/**
 * Maximum string length for a wake-up source tag (not including the null-terminator)
 */
//--------------------------------------------------------------------------------------------------
#define LE_PM_TAG_LEN 31

//--------------------------------------------------------------------------------------------------
/**
 * Maximum string length for a wake-up source tag (including the null-terminator)
 */
//--------------------------------------------------------------------------------------------------
#define LE_PM_TAG_LEN_BYTES 32

//--------------------------------------------------------------------------------------------------
/**
 * Option LE_PM_REF_COUNT to manage a reference counted wakeup source
 */
//--------------------------------------------------------------------------------------------------
#define LE_PM_REF_COUNT 1

//--------------------------------------------------------------------------------------------------
/**
 * Reference to wakeup source used by StayAwake and Relax function
 */
//--------------------------------------------------------------------------------------------------
typedef struct le_pm_WakeupSource* le_pm_WakeupSourceRef_t;


//--------------------------------------------------------------------------------------------------
/**
 * Create a wakeup source
 *
 * @return
 *      - Reference to wakeup source (to be used later for acquiring/releasing)
 *
 * @note The process exits if an invalid or existing tag is passed
 */
//--------------------------------------------------------------------------------------------------
le_pm_WakeupSourceRef_t le_pm_NewWakeupSource
(
    uint32_t createOpts,
        ///< [IN] Wakeup source options
    const char* LE_NONNULL wsTag
        ///< [IN] Context-specific wakeup source tag
);

//--------------------------------------------------------------------------------------------------
/**
 * Acquire a wakeup source
 *
 * @return
 *     - LE_OK          if the wakeup source is acquired
 *     - LE_NO_MEMORY   if the wakeup sources limit is reached
 *     - LE_FAULT       for other errors
 *
 * @note The process exits if an invalid reference is passed
 * @note The wakeup sources limit is fixed by the kernel CONFIG_PM_WAKELOCKS_LIMIT configuration
 *       variable
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_pm_StayAwake
(
    le_pm_WakeupSourceRef_t wsRef
        ///< [IN] Reference to a created wakeup source
);

//--------------------------------------------------------------------------------------------------
/**
 * Release a previously acquired wakeup source
 *
 * @return
 *     - LE_OK          if the wakeup source is acquired
 *     - LE_NOT_FOUND   if the wakeup source was not currently acquired
 *     - LE_FAULT       for other errors
 *
 * @note The process exits if an invalid reference is passed
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_pm_Relax
(
    le_pm_WakeupSourceRef_t wsRef
        ///< [IN] Reference to a created wakeup source
);

//--------------------------------------------------------------------------------------------------
/**
 * Release and destroy all acquired wakeup source, kill all clients
 *
 * @return
 *     - LE_OK              if the wakeup source is acquired
 *     - LE_NOT_PERMITTED   if the le_pm_StayAwake() has not failed with LE_NO_MEMORY
 *     - LE_FAULT           for other errors
 *
 * @note The service is available only if le_pm_StayAwake() has returned LE_NO_MEMORY. It should be
 *       used in the way to release and destroy all wakeup sources.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_pm_ForceRelaxAndDestroyAllWakeupSource
(
    void
);

#endif // LE_PM_INTERFACE_H_INCLUDE_GUARD