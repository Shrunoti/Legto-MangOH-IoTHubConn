
/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */


#include "le_atServer_messages.h"
#include "le_atServer_server.h"


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
le_msg_ServiceRef_t le_atServer_GetServiceRef
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
le_msg_SessionRef_t le_atServer_GetClientSessionRef
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
void le_atServer_AdvertiseService
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
    _ServerDataPool = le_mem_CreatePool("le_atServer_ServerData", sizeof(_ServerData_t));

    // Create safe reference map for handler references.
    // The size of the map should be based on the number of handlers defined for the server.
    // Don't expect that to be more than 2-3, so use 3 as a reasonable guess.
    _HandlerRefMap = le_ref_CreateMap("le_atServer_ServerHandlers", 3);

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


static void Handle_le_atServer_Suspend
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
    le_atServer_DeviceRef_t device;
    if (!le_pack_UnpackReference( &_msgBufPtr, &_msgBufSize,
                                               &device ))
    {
        goto error_unpack;
    }

    // Define storage for output parameters

    // Call the function
    le_result_t _result;
    _result  = le_atServer_Suspend ( 
        device );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;
    _msgBufSize = _MAX_MSG_SIZE;

    // Pack the result first
    LE_ASSERT(le_pack_PackResult( &_msgBufPtr, &_msgBufSize, _result ));

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


static void Handle_le_atServer_Resume
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
    le_atServer_DeviceRef_t device;
    if (!le_pack_UnpackReference( &_msgBufPtr, &_msgBufSize,
                                               &device ))
    {
        goto error_unpack;
    }

    // Define storage for output parameters

    // Call the function
    le_result_t _result;
    _result  = le_atServer_Resume ( 
        device );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;
    _msgBufSize = _MAX_MSG_SIZE;

    // Pack the result first
    LE_ASSERT(le_pack_PackResult( &_msgBufPtr, &_msgBufSize, _result ));

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


static void Handle_le_atServer_Open
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
    int fd;
    fd = le_msg_GetFd(_msgRef);

    // Define storage for output parameters

    // Call the function
    le_atServer_DeviceRef_t _result;
    _result  = le_atServer_Open ( 
        fd );

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
}


static void Handle_le_atServer_Close
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
    le_atServer_DeviceRef_t device;
    if (!le_pack_UnpackReference( &_msgBufPtr, &_msgBufSize,
                                               &device ))
    {
        goto error_unpack;
    }

    // Define storage for output parameters

    // Call the function
    le_result_t _result;
    _result  = le_atServer_Close ( 
        device );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;
    _msgBufSize = _MAX_MSG_SIZE;

    // Pack the result first
    LE_ASSERT(le_pack_PackResult( &_msgBufPtr, &_msgBufSize, _result ));

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


static void Handle_le_atServer_Create
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
    char name[512];
    if (!le_pack_UnpackString( &_msgBufPtr, &_msgBufSize,
                               name,
                               sizeof(name),
                               511 ))
    {
        goto error_unpack;
    }

    // Define storage for output parameters

    // Call the function
    le_atServer_CmdRef_t _result;
    _result  = le_atServer_Create ( 
        name );

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


static void Handle_le_atServer_Delete
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
    le_atServer_CmdRef_t commandRef;
    if (!le_pack_UnpackReference( &_msgBufPtr, &_msgBufSize,
                                               &commandRef ))
    {
        goto error_unpack;
    }

    // Define storage for output parameters

    // Call the function
    le_result_t _result;
    _result  = le_atServer_Delete ( 
        commandRef );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;
    _msgBufSize = _MAX_MSG_SIZE;

    // Pack the result first
    LE_ASSERT(le_pack_PackResult( &_msgBufPtr, &_msgBufSize, _result ));

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


