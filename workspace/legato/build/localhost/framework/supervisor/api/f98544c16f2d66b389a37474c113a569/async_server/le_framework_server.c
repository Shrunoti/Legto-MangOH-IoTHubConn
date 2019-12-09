
/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */


#include "le_framework_messages.h"
#include "le_framework_server.h"


//--------------------------------------------------------------------------------------------------
// Generic Server Types, Variables and Functions
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
/**
 * Type definition for generic function to remove a handler, given the handler ref.
 */
//--------------------------------------------------------------------------------------------------
typedef void(* RemoveHandlerFunc_t)(void *handlerRef);


//--------------------------------------------------------------------------------------------------
/**
 * Server Data Objects
 *
 * This object is used to store additional context info for each request
 */
//--------------------------------------------------------------------------------------------------
typedef struct
{
    le_msg_SessionRef_t   clientSessionRef;     ///< The client to send the response to
    void*                 contextPtr;           ///< ContextPtr registered with handler
    le_event_HandlerRef_t handlerRef;           ///< HandlerRef for the registered handler
    RemoveHandlerFunc_t   removeHandlerFunc;    ///< Function to remove the registered handler
}
_ServerData_t;

//--------------------------------------------------------------------------------------------------
/**
 * Server command object.
 *
 * This object is used to store additional information about a command
 */
//--------------------------------------------------------------------------------------------------
typedef struct le_framework_ServerCmd
{
    le_msg_MessageRef_t msgRef;           ///< Reference to the message
    le_dls_Link_t cmdLink;                ///< Link to server cmd objects
    uint32_t requiredOutputs;           ///< Outputs which must be sent (if any)
} le_framework_ServerCmd_t;

//--------------------------------------------------------------------------------------------------
/**
 * The memory pool for server data objects
 */
//--------------------------------------------------------------------------------------------------
static le_mem_PoolRef_t _ServerDataPool;


//--------------------------------------------------------------------------------------------------
/**
 * Safe Reference Map for use with Add/Remove handler references
 *
 * @warning Use _Mutex, defined below, to protect accesses to this data.
 */
//--------------------------------------------------------------------------------------------------
static le_ref_MapRef_t _HandlerRefMap;

//--------------------------------------------------------------------------------------------------
/**
 * The memory pool for server command objects
 */
//--------------------------------------------------------------------------------------------------
static le_mem_PoolRef_t _ServerCmdPool;

//--------------------------------------------------------------------------------------------------
/**
 * Mutex and associated macros for use with the above HandlerRefMap.
 *
 * Unused attribute is needed because this variable may not always get used.
 */
//--------------------------------------------------------------------------------------------------
__attribute__((unused)) static pthread_mutex_t _Mutex = PTHREAD_MUTEX_INITIALIZER;   // POSIX "Fast" mutex.

/// Locks the mutex.
#define _LOCK    LE_ASSERT(pthread_mutex_lock(&_Mutex) == 0);

/// Unlocks the mutex.
#define _UNLOCK  LE_ASSERT(pthread_mutex_unlock(&_Mutex) == 0);


//--------------------------------------------------------------------------------------------------
/**
 * Forward declaration needed by StartServer
 */
//--------------------------------------------------------------------------------------------------
static void ServerMsgRecvHandler
(
    le_msg_MessageRef_t msgRef,
    void*               contextPtr
);


//--------------------------------------------------------------------------------------------------
/**
 * Server Service Reference
 */
//--------------------------------------------------------------------------------------------------
static le_msg_ServiceRef_t _ServerServiceRef;


//--------------------------------------------------------------------------------------------------
/**
 * Server Thread Reference
 *
 * Reference to the thread that is registered to provide this service.
 */
//--------------------------------------------------------------------------------------------------
static le_thread_Ref_t _ServerThreadRef;


//--------------------------------------------------------------------------------------------------
/**
 * Client Session Reference for the current message received from a client
 */
//--------------------------------------------------------------------------------------------------
static le_msg_SessionRef_t _ClientSessionRef;

