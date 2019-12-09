

/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */


#ifndef LE_UPDATECTRL_INTERFACE_H_INCLUDE_GUARD
#define LE_UPDATECTRL_INTERFACE_H_INCLUDE_GUARD


#include "legato.h"


//--------------------------------------------------------------------------------------------------
/**
 * Get the server service reference
 */
//--------------------------------------------------------------------------------------------------
le_msg_ServiceRef_t le_updateCtrl_GetServiceRef
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * Get the client session reference for the current message
 */
//--------------------------------------------------------------------------------------------------
le_msg_SessionRef_t le_updateCtrl_GetClientSessionRef
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * Initialize the server and advertise the service.
 */
//--------------------------------------------------------------------------------------------------
void le_updateCtrl_AdvertiseService
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