
/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */


#include "le_mrc_messages.h"
#include "le_mrc_server.h"


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
le_msg_ServiceRef_t le_mrc_GetServiceRef
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
le_msg_SessionRef_t le_mrc_GetClientSessionRef
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
void le_mrc_AdvertiseService
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
    _ServerDataPool = le_mem_CreatePool("le_mrc_ServerData", sizeof(_ServerData_t));

    // Create safe reference map for handler references.
    // The size of the map should be based on the number of handlers defined for the server.
    // Don't expect that to be more than 2-3, so use 3 as a reasonable guess.
    _HandlerRefMap = le_ref_CreateMap("le_mrc_ServerHandlers", 3);

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


static void AsyncResponse_le_mrc_AddNetRegStateEventHandler
(
    le_mrc_NetRegState_t state,
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
    _msgPtr->id = _MSGID_le_mrc_AddNetRegStateEventHandler;
    _msgBufPtr = _msgPtr->buffer;
    _msgBufSize = _MAX_MSG_SIZE;

    // Always pack the client context pointer first
    LE_ASSERT(le_pack_PackReference( &_msgBufPtr, &_msgBufSize, serverDataPtr->contextPtr ))

    // Pack the input parameters
    
    LE_ASSERT(le_pack_PackUint32( &_msgBufPtr, &_msgBufSize,
                                                  state ));

    // Send the async response to the client
    TRACE("Sending message to client session %p : %ti bytes sent",
          serverDataPtr->clientSessionRef,
          _msgBufPtr-_msgPtr->buffer);

    SendMsgToClient(_msgRef);
}


