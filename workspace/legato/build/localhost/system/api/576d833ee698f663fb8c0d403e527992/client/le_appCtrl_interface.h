

/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */

/**
 * @page c_appCtrl App Control API
 *
 * @ref le_appCtrl_interface.h "API Reference"
 *
 * API for controlling (starting, stopping, and debugging) apps.
 *
 * @warning This API should only be made available to privileged users because it allows
 *          denial of service and arbitrary program execution in other apps.  It also gives the
 *          user the ability to grant themselves access to any file, socket or device on the
 *          target system.
 *
 * @section le_appCtrlApi_binding Binding
 *
 * You can use a definition @c .adef file to bind your client-side app and component to the
 * server-side App Control service.
 *
 * This code sample shows how to bind an app to this service:
 * @code
 * bindings
 * {
 *     myExe.myComp.le_appCtrl -> <root>.le_appCtrl
 * }
 * @endcode
 *
 * See @ref defFilesAdef for details.
 *
 *
 * @section le_appCtrlApi_start Start App
 *
 * Use le_appCtrl_Start() (const char * appName) to start an app.
 *
 * The code sample shows how to use the App Control service to start an app:
 *
 * @code
 *  le_result_t result = le_appCtrl_Start("myApp");
 * @endcode
 *
 * where @c myApp is the name of the app.
 *
 *
 * @section le_appCtrlApi_stop Stop App
 *
 * Use le_appCtrl_Stop() to stop an app.
 *
 * This code sample shows how to use the App Control service to stop an app.
 *
 * @code
 *  le_result_t result = le_appCtrl_Stop("myApp");
 * @endcode
 *
 * where @c myApp is the name of the app.
 *
 *
 * @section le_appCtrlApi_debug Debugging Features
 *
 * Several functions are provided to support the construction of tools for debugging apps.
 * These functions are used by the @c appCtrl and @ref toolsTarget_sbtrace tools. See the
 * implementations of those tools (under <c>framework/tools/target/linux</c>) for real-world
 * examples of how these functions can be used.
 *
 * - le_appCtrl_GetRef() can be used to get a reference to an app.  This reference can then be used
 *   to apply overrides to the way that app will be started when le_appCtrl_Start() is called.
 *
 * - le_appCtrl_SetRun() can be used to set or clear the "run" flag for a given process in the app.
 *   Clearing the "run" flag will result in that process not being started when the app starts.
 *   This can be used to start an app without one or more processes that are to be started later
 *   using strace or gdbserver, for example.
 *
 * - le_appCtrl_Import() can be used to import (into an app's container) something from the
 *   file system outside of the app's container.  This is used by the @ref toolsHost_sbhelper
 *   and @ref toolsTarget_sbtrace interactive sandbox configuration helper tools.
 *
 * - le_appCtrl_SetDevicePerm() is used to set the permissions of a device file.  This is also
 *   used by the @ref toolsHost_sbhelper and @ref toolsTarget_sbtrace interactive sandbox
 *   configuration helper tools.
 *
 * - le_appCtrl_AddTraceAttachHandler() is used to register a call-back handler function to be
 *   called whenever the Supervisor has created a process in the app. After calling this call-back,
 *   the Supervisor will wait until le_appCtrl_TraceUnblock() is called for that process before
 *   allowing it to run.  This gives a debugger the opportunity to attach to the process before
 *   running it.
 *
 * - le_appCtrl_ReleaseRef() releases the reference returned by le_appCtrl_GetRef() and resets all
 *   the app control overrides that were set using that reference.
 *
 * @subsection le_appCtrlApi_debug_syncProcStart Synchronizing Process Start
 *
 * Some tools need to "attach" to a process (e.g., using the Unix ptrace() API) when the process
 * starts. If the process is started by the Supervisor when the app starts, then synchoronization
 * can be achieved by calling le_appCtrl_AddTraceAttachHandler() to register a
 * "Trace Attach Handler" call-back for the app before starting it. When the Supervisor starts an
 * app that has a Trace Attach Handler registered for it, the Supervisor will prepare the app
 * container, as usual, and for each app process it starts, it will <c>fork()</c> a child process,
 * put it into its runtime container, call the Trace Attach Handler function and block the child
 * process before <c>exec()</c>ing the app's program.  When the Trace Attach Handler function
 * is called, it is passed the PID of the child process, which the debug tool can use to attach
 * to the process before calling le_appCtrl_TraceUnblock() to ask the Supervisor to unblock the
 * app's process.
 *
 * @code
 * {
 *     ...
 *
 *     // Get a reference to the app.
 *     le_appCtrl_AppRef_t appRef = le_appCtrl_GetRef(appNameStr);
 *     if (appRef == NULL)
 *     {
 *         fprintf(stderr, "App '%s' could not be started. Check logs for more info.\n", appNameStr);
 *         exit(EXIT_FAILURE);
 *     }
 *
 *     // Set an attach handler.
 *     le_appCtrl_AddTraceAttachHandler(appRef, AttachHandler, NULL);
 *
 *     // Start the app.
 *     le_result_t result = le_appCtrl_Start(appNameStr);
 *     if (result != LE_OK)
 *     {
 *         fprintf(stderr, "App '%s' could not be started. Check logs for more info.\n", appNameStr);
 *         exit(EXIT_FAILURE);
 *     }
 * }
 *
 * static void AttachHandler
 * (
 *     le_appCtrl_AppRef_t appRef,         ///< [IN] App reference.
 *     int32_t pid,                        ///< [IN] PID of the process to attach to.
 *     const char* procNamePtr,            ///< [IN] Name of the process.
 *     void* contextPtr                    ///< [IN] Not used.
 * )
 * {
 *     // Attach to the process.
 *     ...
 *
 *     // Ask the supervisor to unblock the process.
 *     le_appCtrl_TraceUnblock(appRef, pid);
 * }
 *
 * @endcode
 *
 * @subsection le_appCtrlApi_debug_suppressProcStart Supressing Start of a Process
 *
 * It's possible to ask the Supervisor to start an app without starting one or more of the
 * app's processes. This is done by calling le_appCtrl_SetRun() to set the "Run" flag to false
 * for the process before calling le_appCtrl_Start() to start the app.
 *
 * @code
 * {
 *     ...
 *
 *     // Get a reference to the app.
 *     le_appCtrl_AppRef_t appRef = le_appCtrl_GetRef(appNameStr);
 *     if (appRef == NULL)
 *     {
 *         fprintf(stderr, "App '%s' could not be started. Check logs for more info.\n", appNameStr);
 *         exit(EXIT_FAILURE);
 *     }
 *
 *     le_appCtrl_SetRun(appRef, procNameStr, false);
 *
 *     le_appCtrl_Start(appNameStr);
 * }
 * @endcode
 *
 * @subsection le_appCtrlApi_debug_importIntoContainer Importing Files Into App Containers
 *
 * le_appCtrl_Import() can be used to import things from the real root file system into an
 * app's run-time container.  le_appCtrl_SetDevicePerm() can be used to set the access permissions
 * for a device file such that the app can access it.
 *
 * @code
 * le_result_t r = le_appCtrl_Import(appRef, path);
 *
 * if (r != LE_OK)
 * {
 *     fprintf(stderr, "Could not import file '%s'. %s.\n", path, LE_RESULT_TXT(r));
 * }
 * else if (IsDevice(path))
 * {
 *     // Give the app read access to the device file.
 *     if (le_appCtrl_SetDevicePerm(appRef, path, "r") != LE_OK)
 *     {
 *         fprintf(stderr, "Could not set permissions to %s for '%s'.\n", permStr, path);
 *     }
 * }
 * @endcode
 *
 * <HR>
 *
 * Copyright (C) Sierra Wireless Inc.
 */
