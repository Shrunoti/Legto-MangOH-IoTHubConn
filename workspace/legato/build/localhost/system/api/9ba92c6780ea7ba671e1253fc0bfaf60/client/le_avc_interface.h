

/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */

/**
 * @page c_le_avc_legacy AirVantage Connector
 *
 * @deprecated This API should not be used for new applications and will be removed in a future
 * version of Legato. It has been replaced by @ref c_le_avc .
 *
 * @ref le_avc_interface.h "API Reference" <br>
 * @ref howToAVConnect "How To Connect"
 *
 * <HR>
 *
 * The AirVantage connector service provides an API to communicate with the AirVantage server.
 *
 * @section c_le_avc_update Firmware/Application Update
 *
 * The API for firmware/application update is divided into two parts:
 *  - Allow a control app to contact the server for pending updates. If available,
 *    can select to download or install the update.
 *    <br>
 *  - Allow an app to block an update from being installed. An app may need this if it's
 *    performing a critical operation that can't be interrupted (e.g., eCall). This is necessary
 *    as installing an update will cause the app to restart (either the app itself needs to restart
 *    or a firmware change causes the modem to reset).
 *
 * @subsection c_le_avc_update_control Update Control
 *
 * A control app can start a session with an AirVantage server to determine if there
 * is a pending update. This is done with le_avc_StartSession(). The current session can be
 * stopped using le_avc_StopSession().  This cancels any query for pending update, or suspends a
 * download in progress; however, it <b>won't stop</b> an install in progress.
 *
 * The status of a pending update is sent to the control app using a notification.
 * The control app can use le_avc_AddStatusEventHandler() to register a handler function to
 * receive this notification. The notification will be received after a session is started using
 * le_avc_StartSession(). Sometimes, the modem firmware may decide to start a session with
 * the AirVantage server. In this case, the notification could also be received, even if the
 * control app has not explicitly called le_avc_StartSession().
 *
 * There can only be one pending update.  To query the type of update, use le_avc_GetUpdateType().
 * If it's an app update, then le_avc_GetAppUpdateName() can be used to get the name of
 * the app.
 *
 * If a download is pending, le_avc_AcceptDownload() can be used to allow the update to be
 * downloaded. An AirVantage session will be initiated if this api is called while there is no
 * active AirVantage session. le_avc_DeferDownload() can be used to defer the download for the
 * specified number of minutes. After the defer time has elapsed, the pending download notification
 * will be re-sent again to all registered apps. le_avc_AcceptDownload() can be used to accept the
 * download even before the defer timer expires. This behaviour is true for deferring install
 * and uninstall as well.
 *
 * Once an update has been downloaded, a new notification will be received to indicate that an
 * install is pending. The control app can then use le_avc_AcceptInstall() to allow the
 * install to proceed. Note that even if the control app calls le_avc_AcceptInstall(),
 * the install may still be blocked by an app using the @ref c_le_avc_update_app
 * functions. To defer the decision, the control app can use le_avc_DeferInstall() to
 * defer the install for the specified number of minutes. After the defer time has elapsed, the
 * pending install notification will be re-sent to allow the control app to make a new
 * decision, or again defer.
 *
 * If an uninstall is pending, then le_avc_AcceptUninstall() can be used to allow the uninstall to
 * proceed. To defer the decision, le_avc_DeferUninstall() can be used to defer the uninstall for
 * the specified number of minutes. In case of an upgrade, the existing application will not be
 * uninstalled after le_avc_AcceptUninstall() is called. le_avc_AcceptUninstall() is only used to
 * signal the server to start downloading the new application. During an upgrade process the
 * control app has to accept the uninstall of the existing version followed by accepting the
 * download and install of the new version.
 *
 * Accepting an app install or uninstall will not initiate an AirVantage session if no session
 * is active.  The control app should start an AirVantage session before accepting an app
 * install/uninstall, to ensure the process is completed, and the server is updated.
 *
 * If a control app doesn't register for notifications using le_avc_AddStatusEventHandler(),
 * then any pending downloads and installs will happen automatically, subject to any restrictions
 * imposed by app using the @ref c_le_avc_update_app functions.  Also, only the control app
 * registered for notifications will be allowed to use the other update control API functions.
 *
 * Only one control app is allowed to register for notifications.  Any subsequent attempts by
 * either the same app, or a different app is treated as a fatal error (i.e. non-recoverable)
 * and will result in the client app being terminated.
 *
 * In case of any error incurred during app download/install, an error code will be set and the
 * control app can retrieve it by calling le_avc_GetErrorCode().
 *
 * @subsection c_le_avc_update_app Application Blocking
 *
 * When an app is about to perform a critical operation, it can block the installation of
 * an update with le_avc_BlockInstall(), and after it's finished with the critical operation, it
 * can unblock the install with le_avc_UnblockInstall().
 *
 * What constitutes a critical operation depends on the app.  An eCall app might
 * block installs for the duration that it runs.  A data collection app that wakes up
 * once an hour might block installs while it collects and stores and/or transmits a new data
 * sample, and then unblock installs just before it goes to sleep again.
 *
 * If an install can't be applied because it's blocked, another attempt to apply the install
 * will be made at a later time.
 *
 * The control app can add a session controller handler using le_avc_AddSessionRequestEventHandler().
 * If the control app registers a session request event handler, requests by user apps to open or
 * close session will be forwarded to the control app. If the control app doesn't register a session
 * request handler, avms session can be opened anytime by user apps.
 *
 * @section c_le_avc_GetPollingTimer Polling Timer
 *
 * The embedded module will periodically initiate a connection to the Device Services server
 * according to the polling timer. The device initates the connection periodically, so that it can
 * poll the server if there are any pending jobs. Writing 0 to the polling timer disables polling
 * mode. Polling timer values range from 0 to 525600 minutes. The polling timer value is persistent.
 * le_avc_GetPollingTimer() reads the polling timer and le_avc_SetPollingTimer() writes the polling
 * timer.
 *
 * @section c_le_avc_GetRetryTimers Retry Timers
 *
 * If an error occurs during a connection to the Device Services server (WWAN DATA establishment
 * failed, http error code received), the embedded module will initiate a new connection according
 * to the values defined in the retry timers. The timers are tried in sequence until a connection is
 * established, or all enabled retry timers are exhausted. After all the enabled timers are
 * exhausted, a connection will be initiated only on a command from user (le_avc_startSession() )
 * or at the expiry of the polling timer. The retry timer values are persistent. Writing 0
 * to one of the retry timers will disable it. Retry timer values range from 0 to 20160 minutes.
 * The api le_avc_GetRetryTimers() reads the polling timers in an array and the api
 * le_avc_SetRetryTimers() writes the polling timers. When writing to the retry timers, values of
 * all the 8 timers have to be defined.
 *
 * @section le_avcService_configdb Service Configuration Tree
 * @copydoc le_avcService_configdbPage_Hide
 *
 * <HR>
 *
 * Copyright (C) Sierra Wireless Inc.
 */
