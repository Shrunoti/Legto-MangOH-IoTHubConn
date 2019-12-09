

/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */

/**
 * @page c_le_avc AirVantage Connector API
 *
 * @ref le_avc_interface.h "API Reference" <br>
 * @ref legatoServicesAirVantage "AirVantage Connector Platform Service"
 *
 *
 * The AirVantage connector service provides an API to communicate with the AirVantage Server to
 * download and install updates.
 *
 * @section c_le_avc_update Firmware/Application Updates
 *
 * The API for firmware/application update is divided into two parts:
 *  - Allow an App to contact the server for pending updates. If available,
 *    can select to download or install the update.
 *  - Allow an App to block an update from being installed. An App may need this if it's
 *    performing a critical operation that can't be interrupted (e.g., eCall). This is necessary
 *    as installing an update will cause the App to restart (either the App itself needs to restart
 *    or a firmware change causes the modem to reset).
 *
 * @subsection c_le_avc_user_agreement User Agreement
 *
 * When writing a control app for the AirVantage Service it is up to the developer to ensure that
 * user agreements have been enabled for all actions the control app performs.  By default all
 * user agreements are enabled except for LE_AVC_CONNECTION_PENDING and the avcService app will
 * notify the control app and wait for a response before downloading or installing the update.
 * If the avcService can't communicate with the control app it waits for 3 minutes and tries again.
 * To not have the avcService wait for the control app to accept the agreement the user agreement
 * must be disabled.
 *
 * There are 5 events that require user agreements:
 * - LE_AVC_CONNECTION_PENDING
 * - LE_AVC_DOWNLOAD_PENDING
 * - LE_AVC_INSTALL_PENDING
 * - LE_AVC_REBOOT_PENDING
 * - LE_AVC_UNINSTALL_PENDING
 *
 * An app can respond to these user agreement notification by opening a connection if the
 * notification is connection pending or by accepting or deferring the operation if the notification
 * is one of download, install, uninstall or reboot pending.
 *
 * Whether avcService should forward these notifications to an app or act on these notifications by
 * itself is determined by the individual configuration flags that enables or disables user
 * agreement for these operations. If the user agreement flag is turned off for an operation,
 * the default action by avcService is to automatically accept that pending operation. An app can
 * register its handler by calling le_avc_AddStatusEventHandler() described in
 * c_le_avc_update_control and the configuration flags can be set by le_avc_SetUserAgreement().
 *
 * Example of enabling user agreement for LE_AVC_DOWNLOAD_PENDING:
 * @code
 *        le_result_t result;
 *        result = le_avc_SetUserAgreement(LE_AVC_USER_AGREEMENT_DOWNLOAD, true);
 *
 *        if (result != LE_OK)
 *        {
 *            LE_ERROR("Failed to enable user agreement for download operation");
 *        }
 * @endcode
 *
 * @subsection c_le_avc_update_control Update Control
 *
 * Any App can start a session with an AirVantage server to determine if there
 * is a pending update. This is done with le_avc_StartSession().
 *
 * The current session can be stopped using le_avc_StopSession(). This cancels any query for pending
 * update, or suspends a download in progress; however, it <b>won't stop</b> an install in progress.
 *
 * The status of a pending update is sent to all Apps registered for receiving a notification.
 * An App can use le_avc_AddStatusEventHandler() to register a handler function to
 * receive this notification. The notification will be received after a session is started.
 *
 * Example of registering an AVC handler and starting a session with fault checking:
 * @code
 *    // Start AVC Session
 *    LE_INFO("AirVantage Connection Controller started.");
 *    le_avc_AddStatusEventHandler(avcStatusHandler, NULL);    //register a AVC handler
 *
 *    //Start AVC session. Note: AVC handler must be registered prior starting a session
 *    le_result_t result = le_avc_StartSession();
 *    if (LE_FAULT == result)
 *    {
 *        le_avc_StopSession();
 *        le_avc_StartSession();
 *    }
 * @endcode
 *
 * An App can use le_avc_AddCommInfoHandler() to register a handler function and receive
 * communication information notifications. This function returns a reference. To unregister
 * the handler, le_avc_RemoveCommInfoHandler() should be called with the previously returned
 * reference.
 *
 *
 * Sometimes, @c avcService may decide to start a session with the AirVantage
 * server; for instance if a call to le_avc_AcceptDownload() when the session is stopped,
 * @c avcService will open a session to proceed with the download. In this case, a session started
 * notification could also be received, even if le_avc_StartSession() is not called explicitly.
 *
 * @subsubsection c_le_avc_updateControl_pending Pending Updates
 *
 * There can only be one pending update at a time.  To query the type of update, use
 * le_avc_GetUpdateType().  App updates can call le_avc_GetAppUpdateName() to retrieve the App name.
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
 * install is pending. Apps can then use le_avc_AcceptInstall() to allow the install to proceed.
 *
 * If an uninstall is pending, then le_avc_AcceptUninstall() can be used to allow the uninstall to
 * proceed. To defer the decision, le_avc_DeferUninstall() can be used to defer the uninstall for
 * the specified number of minutes. In case of an upgrade, the existing application will not be
 * uninstalled after le_avc_AcceptUninstall() is called. le_avc_AcceptUninstall() is only used to
 * signal the server to start downloading the new application. To proceed with an upgrade process,
 * accept the uninstall of the existing version followed by accepting the download and install of
 * the new version.
 *
 * @code
 *   switch (updateStatus)
 *        {
 *            case LE_AVC_DOWNLOAD_PENDING:
 *                LE_INFO("Accepting %s update.", GetUpdateType());
 *                res = le_avc_AcceptDownload();
 *                if (res != LE_OK)
 *                {
 *                    LE_ERROR("Failed to accept download from AirVantage (%s)", LE_RESULT_TXT(res));
 *                }
 *                break;
 *
 *            case LE_AVC_INSTALL_PENDING:
 *                LE_INFO("Accepting %s installation.", GetUpdateType());
 *                res = le_avc_AcceptInstall();
 *                if (res != LE_OK)
 *                {
 *                    LE_ERROR("Failed to accept install from AirVantage (%s)", LE_RESULT_TXT(res));
 *                }
 *                break;
 *
 *            case LE_AVC_UNINSTALL_PENDING:
 *                LE_INFO("Accepting %s uninstall.", GetUpdateType());
 *                res = le_avc_AcceptUninstall();
 *                if (res != LE_OK)
 *                {
 *                    LE_ERROR("Failed to accept uninstall from AirVantage (%s)", LE_RESULT_TXT(res));
 *                }
 *                break;
 *
 *            case LE_AVC_REBOOT_PENDING:
 *                LE_INFO("Accepting device reboot.");
 *                res = le_avc_AcceptReboot();
 *                if (res != LE_OK)
 *                {
 *                    LE_ERROR("Failed to accept reboot from AirVantage (%s)", LE_RESULT_TXT(res));
 *                }
 *                break;
 *
 *            default:
 *                // No action required
 *                break;
 *        }
 * @endcode
 *
 * @note Even if an App calls le_avc_AcceptInstall(), the install may still be blocked by another
 * App using the @ref c_le_avc_update_app functions. To defer the decision, an App can use
 * le_avc_DeferInstall() to defer the install for the specified number of minutes. After the defer
 * time has elapsed, the pending install notification will be re-sent to allow Apps to make a new
 * decision, or again defer.
 *
 *
 * @subsubsection c_le_avc_updateControl_accepting Accepting Installs/Uninstalls
 *
 * Accepting an App install or uninstall will not initiate an AirVantage session if no session
 * is active.  An App should start an AirVantage session before accepting an App install/uninstall,
 * to ensure the process is completed, and the server is updated.
 *
 * If no app has registered for notifications using le_avc_AddStatusEventHandler(),
 * then any pending downloads and installs will happen automatically, subject to any restrictions
 * imposed by app using the @ref c_le_avc_update_app functions.
 *
 * There is no restriction on the number of Apps registering for notifications.
 *
 * In case of any error incurred during App download/install, an error code will be set which can be
 * retrieved by calling le_avc_GetErrorCode().
 *
 * @subsection c_le_avc_update_app Application Installation Blocking
 *
 * When an App is about to perform a critical operation, it can block the installation of
 * an update with le_avc_BlockInstall(), and after it's finished with the critical operation, it
 * can unblock the install with le_avc_UnblockInstall().
 *
 * What constitutes a critical operation depends on the App.  An eCall App might
 * block installs for the duration that it runs.  A data collection App that wakes up
 * once an hour might block installs while it collects and stores and/or transmits a new data
 * sample, and then unblock installs just before it goes to sleep again.
 *
 * If an install can't be applied because it's blocked, another attempt to apply the install
 * will be made at a later time.
 *
 * An App can add a session control handler using le_avc_AddSessionRequestEventHandler(). Requests
 * by user Apps to open or close session will be forwarded to the session control handler. If no App
 * has registered a session request handler, AVC session can be opened or closed anytime by
 * user Apps.
 *
 * @subsection c_le_avc_suspend_resume Suspend and resume an update
 *
 * A firmware/application update can be interrupted or suspended by different events:
 * - a device reboot
 * - a network loss
 * - a manual suspend through a session stop.
 *
 * After the update suspension, when the @c avcService is ready to resume the update process:
 * - If the user agreements are disabled, the update process is automatically restarted.
 * - If the user agreements are enabled, the previous @c PENDING notification is sent to the App,
 * as indicated in the table below.
 *
 * @note The @c avcService might take a while to suspend the update process (typically a few
 * minutes), e.g. in some cases of firmware update. The @c PENDING notification will therefore be
 * sent only when the suspension is over. Trying to resume the update process while the suspension
 * is not over and the notification is not received has no effect.
 *
 * | Last received event            | Event sent to resume                                        | Expected action from App |
 * |--------------------------------|-------------------------------------------------------------|--------------------------|
 * | @c LE_AVC_DOWNLOAD_PENDING     | @c LE_AVC_DOWNLOAD_PENDING                                  | Accept download          |
 * | @c LE_AVC_DOWNLOAD_IN_PROGRESS | @c LE_AVC_DOWNLOAD_PENDING with remaining bytes to download | Accept download          |
 * | @c LE_AVC_DOWNLOAD_COMPLETE    | @c LE_AVC_DOWNLOAD_PENDING with zero bytes to download      | Accept download          |
 * | @c LE_AVC_INSTALL_PENDING      | @c LE_AVC_INSTALL_PENDING                                   | Accept install           |
 * | @c LE_AVC_INSTALL_IN_PROGRESS  | @c LE_AVC_INSTALL_PENDING                                   | Accept install           |
 * | @c LE_AVC_UNINSTALL_PENDING    | @c LE_AVC_UNINSTALL_PENDING                                 | Accept uninstall         |
 * | @c LE_AVC_CONNECTION_PENDING   | @c LE_AVC_CONNECTION_PENDING                                | Start session            |
 *
 * @note If firmware is updated (via fwupdate tool) or new legato is installed (via instlegato),
 * all suspend/resume information stored by avcService is erased. So if developer updates firmware
 * or legato (via ethernet or ecm etc.) in the middle of any update initiated by avcService, this
 * need to be cancelled and restarted again from airVantage server.
 *
 * @section c_le_avc_Timers Timers
 *
 * Polling timers sets the time that the Target will communicate with the AirVantage Server to check
 * for new jobs.  Retry timers will try and re-establish a connection to the AirVantage Server in
 * accordance with the times that are declared.
 *
 * @subsection c_le_avc_PollingTimer Polling Timer
 *
 * The target will periodically initiate a connection to the AirVantage Server according to the
 * settings for the polling timer to check if there are any pending jobs.  The polling timer will
 * initiate a session when it starts the count (at 0 minutes) and then again at the specified time
 * set.
 *
 * To disable the polling timer call le_avc_SetPollingTimer() and set the value to 0.
 *
 * The polling timer accepts ranges from 0 to 525600 minutes.  The polling timer does not get reset
 * with reboots or power loss.  If the target is powered off during the polling time, it will
 * connect to the AVC Server upon startup of the app.  For example, if the polling timer is set to 1
 * hour then and the target reboots at the 20 minute mark, the polling timer will still initiate a
 * connection at the 1 hour mark.  If the target is powered off or in the middle of rebooting at the
 * 1 hour mark, as soon as the app is started again, the polling timer will initiate a connection to
 * the AirVantage Server.
 *
 * Polling timer initiated sessions will be disconnected after 20 seconds of inactivity. This does
 * not apply to AirVantage sessions that have been initiated by an app. The app is responsible for
 * disconnecting the session it initiates within a reasonable timeframe. Leaving the session open
 * for long period will result in power wastage.
 *
 * To read the polling timer call: le_avc_GetPollingTimer()
 *
 * To write a new value to the polling timer call: le_avc_SetPollingTimer()
 *
 * Writing to the polling timer stops the current polling timer if it is running and starts a timer
 * with the new value. The next connection will be initiated when the new polling timer reaches it's
 * set value.
 *
 * @subsection c_le_avc_RetryTimers Retry Timers
 *
 * If an error occurs during a connection to the Device Services server (WWAN DATA establishment
 * failed and an http error code is received) the embedded module will initiate a new connection
 * according to the values defined in the retry timers.
 *
 * The timers are tried in sequence until a connection is established, or all enabled retry timers
 * are exhausted. After all the enabled timers are exhausted, a new session must be initiated again
 * by calling le_avc_startSession() after the expiry of the retry timer.
 *
 * The retry timer values are persistent (reboots and updates do not effect the retry timers).
 * If you wish to disable a retry timer set the timer value
 * to 0.  You must always pass in at least 8 values to the retry timers.
 *
 * Retry timer values range from 0 to 20160 minutes.
 * The function le_avc_GetRetryTimers() reads the retry timers in an array and the function
 * le_avc_SetRetryTimers() writes the retry timers. When writing to the retry timers, values of
 * all the 8 timers have to be defined.
 *
 * Example of calling retry timers, the session will be retried after 15 minutes, 1 hour, 4 hours,
 * 8 hours, 1 day and 2 days, the last two retries are disabled:
 * @code
 *   uint16_t RetryTimers[LE_AVC_NUM_RETRY_TIMERS] = {15, 60, 240, 480, 1440, 2880, 0, 0};
 *   le_avc_SetRetryTimers(RetryTimers, LE_AVC_NUM_RETRY_TIMERS);
 * @endcode
 *
 * @section c_le_avc_reboot Device reboot
 *
 * The AirVantage server can request to reboot the device. If a reboot is requested a notification
 * is sent to the registered Apps. The App can either accept the reboot with le_avc_AcceptReboot()
 * or defer it for a specified number of minutes with le_avc_DeferReboot(). After the defer time
 * has elapsed, the pending reboot notification will be re-sent. This allows the registered app to
 * make a new decision or defer the reboot again.
 *
 * If no App has registered for notifications using le_avc_AddStatusEventHandler(), then
 * any pending reboot will happen automatically.
 *
 * @section c_le_avc_GetCredentialStatus Credential Status
 * The device is provisioned with bootstrap credentials from factory. The Device Management (DM)
 * credentials are provisioned by AirVantage Bootstrap Server. This API is used to retrieve the
 * status of credentials provisioned on the device.
 *
 * @section c_le_avc_connection Connection pending
 *
 * The AirVantage agent can request a connection to the AirVantage server, especially when a
 * firmware package is installed (after a platform reboot) or device reboots in the middle of
 * software update (after finishing software update on reboot). In this case a notification is sent
 * to the control App, which can start the connection with le_avc_StartSession().
 *
 * @section c_le_avc_routing Data routing
 *
 * By default the AirVantage connection uses the default mobile data profile and the default route
 * set by the data connection service.
 *
 * If the user wishes to control the network configuration, e.g. to use the AirVantage agent with
 * multi-PDP contexts, they should first bind the application to the data connection service:
 * @verbatim
   bindings:
   {
      clientExe.clientComponent.le_data -> dataConnectionService.le_data
   }
   @endverbatim
 *
 * The data connection service should then be configured before launching the AirVantage connection:
 * - le_data_SetCellularProfileIndex() allows to change the data profile to use.
 * - le_data_GetDefaultRouteStatus() indicates if the default route is activated in the data
 * connection service. This default route can be deactivated in the data connection service
 * configuration database, as explained in @ref c_le_data_defaultRoute. If the default route is
 * deactivated, the AirVantage agent will automatically add routes to be able to reach the
 * AirVantage server through the connection used by AirVantage.
 *
 * @section c_le_avc_timeout Connection / Download timeout
 *
 * The AirVantage connector service will abort FOTA/SOTA download, if it takes more than
 * 300 seconds to establish a connection. Download will also be aborted, if the download speed is
 * too low (less than 100 bytes /second) for too long (for more than 1000 seconds).
 * These values are chosen based on experiments on low speed network. There is no configuration
 * for these timeouts.
 *
 * @section le_avcService_configdb Service Configuration Tree
 * @copydoc le_avcService_configdbPage_Hide
 *
 *
 * Copyright (C) Sierra Wireless Inc.
 */