//--------------------------------------------------------------------------------------------------
/**
 * Trace reference used for controlling tracing in this module.
 */
//--------------------------------------------------------------------------------------------------
#if defined(MK_TOOLS_BUILD) && !defined(NO_LOG_SESSION)

static le_log_TraceRef_t TraceRef;

/// Macro used to generate trace output in this module.
/// Takes the same parameters as LE_DEBUG() et. al.
#define TRACE(...) LE_TRACE(TraceRef, ##__VA_ARGS__)

/// Macro used to query current trace state in this module
#define IS_TRACE_ENABLED LE_IS_TRACE_ENABLED(TraceRef)

#else

#define TRACE(...)
#define IS_TRACE_ENABLED 0

#endif

//--------------------------------------------------------------------------------------------------
/**
 * Cleanup client data if the client is no longer connected
 */
//--------------------------------------------------------------------------------------------------
static void CleanupClientData
(
    le_msg_SessionRef_t sessionRef,
    void *contextPtr
)
{
    LE_DEBUG("Client %p is closed !!!", sessionRef);

    // Iterate over the server data reference map and remove anything that matches
    // the client session.
    _LOCK

    // Store the client session ref so it can be retrieved by the server using the
    // GetClientSessionRef() function, if it's needed inside handler removal functions.
    _ClientSessionRef = sessionRef;

    le_ref_IterRef_t iterRef = le_ref_GetIterator(_HandlerRefMap);
    le_result_t result = le_ref_NextNode(iterRef);
    _ServerData_t const* serverDataPtr;

    while ( result == LE_OK )
    {
        serverDataPtr =  le_ref_GetValue(iterRef);

        if ( sessionRef != serverDataPtr->clientSessionRef )
        {
            LE_DEBUG("Found session ref %p; does not match",
                     serverDataPtr->clientSessionRef);
        }
        else
        {
            LE_DEBUG("Found session ref %p; match found, so needs cleanup",
                     serverDataPtr->clientSessionRef);

            // Remove the handler, if the Remove handler functions exists.
            if ( serverDataPtr->removeHandlerFunc != NULL )
            {
                serverDataPtr->removeHandlerFunc( serverDataPtr->handlerRef );
            }

            // Release the server data block
            le_mem_Release((void*)serverDataPtr);

            // Delete the associated safeRef
            le_ref_DeleteRef( _HandlerRefMap, (void*)le_ref_GetSafeRef(iterRef) );
        }

        // Get the next value in the reference mpa
        result = le_ref_NextNode(iterRef);
    }

    // Clear the client session ref, since the event has now been processed.
    _ClientSessionRef = 0;

    _UNLOCK
}


//--------------------------------------------------------------------------------------------------
/**
 * Send the message to the client (queued version)
 *
 * This is a wrapper around le_msg_Send() with an extra parameter so that it can be used
 * with le_event_QueueFunctionToThread().
 */
//--------------------------------------------------------------------------------------------------
__attribute__((unused)) static void SendMsgToClientQueued
(
    void*  msgRef,  ///< [in] Reference to the message.
    void*  unused   ///< [in] Not used
)
{
    le_msg_Send(msgRef);
}


//--------------------------------------------------------------------------------------------------
/**
 * Send the message to the client.
 */
//--------------------------------------------------------------------------------------------------
__attribute__((unused)) static void SendMsgToClient
(
    le_msg_MessageRef_t msgRef      ///< [in] Reference to the message.
)
{
    /*
     * If called from a thread other than the server thread, queue the message onto the server
     * thread.  This is necessary to allow async response/handler functions to be called from any
     * thread, whereas messages to the client can only be sent from the server thread.
     */
    if ( le_thread_GetCurrent() != _ServerThreadRef )
    {
        le_event_QueueFunctionToThread(_ServerThreadRef,
                                       SendMsgToClientQueued,
                                       msgRef,
                                       NULL);
    }
    else
    {
        le_msg_Send(msgRef);
    }
}