static void AsyncResponse_le_atServer_AddCommandHandler
(
    le_atServer_CmdRef_t commandRef,
    le_atServer_Type_t type,
    uint32_t parametersNumber,
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
    _msgPtr->id = _MSGID_le_atServer_AddCommandHandler;
    _msgBufPtr = _msgPtr->buffer;
    _msgBufSize = _MAX_MSG_SIZE;

    // Always pack the client context pointer first
    LE_ASSERT(le_pack_PackReference( &_msgBufPtr, &_msgBufSize, serverDataPtr->contextPtr ))

    // Pack the input parameters
    
    LE_ASSERT(le_pack_PackReference( &_msgBufPtr, &_msgBufSize,
                                                  commandRef ));
    LE_ASSERT(le_pack_PackUint32( &_msgBufPtr, &_msgBufSize,
                                                  type ));
    LE_ASSERT(le_pack_PackUint32( &_msgBufPtr, &_msgBufSize,
                                                  parametersNumber ));

    // Send the async response to the client
    TRACE("Sending message to client session %p : %ti bytes sent",
          serverDataPtr->clientSessionRef,
          _msgBufPtr-_msgPtr->buffer);

    SendMsgToClient(_msgRef);
}


static void Handle_le_atServer_AddCommandHandler
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
    le_atServer_CmdRef_t commandRef;
    if (!le_pack_UnpackReference( &_msgBufPtr, &_msgBufSize,
                                               &commandRef ))
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
    le_atServer_CommandHandlerRef_t _result;
    _result  = le_atServer_AddCommandHandler ( 
        commandRef, AsyncResponse_le_atServer_AddCommandHandler, 
        contextPtr );

    if (_result)
    {
        // Put the handler reference result and a pointer to the associated remove function
        // into the server data object.  This function pointer is needed in case the client
        // is closed and the handlers need to be removed.
        serverDataPtr->handlerRef = (le_event_HandlerRef_t)_result;
        serverDataPtr->removeHandlerFunc =
            (RemoveHandlerFunc_t)le_atServer_RemoveCommandHandler;

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


static void Handle_le_atServer_RemoveCommandHandler
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
    le_atServer_CommandHandlerRef_t handlerRef;
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
    handlerRef = (le_atServer_CommandHandlerRef_t)serverDataPtr->handlerRef;
    le_mem_Release(serverDataPtr);

    // Define storage for output parameters

    // Call the function
    le_atServer_RemoveCommandHandler ( 
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


static void Handle_le_atServer_GetParameter
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
    uint32_t _requiredOutputs = 0;
    if (!le_pack_UnpackUint32(&_msgBufPtr, &_msgBufSize, &_requiredOutputs))
    {
        goto error_unpack;
    }

    // Unpack the input parameters from the message
    le_atServer_CmdRef_t commandRef;
    if (!le_pack_UnpackReference( &_msgBufPtr, &_msgBufSize,
                                               &commandRef ))
    {
        goto error_unpack;
    }
    uint32_t index;
    if (!le_pack_UnpackUint32( &_msgBufPtr, &_msgBufSize,
                                               &index ))
    {
        goto error_unpack;
    }
    size_t parameterSize;
    if (!le_pack_UnpackSize( &_msgBufPtr, &_msgBufSize,
                               &parameterSize ))
    {
        goto error_unpack;
    }
    if ( (parameterSize > 127) &&
         (parameterSize < UINT32_MAX) )
    {
        LE_DEBUG("Adjusting parameterSize from %zu to 127", parameterSize);
        parameterSize = 127;
    }
    if (parameterSize >= UINT32_MAX)
    {
        parameterSize = 0;
    }
    else
    {
        parameterSize++;
    }

    // Define storage for output parameters
    char parameterBuffer[128];
    char *parameter = parameterBuffer;
    parameter[0] = 0;
    if (!(_requiredOutputs & (1u << 0)))
    {
        parameter = NULL;
        parameterSize = 0;
    }

    // Call the function
    le_result_t _result;
    _result  = le_atServer_GetParameter ( 
        commandRef, 
        index, 
        parameter, 
        parameterSize );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;
    _msgBufSize = _MAX_MSG_SIZE;

    // Pack the result first
    LE_ASSERT(le_pack_PackResult( &_msgBufPtr, &_msgBufSize, _result ));

    // Pack any "out" parameters
    if (parameter)
    {
        LE_ASSERT(le_pack_PackString( &_msgBufPtr, &_msgBufSize,
                                      parameter, 127 ));
    }

    // Return the response
    TRACE("Sending response to client session %p : %ti bytes sent",
          le_msg_GetSession(_msgRef),
          _msgBufPtr-_msgBufStartPtr);


    le_msg_Respond(_msgRef);

    return;

error_unpack:
    LE_KILL_CLIENT("Error unpacking message");
}


static void Handle_le_atServer_GetCommandName
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
    uint32_t _requiredOutputs = 0;
    if (!le_pack_UnpackUint32(&_msgBufPtr, &_msgBufSize, &_requiredOutputs))
    {
        goto error_unpack;
    }

    // Unpack the input parameters from the message
    le_atServer_CmdRef_t commandRef;
    if (!le_pack_UnpackReference( &_msgBufPtr, &_msgBufSize,
                                               &commandRef ))
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

    // Define storage for output parameters
    char nameBuffer[512];
    char *name = nameBuffer;
    name[0] = 0;
    if (!(_requiredOutputs & (1u << 0)))
    {
        name = NULL;
        nameSize = 0;
    }

    // Call the function
    le_result_t _result;
    _result  = le_atServer_GetCommandName ( 
        commandRef, 
        name, 
        nameSize );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;
    _msgBufSize = _MAX_MSG_SIZE;

    // Pack the result first
    LE_ASSERT(le_pack_PackResult( &_msgBufPtr, &_msgBufSize, _result ));

    // Pack any "out" parameters
    if (name)
    {
        LE_ASSERT(le_pack_PackString( &_msgBufPtr, &_msgBufSize,
                                      name, 511 ));
    }

    // Return the response
    TRACE("Sending response to client session %p : %ti bytes sent",
          le_msg_GetSession(_msgRef),
          _msgBufPtr-_msgBufStartPtr);


    le_msg_Respond(_msgRef);

    return;

error_unpack:
    LE_KILL_CLIENT("Error unpacking message");
}


