

/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */

/**
 * @page c_le_voicecall Voice Call Service
 *
 * @ref le_voicecall_interface.h "API Reference"
 *
 * <HR>
 *
 * A voice call is needed for applications that want to establish a voice communication with
 *  a remote party.  The voice call can be over a mobile network, or over VoIP.
 *
 * @section le_voicecall_binding IPC interfaces binding
 *
 * All the functions of this API are provided by the @b voiceCallService application service.
 *
 * Here's a code sample binding to Voice Call services:
 * @verbatim
   bindings:
   {
      clientExe.clientComponent.le_voicecall -> voiceCallService.le_voicecall
   }
   @endverbatim
 *
 * @section c_le_voicecall_outgoing Starting a Voice call
 *
 * A voice call can be started using le_voicecall_Start() with the destination identifier passed as
 *  a parameter.
 *
 * @note Available interfaces depend on used platform.
 *
 * Before the voice call is started, an application registers a state handler using
 * le_voicecall_AddStateHandler(). Once the voice call is established, the handler will be called
 *  indicating it's now connected. If the state of the voice call changes, then the handler will be
 *  called with the new state. To release a voice call, an application can use le_voicecall_End().
 * Application must use le_voicecall_Delete() to release @ref le_voicecall_CallRef_t voice call
 *  reference object when it is no more used.
 *
 * If le_voicecall_End() failed a @ref LE_VOICECALL_EVENT_END_FAILED event will be sent.
 *
 * If a voice call is already started when le_voicecall_Start() is called(), a
 * new voice call will not be established. Instead, @ref LE_VOICECALL_EVENT_RESOURCE_BUSY event
 *  will be sent. This event means call was not processed, while a
 *  @ref LE_VOICECALL_EVENT_TERMINATED event means that the call was not processed and then
 *  terminated or failed.
 *
 * Once an application makes a voice call request, it should monitor the establishment state
 * reported to the registered state handler.
 *
 * Once the @ref LE_VOICECALL_EVENT_CONNECTED voice call event is received by the application, it
 *  must get the Rx and Tx audio streams with le_voicecall_GetRxAudioStream() and
 *  le_voicecall_GetTxAudioStream() functions in order to set up the audio path. The audio path
 *  can be set up thanks to the audio API (cf. @ref c_audio).
 *
 * If a @ref LE_VOICECALL_EVENT_TERMINATED event is received, application can get the termination
 *  reason by using le_voicecall_GetTerminationReason().
 *
 * @note The voice call use the mobile network. VoIP is not yet supported.
 *
 * @section c_le_voicecall_incoming Answering a Voice call
 *
 * An Incoming voice call will be notified by an @ref LE_VOICECALL_EVENT_INCOMING event on state
 *  handler with a Call reference le_voicecall_CallRef_t().
 *
 * Application can answer the call by using le_voicecall_Answer() or reject the call by using
 *  le_voicecall_End() passing the call reference @ref le_voicecall_CallRef_t.
 *
 * If le_voicecall_End() failed a @ref LE_VOICECALL_EVENT_END_FAILED event will be sent. If
 *  le_voicecall_Answer() failed a @ref LE_VOICECALL_EVENT_ANSWER_FAILED event will be sent.
 *
 * Application have to use le_voicecall_Delete() to release @ref le_voicecall_CallRef_t voice call
 *  reference object when it is no more used.
 *
 * <HR>
 *
 * Copyright (C) Sierra Wireless Inc.
 */
/**
 * @file le_voicecall_interface.h
 *
 * Legato @ref c_le_voicecall include file.
 *
 * Copyright (C) Sierra Wireless Inc.
 */

#ifndef LE_VOICECALL_INTERFACE_H_INCLUDE_GUARD
#define LE_VOICECALL_INTERFACE_H_INCLUDE_GUARD


#include "legato.h"

// Interface specific includes
#include "le_mdmDefs_interface.h"
#include "le_audio_interface.h"


//--------------------------------------------------------------------------------------------------
/**
 * Type for handler called when a server disconnects.
 */