//--------------------------------------------------------------------------------------------------
/**
 * Get the server service reference
 */
//--------------------------------------------------------------------------------------------------
le_msg_ServiceRef_t le_framework_GetServiceRef
(
    void
)
{
    return _ServerServiceRef;
}


//--------------------------------------------------------------------------------------------------
/**
 * Get the client session reference for the current message
 */
//--------------------------------------------------------------------------------------------------
le_msg_SessionRef_t le_framework_GetClientSessionRef
(
    void
)
{
    return _ClientSessionRef;
}


//--------------------------------------------------------------------------------------------------
/**
 * Initialize the server and advertise the service.
 */
//--------------------------------------------------------------------------------------------------
void le_framework_AdvertiseService
(
    void
)
{
    LE_DEBUG("======= Starting Server %s ========", SERVICE_INSTANCE_NAME);

    // Get a reference to the trace keyword that is used to control tracing in this module.
#if defined(MK_TOOLS_BUILD) && !defined(NO_LOG_SESSION)
    TraceRef = le_log_GetTraceRef("ipc");
#endif
    le_msg_ProtocolRef_t protocolRef;

    // Create the server data pool
    _ServerDataPool = le_mem_CreatePool("le_framework_ServerData", sizeof(_ServerData_t));

    // Create the server command pool
    _ServerCmdPool = le_mem_CreatePool("le_framework_ServerCmd", sizeof(le_framework_ServerCmd_t));

    // Create safe reference map for handler references.
    // The size of the map should be based on the number of handlers defined for the server.
    // Don't expect that to be more than 2-3, so use 3 as a reasonable guess.
    _HandlerRefMap = le_ref_CreateMap("le_framework_ServerHandlers", 3);

    // Start the server side of the service
    protocolRef = le_msg_GetProtocolRef(PROTOCOL_ID_STR, sizeof(_Message_t));
    _ServerServiceRef = le_msg_CreateService(protocolRef, SERVICE_INSTANCE_NAME);
    le_msg_SetServiceRecvHandler(_ServerServiceRef, ServerMsgRecvHandler, NULL);
    le_msg_AdvertiseService(_ServerServiceRef);

    // Register for client sessions being closed
    le_msg_AddServiceCloseHandler(_ServerServiceRef, CleanupClientData, NULL);

    // Need to keep track of the thread that is registered to provide this service.
    _ServerThreadRef = le_thread_GetCurrent();
}


//--------------------------------------------------------------------------------------------------
// Client Specific Server Code
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
/**
 * Server-side respond function for le_framework_Stop
 */
//--------------------------------------------------------------------------------------------------
void le_framework_StopRespond
(
    le_framework_ServerCmdRef_t _cmdRef,
    le_result_t _result
)
{
    LE_ASSERT(_cmdRef != NULL);

    // Get the message related data
    le_msg_MessageRef_t _msgRef = _cmdRef->msgRef;
    _Message_t* _msgPtr = le_msg_GetPayloadPtr(_msgRef);
    __attribute__((unused)) uint8_t* _msgBufPtr = _msgPtr->buffer;
    __attribute__((unused)) size_t _msgBufSize = _MAX_MSG_SIZE;

    // Ensure the passed in msgRef is for the correct message
    LE_ASSERT(_msgPtr->id == _MSGID_le_framework_Stop);

    // Ensure that this Respond function has not already been called
    LE_FATAL_IF( !le_msg_NeedsResponse(_msgRef), "Response has already been sent");

    // Pack the result first
    LE_ASSERT(le_pack_PackResult( &_msgBufPtr, &_msgBufSize,
                                                    _result ));

    // Null-out any parameters which are not required so pack knows not to pack them.

    // Pack any "out" parameters

    // Return the response
    TRACE("Sending response to client session %p", le_msg_GetSession(_msgRef));

    le_msg_Respond(_msgRef);

    // Release the command
    le_mem_Release(_cmdRef);
}