static void Handle_le_mrc_AddNetRegStateEventHandler
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
    le_mrc_NetRegStateEventHandlerRef_t _result;
    _result  = le_mrc_AddNetRegStateEventHandler ( AsyncResponse_le_mrc_AddNetRegStateEventHandler, 
        contextPtr );

    if (_result)
    {
        // Put the handler reference result and a pointer to the associated remove function
        // into the server data object.  This function pointer is needed in case the client
        // is closed and the handlers need to be removed.
        serverDataPtr->handlerRef = (le_event_HandlerRef_t)_result;
        serverDataPtr->removeHandlerFunc =
            (RemoveHandlerFunc_t)le_mrc_RemoveNetRegStateEventHandler;

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


static void Handle_le_mrc_RemoveNetRegStateEventHandler
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
    le_mrc_NetRegStateEventHandlerRef_t handlerRef;
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
    handlerRef = (le_mrc_NetRegStateEventHandlerRef_t)serverDataPtr->handlerRef;
    le_mem_Release(serverDataPtr);

    // Define storage for output parameters

    // Call the function
    le_mrc_RemoveNetRegStateEventHandler ( 
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


static void AsyncResponse_le_mrc_AddRatChangeHandler
(
    le_mrc_Rat_t rat,
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
    _msgPtr->id = _MSGID_le_mrc_AddRatChangeHandler;
    _msgBufPtr = _msgPtr->buffer;
    _msgBufSize = _MAX_MSG_SIZE;

    // Always pack the client context pointer first
    LE_ASSERT(le_pack_PackReference( &_msgBufPtr, &_msgBufSize, serverDataPtr->contextPtr ))

    // Pack the input parameters
    
    LE_ASSERT(le_pack_PackUint32( &_msgBufPtr, &_msgBufSize,
                                                  rat ));

    // Send the async response to the client
    TRACE("Sending message to client session %p : %ti bytes sent",
          serverDataPtr->clientSessionRef,
          _msgBufPtr-_msgPtr->buffer);

    SendMsgToClient(_msgRef);
}


static void Handle_le_mrc_AddRatChangeHandler
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
    le_mrc_RatChangeHandlerRef_t _result;
    _result  = le_mrc_AddRatChangeHandler ( AsyncResponse_le_mrc_AddRatChangeHandler, 
        contextPtr );

    if (_result)
    {
        // Put the handler reference result and a pointer to the associated remove function
        // into the server data object.  This function pointer is needed in case the client
        // is closed and the handlers need to be removed.
        serverDataPtr->handlerRef = (le_event_HandlerRef_t)_result;
        serverDataPtr->removeHandlerFunc =
            (RemoveHandlerFunc_t)le_mrc_RemoveRatChangeHandler;

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


static void Handle_le_mrc_RemoveRatChangeHandler
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
    le_mrc_RatChangeHandlerRef_t handlerRef;
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
    handlerRef = (le_mrc_RatChangeHandlerRef_t)serverDataPtr->handlerRef;
    le_mem_Release(serverDataPtr);

    // Define storage for output parameters

    // Call the function
    le_mrc_RemoveRatChangeHandler ( 
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


static void AsyncResponse_le_mrc_AddPacketSwitchedChangeHandler
(
    le_mrc_NetRegState_t state,
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
    _msgPtr->id = _MSGID_le_mrc_AddPacketSwitchedChangeHandler;
    _msgBufPtr = _msgPtr->buffer;
    _msgBufSize = _MAX_MSG_SIZE;

    // Always pack the client context pointer first
    LE_ASSERT(le_pack_PackReference( &_msgBufPtr, &_msgBufSize, serverDataPtr->contextPtr ))

    // Pack the input parameters
    
    LE_ASSERT(le_pack_PackUint32( &_msgBufPtr, &_msgBufSize,
                                                  state ));

    // Send the async response to the client
    TRACE("Sending message to client session %p : %ti bytes sent",
          serverDataPtr->clientSessionRef,
          _msgBufPtr-_msgPtr->buffer);

    SendMsgToClient(_msgRef);
}


static void Handle_le_mrc_AddPacketSwitchedChangeHandler
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
    le_mrc_PacketSwitchedChangeHandlerRef_t _result;
    _result  = le_mrc_AddPacketSwitchedChangeHandler ( AsyncResponse_le_mrc_AddPacketSwitchedChangeHandler, 
        contextPtr );

    if (_result)
    {
        // Put the handler reference result and a pointer to the associated remove function
        // into the server data object.  This function pointer is needed in case the client
        // is closed and the handlers need to be removed.
        serverDataPtr->handlerRef = (le_event_HandlerRef_t)_result;
        serverDataPtr->removeHandlerFunc =
            (RemoveHandlerFunc_t)le_mrc_RemovePacketSwitchedChangeHandler;

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


static void Handle_le_mrc_RemovePacketSwitchedChangeHandler
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
    le_mrc_PacketSwitchedChangeHandlerRef_t handlerRef;
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
    handlerRef = (le_mrc_PacketSwitchedChangeHandlerRef_t)serverDataPtr->handlerRef;
    le_mem_Release(serverDataPtr);

    // Define storage for output parameters

    // Call the function
    le_mrc_RemovePacketSwitchedChangeHandler ( 
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


static void AsyncResponse_le_mrc_AddSignalStrengthChangeHandler
(
    int32_t ss,
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
    _msgPtr->id = _MSGID_le_mrc_AddSignalStrengthChangeHandler;
    _msgBufPtr = _msgPtr->buffer;
    _msgBufSize = _MAX_MSG_SIZE;

    // Always pack the client context pointer first
    LE_ASSERT(le_pack_PackReference( &_msgBufPtr, &_msgBufSize, serverDataPtr->contextPtr ))

    // Pack the input parameters
    
    LE_ASSERT(le_pack_PackInt32( &_msgBufPtr, &_msgBufSize,
                                                  ss ));

    // Send the async response to the client
    TRACE("Sending message to client session %p : %ti bytes sent",
          serverDataPtr->clientSessionRef,
          _msgBufPtr-_msgPtr->buffer);

    SendMsgToClient(_msgRef);
}


static void Handle_le_mrc_AddSignalStrengthChangeHandler
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
    le_mrc_Rat_t rat;
    if (!le_pack_UnpackUint32( &_msgBufPtr, &_msgBufSize,
                                               &rat ))
    {
        goto error_unpack;
    }
    int32_t lowerRangeThreshold;
    if (!le_pack_UnpackInt32( &_msgBufPtr, &_msgBufSize,
                                               &lowerRangeThreshold ))
    {
        goto error_unpack;
    }
    int32_t upperRangeThreshold;
    if (!le_pack_UnpackInt32( &_msgBufPtr, &_msgBufSize,
                                               &upperRangeThreshold ))
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
    le_mrc_SignalStrengthChangeHandlerRef_t _result;
    _result  = le_mrc_AddSignalStrengthChangeHandler ( 
        rat, 
        lowerRangeThreshold, 
        upperRangeThreshold, AsyncResponse_le_mrc_AddSignalStrengthChangeHandler, 
        contextPtr );

    if (_result)
    {
        // Put the handler reference result and a pointer to the associated remove function
        // into the server data object.  This function pointer is needed in case the client
        // is closed and the handlers need to be removed.
        serverDataPtr->handlerRef = (le_event_HandlerRef_t)_result;
        serverDataPtr->removeHandlerFunc =
            (RemoveHandlerFunc_t)le_mrc_RemoveSignalStrengthChangeHandler;

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


static void Handle_le_mrc_RemoveSignalStrengthChangeHandler
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
    le_mrc_SignalStrengthChangeHandlerRef_t handlerRef;
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
    handlerRef = (le_mrc_SignalStrengthChangeHandlerRef_t)serverDataPtr->handlerRef;
    le_mem_Release(serverDataPtr);

    // Define storage for output parameters

    // Call the function
    le_mrc_RemoveSignalStrengthChangeHandler ( 
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


static void Handle_le_mrc_SetSignalStrengthIndThresholds
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
    le_mrc_Rat_t rat;
    if (!le_pack_UnpackUint32( &_msgBufPtr, &_msgBufSize,
                                               &rat ))
    {
        goto error_unpack;
    }
    int32_t lowerRangeThreshold;
    if (!le_pack_UnpackInt32( &_msgBufPtr, &_msgBufSize,
                                               &lowerRangeThreshold ))
    {
        goto error_unpack;
    }
    int32_t upperRangeThreshold;
    if (!le_pack_UnpackInt32( &_msgBufPtr, &_msgBufSize,
                                               &upperRangeThreshold ))
    {
        goto error_unpack;
    }

    // Define storage for output parameters

    // Call the function
    le_result_t _result;
    _result  = le_mrc_SetSignalStrengthIndThresholds ( 
        rat, 
        lowerRangeThreshold, 
        upperRangeThreshold );

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


static void Handle_le_mrc_SetSignalStrengthIndDelta
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
    le_mrc_Rat_t rat;
    if (!le_pack_UnpackUint32( &_msgBufPtr, &_msgBufSize,
                                               &rat ))
    {
        goto error_unpack;
    }
    uint16_t delta;
    if (!le_pack_UnpackUint16( &_msgBufPtr, &_msgBufSize,
                                               &delta ))
    {
        goto error_unpack;
    }

    // Define storage for output parameters

    // Call the function
    le_result_t _result;
    _result  = le_mrc_SetSignalStrengthIndDelta ( 
        rat, 
        delta );

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


static void Handle_le_mrc_SetAutomaticRegisterMode
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
    le_result_t _result;
    _result  = le_mrc_SetAutomaticRegisterMode (  );

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
}


static void Handle_le_mrc_SetManualRegisterMode
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
    char mcc[4];
    if (!le_pack_UnpackString( &_msgBufPtr, &_msgBufSize,
                               mcc,
                               sizeof(mcc),
                               3 ))
    {
        goto error_unpack;
    }
    char mnc[4];
    if (!le_pack_UnpackString( &_msgBufPtr, &_msgBufSize,
                               mnc,
                               sizeof(mnc),
                               3 ))
    {
        goto error_unpack;
    }

    // Define storage for output parameters

    // Call the function
    le_result_t _result;
    _result  = le_mrc_SetManualRegisterMode ( 
        mcc, 
        mnc );

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


static void AsyncResponse_le_mrc_SetManualRegisterModeAsync
(
    le_result_t result,
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
        LE_FATAL("Handler passed to le_mrc_SetManualRegisterModeAsync() can't be called more than once");
    }
    

    // Will not be used if no data is sent back to client
    __attribute__((unused)) uint8_t* _msgBufPtr;
    __attribute__((unused)) size_t _msgBufSize;

    // Create a new message object and get the message buffer
    _msgRef = le_msg_CreateMsg(serverDataPtr->clientSessionRef);
    _msgPtr = le_msg_GetPayloadPtr(_msgRef);
    _msgPtr->id = _MSGID_le_mrc_SetManualRegisterModeAsync;
    _msgBufPtr = _msgPtr->buffer;
    _msgBufSize = _MAX_MSG_SIZE;

    // Always pack the client context pointer first
    LE_ASSERT(le_pack_PackReference( &_msgBufPtr, &_msgBufSize, serverDataPtr->contextPtr ))

    // Pack the input parameters
    
    LE_ASSERT(le_pack_PackResult( &_msgBufPtr, &_msgBufSize,
                                                  result ));

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


static void Handle_le_mrc_SetManualRegisterModeAsync
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
    char mcc[4];
    if (!le_pack_UnpackString( &_msgBufPtr, &_msgBufSize,
                               mcc,
                               sizeof(mcc),
                               3 ))
    {
        goto error_unpack;
    }
    char mnc[4];
    if (!le_pack_UnpackString( &_msgBufPtr, &_msgBufSize,
                               mnc,
                               sizeof(mnc),
                               3 ))
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
    le_mrc_SetManualRegisterModeAsync ( 
        mcc, 
        mnc, AsyncResponse_le_mrc_SetManualRegisterModeAsync, 
        contextPtr );

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


static void Handle_le_mrc_GetRegisterMode
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
    size_t mccPtrSize;
    if (!le_pack_UnpackSize( &_msgBufPtr, &_msgBufSize,
                               &mccPtrSize ))
    {
        goto error_unpack;
    }
    if ( (mccPtrSize > 3) &&
         (mccPtrSize < UINT32_MAX) )
    {
        LE_DEBUG("Adjusting mccPtrSize from %zu to 3", mccPtrSize);
        mccPtrSize = 3;
    }
    if (mccPtrSize >= UINT32_MAX)
    {
        mccPtrSize = 0;
    }
    else
    {
        mccPtrSize++;
    }
    size_t mncPtrSize;
    if (!le_pack_UnpackSize( &_msgBufPtr, &_msgBufSize,
                               &mncPtrSize ))
    {
        goto error_unpack;
    }
    if ( (mncPtrSize > 3) &&
         (mncPtrSize < UINT32_MAX) )
    {
        LE_DEBUG("Adjusting mncPtrSize from %zu to 3", mncPtrSize);
        mncPtrSize = 3;
    }
    if (mncPtrSize >= UINT32_MAX)
    {
        mncPtrSize = 0;
    }
    else
    {
        mncPtrSize++;
    }

    // Define storage for output parameters
    bool isManualPtrBuffer;
    bool *isManualPtrPtr = &isManualPtrBuffer;
    if (!(_requiredOutputs & (1u << 0)))
    {
        isManualPtrPtr = NULL;
    }
    char mccPtrBuffer[4];
    char *mccPtr = mccPtrBuffer;
    mccPtr[0] = 0;
    if (!(_requiredOutputs & (1u << 1)))
    {
        mccPtr = NULL;
        mccPtrSize = 0;
    }
    char mncPtrBuffer[4];
    char *mncPtr = mncPtrBuffer;
    mncPtr[0] = 0;
    if (!(_requiredOutputs & (1u << 2)))
    {
        mncPtr = NULL;
        mncPtrSize = 0;
    }

    // Call the function
    le_result_t _result;
    _result  = le_mrc_GetRegisterMode ( 
        isManualPtrPtr, 
        mccPtr, 
        mccPtrSize, 
        mncPtr, 
        mncPtrSize );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;
    _msgBufSize = _MAX_MSG_SIZE;

    // Pack the result first
    LE_ASSERT(le_pack_PackResult( &_msgBufPtr, &_msgBufSize, _result ));

    // Pack any "out" parameters
    if (isManualPtrPtr)
    {
        LE_ASSERT(le_pack_PackBool( &_msgBufPtr, &_msgBufSize,
                                                      *isManualPtrPtr ));
    }
    if (mccPtr)
    {
        LE_ASSERT(le_pack_PackString( &_msgBufPtr, &_msgBufSize,
                                      mccPtr, 3 ));
    }
    if (mncPtr)
    {
        LE_ASSERT(le_pack_PackString( &_msgBufPtr, &_msgBufSize,
                                      mncPtr, 3 ));
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


static void Handle_le_mrc_GetPacketSwitchedState
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

    // Define storage for output parameters
    le_mrc_NetRegState_t stateBuffer;
    le_mrc_NetRegState_t *statePtr = &stateBuffer;
    if (!(_requiredOutputs & (1u << 0)))
    {
        statePtr = NULL;
    }

    // Call the function
    le_result_t _result;
    _result  = le_mrc_GetPacketSwitchedState ( 
        statePtr );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;
    _msgBufSize = _MAX_MSG_SIZE;

    // Pack the result first
    LE_ASSERT(le_pack_PackResult( &_msgBufPtr, &_msgBufSize, _result ));

    // Pack any "out" parameters
    if (statePtr)
    {
        LE_ASSERT(le_pack_PackUint32( &_msgBufPtr, &_msgBufSize,
                                                      *statePtr ));
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


static void Handle_le_mrc_GetPlatformSpecificRegistrationErrorCode
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
    int32_t _result;
    _result  = le_mrc_GetPlatformSpecificRegistrationErrorCode (  );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;
    _msgBufSize = _MAX_MSG_SIZE;

    // Pack the result first
    LE_ASSERT(le_pack_PackInt32( &_msgBufPtr, &_msgBufSize, _result ));

    // Pack any "out" parameters

    // Return the response
    TRACE("Sending response to client session %p : %ti bytes sent",
          le_msg_GetSession(_msgRef),
          _msgBufPtr-_msgBufStartPtr);


    le_msg_Respond(_msgRef);

    return;
}


static void Handle_le_mrc_SetRatPreferences
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
    le_mrc_RatBitMask_t ratMask;
    if (!le_pack_UnpackUint32( &_msgBufPtr, &_msgBufSize,
                                               &ratMask ))
    {
        goto error_unpack;
    }

    // Define storage for output parameters

    // Call the function
    le_result_t _result;
    _result  = le_mrc_SetRatPreferences ( 
        ratMask );

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


static void Handle_le_mrc_GetRatPreferences
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

    // Define storage for output parameters
    le_mrc_RatBitMask_t ratMaskPtrBuffer;
    le_mrc_RatBitMask_t *ratMaskPtrPtr = &ratMaskPtrBuffer;
    if (!(_requiredOutputs & (1u << 0)))
    {
        ratMaskPtrPtr = NULL;
    }

    // Call the function
    le_result_t _result;
    _result  = le_mrc_GetRatPreferences ( 
        ratMaskPtrPtr );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;
    _msgBufSize = _MAX_MSG_SIZE;

    // Pack the result first
    LE_ASSERT(le_pack_PackResult( &_msgBufPtr, &_msgBufSize, _result ));

    // Pack any "out" parameters
    if (ratMaskPtrPtr)
    {
        LE_ASSERT(le_pack_PackUint32( &_msgBufPtr, &_msgBufSize,
                                                      *ratMaskPtrPtr ));
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


static void Handle_le_mrc_SetBandPreferences
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
    le_mrc_BandBitMask_t bandMask;
    if (!le_pack_UnpackUint64( &_msgBufPtr, &_msgBufSize,
                                               &bandMask ))
    {
        goto error_unpack;
    }

    // Define storage for output parameters

    // Call the function
    le_result_t _result;
    _result  = le_mrc_SetBandPreferences ( 
        bandMask );

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


static void Handle_le_mrc_GetBandPreferences
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

    // Define storage for output parameters
    le_mrc_BandBitMask_t bandMaskPtrBuffer;
    le_mrc_BandBitMask_t *bandMaskPtrPtr = &bandMaskPtrBuffer;
    if (!(_requiredOutputs & (1u << 0)))
    {
        bandMaskPtrPtr = NULL;
    }

    // Call the function
    le_result_t _result;
    _result  = le_mrc_GetBandPreferences ( 
        bandMaskPtrPtr );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;
    _msgBufSize = _MAX_MSG_SIZE;

    // Pack the result first
    LE_ASSERT(le_pack_PackResult( &_msgBufPtr, &_msgBufSize, _result ));

    // Pack any "out" parameters
    if (bandMaskPtrPtr)
    {
        LE_ASSERT(le_pack_PackUint64( &_msgBufPtr, &_msgBufSize,
                                                      *bandMaskPtrPtr ));
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


static void Handle_le_mrc_SetLteBandPreferences
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
    le_mrc_LteBandBitMask_t bandMask;
    if (!le_pack_UnpackUint32( &_msgBufPtr, &_msgBufSize,
                                               &bandMask ))
    {
        goto error_unpack;
    }

    // Define storage for output parameters

    // Call the function
    le_result_t _result;
    _result  = le_mrc_SetLteBandPreferences ( 
        bandMask );

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


static void Handle_le_mrc_GetLteBandPreferences
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

    // Define storage for output parameters
    le_mrc_LteBandBitMask_t bandMaskPtrBuffer;
    le_mrc_LteBandBitMask_t *bandMaskPtrPtr = &bandMaskPtrBuffer;
    if (!(_requiredOutputs & (1u << 0)))
    {
        bandMaskPtrPtr = NULL;
    }

    // Call the function
    le_result_t _result;
    _result  = le_mrc_GetLteBandPreferences ( 
        bandMaskPtrPtr );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;
    _msgBufSize = _MAX_MSG_SIZE;

    // Pack the result first
    LE_ASSERT(le_pack_PackResult( &_msgBufPtr, &_msgBufSize, _result ));

    // Pack any "out" parameters
    if (bandMaskPtrPtr)
    {
        LE_ASSERT(le_pack_PackUint32( &_msgBufPtr, &_msgBufSize,
                                                      *bandMaskPtrPtr ));
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


static void Handle_le_mrc_SetTdScdmaBandPreferences
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
    le_mrc_TdScdmaBandBitMask_t bandMask;
    if (!le_pack_UnpackUint32( &_msgBufPtr, &_msgBufSize,
                                               &bandMask ))
    {
        goto error_unpack;
    }

    // Define storage for output parameters

    // Call the function
    le_result_t _result;
    _result  = le_mrc_SetTdScdmaBandPreferences ( 
        bandMask );

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


static void Handle_le_mrc_GetTdScdmaBandPreferences
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

    // Define storage for output parameters
    le_mrc_TdScdmaBandBitMask_t bandMaskPtrBuffer;
    le_mrc_TdScdmaBandBitMask_t *bandMaskPtrPtr = &bandMaskPtrBuffer;
    if (!(_requiredOutputs & (1u << 0)))
    {
        bandMaskPtrPtr = NULL;
    }

    // Call the function
    le_result_t _result;
    _result  = le_mrc_GetTdScdmaBandPreferences ( 
        bandMaskPtrPtr );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;
    _msgBufSize = _MAX_MSG_SIZE;

    // Pack the result first
    LE_ASSERT(le_pack_PackResult( &_msgBufPtr, &_msgBufSize, _result ));

    // Pack any "out" parameters
    if (bandMaskPtrPtr)
    {
        LE_ASSERT(le_pack_PackUint32( &_msgBufPtr, &_msgBufSize,
                                                      *bandMaskPtrPtr ));
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


static void Handle_le_mrc_AddPreferredOperator
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
    char mcc[4];
    if (!le_pack_UnpackString( &_msgBufPtr, &_msgBufSize,
                               mcc,
                               sizeof(mcc),
                               3 ))
    {
        goto error_unpack;
    }
    char mnc[4];
    if (!le_pack_UnpackString( &_msgBufPtr, &_msgBufSize,
                               mnc,
                               sizeof(mnc),
                               3 ))
    {
        goto error_unpack;
    }
    le_mrc_RatBitMask_t ratMask;
    if (!le_pack_UnpackUint32( &_msgBufPtr, &_msgBufSize,
                                               &ratMask ))
    {
        goto error_unpack;
    }

    // Define storage for output parameters

    // Call the function
    le_result_t _result;
    _result  = le_mrc_AddPreferredOperator ( 
        mcc, 
        mnc, 
        ratMask );

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


static void Handle_le_mrc_RemovePreferredOperator
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
    char mcc[4];
    if (!le_pack_UnpackString( &_msgBufPtr, &_msgBufSize,
                               mcc,
                               sizeof(mcc),
                               3 ))
    {
        goto error_unpack;
    }
    char mnc[4];
    if (!le_pack_UnpackString( &_msgBufPtr, &_msgBufSize,
                               mnc,
                               sizeof(mnc),
                               3 ))
    {
        goto error_unpack;
    }

    // Define storage for output parameters

    // Call the function
    le_result_t _result;
    _result  = le_mrc_RemovePreferredOperator ( 
        mcc, 
        mnc );

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


static void Handle_le_mrc_GetPreferredOperatorsList
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
    le_mrc_PreferredOperatorListRef_t _result;
    _result  = le_mrc_GetPreferredOperatorsList (  );

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


static void Handle_le_mrc_GetFirstPreferredOperator
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
    le_mrc_PreferredOperatorListRef_t preferredOperatorListRef;
    if (!le_pack_UnpackReference( &_msgBufPtr, &_msgBufSize,
                                               &preferredOperatorListRef ))
    {
        goto error_unpack;
    }

    // Define storage for output parameters

    // Call the function
    le_mrc_PreferredOperatorRef_t _result;
    _result  = le_mrc_GetFirstPreferredOperator ( 
        preferredOperatorListRef );

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


static void Handle_le_mrc_GetNextPreferredOperator
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
    le_mrc_PreferredOperatorListRef_t preferredOperatorListRef;
    if (!le_pack_UnpackReference( &_msgBufPtr, &_msgBufSize,
                                               &preferredOperatorListRef ))
    {
        goto error_unpack;
    }

    // Define storage for output parameters

    // Call the function
    le_mrc_PreferredOperatorRef_t _result;
    _result  = le_mrc_GetNextPreferredOperator ( 
        preferredOperatorListRef );

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


static void Handle_le_mrc_DeletePreferredOperatorsList
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
    le_mrc_PreferredOperatorListRef_t preferredOperatorListRef;
    if (!le_pack_UnpackReference( &_msgBufPtr, &_msgBufSize,
                                               &preferredOperatorListRef ))
    {
        goto error_unpack;
    }

    // Define storage for output parameters

    // Call the function
    le_mrc_DeletePreferredOperatorsList ( 
        preferredOperatorListRef );

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


static void Handle_le_mrc_GetPreferredOperatorDetails
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
    le_mrc_PreferredOperatorRef_t preferredOperatorRef;
    if (!le_pack_UnpackReference( &_msgBufPtr, &_msgBufSize,
                                               &preferredOperatorRef ))
    {
        goto error_unpack;
    }
    size_t mccPtrSize;
    if (!le_pack_UnpackSize( &_msgBufPtr, &_msgBufSize,
                               &mccPtrSize ))
    {
        goto error_unpack;
    }
    if ( (mccPtrSize > 3) &&
         (mccPtrSize < UINT32_MAX) )
    {
        LE_DEBUG("Adjusting mccPtrSize from %zu to 3", mccPtrSize);
        mccPtrSize = 3;
    }
    if (mccPtrSize >= UINT32_MAX)
    {
        mccPtrSize = 0;
    }
    else
    {
        mccPtrSize++;
    }
    size_t mncPtrSize;
    if (!le_pack_UnpackSize( &_msgBufPtr, &_msgBufSize,
                               &mncPtrSize ))
    {
        goto error_unpack;
    }
    if ( (mncPtrSize > 3) &&
         (mncPtrSize < UINT32_MAX) )
    {
        LE_DEBUG("Adjusting mncPtrSize from %zu to 3", mncPtrSize);
        mncPtrSize = 3;
    }
    if (mncPtrSize >= UINT32_MAX)
    {
        mncPtrSize = 0;
    }
    else
    {
        mncPtrSize++;
    }

    // Define storage for output parameters
    char mccPtrBuffer[4];
    char *mccPtr = mccPtrBuffer;
    mccPtr[0] = 0;
    if (!(_requiredOutputs & (1u << 0)))
    {
        mccPtr = NULL;
        mccPtrSize = 0;
    }
    char mncPtrBuffer[4];
    char *mncPtr = mncPtrBuffer;
    mncPtr[0] = 0;
    if (!(_requiredOutputs & (1u << 1)))
    {
        mncPtr = NULL;
        mncPtrSize = 0;
    }
    le_mrc_RatBitMask_t ratMaskBuffer;
    le_mrc_RatBitMask_t *ratMaskPtr = &ratMaskBuffer;
    if (!(_requiredOutputs & (1u << 2)))
    {
        ratMaskPtr = NULL;
    }

    // Call the function
    le_result_t _result;
    _result  = le_mrc_GetPreferredOperatorDetails ( 
        preferredOperatorRef, 
        mccPtr, 
        mccPtrSize, 
        mncPtr, 
        mncPtrSize, 
        ratMaskPtr );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;
    _msgBufSize = _MAX_MSG_SIZE;

    // Pack the result first
    LE_ASSERT(le_pack_PackResult( &_msgBufPtr, &_msgBufSize, _result ));

    // Pack any "out" parameters
    if (mccPtr)
    {
        LE_ASSERT(le_pack_PackString( &_msgBufPtr, &_msgBufSize,
                                      mccPtr, 3 ));
    }
    if (mncPtr)
    {
        LE_ASSERT(le_pack_PackString( &_msgBufPtr, &_msgBufSize,
                                      mncPtr, 3 ));
    }
    if (ratMaskPtr)
    {
        LE_ASSERT(le_pack_PackUint32( &_msgBufPtr, &_msgBufSize,
                                                      *ratMaskPtr ));
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


static void Handle_le_mrc_GetNetRegState
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

    // Define storage for output parameters
    le_mrc_NetRegState_t stateBuffer;
    le_mrc_NetRegState_t *statePtr = &stateBuffer;
    if (!(_requiredOutputs & (1u << 0)))
    {
        statePtr = NULL;
    }

    // Call the function
    le_result_t _result;
    _result  = le_mrc_GetNetRegState ( 
        statePtr );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;
    _msgBufSize = _MAX_MSG_SIZE;

    // Pack the result first
    LE_ASSERT(le_pack_PackResult( &_msgBufPtr, &_msgBufSize, _result ));

    // Pack any "out" parameters
    if (statePtr)
    {
        LE_ASSERT(le_pack_PackUint32( &_msgBufPtr, &_msgBufSize,
                                                      *statePtr ));
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


static void Handle_le_mrc_GetSignalQual
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

    // Define storage for output parameters
    uint32_t qualityBuffer;
    uint32_t *qualityPtr = &qualityBuffer;
    if (!(_requiredOutputs & (1u << 0)))
    {
        qualityPtr = NULL;
    }

    // Call the function
    le_result_t _result;
    _result  = le_mrc_GetSignalQual ( 
        qualityPtr );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;
    _msgBufSize = _MAX_MSG_SIZE;

    // Pack the result first
    LE_ASSERT(le_pack_PackResult( &_msgBufPtr, &_msgBufSize, _result ));

    // Pack any "out" parameters
    if (qualityPtr)
    {
        LE_ASSERT(le_pack_PackUint32( &_msgBufPtr, &_msgBufSize,
                                                      *qualityPtr ));
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


static void Handle_le_mrc_SetRadioPower
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
    le_onoff_t power;
    if (!le_pack_UnpackOnOff( &_msgBufPtr, &_msgBufSize,
                                               &power ))
    {
        goto error_unpack;
    }

    // Define storage for output parameters

    // Call the function
    le_result_t _result;
    _result  = le_mrc_SetRadioPower ( 
        power );

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


static void Handle_le_mrc_GetRadioPower
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

    // Define storage for output parameters
    le_onoff_t powerBuffer;
    le_onoff_t *powerPtr = &powerBuffer;
    if (!(_requiredOutputs & (1u << 0)))
    {
        powerPtr = NULL;
    }

    // Call the function
    le_result_t _result;
    _result  = le_mrc_GetRadioPower ( 
        powerPtr );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;
    _msgBufSize = _MAX_MSG_SIZE;

    // Pack the result first
    LE_ASSERT(le_pack_PackResult( &_msgBufPtr, &_msgBufSize, _result ));

    // Pack any "out" parameters
    if (powerPtr)
    {
        LE_ASSERT(le_pack_PackOnOff( &_msgBufPtr, &_msgBufSize,
                                                      *powerPtr ));
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


static void Handle_le_mrc_PerformCellularNetworkScan
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
    le_mrc_RatBitMask_t ratMask;
    if (!le_pack_UnpackUint32( &_msgBufPtr, &_msgBufSize,
                                               &ratMask ))
    {
        goto error_unpack;
    }

    // Define storage for output parameters

    // Call the function
    le_mrc_ScanInformationListRef_t _result;
    _result  = le_mrc_PerformCellularNetworkScan ( 
        ratMask );

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


static void AsyncResponse_le_mrc_PerformCellularNetworkScanAsync
(
    le_mrc_ScanInformationListRef_t listRef,
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
        LE_FATAL("Handler passed to le_mrc_PerformCellularNetworkScanAsync() can't be called more than once");
    }
    

    // Will not be used if no data is sent back to client
    __attribute__((unused)) uint8_t* _msgBufPtr;
    __attribute__((unused)) size_t _msgBufSize;

    // Create a new message object and get the message buffer
    _msgRef = le_msg_CreateMsg(serverDataPtr->clientSessionRef);
    _msgPtr = le_msg_GetPayloadPtr(_msgRef);
    _msgPtr->id = _MSGID_le_mrc_PerformCellularNetworkScanAsync;
    _msgBufPtr = _msgPtr->buffer;
    _msgBufSize = _MAX_MSG_SIZE;

    // Always pack the client context pointer first
    LE_ASSERT(le_pack_PackReference( &_msgBufPtr, &_msgBufSize, serverDataPtr->contextPtr ))

    // Pack the input parameters
    
    LE_ASSERT(le_pack_PackReference( &_msgBufPtr, &_msgBufSize,
                                                  listRef ));

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


static void Handle_le_mrc_PerformCellularNetworkScanAsync
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
    le_mrc_RatBitMask_t ratMask;
    if (!le_pack_UnpackUint32( &_msgBufPtr, &_msgBufSize,
                                               &ratMask ))
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
    le_mrc_PerformCellularNetworkScanAsync ( 
        ratMask, AsyncResponse_le_mrc_PerformCellularNetworkScanAsync, 
        contextPtr );

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


static void Handle_le_mrc_GetFirstCellularNetworkScan
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
    le_mrc_ScanInformationListRef_t scanInformationListRef;
    if (!le_pack_UnpackReference( &_msgBufPtr, &_msgBufSize,
                                               &scanInformationListRef ))
    {
        goto error_unpack;
    }

    // Define storage for output parameters

    // Call the function
    le_mrc_ScanInformationRef_t _result;
    _result  = le_mrc_GetFirstCellularNetworkScan ( 
        scanInformationListRef );

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


static void Handle_le_mrc_GetNextCellularNetworkScan
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
    le_mrc_ScanInformationListRef_t scanInformationListRef;
    if (!le_pack_UnpackReference( &_msgBufPtr, &_msgBufSize,
                                               &scanInformationListRef ))
    {
        goto error_unpack;
    }

    // Define storage for output parameters

    // Call the function
    le_mrc_ScanInformationRef_t _result;
    _result  = le_mrc_GetNextCellularNetworkScan ( 
        scanInformationListRef );

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


static void Handle_le_mrc_DeleteCellularNetworkScan
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
    le_mrc_ScanInformationListRef_t scanInformationListRef;
    if (!le_pack_UnpackReference( &_msgBufPtr, &_msgBufSize,
                                               &scanInformationListRef ))
    {
        goto error_unpack;
    }

    // Define storage for output parameters

    // Call the function
    le_mrc_DeleteCellularNetworkScan ( 
        scanInformationListRef );

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


static void Handle_le_mrc_GetCellularNetworkMccMnc
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
    le_mrc_ScanInformationRef_t scanInformationRef;
    if (!le_pack_UnpackReference( &_msgBufPtr, &_msgBufSize,
                                               &scanInformationRef ))
    {
        goto error_unpack;
    }
    size_t mccPtrSize;
    if (!le_pack_UnpackSize( &_msgBufPtr, &_msgBufSize,
                               &mccPtrSize ))
    {
        goto error_unpack;
    }
    if ( (mccPtrSize > 3) &&
         (mccPtrSize < UINT32_MAX) )
    {
        LE_DEBUG("Adjusting mccPtrSize from %zu to 3", mccPtrSize);
        mccPtrSize = 3;
    }
    if (mccPtrSize >= UINT32_MAX)
    {
        mccPtrSize = 0;
    }
    else
    {
        mccPtrSize++;
    }
    size_t mncPtrSize;
    if (!le_pack_UnpackSize( &_msgBufPtr, &_msgBufSize,
                               &mncPtrSize ))
    {
        goto error_unpack;
    }
    if ( (mncPtrSize > 3) &&
         (mncPtrSize < UINT32_MAX) )
    {
        LE_DEBUG("Adjusting mncPtrSize from %zu to 3", mncPtrSize);
        mncPtrSize = 3;
    }
    if (mncPtrSize >= UINT32_MAX)
    {
        mncPtrSize = 0;
    }
    else
    {
        mncPtrSize++;
    }

    // Define storage for output parameters
    char mccPtrBuffer[4];
    char *mccPtr = mccPtrBuffer;
    mccPtr[0] = 0;
    if (!(_requiredOutputs & (1u << 0)))
    {
        mccPtr = NULL;
        mccPtrSize = 0;
    }
    char mncPtrBuffer[4];
    char *mncPtr = mncPtrBuffer;
    mncPtr[0] = 0;
    if (!(_requiredOutputs & (1u << 1)))
    {
        mncPtr = NULL;
        mncPtrSize = 0;
    }

    // Call the function
    le_result_t _result;
    _result  = le_mrc_GetCellularNetworkMccMnc ( 
        scanInformationRef, 
        mccPtr, 
        mccPtrSize, 
        mncPtr, 
        mncPtrSize );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;
    _msgBufSize = _MAX_MSG_SIZE;

    // Pack the result first
    LE_ASSERT(le_pack_PackResult( &_msgBufPtr, &_msgBufSize, _result ));

    // Pack any "out" parameters
    if (mccPtr)
    {
        LE_ASSERT(le_pack_PackString( &_msgBufPtr, &_msgBufSize,
                                      mccPtr, 3 ));
    }
    if (mncPtr)
    {
        LE_ASSERT(le_pack_PackString( &_msgBufPtr, &_msgBufSize,
                                      mncPtr, 3 ));
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


static void Handle_le_mrc_GetCellularNetworkName
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
    le_mrc_ScanInformationRef_t scanInformationRef;
    if (!le_pack_UnpackReference( &_msgBufPtr, &_msgBufSize,
                                               &scanInformationRef ))
    {
        goto error_unpack;
    }
    size_t namePtrSize;
    if (!le_pack_UnpackSize( &_msgBufPtr, &_msgBufSize,
                               &namePtrSize ))
    {
        goto error_unpack;
    }
    if ( (namePtrSize > 100) &&
         (namePtrSize < UINT32_MAX) )
    {
        LE_DEBUG("Adjusting namePtrSize from %zu to 100", namePtrSize);
        namePtrSize = 100;
    }
    if (namePtrSize >= UINT32_MAX)
    {
        namePtrSize = 0;
    }
    else
    {
        namePtrSize++;
    }

    // Define storage for output parameters
    char namePtrBuffer[101];
    char *namePtr = namePtrBuffer;
    namePtr[0] = 0;
    if (!(_requiredOutputs & (1u << 0)))
    {
        namePtr = NULL;
        namePtrSize = 0;
    }

    // Call the function
    le_result_t _result;
    _result  = le_mrc_GetCellularNetworkName ( 
        scanInformationRef, 
        namePtr, 
        namePtrSize );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;
    _msgBufSize = _MAX_MSG_SIZE;

    // Pack the result first
    LE_ASSERT(le_pack_PackResult( &_msgBufPtr, &_msgBufSize, _result ));

    // Pack any "out" parameters
    if (namePtr)
    {
        LE_ASSERT(le_pack_PackString( &_msgBufPtr, &_msgBufSize,
                                      namePtr, 100 ));
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


static void Handle_le_mrc_GetCellularNetworkRat
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
    le_mrc_ScanInformationRef_t scanInformationRef;
    if (!le_pack_UnpackReference( &_msgBufPtr, &_msgBufSize,
                                               &scanInformationRef ))
    {
        goto error_unpack;
    }

    // Define storage for output parameters

    // Call the function
    le_mrc_Rat_t _result;
    _result  = le_mrc_GetCellularNetworkRat ( 
        scanInformationRef );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;
    _msgBufSize = _MAX_MSG_SIZE;

    // Pack the result first
    LE_ASSERT(le_pack_PackUint32( &_msgBufPtr, &_msgBufSize, _result ));

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


static void Handle_le_mrc_IsCellularNetworkInUse
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
    le_mrc_ScanInformationRef_t scanInformationRef;
    if (!le_pack_UnpackReference( &_msgBufPtr, &_msgBufSize,
                                               &scanInformationRef ))
    {
        goto error_unpack;
    }

    // Define storage for output parameters

    // Call the function
    bool _result;
    _result  = le_mrc_IsCellularNetworkInUse ( 
        scanInformationRef );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;
    _msgBufSize = _MAX_MSG_SIZE;

    // Pack the result first
    LE_ASSERT(le_pack_PackBool( &_msgBufPtr, &_msgBufSize, _result ));

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


static void Handle_le_mrc_IsCellularNetworkAvailable
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
    le_mrc_ScanInformationRef_t scanInformationRef;
    if (!le_pack_UnpackReference( &_msgBufPtr, &_msgBufSize,
                                               &scanInformationRef ))
    {
        goto error_unpack;
    }

    // Define storage for output parameters

    // Call the function
    bool _result;
    _result  = le_mrc_IsCellularNetworkAvailable ( 
        scanInformationRef );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;
    _msgBufSize = _MAX_MSG_SIZE;

    // Pack the result first
    LE_ASSERT(le_pack_PackBool( &_msgBufPtr, &_msgBufSize, _result ));

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


static void Handle_le_mrc_IsCellularNetworkHome
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
    le_mrc_ScanInformationRef_t scanInformationRef;
    if (!le_pack_UnpackReference( &_msgBufPtr, &_msgBufSize,
                                               &scanInformationRef ))
    {
        goto error_unpack;
    }

    // Define storage for output parameters

    // Call the function
    bool _result;
    _result  = le_mrc_IsCellularNetworkHome ( 
        scanInformationRef );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;
    _msgBufSize = _MAX_MSG_SIZE;

    // Pack the result first
    LE_ASSERT(le_pack_PackBool( &_msgBufPtr, &_msgBufSize, _result ));

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


static void Handle_le_mrc_IsCellularNetworkForbidden
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
    le_mrc_ScanInformationRef_t scanInformationRef;
    if (!le_pack_UnpackReference( &_msgBufPtr, &_msgBufSize,
                                               &scanInformationRef ))
    {
        goto error_unpack;
    }

    // Define storage for output parameters

    // Call the function
    bool _result;
    _result  = le_mrc_IsCellularNetworkForbidden ( 
        scanInformationRef );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;
    _msgBufSize = _MAX_MSG_SIZE;

    // Pack the result first
    LE_ASSERT(le_pack_PackBool( &_msgBufPtr, &_msgBufSize, _result ));

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


static void Handle_le_mrc_GetCurrentNetworkName
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
    size_t nameStrSize;
    if (!le_pack_UnpackSize( &_msgBufPtr, &_msgBufSize,
                               &nameStrSize ))
    {
        goto error_unpack;
    }
    if ( (nameStrSize > 100) &&
         (nameStrSize < UINT32_MAX) )
    {
        LE_DEBUG("Adjusting nameStrSize from %zu to 100", nameStrSize);
        nameStrSize = 100;
    }
    if (nameStrSize >= UINT32_MAX)
    {
        nameStrSize = 0;
    }
    else
    {
        nameStrSize++;
    }

    // Define storage for output parameters
    char nameStrBuffer[101];
    char *nameStr = nameStrBuffer;
    nameStr[0] = 0;
    if (!(_requiredOutputs & (1u << 0)))
    {
        nameStr = NULL;
        nameStrSize = 0;
    }

    // Call the function
    le_result_t _result;
    _result  = le_mrc_GetCurrentNetworkName ( 
        nameStr, 
        nameStrSize );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;
    _msgBufSize = _MAX_MSG_SIZE;

    // Pack the result first
    LE_ASSERT(le_pack_PackResult( &_msgBufPtr, &_msgBufSize, _result ));

    // Pack any "out" parameters
    if (nameStr)
    {
        LE_ASSERT(le_pack_PackString( &_msgBufPtr, &_msgBufSize,
                                      nameStr, 100 ));
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


static void Handle_le_mrc_GetCurrentNetworkMccMnc
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
    size_t mccStrSize;
    if (!le_pack_UnpackSize( &_msgBufPtr, &_msgBufSize,
                               &mccStrSize ))
    {
        goto error_unpack;
    }
    if ( (mccStrSize > 3) &&
         (mccStrSize < UINT32_MAX) )
    {
        LE_DEBUG("Adjusting mccStrSize from %zu to 3", mccStrSize);
        mccStrSize = 3;
    }
    if (mccStrSize >= UINT32_MAX)
    {
        mccStrSize = 0;
    }
    else
    {
        mccStrSize++;
    }
    size_t mncStrSize;
    if (!le_pack_UnpackSize( &_msgBufPtr, &_msgBufSize,
                               &mncStrSize ))
    {
        goto error_unpack;
    }
    if ( (mncStrSize > 3) &&
         (mncStrSize < UINT32_MAX) )
    {
        LE_DEBUG("Adjusting mncStrSize from %zu to 3", mncStrSize);
        mncStrSize = 3;
    }
    if (mncStrSize >= UINT32_MAX)
    {
        mncStrSize = 0;
    }
    else
    {
        mncStrSize++;
    }

    // Define storage for output parameters
    char mccStrBuffer[4];
    char *mccStr = mccStrBuffer;
    mccStr[0] = 0;
    if (!(_requiredOutputs & (1u << 0)))
    {
        mccStr = NULL;
        mccStrSize = 0;
    }
    char mncStrBuffer[4];
    char *mncStr = mncStrBuffer;
    mncStr[0] = 0;
    if (!(_requiredOutputs & (1u << 1)))
    {
        mncStr = NULL;
        mncStrSize = 0;
    }

    // Call the function
    le_result_t _result;
    _result  = le_mrc_GetCurrentNetworkMccMnc ( 
        mccStr, 
        mccStrSize, 
        mncStr, 
        mncStrSize );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;
    _msgBufSize = _MAX_MSG_SIZE;

    // Pack the result first
    LE_ASSERT(le_pack_PackResult( &_msgBufPtr, &_msgBufSize, _result ));

    // Pack any "out" parameters
    if (mccStr)
    {
        LE_ASSERT(le_pack_PackString( &_msgBufPtr, &_msgBufSize,
                                      mccStr, 3 ));
    }
    if (mncStr)
    {
        LE_ASSERT(le_pack_PackString( &_msgBufPtr, &_msgBufSize,
                                      mncStr, 3 ));
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


static void Handle_le_mrc_GetRadioAccessTechInUse
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

    // Define storage for output parameters
    le_mrc_Rat_t ratBuffer;
    le_mrc_Rat_t *ratPtr = &ratBuffer;
    if (!(_requiredOutputs & (1u << 0)))
    {
        ratPtr = NULL;
    }

    // Call the function
    le_result_t _result;
    _result  = le_mrc_GetRadioAccessTechInUse ( 
        ratPtr );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;
    _msgBufSize = _MAX_MSG_SIZE;

    // Pack the result first
    LE_ASSERT(le_pack_PackResult( &_msgBufPtr, &_msgBufSize, _result ));

    // Pack any "out" parameters
    if (ratPtr)
    {
        LE_ASSERT(le_pack_PackUint32( &_msgBufPtr, &_msgBufSize,
                                                      *ratPtr ));
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


static void Handle_le_mrc_GetNeighborCellsInfo
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
    le_mrc_NeighborCellsRef_t _result;
    _result  = le_mrc_GetNeighborCellsInfo (  );

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


static void Handle_le_mrc_DeleteNeighborCellsInfo
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
    le_mrc_NeighborCellsRef_t ngbrCellsRef;
    if (!le_pack_UnpackReference( &_msgBufPtr, &_msgBufSize,
                                               &ngbrCellsRef ))
    {
        goto error_unpack;
    }

    // Define storage for output parameters

    // Call the function
    le_mrc_DeleteNeighborCellsInfo ( 
        ngbrCellsRef );

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


static void Handle_le_mrc_GetFirstNeighborCellInfo
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
    le_mrc_NeighborCellsRef_t ngbrCellsRef;
    if (!le_pack_UnpackReference( &_msgBufPtr, &_msgBufSize,
                                               &ngbrCellsRef ))
    {
        goto error_unpack;
    }

    // Define storage for output parameters

    // Call the function
    le_mrc_CellInfoRef_t _result;
    _result  = le_mrc_GetFirstNeighborCellInfo ( 
        ngbrCellsRef );

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


static void Handle_le_mrc_GetNextNeighborCellInfo
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
    le_mrc_NeighborCellsRef_t ngbrCellsRef;
    if (!le_pack_UnpackReference( &_msgBufPtr, &_msgBufSize,
                                               &ngbrCellsRef ))
    {
        goto error_unpack;
    }

    // Define storage for output parameters

    // Call the function
    le_mrc_CellInfoRef_t _result;
    _result  = le_mrc_GetNextNeighborCellInfo ( 
        ngbrCellsRef );

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


static void Handle_le_mrc_GetNeighborCellId
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
    le_mrc_CellInfoRef_t ngbrCellInfoRef;
    if (!le_pack_UnpackReference( &_msgBufPtr, &_msgBufSize,
                                               &ngbrCellInfoRef ))
    {
        goto error_unpack;
    }

    // Define storage for output parameters

    // Call the function
    uint32_t _result;
    _result  = le_mrc_GetNeighborCellId ( 
        ngbrCellInfoRef );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;
    _msgBufSize = _MAX_MSG_SIZE;

    // Pack the result first
    LE_ASSERT(le_pack_PackUint32( &_msgBufPtr, &_msgBufSize, _result ));

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


static void Handle_le_mrc_GetNeighborCellLocAreaCode
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
    le_mrc_CellInfoRef_t ngbrCellInfoRef;
    if (!le_pack_UnpackReference( &_msgBufPtr, &_msgBufSize,
                                               &ngbrCellInfoRef ))
    {
        goto error_unpack;
    }

    // Define storage for output parameters

    // Call the function
    uint32_t _result;
    _result  = le_mrc_GetNeighborCellLocAreaCode ( 
        ngbrCellInfoRef );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;
    _msgBufSize = _MAX_MSG_SIZE;

    // Pack the result first
    LE_ASSERT(le_pack_PackUint32( &_msgBufPtr, &_msgBufSize, _result ));

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


static void Handle_le_mrc_GetNeighborCellRxLevel
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
    le_mrc_CellInfoRef_t ngbrCellInfoRef;
    if (!le_pack_UnpackReference( &_msgBufPtr, &_msgBufSize,
                                               &ngbrCellInfoRef ))
    {
        goto error_unpack;
    }

    // Define storage for output parameters

    // Call the function
    int32_t _result;
    _result  = le_mrc_GetNeighborCellRxLevel ( 
        ngbrCellInfoRef );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;
    _msgBufSize = _MAX_MSG_SIZE;

    // Pack the result first
    LE_ASSERT(le_pack_PackInt32( &_msgBufPtr, &_msgBufSize, _result ));

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


static void Handle_le_mrc_GetNeighborCellRat
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
    le_mrc_CellInfoRef_t ngbrCellInfoRef;
    if (!le_pack_UnpackReference( &_msgBufPtr, &_msgBufSize,
                                               &ngbrCellInfoRef ))
    {
        goto error_unpack;
    }

    // Define storage for output parameters

    // Call the function
    le_mrc_Rat_t _result;
    _result  = le_mrc_GetNeighborCellRat ( 
        ngbrCellInfoRef );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;
    _msgBufSize = _MAX_MSG_SIZE;

    // Pack the result first
    LE_ASSERT(le_pack_PackUint32( &_msgBufPtr, &_msgBufSize, _result ));

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


static void Handle_le_mrc_GetNeighborCellUmtsEcIo
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
    le_mrc_CellInfoRef_t ngbrCellInfoRef;
    if (!le_pack_UnpackReference( &_msgBufPtr, &_msgBufSize,
                                               &ngbrCellInfoRef ))
    {
        goto error_unpack;
    }

    // Define storage for output parameters

    // Call the function
    int32_t _result;
    _result  = le_mrc_GetNeighborCellUmtsEcIo ( 
        ngbrCellInfoRef );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;
    _msgBufSize = _MAX_MSG_SIZE;

    // Pack the result first
    LE_ASSERT(le_pack_PackInt32( &_msgBufPtr, &_msgBufSize, _result ));

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


static void Handle_le_mrc_GetNeighborCellLteIntraFreq
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
    le_mrc_CellInfoRef_t ngbrCellInfoRef;
    if (!le_pack_UnpackReference( &_msgBufPtr, &_msgBufSize,
                                               &ngbrCellInfoRef ))
    {
        goto error_unpack;
    }

    // Define storage for output parameters
    int32_t rsrqBuffer;
    int32_t *rsrqPtr = &rsrqBuffer;
    if (!(_requiredOutputs & (1u << 0)))
    {
        rsrqPtr = NULL;
    }
    int32_t rsrpBuffer;
    int32_t *rsrpPtr = &rsrpBuffer;
    if (!(_requiredOutputs & (1u << 1)))
    {
        rsrpPtr = NULL;
    }

    // Call the function
    le_result_t _result;
    _result  = le_mrc_GetNeighborCellLteIntraFreq ( 
        ngbrCellInfoRef, 
        rsrqPtr, 
        rsrpPtr );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;
    _msgBufSize = _MAX_MSG_SIZE;

    // Pack the result first
    LE_ASSERT(le_pack_PackResult( &_msgBufPtr, &_msgBufSize, _result ));

    // Pack any "out" parameters
    if (rsrqPtr)
    {
        LE_ASSERT(le_pack_PackInt32( &_msgBufPtr, &_msgBufSize,
                                                      *rsrqPtr ));
    }
    if (rsrpPtr)
    {
        LE_ASSERT(le_pack_PackInt32( &_msgBufPtr, &_msgBufSize,
                                                      *rsrpPtr ));
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


static void Handle_le_mrc_GetNeighborCellLteInterFreq
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
    le_mrc_CellInfoRef_t ngbrCellInfoRef;
    if (!le_pack_UnpackReference( &_msgBufPtr, &_msgBufSize,
                                               &ngbrCellInfoRef ))
    {
        goto error_unpack;
    }

    // Define storage for output parameters
    int32_t rsrqBuffer;
    int32_t *rsrqPtr = &rsrqBuffer;
    if (!(_requiredOutputs & (1u << 0)))
    {
        rsrqPtr = NULL;
    }
    int32_t rsrpBuffer;
    int32_t *rsrpPtr = &rsrpBuffer;
    if (!(_requiredOutputs & (1u << 1)))
    {
        rsrpPtr = NULL;
    }

    // Call the function
    le_result_t _result;
    _result  = le_mrc_GetNeighborCellLteInterFreq ( 
        ngbrCellInfoRef, 
        rsrqPtr, 
        rsrpPtr );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;
    _msgBufSize = _MAX_MSG_SIZE;

    // Pack the result first
    LE_ASSERT(le_pack_PackResult( &_msgBufPtr, &_msgBufSize, _result ));

    // Pack any "out" parameters
    if (rsrqPtr)
    {
        LE_ASSERT(le_pack_PackInt32( &_msgBufPtr, &_msgBufSize,
                                                      *rsrqPtr ));
    }
    if (rsrpPtr)
    {
        LE_ASSERT(le_pack_PackInt32( &_msgBufPtr, &_msgBufSize,
                                                      *rsrpPtr ));
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


static void Handle_le_mrc_MeasureSignalMetrics
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
    le_mrc_MetricsRef_t _result;
    _result  = le_mrc_MeasureSignalMetrics (  );

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


static void Handle_le_mrc_DeleteSignalMetrics
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
    le_mrc_MetricsRef_t MetricsRef;
    if (!le_pack_UnpackReference( &_msgBufPtr, &_msgBufSize,
                                               &MetricsRef ))
    {
        goto error_unpack;
    }

    // Define storage for output parameters

    // Call the function
    le_mrc_DeleteSignalMetrics ( 
        MetricsRef );

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


static void Handle_le_mrc_GetRatOfSignalMetrics
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
    le_mrc_MetricsRef_t MetricsRef;
    if (!le_pack_UnpackReference( &_msgBufPtr, &_msgBufSize,
                                               &MetricsRef ))
    {
        goto error_unpack;
    }

    // Define storage for output parameters

    // Call the function
    le_mrc_Rat_t _result;
    _result  = le_mrc_GetRatOfSignalMetrics ( 
        MetricsRef );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;
    _msgBufSize = _MAX_MSG_SIZE;

    // Pack the result first
    LE_ASSERT(le_pack_PackUint32( &_msgBufPtr, &_msgBufSize, _result ));

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


static void Handle_le_mrc_GetGsmSignalMetrics
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
    le_mrc_MetricsRef_t MetricsRef;
    if (!le_pack_UnpackReference( &_msgBufPtr, &_msgBufSize,
                                               &MetricsRef ))
    {
        goto error_unpack;
    }

    // Define storage for output parameters
    int32_t rssiBuffer;
    int32_t *rssiPtr = &rssiBuffer;
    if (!(_requiredOutputs & (1u << 0)))
    {
        rssiPtr = NULL;
    }
    uint32_t berBuffer;
    uint32_t *berPtr = &berBuffer;
    if (!(_requiredOutputs & (1u << 1)))
    {
        berPtr = NULL;
    }

    // Call the function
    le_result_t _result;
    _result  = le_mrc_GetGsmSignalMetrics ( 
        MetricsRef, 
        rssiPtr, 
        berPtr );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;
    _msgBufSize = _MAX_MSG_SIZE;

    // Pack the result first
    LE_ASSERT(le_pack_PackResult( &_msgBufPtr, &_msgBufSize, _result ));

    // Pack any "out" parameters
    if (rssiPtr)
    {
        LE_ASSERT(le_pack_PackInt32( &_msgBufPtr, &_msgBufSize,
                                                      *rssiPtr ));
    }
    if (berPtr)
    {
        LE_ASSERT(le_pack_PackUint32( &_msgBufPtr, &_msgBufSize,
                                                      *berPtr ));
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


static void Handle_le_mrc_GetUmtsSignalMetrics
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
    le_mrc_MetricsRef_t MetricsRef;
    if (!le_pack_UnpackReference( &_msgBufPtr, &_msgBufSize,
                                               &MetricsRef ))
    {
        goto error_unpack;
    }

    // Define storage for output parameters
    int32_t ssBuffer;
    int32_t *ssPtr = &ssBuffer;
    if (!(_requiredOutputs & (1u << 0)))
    {
        ssPtr = NULL;
    }
    uint32_t blerBuffer;
    uint32_t *blerPtr = &blerBuffer;
    if (!(_requiredOutputs & (1u << 1)))
    {
        blerPtr = NULL;
    }
    int32_t ecioBuffer;
    int32_t *ecioPtr = &ecioBuffer;
    if (!(_requiredOutputs & (1u << 2)))
    {
        ecioPtr = NULL;
    }
    int32_t rscpBuffer;
    int32_t *rscpPtr = &rscpBuffer;
    if (!(_requiredOutputs & (1u << 3)))
    {
        rscpPtr = NULL;
    }
    int32_t sinrBuffer;
    int32_t *sinrPtr = &sinrBuffer;
    if (!(_requiredOutputs & (1u << 4)))
    {
        sinrPtr = NULL;
    }

    // Call the function
    le_result_t _result;
    _result  = le_mrc_GetUmtsSignalMetrics ( 
        MetricsRef, 
        ssPtr, 
        blerPtr, 
        ecioPtr, 
        rscpPtr, 
        sinrPtr );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;
    _msgBufSize = _MAX_MSG_SIZE;

    // Pack the result first
    LE_ASSERT(le_pack_PackResult( &_msgBufPtr, &_msgBufSize, _result ));

    // Pack any "out" parameters
    if (ssPtr)
    {
        LE_ASSERT(le_pack_PackInt32( &_msgBufPtr, &_msgBufSize,
                                                      *ssPtr ));
    }
    if (blerPtr)
    {
        LE_ASSERT(le_pack_PackUint32( &_msgBufPtr, &_msgBufSize,
                                                      *blerPtr ));
    }
    if (ecioPtr)
    {
        LE_ASSERT(le_pack_PackInt32( &_msgBufPtr, &_msgBufSize,
                                                      *ecioPtr ));
    }
    if (rscpPtr)
    {
        LE_ASSERT(le_pack_PackInt32( &_msgBufPtr, &_msgBufSize,
                                                      *rscpPtr ));
    }
    if (sinrPtr)
    {
        LE_ASSERT(le_pack_PackInt32( &_msgBufPtr, &_msgBufSize,
                                                      *sinrPtr ));
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


static void Handle_le_mrc_GetLteSignalMetrics
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
    le_mrc_MetricsRef_t MetricsRef;
    if (!le_pack_UnpackReference( &_msgBufPtr, &_msgBufSize,
                                               &MetricsRef ))
    {
        goto error_unpack;
    }

    // Define storage for output parameters
    int32_t ssBuffer;
    int32_t *ssPtr = &ssBuffer;
    if (!(_requiredOutputs & (1u << 0)))
    {
        ssPtr = NULL;
    }
    uint32_t blerBuffer;
    uint32_t *blerPtr = &blerBuffer;
    if (!(_requiredOutputs & (1u << 1)))
    {
        blerPtr = NULL;
    }
    int32_t rsrqBuffer;
    int32_t *rsrqPtr = &rsrqBuffer;
    if (!(_requiredOutputs & (1u << 2)))
    {
        rsrqPtr = NULL;
    }
    int32_t rsrpBuffer;
    int32_t *rsrpPtr = &rsrpBuffer;
    if (!(_requiredOutputs & (1u << 3)))
    {
        rsrpPtr = NULL;
    }
    int32_t snrBuffer;
    int32_t *snrPtr = &snrBuffer;
    if (!(_requiredOutputs & (1u << 4)))
    {
        snrPtr = NULL;
    }

    // Call the function
    le_result_t _result;
    _result  = le_mrc_GetLteSignalMetrics ( 
        MetricsRef, 
        ssPtr, 
        blerPtr, 
        rsrqPtr, 
        rsrpPtr, 
        snrPtr );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;
    _msgBufSize = _MAX_MSG_SIZE;

    // Pack the result first
    LE_ASSERT(le_pack_PackResult( &_msgBufPtr, &_msgBufSize, _result ));

    // Pack any "out" parameters
    if (ssPtr)
    {
        LE_ASSERT(le_pack_PackInt32( &_msgBufPtr, &_msgBufSize,
                                                      *ssPtr ));
    }
    if (blerPtr)
    {
        LE_ASSERT(le_pack_PackUint32( &_msgBufPtr, &_msgBufSize,
                                                      *blerPtr ));
    }
    if (rsrqPtr)
    {
        LE_ASSERT(le_pack_PackInt32( &_msgBufPtr, &_msgBufSize,
                                                      *rsrqPtr ));
    }
    if (rsrpPtr)
    {
        LE_ASSERT(le_pack_PackInt32( &_msgBufPtr, &_msgBufSize,
                                                      *rsrpPtr ));
    }
    if (snrPtr)
    {
        LE_ASSERT(le_pack_PackInt32( &_msgBufPtr, &_msgBufSize,
                                                      *snrPtr ));
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


static void Handle_le_mrc_GetCdmaSignalMetrics
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
    le_mrc_MetricsRef_t MetricsRef;
    if (!le_pack_UnpackReference( &_msgBufPtr, &_msgBufSize,
                                               &MetricsRef ))
    {
        goto error_unpack;
    }

    // Define storage for output parameters
    int32_t ssBuffer;
    int32_t *ssPtr = &ssBuffer;
    if (!(_requiredOutputs & (1u << 0)))
    {
        ssPtr = NULL;
    }
    uint32_t erBuffer;
    uint32_t *erPtr = &erBuffer;
    if (!(_requiredOutputs & (1u << 1)))
    {
        erPtr = NULL;
    }
    int32_t ecioBuffer;
    int32_t *ecioPtr = &ecioBuffer;
    if (!(_requiredOutputs & (1u << 2)))
    {
        ecioPtr = NULL;
    }
    int32_t sinrBuffer;
    int32_t *sinrPtr = &sinrBuffer;
    if (!(_requiredOutputs & (1u << 3)))
    {
        sinrPtr = NULL;
    }
    int32_t ioBuffer;
    int32_t *ioPtr = &ioBuffer;
    if (!(_requiredOutputs & (1u << 4)))
    {
        ioPtr = NULL;
    }

    // Call the function
    le_result_t _result;
    _result  = le_mrc_GetCdmaSignalMetrics ( 
        MetricsRef, 
        ssPtr, 
        erPtr, 
        ecioPtr, 
        sinrPtr, 
        ioPtr );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;
    _msgBufSize = _MAX_MSG_SIZE;

    // Pack the result first
    LE_ASSERT(le_pack_PackResult( &_msgBufPtr, &_msgBufSize, _result ));

    // Pack any "out" parameters
    if (ssPtr)
    {
        LE_ASSERT(le_pack_PackInt32( &_msgBufPtr, &_msgBufSize,
                                                      *ssPtr ));
    }
    if (erPtr)
    {
        LE_ASSERT(le_pack_PackUint32( &_msgBufPtr, &_msgBufSize,
                                                      *erPtr ));
    }
    if (ecioPtr)
    {
        LE_ASSERT(le_pack_PackInt32( &_msgBufPtr, &_msgBufSize,
                                                      *ecioPtr ));
    }
    if (sinrPtr)
    {
        LE_ASSERT(le_pack_PackInt32( &_msgBufPtr, &_msgBufSize,
                                                      *sinrPtr ));
    }
    if (ioPtr)
    {
        LE_ASSERT(le_pack_PackInt32( &_msgBufPtr, &_msgBufSize,
                                                      *ioPtr ));
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


static void Handle_le_mrc_GetServingCellId
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
    uint32_t _result;
    _result  = le_mrc_GetServingCellId (  );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;
    _msgBufSize = _MAX_MSG_SIZE;

    // Pack the result first
    LE_ASSERT(le_pack_PackUint32( &_msgBufPtr, &_msgBufSize, _result ));

    // Pack any "out" parameters

    // Return the response
    TRACE("Sending response to client session %p : %ti bytes sent",
          le_msg_GetSession(_msgRef),
          _msgBufPtr-_msgBufStartPtr);


    le_msg_Respond(_msgRef);

    return;
}


static void Handle_le_mrc_GetServingCellLocAreaCode
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
    uint32_t _result;
    _result  = le_mrc_GetServingCellLocAreaCode (  );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;
    _msgBufSize = _MAX_MSG_SIZE;

    // Pack the result first
    LE_ASSERT(le_pack_PackUint32( &_msgBufPtr, &_msgBufSize, _result ));

    // Pack any "out" parameters

    // Return the response
    TRACE("Sending response to client session %p : %ti bytes sent",
          le_msg_GetSession(_msgRef),
          _msgBufPtr-_msgBufStartPtr);


    le_msg_Respond(_msgRef);

    return;
}


static void Handle_le_mrc_GetServingCellLteTracAreaCode
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
    uint16_t _result;
    _result  = le_mrc_GetServingCellLteTracAreaCode (  );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;
    _msgBufSize = _MAX_MSG_SIZE;

    // Pack the result first
    LE_ASSERT(le_pack_PackUint16( &_msgBufPtr, &_msgBufSize, _result ));

    // Pack any "out" parameters

    // Return the response
    TRACE("Sending response to client session %p : %ti bytes sent",
          le_msg_GetSession(_msgRef),
          _msgBufPtr-_msgBufStartPtr);


    le_msg_Respond(_msgRef);

    return;
}


static void Handle_le_mrc_GetBandCapabilities
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

    // Define storage for output parameters
    le_mrc_BandBitMask_t bandMaskPtrBuffer;
    le_mrc_BandBitMask_t *bandMaskPtrPtr = &bandMaskPtrBuffer;
    if (!(_requiredOutputs & (1u << 0)))
    {
        bandMaskPtrPtr = NULL;
    }

    // Call the function
    le_result_t _result;
    _result  = le_mrc_GetBandCapabilities ( 
        bandMaskPtrPtr );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;
    _msgBufSize = _MAX_MSG_SIZE;

    // Pack the result first
    LE_ASSERT(le_pack_PackResult( &_msgBufPtr, &_msgBufSize, _result ));

    // Pack any "out" parameters
    if (bandMaskPtrPtr)
    {
        LE_ASSERT(le_pack_PackUint64( &_msgBufPtr, &_msgBufSize,
                                                      *bandMaskPtrPtr ));
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


static void Handle_le_mrc_GetLteBandCapabilities
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

    // Define storage for output parameters
    le_mrc_LteBandBitMask_t bandMaskPtrBuffer;
    le_mrc_LteBandBitMask_t *bandMaskPtrPtr = &bandMaskPtrBuffer;
    if (!(_requiredOutputs & (1u << 0)))
    {
        bandMaskPtrPtr = NULL;
    }

    // Call the function
    le_result_t _result;
    _result  = le_mrc_GetLteBandCapabilities ( 
        bandMaskPtrPtr );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;
    _msgBufSize = _MAX_MSG_SIZE;

    // Pack the result first
    LE_ASSERT(le_pack_PackResult( &_msgBufPtr, &_msgBufSize, _result ));

    // Pack any "out" parameters
    if (bandMaskPtrPtr)
    {
        LE_ASSERT(le_pack_PackUint32( &_msgBufPtr, &_msgBufSize,
                                                      *bandMaskPtrPtr ));
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


static void Handle_le_mrc_GetTdScdmaBandCapabilities
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

    // Define storage for output parameters
    le_mrc_TdScdmaBandBitMask_t bandMaskPtrBuffer;
    le_mrc_TdScdmaBandBitMask_t *bandMaskPtrPtr = &bandMaskPtrBuffer;
    if (!(_requiredOutputs & (1u << 0)))
    {
        bandMaskPtrPtr = NULL;
    }

    // Call the function
    le_result_t _result;
    _result  = le_mrc_GetTdScdmaBandCapabilities ( 
        bandMaskPtrPtr );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;
    _msgBufSize = _MAX_MSG_SIZE;

    // Pack the result first
    LE_ASSERT(le_pack_PackResult( &_msgBufPtr, &_msgBufSize, _result ));

    // Pack any "out" parameters
    if (bandMaskPtrPtr)
    {
        LE_ASSERT(le_pack_PackUint32( &_msgBufPtr, &_msgBufSize,
                                                      *bandMaskPtrPtr ));
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


static void AsyncResponse_le_mrc_AddNetworkRejectHandler
(
    const char* LE_NONNULL mcc,
    const char* LE_NONNULL mnc,
    le_mrc_Rat_t rat,
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
    _msgPtr->id = _MSGID_le_mrc_AddNetworkRejectHandler;
    _msgBufPtr = _msgPtr->buffer;
    _msgBufSize = _MAX_MSG_SIZE;

    // Always pack the client context pointer first
    LE_ASSERT(le_pack_PackReference( &_msgBufPtr, &_msgBufSize, serverDataPtr->contextPtr ))

    // Pack the input parameters
    
    LE_ASSERT(le_pack_PackString( &_msgBufPtr, &_msgBufSize,
                                  mcc, 3 ));
    LE_ASSERT(le_pack_PackString( &_msgBufPtr, &_msgBufSize,
                                  mnc, 3 ));
    LE_ASSERT(le_pack_PackUint32( &_msgBufPtr, &_msgBufSize,
                                                  rat ));

    // Send the async response to the client
    TRACE("Sending message to client session %p : %ti bytes sent",
          serverDataPtr->clientSessionRef,
          _msgBufPtr-_msgPtr->buffer);

    SendMsgToClient(_msgRef);
}


static void Handle_le_mrc_AddNetworkRejectHandler
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
    le_mrc_NetworkRejectHandlerRef_t _result;
    _result  = le_mrc_AddNetworkRejectHandler ( AsyncResponse_le_mrc_AddNetworkRejectHandler, 
        contextPtr );

    if (_result)
    {
        // Put the handler reference result and a pointer to the associated remove function
        // into the server data object.  This function pointer is needed in case the client
        // is closed and the handlers need to be removed.
        serverDataPtr->handlerRef = (le_event_HandlerRef_t)_result;
        serverDataPtr->removeHandlerFunc =
            (RemoveHandlerFunc_t)le_mrc_RemoveNetworkRejectHandler;

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


static void Handle_le_mrc_RemoveNetworkRejectHandler
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
    le_mrc_NetworkRejectHandlerRef_t handlerRef;
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
    handlerRef = (le_mrc_NetworkRejectHandlerRef_t)serverDataPtr->handlerRef;
    le_mem_Release(serverDataPtr);

    // Define storage for output parameters

    // Call the function
    le_mrc_RemoveNetworkRejectHandler ( 
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


static void AsyncResponse_le_mrc_AddJammingDetectionEventHandler
(
    le_mrc_JammingReport_t report,
    le_mrc_JammingStatus_t status,
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
    _msgPtr->id = _MSGID_le_mrc_AddJammingDetectionEventHandler;
    _msgBufPtr = _msgPtr->buffer;
    _msgBufSize = _MAX_MSG_SIZE;

    // Always pack the client context pointer first
    LE_ASSERT(le_pack_PackReference( &_msgBufPtr, &_msgBufSize, serverDataPtr->contextPtr ))

    // Pack the input parameters
    
    LE_ASSERT(le_pack_PackUint32( &_msgBufPtr, &_msgBufSize,
                                                  report ));
    LE_ASSERT(le_pack_PackUint32( &_msgBufPtr, &_msgBufSize,
                                                  status ));

    // Send the async response to the client
    TRACE("Sending message to client session %p : %ti bytes sent",
          serverDataPtr->clientSessionRef,
          _msgBufPtr-_msgPtr->buffer);

    SendMsgToClient(_msgRef);
}


static void Handle_le_mrc_AddJammingDetectionEventHandler
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
    le_mrc_JammingDetectionEventHandlerRef_t _result;
    _result  = le_mrc_AddJammingDetectionEventHandler ( AsyncResponse_le_mrc_AddJammingDetectionEventHandler, 
        contextPtr );

    if (_result)
    {
        // Put the handler reference result and a pointer to the associated remove function
        // into the server data object.  This function pointer is needed in case the client
        // is closed and the handlers need to be removed.
        serverDataPtr->handlerRef = (le_event_HandlerRef_t)_result;
        serverDataPtr->removeHandlerFunc =
            (RemoveHandlerFunc_t)le_mrc_RemoveJammingDetectionEventHandler;

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


static void Handle_le_mrc_RemoveJammingDetectionEventHandler
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
    le_mrc_JammingDetectionEventHandlerRef_t handlerRef;
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
    handlerRef = (le_mrc_JammingDetectionEventHandlerRef_t)serverDataPtr->handlerRef;
    le_mem_Release(serverDataPtr);

    // Define storage for output parameters

    // Call the function
    le_mrc_RemoveJammingDetectionEventHandler ( 
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


static void Handle_le_mrc_StartJammingDetection
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
    le_result_t _result;
    _result  = le_mrc_StartJammingDetection (  );

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
}


static void Handle_le_mrc_StopJammingDetection
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
    le_result_t _result;
    _result  = le_mrc_StopJammingDetection (  );

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
        case _MSGID_le_mrc_AddNetRegStateEventHandler : Handle_le_mrc_AddNetRegStateEventHandler(msgRef); break;
        case _MSGID_le_mrc_RemoveNetRegStateEventHandler : Handle_le_mrc_RemoveNetRegStateEventHandler(msgRef); break;
        case _MSGID_le_mrc_AddRatChangeHandler : Handle_le_mrc_AddRatChangeHandler(msgRef); break;
        case _MSGID_le_mrc_RemoveRatChangeHandler : Handle_le_mrc_RemoveRatChangeHandler(msgRef); break;
        case _MSGID_le_mrc_AddPacketSwitchedChangeHandler : Handle_le_mrc_AddPacketSwitchedChangeHandler(msgRef); break;
        case _MSGID_le_mrc_RemovePacketSwitchedChangeHandler : Handle_le_mrc_RemovePacketSwitchedChangeHandler(msgRef); break;
        case _MSGID_le_mrc_AddSignalStrengthChangeHandler : Handle_le_mrc_AddSignalStrengthChangeHandler(msgRef); break;
        case _MSGID_le_mrc_RemoveSignalStrengthChangeHandler : Handle_le_mrc_RemoveSignalStrengthChangeHandler(msgRef); break;
        case _MSGID_le_mrc_SetSignalStrengthIndThresholds : Handle_le_mrc_SetSignalStrengthIndThresholds(msgRef); break;
        case _MSGID_le_mrc_SetSignalStrengthIndDelta : Handle_le_mrc_SetSignalStrengthIndDelta(msgRef); break;
        case _MSGID_le_mrc_SetAutomaticRegisterMode : Handle_le_mrc_SetAutomaticRegisterMode(msgRef); break;
        case _MSGID_le_mrc_SetManualRegisterMode : Handle_le_mrc_SetManualRegisterMode(msgRef); break;
        case _MSGID_le_mrc_SetManualRegisterModeAsync : Handle_le_mrc_SetManualRegisterModeAsync(msgRef); break;
        case _MSGID_le_mrc_GetRegisterMode : Handle_le_mrc_GetRegisterMode(msgRef); break;
        case _MSGID_le_mrc_GetPacketSwitchedState : Handle_le_mrc_GetPacketSwitchedState(msgRef); break;
        case _MSGID_le_mrc_GetPlatformSpecificRegistrationErrorCode : Handle_le_mrc_GetPlatformSpecificRegistrationErrorCode(msgRef); break;
        case _MSGID_le_mrc_SetRatPreferences : Handle_le_mrc_SetRatPreferences(msgRef); break;
        case _MSGID_le_mrc_GetRatPreferences : Handle_le_mrc_GetRatPreferences(msgRef); break;
        case _MSGID_le_mrc_SetBandPreferences : Handle_le_mrc_SetBandPreferences(msgRef); break;
        case _MSGID_le_mrc_GetBandPreferences : Handle_le_mrc_GetBandPreferences(msgRef); break;
        case _MSGID_le_mrc_SetLteBandPreferences : Handle_le_mrc_SetLteBandPreferences(msgRef); break;
        case _MSGID_le_mrc_GetLteBandPreferences : Handle_le_mrc_GetLteBandPreferences(msgRef); break;
        case _MSGID_le_mrc_SetTdScdmaBandPreferences : Handle_le_mrc_SetTdScdmaBandPreferences(msgRef); break;
        case _MSGID_le_mrc_GetTdScdmaBandPreferences : Handle_le_mrc_GetTdScdmaBandPreferences(msgRef); break;
        case _MSGID_le_mrc_AddPreferredOperator : Handle_le_mrc_AddPreferredOperator(msgRef); break;
        case _MSGID_le_mrc_RemovePreferredOperator : Handle_le_mrc_RemovePreferredOperator(msgRef); break;
        case _MSGID_le_mrc_GetPreferredOperatorsList : Handle_le_mrc_GetPreferredOperatorsList(msgRef); break;
        case _MSGID_le_mrc_GetFirstPreferredOperator : Handle_le_mrc_GetFirstPreferredOperator(msgRef); break;
        case _MSGID_le_mrc_GetNextPreferredOperator : Handle_le_mrc_GetNextPreferredOperator(msgRef); break;
        case _MSGID_le_mrc_DeletePreferredOperatorsList : Handle_le_mrc_DeletePreferredOperatorsList(msgRef); break;
        case _MSGID_le_mrc_GetPreferredOperatorDetails : Handle_le_mrc_GetPreferredOperatorDetails(msgRef); break;
        case _MSGID_le_mrc_GetNetRegState : Handle_le_mrc_GetNetRegState(msgRef); break;
        case _MSGID_le_mrc_GetSignalQual : Handle_le_mrc_GetSignalQual(msgRef); break;
        case _MSGID_le_mrc_SetRadioPower : Handle_le_mrc_SetRadioPower(msgRef); break;
        case _MSGID_le_mrc_GetRadioPower : Handle_le_mrc_GetRadioPower(msgRef); break;
        case _MSGID_le_mrc_PerformCellularNetworkScan : Handle_le_mrc_PerformCellularNetworkScan(msgRef); break;
        case _MSGID_le_mrc_PerformCellularNetworkScanAsync : Handle_le_mrc_PerformCellularNetworkScanAsync(msgRef); break;
        case _MSGID_le_mrc_GetFirstCellularNetworkScan : Handle_le_mrc_GetFirstCellularNetworkScan(msgRef); break;
        case _MSGID_le_mrc_GetNextCellularNetworkScan : Handle_le_mrc_GetNextCellularNetworkScan(msgRef); break;
        case _MSGID_le_mrc_DeleteCellularNetworkScan : Handle_le_mrc_DeleteCellularNetworkScan(msgRef); break;
        case _MSGID_le_mrc_GetCellularNetworkMccMnc : Handle_le_mrc_GetCellularNetworkMccMnc(msgRef); break;
        case _MSGID_le_mrc_GetCellularNetworkName : Handle_le_mrc_GetCellularNetworkName(msgRef); break;
        case _MSGID_le_mrc_GetCellularNetworkRat : Handle_le_mrc_GetCellularNetworkRat(msgRef); break;
        case _MSGID_le_mrc_IsCellularNetworkInUse : Handle_le_mrc_IsCellularNetworkInUse(msgRef); break;
        case _MSGID_le_mrc_IsCellularNetworkAvailable : Handle_le_mrc_IsCellularNetworkAvailable(msgRef); break;
        case _MSGID_le_mrc_IsCellularNetworkHome : Handle_le_mrc_IsCellularNetworkHome(msgRef); break;
        case _MSGID_le_mrc_IsCellularNetworkForbidden : Handle_le_mrc_IsCellularNetworkForbidden(msgRef); break;
        case _MSGID_le_mrc_GetCurrentNetworkName : Handle_le_mrc_GetCurrentNetworkName(msgRef); break;
        case _MSGID_le_mrc_GetCurrentNetworkMccMnc : Handle_le_mrc_GetCurrentNetworkMccMnc(msgRef); break;
        case _MSGID_le_mrc_GetRadioAccessTechInUse : Handle_le_mrc_GetRadioAccessTechInUse(msgRef); break;
        case _MSGID_le_mrc_GetNeighborCellsInfo : Handle_le_mrc_GetNeighborCellsInfo(msgRef); break;
        case _MSGID_le_mrc_DeleteNeighborCellsInfo : Handle_le_mrc_DeleteNeighborCellsInfo(msgRef); break;
        case _MSGID_le_mrc_GetFirstNeighborCellInfo : Handle_le_mrc_GetFirstNeighborCellInfo(msgRef); break;
        case _MSGID_le_mrc_GetNextNeighborCellInfo : Handle_le_mrc_GetNextNeighborCellInfo(msgRef); break;
        case _MSGID_le_mrc_GetNeighborCellId : Handle_le_mrc_GetNeighborCellId(msgRef); break;
        case _MSGID_le_mrc_GetNeighborCellLocAreaCode : Handle_le_mrc_GetNeighborCellLocAreaCode(msgRef); break;
        case _MSGID_le_mrc_GetNeighborCellRxLevel : Handle_le_mrc_GetNeighborCellRxLevel(msgRef); break;
        case _MSGID_le_mrc_GetNeighborCellRat : Handle_le_mrc_GetNeighborCellRat(msgRef); break;
        case _MSGID_le_mrc_GetNeighborCellUmtsEcIo : Handle_le_mrc_GetNeighborCellUmtsEcIo(msgRef); break;
        case _MSGID_le_mrc_GetNeighborCellLteIntraFreq : Handle_le_mrc_GetNeighborCellLteIntraFreq(msgRef); break;
        case _MSGID_le_mrc_GetNeighborCellLteInterFreq : Handle_le_mrc_GetNeighborCellLteInterFreq(msgRef); break;
        case _MSGID_le_mrc_MeasureSignalMetrics : Handle_le_mrc_MeasureSignalMetrics(msgRef); break;
        case _MSGID_le_mrc_DeleteSignalMetrics : Handle_le_mrc_DeleteSignalMetrics(msgRef); break;
        case _MSGID_le_mrc_GetRatOfSignalMetrics : Handle_le_mrc_GetRatOfSignalMetrics(msgRef); break;
        case _MSGID_le_mrc_GetGsmSignalMetrics : Handle_le_mrc_GetGsmSignalMetrics(msgRef); break;
        case _MSGID_le_mrc_GetUmtsSignalMetrics : Handle_le_mrc_GetUmtsSignalMetrics(msgRef); break;
        case _MSGID_le_mrc_GetLteSignalMetrics : Handle_le_mrc_GetLteSignalMetrics(msgRef); break;
        case _MSGID_le_mrc_GetCdmaSignalMetrics : Handle_le_mrc_GetCdmaSignalMetrics(msgRef); break;
        case _MSGID_le_mrc_GetServingCellId : Handle_le_mrc_GetServingCellId(msgRef); break;
        case _MSGID_le_mrc_GetServingCellLocAreaCode : Handle_le_mrc_GetServingCellLocAreaCode(msgRef); break;
        case _MSGID_le_mrc_GetServingCellLteTracAreaCode : Handle_le_mrc_GetServingCellLteTracAreaCode(msgRef); break;
        case _MSGID_le_mrc_GetBandCapabilities : Handle_le_mrc_GetBandCapabilities(msgRef); break;
        case _MSGID_le_mrc_GetLteBandCapabilities : Handle_le_mrc_GetLteBandCapabilities(msgRef); break;
        case _MSGID_le_mrc_GetTdScdmaBandCapabilities : Handle_le_mrc_GetTdScdmaBandCapabilities(msgRef); break;
        case _MSGID_le_mrc_AddNetworkRejectHandler : Handle_le_mrc_AddNetworkRejectHandler(msgRef); break;
        case _MSGID_le_mrc_RemoveNetworkRejectHandler : Handle_le_mrc_RemoveNetworkRejectHandler(msgRef); break;
        case _MSGID_le_mrc_AddJammingDetectionEventHandler : Handle_le_mrc_AddJammingDetectionEventHandler(msgRef); break;
        case _MSGID_le_mrc_RemoveJammingDetectionEventHandler : Handle_le_mrc_RemoveJammingDetectionEventHandler(msgRef); break;
        case _MSGID_le_mrc_StartJammingDetection : Handle_le_mrc_StartJammingDetection(msgRef); break;
        case _MSGID_le_mrc_StopJammingDetection : Handle_le_mrc_StopJammingDetection(msgRef); break;

        default: LE_ERROR("Unknowm msg id = %i", msgPtr->id);
    }

    // Clear the client session ref associated with the current message, since the message
    // has now been processed.
    _ClientSessionRef = 0;
}