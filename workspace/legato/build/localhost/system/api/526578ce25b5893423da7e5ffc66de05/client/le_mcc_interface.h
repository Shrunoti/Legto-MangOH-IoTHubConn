

/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */

/**
 * @page c_mcc Modem Call Control
 *
 * @ref le_mcc_interface.h "API Reference"
 *
 * <HR>
 *
 * The Modem Call Control (mcc) API functions of this API are provided by the @b modemService
 * service.
 *
 * @section le_mcc_binding IPC interfaces binding
 *
 * Here's a code sample binding to modem services:
 * @verbatim
   bindings:
   {
      clientExe.clientComponent.le_mcc -> modemService.le_mcc
   }
   @endverbatim
 *
 * @section le_mcc_starting_a_call Starting a Call
 *
 * To initiate a call, create a new call object with a destination telephone
 * number calling the le_mcc_Create() function.
 *
 * le_mcc_Start() must still initiate the call when ready.
 *
 * The le_mcc_Start() function initiates a call attempt (it's asynchronous because it can take
 * time for a call to connect). If function failed, the le_mcc_GetTerminationReason() API can be
 * used to retrieve the terminal reason.
 *
 * It's essential to register a handler function to get the call events. Use
 * le_mcc_AddCallEventHandler() API to install that handler function. The handler will be
 * called for all calls' events (incoming and outgoing).
 *
 * The le_mcc_RemoveCallEventHandler() API uninstalls the handler function.
 *
 * The following APIs can be used to manage incoming or outgoing calls:
 * - le_mcc_GetTerminationReason() - termination reason.
 * - le_mcc_GetPlatformSpecificTerminationCode() - let you get the platform specific
 *   termination code by retrieving the termination code from @c le_mcc_CallRef_t.
 * Please refer to @ref platformConstraintsSpecificErrorCodes for platform specific
 * termination code description.
 *
 * - le_mcc_IsConnected() - connection status.
 * - le_mcc_GetRemoteTel() - displays remote party telephone number associated with the call.
 * - le_mcc_HangUp() will disconnect this call.
 *
 * When finished with the call object, call le_mcc_Delete() to free all the allocated resources
 * associated with the object.
 *
 * Multi-threading/multi-application behaviour: the callRef is linked to a specific client
 * (i.e. connection with the mcc service). Each client will have its own callRef for a same call.
 * That is, if a call event handler is registered by one thread but le_mcc_Create() is called by
 * another thread, they will each get different call references for the same call. So, when multiple
 * threads are being used to work with the same call, a comparison of the call references themselves
 * can't be used to tell whether or not they refer to the same call.
 *
 * The Adaptive Multi Rate (AMR) is an audio compression format optimized for speech coding and used
 * during a voice call. Two AMRs are supported:
 * An AMR Narrowband that encodes a bandwidth of 200–3400 Hz signals at variable bitrates ranging
 * from 4.75 to 12.2 kbit/s and an AMR Wideband that encodes a wider bandwidth of 50–7000 Hz and
 * thus improves the speech quality.
 *
 * le_mcc_SetAmrWbCapability() function enables or disables the AMR Wideband supported capability.
 * le_mcc_GetAmrWbCapability() function returns the AMR Wideband capability status.
 *
 * @section le_mcc_answering_a_call Answering a call
 *
 * Receiving calls is similar sending calls. Add a handler through le_mcc_AddCallEventHandler()
 * to be notified of incoming calls.
 *
 * To answer, call le_mcc_Answer(). To reject it, call le_mcc_HangUp().
 *
 *
 * @section le_mcc_ending_all_call Ending all calls
 *
 * A special function can be used to hang-up all the ongoing calls: le_mcc_HangUpAll().
 * This function can be used to hang-up any calls that have been initiated through another client
 * like AT commands.
 *
 *
 *
 * @section le_mcc_ss Supplementary service
 *
 * Calling Line Identification Restriction (CLIR) can be activated or deactivated by
 * le_mcc_SetCallerIdRestrict() API. The status is independent for each call object reference.
 * Status can be read with the le_mcc_GetCallerIdRestrict() API. If the status is not set,
 * le_mcc_GetCallerIdRestrict() API returns LE_UNAVAILABLE.
 * By default the CLIR status is not set.
 *
 * Call waiting supplementary service can be activated or deactivated by
 * le_mcc_SetCallWaitingService(). Its status can be given by le_mcc_GetCallWaitingService().
 * A call waiting can be answered using le_mcc_ActivateCall() API. This API is also used to
 * activate an on hold call (current call is then placed on hold).
 * An activated, waiting or on hold call can be released using le_mcc_HangUp() function.
 * See 3GPP TS 02.83 / 22.083 for any details concerning call waiting / call hold supplementary
 * services.
 *
 * @todo Other Supplementary services will be available in a future release. Create simpler examples.
 *
 *  @section le_mcc_samples Sample codes
 *
 * A sample code that implements a dialing call can be found in \b le_mccTest.c file (please refer
 * to @ref c_mccTest page).
 *
 *
 * <HR>
 *
 * Copyright (C) Sierra Wireless Inc.
 */