static void Handle_le_framework_Stop
(
    le_msg_MessageRef_t _msgRef
)
{
    // Create a server command object
    le_framework_ServerCmd_t* _serverCmdPtr = le_mem_ForceAlloc(_ServerCmdPool);
    _serverCmdPtr->cmdLink = LE_DLS_LINK_INIT;
    _serverCmdPtr->msgRef = _msgRef;

    // Get the message buffer pointer
    __attribute__((unused)) uint8_t* _msgBufPtr =
        ((_Message_t*)le_msg_GetPayloadPtr(_msgRef))->buffer;
    __attribute__((unused)) size_t _msgBufSize = _MAX_MSG_SIZE;

    // Unpack which outputs are needed.
    _serverCmdPtr->requiredOutputs = 0;

    // Unpack the input parameters from the message

    // Call the function
    le_framework_Stop ( _serverCmdPtr );

    return;
}


//--------------------------------------------------------------------------------------------------
/**
 * Server-side respond function for le_framework_Restart
 */
//--------------------------------------------------------------------------------------------------
void le_framework_RestartRespond
(
    le_framework_ServerCmdRef_t _cmdRef,
    le_result_t _result
)
{
    LE_ASSERT(_cmdRef != NULL);

    // Get the message related data
    le_msg_MessageRef_t _msgRef = _cmdRef->msgRef;
    _Message_t* _msgPtr = le_msg_GetPayloadPtr(_msgRef);
    __attribute__((unused)) uint8_t* _msgBufPtr = _msgPtr->buffer;
    __attribute__((unused)) size_t _msgBufSize = _MAX_MSG_SIZE;

    // Ensure the passed in msgRef is for the correct message
    LE_ASSERT(_msgPtr->id == _MSGID_le_framework_Restart);

    // Ensure that this Respond function has not already been called
    LE_FATAL_IF( !le_msg_NeedsResponse(_msgRef), "Response has already been sent");

    // Pack the result first
    LE_ASSERT(le_pack_PackResult( &_msgBufPtr, &_msgBufSize,
                                                    _result ));

    // Null-out any parameters which are not required so pack knows not to pack them.

    // Pack any "out" parameters

    // Return the response
    TRACE("Sending response to client session %p", le_msg_GetSession(_msgRef));

    le_msg_Respond(_msgRef);

    // Release the command
    le_mem_Release(_cmdRef);
}

static void Handle_le_framework_Restart
(
    le_msg_MessageRef_t _msgRef
)
{
    // Create a server command object
    le_framework_ServerCmd_t* _serverCmdPtr = le_mem_ForceAlloc(_ServerCmdPool);
    _serverCmdPtr->cmdLink = LE_DLS_LINK_INIT;
    _serverCmdPtr->msgRef = _msgRef;

    // Get the message buffer pointer
    __attribute__((unused)) uint8_t* _msgBufPtr =
        ((_Message_t*)le_msg_GetPayloadPtr(_msgRef))->buffer;
    __attribute__((unused)) size_t _msgBufSize = _MAX_MSG_SIZE;

    // Unpack which outputs are needed.
    _serverCmdPtr->requiredOutputs = 0;

    // Unpack the input parameters from the message
    bool manualRestart;
    if (!le_pack_UnpackBool( &_msgBufPtr, &_msgBufSize,
                                               &manualRestart ))
    {
        goto error_unpack;
    }

    // Call the function
    le_framework_Restart ( _serverCmdPtr, manualRestart );

    return;

error_unpack:
    le_mem_Release(_serverCmdPtr);

    LE_KILL_CLIENT("Error unpacking inputs");
}


//--------------------------------------------------------------------------------------------------
/**
 * Server-side respond function for le_framework_IsStopping
 */