//--------------------------------------------------------------------------------------------------
typedef void (*le_voicecall_DisconnectHandler_t)(void *);

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
void le_voicecall_ConnectService
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
le_result_t le_voicecall_TryConnectService
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
void le_voicecall_SetServerDisconnectHandler
(
    le_voicecall_DisconnectHandler_t disconnectHandler,
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
void le_voicecall_DisconnectService
(
    void
);


//--------------------------------------------------------------------------------------------------
/**
 * Reference returned by Start function and used by End function
 */
//--------------------------------------------------------------------------------------------------
typedef struct le_voicecall_Call* le_voicecall_CallRef_t;


//--------------------------------------------------------------------------------------------------
/**
 * Voice call establishment states.
 */
//--------------------------------------------------------------------------------------------------
typedef enum
{
    LE_VOICECALL_EVENT_ALERTING = 0,
        ///< Voice call establishment in progress.
        ///< Far end is now alerting its user (outgoing call).
    LE_VOICECALL_EVENT_CONNECTED = 1,
        ///< Call has been established, and is media is active.
    LE_VOICECALL_EVENT_TERMINATED = 2,
        ///< Call has terminated.
    LE_VOICECALL_EVENT_OFFLINE = 3,
        ///< NO Service available to try establish a voice call.
    LE_VOICECALL_EVENT_BUSY = 4,
        ///< Remote party (callee) is busy.
    LE_VOICECALL_EVENT_RESOURCE_BUSY = 5,
        ///< All local connection resources (lines/channels) are in use.
    LE_VOICECALL_EVENT_CALL_END_FAILED = 6,
        ///< Call ending failed.
    LE_VOICECALL_EVENT_CALL_ANSWER_FAILED = 7,
        ///< Call answering failed.
    LE_VOICECALL_EVENT_INCOMING = 8
        ///< Incoming voice call in progress.
}
le_voicecall_Event_t;


//--------------------------------------------------------------------------------------------------
/**
 * Voice call termination reason.
 */
//--------------------------------------------------------------------------------------------------
typedef enum
{
    LE_VOICECALL_TERM_NETWORK_FAIL = 0,
        ///< Network could not complete the call.
    LE_VOICECALL_TERM_BAD_ADDRESS = 1,
        ///< Remote address could not be resolved.
    LE_VOICECALL_TERM_BUSY = 2,
        ///< Caller is currently busy and cannot take the call.
    LE_VOICECALL_TERM_LOCAL_ENDED = 3,
        ///< Local party ended the call.
    LE_VOICECALL_TERM_REMOTE_ENDED = 4,
        ///< Remote party ended the call.
    LE_VOICECALL_TERM_UNDEFINED = 5
        ///< Undefined reason.
}
le_voicecall_TerminationReason_t;


//--------------------------------------------------------------------------------------------------
/**
 * Reference type used by Add/Remove functions for EVENT 'le_voicecall_State'
 */
//--------------------------------------------------------------------------------------------------
typedef struct le_voicecall_StateHandler* le_voicecall_StateHandlerRef_t;


//--------------------------------------------------------------------------------------------------
/**
 * Handler for voice call state changes.
 */
//--------------------------------------------------------------------------------------------------
typedef void (*le_voicecall_StateHandlerFunc_t)
(
    le_voicecall_CallRef_t reference,
        ///< Event voice call object reference.
    const char* LE_NONNULL identifier,
        ///< Identifier of the remote party
        ///< associated with the call.
    le_voicecall_Event_t event,
        ///< Voice call event.
    void* contextPtr
        ///<
);

//--------------------------------------------------------------------------------------------------
/**
 * Add handler function for EVENT 'le_voicecall_State'
 *
 * This event provides information on voice call state changes
 *
 */
//--------------------------------------------------------------------------------------------------
le_voicecall_StateHandlerRef_t le_voicecall_AddStateHandler
(
    le_voicecall_StateHandlerFunc_t handlerPtr,
        ///< [IN]
    void* contextPtr
        ///< [IN]
);

//--------------------------------------------------------------------------------------------------
/**
 * Remove handler function for EVENT 'le_voicecall_State'
 */
//--------------------------------------------------------------------------------------------------
void le_voicecall_RemoveStateHandler
(
    le_voicecall_StateHandlerRef_t handlerRef
        ///< [IN]
);

//--------------------------------------------------------------------------------------------------
/**
 * Start a voice call.
 *
 * @return
 *      - Reference to the voice call (to be used later for releasing the voice call)
 *      - NULL if the voice call could not be processed
 */
//--------------------------------------------------------------------------------------------------
le_voicecall_CallRef_t le_voicecall_Start
(
    const char* LE_NONNULL DestinationID
        ///< [IN] Destination identifier for the voice
        ///< call establishment.
);

//--------------------------------------------------------------------------------------------------
/**
 * Release a voice call.
 *
 * @return
 *      - LE_OK if the end of voice call can be processed.
 *      - LE_NOT_FOUND if the voice call object reference is not found.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_voicecall_End
(
    le_voicecall_CallRef_t reference
        ///< [IN] Voice call object reference to hang-up.
);

//--------------------------------------------------------------------------------------------------
/**
 * Delete voice call object reference create by le_voicecall_Start() or an incoming voice call.
 *
 * @return
 *      - LE_OK if the delete of voice call can be processed.
 *      - LE_FAULT if the voice call is not terminated.
 *      - LE_NOT_FOUND if the voice call object reference is not found.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_voicecall_Delete
(
    le_voicecall_CallRef_t reference
        ///< [IN] Voice call object reference to delete.
);

//--------------------------------------------------------------------------------------------------
/**
 * Answer to incoming voice call.
 *
 * @return
 *      - LE_OK if the incoming voice call can be answered
 *      - LE_NOT_FOUND if the incoming voice call object reference is not found.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_voicecall_Answer
(
    le_voicecall_CallRef_t reference
        ///< [IN] Incoming voice call object reference to answer.
);

//--------------------------------------------------------------------------------------------------
/**
 * Get the termination reason of a voice call reference.
 *
 * @return
 *      - LE_OK if the termination reason is got
 *      - LE_NOT_FOUND if the incoming voice call object reference is not found.
 *      - LE_FAULT if the voice call is not terminated.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_voicecall_GetTerminationReason
(
    le_voicecall_CallRef_t reference,
        ///< [IN] Voice call object reference to read from.
    le_voicecall_TerminationReason_t* reasonPtr
        ///< [OUT] Termination reason of the voice call.
);

//--------------------------------------------------------------------------------------------------
/**
 * Called to get the transmitted audio stream. All audio generated on this
 * end of the call is sent on this stream.
 *
 * @return Transmitted audio stream reference.
 */
//--------------------------------------------------------------------------------------------------
le_audio_StreamRef_t le_voicecall_GetTxAudioStream
(
    le_voicecall_CallRef_t reference
        ///< [IN] Voice call object reference to read from.
);

//--------------------------------------------------------------------------------------------------
/**
 * Called to get the received audio stream. All audio received from the
 * other end of the call is received on this stream.
 *
 * @return Received audio stream reference.
 */
//--------------------------------------------------------------------------------------------------
le_audio_StreamRef_t le_voicecall_GetRxAudioStream
(
    le_voicecall_CallRef_t reference
        ///< [IN] Voice call object reference to read from.
);

#endif // LE_VOICECALL_INTERFACE_H_INCLUDE_GUARD