static void Handle_le_atServer_GetDevice
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
    uint32_t _requiredOutputs = 0;
    if (!le_pack_UnpackUint32(&_msgBufPtr, &_msgBufSize, &_requiredOutputs))
    {
        goto error_unpack;
    }

    // Unpack the input parameters from the message
    le_atServer_CmdRef_t commandRef;
    if (!le_pack_UnpackReference( &_msgBufPtr, &_msgBufSize,
                                               &commandRef ))
    {
        goto error_unpack;
    }

    // Define storage for output parameters
    le_atServer_DeviceRef_t deviceRefBuffer;
    le_atServer_DeviceRef_t *deviceRefPtr = &deviceRefBuffer;
    if (!(_requiredOutputs & (1u << 0)))
    {
        deviceRefPtr = NULL;
    }

    // Call the function
    le_result_t _result;
    _result  = le_atServer_GetDevice ( 
        commandRef, 
        deviceRefPtr );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;
    _msgBufSize = _MAX_MSG_SIZE;

    // Pack the result first
    LE_ASSERT(le_pack_PackResult( &_msgBufPtr, &_msgBufSize, _result ));

    // Pack any "out" parameters
    if (deviceRefPtr)
    {
        LE_ASSERT(le_pack_PackReference( &_msgBufPtr, &_msgBufSize,
                                                      *deviceRefPtr ));
    }

    // Return the response
    TRACE("Sending response to client session %p : %ti bytes sent",
          le_msg_GetSession(_msgRef),
          _msgBufPtr-_msgBufStartPtr);


    le_msg_Respond(_msgRef);

    return;

error_unpack:
    LE_KILL_CLIENT("Error unpacking message");
}


static void Handle_le_atServer_SendIntermediateResponse
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
    le_atServer_CmdRef_t commandRef;
    if (!le_pack_UnpackReference( &_msgBufPtr, &_msgBufSize,
                                               &commandRef ))
    {
        goto error_unpack;
    }
    char intermediateRsp[514];
    if (!le_pack_UnpackString( &_msgBufPtr, &_msgBufSize,
                               intermediateRsp,
                               sizeof(intermediateRsp),
                               513 ))
    {
        goto error_unpack;
    }

    // Define storage for output parameters

    // Call the function
    le_result_t _result;
    _result  = le_atServer_SendIntermediateResponse ( 
        commandRef, 
        intermediateRsp );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;
    _msgBufSize = _MAX_MSG_SIZE;

    // Pack the result first
    LE_ASSERT(le_pack_PackResult( &_msgBufPtr, &_msgBufSize, _result ));

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