//--------------------------------------------------------------------------------------------------
void le_framework_IsStoppingRespond
(
    le_framework_ServerCmdRef_t _cmdRef,
    bool _result
)
{
    LE_ASSERT(_cmdRef != NULL);

    // Get the message related data
    le_msg_MessageRef_t _msgRef = _cmdRef->msgRef;
    _Message_t* _msgPtr = le_msg_GetPayloadPtr(_msgRef);
    __attribute__((unused)) uint8_t* _msgBufPtr = _msgPtr->buffer;
    __attribute__((unused)) size_t _msgBufSize = _MAX_MSG_SIZE;

    // Ensure the passed in msgRef is for the correct message
    LE_ASSERT(_msgPtr->id == _MSGID_le_framework_IsStopping);

    // Ensure that this Respond function has not already been called
    LE_FATAL_IF( !le_msg_NeedsResponse(_msgRef), "Response has already been sent");

    // Pack the result first
    LE_ASSERT(le_pack_PackBool( &_msgBufPtr, &_msgBufSize,
                                                    _result ));

    // Null-out any parameters which are not required so pack knows not to pack them.

    // Pack any "out" parameters

    // Return the response
    TRACE("Sending response to client session %p", le_msg_GetSession(_msgRef));

    le_msg_Respond(_msgRef);

    // Release the command
    le_mem_Release(_cmdRef);
}

static void Handle_le_framework_IsStopping
(
    le_msg_MessageRef_t _msgRef
)
{
    // Create a server command object
    le_framework_ServerCmd_t* _serverCmdPtr = le_mem_ForceAlloc(_ServerCmdPool);
    _serverCmdPtr->cmdLink = LE_DLS_LINK_INIT;
    _serverCmdPtr->msgRef = _msgRef;

    // Get the message buffer pointer
    __attribute__((unused)) uint8_t* _msgBufPtr =
        ((_Message_t*)le_msg_GetPayloadPtr(_msgRef))->buffer;
    __attribute__((unused)) size_t _msgBufSize = _MAX_MSG_SIZE;

    // Unpack which outputs are needed.
    _serverCmdPtr->requiredOutputs = 0;

    // Unpack the input parameters from the message

    // Call the function
    le_framework_IsStopping ( _serverCmdPtr );

    return;
}


//--------------------------------------------------------------------------------------------------
/**
 * Server-side respond function for le_framework_IsReadOnly
 */
//--------------------------------------------------------------------------------------------------
void le_framework_IsReadOnlyRespond
(
    le_framework_ServerCmdRef_t _cmdRef,
    bool _result
)
{
    LE_ASSERT(_cmdRef != NULL);

    // Get the message related data
    le_msg_MessageRef_t _msgRef = _cmdRef->msgRef;
    _Message_t* _msgPtr = le_msg_GetPayloadPtr(_msgRef);
    __attribute__((unused)) uint8_t* _msgBufPtr = _msgPtr->buffer;
    __attribute__((unused)) size_t _msgBufSize = _MAX_MSG_SIZE;

    // Ensure the passed in msgRef is for the correct message
    LE_ASSERT(_msgPtr->id == _MSGID_le_framework_IsReadOnly);

    // Ensure that this Respond function has not already been called
    LE_FATAL_IF( !le_msg_NeedsResponse(_msgRef), "Response has already been sent");

    // Pack the result first
    LE_ASSERT(le_pack_PackBool( &_msgBufPtr, &_msgBufSize,
                                                    _result ));

    // Null-out any parameters which are not required so pack knows not to pack them.

    // Pack any "out" parameters

    // Return the response
    TRACE("Sending response to client session %p", le_msg_GetSession(_msgRef));

    le_msg_Respond(_msgRef);

    // Release the command
    le_mem_Release(_cmdRef);
}

static void Handle_le_framework_IsReadOnly
(
    le_msg_MessageRef_t _msgRef
)
{
    // Create a server command object
    le_framework_ServerCmd_t* _serverCmdPtr = le_mem_ForceAlloc(_ServerCmdPool);
    _serverCmdPtr->cmdLink = LE_DLS_LINK_INIT;
    _serverCmdPtr->msgRef = _msgRef;

    // Get the message buffer pointer
    __attribute__((unused)) uint8_t* _msgBufPtr =
        ((_Message_t*)le_msg_GetPayloadPtr(_msgRef))->buffer;
    __attribute__((unused)) size_t _msgBufSize = _MAX_MSG_SIZE;

    // Unpack which outputs are needed.
    _serverCmdPtr->requiredOutputs = 0;

    // Unpack the input parameters from the message

    // Call the function
    le_framework_IsReadOnly ( _serverCmdPtr );

    return;
}


