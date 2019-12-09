

/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */

/**
 * @page c_updateCtrl Update Control API
 *
 * @ref le_updateCtrl_interface.h "API Reference"
 *
 * <HR>
 *
 * This API is used by privileged apps to control the software update process.  It can be used to:
 * - prevent updates from happening or
 * - control the marking of a system update as "good" or "bad".
 *
 * Note that this is not part of the Update API (le_update) because the
 * Update API can be used by less trusted apps to feed signed/encrypted updates to the
 * Update Daemon, and we may not trust those same apps to have the ability to prevent someone
 * else from updating or rolling-back the system.
 *
 *
 * @section le_updateCtrl_prevention Preventing Updates During Critical Operations
 *
 * To protect against updates during critical periods of operation (such as when performing an
 * emergency call in response to a vehicle collision), the following functions are provided:
 *
 * - le_updateCtrl_Defer() - prevent all updates until further notice.
 * - le_updateCtrl_Allow() - allow updates to go ahead.
 *
 * Updates will not be allowed to go ahead until no clients are deferring updates. So, if client A
 * and client B both call le_updateCtrl_Defer(), updates will be deferred until both client A and
 * client B have called le_updateCtrl_Allow().
 *
 * le_updateCtrl_Defer() will also prevent rollbacks unless a fault triggers the Supervisor to
 * restart the framework or reboot the target.  See below for more information on rollbacks.
 *
 *
 * @section le_updateCtrl_probation Controlling the Probation Period
 *
 * Whenever a new system is started, a probation period begins.  If the system stays running for
 * the entire probation period, the system is marked "good".  But, during the probation period,
 * if an app faults enough times in a short enough period for the Supervisor to give up on trying
 * to start it, the system update is marked "bad" and the system is rolled-back to the last known
 * "good" system.
 *
 * In some cases, a customer may want to extend the probation period of their new system for a
 * non-deterministic amount of time, until it has had a chance to perform some action in the
 * real world (such as successfully "phoning home" at least once).
 *
 * To support this, the functions le_updateCtrl_LockProbation() and le_updateCtrl_UnlockProbation()
 * are provided, where each call to "Lock" must be matched with a call to "Unlock".
 *
 * For those who want to mark "good" at some point of their own determination, regardless
 * of how long the system has been up, le_updateCtrl_MarkGood() is provided.
 *
 * And for those who want to mark the system "bad" and trigger a rollback at some point of
 * their own choosing, le_updateCtrl_FailProbation() is provided.
 *
 * Calls to any of the functions that affect the probation period are ignored if the probation
 * period is already over (if the system is already marked "good"). Once it is "good", it stays
 * "good".
 *
 * If someone tries to update the system while it is still in its probation period, the update
 * will be refused.  To override this, call le_updateCtrl_FailProbation() or
 * le_updateCtrl_MarkGood() to end the probation period before trying to apply the new
 * system update.
 *
 * @note If a reboot occurs during the probation period, the probation period will start over.
 * But, if the system reboots more than a few times without reaching the end of its probation
 * period first, then the system will be rolled-back to the last known "good" system.
 *
 * <HR>
 *
 * Copyright (C) Sierra Wireless Inc.
 */
/**
 * @file le_updateCtrl_interface.h
 *
 * Legato @ref c_updateCtrl include file.
 *
 * Copyright (C) Sierra Wireless Inc.
 */

#ifndef LE_UPDATECTRL_INTERFACE_H_INCLUDE_GUARD
#define LE_UPDATECTRL_INTERFACE_H_INCLUDE_GUARD


#include "legato.h"


//--------------------------------------------------------------------------------------------------
/**
 * Type for handler called when a server disconnects.
 */
//--------------------------------------------------------------------------------------------------
typedef void (*le_updateCtrl_DisconnectHandler_t)(void *);

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
void le_updateCtrl_ConnectService
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
le_result_t le_updateCtrl_TryConnectService
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
void le_updateCtrl_SetServerDisconnectHandler
(
    le_updateCtrl_DisconnectHandler_t disconnectHandler,
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
void le_updateCtrl_DisconnectService
(
    void
);


//--------------------------------------------------------------------------------------------------
/**
 * State of the system.
 */
//--------------------------------------------------------------------------------------------------
typedef enum
{
    LE_UPDATECTRL_SYSTEMSTATE_GOOD = 0,
        ///< "good"
    LE_UPDATECTRL_SYSTEMSTATE_BAD = 1,
        ///< "bad"
    LE_UPDATECTRL_SYSTEMSTATE_PROBATION = 2
        ///< "tried N" or untried.
}
le_updateCtrl_SystemState_t;


//--------------------------------------------------------------------------------------------------
/**
 * Prevent all updates (and roll-backs) until further notice.
 */
//--------------------------------------------------------------------------------------------------
void le_updateCtrl_Defer
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * Allow updates to go ahead.
 */
//--------------------------------------------------------------------------------------------------
void le_updateCtrl_Allow
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * Prevent the probation period from ending.
 *
 * @return false if lock failed (for example if not in a probation period).
 */
//--------------------------------------------------------------------------------------------------
bool le_updateCtrl_LockProbation
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * Cancels a call to LockProbation(), allow the probation period to end.
 *
 * @note Must match previous successful call to LockProbation().
 */
//--------------------------------------------------------------------------------------------------
void le_updateCtrl_UnlockProbation
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * Marks the system "good", ending the probation period.
 *
 * @return
 *      - LE_OK         The system was marked Good
 *      - LE_BUSY       Someone holds a probation lock
 *      - LE_DUPLICATE  Probation has expired - the system has already been marked
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_updateCtrl_MarkGood
(
    bool force
        ///< [IN] True to set system Good even if someone holds a probation lock
);

//--------------------------------------------------------------------------------------------------
/**
 * Marks the system "bad" and triggers a roll-back to a "good" system.
 *
 * @note Ignored if the probation period has already ended.  Also, the roll-back may be delayed if
 * someone is deferring updates using le_updateCtrl_Defer().
 */
//--------------------------------------------------------------------------------------------------
void le_updateCtrl_FailProbation
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * Get the current system state.
 *
 * @note Can only be called if updates have been deferred or if a probation lock is held.
 * Otherwise the system state could change between the time this function is called and when
 * the return value is checked.
 */
//--------------------------------------------------------------------------------------------------
le_updateCtrl_SystemState_t le_updateCtrl_GetSystemState
(
    void
);

#endif // LE_UPDATECTRL_INTERFACE_H_INCLUDE_GUARD