static void Handle_le_atServer_SendFinalResponse
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
    le_atServer_CmdRef_t commandRef;
    if (!le_pack_UnpackReference( &_msgBufPtr, &_msgBufSize,
                                               &commandRef ))
    {
        goto error_unpack;
    }
    le_atServer_FinalRsp_t finalResponse;
    if (!le_pack_UnpackUint32( &_msgBufPtr, &_msgBufSize,
                                               &finalResponse ))
    {
        goto error_unpack;
    }
    bool customStringAvailable;
    if (!le_pack_UnpackBool( &_msgBufPtr, &_msgBufSize,
                                               &customStringAvailable ))
    {
        goto error_unpack;
    }
    char finalRsp[514];
    if (!le_pack_UnpackString( &_msgBufPtr, &_msgBufSize,
                               finalRsp,
                               sizeof(finalRsp),
                               513 ))
    {
        goto error_unpack;
    }

    // Define storage for output parameters

    // Call the function
    le_result_t _result;
    _result  = le_atServer_SendFinalResponse ( 
        commandRef, 
        finalResponse, 
        customStringAvailable, 
        finalRsp );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;
    _msgBufSize = _MAX_MSG_SIZE;

    // Pack the result first
    LE_ASSERT(le_pack_PackResult( &_msgBufPtr, &_msgBufSize, _result ));

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


static void Handle_le_atServer_SendFinalResultCode
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
    le_atServer_CmdRef_t commandRef;
    if (!le_pack_UnpackReference( &_msgBufPtr, &_msgBufSize,
                                               &commandRef ))
    {
        goto error_unpack;
    }
    le_atServer_FinalRsp_t finalResult;
    if (!le_pack_UnpackUint32( &_msgBufPtr, &_msgBufSize,
                                               &finalResult ))
    {
        goto error_unpack;
    }
    char pattern[514];
    if (!le_pack_UnpackString( &_msgBufPtr, &_msgBufSize,
                               pattern,
                               sizeof(pattern),
                               513 ))
    {
        goto error_unpack;
    }
    uint32_t errorCode;
    if (!le_pack_UnpackUint32( &_msgBufPtr, &_msgBufSize,
                                               &errorCode ))
    {
        goto error_unpack;
    }

    // Define storage for output parameters

    // Call the function
    le_result_t _result;
    _result  = le_atServer_SendFinalResultCode ( 
        commandRef, 
        finalResult, 
        pattern, 
        errorCode );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;
    _msgBufSize = _MAX_MSG_SIZE;

    // Pack the result first
    LE_ASSERT(le_pack_PackResult( &_msgBufPtr, &_msgBufSize, _result ));

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


static void Handle_le_atServer_SendUnsolicitedResponse
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
    char unsolRsp[514];
    if (!le_pack_UnpackString( &_msgBufPtr, &_msgBufSize,
                               unsolRsp,
                               sizeof(unsolRsp),
                               513 ))
    {
        goto error_unpack;
    }
    le_atServer_AvailableDevice_t availableDevice;
    if (!le_pack_UnpackUint32( &_msgBufPtr, &_msgBufSize,
                                               &availableDevice ))
    {
        goto error_unpack;
    }
    le_atServer_DeviceRef_t device;
    if (!le_pack_UnpackReference( &_msgBufPtr, &_msgBufSize,
                                               &device ))
    {
        goto error_unpack;
    }

    // Define storage for output parameters

    // Call the function
    le_result_t _result;
    _result  = le_atServer_SendUnsolicitedResponse ( 
        unsolRsp, 
        availableDevice, 
        device );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;
    _msgBufSize = _MAX_MSG_SIZE;

    // Pack the result first
    LE_ASSERT(le_pack_PackResult( &_msgBufPtr, &_msgBufSize, _result ));

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


static void Handle_le_atServer_EnableEcho
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
    le_atServer_DeviceRef_t device;
    if (!le_pack_UnpackReference( &_msgBufPtr, &_msgBufSize,
                                               &device ))
    {
        goto error_unpack;
    }

    // Define storage for output parameters

    // Call the function
    le_result_t _result;
    _result  = le_atServer_EnableEcho ( 
        device );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;
    _msgBufSize = _MAX_MSG_SIZE;

    // Pack the result first
    LE_ASSERT(le_pack_PackResult( &_msgBufPtr, &_msgBufSize, _result ));

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


