

/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */

/**
 * @page c_le_dualsys Dual Systems Update API
 *
 * @ref le_dualsys_interface.h "API Reference"
 *
 *
 * Dual systems are systems composed with two set of partitions: one is said "ACTIVE", so currently
 * in use, and the other said "UPDATE", so not used but ready-to-use.
 *
 * On dual systems, when Legato updates an image through the le_fwupdate API, it works on the
 * UPDATE system.
 * And then an "install" command performs a switch from UPDATE to ACTIVE after a reboot:
 * The new ACTIVE system is used. The UPDATE partition becomes ready for a new update or for
 * roll-back if the new ACTIVE fails to start properly.
 *
 * In each set, three sub-systems are defined:
 *     - The "modem" group for all binaries and filesystem related to modem core,
 *     - The "lk" group for the LK bootloader,
 *     - The "linux" group the kernel, rootfs and legato partitions.
 *
 * @section le_dualsys_binding IPC interfaces binding
 *
 * All the functions of this API are provided by the @b le_dualsys service.
 *
 * Here's a code sample binding to the le_dualsys service:
 * @verbatim
   bindings:
   {
      clientExe.clientComponent.le_dualsys -> fwupdateService.le_dualsys
   }
   @endverbatim
 *
 *
 * @warning All of these APIs are only available for all dual systems platforms. Please refer
 * to the Product Technical Specification document of your platform for further details.
 * Please refer to @subpage platformConstraintsFwUpdate for details.
 *
 * @section le_dualsys_DisableSyncBeforeUpdate Disable the check of sync before updating
 *
 * It is possible to force the update of any partition even if the systems are not synchronized.
 * This is done by calling le_fwupdate_DisableSyncBeforeUpdate() with the value true. If the value
 * is false, the synchronization will be mandatory before performing an update.
 * Note that the synchronization is automatically done by calling the function
 * le_fwupdate_InitDownload() when the synchronization check is enabled.
 *
 * @section le_dualsys_system Defining a System
 *
 * It is possible to define a system by calling le_dualsys_SetSystem().  This function takes
 * three values, one for the modem sub-system, one for the lk sub-system and one for the linux
 * sub-system.  The values are one of 0 for system 1 or 1 for system 2.  When set, the system will
 * be usable after the next reset.
 * If no error is encountered, this function will perform a reset.
 *
 * It is possible the get the last defined system by calling the le_dualsys_GetSystem().  This
 * function returns the values for the three sub-systems.  This system is updated accordingly
 * the last system set by le_dualsys_SetSystem().
 *
 * It is possible to get the current active system by calling le_dualsys_GetCurrentSystem().
 * This function returns the values for the three sub-systems.  If le_dualsys_SetSystem() is
 * called before le_dualsys_GetCurrentSystem(), the returned values may differ as they represent
 * the current system in use.
 *
 * <HR>
 *
 * Copyright (C) Sierra Wireless Inc.
 */
/**
 * @file le_dualsys_interface.h
 *
 * Legato @ref c_le_dualsys include file.
 *
 * Copyright (C) Sierra Wireless Inc.
 */

#ifndef LE_DUALSYS_INTERFACE_H_INCLUDE_GUARD
#define LE_DUALSYS_INTERFACE_H_INCLUDE_GUARD


#include "legato.h"


//--------------------------------------------------------------------------------------------------
/**
 * Type for handler called when a server disconnects.
 */
//--------------------------------------------------------------------------------------------------
typedef void (*le_dualsys_DisconnectHandler_t)(void *);

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
void le_dualsys_ConnectService
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
le_result_t le_dualsys_TryConnectService
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
void le_dualsys_SetServerDisconnectHandler
(
    le_dualsys_DisconnectHandler_t disconnectHandler,
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
void le_dualsys_DisconnectService
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