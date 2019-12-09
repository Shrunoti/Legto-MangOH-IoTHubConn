

/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */

/**
 * @page c_update Update API
 *
 * @ref le_update_interface.h "API Reference" <br>
 * @ref legatoServicesUpdate <br>
 * @ref legatoServicesUpdatePack <br>
 * @ref conceptsUpdates <br>
 * @ref getStartedUpdate
 *
 *
 * This API uses @ref legatoServicesUpdatePack to update a target device software/firmware.
 *
 * Update packs can contain one or more @e tasks to be performed by the
 * @ref basicRuntimeArch_updateDaemon.
 *
 * From the client view, the update service follows this state machine
 * while doing an update:
 *
 *@image html updateApi.png
 *
 * @section API_Usage_Guideline API Usage Guidelines
 *
 * Typically, the API is used like this:
 *
 * 1. Client calls le_update_Start() providing a file descriptor for the update service to
 *    read the update pack and a notification callback function to call with updates.
 *
 * 2. Progress reports are sent to the client periodically through the notification function.
 *
 * 3. If the update fails, le_update_GetErrorCode() can be used to find out more info.
 *
 * 4. When the client is finished with the update, the client MUST call le_update_End() to
 *    relocate resources.
 *
 * To cancel an update before it finishes, call le_update_End().
 *
 * If the client disconnects before ending the update session, the session will automatically end.
 * If the update is still in progress, it may be cancelled (if it's not too late).
 *
 *
 * @section update_example Sample Code
 *
 * This C code sample calls an update service provider API to perform an update:
 *
 * @code
 *
 * void SoftwareUpdate(void)
 * {
 *     int fd = 0;  // Update data coming via STDIN
 *
 *     le_result_t result = le_update_Start(fd, UpdateProgressHandler, NULL);
 *     if (result != LE_OK)
 *     {
 *          fprintf(stderr, "Update refused by server.  Try again later.\n");
 *
 *          // NOTE: It's okay to not delete the update here because we are exiting, so the handle
 *          //       will be deleted automatically.
 *
 *          exit(EXIT_FAILURE);
 *     }
 * }
 *
 *
 * // Sample callback function implementation.
 * static void UpdateProgressHandler
 * (
 *     le_update_State_t updateState,   ///< Current state of the update.
 *     uint32 percentDone,              ///< Percent done for current state.
 *     void* contextPtr                 ///< Context pointer (NULL).
 * )
 * {
 *     switch(updateState)
 *     {
 *         case LE_UPDATE_STATE_UNPACKING:
 *             fprintf(stdout, "Unpacking: %d%% \n", percentDone);
 *             break;
 *
 *         case LE_UPDATE_STATE_DOWNLOAD_SUCCESS:
 *             le_update_Install();
 *             break;
 *
 *         case LE_UPDATE_STATE_APPLYING:
 *             fprintf(stdout, "Applying: %d%% \n", percentDone);
 *             break;
 *
 *         case LE_UPDATE_STATE_SUCCESS:
 *             fprintf(stdout, "\nSUCCESS\n");
 *             exit(EXIT_SUCCESS);
 *
 *         case LE_UPDATE_STATE_FAILED:
 *             fprintf(stderr, "\nFAILED: error code %d\n", le_update_GetErrorCode());
 *             exit(EXIT_FAILURE);
 *     }
 * }
 *
 * @endcode
 *
 * @section Update_API_System_Info Installed System Information
 *
 * It is possible to get the index and hash for all of the currently installed systems.  The
 * following is an example of how one would list all installed systems and their hashes.
 *
 * @code
 *
 * int32_t systemIndex = le_update_GetCurrentSysIndex();
 *
 * do
 * {
 *     char hashBuffer[LE_LIMIT_MD5_STR_LEN + 1];
 *
 *     if (le_update_GetSystemHash(systemIndex, hashBuffer, sizeof(hashBuffer)) == LE_OK)
 *     {
 *         LE_INFO("System: %d -- %s", systemIndex, hashBuffer);
 *     }
 *     else
 *     {
 *         LE_ERROR("System: %d -- NOT FOUND", systemIndex);
 *     }
 * }
 * while ((systemIndex = le_update_GetPreviousSystemIndex(systemIndex)) != -1);
 *
 * @endcode
 *
 * Copyright (C) Sierra Wireless Inc.
 */
/**
 * @file le_update_interface.h
 *
 * Legato @ref c_update include file.
 *
 * Copyright (C) Sierra Wireless Inc.
 */

#ifndef LE_UPDATE_INTERFACE_H_INCLUDE_GUARD
#define LE_UPDATE_INTERFACE_H_INCLUDE_GUARD


#include "legato.h"

// Interface specific includes
#include "le_limit_interface.h"


//--------------------------------------------------------------------------------------------------
/**
 * Type for handler called when a server disconnects.
 */