//--------------------------------------------------------------------------------------------------
/**
 * Server-side respond function for le_framework_NotifyExpectedReboot
 */
//--------------------------------------------------------------------------------------------------
void le_framework_NotifyExpectedRebootRespond
(
    le_framework_ServerCmdRef_t _cmdRef
)
{
    LE_ASSERT(_cmdRef != NULL);

    // Get the message related data
    le_msg_MessageRef_t _msgRef = _cmdRef->msgRef;
    _Message_t* _msgPtr = le_msg_GetPayloadPtr(_msgRef);
    __attribute__((unused)) uint8_t* _msgBufPtr = _msgPtr->buffer;
    __attribute__((unused)) size_t _msgBufSize = _MAX_MSG_SIZE;

    // Ensure the passed in msgRef is for the correct message
    LE_ASSERT(_msgPtr->id == _MSGID_le_framework_NotifyExpectedReboot);

    // Ensure that this Respond function has not already been called
    LE_FATAL_IF( !le_msg_NeedsResponse(_msgRef), "Response has already been sent");

    // Null-out any parameters which are not required so pack knows not to pack them.

    // Pack any "out" parameters

    // Return the response
    TRACE("Sending response to client session %p", le_msg_GetSession(_msgRef));

    le_msg_Respond(_msgRef);

    // Release the command
    le_mem_Release(_cmdRef);
}

static void Handle_le_framework_NotifyExpectedReboot
(
    le_msg_MessageRef_t _msgRef
)
{
    // Create a server command object
    le_framework_ServerCmd_t* _serverCmdPtr = le_mem_ForceAlloc(_ServerCmdPool);
    _serverCmdPtr->cmdLink = LE_DLS_LINK_INIT;
    _serverCmdPtr->msgRef = _msgRef;

    // Get the message buffer pointer
    __attribute__((unused)) uint8_t* _msgBufPtr =
        ((_Message_t*)le_msg_GetPayloadPtr(_msgRef))->buffer;
    __attribute__((unused)) size_t _msgBufSize = _MAX_MSG_SIZE;

    // Unpack which outputs are needed.
    _serverCmdPtr->requiredOutputs = 0;

    // Unpack the input parameters from the message

    // Call the function
    le_framework_NotifyExpectedReboot ( _serverCmdPtr );

    return;
}


static void ServerMsgRecvHandler
(
    le_msg_MessageRef_t msgRef,
    void*               contextPtr
)
{
    // Get the message payload so that we can get the message "id"
    _Message_t* msgPtr = le_msg_GetPayloadPtr(msgRef);

    // Get the client session ref for the current message.  This ref is used by the server to
    // get info about the client process, such as user id.  If there are multiple clients, then
    // the session ref may be different for each message, hence it has to be queried each time.
    _ClientSessionRef = le_msg_GetSession(msgRef);

    // Dispatch to appropriate message handler and get response
    switch (msgPtr->id)
    {
        case _MSGID_le_framework_Stop : Handle_le_framework_Stop(msgRef); break;
        case _MSGID_le_framework_Restart : Handle_le_framework_Restart(msgRef); break;
        case _MSGID_le_framework_IsStopping : Handle_le_framework_IsStopping(msgRef); break;
        case _MSGID_le_framework_IsReadOnly : Handle_le_framework_IsReadOnly(msgRef); break;
        case _MSGID_le_framework_NotifyExpectedReboot : Handle_le_framework_NotifyExpectedReboot(msgRef); break;

        default: LE_ERROR("Unknowm msg id = %i", msgPtr->id);
    }

    // Clear the client session ref associated with the current message, since the message
    // has now been processed.
    _ClientSessionRef = 0;
}