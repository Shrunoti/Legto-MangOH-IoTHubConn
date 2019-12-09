
/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */


#include "le_cfg_messages.h"
#include "le_cfg_server.h"


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
typedef struct le_cfg_ServerCmd
{
    le_msg_MessageRef_t msgRef;           ///< Reference to the message
    le_dls_Link_t cmdLink;                ///< Link to server cmd objects
    uint32_t requiredOutputs;           ///< Outputs which must be sent (if any)
} le_cfg_ServerCmd_t;

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
le_msg_ServiceRef_t le_cfg_GetServiceRef
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
le_msg_SessionRef_t le_cfg_GetClientSessionRef
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
void le_cfg_AdvertiseService
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
    _ServerDataPool = le_mem_CreatePool("le_cfg_ServerData", sizeof(_ServerData_t));

    // Create the server command pool
    _ServerCmdPool = le_mem_CreatePool("le_cfg_ServerCmd", sizeof(le_cfg_ServerCmd_t));

    // Create safe reference map for handler references.
    // The size of the map should be based on the number of handlers defined for the server.
    // Don't expect that to be more than 2-3, so use 3 as a reasonable guess.
    _HandlerRefMap = le_ref_CreateMap("le_cfg_ServerHandlers", 3);

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
 * Server-side respond function for le_cfg_CreateReadTxn
 */
//--------------------------------------------------------------------------------------------------
void le_cfg_CreateReadTxnRespond
(
    le_cfg_ServerCmdRef_t _cmdRef,
    le_cfg_IteratorRef_t _result
)
{
    LE_ASSERT(_cmdRef != NULL);

    // Get the message related data
    le_msg_MessageRef_t _msgRef = _cmdRef->msgRef;
    _Message_t* _msgPtr = le_msg_GetPayloadPtr(_msgRef);
    __attribute__((unused)) uint8_t* _msgBufPtr = _msgPtr->buffer;
    __attribute__((unused)) size_t _msgBufSize = _MAX_MSG_SIZE;

    // Ensure the passed in msgRef is for the correct message
    LE_ASSERT(_msgPtr->id == _MSGID_le_cfg_CreateReadTxn);

    // Ensure that this Respond function has not already been called
    LE_FATAL_IF( !le_msg_NeedsResponse(_msgRef), "Response has already been sent");

    // Pack the result first
    LE_ASSERT(le_pack_PackReference( &_msgBufPtr, &_msgBufSize,
                                                    _result ));

    // Null-out any parameters which are not required so pack knows not to pack them.

    // Pack any "out" parameters

    // Return the response
    TRACE("Sending response to client session %p", le_msg_GetSession(_msgRef));

    le_msg_Respond(_msgRef);

    // Release the command
    le_mem_Release(_cmdRef);
}

static void Handle_le_cfg_CreateReadTxn
(
    le_msg_MessageRef_t _msgRef
)
{
    // Create a server command object
    le_cfg_ServerCmd_t* _serverCmdPtr = le_mem_ForceAlloc(_ServerCmdPool);
    _serverCmdPtr->cmdLink = LE_DLS_LINK_INIT;
    _serverCmdPtr->msgRef = _msgRef;

    // Get the message buffer pointer
    __attribute__((unused)) uint8_t* _msgBufPtr =
        ((_Message_t*)le_msg_GetPayloadPtr(_msgRef))->buffer;
    __attribute__((unused)) size_t _msgBufSize = _MAX_MSG_SIZE;

    // Unpack which outputs are needed.
    _serverCmdPtr->requiredOutputs = 0;

    // Unpack the input parameters from the message
    char basePath[512];
    if (!le_pack_UnpackString( &_msgBufPtr, &_msgBufSize,
                               basePath,
                               sizeof(basePath),
                               511 ))
    {
        goto error_unpack;
    }

    // Call the function
    le_cfg_CreateReadTxn ( _serverCmdPtr, basePath );

    return;

error_unpack:
    le_mem_Release(_serverCmdPtr);

    LE_KILL_CLIENT("Error unpacking inputs");
}


//--------------------------------------------------------------------------------------------------
/**
 * Server-side respond function for le_cfg_CreateWriteTxn
 */
//--------------------------------------------------------------------------------------------------
void le_cfg_CreateWriteTxnRespond
(
    le_cfg_ServerCmdRef_t _cmdRef,
    le_cfg_IteratorRef_t _result
)
{
    LE_ASSERT(_cmdRef != NULL);

    // Get the message related data
    le_msg_MessageRef_t _msgRef = _cmdRef->msgRef;
    _Message_t* _msgPtr = le_msg_GetPayloadPtr(_msgRef);
    __attribute__((unused)) uint8_t* _msgBufPtr = _msgPtr->buffer;
    __attribute__((unused)) size_t _msgBufSize = _MAX_MSG_SIZE;

    // Ensure the passed in msgRef is for the correct message
    LE_ASSERT(_msgPtr->id == _MSGID_le_cfg_CreateWriteTxn);

    // Ensure that this Respond function has not already been called
    LE_FATAL_IF( !le_msg_NeedsResponse(_msgRef), "Response has already been sent");

    // Pack the result first
    LE_ASSERT(le_pack_PackReference( &_msgBufPtr, &_msgBufSize,
                                                    _result ));

    // Null-out any parameters which are not required so pack knows not to pack them.

    // Pack any "out" parameters

    // Return the response
    TRACE("Sending response to client session %p", le_msg_GetSession(_msgRef));

    le_msg_Respond(_msgRef);

    // Release the command
    le_mem_Release(_cmdRef);
}

static void Handle_le_cfg_CreateWriteTxn
(
    le_msg_MessageRef_t _msgRef
)
{
    // Create a server command object
    le_cfg_ServerCmd_t* _serverCmdPtr = le_mem_ForceAlloc(_ServerCmdPool);
    _serverCmdPtr->cmdLink = LE_DLS_LINK_INIT;
    _serverCmdPtr->msgRef = _msgRef;

    // Get the message buffer pointer
    __attribute__((unused)) uint8_t* _msgBufPtr =
        ((_Message_t*)le_msg_GetPayloadPtr(_msgRef))->buffer;
    __attribute__((unused)) size_t _msgBufSize = _MAX_MSG_SIZE;

    // Unpack which outputs are needed.
    _serverCmdPtr->requiredOutputs = 0;

    // Unpack the input parameters from the message
    char basePath[512];
    if (!le_pack_UnpackString( &_msgBufPtr, &_msgBufSize,
                               basePath,
                               sizeof(basePath),
                               511 ))
    {
        goto error_unpack;
    }

    // Call the function
    le_cfg_CreateWriteTxn ( _serverCmdPtr, basePath );

    return;

error_unpack:
    le_mem_Release(_serverCmdPtr);

    LE_KILL_CLIENT("Error unpacking inputs");
}


//--------------------------------------------------------------------------------------------------
/**
 * Server-side respond function for le_cfg_CommitTxn
 */