/**
 * @interface le_avcService_configdbPage_Hide
 *
 * The configuration database path for the activityTimeout is:
 * @verbatim
 /
     apps/
         avcService/
                  activityTimeout
 @endverbatim
 *
 *
 * After an AirVantage session is started, if there's no activity between the device and the server
 * within the timer interval, then LE_AVC_NO_UPDATE state will be returned to the app. However,
 * this activity timeout can be overridden by setting an integer value for
 * /apps/avcService/activityTimeout. The activity timer is initialized only when the @c avcService
 * starts. If a valid entry >0 is found, then it will be used instead of the default value of 20
 * seconds. The following steps should be used to set the activityTimeout.
 *
 *
 * @verbatim
 config set /apps/avcService/activityTimeout xx int
 app restart avcService
 @endverbatim
 *
 * @note
 * Everytime a new value is written to activityTimeout, the avcService needs to be
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
 *  Polling timer value range in minutes. 525600 minutes = 1 year.
 */
//--------------------------------------------------------------------------------------------------
#define LE_AVC_POLLING_TIMER_MAX_VAL 525600

//--------------------------------------------------------------------------------------------------
/**
 */
//--------------------------------------------------------------------------------------------------
#define LE_AVC_POLLING_TIMER_MIN_VAL 0

//--------------------------------------------------------------------------------------------------
/**
 *  Retry timer value range in minutes. 20160 minutes = 2 weeks.
 */