static void Handle_le_atServer_DisableEcho
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
    le_atServer_DeviceRef_t device;
    if (!le_pack_UnpackReference( &_msgBufPtr, &_msgBufSize,
                                               &device ))
    {
        goto error_unpack;
    }

    // Define storage for output parameters

    // Call the function
    le_result_t _result;
    _result  = le_atServer_DisableEcho ( 
        device );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;
    _msgBufSize = _MAX_MSG_SIZE;

    // Pack the result first
    LE_ASSERT(le_pack_PackResult( &_msgBufPtr, &_msgBufSize, _result ));

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


static void Handle_le_atServer_OpenBridge
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
    int fd;
    fd = le_msg_GetFd(_msgRef);

    // Define storage for output parameters

    // Call the function
    le_atServer_BridgeRef_t _result;
    _result  = le_atServer_OpenBridge ( 
        fd );

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
}


static void Handle_le_atServer_CloseBridge
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
    le_atServer_BridgeRef_t bridgeRef;
    if (!le_pack_UnpackReference( &_msgBufPtr, &_msgBufSize,
                                               &bridgeRef ))
    {
        goto error_unpack;
    }

    // Define storage for output parameters

    // Call the function
    le_result_t _result;
    _result  = le_atServer_CloseBridge ( 
        bridgeRef );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;
    _msgBufSize = _MAX_MSG_SIZE;

    // Pack the result first
    LE_ASSERT(le_pack_PackResult( &_msgBufPtr, &_msgBufSize, _result ));

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


static void Handle_le_atServer_AddDeviceToBridge
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
    le_atServer_DeviceRef_t deviceRef;
    if (!le_pack_UnpackReference( &_msgBufPtr, &_msgBufSize,
                                               &deviceRef ))
    {
        goto error_unpack;
    }
    le_atServer_BridgeRef_t bridgeRef;
    if (!le_pack_UnpackReference( &_msgBufPtr, &_msgBufSize,
                                               &bridgeRef ))
    {
        goto error_unpack;
    }

    // Define storage for output parameters

    // Call the function
    le_result_t _result;
    _result  = le_atServer_AddDeviceToBridge ( 
        deviceRef, 
        bridgeRef );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;
    _msgBufSize = _MAX_MSG_SIZE;

    // Pack the result first
    LE_ASSERT(le_pack_PackResult( &_msgBufPtr, &_msgBufSize, _result ));

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


static void Handle_le_atServer_RemoveDeviceFromBridge
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
    le_atServer_DeviceRef_t deviceRef;
    if (!le_pack_UnpackReference( &_msgBufPtr, &_msgBufSize,
                                               &deviceRef ))
    {
        goto error_unpack;
    }
    le_atServer_BridgeRef_t bridgeRef;
    if (!le_pack_UnpackReference( &_msgBufPtr, &_msgBufSize,
                                               &bridgeRef ))
    {
        goto error_unpack;
    }

    // Define storage for output parameters

    // Call the function
    le_result_t _result;
    _result  = le_atServer_RemoveDeviceFromBridge ( 
        deviceRef, 
        bridgeRef );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;
    _msgBufSize = _MAX_MSG_SIZE;

    // Pack the result first
    LE_ASSERT(le_pack_PackResult( &_msgBufPtr, &_msgBufSize, _result ));

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


static void Handle_le_atServer_EnableVerboseErrorCodes
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

    // Define storage for output parameters

    // Call the function
    le_atServer_EnableVerboseErrorCodes (  );

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
}


static void Handle_le_atServer_EnableExtendedErrorCodes
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

    // Define storage for output parameters

    // Call the function
    le_atServer_EnableExtendedErrorCodes (  );

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
}


static void Handle_le_atServer_DisableExtendedErrorCodes
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

    // Define storage for output parameters

    // Call the function
    le_atServer_DisableExtendedErrorCodes (  );

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
}


static void Handle_le_atServer_CreateErrorCode
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
    uint32_t errorCode;
    if (!le_pack_UnpackUint32( &_msgBufPtr, &_msgBufSize,
                                               &errorCode ))
    {
        goto error_unpack;
    }
    char pattern[514];
    if (!le_pack_UnpackString( &_msgBufPtr, &_msgBufSize,
                               pattern,
                               sizeof(pattern),
                               513 ))
    {
        goto error_unpack;
    }

    // Define storage for output parameters

    // Call the function
    le_atServer_ErrorCodeRef_t _result;
    _result  = le_atServer_CreateErrorCode ( 
        errorCode, 
        pattern );

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