/**
 * @file le_mcc_interface.h
 *
 * Legato @ref c_mcc include file.
 *
 * Copyright (C) Sierra Wireless Inc.
 */
/**
 * @page c_mccTest Sample code of Modem Call control
 *
 * @include "apps/test/modemServices/mcc/mccIntegrationTest/mccTest/le_mccTest.c"
 */

#ifndef LE_MCC_INTERFACE_H_INCLUDE_GUARD
#define LE_MCC_INTERFACE_H_INCLUDE_GUARD


#include "legato.h"

// Interface specific includes
#include "le_mdmDefs_interface.h"


//--------------------------------------------------------------------------------------------------
/**
 * Type for handler called when a server disconnects.
 */
//--------------------------------------------------------------------------------------------------
typedef void (*le_mcc_DisconnectHandler_t)(void *);

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
void le_mcc_ConnectService
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
le_result_t le_mcc_TryConnectService
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
void le_mcc_SetServerDisconnectHandler
(
    le_mcc_DisconnectHandler_t disconnectHandler,
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
void le_mcc_DisconnectService
(
    void
);


//--------------------------------------------------------------------------------------------------
/**
 *  Enumeration of the possible events that may be reported to a call event handler.
 */
//--------------------------------------------------------------------------------------------------
typedef enum
{
    LE_MCC_EVENT_SETUP = 0,
        ///< Call is being set up.
    LE_MCC_EVENT_INCOMING = 1,
        ///< Incoming call attempt (new call).
    LE_MCC_EVENT_ORIGINATING = 2,
        ///< Outgoing call attempt.
    LE_MCC_EVENT_ALERTING = 3,
        ///< Far end is now alerting its user (outgoing call).
    LE_MCC_EVENT_CONNECTED = 4,
        ///< Call has been established, and is media is active.
    LE_MCC_EVENT_TERMINATED = 5,
        ///< Call has terminated.
    LE_MCC_EVENT_WAITING = 6,
        ///< Call is waiting
    LE_MCC_EVENT_ON_HOLD = 7,
        ///< Remote party has put the call on hold.
    LE_MCC_EVENT_MAX = 8
        ///< Enumerate max value.
}
le_mcc_Event_t;


//--------------------------------------------------------------------------------------------------
/**
 *  Enumeration of the possible reasons for call termination.
 */
//--------------------------------------------------------------------------------------------------
typedef enum
{
    LE_MCC_TERM_LOCAL_ENDED = 0,
        ///< Local party ended the call (Normal Call Clearing).
    LE_MCC_TERM_REMOTE_ENDED = 1,
        ///< Remote party ended the call (Normal Call Clearing).
    LE_MCC_TERM_NETWORK_FAIL = 2,
        ///< Network could not complete the call.
    LE_MCC_TERM_UNASSIGNED_NUMBER = 3,
        ///< cf. 3GPP 24.008 Annex H
    LE_MCC_TERM_NO_ROUTE_TO_DESTINATION = 4,
        ///< cf. 3GPP 24.008 Annex H
    LE_MCC_TERM_CHANNEL_UNACCEPTABLE = 5,
        ///< cf. 3GPP 24.008 Annex H
    LE_MCC_TERM_OPERATOR_DETERMINED_BARRING = 6,
        ///< cf. 3GPP 24.008 Annex H
    LE_MCC_TERM_USER_BUSY = 7,
        ///< cf. 3GPP 24.008 Annex H
    LE_MCC_TERM_NO_USER_RESPONDING = 8,
        ///< cf. 3GPP 24.008 Annex H
    LE_MCC_TERM_USER_ALERTING_NO_ANSWER = 9,
        ///< cf. 3GPP 24.008 Annex H
    LE_MCC_TERM_CALL_REJECTED = 10,
        ///< cf. 3GPP 24.008 Annex H
    LE_MCC_TERM_NUMBER_CHANGED = 11,
        ///< cf. 3GPP 24.008 Annex H
    LE_MCC_TERM_PREEMPTION = 12,
        ///< cf. 3GPP 24.008 Annex H
    LE_MCC_TERM_DESTINATION_OUT_OF_ORDER = 13,
        ///< cf. 3GPP 24.008 Annex H
    LE_MCC_TERM_INVALID_NUMBER_FORMAT = 14,
        ///< cf. 3GPP 24.008 Annex H
    LE_MCC_TERM_FACILITY_REJECTED = 15,
        ///< cf. 3GPP 24.008 Annex H
    LE_MCC_TERM_RESP_TO_STATUS_ENQUIRY = 16,
        ///< cf. 3GPP 24.008 Annex H
    LE_MCC_TERM_NORMAL_UNSPECIFIED = 17,
        ///< cf. 3GPP 24.008 Annex H
    LE_MCC_TERM_NO_CIRCUIT_OR_CHANNEL_AVAILABLE = 18,
        ///< cf. 3GPP 24.008 Annex H
    LE_MCC_TERM_NETWORK_OUT_OF_ORDER = 19,
        ///< cf. 3GPP 24.008 Annex H
    LE_MCC_TERM_TEMPORARY_FAILURE = 20,
        ///< cf. 3GPP 24.008 Annex H
    LE_MCC_TERM_SWITCHING_EQUIPMENT_CONGESTION = 21,
        ///< cf. 3GPP 24.008 Annex H
    LE_MCC_TERM_ACCESS_INFORMATION_DISCARDED = 22,
        ///< cf. 3GPP 24.008 Annex H
    LE_MCC_TERM_REQUESTED_CIRCUIT_OR_CHANNEL_NOT_AVAILABLE = 23,
        ///< cf. 3GPP 24.008 Annex H
    LE_MCC_TERM_RESOURCES_UNAVAILABLE_OR_UNSPECIFIED = 24,
        ///< cf. 3GPP 24.008 Annex H
    LE_MCC_TERM_QOS_UNAVAILABLE = 25,
        ///< cf. 3GPP 24.008 Annex H
    LE_MCC_TERM_REQUESTED_FACILITY_NOT_SUBSCRIBED = 26,
        ///< cf. 3GPP 24.008 Annex H
    LE_MCC_TERM_INCOMING_CALLS_BARRED_WITHIN_CUG = 27,
        ///< cf. 3GPP 24.008 Annex H
    LE_MCC_TERM_BEARER_CAPABILITY_NOT_AUTH = 28,
        ///< cf. 3GPP 24.008 Annex H
    LE_MCC_TERM_BEARER_CAPABILITY_UNAVAILABLE = 29,
        ///< cf. 3GPP 24.008 Annex H
    LE_MCC_TERM_SERVICE_OPTION_NOT_AVAILABLE = 30,
        ///< cf. 3GPP 24.008 Annex H
    LE_MCC_TERM_ACM_LIMIT_EXCEEDED = 31,
        ///< cf. 3GPP 24.008 Annex H
    LE_MCC_TERM_BEARER_SERVICE_NOT_IMPLEMENTED = 32,
        ///< cf. 3GPP 24.008 Annex H
    LE_MCC_TERM_REQUESTED_FACILITY_NOT_IMPLEMENTED = 33,
        ///< cf. 3GPP 24.008 Annex H
    LE_MCC_TERM_ONLY_DIGITAL_INFORMATION_BEARER_AVAILABLE = 34,
        ///< cf. 3GPP 24.008 Annex H
    LE_MCC_TERM_SERVICE_OR_OPTION_NOT_IMPLEMENTED = 35,
        ///< cf. 3GPP 24.008 Annex H
    LE_MCC_TERM_INVALID_TRANSACTION_IDENTIFIER = 36,
        ///< cf. 3GPP 24.008 Annex H
    LE_MCC_TERM_USER_NOT_MEMBER_OF_CUG = 37,
        ///< cf. 3GPP 24.008 Annex H
    LE_MCC_TERM_INCOMPATIBLE_DESTINATION = 38,
        ///< cf. 3GPP 24.008 Annex H
    LE_MCC_TERM_INVALID_TRANSIT_NW_SELECTION = 39,
        ///< cf. 3GPP 24.008 Annex H
    LE_MCC_TERM_SEMANTICALLY_INCORRECT_MESSAGE = 40,
        ///< cf. 3GPP 24.008 Annex H
    LE_MCC_TERM_INVALID_MANDATORY_INFORMATION = 41,
        ///< cf. 3GPP 24.008 Annex H
    LE_MCC_TERM_MESSAGE_TYPE_NON_IMPLEMENTED = 42,
        ///< cf. 3GPP 24.008 Annex H
    LE_MCC_TERM_MESSAGE_TYPE_NOT_COMPATIBLE_WITH_PROTOCOL_STATE = 43,
        ///< cf. 3GPP 24.008 Annex H
    LE_MCC_TERM_INFORMATION_ELEMENT_NON_EXISTENT = 44,
        ///< cf. 3GPP 24.008 Annex H
    LE_MCC_TERM_CONDITONAL_IE_ERROR = 45,
        ///< cf. 3GPP 24.008 Annex H
    LE_MCC_TERM_MESSAGE_NOT_COMPATIBLE_WITH_PROTOCOL_STATE = 46,
        ///< cf. 3GPP 24.008 Annex H
    LE_MCC_TERM_RECOVERY_ON_TIMER_EXPIRY = 47,
        ///< cf. 3GPP 24.008 Annex H
    LE_MCC_TERM_PROTOCOL_ERROR_UNSPECIFIED = 48,
        ///< cf. 3GPP 24.008 Annex H
    LE_MCC_TERM_INTERWORKING_UNSPECIFIED = 49,
        ///< cf. 3GPP 24.008 Annex H
    LE_MCC_TERM_SERVICE_TEMPORARILY_OUT_OF_ORDER = 50,
        ///< cf. 3GPP 24.008 10.5.3.6
    LE_MCC_TERM_NOT_ALLOWED = 51,
        ///< Call operations not allowed
        ///<  (i.e. Radio off).
    LE_MCC_TERM_FDN_ACTIVE = 52,
        ///< FDN is active and number is not
        ///< in the FDN.
    LE_MCC_TERM_NO_SERVICE = 53,
        ///< No service or bad signal quality
    LE_MCC_TERM_PLATFORM_SPECIFIC = 54,
        ///< Platform specific code.
    LE_MCC_TERM_UNDEFINED = 55
        ///< Undefined reason.
}
le_mcc_TerminationReason_t;


//--------------------------------------------------------------------------------------------------
/**
 *  Reference type for managing active calls.
 */
//--------------------------------------------------------------------------------------------------
typedef struct le_mcc_Call* le_mcc_CallRef_t;


//--------------------------------------------------------------------------------------------------
/**
 * Reference type used by Add/Remove functions for EVENT 'le_mcc_CallEvent'
 */
//--------------------------------------------------------------------------------------------------
typedef struct le_mcc_CallEventHandler* le_mcc_CallEventHandlerRef_t;


//--------------------------------------------------------------------------------------------------
/**
 * Handler for call state changes.
 *
 * @note The callRef has to be deleted using le_mcc_Delete() when LE_MCC_TERMINATED event
 * is received, except if is has to be used for a future usage (with le_mcc_profile handler for
 * instance, or dialing again the same number).
 *
 */
//--------------------------------------------------------------------------------------------------
typedef void (*le_mcc_CallEventHandlerFunc_t)
(
    le_mcc_CallRef_t callRef,
        ///< The call reference.
    le_mcc_Event_t event,
        ///< Call event.
    void* contextPtr
        ///<
);

//--------------------------------------------------------------------------------------------------
/**
 * Create a call reference.
 *
 * @note Return NULL if call reference can't be created
 *
 * @note If destination number is too long (max LE_MDMDEFS_PHONE_NUM_MAX_LEN digits),
 * it is a fatal error, the function will not return.
 *
 */
//--------------------------------------------------------------------------------------------------
le_mcc_CallRef_t le_mcc_Create
(
    const char* LE_NONNULL phoneNumPtr
        ///< [IN] The target number we are going to
        ///< call.
);

//--------------------------------------------------------------------------------------------------
/**
 * Call to free up a call reference.
 *
 * @return
 *     - LE_OK        The function succeed.
 *     - LE_NOT_FOUND The call reference was not found.
 *     - LE_FAULT      The function failed.
 *
 *
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_mcc_Delete
(
    le_mcc_CallRef_t callRef
        ///< [IN] The call object to free.
);

//--------------------------------------------------------------------------------------------------
/**
 * Start a call attempt.
 *
 * Asynchronous due to possible time to connect.
 *
 * As the call attempt proceeds, the profile's registered call event handler receives events.
 *
 * @return
 *      - LE_OK            Function succeed.
 *      - LE_BUSY          The call is already in progress
 *
 * * @note As this is an asynchronous call, a successful only confirms a call has been
 *       started. Don't assume a call has been successful yet.
 *
 * @note If the caller is passing a bad pointer into this function, it is a fatal error, the
 *       function will not return.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_mcc_Start
(
    le_mcc_CallRef_t callRef
        ///< [IN] Reference to the call object.
);

//--------------------------------------------------------------------------------------------------
/**
 * Allow the caller to know if the given call is actually connected or not.
 *
 * @return TRUE if the call is connected, FALSE otherwise.
 *
 * @note If the caller is passing a bad pointer into this function, it is a fatal error, the
 *       function will not return.
 */
//--------------------------------------------------------------------------------------------------
bool le_mcc_IsConnected
(
    le_mcc_CallRef_t callRef
        ///< [IN] The call reference to read.
);

//--------------------------------------------------------------------------------------------------
/**
 * Read out the remote party telephone number associated with the call.
 *
 * Output parameter is updated with the telephone number. If the Telephone number string length exceeds
 * the value of 'len' parameter, the LE_OVERFLOW error code is returned and 'telPtr' is used until
 * 'len-1' characters and a null-character is implicitly appended at the end of 'telPtr'.
 * Note that 'len' sould be at least equal to LE_MDMDEFS_PHONE_NUM_MAX_BYTES, otherwise LE_OVERFLOW
 * error code will be common.
 *
 * @return LE_OVERFLOW      The Telephone number length exceed the maximum length.
 * @return LE_OK            The function succeeded.
 *
 * @note If the caller is passing a bad pointer into this function, it is a fatal error, the
 *       function will not return.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_mcc_GetRemoteTel
(
    le_mcc_CallRef_t callRef,
        ///< [IN]  The call reference to read from.
    char* telPtr,
        ///< [OUT] The telephone number string.
    size_t telPtrSize
        ///< [IN]
);

//--------------------------------------------------------------------------------------------------
/**
 * Called to get the termination reason.
 *
 * @return The termination reason.
 *
 * @note If the caller is passing a bad pointer into this function, it is a fatal error, the
 *       function will not return.
 */
//--------------------------------------------------------------------------------------------------
le_mcc_TerminationReason_t le_mcc_GetTerminationReason
(
    le_mcc_CallRef_t callRef
        ///< [IN] The call reference to read from.
);

//--------------------------------------------------------------------------------------------------
/**
 * Called to get the platform specific termination code.
 *
 * Refer to @ref platformConstraintsSpecificErrorCodes for platform specific
 * termination code description.
 *
 * @return The platform specific termination code.
 *
 * @note If the caller is passing a bad pointer into this function, it is a fatal error, the
 *       function will not return.
 */
//--------------------------------------------------------------------------------------------------
int32_t le_mcc_GetPlatformSpecificTerminationCode
(
    le_mcc_CallRef_t callRef
        ///< [IN] The call reference to read from.
);

//--------------------------------------------------------------------------------------------------
/**
 *  Answers incoming call.
 *
 * @return LE_TIMEOUT       No response was received from the Modem.
 * @return LE_OK            The function succeeded.
 *
 * @note If the caller is passing a bad pointer into this function, it is a fatal error, the
 *       function will not return.
 * @note A call waiting call must be answered using SetCallActive() API. This API doesn't manage
 * call waiting supplementary service.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_mcc_Answer
(
    le_mcc_CallRef_t callRef
        ///< [IN] The call reference.
);

//--------------------------------------------------------------------------------------------------
/**
 * Disconnect, or hang up, the specifed call. Any active call handlers
 * will be notified.
 *
 * @return LE_FAULT         The function failed.
 * @return LE_TIMEOUT       No response was received from the Modem.
 * @return LE_OK            Function succeeded.
 *
 * @note If the caller is passing a bad pointer into this function, it is a fatal error, the
 *       function will not return.
 * @note this API can also be used to disconnect a waiting or on hold call.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_mcc_HangUp
(
    le_mcc_CallRef_t callRef
        ///< [IN] The call to end.
);

//--------------------------------------------------------------------------------------------------
/**
 * This function will disconnect, or hang up all the ongoing calls. Any active call handlers will
 * be notified.
 *
 * @return LE_FAULT         The function failed.
 * @return LE_TIMEOUT       No response was received from the Modem.
 * @return LE_OK            The function succeeded.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_mcc_HangUpAll
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * This function return the Calling Line Identification Restriction (CLIR) status on the specific
 *  call.
 *
 * The output parameter is updated with the CLIR status.
 *    - LE_ON  Disable presentation of own phone number to remote.
 *    - LE_OFF Enable presentation of own phone number to remote.
 *
 * @return
 *    - LE_OK          The function succeed.
 *    - LE_NOT_FOUND   The call reference was not found.
 *    - LE_UNAVAILABLE CLIR status was not set.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_mcc_GetCallerIdRestrict
(
    le_mcc_CallRef_t callRef,
        ///< [IN] The call reference.
    le_onoff_t* clirStatusPtrPtr
        ///< [OUT] the Calling Line Identification Restriction (CLIR) status
);

//--------------------------------------------------------------------------------------------------
/**
 * This function set the Calling Line Identification Restriction (CLIR) status on the specific call.
 * By default the CLIR status is not set.
 *
 * @return
 *     - LE_OK        The function succeed.
 *     - LE_NOT_FOUND The call reference was not found.
 *
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_mcc_SetCallerIdRestrict
(
    le_mcc_CallRef_t callRef,
        ///< [IN] The call reference.
    le_onoff_t clirStatus
        ///< [IN] The Calling Line Identification Restriction (CLIR) status.
);

//--------------------------------------------------------------------------------------------------
/**
 * Add handler function for EVENT 'le_mcc_CallEvent'
 *
 * Register an event handler that will be notified when an call's event occurs.
 *
 * @return A reference to the new event handler object.
 *
 * @note It is a fatal error if this function does succeed.  If this function fails, it will not
 *       return.
 *
 */
//--------------------------------------------------------------------------------------------------
le_mcc_CallEventHandlerRef_t le_mcc_AddCallEventHandler
(
    le_mcc_CallEventHandlerFunc_t handlerPtr,
        ///< [IN]
    void* contextPtr
        ///< [IN]
);

//--------------------------------------------------------------------------------------------------
/**
 * Remove handler function for EVENT 'le_mcc_CallEvent'
 */
//--------------------------------------------------------------------------------------------------
void le_mcc_RemoveCallEventHandler
(
    le_mcc_CallEventHandlerRef_t handlerRef
        ///< [IN]
);

//--------------------------------------------------------------------------------------------------
/**
 * This function activates or deactivates the call waiting service.
 *
 * @return
 *     - LE_OK        The function succeed.
 *     - LE_FAULT     The function failed.
 *
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_mcc_SetCallWaitingService
(
    bool active
        ///< [IN] The call waiting activation.
);

//--------------------------------------------------------------------------------------------------
/**
 * This function gets the call waiting service status.
 *
 * @return
 *     - LE_OK        The function succeed.
 *     - LE_FAULT     The function failed.
 *
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_mcc_GetCallWaitingService
(
    bool* activePtr
        ///< [OUT] The call waiting activation.
);

//--------------------------------------------------------------------------------------------------
/**
 * This function activates the specified call. Other calls are placed on hold.
 *
 * @return
 *     - LE_OK        The function succeed.
 *     - LE_FAULT     The function failed.
 *
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_mcc_ActivateCall
(
    le_mcc_CallRef_t callRef
        ///< [IN] The call reference.
);

//--------------------------------------------------------------------------------------------------
/**
 * This function enables/disables the audio AMR Wideband capability.
 *
 * @return
 *     -  LE_OK             The function succeeded.
 *     -  LE_UNAVAILABLE    The service is not available.
 *     -  LE_FAULT          On any other failure.
 *
 * @note The capability setting takes effect immediately and is not persistent to reset.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_mcc_SetAmrWbCapability
(
    bool enable
        ///< [IN] True enables the AMR Wideband capability, false disables it.
);

//--------------------------------------------------------------------------------------------------
/**
 * This function gets the audio AMR Wideband capability.
 *
 * @return
 *     - LE_OK            The function succeeded.
 *     - LE_UNAVAILABLE   The service is not available.
 *     - LE_FAULT         On any other failure.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_mcc_GetAmrWbCapability
(
    bool* enabledPtr
        ///< [OUT] True if AMR Wideband capability is enabled, false otherwise.
);

#endif // LE_MCC_INTERFACE_H_INCLUDE_GUARD