//--------------------------------------------------------------------------------------------------
#define LE_AVC_RETRY_TIMER_MAX_VAL 20160

//--------------------------------------------------------------------------------------------------
/**
 */
//--------------------------------------------------------------------------------------------------
#define LE_AVC_RETRY_TIMER_MIN_VAL 0

//--------------------------------------------------------------------------------------------------
/**
 * Default HTTP status.
 */
//--------------------------------------------------------------------------------------------------
#define LE_AVC_HTTP_STATUS_INVALID 65535

//--------------------------------------------------------------------------------------------------
/**
 * Communication info max string size
 */
//--------------------------------------------------------------------------------------------------
#define LE_AVC_COMM_INFO_STR_MAX_LEN 256

//--------------------------------------------------------------------------------------------------
/**
 * Maximum length of a LwM2M resource excluding any termination character.
 */
//--------------------------------------------------------------------------------------------------
#define LE_AVC_MAX_LWM2M_RESOURCE_LEN 512

//--------------------------------------------------------------------------------------------------
/**
 * Maximum length of a LwM2M resource. One extra byte is added for the null character.
 */
//--------------------------------------------------------------------------------------------------
#define LE_AVC_MAX_LWM2M_RESOURCE_BYTES 513

//--------------------------------------------------------------------------------------------------
/**
 * Communication info codes
 */
