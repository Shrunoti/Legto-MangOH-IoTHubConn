
/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */


#include "le_cfgAdmin_messages.h"
#include "le_cfgAdmin_server.h"


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
typedef struct le_cfgAdmin_ServerCmd
{
    le_msg_MessageRef_t msgRef;           ///< Reference to the message
    le_dls_Link_t cmdLink;                ///< Link to server cmd objects
    uint32_t requiredOutputs;           ///< Outputs which must be sent (if any)
} le_cfgAdmin_ServerCmd_t;

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
le_msg_ServiceRef_t le_cfgAdmin_GetServiceRef
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
le_msg_SessionRef_t le_cfgAdmin_GetClientSessionRef
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
void le_cfgAdmin_AdvertiseService
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
    _ServerDataPool = le_mem_CreatePool("le_cfgAdmin_ServerData", sizeof(_ServerData_t));

    // Create the server command pool
    _ServerCmdPool = le_mem_CreatePool("le_cfgAdmin_ServerCmd", sizeof(le_cfgAdmin_ServerCmd_t));

    // Create safe reference map for handler references.
    // The size of the map should be based on the number of handlers defined for the server.
    // Don't expect that to be more than 2-3, so use 3 as a reasonable guess.
    _HandlerRefMap = le_ref_CreateMap("le_cfgAdmin_ServerHandlers", 3);

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
 * Server-side respond function for le_cfgAdmin_ImportTree
 */
