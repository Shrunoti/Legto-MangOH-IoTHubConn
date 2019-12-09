

/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */


#ifndef LE_DUALSYS_INTERFACE_H_INCLUDE_GUARD
#define LE_DUALSYS_INTERFACE_H_INCLUDE_GUARD


#include "legato.h"


//--------------------------------------------------------------------------------------------------
/**
 * Get the server service reference
 */
//--------------------------------------------------------------------------------------------------
le_msg_ServiceRef_t le_dualsys_GetServiceRef
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * Get the client session reference for the current message
 */
//--------------------------------------------------------------------------------------------------
le_msg_SessionRef_t le_dualsys_GetClientSessionRef
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * Initialize the server and advertise the service.
 */
//--------------------------------------------------------------------------------------------------
void le_dualsys_AdvertiseService
(
    void
);


//--------------------------------------------------------------------------------------------------
/**
 * System
 * Define on which system 1 or 2 belong the partition group (0 = system 1, 1 = system 2)
 */
//--------------------------------------------------------------------------------------------------
typedef enum
{
    LE_DUALSYS_MODEM_GROUP = 0x1,        ///< System 1 or 2 for MODEM sub-system
    LE_DUALSYS_LK_GROUP = 0x2,        ///< System 1 or 2 for LK sub-system
    LE_DUALSYS_LINUX_GROUP = 0x4        ///< System 1 or 2 for LINUX sub-system
}
le_dualsys_System_t;



//--------------------------------------------------------------------------------------------------
/**
 * Disable (true) or enable (false) the synchronisation check before performing an update.
 * The default behavior at startup is always to have the check enabled. It remains enabled
 * until this service is called with the value true. To re-enable the synchronization check
 * call this service with the value false.
 *
 * @note Upgrading some partitions without performing a sync before may let the whole system
 *       into a unworkable state. THIS IS THE RESPONSABILITY OF THE CALLER TO KNOW WHAT IMAGES
 *       ARE ALREADY FLASHED INTO THE UPDATE SYSTEM.
 *
 * @note Fuction must be called after each target reboot or updateDaemon restart
 *
 * @return
 *      - LE_OK              On success
 *      - LE_UNSUPPORTED     The feature is not supported
 *      - LE_FAULT           On failure
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_dualsys_DisableSyncBeforeUpdate
(
    bool isDisabled
        ///< [IN] State of sync check : true (disable) or false (enable)
);



//--------------------------------------------------------------------------------------------------
/**
 * Define a new "system" by setting the three sub-systems.  This system will become the current
 * system in use after the reset performed by this service, if no error are reported.
 *
 * @note On success, a device reboot is initiated without returning any value.
 *
 * @return
 *      - LE_FAULT           On failure
 *      - LE_UNSUPPORTED     The feature is not supported
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_dualsys_SetSystem
(
    le_dualsys_System_t systemMask
        ///< [IN] Sub-system bitmask for "modem/lk/linux" partitions
);



//--------------------------------------------------------------------------------------------------
/**
 * Get the current "system" in use.
 *
 * @return
 *      - LE_OK            On success
 *      - LE_FAULT         On failure
 *      - LE_UNSUPPORTED   The feature is not supported
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_dualsys_GetCurrentSystem
(
    le_dualsys_System_t* systemMaskPtr
        ///< [OUT] Sub-system bitmask for "modem/lk/linux" partitions
);


#endif // LE_DUALSYS_INTERFACE_H_INCLUDE_GUARD