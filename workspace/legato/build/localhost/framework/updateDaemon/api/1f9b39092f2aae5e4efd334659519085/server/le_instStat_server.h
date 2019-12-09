

/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */


#ifndef LE_INSTSTAT_INTERFACE_H_INCLUDE_GUARD
#define LE_INSTSTAT_INTERFACE_H_INCLUDE_GUARD


#include "legato.h"

// Interface specific includes
#include "le_limit_server.h"


//--------------------------------------------------------------------------------------------------
/**
 * Get the server service reference
 */
//--------------------------------------------------------------------------------------------------
le_msg_ServiceRef_t le_instStat_GetServiceRef
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * Get the client session reference for the current message
 */
//--------------------------------------------------------------------------------------------------
le_msg_SessionRef_t le_instStat_GetClientSessionRef
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * Initialize the server and advertise the service.
 */
//--------------------------------------------------------------------------------------------------
void le_instStat_AdvertiseService
(
    void
);


//--------------------------------------------------------------------------------------------------
/**
 * Reference type used by Add/Remove functions for EVENT 'le_instStat_AppInstallEvent'
 */
//--------------------------------------------------------------------------------------------------
typedef struct le_instStat_AppInstallEventHandler* le_instStat_AppInstallEventHandlerRef_t;


//--------------------------------------------------------------------------------------------------
/**
 * Reference type used by Add/Remove functions for EVENT 'le_instStat_AppUninstallEvent'
 */
//--------------------------------------------------------------------------------------------------
typedef struct le_instStat_AppUninstallEventHandler* le_instStat_AppUninstallEventHandlerRef_t;


//--------------------------------------------------------------------------------------------------
/**
 * Handler for application install.  Called when an application is installed.
 */
//--------------------------------------------------------------------------------------------------
typedef void (*le_instStat_AppInstallHandlerFunc_t)
(
    const char* LE_NONNULL appName,
        ///< Name of the application installed.
    void* contextPtr
        ///<
);

//--------------------------------------------------------------------------------------------------
/**
 * Handler for application uninstall.  Called when an application is uninstalled.
 */
//--------------------------------------------------------------------------------------------------
typedef void (*le_instStat_AppUninstallHandlerFunc_t)
(
    const char* LE_NONNULL appName,
        ///< Name of the application uninstalled.
    void* contextPtr
        ///<
);


//--------------------------------------------------------------------------------------------------
/**
 * Add handler function for EVENT 'le_instStat_AppInstallEvent'
 *
 * This event provides a notification of when an application is installed.
 */
//--------------------------------------------------------------------------------------------------
le_instStat_AppInstallEventHandlerRef_t le_instStat_AddAppInstallEventHandler
(
    le_instStat_AppInstallHandlerFunc_t handlerPtr,
        ///< [IN]
    void* contextPtr
        ///< [IN]
);



//--------------------------------------------------------------------------------------------------
/**
 * Remove handler function for EVENT 'le_instStat_AppInstallEvent'
 */
//--------------------------------------------------------------------------------------------------
void le_instStat_RemoveAppInstallEventHandler
(
    le_instStat_AppInstallEventHandlerRef_t handlerRef
        ///< [IN]
);



//--------------------------------------------------------------------------------------------------
/**
 * Add handler function for EVENT 'le_instStat_AppUninstallEvent'
 *
 * This event provides a notification of when an application is uninstalled.
 */
//--------------------------------------------------------------------------------------------------
le_instStat_AppUninstallEventHandlerRef_t le_instStat_AddAppUninstallEventHandler
(
    le_instStat_AppUninstallHandlerFunc_t handlerPtr,
        ///< [IN]
    void* contextPtr
        ///< [IN]
);



//--------------------------------------------------------------------------------------------------
/**
 * Remove handler function for EVENT 'le_instStat_AppUninstallEvent'
 */
//--------------------------------------------------------------------------------------------------
void le_instStat_RemoveAppUninstallEventHandler
(
    le_instStat_AppUninstallEventHandlerRef_t handlerRef
        ///< [IN]
);


#endif // LE_INSTSTAT_INTERFACE_H_INCLUDE_GUARD