/**
 * @interface le_avcService_configdbPage_Hide
 *
 * The configuration database path for the modemActivityTimeout is:
 * @verbatim
   /
       apps/
           avcService/
                    modemActivityTimeout
   @endverbatim
 *
 *
 * After an AirVantage session is started, if there's no activity from the modem within the timer
 * interval,then LE_AVC_NO_UPDATE state will be returned to the app. However, this modem activity
 * timeout can be overridden by setting an integer value at /apps/avcService/modemActivityTimeout.
 * The modem activity timer is initialized only when the avcService starts. If a valid entry >0 is
 * found,then it will be used instead of the default value of 20 seconds. The following steps should
 * be used to set the modemActivtyTimeout.
 *
 *
 * @verbatim
   config set /apps/avcService/modemActivityTimeout xx int
   app restart avcService
   @endverbatim
 *
 * @note
 * Everytime a new value is written to modemActivityTimeout, the avcService needs to be
 * restarted to read the new value.
 *
 *
 */
/**
 * @file le_avc_interface.h
 *
 * Legato @ref c_le_avc include file.
 *
 * Copyright (C) Sierra Wireless Inc.
 */

#ifndef LE_AVC_INTERFACE_H_INCLUDE_GUARD
#define LE_AVC_INTERFACE_H_INCLUDE_GUARD


