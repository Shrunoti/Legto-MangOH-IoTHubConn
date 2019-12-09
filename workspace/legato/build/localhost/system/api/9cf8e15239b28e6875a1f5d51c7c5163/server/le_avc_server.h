

/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */


#ifndef LE_AVC_INTERFACE_H_INCLUDE_GUARD
#define LE_AVC_INTERFACE_H_INCLUDE_GUARD


#include "legato.h"

// Interface specific includes
#include "le_limit_server.h"


//--------------------------------------------------------------------------------------------------
/**
 * Get the server service reference
 */
//--------------------------------------------------------------------------------------------------
le_msg_ServiceRef_t le_avc_GetServiceRef
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * Get the client session reference for the current message
 */
//--------------------------------------------------------------------------------------------------
le_msg_SessionRef_t le_avc_GetClientSessionRef
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * Initialize the server and advertise the service.
 */
//--------------------------------------------------------------------------------------------------
void le_avc_AdvertiseService
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