//--------------------------------------------------------------------------------------------------
#define LE_AVC_COMM_INFO_UDP_NO_ERR 0

//--------------------------------------------------------------------------------------------------
/**
 */
//--------------------------------------------------------------------------------------------------
#define LE_AVC_COMM_INFO_UDP_OPEN_ERR 1

//--------------------------------------------------------------------------------------------------
/**
 */
//--------------------------------------------------------------------------------------------------
#define LE_AVC_COMM_INFO_UDP_CLOSE_ERR 2

//--------------------------------------------------------------------------------------------------
/**
 */
//--------------------------------------------------------------------------------------------------
#define LE_AVC_COMM_INFO_UDP_SEND_ERR 3

//--------------------------------------------------------------------------------------------------
/**
 */
//--------------------------------------------------------------------------------------------------
#define LE_AVC_COMM_INFO_UDP_RECV_ERR 4

//--------------------------------------------------------------------------------------------------
/**
 */
//--------------------------------------------------------------------------------------------------
#define LE_AVC_COMM_INFO_UDP_CONNECT_ERR 5

//--------------------------------------------------------------------------------------------------
/**
 */
//--------------------------------------------------------------------------------------------------
#define LE_AVC_COMM_INFO_BEARER_UP 6

//--------------------------------------------------------------------------------------------------
/**
 */