static void Handle_le_atServer_DeleteErrorCode
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
    le_atServer_ErrorCodeRef_t errorCodeRef;
    if (!le_pack_UnpackReference( &_msgBufPtr, &_msgBufSize,
                                               &errorCodeRef ))
    {
        goto error_unpack;
    }

    // Define storage for output parameters

    // Call the function
    le_result_t _result;
    _result  = le_atServer_DeleteErrorCode ( 
        errorCodeRef );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;
    _msgBufSize = _MAX_MSG_SIZE;

    // Pack the result first
    LE_ASSERT(le_pack_PackResult( &_msgBufPtr, &_msgBufSize, _result ));

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


static void Handle_le_atServer_SetVerboseErrorCode
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
    le_atServer_ErrorCodeRef_t errorCodeRef;
    if (!le_pack_UnpackReference( &_msgBufPtr, &_msgBufSize,
                                               &errorCodeRef ))
    {
        goto error_unpack;
    }
    char verboseCode[514];
    if (!le_pack_UnpackString( &_msgBufPtr, &_msgBufSize,
                               verboseCode,
                               sizeof(verboseCode),
                               513 ))
    {
        goto error_unpack;
    }

    // Define storage for output parameters

    // Call the function
    le_result_t _result;
    _result  = le_atServer_SetVerboseErrorCode ( 
        errorCodeRef, 
        verboseCode );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;
    _msgBufSize = _MAX_MSG_SIZE;

    // Pack the result first
    LE_ASSERT(le_pack_PackResult( &_msgBufPtr, &_msgBufSize, _result ));

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


static void AsyncResponse_le_atServer_GetTextAsync
(
    le_atServer_CmdRef_t cmdRef,
    le_result_t result,
    const char* LE_NONNULL text,
    uint32_t len,
    void* contextPtr
)
{
    le_msg_MessageRef_t _msgRef;
    _Message_t* _msgPtr;
    _ServerData_t* serverDataPtr = (_ServerData_t*)contextPtr;

    // This is a one-time handler; if the server accidently calls it a second time, then
    // the client sesssion ref would be NULL.
    if ( serverDataPtr->clientSessionRef == NULL )
    {
        LE_FATAL("Handler passed to le_atServer_GetTextAsync() can't be called more than once");
    }
    

    // Will not be used if no data is sent back to client
    __attribute__((unused)) uint8_t* _msgBufPtr;
    __attribute__((unused)) size_t _msgBufSize;

    // Create a new message object and get the message buffer
    _msgRef = le_msg_CreateMsg(serverDataPtr->clientSessionRef);
    _msgPtr = le_msg_GetPayloadPtr(_msgRef);
    _msgPtr->id = _MSGID_le_atServer_GetTextAsync;
    _msgBufPtr = _msgPtr->buffer;
    _msgBufSize = _MAX_MSG_SIZE;

    // Always pack the client context pointer first
    LE_ASSERT(le_pack_PackReference( &_msgBufPtr, &_msgBufSize, serverDataPtr->contextPtr ))

    // Pack the input parameters
    
    LE_ASSERT(le_pack_PackReference( &_msgBufPtr, &_msgBufSize,
                                                  cmdRef ));
    LE_ASSERT(le_pack_PackResult( &_msgBufPtr, &_msgBufSize,
                                                  result ));
    LE_ASSERT(le_pack_PackString( &_msgBufPtr, &_msgBufSize,
                                  text, 4095 ));
    LE_ASSERT(le_pack_PackUint32( &_msgBufPtr, &_msgBufSize,
                                                  len ));

    // Send the async response to the client
    TRACE("Sending message to client session %p : %ti bytes sent",
          serverDataPtr->clientSessionRef,
          _msgBufPtr-_msgPtr->buffer);

    SendMsgToClient(_msgRef);

    // The registered handler has been called, so no longer need the server data.
    // Explicitly set clientSessionRef to NULL, so that we can catch if this function gets
    // accidently called again.
    serverDataPtr->clientSessionRef = NULL;
    le_mem_Release(serverDataPtr);
}