//--------------------------------------------------------------------------------------------------
void le_cfg_CommitTxnRespond
(
    le_cfg_ServerCmdRef_t _cmdRef
)
{
    LE_ASSERT(_cmdRef != NULL);

    // Get the message related data
    le_msg_MessageRef_t _msgRef = _cmdRef->msgRef;
    _Message_t* _msgPtr = le_msg_GetPayloadPtr(_msgRef);
    __attribute__((unused)) uint8_t* _msgBufPtr = _msgPtr->buffer;
    __attribute__((unused)) size_t _msgBufSize = _MAX_MSG_SIZE;

    // Ensure the passed in msgRef is for the correct message
    LE_ASSERT(_msgPtr->id == _MSGID_le_cfg_CommitTxn);

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

static void Handle_le_cfg_CommitTxn
(
    le_msg_MessageRef_t _msgRef
)
{
    // Create a server command object
    le_cfg_ServerCmd_t* _serverCmdPtr = le_mem_ForceAlloc(_ServerCmdPool);
    _serverCmdPtr->cmdLink = LE_DLS_LINK_INIT;
    _serverCmdPtr->msgRef = _msgRef;

    // Get the message buffer pointer
    __attribute__((unused)) uint8_t* _msgBufPtr =
        ((_Message_t*)le_msg_GetPayloadPtr(_msgRef))->buffer;
    __attribute__((unused)) size_t _msgBufSize = _MAX_MSG_SIZE;

    // Unpack which outputs are needed.
    _serverCmdPtr->requiredOutputs = 0;

    // Unpack the input parameters from the message
    le_cfg_IteratorRef_t iteratorRef;
    if (!le_pack_UnpackReference( &_msgBufPtr, &_msgBufSize,
                                               &iteratorRef ))
    {
        goto error_unpack;
    }

    // Call the function
    le_cfg_CommitTxn ( _serverCmdPtr, iteratorRef );

    return;

error_unpack:
    le_mem_Release(_serverCmdPtr);

    LE_KILL_CLIENT("Error unpacking inputs");
}


//--------------------------------------------------------------------------------------------------
/**
 * Server-side respond function for le_cfg_CancelTxn
 */
//--------------------------------------------------------------------------------------------------
void le_cfg_CancelTxnRespond
(
    le_cfg_ServerCmdRef_t _cmdRef
)
{
    LE_ASSERT(_cmdRef != NULL);

    // Get the message related data
    le_msg_MessageRef_t _msgRef = _cmdRef->msgRef;
    _Message_t* _msgPtr = le_msg_GetPayloadPtr(_msgRef);
    __attribute__((unused)) uint8_t* _msgBufPtr = _msgPtr->buffer;
    __attribute__((unused)) size_t _msgBufSize = _MAX_MSG_SIZE;

    // Ensure the passed in msgRef is for the correct message
    LE_ASSERT(_msgPtr->id == _MSGID_le_cfg_CancelTxn);

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

static void Handle_le_cfg_CancelTxn
(
    le_msg_MessageRef_t _msgRef
)
{
    // Create a server command object
    le_cfg_ServerCmd_t* _serverCmdPtr = le_mem_ForceAlloc(_ServerCmdPool);
    _serverCmdPtr->cmdLink = LE_DLS_LINK_INIT;
    _serverCmdPtr->msgRef = _msgRef;

    // Get the message buffer pointer
    __attribute__((unused)) uint8_t* _msgBufPtr =
        ((_Message_t*)le_msg_GetPayloadPtr(_msgRef))->buffer;
    __attribute__((unused)) size_t _msgBufSize = _MAX_MSG_SIZE;

    // Unpack which outputs are needed.
    _serverCmdPtr->requiredOutputs = 0;

    // Unpack the input parameters from the message
    le_cfg_IteratorRef_t iteratorRef;
    if (!le_pack_UnpackReference( &_msgBufPtr, &_msgBufSize,
                                               &iteratorRef ))
    {
        goto error_unpack;
    }

    // Call the function
    le_cfg_CancelTxn ( _serverCmdPtr, iteratorRef );

    return;

error_unpack:
    le_mem_Release(_serverCmdPtr);

    LE_KILL_CLIENT("Error unpacking inputs");
}


//--------------------------------------------------------------------------------------------------
/**
 * Server-side respond function for le_cfg_GoToNode
 */
//--------------------------------------------------------------------------------------------------
void le_cfg_GoToNodeRespond
(
    le_cfg_ServerCmdRef_t _cmdRef
)
{
    LE_ASSERT(_cmdRef != NULL);

    // Get the message related data
    le_msg_MessageRef_t _msgRef = _cmdRef->msgRef;
    _Message_t* _msgPtr = le_msg_GetPayloadPtr(_msgRef);
    __attribute__((unused)) uint8_t* _msgBufPtr = _msgPtr->buffer;
    __attribute__((unused)) size_t _msgBufSize = _MAX_MSG_SIZE;

    // Ensure the passed in msgRef is for the correct message
    LE_ASSERT(_msgPtr->id == _MSGID_le_cfg_GoToNode);

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

static void Handle_le_cfg_GoToNode
(
    le_msg_MessageRef_t _msgRef
)
{
    // Create a server command object
    le_cfg_ServerCmd_t* _serverCmdPtr = le_mem_ForceAlloc(_ServerCmdPool);
    _serverCmdPtr->cmdLink = LE_DLS_LINK_INIT;
    _serverCmdPtr->msgRef = _msgRef;

    // Get the message buffer pointer
    __attribute__((unused)) uint8_t* _msgBufPtr =
        ((_Message_t*)le_msg_GetPayloadPtr(_msgRef))->buffer;
    __attribute__((unused)) size_t _msgBufSize = _MAX_MSG_SIZE;

    // Unpack which outputs are needed.
    _serverCmdPtr->requiredOutputs = 0;

    // Unpack the input parameters from the message
    le_cfg_IteratorRef_t iteratorRef;
    if (!le_pack_UnpackReference( &_msgBufPtr, &_msgBufSize,
                                               &iteratorRef ))
    {
        goto error_unpack;
    }
    char newPath[512];
    if (!le_pack_UnpackString( &_msgBufPtr, &_msgBufSize,
                               newPath,
                               sizeof(newPath),
                               511 ))
    {
        goto error_unpack;
    }

    // Call the function
    le_cfg_GoToNode ( _serverCmdPtr, iteratorRef, newPath );

    return;

error_unpack:
    le_mem_Release(_serverCmdPtr);

    LE_KILL_CLIENT("Error unpacking inputs");
}


//--------------------------------------------------------------------------------------------------
/**
 * Server-side respond function for le_cfg_GoToParent
 */
//--------------------------------------------------------------------------------------------------
void le_cfg_GoToParentRespond
(
    le_cfg_ServerCmdRef_t _cmdRef,
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
    LE_ASSERT(_msgPtr->id == _MSGID_le_cfg_GoToParent);

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

static void Handle_le_cfg_GoToParent
(
    le_msg_MessageRef_t _msgRef
)
{
    // Create a server command object
    le_cfg_ServerCmd_t* _serverCmdPtr = le_mem_ForceAlloc(_ServerCmdPool);
    _serverCmdPtr->cmdLink = LE_DLS_LINK_INIT;
    _serverCmdPtr->msgRef = _msgRef;

    // Get the message buffer pointer
    __attribute__((unused)) uint8_t* _msgBufPtr =
        ((_Message_t*)le_msg_GetPayloadPtr(_msgRef))->buffer;
    __attribute__((unused)) size_t _msgBufSize = _MAX_MSG_SIZE;

    // Unpack which outputs are needed.
    _serverCmdPtr->requiredOutputs = 0;

    // Unpack the input parameters from the message
    le_cfg_IteratorRef_t iteratorRef;
    if (!le_pack_UnpackReference( &_msgBufPtr, &_msgBufSize,
                                               &iteratorRef ))
    {
        goto error_unpack;
    }

    // Call the function
    le_cfg_GoToParent ( _serverCmdPtr, iteratorRef );

    return;

error_unpack:
    le_mem_Release(_serverCmdPtr);

    LE_KILL_CLIENT("Error unpacking inputs");
}


//--------------------------------------------------------------------------------------------------
/**
 * Server-side respond function for le_cfg_GoToFirstChild
 */
//--------------------------------------------------------------------------------------------------
void le_cfg_GoToFirstChildRespond
(
    le_cfg_ServerCmdRef_t _cmdRef,
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
    LE_ASSERT(_msgPtr->id == _MSGID_le_cfg_GoToFirstChild);

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

static void Handle_le_cfg_GoToFirstChild
(
    le_msg_MessageRef_t _msgRef
)
{
    // Create a server command object
    le_cfg_ServerCmd_t* _serverCmdPtr = le_mem_ForceAlloc(_ServerCmdPool);
    _serverCmdPtr->cmdLink = LE_DLS_LINK_INIT;
    _serverCmdPtr->msgRef = _msgRef;

    // Get the message buffer pointer
    __attribute__((unused)) uint8_t* _msgBufPtr =
        ((_Message_t*)le_msg_GetPayloadPtr(_msgRef))->buffer;
    __attribute__((unused)) size_t _msgBufSize = _MAX_MSG_SIZE;

    // Unpack which outputs are needed.
    _serverCmdPtr->requiredOutputs = 0;

    // Unpack the input parameters from the message
    le_cfg_IteratorRef_t iteratorRef;
    if (!le_pack_UnpackReference( &_msgBufPtr, &_msgBufSize,
                                               &iteratorRef ))
    {
        goto error_unpack;
    }

    // Call the function
    le_cfg_GoToFirstChild ( _serverCmdPtr, iteratorRef );

    return;

error_unpack:
    le_mem_Release(_serverCmdPtr);

    LE_KILL_CLIENT("Error unpacking inputs");
}


//--------------------------------------------------------------------------------------------------
/**
 * Server-side respond function for le_cfg_GoToNextSibling
 */
//--------------------------------------------------------------------------------------------------
void le_cfg_GoToNextSiblingRespond
(
    le_cfg_ServerCmdRef_t _cmdRef,
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
    LE_ASSERT(_msgPtr->id == _MSGID_le_cfg_GoToNextSibling);

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

static void Handle_le_cfg_GoToNextSibling
(
    le_msg_MessageRef_t _msgRef
)
{
    // Create a server command object
    le_cfg_ServerCmd_t* _serverCmdPtr = le_mem_ForceAlloc(_ServerCmdPool);
    _serverCmdPtr->cmdLink = LE_DLS_LINK_INIT;
    _serverCmdPtr->msgRef = _msgRef;

    // Get the message buffer pointer
    __attribute__((unused)) uint8_t* _msgBufPtr =
        ((_Message_t*)le_msg_GetPayloadPtr(_msgRef))->buffer;
    __attribute__((unused)) size_t _msgBufSize = _MAX_MSG_SIZE;

    // Unpack which outputs are needed.
    _serverCmdPtr->requiredOutputs = 0;

    // Unpack the input parameters from the message
    le_cfg_IteratorRef_t iteratorRef;
    if (!le_pack_UnpackReference( &_msgBufPtr, &_msgBufSize,
                                               &iteratorRef ))
    {
        goto error_unpack;
    }

    // Call the function
    le_cfg_GoToNextSibling ( _serverCmdPtr, iteratorRef );

    return;

error_unpack:
    le_mem_Release(_serverCmdPtr);

    LE_KILL_CLIENT("Error unpacking inputs");
}


//--------------------------------------------------------------------------------------------------
/**
 * Server-side respond function for le_cfg_GetPath
 */
//--------------------------------------------------------------------------------------------------
void le_cfg_GetPathRespond
(
    le_cfg_ServerCmdRef_t _cmdRef,
    le_result_t _result,
    const char* pathBuffer
)
{
    LE_ASSERT(_cmdRef != NULL);

    // Get the message related data
    le_msg_MessageRef_t _msgRef = _cmdRef->msgRef;
    _Message_t* _msgPtr = le_msg_GetPayloadPtr(_msgRef);
    __attribute__((unused)) uint8_t* _msgBufPtr = _msgPtr->buffer;
    __attribute__((unused)) size_t _msgBufSize = _MAX_MSG_SIZE;

    // Ensure the passed in msgRef is for the correct message
    LE_ASSERT(_msgPtr->id == _MSGID_le_cfg_GetPath);

    // Ensure that this Respond function has not already been called
    LE_FATAL_IF( !le_msg_NeedsResponse(_msgRef), "Response has already been sent");

    // Pack the result first
    LE_ASSERT(le_pack_PackResult( &_msgBufPtr, &_msgBufSize,
                                                    _result ));

    // Null-out any parameters which are not required so pack knows not to pack them.
    
    LE_ASSERT(pathBuffer);
    if (!(_cmdRef->requiredOutputs & (1 << 0)))
    {
        pathBuffer = NULL;
    }

    // Pack any "out" parameters
    if (pathBuffer)
    {
        LE_ASSERT(le_pack_PackString( &_msgBufPtr, &_msgBufSize,
                                      pathBuffer, 511 ));
    }

    // Return the response
    TRACE("Sending response to client session %p", le_msg_GetSession(_msgRef));

    le_msg_Respond(_msgRef);

    // Release the command
    le_mem_Release(_cmdRef);
}

static void Handle_le_cfg_GetPath
(
    le_msg_MessageRef_t _msgRef
)
{
    // Create a server command object
    le_cfg_ServerCmd_t* _serverCmdPtr = le_mem_ForceAlloc(_ServerCmdPool);
    _serverCmdPtr->cmdLink = LE_DLS_LINK_INIT;
    _serverCmdPtr->msgRef = _msgRef;

    // Get the message buffer pointer
    __attribute__((unused)) uint8_t* _msgBufPtr =
        ((_Message_t*)le_msg_GetPayloadPtr(_msgRef))->buffer;
    __attribute__((unused)) size_t _msgBufSize = _MAX_MSG_SIZE;

    // Unpack which outputs are needed.
    _serverCmdPtr->requiredOutputs = 0;
    if (!le_pack_UnpackUint32(&_msgBufPtr, &_msgBufSize, &_serverCmdPtr->requiredOutputs))
    {
        goto error_unpack;
    }

    // Unpack the input parameters from the message
    le_cfg_IteratorRef_t iteratorRef;
    if (!le_pack_UnpackReference( &_msgBufPtr, &_msgBufSize,
                                               &iteratorRef ))
    {
        goto error_unpack;
    }
    char path[512];
    if (!le_pack_UnpackString( &_msgBufPtr, &_msgBufSize,
                               path,
                               sizeof(path),
                               511 ))
    {
        goto error_unpack;
    }
    size_t pathBufferSize;
    if (!le_pack_UnpackSize( &_msgBufPtr, &_msgBufSize,
                               &pathBufferSize ))
    {
        goto error_unpack;
    }
    if ( (pathBufferSize > 511) &&
         (pathBufferSize < UINT32_MAX) )
    {
        LE_DEBUG("Adjusting pathBufferSize from %zu to 511", pathBufferSize);
        pathBufferSize = 511;
    }
    if (pathBufferSize >= UINT32_MAX)
    {
        pathBufferSize = 0;
    }
    else
    {
        pathBufferSize++;
    }

    // Call the function
    le_cfg_GetPath ( _serverCmdPtr, iteratorRef, path, pathBufferSize );

    return;

error_unpack:
    le_mem_Release(_serverCmdPtr);

    LE_KILL_CLIENT("Error unpacking inputs");
}


//--------------------------------------------------------------------------------------------------
/**
 * Server-side respond function for le_cfg_GetNodeType
 */
//--------------------------------------------------------------------------------------------------
void le_cfg_GetNodeTypeRespond
(
    le_cfg_ServerCmdRef_t _cmdRef,
    le_cfg_nodeType_t _result
)
{
    LE_ASSERT(_cmdRef != NULL);

    // Get the message related data
    le_msg_MessageRef_t _msgRef = _cmdRef->msgRef;
    _Message_t* _msgPtr = le_msg_GetPayloadPtr(_msgRef);
    __attribute__((unused)) uint8_t* _msgBufPtr = _msgPtr->buffer;
    __attribute__((unused)) size_t _msgBufSize = _MAX_MSG_SIZE;

    // Ensure the passed in msgRef is for the correct message
    LE_ASSERT(_msgPtr->id == _MSGID_le_cfg_GetNodeType);

    // Ensure that this Respond function has not already been called
    LE_FATAL_IF( !le_msg_NeedsResponse(_msgRef), "Response has already been sent");

    // Pack the result first
    LE_ASSERT(le_pack_PackUint32( &_msgBufPtr, &_msgBufSize,
                                                    _result ));

    // Null-out any parameters which are not required so pack knows not to pack them.

    // Pack any "out" parameters

    // Return the response
    TRACE("Sending response to client session %p", le_msg_GetSession(_msgRef));

    le_msg_Respond(_msgRef);

    // Release the command
    le_mem_Release(_cmdRef);
}

static void Handle_le_cfg_GetNodeType
(
    le_msg_MessageRef_t _msgRef
)
{
    // Create a server command object
    le_cfg_ServerCmd_t* _serverCmdPtr = le_mem_ForceAlloc(_ServerCmdPool);
    _serverCmdPtr->cmdLink = LE_DLS_LINK_INIT;
    _serverCmdPtr->msgRef = _msgRef;

    // Get the message buffer pointer
    __attribute__((unused)) uint8_t* _msgBufPtr =
        ((_Message_t*)le_msg_GetPayloadPtr(_msgRef))->buffer;
    __attribute__((unused)) size_t _msgBufSize = _MAX_MSG_SIZE;

    // Unpack which outputs are needed.
    _serverCmdPtr->requiredOutputs = 0;

    // Unpack the input parameters from the message
    le_cfg_IteratorRef_t iteratorRef;
    if (!le_pack_UnpackReference( &_msgBufPtr, &_msgBufSize,
                                               &iteratorRef ))
    {
        goto error_unpack;
    }
    char path[512];
    if (!le_pack_UnpackString( &_msgBufPtr, &_msgBufSize,
                               path,
                               sizeof(path),
                               511 ))
    {
        goto error_unpack;
    }

    // Call the function
    le_cfg_GetNodeType ( _serverCmdPtr, iteratorRef, path );

    return;

error_unpack:
    le_mem_Release(_serverCmdPtr);

    LE_KILL_CLIENT("Error unpacking inputs");
}


//--------------------------------------------------------------------------------------------------
/**
 * Server-side respond function for le_cfg_GetNodeName
 */
//--------------------------------------------------------------------------------------------------
void le_cfg_GetNodeNameRespond
(
    le_cfg_ServerCmdRef_t _cmdRef,
    le_result_t _result,
    const char* name
)
{
    LE_ASSERT(_cmdRef != NULL);

    // Get the message related data
    le_msg_MessageRef_t _msgRef = _cmdRef->msgRef;
    _Message_t* _msgPtr = le_msg_GetPayloadPtr(_msgRef);
    __attribute__((unused)) uint8_t* _msgBufPtr = _msgPtr->buffer;
    __attribute__((unused)) size_t _msgBufSize = _MAX_MSG_SIZE;

    // Ensure the passed in msgRef is for the correct message
    LE_ASSERT(_msgPtr->id == _MSGID_le_cfg_GetNodeName);

    // Ensure that this Respond function has not already been called
    LE_FATAL_IF( !le_msg_NeedsResponse(_msgRef), "Response has already been sent");

    // Pack the result first
    LE_ASSERT(le_pack_PackResult( &_msgBufPtr, &_msgBufSize,
                                                    _result ));

    // Null-out any parameters which are not required so pack knows not to pack them.
    
    LE_ASSERT(name);
    if (!(_cmdRef->requiredOutputs & (1 << 0)))
    {
        name = NULL;
    }

    // Pack any "out" parameters
    if (name)
    {
        LE_ASSERT(le_pack_PackString( &_msgBufPtr, &_msgBufSize,
                                      name, 511 ));
    }

    // Return the response
    TRACE("Sending response to client session %p", le_msg_GetSession(_msgRef));

    le_msg_Respond(_msgRef);

    // Release the command
    le_mem_Release(_cmdRef);
}

static void Handle_le_cfg_GetNodeName
(
    le_msg_MessageRef_t _msgRef
)
{
    // Create a server command object
    le_cfg_ServerCmd_t* _serverCmdPtr = le_mem_ForceAlloc(_ServerCmdPool);
    _serverCmdPtr->cmdLink = LE_DLS_LINK_INIT;
    _serverCmdPtr->msgRef = _msgRef;

    // Get the message buffer pointer
    __attribute__((unused)) uint8_t* _msgBufPtr =
        ((_Message_t*)le_msg_GetPayloadPtr(_msgRef))->buffer;
    __attribute__((unused)) size_t _msgBufSize = _MAX_MSG_SIZE;

    // Unpack which outputs are needed.
    _serverCmdPtr->requiredOutputs = 0;
    if (!le_pack_UnpackUint32(&_msgBufPtr, &_msgBufSize, &_serverCmdPtr->requiredOutputs))
    {
        goto error_unpack;
    }

    // Unpack the input parameters from the message
    le_cfg_IteratorRef_t iteratorRef;
    if (!le_pack_UnpackReference( &_msgBufPtr, &_msgBufSize,
                                               &iteratorRef ))
    {
        goto error_unpack;
    }
    char path[512];
    if (!le_pack_UnpackString( &_msgBufPtr, &_msgBufSize,
                               path,
                               sizeof(path),
                               511 ))
    {
        goto error_unpack;
    }
    size_t nameSize;
    if (!le_pack_UnpackSize( &_msgBufPtr, &_msgBufSize,
                               &nameSize ))
    {
        goto error_unpack;
    }
    if ( (nameSize > 511) &&
         (nameSize < UINT32_MAX) )
    {
        LE_DEBUG("Adjusting nameSize from %zu to 511", nameSize);
        nameSize = 511;
    }
    if (nameSize >= UINT32_MAX)
    {
        nameSize = 0;
    }
    else
    {
        nameSize++;
    }

    // Call the function
    le_cfg_GetNodeName ( _serverCmdPtr, iteratorRef, path, nameSize );

    return;

error_unpack:
    le_mem_Release(_serverCmdPtr);

    LE_KILL_CLIENT("Error unpacking inputs");
}


static void AsyncResponse_le_cfg_AddChangeHandler
(
    void* contextPtr
)
{
    le_msg_MessageRef_t _msgRef;
    _Message_t* _msgPtr;
    _ServerData_t* serverDataPtr = (_ServerData_t*)contextPtr;

    // Will not be used if no data is sent back to client
    __attribute__((unused)) uint8_t* _msgBufPtr;
    __attribute__((unused)) size_t _msgBufSize;

    // Create a new message object and get the message buffer
    _msgRef = le_msg_CreateMsg(serverDataPtr->clientSessionRef);
    _msgPtr = le_msg_GetPayloadPtr(_msgRef);
    _msgPtr->id = _MSGID_le_cfg_AddChangeHandler;
    _msgBufPtr = _msgPtr->buffer;
    _msgBufSize = _MAX_MSG_SIZE;

    // Always pack the client context pointer first
    LE_ASSERT(le_pack_PackReference( &_msgBufPtr, &_msgBufSize, serverDataPtr->contextPtr ))

    // Pack the input parameters
    

    // Send the async response to the client
    TRACE("Sending message to client session %p : %ti bytes sent",
          serverDataPtr->clientSessionRef,
          _msgBufPtr-_msgPtr->buffer);

    SendMsgToClient(_msgRef);
}


static void Handle_le_cfg_AddChangeHandler
(
    le_msg_MessageRef_t _msgRef

)
{
    // Get the message buffer pointer
    __attribute__((unused)) uint8_t* _msgBufPtr =
        ((_Message_t*)le_msg_GetPayloadPtr(_msgRef))->buffer;
    __attribute__((unused)) size_t _msgBufSize = _MAX_MSG_SIZE;

    // Needed if we are returning a result or output values
    uint8_t* _msgBufStartPtr = _msgBufPtr;

    // Unpack which outputs are needed

    // Unpack the input parameters from the message
    char newPath[512];
    if (!le_pack_UnpackString( &_msgBufPtr, &_msgBufSize,
                               newPath,
                               sizeof(newPath),
                               511 ))
    {
        goto error_unpack;
    }
    void *contextPtr;
    if (!le_pack_UnpackReference( &_msgBufPtr, &_msgBufSize, &contextPtr ))
    {
        goto error_unpack;
    }

    // Create a new server data object and fill it in
    _ServerData_t* serverDataPtr = le_mem_ForceAlloc(_ServerDataPool);
    serverDataPtr->clientSessionRef = le_msg_GetSession(_msgRef);
    serverDataPtr->contextPtr = contextPtr;
    serverDataPtr->handlerRef = NULL;
    serverDataPtr->removeHandlerFunc = NULL;
    contextPtr = serverDataPtr;

    // Define storage for output parameters

    // Call the function
    le_cfg_ChangeHandlerRef_t _result;
    _result  = le_cfg_AddChangeHandler ( 
        newPath, AsyncResponse_le_cfg_AddChangeHandler, 
        contextPtr );

    if (_result)
    {
        // Put the handler reference result and a pointer to the associated remove function
        // into the server data object.  This function pointer is needed in case the client
        // is closed and the handlers need to be removed.
        serverDataPtr->handlerRef = (le_event_HandlerRef_t)_result;
        serverDataPtr->removeHandlerFunc =
            (RemoveHandlerFunc_t)le_cfg_RemoveChangeHandler;

        // Return a safe reference to the server data object as the reference.
        _LOCK
        _result = le_ref_CreateRef(_HandlerRefMap, serverDataPtr);
        _UNLOCK
    }
    else
    {
        // Adding handler failed; release serverDataPtr and return NULL back to the client.
        le_mem_Release(serverDataPtr);
    }

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;
    _msgBufSize = _MAX_MSG_SIZE;

    // Pack the result first
    LE_ASSERT(le_pack_PackReference( &_msgBufPtr, &_msgBufSize, _result ));

    // Pack any "out" parameters

    // Return the response
    TRACE("Sending response to client session %p : %ti bytes sent",
          le_msg_GetSession(_msgRef),
          _msgBufPtr-_msgBufStartPtr);


    le_msg_Respond(_msgRef);

    return;

error_unpack:
    LE_KILL_CLIENT("Error unpacking message");
}


static void Handle_le_cfg_RemoveChangeHandler
(
    le_msg_MessageRef_t _msgRef

)
{
    // Get the message buffer pointer
    __attribute__((unused)) uint8_t* _msgBufPtr =
        ((_Message_t*)le_msg_GetPayloadPtr(_msgRef))->buffer;
    __attribute__((unused)) size_t _msgBufSize = _MAX_MSG_SIZE;

    // Needed if we are returning a result or output values
    uint8_t* _msgBufStartPtr = _msgBufPtr;

    // Unpack which outputs are needed

    // Unpack the input parameters from the message
    le_cfg_ChangeHandlerRef_t handlerRef;
    if (!le_pack_UnpackReference( &_msgBufPtr, &_msgBufSize,
                                  &handlerRef ))
    {
        goto error_unpack;
    }
    // The passed in handlerRef is a safe reference for the server data object.  Need to get the
    // real handlerRef from the server data object and then delete both the safe reference and
    // the object since they are no longer needed.
    _LOCK
    _ServerData_t* serverDataPtr = le_ref_Lookup(_HandlerRefMap,
                                                 handlerRef);
    if ( serverDataPtr == NULL )
    {
        _UNLOCK
        LE_KILL_CLIENT("Invalid reference");
        return;
    }
    le_ref_DeleteRef(_HandlerRefMap, handlerRef);
    _UNLOCK
    handlerRef = (le_cfg_ChangeHandlerRef_t)serverDataPtr->handlerRef;
    le_mem_Release(serverDataPtr);

    // Define storage for output parameters

    // Call the function
    le_cfg_RemoveChangeHandler ( 
        handlerRef );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;
    _msgBufSize = _MAX_MSG_SIZE;

    // Pack any "out" parameters

    // Return the response
    TRACE("Sending response to client session %p : %ti bytes sent",
          le_msg_GetSession(_msgRef),
          _msgBufPtr-_msgBufStartPtr);


    le_msg_Respond(_msgRef);

    return;

error_unpack:
    LE_KILL_CLIENT("Error unpacking message");
}


//--------------------------------------------------------------------------------------------------
/**
 * Server-side respond function for le_cfg_DeleteNode
 */
//--------------------------------------------------------------------------------------------------
void le_cfg_DeleteNodeRespond
(
    le_cfg_ServerCmdRef_t _cmdRef
)
{
    LE_ASSERT(_cmdRef != NULL);

    // Get the message related data
    le_msg_MessageRef_t _msgRef = _cmdRef->msgRef;
    _Message_t* _msgPtr = le_msg_GetPayloadPtr(_msgRef);
    __attribute__((unused)) uint8_t* _msgBufPtr = _msgPtr->buffer;
    __attribute__((unused)) size_t _msgBufSize = _MAX_MSG_SIZE;

    // Ensure the passed in msgRef is for the correct message
    LE_ASSERT(_msgPtr->id == _MSGID_le_cfg_DeleteNode);

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

static void Handle_le_cfg_DeleteNode
(
    le_msg_MessageRef_t _msgRef
)
{
    // Create a server command object
    le_cfg_ServerCmd_t* _serverCmdPtr = le_mem_ForceAlloc(_ServerCmdPool);
    _serverCmdPtr->cmdLink = LE_DLS_LINK_INIT;
    _serverCmdPtr->msgRef = _msgRef;

    // Get the message buffer pointer
    __attribute__((unused)) uint8_t* _msgBufPtr =
        ((_Message_t*)le_msg_GetPayloadPtr(_msgRef))->buffer;
    __attribute__((unused)) size_t _msgBufSize = _MAX_MSG_SIZE;

    // Unpack which outputs are needed.
    _serverCmdPtr->requiredOutputs = 0;

    // Unpack the input parameters from the message
    le_cfg_IteratorRef_t iteratorRef;
    if (!le_pack_UnpackReference( &_msgBufPtr, &_msgBufSize,
                                               &iteratorRef ))
    {
        goto error_unpack;
    }
    char path[512];
    if (!le_pack_UnpackString( &_msgBufPtr, &_msgBufSize,
                               path,
                               sizeof(path),
                               511 ))
    {
        goto error_unpack;
    }

    // Call the function
    le_cfg_DeleteNode ( _serverCmdPtr, iteratorRef, path );

    return;

error_unpack:
    le_mem_Release(_serverCmdPtr);

    LE_KILL_CLIENT("Error unpacking inputs");
}


//--------------------------------------------------------------------------------------------------
/**
 * Server-side respond function for le_cfg_IsEmpty
 */
//--------------------------------------------------------------------------------------------------
void le_cfg_IsEmptyRespond
(
    le_cfg_ServerCmdRef_t _cmdRef,
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
    LE_ASSERT(_msgPtr->id == _MSGID_le_cfg_IsEmpty);

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

static void Handle_le_cfg_IsEmpty
(
    le_msg_MessageRef_t _msgRef
)
{
    // Create a server command object
    le_cfg_ServerCmd_t* _serverCmdPtr = le_mem_ForceAlloc(_ServerCmdPool);
    _serverCmdPtr->cmdLink = LE_DLS_LINK_INIT;
    _serverCmdPtr->msgRef = _msgRef;

    // Get the message buffer pointer
    __attribute__((unused)) uint8_t* _msgBufPtr =
        ((_Message_t*)le_msg_GetPayloadPtr(_msgRef))->buffer;
    __attribute__((unused)) size_t _msgBufSize = _MAX_MSG_SIZE;

    // Unpack which outputs are needed.
    _serverCmdPtr->requiredOutputs = 0;

    // Unpack the input parameters from the message
    le_cfg_IteratorRef_t iteratorRef;
    if (!le_pack_UnpackReference( &_msgBufPtr, &_msgBufSize,
                                               &iteratorRef ))
    {
        goto error_unpack;
    }
    char path[512];
    if (!le_pack_UnpackString( &_msgBufPtr, &_msgBufSize,
                               path,
                               sizeof(path),
                               511 ))
    {
        goto error_unpack;
    }

    // Call the function
    le_cfg_IsEmpty ( _serverCmdPtr, iteratorRef, path );

    return;

error_unpack:
    le_mem_Release(_serverCmdPtr);

    LE_KILL_CLIENT("Error unpacking inputs");
}


//--------------------------------------------------------------------------------------------------
/**
 * Server-side respond function for le_cfg_SetEmpty
 */
//--------------------------------------------------------------------------------------------------
void le_cfg_SetEmptyRespond
(
    le_cfg_ServerCmdRef_t _cmdRef
)
{
    LE_ASSERT(_cmdRef != NULL);

    // Get the message related data
    le_msg_MessageRef_t _msgRef = _cmdRef->msgRef;
    _Message_t* _msgPtr = le_msg_GetPayloadPtr(_msgRef);
    __attribute__((unused)) uint8_t* _msgBufPtr = _msgPtr->buffer;
    __attribute__((unused)) size_t _msgBufSize = _MAX_MSG_SIZE;

    // Ensure the passed in msgRef is for the correct message
    LE_ASSERT(_msgPtr->id == _MSGID_le_cfg_SetEmpty);

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

static void Handle_le_cfg_SetEmpty
(
    le_msg_MessageRef_t _msgRef
)
{
    // Create a server command object
    le_cfg_ServerCmd_t* _serverCmdPtr = le_mem_ForceAlloc(_ServerCmdPool);
    _serverCmdPtr->cmdLink = LE_DLS_LINK_INIT;
    _serverCmdPtr->msgRef = _msgRef;

    // Get the message buffer pointer
    __attribute__((unused)) uint8_t* _msgBufPtr =
        ((_Message_t*)le_msg_GetPayloadPtr(_msgRef))->buffer;
    __attribute__((unused)) size_t _msgBufSize = _MAX_MSG_SIZE;

    // Unpack which outputs are needed.
    _serverCmdPtr->requiredOutputs = 0;

    // Unpack the input parameters from the message
    le_cfg_IteratorRef_t iteratorRef;
    if (!le_pack_UnpackReference( &_msgBufPtr, &_msgBufSize,
                                               &iteratorRef ))
    {
        goto error_unpack;
    }
    char path[512];
    if (!le_pack_UnpackString( &_msgBufPtr, &_msgBufSize,
                               path,
                               sizeof(path),
                               511 ))
    {
        goto error_unpack;
    }

    // Call the function
    le_cfg_SetEmpty ( _serverCmdPtr, iteratorRef, path );

    return;

error_unpack:
    le_mem_Release(_serverCmdPtr);

    LE_KILL_CLIENT("Error unpacking inputs");
}


//--------------------------------------------------------------------------------------------------
/**
 * Server-side respond function for le_cfg_NodeExists
 */
//--------------------------------------------------------------------------------------------------
void le_cfg_NodeExistsRespond
(
    le_cfg_ServerCmdRef_t _cmdRef,
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
    LE_ASSERT(_msgPtr->id == _MSGID_le_cfg_NodeExists);

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

static void Handle_le_cfg_NodeExists
(
    le_msg_MessageRef_t _msgRef
)
{
    // Create a server command object
    le_cfg_ServerCmd_t* _serverCmdPtr = le_mem_ForceAlloc(_ServerCmdPool);
    _serverCmdPtr->cmdLink = LE_DLS_LINK_INIT;
    _serverCmdPtr->msgRef = _msgRef;

    // Get the message buffer pointer
    __attribute__((unused)) uint8_t* _msgBufPtr =
        ((_Message_t*)le_msg_GetPayloadPtr(_msgRef))->buffer;
    __attribute__((unused)) size_t _msgBufSize = _MAX_MSG_SIZE;

    // Unpack which outputs are needed.
    _serverCmdPtr->requiredOutputs = 0;

    // Unpack the input parameters from the message
    le_cfg_IteratorRef_t iteratorRef;
    if (!le_pack_UnpackReference( &_msgBufPtr, &_msgBufSize,
                                               &iteratorRef ))
    {
        goto error_unpack;
    }
    char path[512];
    if (!le_pack_UnpackString( &_msgBufPtr, &_msgBufSize,
                               path,
                               sizeof(path),
                               511 ))
    {
        goto error_unpack;
    }

    // Call the function
    le_cfg_NodeExists ( _serverCmdPtr, iteratorRef, path );

    return;

error_unpack:
    le_mem_Release(_serverCmdPtr);

    LE_KILL_CLIENT("Error unpacking inputs");
}


//--------------------------------------------------------------------------------------------------
/**
 * Server-side respond function for le_cfg_GetString
 */
//--------------------------------------------------------------------------------------------------
void le_cfg_GetStringRespond
(
    le_cfg_ServerCmdRef_t _cmdRef,
    le_result_t _result,
    const char* value
)
{
    LE_ASSERT(_cmdRef != NULL);

    // Get the message related data
    le_msg_MessageRef_t _msgRef = _cmdRef->msgRef;
    _Message_t* _msgPtr = le_msg_GetPayloadPtr(_msgRef);
    __attribute__((unused)) uint8_t* _msgBufPtr = _msgPtr->buffer;
    __attribute__((unused)) size_t _msgBufSize = _MAX_MSG_SIZE;

    // Ensure the passed in msgRef is for the correct message
    LE_ASSERT(_msgPtr->id == _MSGID_le_cfg_GetString);

    // Ensure that this Respond function has not already been called
    LE_FATAL_IF( !le_msg_NeedsResponse(_msgRef), "Response has already been sent");

    // Pack the result first
    LE_ASSERT(le_pack_PackResult( &_msgBufPtr, &_msgBufSize,
                                                    _result ));

    // Null-out any parameters which are not required so pack knows not to pack them.
    
    LE_ASSERT(value);
    if (!(_cmdRef->requiredOutputs & (1 << 0)))
    {
        value = NULL;
    }

    // Pack any "out" parameters
    if (value)
    {
        LE_ASSERT(le_pack_PackString( &_msgBufPtr, &_msgBufSize,
                                      value, 511 ));
    }

    // Return the response
    TRACE("Sending response to client session %p", le_msg_GetSession(_msgRef));

    le_msg_Respond(_msgRef);

    // Release the command
    le_mem_Release(_cmdRef);
}

static void Handle_le_cfg_GetString
(
    le_msg_MessageRef_t _msgRef
)
{
    // Create a server command object
    le_cfg_ServerCmd_t* _serverCmdPtr = le_mem_ForceAlloc(_ServerCmdPool);
    _serverCmdPtr->cmdLink = LE_DLS_LINK_INIT;
    _serverCmdPtr->msgRef = _msgRef;

    // Get the message buffer pointer
    __attribute__((unused)) uint8_t* _msgBufPtr =
        ((_Message_t*)le_msg_GetPayloadPtr(_msgRef))->buffer;
    __attribute__((unused)) size_t _msgBufSize = _MAX_MSG_SIZE;

    // Unpack which outputs are needed.
    _serverCmdPtr->requiredOutputs = 0;
    if (!le_pack_UnpackUint32(&_msgBufPtr, &_msgBufSize, &_serverCmdPtr->requiredOutputs))
    {
        goto error_unpack;
    }

    // Unpack the input parameters from the message
    le_cfg_IteratorRef_t iteratorRef;
    if (!le_pack_UnpackReference( &_msgBufPtr, &_msgBufSize,
                                               &iteratorRef ))
    {
        goto error_unpack;
    }
    char path[512];
    if (!le_pack_UnpackString( &_msgBufPtr, &_msgBufSize,
                               path,
                               sizeof(path),
                               511 ))
    {
        goto error_unpack;
    }
    size_t valueSize;
    if (!le_pack_UnpackSize( &_msgBufPtr, &_msgBufSize,
                               &valueSize ))
    {
        goto error_unpack;
    }
    if ( (valueSize > 511) &&
         (valueSize < UINT32_MAX) )
    {
        LE_DEBUG("Adjusting valueSize from %zu to 511", valueSize);
        valueSize = 511;
    }
    if (valueSize >= UINT32_MAX)
    {
        valueSize = 0;
    }
    else
    {
        valueSize++;
    }
    char defaultValue[512];
    if (!le_pack_UnpackString( &_msgBufPtr, &_msgBufSize,
                               defaultValue,
                               sizeof(defaultValue),
                               511 ))
    {
        goto error_unpack;
    }

    // Call the function
    le_cfg_GetString ( _serverCmdPtr, iteratorRef, path, valueSize, defaultValue );

    return;

error_unpack:
    le_mem_Release(_serverCmdPtr);

    LE_KILL_CLIENT("Error unpacking inputs");
}


//--------------------------------------------------------------------------------------------------
/**
 * Server-side respond function for le_cfg_SetString
 */
//--------------------------------------------------------------------------------------------------
void le_cfg_SetStringRespond
(
    le_cfg_ServerCmdRef_t _cmdRef
)
{
    LE_ASSERT(_cmdRef != NULL);

    // Get the message related data
    le_msg_MessageRef_t _msgRef = _cmdRef->msgRef;
    _Message_t* _msgPtr = le_msg_GetPayloadPtr(_msgRef);
    __attribute__((unused)) uint8_t* _msgBufPtr = _msgPtr->buffer;
    __attribute__((unused)) size_t _msgBufSize = _MAX_MSG_SIZE;

    // Ensure the passed in msgRef is for the correct message
    LE_ASSERT(_msgPtr->id == _MSGID_le_cfg_SetString);

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

static void Handle_le_cfg_SetString
(
    le_msg_MessageRef_t _msgRef
)
{
    // Create a server command object
    le_cfg_ServerCmd_t* _serverCmdPtr = le_mem_ForceAlloc(_ServerCmdPool);
    _serverCmdPtr->cmdLink = LE_DLS_LINK_INIT;
    _serverCmdPtr->msgRef = _msgRef;

    // Get the message buffer pointer
    __attribute__((unused)) uint8_t* _msgBufPtr =
        ((_Message_t*)le_msg_GetPayloadPtr(_msgRef))->buffer;
    __attribute__((unused)) size_t _msgBufSize = _MAX_MSG_SIZE;

    // Unpack which outputs are needed.
    _serverCmdPtr->requiredOutputs = 0;

    // Unpack the input parameters from the message
    le_cfg_IteratorRef_t iteratorRef;
    if (!le_pack_UnpackReference( &_msgBufPtr, &_msgBufSize,
                                               &iteratorRef ))
    {
        goto error_unpack;
    }
    char path[512];
    if (!le_pack_UnpackString( &_msgBufPtr, &_msgBufSize,
                               path,
                               sizeof(path),
                               511 ))
    {
        goto error_unpack;
    }
    char value[512];
    if (!le_pack_UnpackString( &_msgBufPtr, &_msgBufSize,
                               value,
                               sizeof(value),
                               511 ))
    {
        goto error_unpack;
    }

    // Call the function
    le_cfg_SetString ( _serverCmdPtr, iteratorRef, path, value );

    return;

error_unpack:
    le_mem_Release(_serverCmdPtr);

    LE_KILL_CLIENT("Error unpacking inputs");
}


//--------------------------------------------------------------------------------------------------
/**
 * Server-side respond function for le_cfg_GetInt
 */
//--------------------------------------------------------------------------------------------------
void le_cfg_GetIntRespond
(
    le_cfg_ServerCmdRef_t _cmdRef,
    int32_t _result
)
{
    LE_ASSERT(_cmdRef != NULL);

    // Get the message related data
    le_msg_MessageRef_t _msgRef = _cmdRef->msgRef;
    _Message_t* _msgPtr = le_msg_GetPayloadPtr(_msgRef);
    __attribute__((unused)) uint8_t* _msgBufPtr = _msgPtr->buffer;
    __attribute__((unused)) size_t _msgBufSize = _MAX_MSG_SIZE;

    // Ensure the passed in msgRef is for the correct message
    LE_ASSERT(_msgPtr->id == _MSGID_le_cfg_GetInt);

    // Ensure that this Respond function has not already been called
    LE_FATAL_IF( !le_msg_NeedsResponse(_msgRef), "Response has already been sent");

    // Pack the result first
    LE_ASSERT(le_pack_PackInt32( &_msgBufPtr, &_msgBufSize,
                                                    _result ));

    // Null-out any parameters which are not required so pack knows not to pack them.

    // Pack any "out" parameters

    // Return the response
    TRACE("Sending response to client session %p", le_msg_GetSession(_msgRef));

    le_msg_Respond(_msgRef);

    // Release the command
    le_mem_Release(_cmdRef);
}

static void Handle_le_cfg_GetInt
(
    le_msg_MessageRef_t _msgRef
)
{
    // Create a server command object
    le_cfg_ServerCmd_t* _serverCmdPtr = le_mem_ForceAlloc(_ServerCmdPool);
    _serverCmdPtr->cmdLink = LE_DLS_LINK_INIT;
    _serverCmdPtr->msgRef = _msgRef;

    // Get the message buffer pointer
    __attribute__((unused)) uint8_t* _msgBufPtr =
        ((_Message_t*)le_msg_GetPayloadPtr(_msgRef))->buffer;
    __attribute__((unused)) size_t _msgBufSize = _MAX_MSG_SIZE;

    // Unpack which outputs are needed.
    _serverCmdPtr->requiredOutputs = 0;

    // Unpack the input parameters from the message
    le_cfg_IteratorRef_t iteratorRef;
    if (!le_pack_UnpackReference( &_msgBufPtr, &_msgBufSize,
                                               &iteratorRef ))
    {
        goto error_unpack;
    }
    char path[512];
    if (!le_pack_UnpackString( &_msgBufPtr, &_msgBufSize,
                               path,
                               sizeof(path),
                               511 ))
    {
        goto error_unpack;
    }
    int32_t defaultValue;
    if (!le_pack_UnpackInt32( &_msgBufPtr, &_msgBufSize,
                                               &defaultValue ))
    {
        goto error_unpack;
    }

    // Call the function
    le_cfg_GetInt ( _serverCmdPtr, iteratorRef, path, defaultValue );

    return;

error_unpack:
    le_mem_Release(_serverCmdPtr);

    LE_KILL_CLIENT("Error unpacking inputs");
}


//--------------------------------------------------------------------------------------------------
/**
 * Server-side respond function for le_cfg_SetInt
 */
//--------------------------------------------------------------------------------------------------
void le_cfg_SetIntRespond
(
    le_cfg_ServerCmdRef_t _cmdRef
)
{
    LE_ASSERT(_cmdRef != NULL);

    // Get the message related data
    le_msg_MessageRef_t _msgRef = _cmdRef->msgRef;
    _Message_t* _msgPtr = le_msg_GetPayloadPtr(_msgRef);
    __attribute__((unused)) uint8_t* _msgBufPtr = _msgPtr->buffer;
    __attribute__((unused)) size_t _msgBufSize = _MAX_MSG_SIZE;

    // Ensure the passed in msgRef is for the correct message
    LE_ASSERT(_msgPtr->id == _MSGID_le_cfg_SetInt);

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

static void Handle_le_cfg_SetInt
(
    le_msg_MessageRef_t _msgRef
)
{
    // Create a server command object
    le_cfg_ServerCmd_t* _serverCmdPtr = le_mem_ForceAlloc(_ServerCmdPool);
    _serverCmdPtr->cmdLink = LE_DLS_LINK_INIT;
    _serverCmdPtr->msgRef = _msgRef;

    // Get the message buffer pointer
    __attribute__((unused)) uint8_t* _msgBufPtr =
        ((_Message_t*)le_msg_GetPayloadPtr(_msgRef))->buffer;
    __attribute__((unused)) size_t _msgBufSize = _MAX_MSG_SIZE;

    // Unpack which outputs are needed.
    _serverCmdPtr->requiredOutputs = 0;

    // Unpack the input parameters from the message
    le_cfg_IteratorRef_t iteratorRef;
    if (!le_pack_UnpackReference( &_msgBufPtr, &_msgBufSize,
                                               &iteratorRef ))
    {
        goto error_unpack;
    }
    char path[512];
    if (!le_pack_UnpackString( &_msgBufPtr, &_msgBufSize,
                               path,
                               sizeof(path),
                               511 ))
    {
        goto error_unpack;
    }
    int32_t value;
    if (!le_pack_UnpackInt32( &_msgBufPtr, &_msgBufSize,
                                               &value ))
    {
        goto error_unpack;
    }

    // Call the function
    le_cfg_SetInt ( _serverCmdPtr, iteratorRef, path, value );

    return;

error_unpack:
    le_mem_Release(_serverCmdPtr);

    LE_KILL_CLIENT("Error unpacking inputs");
}


//--------------------------------------------------------------------------------------------------
/**
 * Server-side respond function for le_cfg_GetFloat
 */
//--------------------------------------------------------------------------------------------------
void le_cfg_GetFloatRespond
(
    le_cfg_ServerCmdRef_t _cmdRef,
    double _result
)
{
    LE_ASSERT(_cmdRef != NULL);

    // Get the message related data
    le_msg_MessageRef_t _msgRef = _cmdRef->msgRef;
    _Message_t* _msgPtr = le_msg_GetPayloadPtr(_msgRef);
    __attribute__((unused)) uint8_t* _msgBufPtr = _msgPtr->buffer;
    __attribute__((unused)) size_t _msgBufSize = _MAX_MSG_SIZE;

    // Ensure the passed in msgRef is for the correct message
    LE_ASSERT(_msgPtr->id == _MSGID_le_cfg_GetFloat);

    // Ensure that this Respond function has not already been called
    LE_FATAL_IF( !le_msg_NeedsResponse(_msgRef), "Response has already been sent");

    // Pack the result first
    LE_ASSERT(le_pack_PackDouble( &_msgBufPtr, &_msgBufSize,
                                                    _result ));

    // Null-out any parameters which are not required so pack knows not to pack them.

    // Pack any "out" parameters

    // Return the response
    TRACE("Sending response to client session %p", le_msg_GetSession(_msgRef));

    le_msg_Respond(_msgRef);

    // Release the command
    le_mem_Release(_cmdRef);
}

static void Handle_le_cfg_GetFloat
(
    le_msg_MessageRef_t _msgRef
)
{
    // Create a server command object
    le_cfg_ServerCmd_t* _serverCmdPtr = le_mem_ForceAlloc(_ServerCmdPool);
    _serverCmdPtr->cmdLink = LE_DLS_LINK_INIT;
    _serverCmdPtr->msgRef = _msgRef;

    // Get the message buffer pointer
    __attribute__((unused)) uint8_t* _msgBufPtr =
        ((_Message_t*)le_msg_GetPayloadPtr(_msgRef))->buffer;
    __attribute__((unused)) size_t _msgBufSize = _MAX_MSG_SIZE;

    // Unpack which outputs are needed.
    _serverCmdPtr->requiredOutputs = 0;

    // Unpack the input parameters from the message
    le_cfg_IteratorRef_t iteratorRef;
    if (!le_pack_UnpackReference( &_msgBufPtr, &_msgBufSize,
                                               &iteratorRef ))
    {
        goto error_unpack;
    }
    char path[512];
    if (!le_pack_UnpackString( &_msgBufPtr, &_msgBufSize,
                               path,
                               sizeof(path),
                               511 ))
    {
        goto error_unpack;
    }
    double defaultValue;
    if (!le_pack_UnpackDouble( &_msgBufPtr, &_msgBufSize,
                                               &defaultValue ))
    {
        goto error_unpack;
    }

    // Call the function
    le_cfg_GetFloat ( _serverCmdPtr, iteratorRef, path, defaultValue );

    return;

error_unpack:
    le_mem_Release(_serverCmdPtr);

    LE_KILL_CLIENT("Error unpacking inputs");
}


//--------------------------------------------------------------------------------------------------
/**
 * Server-side respond function for le_cfg_SetFloat
 */
//--------------------------------------------------------------------------------------------------
void le_cfg_SetFloatRespond
(
    le_cfg_ServerCmdRef_t _cmdRef
)
{
    LE_ASSERT(_cmdRef != NULL);

    // Get the message related data
    le_msg_MessageRef_t _msgRef = _cmdRef->msgRef;
    _Message_t* _msgPtr = le_msg_GetPayloadPtr(_msgRef);
    __attribute__((unused)) uint8_t* _msgBufPtr = _msgPtr->buffer;
    __attribute__((unused)) size_t _msgBufSize = _MAX_MSG_SIZE;

    // Ensure the passed in msgRef is for the correct message
    LE_ASSERT(_msgPtr->id == _MSGID_le_cfg_SetFloat);

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

static void Handle_le_cfg_SetFloat
(
    le_msg_MessageRef_t _msgRef
)
{
    // Create a server command object
    le_cfg_ServerCmd_t* _serverCmdPtr = le_mem_ForceAlloc(_ServerCmdPool);
    _serverCmdPtr->cmdLink = LE_DLS_LINK_INIT;
    _serverCmdPtr->msgRef = _msgRef;

    // Get the message buffer pointer
    __attribute__((unused)) uint8_t* _msgBufPtr =
        ((_Message_t*)le_msg_GetPayloadPtr(_msgRef))->buffer;
    __attribute__((unused)) size_t _msgBufSize = _MAX_MSG_SIZE;

    // Unpack which outputs are needed.
    _serverCmdPtr->requiredOutputs = 0;

    // Unpack the input parameters from the message
    le_cfg_IteratorRef_t iteratorRef;
    if (!le_pack_UnpackReference( &_msgBufPtr, &_msgBufSize,
                                               &iteratorRef ))
    {
        goto error_unpack;
    }
    char path[512];
    if (!le_pack_UnpackString( &_msgBufPtr, &_msgBufSize,
                               path,
                               sizeof(path),
                               511 ))
    {
        goto error_unpack;
    }
    double value;
    if (!le_pack_UnpackDouble( &_msgBufPtr, &_msgBufSize,
                                               &value ))
    {
        goto error_unpack;
    }

    // Call the function
    le_cfg_SetFloat ( _serverCmdPtr, iteratorRef, path, value );

    return;

error_unpack:
    le_mem_Release(_serverCmdPtr);

    LE_KILL_CLIENT("Error unpacking inputs");
}


//--------------------------------------------------------------------------------------------------
/**
 * Server-side respond function for le_cfg_GetBool
 */
//--------------------------------------------------------------------------------------------------
void le_cfg_GetBoolRespond
(
    le_cfg_ServerCmdRef_t _cmdRef,
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
    LE_ASSERT(_msgPtr->id == _MSGID_le_cfg_GetBool);

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

static void Handle_le_cfg_GetBool
(
    le_msg_MessageRef_t _msgRef
)
{
    // Create a server command object
    le_cfg_ServerCmd_t* _serverCmdPtr = le_mem_ForceAlloc(_ServerCmdPool);
    _serverCmdPtr->cmdLink = LE_DLS_LINK_INIT;
    _serverCmdPtr->msgRef = _msgRef;

    // Get the message buffer pointer
    __attribute__((unused)) uint8_t* _msgBufPtr =
        ((_Message_t*)le_msg_GetPayloadPtr(_msgRef))->buffer;
    __attribute__((unused)) size_t _msgBufSize = _MAX_MSG_SIZE;

    // Unpack which outputs are needed.
    _serverCmdPtr->requiredOutputs = 0;

    // Unpack the input parameters from the message
    le_cfg_IteratorRef_t iteratorRef;
    if (!le_pack_UnpackReference( &_msgBufPtr, &_msgBufSize,
                                               &iteratorRef ))
    {
        goto error_unpack;
    }
    char path[512];
    if (!le_pack_UnpackString( &_msgBufPtr, &_msgBufSize,
                               path,
                               sizeof(path),
                               511 ))
    {
        goto error_unpack;
    }
    bool defaultValue;
    if (!le_pack_UnpackBool( &_msgBufPtr, &_msgBufSize,
                                               &defaultValue ))
    {
        goto error_unpack;
    }

    // Call the function
    le_cfg_GetBool ( _serverCmdPtr, iteratorRef, path, defaultValue );

    return;

error_unpack:
    le_mem_Release(_serverCmdPtr);

    LE_KILL_CLIENT("Error unpacking inputs");
}


//--------------------------------------------------------------------------------------------------
/**
 * Server-side respond function for le_cfg_SetBool
 */
//--------------------------------------------------------------------------------------------------
void le_cfg_SetBoolRespond
(
    le_cfg_ServerCmdRef_t _cmdRef
)
{
    LE_ASSERT(_cmdRef != NULL);

    // Get the message related data
    le_msg_MessageRef_t _msgRef = _cmdRef->msgRef;
    _Message_t* _msgPtr = le_msg_GetPayloadPtr(_msgRef);
    __attribute__((unused)) uint8_t* _msgBufPtr = _msgPtr->buffer;
    __attribute__((unused)) size_t _msgBufSize = _MAX_MSG_SIZE;

    // Ensure the passed in msgRef is for the correct message
    LE_ASSERT(_msgPtr->id == _MSGID_le_cfg_SetBool);

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

static void Handle_le_cfg_SetBool
(
    le_msg_MessageRef_t _msgRef
)
{
    // Create a server command object
    le_cfg_ServerCmd_t* _serverCmdPtr = le_mem_ForceAlloc(_ServerCmdPool);
    _serverCmdPtr->cmdLink = LE_DLS_LINK_INIT;
    _serverCmdPtr->msgRef = _msgRef;

    // Get the message buffer pointer
    __attribute__((unused)) uint8_t* _msgBufPtr =
        ((_Message_t*)le_msg_GetPayloadPtr(_msgRef))->buffer;
    __attribute__((unused)) size_t _msgBufSize = _MAX_MSG_SIZE;

    // Unpack which outputs are needed.
    _serverCmdPtr->requiredOutputs = 0;

    // Unpack the input parameters from the message
    le_cfg_IteratorRef_t iteratorRef;
    if (!le_pack_UnpackReference( &_msgBufPtr, &_msgBufSize,
                                               &iteratorRef ))
    {
        goto error_unpack;
    }
    char path[512];
    if (!le_pack_UnpackString( &_msgBufPtr, &_msgBufSize,
                               path,
                               sizeof(path),
                               511 ))
    {
        goto error_unpack;
    }
    bool value;
    if (!le_pack_UnpackBool( &_msgBufPtr, &_msgBufSize,
                                               &value ))
    {
        goto error_unpack;
    }

    // Call the function
    le_cfg_SetBool ( _serverCmdPtr, iteratorRef, path, value );

    return;

error_unpack:
    le_mem_Release(_serverCmdPtr);

    LE_KILL_CLIENT("Error unpacking inputs");
}


//--------------------------------------------------------------------------------------------------
/**
 * Server-side respond function for le_cfg_QuickDeleteNode
 */
//--------------------------------------------------------------------------------------------------
void le_cfg_QuickDeleteNodeRespond
(
    le_cfg_ServerCmdRef_t _cmdRef
)
{
    LE_ASSERT(_cmdRef != NULL);

    // Get the message related data
    le_msg_MessageRef_t _msgRef = _cmdRef->msgRef;
    _Message_t* _msgPtr = le_msg_GetPayloadPtr(_msgRef);
    __attribute__((unused)) uint8_t* _msgBufPtr = _msgPtr->buffer;
    __attribute__((unused)) size_t _msgBufSize = _MAX_MSG_SIZE;

    // Ensure the passed in msgRef is for the correct message
    LE_ASSERT(_msgPtr->id == _MSGID_le_cfg_QuickDeleteNode);

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

static void Handle_le_cfg_QuickDeleteNode
(
    le_msg_MessageRef_t _msgRef
)
{
    // Create a server command object
    le_cfg_ServerCmd_t* _serverCmdPtr = le_mem_ForceAlloc(_ServerCmdPool);
    _serverCmdPtr->cmdLink = LE_DLS_LINK_INIT;
    _serverCmdPtr->msgRef = _msgRef;

    // Get the message buffer pointer
    __attribute__((unused)) uint8_t* _msgBufPtr =
        ((_Message_t*)le_msg_GetPayloadPtr(_msgRef))->buffer;
    __attribute__((unused)) size_t _msgBufSize = _MAX_MSG_SIZE;

    // Unpack which outputs are needed.
    _serverCmdPtr->requiredOutputs = 0;

    // Unpack the input parameters from the message
    char path[512];
    if (!le_pack_UnpackString( &_msgBufPtr, &_msgBufSize,
                               path,
                               sizeof(path),
                               511 ))
    {
        goto error_unpack;
    }

    // Call the function
    le_cfg_QuickDeleteNode ( _serverCmdPtr, path );

    return;

error_unpack:
    le_mem_Release(_serverCmdPtr);

    LE_KILL_CLIENT("Error unpacking inputs");
}


//--------------------------------------------------------------------------------------------------
/**
 * Server-side respond function for le_cfg_QuickSetEmpty
 */
//--------------------------------------------------------------------------------------------------
void le_cfg_QuickSetEmptyRespond
(
    le_cfg_ServerCmdRef_t _cmdRef
)
{
    LE_ASSERT(_cmdRef != NULL);

    // Get the message related data
    le_msg_MessageRef_t _msgRef = _cmdRef->msgRef;
    _Message_t* _msgPtr = le_msg_GetPayloadPtr(_msgRef);
    __attribute__((unused)) uint8_t* _msgBufPtr = _msgPtr->buffer;
    __attribute__((unused)) size_t _msgBufSize = _MAX_MSG_SIZE;

    // Ensure the passed in msgRef is for the correct message
    LE_ASSERT(_msgPtr->id == _MSGID_le_cfg_QuickSetEmpty);

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

static void Handle_le_cfg_QuickSetEmpty
(
    le_msg_MessageRef_t _msgRef
)
{
    // Create a server command object
    le_cfg_ServerCmd_t* _serverCmdPtr = le_mem_ForceAlloc(_ServerCmdPool);
    _serverCmdPtr->cmdLink = LE_DLS_LINK_INIT;
    _serverCmdPtr->msgRef = _msgRef;

    // Get the message buffer pointer
    __attribute__((unused)) uint8_t* _msgBufPtr =
        ((_Message_t*)le_msg_GetPayloadPtr(_msgRef))->buffer;
    __attribute__((unused)) size_t _msgBufSize = _MAX_MSG_SIZE;

    // Unpack which outputs are needed.
    _serverCmdPtr->requiredOutputs = 0;

    // Unpack the input parameters from the message
    char path[512];
    if (!le_pack_UnpackString( &_msgBufPtr, &_msgBufSize,
                               path,
                               sizeof(path),
                               511 ))
    {
        goto error_unpack;
    }

    // Call the function
    le_cfg_QuickSetEmpty ( _serverCmdPtr, path );

    return;

error_unpack:
    le_mem_Release(_serverCmdPtr);

    LE_KILL_CLIENT("Error unpacking inputs");
}


//--------------------------------------------------------------------------------------------------
/**
 * Server-side respond function for le_cfg_QuickGetString
 */
//--------------------------------------------------------------------------------------------------
void le_cfg_QuickGetStringRespond
(
    le_cfg_ServerCmdRef_t _cmdRef,
    le_result_t _result,
    const char* value
)
{
    LE_ASSERT(_cmdRef != NULL);

    // Get the message related data
    le_msg_MessageRef_t _msgRef = _cmdRef->msgRef;
    _Message_t* _msgPtr = le_msg_GetPayloadPtr(_msgRef);
    __attribute__((unused)) uint8_t* _msgBufPtr = _msgPtr->buffer;
    __attribute__((unused)) size_t _msgBufSize = _MAX_MSG_SIZE;

    // Ensure the passed in msgRef is for the correct message
    LE_ASSERT(_msgPtr->id == _MSGID_le_cfg_QuickGetString);

    // Ensure that this Respond function has not already been called
    LE_FATAL_IF( !le_msg_NeedsResponse(_msgRef), "Response has already been sent");

    // Pack the result first
    LE_ASSERT(le_pack_PackResult( &_msgBufPtr, &_msgBufSize,
                                                    _result ));

    // Null-out any parameters which are not required so pack knows not to pack them.
    
    LE_ASSERT(value);
    if (!(_cmdRef->requiredOutputs & (1 << 0)))
    {
        value = NULL;
    }

    // Pack any "out" parameters
    if (value)
    {
        LE_ASSERT(le_pack_PackString( &_msgBufPtr, &_msgBufSize,
                                      value, 511 ));
    }

    // Return the response
    TRACE("Sending response to client session %p", le_msg_GetSession(_msgRef));

    le_msg_Respond(_msgRef);

    // Release the command
    le_mem_Release(_cmdRef);
}

static void Handle_le_cfg_QuickGetString
(
    le_msg_MessageRef_t _msgRef
)
{
    // Create a server command object
    le_cfg_ServerCmd_t* _serverCmdPtr = le_mem_ForceAlloc(_ServerCmdPool);
    _serverCmdPtr->cmdLink = LE_DLS_LINK_INIT;
    _serverCmdPtr->msgRef = _msgRef;

    // Get the message buffer pointer
    __attribute__((unused)) uint8_t* _msgBufPtr =
        ((_Message_t*)le_msg_GetPayloadPtr(_msgRef))->buffer;
    __attribute__((unused)) size_t _msgBufSize = _MAX_MSG_SIZE;

    // Unpack which outputs are needed.
    _serverCmdPtr->requiredOutputs = 0;
    if (!le_pack_UnpackUint32(&_msgBufPtr, &_msgBufSize, &_serverCmdPtr->requiredOutputs))
    {
        goto error_unpack;
    }

    // Unpack the input parameters from the message
    char path[512];
    if (!le_pack_UnpackString( &_msgBufPtr, &_msgBufSize,
                               path,
                               sizeof(path),
                               511 ))
    {
        goto error_unpack;
    }
    size_t valueSize;
    if (!le_pack_UnpackSize( &_msgBufPtr, &_msgBufSize,
                               &valueSize ))
    {
        goto error_unpack;
    }
    if ( (valueSize > 511) &&
         (valueSize < UINT32_MAX) )
    {
        LE_DEBUG("Adjusting valueSize from %zu to 511", valueSize);
        valueSize = 511;
    }
    if (valueSize >= UINT32_MAX)
    {
        valueSize = 0;
    }
    else
    {
        valueSize++;
    }
    char defaultValue[512];
    if (!le_pack_UnpackString( &_msgBufPtr, &_msgBufSize,
                               defaultValue,
                               sizeof(defaultValue),
                               511 ))
    {
        goto error_unpack;
    }

    // Call the function
    le_cfg_QuickGetString ( _serverCmdPtr, path, valueSize, defaultValue );

    return;

error_unpack:
    le_mem_Release(_serverCmdPtr);

    LE_KILL_CLIENT("Error unpacking inputs");
}


//--------------------------------------------------------------------------------------------------
/**
 * Server-side respond function for le_cfg_QuickSetString
 */
//--------------------------------------------------------------------------------------------------
void le_cfg_QuickSetStringRespond
(
    le_cfg_ServerCmdRef_t _cmdRef
)
{
    LE_ASSERT(_cmdRef != NULL);

    // Get the message related data
    le_msg_MessageRef_t _msgRef = _cmdRef->msgRef;
    _Message_t* _msgPtr = le_msg_GetPayloadPtr(_msgRef);
    __attribute__((unused)) uint8_t* _msgBufPtr = _msgPtr->buffer;
    __attribute__((unused)) size_t _msgBufSize = _MAX_MSG_SIZE;

    // Ensure the passed in msgRef is for the correct message
    LE_ASSERT(_msgPtr->id == _MSGID_le_cfg_QuickSetString);

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

static void Handle_le_cfg_QuickSetString
(
    le_msg_MessageRef_t _msgRef
)
{
    // Create a server command object
    le_cfg_ServerCmd_t* _serverCmdPtr = le_mem_ForceAlloc(_ServerCmdPool);
    _serverCmdPtr->cmdLink = LE_DLS_LINK_INIT;
    _serverCmdPtr->msgRef = _msgRef;

    // Get the message buffer pointer
    __attribute__((unused)) uint8_t* _msgBufPtr =
        ((_Message_t*)le_msg_GetPayloadPtr(_msgRef))->buffer;
    __attribute__((unused)) size_t _msgBufSize = _MAX_MSG_SIZE;

    // Unpack which outputs are needed.
    _serverCmdPtr->requiredOutputs = 0;

    // Unpack the input parameters from the message
    char path[512];
    if (!le_pack_UnpackString( &_msgBufPtr, &_msgBufSize,
                               path,
                               sizeof(path),
                               511 ))
    {
        goto error_unpack;
    }
    char value[512];
    if (!le_pack_UnpackString( &_msgBufPtr, &_msgBufSize,
                               value,
                               sizeof(value),
                               511 ))
    {
        goto error_unpack;
    }

    // Call the function
    le_cfg_QuickSetString ( _serverCmdPtr, path, value );

    return;

error_unpack:
    le_mem_Release(_serverCmdPtr);

    LE_KILL_CLIENT("Error unpacking inputs");
}


//--------------------------------------------------------------------------------------------------
/**
 * Server-side respond function for le_cfg_QuickGetInt
 */
//--------------------------------------------------------------------------------------------------
void le_cfg_QuickGetIntRespond
(
    le_cfg_ServerCmdRef_t _cmdRef,
    int32_t _result
)
{
    LE_ASSERT(_cmdRef != NULL);

    // Get the message related data
    le_msg_MessageRef_t _msgRef = _cmdRef->msgRef;
    _Message_t* _msgPtr = le_msg_GetPayloadPtr(_msgRef);
    __attribute__((unused)) uint8_t* _msgBufPtr = _msgPtr->buffer;
    __attribute__((unused)) size_t _msgBufSize = _MAX_MSG_SIZE;

    // Ensure the passed in msgRef is for the correct message
    LE_ASSERT(_msgPtr->id == _MSGID_le_cfg_QuickGetInt);

    // Ensure that this Respond function has not already been called
    LE_FATAL_IF( !le_msg_NeedsResponse(_msgRef), "Response has already been sent");

    // Pack the result first
    LE_ASSERT(le_pack_PackInt32( &_msgBufPtr, &_msgBufSize,
                                                    _result ));

    // Null-out any parameters which are not required so pack knows not to pack them.

    // Pack any "out" parameters

    // Return the response
    TRACE("Sending response to client session %p", le_msg_GetSession(_msgRef));

    le_msg_Respond(_msgRef);

    // Release the command
    le_mem_Release(_cmdRef);
}

static void Handle_le_cfg_QuickGetInt
(
    le_msg_MessageRef_t _msgRef
)
{
    // Create a server command object
    le_cfg_ServerCmd_t* _serverCmdPtr = le_mem_ForceAlloc(_ServerCmdPool);
    _serverCmdPtr->cmdLink = LE_DLS_LINK_INIT;
    _serverCmdPtr->msgRef = _msgRef;

    // Get the message buffer pointer
    __attribute__((unused)) uint8_t* _msgBufPtr =
        ((_Message_t*)le_msg_GetPayloadPtr(_msgRef))->buffer;
    __attribute__((unused)) size_t _msgBufSize = _MAX_MSG_SIZE;

    // Unpack which outputs are needed.
    _serverCmdPtr->requiredOutputs = 0;

    // Unpack the input parameters from the message
    char path[512];
    if (!le_pack_UnpackString( &_msgBufPtr, &_msgBufSize,
                               path,
                               sizeof(path),
                               511 ))
    {
        goto error_unpack;
    }
    int32_t defaultValue;
    if (!le_pack_UnpackInt32( &_msgBufPtr, &_msgBufSize,
                                               &defaultValue ))
    {
        goto error_unpack;
    }

    // Call the function
    le_cfg_QuickGetInt ( _serverCmdPtr, path, defaultValue );

    return;

error_unpack:
    le_mem_Release(_serverCmdPtr);

    LE_KILL_CLIENT("Error unpacking inputs");
}


//--------------------------------------------------------------------------------------------------
/**
 * Server-side respond function for le_cfg_QuickSetInt
 */
//--------------------------------------------------------------------------------------------------
void le_cfg_QuickSetIntRespond
(
    le_cfg_ServerCmdRef_t _cmdRef
)
{
    LE_ASSERT(_cmdRef != NULL);

    // Get the message related data
    le_msg_MessageRef_t _msgRef = _cmdRef->msgRef;
    _Message_t* _msgPtr = le_msg_GetPayloadPtr(_msgRef);
    __attribute__((unused)) uint8_t* _msgBufPtr = _msgPtr->buffer;
    __attribute__((unused)) size_t _msgBufSize = _MAX_MSG_SIZE;

    // Ensure the passed in msgRef is for the correct message
    LE_ASSERT(_msgPtr->id == _MSGID_le_cfg_QuickSetInt);

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

static void Handle_le_cfg_QuickSetInt
(
    le_msg_MessageRef_t _msgRef
)
{
    // Create a server command object
    le_cfg_ServerCmd_t* _serverCmdPtr = le_mem_ForceAlloc(_ServerCmdPool);
    _serverCmdPtr->cmdLink = LE_DLS_LINK_INIT;
    _serverCmdPtr->msgRef = _msgRef;

    // Get the message buffer pointer
    __attribute__((unused)) uint8_t* _msgBufPtr =
        ((_Message_t*)le_msg_GetPayloadPtr(_msgRef))->buffer;
    __attribute__((unused)) size_t _msgBufSize = _MAX_MSG_SIZE;

    // Unpack which outputs are needed.
    _serverCmdPtr->requiredOutputs = 0;

    // Unpack the input parameters from the message
    char path[512];
    if (!le_pack_UnpackString( &_msgBufPtr, &_msgBufSize,
                               path,
                               sizeof(path),
                               511 ))
    {
        goto error_unpack;
    }
    int32_t value;
    if (!le_pack_UnpackInt32( &_msgBufPtr, &_msgBufSize,
                                               &value ))
    {
        goto error_unpack;
    }

    // Call the function
    le_cfg_QuickSetInt ( _serverCmdPtr, path, value );

    return;

error_unpack:
    le_mem_Release(_serverCmdPtr);

    LE_KILL_CLIENT("Error unpacking inputs");
}


//--------------------------------------------------------------------------------------------------
/**
 * Server-side respond function for le_cfg_QuickGetFloat
 */
//--------------------------------------------------------------------------------------------------
void le_cfg_QuickGetFloatRespond
(
    le_cfg_ServerCmdRef_t _cmdRef,
    double _result
)
{
    LE_ASSERT(_cmdRef != NULL);

    // Get the message related data
    le_msg_MessageRef_t _msgRef = _cmdRef->msgRef;
    _Message_t* _msgPtr = le_msg_GetPayloadPtr(_msgRef);
    __attribute__((unused)) uint8_t* _msgBufPtr = _msgPtr->buffer;
    __attribute__((unused)) size_t _msgBufSize = _MAX_MSG_SIZE;

    // Ensure the passed in msgRef is for the correct message
    LE_ASSERT(_msgPtr->id == _MSGID_le_cfg_QuickGetFloat);

    // Ensure that this Respond function has not already been called
    LE_FATAL_IF( !le_msg_NeedsResponse(_msgRef), "Response has already been sent");

    // Pack the result first
    LE_ASSERT(le_pack_PackDouble( &_msgBufPtr, &_msgBufSize,
                                                    _result ));

    // Null-out any parameters which are not required so pack knows not to pack them.

    // Pack any "out" parameters

    // Return the response
    TRACE("Sending response to client session %p", le_msg_GetSession(_msgRef));

    le_msg_Respond(_msgRef);

    // Release the command
    le_mem_Release(_cmdRef);
}

static void Handle_le_cfg_QuickGetFloat
(
    le_msg_MessageRef_t _msgRef
)
{
    // Create a server command object
    le_cfg_ServerCmd_t* _serverCmdPtr = le_mem_ForceAlloc(_ServerCmdPool);
    _serverCmdPtr->cmdLink = LE_DLS_LINK_INIT;
    _serverCmdPtr->msgRef = _msgRef;

    // Get the message buffer pointer
    __attribute__((unused)) uint8_t* _msgBufPtr =
        ((_Message_t*)le_msg_GetPayloadPtr(_msgRef))->buffer;
    __attribute__((unused)) size_t _msgBufSize = _MAX_MSG_SIZE;

    // Unpack which outputs are needed.
    _serverCmdPtr->requiredOutputs = 0;

    // Unpack the input parameters from the message
    char path[512];
    if (!le_pack_UnpackString( &_msgBufPtr, &_msgBufSize,
                               path,
                               sizeof(path),
                               511 ))
    {
        goto error_unpack;
    }
    double defaultValue;
    if (!le_pack_UnpackDouble( &_msgBufPtr, &_msgBufSize,
                                               &defaultValue ))
    {
        goto error_unpack;
    }

    // Call the function
    le_cfg_QuickGetFloat ( _serverCmdPtr, path, defaultValue );

    return;

error_unpack:
    le_mem_Release(_serverCmdPtr);

    LE_KILL_CLIENT("Error unpacking inputs");
}


//--------------------------------------------------------------------------------------------------
/**
 * Server-side respond function for le_cfg_QuickSetFloat
 */
//--------------------------------------------------------------------------------------------------
void le_cfg_QuickSetFloatRespond
(
    le_cfg_ServerCmdRef_t _cmdRef
)
{
    LE_ASSERT(_cmdRef != NULL);

    // Get the message related data
    le_msg_MessageRef_t _msgRef = _cmdRef->msgRef;
    _Message_t* _msgPtr = le_msg_GetPayloadPtr(_msgRef);
    __attribute__((unused)) uint8_t* _msgBufPtr = _msgPtr->buffer;
    __attribute__((unused)) size_t _msgBufSize = _MAX_MSG_SIZE;

    // Ensure the passed in msgRef is for the correct message
    LE_ASSERT(_msgPtr->id == _MSGID_le_cfg_QuickSetFloat);

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

static void Handle_le_cfg_QuickSetFloat
(
    le_msg_MessageRef_t _msgRef
)
{
    // Create a server command object
    le_cfg_ServerCmd_t* _serverCmdPtr = le_mem_ForceAlloc(_ServerCmdPool);
    _serverCmdPtr->cmdLink = LE_DLS_LINK_INIT;
    _serverCmdPtr->msgRef = _msgRef;

    // Get the message buffer pointer
    __attribute__((unused)) uint8_t* _msgBufPtr =
        ((_Message_t*)le_msg_GetPayloadPtr(_msgRef))->buffer;
    __attribute__((unused)) size_t _msgBufSize = _MAX_MSG_SIZE;

    // Unpack which outputs are needed.
    _serverCmdPtr->requiredOutputs = 0;

    // Unpack the input parameters from the message
    char path[512];
    if (!le_pack_UnpackString( &_msgBufPtr, &_msgBufSize,
                               path,
                               sizeof(path),
                               511 ))
    {
        goto error_unpack;
    }
    double value;
    if (!le_pack_UnpackDouble( &_msgBufPtr, &_msgBufSize,
                                               &value ))
    {
        goto error_unpack;
    }

    // Call the function
    le_cfg_QuickSetFloat ( _serverCmdPtr, path, value );

    return;

error_unpack:
    le_mem_Release(_serverCmdPtr);

    LE_KILL_CLIENT("Error unpacking inputs");
}


//--------------------------------------------------------------------------------------------------
/**
 * Server-side respond function for le_cfg_QuickGetBool
 */
//--------------------------------------------------------------------------------------------------
void le_cfg_QuickGetBoolRespond
(
    le_cfg_ServerCmdRef_t _cmdRef,
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
    LE_ASSERT(_msgPtr->id == _MSGID_le_cfg_QuickGetBool);

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

static void Handle_le_cfg_QuickGetBool
(
    le_msg_MessageRef_t _msgRef
)
{
    // Create a server command object
    le_cfg_ServerCmd_t* _serverCmdPtr = le_mem_ForceAlloc(_ServerCmdPool);
    _serverCmdPtr->cmdLink = LE_DLS_LINK_INIT;
    _serverCmdPtr->msgRef = _msgRef;

    // Get the message buffer pointer
    __attribute__((unused)) uint8_t* _msgBufPtr =
        ((_Message_t*)le_msg_GetPayloadPtr(_msgRef))->buffer;
    __attribute__((unused)) size_t _msgBufSize = _MAX_MSG_SIZE;

    // Unpack which outputs are needed.
    _serverCmdPtr->requiredOutputs = 0;

    // Unpack the input parameters from the message
    char path[512];
    if (!le_pack_UnpackString( &_msgBufPtr, &_msgBufSize,
                               path,
                               sizeof(path),
                               511 ))
    {
        goto error_unpack;
    }
    bool defaultValue;
    if (!le_pack_UnpackBool( &_msgBufPtr, &_msgBufSize,
                                               &defaultValue ))
    {
        goto error_unpack;
    }

    // Call the function
    le_cfg_QuickGetBool ( _serverCmdPtr, path, defaultValue );

    return;

error_unpack:
    le_mem_Release(_serverCmdPtr);

    LE_KILL_CLIENT("Error unpacking inputs");
}


//--------------------------------------------------------------------------------------------------
/**
 * Server-side respond function for le_cfg_QuickSetBool
 */
//--------------------------------------------------------------------------------------------------
void le_cfg_QuickSetBoolRespond
(
    le_cfg_ServerCmdRef_t _cmdRef
)
{
    LE_ASSERT(_cmdRef != NULL);

    // Get the message related data
    le_msg_MessageRef_t _msgRef = _cmdRef->msgRef;
    _Message_t* _msgPtr = le_msg_GetPayloadPtr(_msgRef);
    __attribute__((unused)) uint8_t* _msgBufPtr = _msgPtr->buffer;
    __attribute__((unused)) size_t _msgBufSize = _MAX_MSG_SIZE;

    // Ensure the passed in msgRef is for the correct message
    LE_ASSERT(_msgPtr->id == _MSGID_le_cfg_QuickSetBool);

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

static void Handle_le_cfg_QuickSetBool
(
    le_msg_MessageRef_t _msgRef
)
{
    // Create a server command object
    le_cfg_ServerCmd_t* _serverCmdPtr = le_mem_ForceAlloc(_ServerCmdPool);
    _serverCmdPtr->cmdLink = LE_DLS_LINK_INIT;
    _serverCmdPtr->msgRef = _msgRef;

    // Get the message buffer pointer
    __attribute__((unused)) uint8_t* _msgBufPtr =
        ((_Message_t*)le_msg_GetPayloadPtr(_msgRef))->buffer;
    __attribute__((unused)) size_t _msgBufSize = _MAX_MSG_SIZE;

    // Unpack which outputs are needed.
    _serverCmdPtr->requiredOutputs = 0;

    // Unpack the input parameters from the message
    char path[512];
    if (!le_pack_UnpackString( &_msgBufPtr, &_msgBufSize,
                               path,
                               sizeof(path),
                               511 ))
    {
        goto error_unpack;
    }
    bool value;
    if (!le_pack_UnpackBool( &_msgBufPtr, &_msgBufSize,
                                               &value ))
    {
        goto error_unpack;
    }

    // Call the function
    le_cfg_QuickSetBool ( _serverCmdPtr, path, value );

    return;

error_unpack:
    le_mem_Release(_serverCmdPtr);

    LE_KILL_CLIENT("Error unpacking inputs");
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
        case _MSGID_le_cfg_CreateReadTxn : Handle_le_cfg_CreateReadTxn(msgRef); break;
        case _MSGID_le_cfg_CreateWriteTxn : Handle_le_cfg_CreateWriteTxn(msgRef); break;
        case _MSGID_le_cfg_CommitTxn : Handle_le_cfg_CommitTxn(msgRef); break;
        case _MSGID_le_cfg_CancelTxn : Handle_le_cfg_CancelTxn(msgRef); break;
        case _MSGID_le_cfg_GoToNode : Handle_le_cfg_GoToNode(msgRef); break;
        case _MSGID_le_cfg_GoToParent : Handle_le_cfg_GoToParent(msgRef); break;
        case _MSGID_le_cfg_GoToFirstChild : Handle_le_cfg_GoToFirstChild(msgRef); break;
        case _MSGID_le_cfg_GoToNextSibling : Handle_le_cfg_GoToNextSibling(msgRef); break;
        case _MSGID_le_cfg_GetPath : Handle_le_cfg_GetPath(msgRef); break;
        case _MSGID_le_cfg_GetNodeType : Handle_le_cfg_GetNodeType(msgRef); break;
        case _MSGID_le_cfg_GetNodeName : Handle_le_cfg_GetNodeName(msgRef); break;
        case _MSGID_le_cfg_AddChangeHandler : Handle_le_cfg_AddChangeHandler(msgRef); break;
        case _MSGID_le_cfg_RemoveChangeHandler : Handle_le_cfg_RemoveChangeHandler(msgRef); break;
        case _MSGID_le_cfg_DeleteNode : Handle_le_cfg_DeleteNode(msgRef); break;
        case _MSGID_le_cfg_IsEmpty : Handle_le_cfg_IsEmpty(msgRef); break;
        case _MSGID_le_cfg_SetEmpty : Handle_le_cfg_SetEmpty(msgRef); break;
        case _MSGID_le_cfg_NodeExists : Handle_le_cfg_NodeExists(msgRef); break;
        case _MSGID_le_cfg_GetString : Handle_le_cfg_GetString(msgRef); break;
        case _MSGID_le_cfg_SetString : Handle_le_cfg_SetString(msgRef); break;
        case _MSGID_le_cfg_GetInt : Handle_le_cfg_GetInt(msgRef); break;
        case _MSGID_le_cfg_SetInt : Handle_le_cfg_SetInt(msgRef); break;
        case _MSGID_le_cfg_GetFloat : Handle_le_cfg_GetFloat(msgRef); break;
        case _MSGID_le_cfg_SetFloat : Handle_le_cfg_SetFloat(msgRef); break;
        case _MSGID_le_cfg_GetBool : Handle_le_cfg_GetBool(msgRef); break;
        case _MSGID_le_cfg_SetBool : Handle_le_cfg_SetBool(msgRef); break;
        case _MSGID_le_cfg_QuickDeleteNode : Handle_le_cfg_QuickDeleteNode(msgRef); break;
        case _MSGID_le_cfg_QuickSetEmpty : Handle_le_cfg_QuickSetEmpty(msgRef); break;
        case _MSGID_le_cfg_QuickGetString : Handle_le_cfg_QuickGetString(msgRef); break;
        case _MSGID_le_cfg_QuickSetString : Handle_le_cfg_QuickSetString(msgRef); break;
        case _MSGID_le_cfg_QuickGetInt : Handle_le_cfg_QuickGetInt(msgRef); break;
        case _MSGID_le_cfg_QuickSetInt : Handle_le_cfg_QuickSetInt(msgRef); break;
        case _MSGID_le_cfg_QuickGetFloat : Handle_le_cfg_QuickGetFloat(msgRef); break;
        case _MSGID_le_cfg_QuickSetFloat : Handle_le_cfg_QuickSetFloat(msgRef); break;
        case _MSGID_le_cfg_QuickGetBool : Handle_le_cfg_QuickGetBool(msgRef); break;
        case _MSGID_le_cfg_QuickSetBool : Handle_le_cfg_QuickSetBool(msgRef); break;

        default: LE_ERROR("Unknowm msg id = %i", msgPtr->id);
    }

    // Clear the client session ref associated with the current message, since the message
    // has now been processed.
    _ClientSessionRef = 0;
}