//--------------------------------------------------------------------------------------------------
#define LE_AVC_COMM_INFO_BEARER_DOWN 7

//--------------------------------------------------------------------------------------------------
/**
 */
//--------------------------------------------------------------------------------------------------
#define LE_AVC_COMM_INFO_PDP_CONTEXT 8

//--------------------------------------------------------------------------------------------------
/**
 */
//--------------------------------------------------------------------------------------------------
#define LE_AVC_COMM_INFO_COAP_201_CREATED 65

//--------------------------------------------------------------------------------------------------
/**
 */
//--------------------------------------------------------------------------------------------------
#define LE_AVC_COMM_INFO_COAP_202_DELETED 66

//--------------------------------------------------------------------------------------------------
/**
 */
//--------------------------------------------------------------------------------------------------
#define LE_AVC_COMM_INFO_COAP_204_CHANGED 68

//--------------------------------------------------------------------------------------------------
/**
 */
//--------------------------------------------------------------------------------------------------
#define LE_AVC_COMM_INFO_COAP_205_CONTENT 69

//--------------------------------------------------------------------------------------------------
/**
 */
//--------------------------------------------------------------------------------------------------
#define LE_AVC_COMM_INFO_COAP_231_CONTINUE 95

//--------------------------------------------------------------------------------------------------
/**
 */
