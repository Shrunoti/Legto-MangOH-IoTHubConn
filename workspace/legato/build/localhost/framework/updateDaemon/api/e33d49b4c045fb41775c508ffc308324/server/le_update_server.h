

/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */


#ifndef LE_UPDATE_INTERFACE_H_INCLUDE_GUARD
#define LE_UPDATE_INTERFACE_H_INCLUDE_GUARD


#include "legato.h"

// Interface specific includes
#include "le_limit_server.h"


//--------------------------------------------------------------------------------------------------
/**
 * Get the server service reference
 */
//--------------------------------------------------------------------------------------------------
le_msg_ServiceRef_t le_update_GetServiceRef
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * Get the client session reference for the current message
 */
//--------------------------------------------------------------------------------------------------
le_msg_SessionRef_t le_update_GetClientSessionRef
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * Initialize the server and advertise the service.
 */
//--------------------------------------------------------------------------------------------------
void le_update_AdvertiseService
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