/**
 * @file le_appCtrl_interface.h
 *
 * Legato @ref c_appCtrl include file.
 *
 * Copyright (C) Sierra Wireless Inc.
 */

#ifndef LE_APPCTRL_INTERFACE_H_INCLUDE_GUARD
#define LE_APPCTRL_INTERFACE_H_INCLUDE_GUARD


#include "legato.h"

// Interface specific includes
#include "le_limit_interface.h"


//--------------------------------------------------------------------------------------------------
/**
 * Type for handler called when a server disconnects.
 */
//--------------------------------------------------------------------------------------------------
typedef void (*le_appCtrl_DisconnectHandler_t)(void *);

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
void le_appCtrl_ConnectService
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
le_result_t le_appCtrl_TryConnectService
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
void le_appCtrl_SetServerDisconnectHandler
(
    le_appCtrl_DisconnectHandler_t disconnectHandler,
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
void le_appCtrl_DisconnectService
(
    void
);


//--------------------------------------------------------------------------------------------------
/**
 */
//--------------------------------------------------------------------------------------------------
typedef struct le_appCtrl_App* le_appCtrl_AppRef_t;


//--------------------------------------------------------------------------------------------------
/**
 * Reference type used by Add/Remove functions for EVENT 'le_appCtrl_TraceAttach'
 */
//--------------------------------------------------------------------------------------------------
typedef struct le_appCtrl_TraceAttachHandler* le_appCtrl_TraceAttachHandlerRef_t;


//--------------------------------------------------------------------------------------------------
/**
 * Handler for attaching to a process that is to be traced.  The process is blocked allowing the
 * tracer to attach to it.  The tracer must call TraceUnblock() to unblock the traced process.
 */
//--------------------------------------------------------------------------------------------------
typedef void (*le_appCtrl_TraceAttachHandlerFunc_t)
(
    le_appCtrl_AppRef_t appRef,
        ///< Ref to the app.
    int32_t pid,
        ///< PID of a process that can be attached to.
    const char* LE_NONNULL procName,
        ///< Name of the process name.
    void* contextPtr
        ///<
);

//--------------------------------------------------------------------------------------------------
/**
 * Gets a reference to an app.
 *
 * @return
 *      Reference to the named app.
 *      NULL on error (check logs for errors).
 */
//--------------------------------------------------------------------------------------------------
le_appCtrl_AppRef_t le_appCtrl_GetRef
(
    const char* LE_NONNULL appName
        ///< [IN] Name of the app to get the ref for.
);

//--------------------------------------------------------------------------------------------------
/**
 * Release the reference to an app, resetting all overrides set for this app using other
 * functions, like SetRun(), SetDevicePerm(), etc.
 */
//--------------------------------------------------------------------------------------------------
void le_appCtrl_ReleaseRef
(
    le_appCtrl_AppRef_t appRef
        ///< [IN] Ref to the app.
);

//--------------------------------------------------------------------------------------------------
/**
 * Sets the run flag for a process in an app.
 *
 * If there is an error this function will kill the calling client.
 */
//--------------------------------------------------------------------------------------------------
void le_appCtrl_SetRun
(
    le_appCtrl_AppRef_t appRef,
        ///< [IN] Ref to the app.
    const char* LE_NONNULL procName,
        ///< [IN] Process name to set the run flag for.
    bool run
        ///< [IN] Flag to run the process or not.
);

//--------------------------------------------------------------------------------------------------
/**
 * Imports a file into the app's working directory.
 *
 * @return
 *      LE_OK if successfully imported the file.
 *      LE_DUPLICATE if the path conflicts with items already in the app's working directory.
 *      LE_NOT_FOUND if the path does not point to a valid file.
 *      LE_BAD_PARAMETER if the path is formatted incorrectly.
 *      LE_FAULT if there was some other error.
 *
 * @note If the caller is passing an invalid reference to the app, it is a fatal error,
 *       the function will not return.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_appCtrl_Import
(
    le_appCtrl_AppRef_t appRef,
        ///< [IN] Ref to the app.
    const char* LE_NONNULL path
        ///< [IN] Absolute path to the file to import.
);

//--------------------------------------------------------------------------------------------------
/**
 * Sets a device file's permissions.
 *
 * @return
 *      LE_OK if successfully set the device's permissions.
 *      LE_NOT_FOUND if the path does not point to a valid device.
 *      LE_BAD_PARAMETER if the path is formatted incorrectly.
 *      LE_FAULT if there was some other error.
 *
 * @note If the caller is passing an invalid reference to the app, it is a fatal error,
 *       the function will not return.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_appCtrl_SetDevicePerm
(
    le_appCtrl_AppRef_t appRef,
        ///< [IN] Ref to the app.
    const char* LE_NONNULL path,
        ///< [IN] Absolute path to the device.
    const char* LE_NONNULL permissions
        ///< [IN] Permission string, "r", "w", "rw".
);

//--------------------------------------------------------------------------------------------------
/**
 * Add handler function for EVENT 'le_appCtrl_TraceAttach'
 *
 * Event that indicates the process is blocked and can be attached to.
 */
//--------------------------------------------------------------------------------------------------
le_appCtrl_TraceAttachHandlerRef_t le_appCtrl_AddTraceAttachHandler
(
    le_appCtrl_AppRef_t appRef,
        ///< [IN] Ref to the app.
    le_appCtrl_TraceAttachHandlerFunc_t attachToPidPtr,
        ///< [IN] Attach handler to register.
    void* contextPtr
        ///< [IN]
);

//--------------------------------------------------------------------------------------------------
/**
 * Remove handler function for EVENT 'le_appCtrl_TraceAttach'
 */
//--------------------------------------------------------------------------------------------------
void le_appCtrl_RemoveTraceAttachHandler
(
    le_appCtrl_TraceAttachHandlerRef_t handlerRef
        ///< [IN]
);

//--------------------------------------------------------------------------------------------------
/**
 * Unblocks the traced process.  This should normally be done once the tracer has successfully
 * attached to the process.
 *
 * @note If the caller is passing an invalid reference to the app, it is a fatal error,
 *       the function will not return.
 */
//--------------------------------------------------------------------------------------------------
void le_appCtrl_TraceUnblock
(
    le_appCtrl_AppRef_t appRef,
        ///< [IN] Ref to the app.
    int32_t pid
        ///< [IN] PID of the process to unblock.
);

//--------------------------------------------------------------------------------------------------
/**
 * Starts an app.
 *
 * @return
 *      LE_OK if the app is successfully started.
 *      LE_DUPLICATE if the app is already running.
 *      LE_NOT_FOUND if the app isn't installed.
 *      LE_FAULT if there was an error and the app could not be launched.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_appCtrl_Start
(
    const char* LE_NONNULL appName
        ///< [IN] Name of the app to start.
);

//--------------------------------------------------------------------------------------------------
/**
 * Stops an app.
 *
 * @return
 *      LE_OK if successful.
 *      LE_NOT_FOUND if the app could not be found.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_appCtrl_Stop
(
    const char* LE_NONNULL appName
        ///< [IN] Name of the app to stop.
);

#endif // LE_APPCTRL_INTERFACE_H_INCLUDE_GUARD