//--------------------------------------------------------------------------------------------------
#define LE_AVC_COMM_INFO_COAP_400_BAD_REQUEST 128

//--------------------------------------------------------------------------------------------------
/**
 */
//--------------------------------------------------------------------------------------------------
#define LE_AVC_COMM_INFO_COAP_401_UNAUTHORIZED 129

//--------------------------------------------------------------------------------------------------
/**
 */
//--------------------------------------------------------------------------------------------------
#define LE_AVC_COMM_INFO_COAP_402_BAD_OPTION 130

//--------------------------------------------------------------------------------------------------
/**
 */
//--------------------------------------------------------------------------------------------------
#define LE_AVC_COMM_INFO_COAP_404_NOT_FOUND 132

//--------------------------------------------------------------------------------------------------
/**
 */
//--------------------------------------------------------------------------------------------------
#define LE_AVC_COMM_INFO_COAP_405_METHOD_NOT_ALLOWED 133

//--------------------------------------------------------------------------------------------------
/**
 */
//--------------------------------------------------------------------------------------------------
#define LE_AVC_COMM_INFO_COAP_406_NOT_ACCEPTABLE 134

//--------------------------------------------------------------------------------------------------
/**
 */
//--------------------------------------------------------------------------------------------------
#define LE_AVC_COMM_INFO_COAP_408_REQ_ENTITY_INCOMPLETE 136

//--------------------------------------------------------------------------------------------------
/**
 */
//--------------------------------------------------------------------------------------------------
#define LE_AVC_COMM_INFO_COAP_412_PRECONDITION_FAILED 140

//--------------------------------------------------------------------------------------------------
/**
 */
//--------------------------------------------------------------------------------------------------
#define LE_AVC_COMM_INFO_COAP_413_ENTITY_TOO_LARGE 141

//--------------------------------------------------------------------------------------------------
/**
 */
//--------------------------------------------------------------------------------------------------
#define LE_AVC_COMM_INFO_COAP_500_INTERNAL_SERVER_ERROR 160

//--------------------------------------------------------------------------------------------------
/**
 */
//--------------------------------------------------------------------------------------------------
#define LE_AVC_COMM_INFO_COAP_501_NOT_IMPLEMENTED 161

//--------------------------------------------------------------------------------------------------
/**
 */