//--------------------------------------------------------------------------------------------------
typedef void (*le_update_DisconnectHandler_t)(void *);

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
void le_update_ConnectService
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
le_result_t le_update_TryConnectService
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
void le_update_SetServerDisconnectHandler
(
    le_update_DisconnectHandler_t disconnectHandler,
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
void le_update_DisconnectService
(
    void
);


//--------------------------------------------------------------------------------------------------
/**
 * Different states in the update state machine. The state machine is maintained to track the
 * update task underway.
 *
 * Example:
 *     for successful installation task, state transitions look like:
 *
 *@verbatim
    --> UNPACKING --> DOWNLOAD_SUCCESS --> APPLYING --> SUCCESS.
            |                                 |
            +---------------------------------+-------> FAILED.
@endverbatim
 */
//--------------------------------------------------------------------------------------------------
typedef enum
{
    LE_UPDATE_STATE_UNPACKING = 0,
        ///< Unpacking update data.
    LE_UPDATE_STATE_DOWNLOAD_SUCCESS = 1,
        ///< Update data downloaded successfully.
    LE_UPDATE_STATE_APPLYING = 2,
        ///< Applying update(i.e. installation/removal operation going on).
    LE_UPDATE_STATE_SUCCESS = 3,
        ///< Successfully completed all update task.
    LE_UPDATE_STATE_FAILED = 4
        ///< Update failed due to some error or deletion request.
}
le_update_State_t;


//--------------------------------------------------------------------------------------------------
/**
 * Error code used to provide diagnostic information after a failed update.
 *
 * @note
 *     Additional information may also be available in the target device's system log.
 */
//--------------------------------------------------------------------------------------------------
typedef enum
{
    LE_UPDATE_ERR_NONE = 0,
        ///< No error.
    LE_UPDATE_ERR_BAD_PACKAGE = 1,
        ///< Encountered bad update package. Check logs.
    LE_UPDATE_ERR_INTERNAL_ERROR = 2,
        ///< Something failed while doing update.  Check logs.
    LE_UPDATE_ERR_SECURITY_FAILURE = 3
        ///< Error while doing security check of the package.
}
le_update_ErrorCode_t;


//--------------------------------------------------------------------------------------------------
/**
 * Reference type used by Add/Remove functions for EVENT 'le_update_Progress'
 */
//--------------------------------------------------------------------------------------------------
typedef struct le_update_ProgressHandler* le_update_ProgressHandlerRef_t;


//--------------------------------------------------------------------------------------------------
/**
 * The client callback function (handler) passed to le_update_Start() must look like this.
 */
//--------------------------------------------------------------------------------------------------
typedef void (*le_update_ProgressHandlerFunc_t)
(
    le_update_State_t updateState,
        ///< Current state of update.
    uint32_t percentDone,
        ///< Percent done for current state. For example, in state
        ///< UNPACKING, a percentDone of 80 means that 80% of the update
        ///< data has been unpacked.
    void* contextPtr
        ///<
);

//--------------------------------------------------------------------------------------------------
/**
 * Add handler function for EVENT 'le_update_Progress'
 *
 * Register for notification of the progress of a given update.
 */
//--------------------------------------------------------------------------------------------------
le_update_ProgressHandlerRef_t le_update_AddProgressHandler
(
    le_update_ProgressHandlerFunc_t handlerPtr,
        ///< [IN] Progress handler
    void* contextPtr
        ///< [IN]
);

//--------------------------------------------------------------------------------------------------
/**
 * Remove handler function for EVENT 'le_update_Progress'
 */
//--------------------------------------------------------------------------------------------------
void le_update_RemoveProgressHandler
(
    le_update_ProgressHandlerRef_t handlerRef
        ///< [IN]
);

//--------------------------------------------------------------------------------------------------
/**
 * Starts an update.
 *
 * Progress is reported via the progress handler callback.
 *
 * @return
 *      - LE_OK if accepted.
 *      - LE_BUSY if another update is in progress.
 *      - LE_UNAVAILABLE if the system is still in "probation" (not marked "good" yet).
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_update_Start
(
    int fd
        ///< [IN] Open file descriptor from which the update can be read.
);

//--------------------------------------------------------------------------------------------------
/**
 * Install the update
 *
 * @return
 *      - LE_OK if installation started.
 *      - LE_BUSY if package download is not finished yet.
 *      - LE_FAULT if there is an error. Check logs
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_update_Install
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * Ends an update session.  If the update isn't finished yet, cancels it.
 */
//--------------------------------------------------------------------------------------------------
void le_update_End
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * Function to get error code when update fails.
 *
 * @return
 *      - Error code of encountered error.
 *      - ERR_NONE if update is in any other state.
 */
//--------------------------------------------------------------------------------------------------
le_update_ErrorCode_t le_update_GetErrorCode
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * Get the index of the currently running system.
 *
 * @return The index of the running system.
 */
//--------------------------------------------------------------------------------------------------
int32_t le_update_GetCurrentSysIndex
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * Gets the hash ID for a given system.
 *
 * @return
 *      - LE_OK if no problems are encountered.
 *      - LE_NOT_FOUND if the given index does not correspond to an available system.
 *      - LE_OVERFLOW if the supplied buffer is too small.
 *      - LE_FORMAT_ERROR if there are problems reading the hash for the system.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_update_GetSystemHash
(
    int32_t systemIndex,
        ///< [IN] The system to read the hash for.
    char* hashStr,
        ///< [OUT] Buffer to hold the system hash string.
    size_t hashStrSize
        ///< [IN]
);

//--------------------------------------------------------------------------------------------------
/**
 * Get the index for the previous system in the chain, using the current system as a starting point.
 *
 * @return The index to the system that's previous to the given system.  -1 is returned if the
 *         previous system was not found.
 */
//--------------------------------------------------------------------------------------------------
int32_t le_update_GetPreviousSystemIndex
(
    int32_t systemIndex
        ///< [IN] Get the system that's older than this system.
);

#endif // LE_UPDATE_INTERFACE_H_INCLUDE_GUARD