#include "legato.h"

// Interface specific includes
#include "le_limit_interface.h"


//--------------------------------------------------------------------------------------------------
/**
 * Type for handler called when a server disconnects.
 */
//--------------------------------------------------------------------------------------------------
typedef void (*le_avc_DisconnectHandler_t)(void *);

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
void le_avc_ConnectService
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
le_result_t le_avc_TryConnectService
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
void le_avc_SetServerDisconnectHandler
(
    le_avc_DisconnectHandler_t disconnectHandler,
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
void le_avc_DisconnectService
(
    void
);


//--------------------------------------------------------------------------------------------------
/**
 * Maximum APN name length without NULL terminator.
 */
//--------------------------------------------------------------------------------------------------
#define LE_AVC_APN_NAME_MAX_LEN 48

//--------------------------------------------------------------------------------------------------
/**
 * Maximum APN name length including NULL terminator.
 */
//--------------------------------------------------------------------------------------------------
#define LE_AVC_APN_NAME_MAX_LEN_BYTES 49

//--------------------------------------------------------------------------------------------------
/**
 * Maximum user name length without NULL terminator.
 */
//--------------------------------------------------------------------------------------------------
#define LE_AVC_USERNAME_MAX_LEN 28

//--------------------------------------------------------------------------------------------------
/**
 * Maximum user name length including NULL terminator.
 */
//--------------------------------------------------------------------------------------------------
#define LE_AVC_USERNAME_MAX_LEN_BYTES 29

//--------------------------------------------------------------------------------------------------
/**
 * Maximum password length without NULL terminator..
 */
//--------------------------------------------------------------------------------------------------
#define LE_AVC_PASSWORD_MAX_LEN 28

//--------------------------------------------------------------------------------------------------
/**
 * Maximum password length including NULL terminator.
 */
//--------------------------------------------------------------------------------------------------
#define LE_AVC_PASSWORD_MAX_LEN_BYTES 29

//--------------------------------------------------------------------------------------------------
/**
 * Maximum number of retry timers.
 */
//--------------------------------------------------------------------------------------------------
#define LE_AVC_NUM_RETRY_TIMERS 8

//--------------------------------------------------------------------------------------------------
/**
 * Default HTTP status.
 */
//--------------------------------------------------------------------------------------------------
#define LE_AVC_HTTP_STATUS_INVALID 65535

//--------------------------------------------------------------------------------------------------
/**
 * Status of session or update
 *
 * If an update is pending, it must first be downloaded and then installed.
 */
//--------------------------------------------------------------------------------------------------
typedef enum
{
    LE_AVC_NO_UPDATE = 0,
        ///< No updates pending
    LE_AVC_DOWNLOAD_PENDING = 1,
        ///< Update pending download
    LE_AVC_DOWNLOAD_IN_PROGRESS = 2,
        ///< Download in progress
    LE_AVC_DOWNLOAD_COMPLETE = 3,
        ///< Download has completed
    LE_AVC_DOWNLOAD_FAILED = 4,
        ///< An error occurred downloading the update
    LE_AVC_INSTALL_PENDING = 5,
        ///< Install is pending (implies download complete)
    LE_AVC_INSTALL_IN_PROGRESS = 6,
        ///< Install in progress
    LE_AVC_INSTALL_COMPLETE = 7,
        ///< Update has been successfully installed
    LE_AVC_INSTALL_FAILED = 8,
        ///< An error occurred installing the update
    LE_AVC_UNINSTALL_PENDING = 9,
        ///< Uninstall is pending
    LE_AVC_UNINSTALL_IN_PROGRESS = 10,
        ///< Uninstall in progress
    LE_AVC_UNINSTALL_COMPLETE = 11,
        ///< App has been successfully uninstalled
    LE_AVC_UNINSTALL_FAILED = 12,
        ///< An error occurred uninstalling the update
    LE_AVC_SESSION_STARTED = 13,
        ///< Session with AV server started
    LE_AVC_SESSION_STOPPED = 14
        ///< Session with AV server stopped
}
le_avc_Status_t;


//--------------------------------------------------------------------------------------------------
/**
 * Request to open or close avms session.
 */
//--------------------------------------------------------------------------------------------------
typedef enum
{
    LE_AVC_SESSION_ACQUIRE = 0,
        ///< Request by user app to open AV session
    LE_AVC_SESSION_RELEASE = 1
        ///< Request by user app to close AV session
}
le_avc_SessionRequest_t;


//--------------------------------------------------------------------------------------------------
/**
 * The type of pending update
 */
//--------------------------------------------------------------------------------------------------
typedef enum
{
    LE_AVC_UNKNOWN_UPDATE = 0,
        ///<
    LE_AVC_FIRMWARE_UPDATE = 1,
        ///<
    LE_AVC_FRAMEWORK_UPDATE = 2,
        ///<
    LE_AVC_APPLICATION_UPDATE = 3
        ///<
}
le_avc_UpdateType_t;


//--------------------------------------------------------------------------------------------------
/**
 * Error code used to provide diagnostic information after a failure (includes both download and
 * install failure).
 *
 * @note
 *     Additional information may also be available in the target device's system log.
 */
//--------------------------------------------------------------------------------------------------
typedef enum
{
    LE_AVC_ERR_NONE = 0,
        ///< No error.
    LE_AVC_ERR_BAD_PACKAGE = 1,
        ///< Encountered a bad package.
    LE_AVC_ERR_INTERNAL = 2,
        ///< Something failed while doing install/download.
    LE_AVC_ERR_SECURITY_FAILURE = 3
        ///< Security check failure while installing the package.
}
le_avc_ErrorCode_t;


//--------------------------------------------------------------------------------------------------
/**
 * Session type indicates whether the device is connected to the bootstrap server or the
 * device management server.
 */
//--------------------------------------------------------------------------------------------------
typedef enum
{
    LE_AVC_BOOTSTRAP_SESSION = 0,
        ///< Bootstrap session.
    LE_AVC_DM_SESSION = 1,
        ///< Device Management session.
    LE_AVC_SESSION_INVALID = 2
        ///< Session type invalid.
}
le_avc_SessionType_t;


//--------------------------------------------------------------------------------------------------
/**
 * Reference type used by Add/Remove functions for EVENT 'le_avc_StatusEvent'
 */
//--------------------------------------------------------------------------------------------------
typedef struct le_avc_StatusEventHandler* le_avc_StatusEventHandlerRef_t;


//--------------------------------------------------------------------------------------------------
/**
 * Reference type used by Add/Remove functions for EVENT 'le_avc_SessionRequestEvent'
 */
//--------------------------------------------------------------------------------------------------
typedef struct le_avc_SessionRequestEventHandler* le_avc_SessionRequestEventHandlerRef_t;


//--------------------------------------------------------------------------------------------------
/**
 * Reference returned by BlockInstall function and used by UnblockInstall function
 */
//--------------------------------------------------------------------------------------------------
typedef struct le_avc_BlockRequest* le_avc_BlockRequestRef_t;


//--------------------------------------------------------------------------------------------------
/**
 * Handler for update availability status
 */
//--------------------------------------------------------------------------------------------------
typedef void (*le_avc_StatusHandlerFunc_t)
(
    le_avc_Status_t updateStatus,
        ///< status of pending update, if available
    int32_t totalNumBytes,
        ///< Total number of bytes to be downloaded
        ///< only valid when updateStatus is one of
        ///< DOWNLOAD_PENDING, DOWNLOAD_IN_PROGRESS or DOWNLOAD_COMPLETE.
        ///< returns -1 if value is unknown
    int32_t dloadProgress,
        ///< download completion in percentage
        ///< only valid when updateStatus is one of
        ///< DOWNLOAD_PENDING, DOWNLOAD_IN_PROGRESS or DOWNLOAD_COMPLETE.
        ///< returns -1 if value is unknown
    void* contextPtr
        ///<
);

//--------------------------------------------------------------------------------------------------
/**
 * Handler for receiving session open or close request.
 */
//--------------------------------------------------------------------------------------------------
typedef void (*le_avc_SessionRequestHandlerFunc_t)
(
    le_avc_SessionRequest_t request,
        ///< Request to open or close AV session
    void* contextPtr
        ///<
);

//--------------------------------------------------------------------------------------------------
/**
 * Add handler function for EVENT 'le_avc_StatusEvent'
 *
 * This event provides information on update availability status
 */
//--------------------------------------------------------------------------------------------------
le_avc_StatusEventHandlerRef_t le_avc_AddStatusEventHandler
(
    le_avc_StatusHandlerFunc_t handlerPtr,
        ///< [IN]
    void* contextPtr
        ///< [IN]
);

//--------------------------------------------------------------------------------------------------
/**
 * Remove handler function for EVENT 'le_avc_StatusEvent'
 */
//--------------------------------------------------------------------------------------------------
void le_avc_RemoveStatusEventHandler
(
    le_avc_StatusEventHandlerRef_t handlerRef
        ///< [IN]
);

//--------------------------------------------------------------------------------------------------
/**
 * Add handler function for EVENT 'le_avc_SessionRequestEvent'
 *
 * This event provides information on session open or close request.
 */
//--------------------------------------------------------------------------------------------------
le_avc_SessionRequestEventHandlerRef_t le_avc_AddSessionRequestEventHandler
(
    le_avc_SessionRequestHandlerFunc_t handlerPtr,
        ///< [IN]
    void* contextPtr
        ///< [IN]
);

//--------------------------------------------------------------------------------------------------
/**
 * Remove handler function for EVENT 'le_avc_SessionRequestEvent'
 */
//--------------------------------------------------------------------------------------------------
void le_avc_RemoveSessionRequestEventHandler
(
    le_avc_SessionRequestEventHandlerRef_t handlerRef
        ///< [IN]
);

//--------------------------------------------------------------------------------------------------
/**
 * Start a session with the AirVantage server
 *
 * This will cause a query to be sent to the server, for pending updates. If a download was
 * previously suspended, then this resumes the download.
 *
 * @return
 *      - LE_OK on success
 *      - LE_FAULT on failure
 *      - LE_DUPLICATE if already connected to AirVantage server.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_avc_StartSession
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * Stop a session with the AirVantage server
 *
 * If a download is in progress, then this suspends the download.
 *
 * @return
 *      - LE_OK on success
 *      - LE_FAULT on failure
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_avc_StopSession
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * Send a specific message to the server to be sure that the route between the device and the server
 * is available.
 * This API needs to be called when any package download is over (successfully or not) and before
 * sending any notification on asset data to the server.
 *
 * @return
 *      - LE_OK when the treatment is launched
 *      - LE_FAULT on failure
 *      - LE_UNSUPPORTED when the API is not supported
 *
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_avc_CheckRoute
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * Accept the currently pending download
 *
 * @return
 *      - LE_OK on success
 *      - LE_FAULT on failure
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_avc_AcceptDownload
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * Defer the currently pending download, for the given number of minutes
 *
 * @return
 *      - LE_OK on success
 *      - LE_FAULT on failure
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_avc_DeferDownload
(
    uint32_t deferMinutes
        ///< [IN]
);

//--------------------------------------------------------------------------------------------------
/**
 * Accept the currently pending install
 *
 * @return
 *      - LE_OK on success
 *      - LE_FAULT on failure
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_avc_AcceptInstall
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * Defer the currently pending install
 *
 * @return
 *      - LE_OK on success
 *      - LE_FAULT on failure
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_avc_DeferInstall
(
    uint32_t deferMinutes
        ///< [IN]
);

//--------------------------------------------------------------------------------------------------
/**
 * Accept the currently pending uninstall
 *
 * @return
 *      - LE_OK on success
 *      - LE_FAULT on failure
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_avc_AcceptUninstall
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * Defer the currently pending uninstall
 *
 * @return
 *      - LE_OK on success
 *      - LE_FAULT on failure
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_avc_DeferUninstall
(
    uint32_t deferMinutes
        ///< [IN]
);

//--------------------------------------------------------------------------------------------------
/**
 * Get the update type of the currently pending update
 *
 * @return
 *      - LE_OK on success
 *      - LE_FAULT if not available
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_avc_GetUpdateType
(
    le_avc_UpdateType_t* updateTypePtr
        ///< [OUT]
);

//--------------------------------------------------------------------------------------------------
/**
 * Get the name for the currently pending app update
 *
 * @return
 *      - LE_OK on success
 *      - LE_FAULT if not available, or isn't APPL_UPDATE type
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_avc_GetAppUpdateName
(
    char* updateName,
        ///< [OUT]
    size_t updateNameSize
        ///< [IN]
);

//--------------------------------------------------------------------------------------------------
/**
 * Prevent any pending updates from being installed.
 *
 * @return
 *      - Reference for block update request (to be used later for unblocking updates)
 *      - NULL if the operation was not successful
 */
//--------------------------------------------------------------------------------------------------
le_avc_BlockRequestRef_t le_avc_BlockInstall
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * Allow any pending updates to be installed
 */
//--------------------------------------------------------------------------------------------------
void le_avc_UnblockInstall
(
    le_avc_BlockRequestRef_t blockRef
        ///< [IN] block request ref returned by le_avc_BlockInstall
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
le_avc_ErrorCode_t le_avc_GetErrorCode
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * Function to read the current session type, or the last session type if there is no
 * active session.
 *
 * @return
 *      - SessionType
 */
//--------------------------------------------------------------------------------------------------
le_avc_SessionType_t le_avc_GetSessionType
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * Function to read the http status of the last download.
 *
 * @return
 *      - HttpStatus as defined in RFC 7231, Section 6.
 */
//--------------------------------------------------------------------------------------------------
uint16_t le_avc_GetHttpStatus
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * Function to read the polling timer.
 *
 * @return
 *      - LE_OK on success
 *      - LE_FAULT if not available
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_avc_GetPollingTimer
(
    uint32_t* pollingTimerPtr
        ///< [OUT] Polling timer (0 to 525600 minutes).
);

//--------------------------------------------------------------------------------------------------
/**
 * Function to read the retry timers.
 *
 * @return
 *      - LE_OK on success.
 *      - LE_FAULT if not able to read the timers.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_avc_GetRetryTimers
(
    uint16_t* timerValuePtr,
        ///< [OUT] Array of the retry timers.
    size_t* timerValueSizePtr
        ///< [INOUT]
);

//--------------------------------------------------------------------------------------------------
/**
 * Function to read APN configuration.
 *
 * @return
 *      - LE_OK on success.
 *      - LE_FAULT if there is any error while reading.
 *      - LE_OVERFLOW if the buffer provided is too small.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_avc_GetApnConfig
(
    char* apnName,
        ///< [OUT]
    size_t apnNameSize,
        ///< [IN]
    char* userName,
        ///< [OUT]
    size_t userNameSize,
        ///< [IN]
    char* userPwd,
        ///< [OUT]
    size_t userPwdSize
        ///< [IN]
);

//--------------------------------------------------------------------------------------------------
/**
 * Function to write APN configuration.
 *
 * @return
 *      - LE_OK on success.
 *      - LE_FAULT if not able to write the APN configuration.
 *      - LE_OVERFLOW if one of the input strings is too long.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_avc_SetApnConfig
(
    const char* LE_NONNULL apnName,
        ///< [IN]
    const char* LE_NONNULL userName,
        ///< [IN]
    const char* LE_NONNULL userPwd
        ///< [IN]
);

//--------------------------------------------------------------------------------------------------
/**
 * Function to set the polling timer.
 *
 * @return
 *      - LE_OK on success.
 *      - LE_FAULT if not able to read the timers.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_avc_SetPollingTimer
(
    uint32_t pollingTimer
        ///< [IN] Polling timer (0 to 525600 minutes).
);

//--------------------------------------------------------------------------------------------------
/**
 * Function to set the retry timers.
 *
 * @return
 *      - LE_OK on success.
 *      - LE_FAULT if not able to write the timers.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_avc_SetRetryTimers
(
    const uint16_t* timerValuePtr,
        ///< [IN] Array of 8 retry timers.
    size_t timerValueSize
        ///< [IN]
);

#endif // LE_AVC_INTERFACE_H_INCLUDE_GUARD