//--------------------------------------------------------------------------------------------------
#define LE_AVC_COMM_INFO_COAP_503_SERVICE_UNAVAILABLE 163

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
        ///< Session with AirVantage device management server started
    LE_AVC_SESSION_BS_STARTED = 14,
        ///< Session with bootstrap server started
    LE_AVC_SESSION_STOPPED = 15,
        ///< Session with AirVantage device management server or bootstrap
        ///< server stopped
    LE_AVC_SESSION_FAILED = 16,
        ///< Session with AirVantage device management server or bootstrap
        ///< server failed
    LE_AVC_REBOOT_PENDING = 17,
        ///< Device reboot is pending
    LE_AVC_CONNECTION_PENDING = 18,
        ///< Connection to the server is required. This is necessary when
        ///< firmware package is installed (after a platform reboot). Also
        ///< necessary after software update if device reboots(or session stops)
        ///< in the middle of software update.
    LE_AVC_AUTH_STARTED = 19,
        ///< Authentication with AirVantage device management server or
        ///< bootstrap server started
    LE_AVC_AUTH_FAILED = 20,
        ///< Authentication with AirVantage device management server or
        ///< bootstrap server failed
    LE_AVC_CERTIFICATION_OK = 21,
        ///< Package is certified sent by a trusted server
    LE_AVC_CERTIFICATION_KO = 22
        ///< Package is not certified sent by a trusted server
}
le_avc_Status_t;


//--------------------------------------------------------------------------------------------------
/**
 * Operations which require user agreement
 */
//--------------------------------------------------------------------------------------------------
typedef enum
{
    LE_AVC_USER_AGREEMENT_CONNECTION = 0,
        ///< User agreement for connection
    LE_AVC_USER_AGREEMENT_DOWNLOAD = 1,
        ///< User agreement for download
    LE_AVC_USER_AGREEMENT_INSTALL = 2,
        ///< User agreement for install
    LE_AVC_USER_AGREEMENT_UNINSTALL = 3,
        ///< User agreement for uninstall
    LE_AVC_USER_AGREEMENT_REBOOT = 4
        ///< User agreement for reboot
}
le_avc_UserAgreement_t;


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
 * Status of the device credentials
 */
//--------------------------------------------------------------------------------------------------
typedef enum
{
    LE_AVC_NO_CREDENTIAL_PROVISIONED = 1,
        ///< Neither Bootstrap nor Device Management
        ///< credential is provisioned.
    LE_AVC_BS_CREDENTIAL_PROVISIONED = 2,
        ///< Bootstrap credential is provisioned but Device
        ///< Management credential is not provisioned.
    LE_AVC_DM_CREDENTIAL_PROVISIONED = 3
        ///< Device Management credential is provisioned.
}
le_avc_CredentialStatus_t;


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
 * Reference type used by Add/Remove functions for EVENT 'le_avc_CommInfo'
 */
//--------------------------------------------------------------------------------------------------
typedef struct le_avc_CommInfoHandler* le_avc_CommInfoHandlerRef_t;


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
    int32_t progress,
        ///< Task completion in percentage
        ///< Valid when updateStatus is one of DOWNLOAD_IN_PROGRESS,
        ///< INSTALL_IN_PROGRESS or UNINSTALL_IN_PROGRESS.
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
 * Handler for receiving communication information.
 */