static void Handle_le_atServer_GetTextAsync
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
    le_atServer_CmdRef_t cmdRef;
    if (!le_pack_UnpackReference( &_msgBufPtr, &_msgBufSize,
                                               &cmdRef ))
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
    le_result_t _result;
    _result  = le_atServer_GetTextAsync ( 
        cmdRef, AsyncResponse_le_atServer_GetTextAsync, 
        contextPtr );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;
    _msgBufSize = _MAX_MSG_SIZE;

    // Pack the result first
    LE_ASSERT(le_pack_PackResult( &_msgBufPtr, &_msgBufSize, _result ));

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
        case _MSGID_le_atServer_Suspend : Handle_le_atServer_Suspend(msgRef); break;
        case _MSGID_le_atServer_Resume : Handle_le_atServer_Resume(msgRef); break;
        case _MSGID_le_atServer_Open : Handle_le_atServer_Open(msgRef); break;
        case _MSGID_le_atServer_Close : Handle_le_atServer_Close(msgRef); break;
        case _MSGID_le_atServer_Create : Handle_le_atServer_Create(msgRef); break;
        case _MSGID_le_atServer_Delete : Handle_le_atServer_Delete(msgRef); break;
        case _MSGID_le_atServer_AddCommandHandler : Handle_le_atServer_AddCommandHandler(msgRef); break;
        case _MSGID_le_atServer_RemoveCommandHandler : Handle_le_atServer_RemoveCommandHandler(msgRef); break;
        case _MSGID_le_atServer_GetParameter : Handle_le_atServer_GetParameter(msgRef); break;
        case _MSGID_le_atServer_GetCommandName : Handle_le_atServer_GetCommandName(msgRef); break;
        case _MSGID_le_atServer_GetDevice : Handle_le_atServer_GetDevice(msgRef); break;
        case _MSGID_le_atServer_SendIntermediateResponse : Handle_le_atServer_SendIntermediateResponse(msgRef); break;
        case _MSGID_le_atServer_SendFinalResponse : Handle_le_atServer_SendFinalResponse(msgRef); break;
        case _MSGID_le_atServer_SendFinalResultCode : Handle_le_atServer_SendFinalResultCode(msgRef); break;
        case _MSGID_le_atServer_SendUnsolicitedResponse : Handle_le_atServer_SendUnsolicitedResponse(msgRef); break;
        case _MSGID_le_atServer_EnableEcho : Handle_le_atServer_EnableEcho(msgRef); break;
        case _MSGID_le_atServer_DisableEcho : Handle_le_atServer_DisableEcho(msgRef); break;
        case _MSGID_le_atServer_OpenBridge : Handle_le_atServer_OpenBridge(msgRef); break;
        case _MSGID_le_atServer_CloseBridge : Handle_le_atServer_CloseBridge(msgRef); break;
        case _MSGID_le_atServer_AddDeviceToBridge : Handle_le_atServer_AddDeviceToBridge(msgRef); break;
        case _MSGID_le_atServer_RemoveDeviceFromBridge : Handle_le_atServer_RemoveDeviceFromBridge(msgRef); break;
        case _MSGID_le_atServer_EnableVerboseErrorCodes : Handle_le_atServer_EnableVerboseErrorCodes(msgRef); break;
        case _MSGID_le_atServer_EnableExtendedErrorCodes : Handle_le_atServer_EnableExtendedErrorCodes(msgRef); break;
        case _MSGID_le_atServer_DisableExtendedErrorCodes : Handle_le_atServer_DisableExtendedErrorCodes(msgRef); break;
        case _MSGID_le_atServer_CreateErrorCode : Handle_le_atServer_CreateErrorCode(msgRef); break;
        case _MSGID_le_atServer_DeleteErrorCode : Handle_le_atServer_DeleteErrorCode(msgRef); break;
        case _MSGID_le_atServer_SetVerboseErrorCode : Handle_le_atServer_SetVerboseErrorCode(msgRef); break;
        case _MSGID_le_atServer_GetTextAsync : Handle_le_atServer_GetTextAsync(msgRef); break;

        default: LE_ERROR("Unknowm msg id = %i", msgPtr->id);
    }

    // Clear the client session ref associated with the current message, since the message
    // has now been processed.
    _ClientSessionRef = 0;
}