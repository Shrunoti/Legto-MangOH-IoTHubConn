

/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */

/**
 * @page c_le_instStat Install Status API
 *
 * @ref le_instStat_interface.h "API Reference"
 *
 * This API provides notifications when apps are installed and uninstalled.
 *
 * <HR>
 *
 * Copyright (C) Sierra Wireless Inc.
 */
/**
 * @file le_instStat_interface.h
 *
 * Legato @ref c_le_instStat include file.
 *
 * Copyright (C) Sierra Wireless Inc.
 */

#ifndef LE_INSTSTAT_INTERFACE_H_INCLUDE_GUARD
#define LE_INSTSTAT_INTERFACE_H_INCLUDE_GUARD


#include "legato.h"

// Interface specific includes
#include "le_limit_interface.h"


//--------------------------------------------------------------------------------------------------
/**
 * Type for handler called when a server disconnects.
 */
//--------------------------------------------------------------------------------------------------
typedef void (*le_instStat_DisconnectHandler_t)(void *);

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
void le_instStat_ConnectService
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
le_result_t le_instStat_TryConnectService
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
void le_instStat_SetServerDisconnectHandler
(
    le_instStat_DisconnectHandler_t disconnectHandler,
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
void le_instStat_DisconnectService
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