//--------------------------------------------------------------------------------------------------
typedef void (*le_avc_CommInfoHandlerFunc_t)
(
    uint8_t code,
        ///< Communication information code
    const char* LE_NONNULL str,
        ///< Communication information string
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
 * Add handler function for EVENT 'le_avc_CommInfo'
 *
 * This event provides communication errors.
 */
//--------------------------------------------------------------------------------------------------
le_avc_CommInfoHandlerRef_t le_avc_AddCommInfoHandler
(
    le_avc_CommInfoHandlerFunc_t handlerPtr,
        ///< [IN]
    void* contextPtr
        ///< [IN]
);

//--------------------------------------------------------------------------------------------------
/**
 * Remove handler function for EVENT 'le_avc_CommInfo'
 */
//--------------------------------------------------------------------------------------------------
void le_avc_RemoveCommInfoHandler
(
    le_avc_CommInfoHandlerRef_t handlerRef
        ///< [IN]
);

//--------------------------------------------------------------------------------------------------
/**
 * Start a session with the AirVantage server
 *
 * This will cause a query to be sent to the server, for pending updates.
 *
 * @return
 *      - LE_OK if connection request has been sent.
 *      - LE_FAULT on failure
 *      - LE_DUPLICATE if already connected.
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
 *      - LE_DUPLICATE if already disconnected
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
 * Defer the currently pending connection, for the given number of minutes
 *
 * @return
 *      - LE_OK on success
 *      - LE_FAULT on failure
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_avc_DeferConnect
(
    uint32_t deferMinutes
        ///< [IN]
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
 * Accept the currently pending reboot
 *
 * @return
 *      - LE_OK on success
 *      - LE_FAULT on failure
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_avc_AcceptReboot
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * Defer the currently pending reboot
 *
 * @return
 *      - LE_OK on success
 *      - LE_FAULT on failure
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_avc_DeferReboot
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
 *      - LE_OUT_OF_RANGE if the polling timer value is out of range (0 to 525600).
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_avc_GetPollingTimer
(
    uint32_t* pollingTimerPtr
        ///< [OUT] Polling timer
);

//--------------------------------------------------------------------------------------------------
/**
 * Function to read the retry timers.
 *
 * @return
 *      - LE_OK on success.
 *      - LE_FAULT if not able to read the timers.
 *      - LE_OUT_OF_RANGE if one of the retry timers is out of range (0 to 20160).
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
 *      - LE_OUT_OF_RANGE if the polling timer value is out of range (0 to 525600).
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_avc_SetPollingTimer
(
    uint32_t pollingTimer
        ///< [IN] Polling timer
);

//--------------------------------------------------------------------------------------------------
/**
 * Function to set the retry timers.
 *
 * @return
 *      - LE_OK on success.
 *      - LE_FAULT if not able to set the timers.
 *      - LE_OUT_OF_RANGE if one of the retry timers is out of range (0 to 20160).
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_avc_SetRetryTimers
(
    const uint16_t* timerValuePtr,
        ///< [IN] Array of 8 retry timers.
    size_t timerValueSize
        ///< [IN]
);

//--------------------------------------------------------------------------------------------------
/**
 * Function to retrieve status of the credentials provisioned on the device.
 *
 * @return
 *     LE_AVC_NO_CREDENTIAL_PROVISIONED
 *          - If neither Bootstrap nor Device Management credential is provisioned.
 *     LE_AVC_BS_CREDENTIAL_PROVISIONED
 *          - If Bootstrap credential is provisioned but Device Management credential is
 *              not provisioned.
 *     LE_AVC_DM_CREDENTIAL_PROVISIONED
 *          - If Device management key is provisioned.
 */
//--------------------------------------------------------------------------------------------------
le_avc_CredentialStatus_t le_avc_GetCredentialStatus
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * Function to set user agreements for download, install, reboot, connection and uninstall.
 *
 * @return
 *      - LE_OK on success.
 *      - LE_FAULT if failed to configure user agreement.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_avc_SetUserAgreement
(
    le_avc_UserAgreement_t updateStatus,
        ///< [IN] Operation for which user agreements has to be set.
    bool enable
        ///< [IN] true = enable, false = disable.
);

//--------------------------------------------------------------------------------------------------
/**
 * Function to get user agreements for download, install, reboot, connection and uninstall.
 *
 * @return
 *      - LE_OK on success.
 *      - LE_FAULT if failed to read user agreement state.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_avc_GetUserAgreement
(
    le_avc_UserAgreement_t updateStatus,
        ///< [IN] Operation for which user agreements has to be read.
    bool* enablePtr
        ///< [OUT] true = enable, false = disable.
);

//--------------------------------------------------------------------------------------------------
/**
 * Function to read a resource from a LWM2M object
 *
 * @return
 *      - LE_OK on success.
 *      - LE_FAULT if failed.
 *      - LE_UNSUPPORTED if unsupported.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_avc_ReadLwm2mResource
(
    uint16_t objectId,
        ///< [IN] Object identifier
    uint16_t objectInstanceId,
        ///< [IN] Object instance identifier
    uint16_t resourceId,
        ///< [IN] Resource identifier
    uint16_t resourceInstanceId,
        ///< [IN] Resource instance identifier
    char* data,
        ///< [OUT] String of requested resources to be read
    size_t dataSize
        ///< [IN]
);

#endif // LE_AVC_INTERFACE_H_INCLUDE_GUARD