//--------------------------------------------------------------------------------------------------
void le_cfgAdmin_ImportTreeRespond
(
    le_cfgAdmin_ServerCmdRef_t _cmdRef,
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
    LE_ASSERT(_msgPtr->id == _MSGID_le_cfgAdmin_ImportTree);

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

static void Handle_le_cfgAdmin_ImportTree
(
    le_msg_MessageRef_t _msgRef
)
{
    // Create a server command object
    le_cfgAdmin_ServerCmd_t* _serverCmdPtr = le_mem_ForceAlloc(_ServerCmdPool);
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
    char filePath[513];
    if (!le_pack_UnpackString( &_msgBufPtr, &_msgBufSize,
                               filePath,
                               sizeof(filePath),
                               512 ))
    {
        goto error_unpack;
    }
    char nodePath[513];
    if (!le_pack_UnpackString( &_msgBufPtr, &_msgBufSize,
                               nodePath,
                               sizeof(nodePath),
                               512 ))
    {
        goto error_unpack;
    }

    // Call the function
    le_cfgAdmin_ImportTree ( _serverCmdPtr, iteratorRef, filePath, nodePath );

    return;

error_unpack:
    le_mem_Release(_serverCmdPtr);

    LE_KILL_CLIENT("Error unpacking inputs");
}


//--------------------------------------------------------------------------------------------------
/**
 * Server-side respond function for le_cfgAdmin_ExportTree
 */
//--------------------------------------------------------------------------------------------------
void le_cfgAdmin_ExportTreeRespond
(
    le_cfgAdmin_ServerCmdRef_t _cmdRef,
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
    LE_ASSERT(_msgPtr->id == _MSGID_le_cfgAdmin_ExportTree);

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

static void Handle_le_cfgAdmin_ExportTree
(
    le_msg_MessageRef_t _msgRef
)
{
    // Create a server command object
    le_cfgAdmin_ServerCmd_t* _serverCmdPtr = le_mem_ForceAlloc(_ServerCmdPool);
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
    char filePath[513];
    if (!le_pack_UnpackString( &_msgBufPtr, &_msgBufSize,
                               filePath,
                               sizeof(filePath),
                               512 ))
    {
        goto error_unpack;
    }
    char nodePath[513];
    if (!le_pack_UnpackString( &_msgBufPtr, &_msgBufSize,
                               nodePath,
                               sizeof(nodePath),
                               512 ))
    {
        goto error_unpack;
    }

    // Call the function
    le_cfgAdmin_ExportTree ( _serverCmdPtr, iteratorRef, filePath, nodePath );

    return;

error_unpack:
    le_mem_Release(_serverCmdPtr);

    LE_KILL_CLIENT("Error unpacking inputs");
}


//--------------------------------------------------------------------------------------------------
/**
 * Server-side respond function for le_cfgAdmin_DeleteTree
 */
//--------------------------------------------------------------------------------------------------
void le_cfgAdmin_DeleteTreeRespond
(
    le_cfgAdmin_ServerCmdRef_t _cmdRef
)
{
    LE_ASSERT(_cmdRef != NULL);

    // Get the message related data
    le_msg_MessageRef_t _msgRef = _cmdRef->msgRef;
    _Message_t* _msgPtr = le_msg_GetPayloadPtr(_msgRef);
    __attribute__((unused)) uint8_t* _msgBufPtr = _msgPtr->buffer;
    __attribute__((unused)) size_t _msgBufSize = _MAX_MSG_SIZE;

    // Ensure the passed in msgRef is for the correct message
    LE_ASSERT(_msgPtr->id == _MSGID_le_cfgAdmin_DeleteTree);

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

static void Handle_le_cfgAdmin_DeleteTree
(
    le_msg_MessageRef_t _msgRef
)
{
    // Create a server command object
    le_cfgAdmin_ServerCmd_t* _serverCmdPtr = le_mem_ForceAlloc(_ServerCmdPool);
    _serverCmdPtr->cmdLink = LE_DLS_LINK_INIT;
    _serverCmdPtr->msgRef = _msgRef;

    // Get the message buffer pointer
    __attribute__((unused)) uint8_t* _msgBufPtr =
        ((_Message_t*)le_msg_GetPayloadPtr(_msgRef))->buffer;
    __attribute__((unused)) size_t _msgBufSize = _MAX_MSG_SIZE;

    // Unpack which outputs are needed.
    _serverCmdPtr->requiredOutputs = 0;

    // Unpack the input parameters from the message
    char treeName[65];
    if (!le_pack_UnpackString( &_msgBufPtr, &_msgBufSize,
                               treeName,
                               sizeof(treeName),
                               64 ))
    {
        goto error_unpack;
    }

    // Call the function
    le_cfgAdmin_DeleteTree ( _serverCmdPtr, treeName );

    return;

error_unpack:
    le_mem_Release(_serverCmdPtr);

    LE_KILL_CLIENT("Error unpacking inputs");
}


//--------------------------------------------------------------------------------------------------
/**
 * Server-side respond function for le_cfgAdmin_CreateTreeIterator
 */
//--------------------------------------------------------------------------------------------------
void le_cfgAdmin_CreateTreeIteratorRespond
(
    le_cfgAdmin_ServerCmdRef_t _cmdRef,
    le_cfgAdmin_IteratorRef_t _result
)
{
    LE_ASSERT(_cmdRef != NULL);

    // Get the message related data
    le_msg_MessageRef_t _msgRef = _cmdRef->msgRef;
    _Message_t* _msgPtr = le_msg_GetPayloadPtr(_msgRef);
    __attribute__((unused)) uint8_t* _msgBufPtr = _msgPtr->buffer;
    __attribute__((unused)) size_t _msgBufSize = _MAX_MSG_SIZE;

    // Ensure the passed in msgRef is for the correct message
    LE_ASSERT(_msgPtr->id == _MSGID_le_cfgAdmin_CreateTreeIterator);

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

static void Handle_le_cfgAdmin_CreateTreeIterator
(
    le_msg_MessageRef_t _msgRef
)
{
    // Create a server command object
    le_cfgAdmin_ServerCmd_t* _serverCmdPtr = le_mem_ForceAlloc(_ServerCmdPool);
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
    le_cfgAdmin_CreateTreeIterator ( _serverCmdPtr );

    return;
}


//--------------------------------------------------------------------------------------------------
/**
 * Server-side respond function for le_cfgAdmin_ReleaseTreeIterator
 */
//--------------------------------------------------------------------------------------------------
void le_cfgAdmin_ReleaseTreeIteratorRespond
(
    le_cfgAdmin_ServerCmdRef_t _cmdRef
)
{
    LE_ASSERT(_cmdRef != NULL);

    // Get the message related data
    le_msg_MessageRef_t _msgRef = _cmdRef->msgRef;
    _Message_t* _msgPtr = le_msg_GetPayloadPtr(_msgRef);
    __attribute__((unused)) uint8_t* _msgBufPtr = _msgPtr->buffer;
    __attribute__((unused)) size_t _msgBufSize = _MAX_MSG_SIZE;

    // Ensure the passed in msgRef is for the correct message
    LE_ASSERT(_msgPtr->id == _MSGID_le_cfgAdmin_ReleaseTreeIterator);

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

static void Handle_le_cfgAdmin_ReleaseTreeIterator
(
    le_msg_MessageRef_t _msgRef
)
{
    // Create a server command object
    le_cfgAdmin_ServerCmd_t* _serverCmdPtr = le_mem_ForceAlloc(_ServerCmdPool);
    _serverCmdPtr->cmdLink = LE_DLS_LINK_INIT;
    _serverCmdPtr->msgRef = _msgRef;

    // Get the message buffer pointer
    __attribute__((unused)) uint8_t* _msgBufPtr =
        ((_Message_t*)le_msg_GetPayloadPtr(_msgRef))->buffer;
    __attribute__((unused)) size_t _msgBufSize = _MAX_MSG_SIZE;

    // Unpack which outputs are needed.
    _serverCmdPtr->requiredOutputs = 0;

    // Unpack the input parameters from the message
    le_cfgAdmin_IteratorRef_t iteratorRef;
    if (!le_pack_UnpackReference( &_msgBufPtr, &_msgBufSize,
                                               &iteratorRef ))
    {
        goto error_unpack;
    }

    // Call the function
    le_cfgAdmin_ReleaseTreeIterator ( _serverCmdPtr, iteratorRef );

    return;

error_unpack:
    le_mem_Release(_serverCmdPtr);

    LE_KILL_CLIENT("Error unpacking inputs");
}


//--------------------------------------------------------------------------------------------------
/**
 * Server-side respond function for le_cfgAdmin_GetTreeName
 */
//--------------------------------------------------------------------------------------------------
void le_cfgAdmin_GetTreeNameRespond
(
    le_cfgAdmin_ServerCmdRef_t _cmdRef,
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
    LE_ASSERT(_msgPtr->id == _MSGID_le_cfgAdmin_GetTreeName);

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
                                      name, 64 ));
    }

    // Return the response
    TRACE("Sending response to client session %p", le_msg_GetSession(_msgRef));

    le_msg_Respond(_msgRef);

    // Release the command
    le_mem_Release(_cmdRef);
}

static void Handle_le_cfgAdmin_GetTreeName
(
    le_msg_MessageRef_t _msgRef
)
{
    // Create a server command object
    le_cfgAdmin_ServerCmd_t* _serverCmdPtr = le_mem_ForceAlloc(_ServerCmdPool);
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
    le_cfgAdmin_IteratorRef_t iteratorRef;
    if (!le_pack_UnpackReference( &_msgBufPtr, &_msgBufSize,
                                               &iteratorRef ))
    {
        goto error_unpack;
    }
    size_t nameSize;
    if (!le_pack_UnpackSize( &_msgBufPtr, &_msgBufSize,
                               &nameSize ))
    {
        goto error_unpack;
    }
    if ( (nameSize > 64) &&
         (nameSize < UINT32_MAX) )
    {
        LE_DEBUG("Adjusting nameSize from %zu to 64", nameSize);
        nameSize = 64;
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
    le_cfgAdmin_GetTreeName ( _serverCmdPtr, iteratorRef, nameSize );

    return;

error_unpack:
    le_mem_Release(_serverCmdPtr);

    LE_KILL_CLIENT("Error unpacking inputs");
}


//--------------------------------------------------------------------------------------------------
/**
 * Server-side respond function for le_cfgAdmin_NextTree
 */
//--------------------------------------------------------------------------------------------------
void le_cfgAdmin_NextTreeRespond
(
    le_cfgAdmin_ServerCmdRef_t _cmdRef,
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
    LE_ASSERT(_msgPtr->id == _MSGID_le_cfgAdmin_NextTree);

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

static void Handle_le_cfgAdmin_NextTree
(
    le_msg_MessageRef_t _msgRef
)
{
    // Create a server command object
    le_cfgAdmin_ServerCmd_t* _serverCmdPtr = le_mem_ForceAlloc(_ServerCmdPool);
    _serverCmdPtr->cmdLink = LE_DLS_LINK_INIT;
    _serverCmdPtr->msgRef = _msgRef;

    // Get the message buffer pointer
    __attribute__((unused)) uint8_t* _msgBufPtr =
        ((_Message_t*)le_msg_GetPayloadPtr(_msgRef))->buffer;
    __attribute__((unused)) size_t _msgBufSize = _MAX_MSG_SIZE;

    // Unpack which outputs are needed.
    _serverCmdPtr->requiredOutputs = 0;

    // Unpack the input parameters from the message
    le_cfgAdmin_IteratorRef_t iteratorRef;
    if (!le_pack_UnpackReference( &_msgBufPtr, &_msgBufSize,
                                               &iteratorRef ))
    {
        goto error_unpack;
    }

    // Call the function
    le_cfgAdmin_NextTree ( _serverCmdPtr, iteratorRef );

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
        case _MSGID_le_cfgAdmin_ImportTree : Handle_le_cfgAdmin_ImportTree(msgRef); break;
        case _MSGID_le_cfgAdmin_ExportTree : Handle_le_cfgAdmin_ExportTree(msgRef); break;
        case _MSGID_le_cfgAdmin_DeleteTree : Handle_le_cfgAdmin_DeleteTree(msgRef); break;
        case _MSGID_le_cfgAdmin_CreateTreeIterator : Handle_le_cfgAdmin_CreateTreeIterator(msgRef); break;
        case _MSGID_le_cfgAdmin_ReleaseTreeIterator : Handle_le_cfgAdmin_ReleaseTreeIterator(msgRef); break;
        case _MSGID_le_cfgAdmin_GetTreeName : Handle_le_cfgAdmin_GetTreeName(msgRef); break;
        case _MSGID_le_cfgAdmin_NextTree : Handle_le_cfgAdmin_NextTree(msgRef); break;

        default: LE_ERROR("Unknowm msg id = %i", msgPtr->id);
    }

    // Clear the client session ref associated with the current message, since the message
    // has now been processed.
    _ClientSessionRef = 0;
}