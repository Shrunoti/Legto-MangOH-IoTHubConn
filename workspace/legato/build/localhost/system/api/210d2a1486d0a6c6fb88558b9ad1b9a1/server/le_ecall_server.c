
/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */


#include "le_ecall_messages.h"
#include "le_ecall_server.h"


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
le_msg_ServiceRef_t le_ecall_GetServiceRef
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
le_msg_SessionRef_t le_ecall_GetClientSessionRef
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
void le_ecall_AdvertiseService
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
    _ServerDataPool = le_mem_CreatePool("le_ecall_ServerData", sizeof(_ServerData_t));

    // Create safe reference map for handler references.
    // The size of the map should be based on the number of handlers defined for the server.
    // Don't expect that to be more than 2-3, so use 3 as a reasonable guess.
    _HandlerRefMap = le_ref_CreateMap("le_ecall_ServerHandlers", 3);

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


static void Handle_le_ecall_ForceOnlyMode
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
    _result  = le_ecall_ForceOnlyMode (  );

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


static void Handle_le_ecall_ForcePersistentOnlyMode
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
    _result  = le_ecall_ForcePersistentOnlyMode (  );

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


static void Handle_le_ecall_ExitOnlyMode
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
    _result  = le_ecall_ExitOnlyMode (  );

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


static void Handle_le_ecall_GetConfiguredOperationMode
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
    le_ecall_OpMode_t opModeBuffer;
    le_ecall_OpMode_t *opModePtr = &opModeBuffer;
    if (!(_requiredOutputs & (1u << 0)))
    {
        opModePtr = NULL;
    }

    // Call the function
    le_result_t _result;
    _result  = le_ecall_GetConfiguredOperationMode ( 
        opModePtr );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;
    _msgBufSize = _MAX_MSG_SIZE;

    // Pack the result first
    LE_ASSERT(le_pack_PackResult( &_msgBufPtr, &_msgBufSize, _result ));

    // Pack any "out" parameters
    if (opModePtr)
    {
        LE_ASSERT(le_pack_PackUint32( &_msgBufPtr, &_msgBufSize,
                                                      *opModePtr ));
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


static void AsyncResponse_le_ecall_AddStateChangeHandler
(
    le_ecall_CallRef_t ecallRef,
    le_ecall_State_t state,
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
    _msgPtr->id = _MSGID_le_ecall_AddStateChangeHandler;
    _msgBufPtr = _msgPtr->buffer;
    _msgBufSize = _MAX_MSG_SIZE;

    // Always pack the client context pointer first
    LE_ASSERT(le_pack_PackReference( &_msgBufPtr, &_msgBufSize, serverDataPtr->contextPtr ))

    // Pack the input parameters
    
    LE_ASSERT(le_pack_PackReference( &_msgBufPtr, &_msgBufSize,
                                                  ecallRef ));
    LE_ASSERT(le_pack_PackUint32( &_msgBufPtr, &_msgBufSize,
                                                  state ));

    // Send the async response to the client
    TRACE("Sending message to client session %p : %ti bytes sent",
          serverDataPtr->clientSessionRef,
          _msgBufPtr-_msgPtr->buffer);

    SendMsgToClient(_msgRef);
}


static void Handle_le_ecall_AddStateChangeHandler
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
    le_ecall_StateChangeHandlerRef_t _result;
    _result  = le_ecall_AddStateChangeHandler ( AsyncResponse_le_ecall_AddStateChangeHandler, 
        contextPtr );

    if (_result)
    {
        // Put the handler reference result and a pointer to the associated remove function
        // into the server data object.  This function pointer is needed in case the client
        // is closed and the handlers need to be removed.
        serverDataPtr->handlerRef = (le_event_HandlerRef_t)_result;
        serverDataPtr->removeHandlerFunc =
            (RemoveHandlerFunc_t)le_ecall_RemoveStateChangeHandler;

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


static void Handle_le_ecall_RemoveStateChangeHandler
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
    le_ecall_StateChangeHandlerRef_t handlerRef;
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
    handlerRef = (le_ecall_StateChangeHandlerRef_t)serverDataPtr->handlerRef;
    le_mem_Release(serverDataPtr);

    // Define storage for output parameters

    // Call the function
    le_ecall_RemoveStateChangeHandler ( 
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


static void Handle_le_ecall_Create
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
    le_ecall_CallRef_t _result;
    _result  = le_ecall_Create (  );

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


static void Handle_le_ecall_Delete
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
    le_ecall_CallRef_t ecallRef;
    if (!le_pack_UnpackReference( &_msgBufPtr, &_msgBufSize,
                                               &ecallRef ))
    {
        goto error_unpack;
    }

    // Define storage for output parameters

    // Call the function
    le_ecall_Delete ( 
        ecallRef );

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


static void Handle_le_ecall_SetSystemStandard
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
    le_ecall_SystemStandard_t systemStandard;
    if (!le_pack_UnpackUint32( &_msgBufPtr, &_msgBufSize,
                                               &systemStandard ))
    {
        goto error_unpack;
    }

    // Define storage for output parameters

    // Call the function
    le_result_t _result;
    _result  = le_ecall_SetSystemStandard ( 
        systemStandard );

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


static void Handle_le_ecall_GetSystemStandard
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
    le_ecall_SystemStandard_t systemStandardBuffer;
    le_ecall_SystemStandard_t *systemStandardPtr = &systemStandardBuffer;
    if (!(_requiredOutputs & (1u << 0)))
    {
        systemStandardPtr = NULL;
    }

    // Call the function
    le_result_t _result;
    _result  = le_ecall_GetSystemStandard ( 
        systemStandardPtr );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;
    _msgBufSize = _MAX_MSG_SIZE;

    // Pack the result first
    LE_ASSERT(le_pack_PackResult( &_msgBufPtr, &_msgBufSize, _result ));

    // Pack any "out" parameters
    if (systemStandardPtr)
    {
        LE_ASSERT(le_pack_PackUint32( &_msgBufPtr, &_msgBufSize,
                                                      *systemStandardPtr ));
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


static void Handle_le_ecall_SetMsdVersion
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
    uint32_t msdVersion;
    if (!le_pack_UnpackUint32( &_msgBufPtr, &_msgBufSize,
                                               &msdVersion ))
    {
        goto error_unpack;
    }

    // Define storage for output parameters

    // Call the function
    le_result_t _result;
    _result  = le_ecall_SetMsdVersion ( 
        msdVersion );

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


static void Handle_le_ecall_GetMsdVersion
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
    uint32_t msdVersionBuffer;
    uint32_t *msdVersionPtr = &msdVersionBuffer;
    if (!(_requiredOutputs & (1u << 0)))
    {
        msdVersionPtr = NULL;
    }

    // Call the function
    le_result_t _result;
    _result  = le_ecall_GetMsdVersion ( 
        msdVersionPtr );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;
    _msgBufSize = _MAX_MSG_SIZE;

    // Pack the result first
    LE_ASSERT(le_pack_PackResult( &_msgBufPtr, &_msgBufSize, _result ));

    // Pack any "out" parameters
    if (msdVersionPtr)
    {
        LE_ASSERT(le_pack_PackUint32( &_msgBufPtr, &_msgBufSize,
                                                      *msdVersionPtr ));
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


static void Handle_le_ecall_SetVehicleType
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
    le_ecall_MsdVehicleType_t vehicleType;
    if (!le_pack_UnpackUint32( &_msgBufPtr, &_msgBufSize,
                                               &vehicleType ))
    {
        goto error_unpack;
    }

    // Define storage for output parameters

    // Call the function
    le_result_t _result;
    _result  = le_ecall_SetVehicleType ( 
        vehicleType );

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


static void Handle_le_ecall_GetVehicleType
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
    le_ecall_MsdVehicleType_t vehicleTypeBuffer;
    le_ecall_MsdVehicleType_t *vehicleTypePtr = &vehicleTypeBuffer;
    if (!(_requiredOutputs & (1u << 0)))
    {
        vehicleTypePtr = NULL;
    }

    // Call the function
    le_result_t _result;
    _result  = le_ecall_GetVehicleType ( 
        vehicleTypePtr );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;
    _msgBufSize = _MAX_MSG_SIZE;

    // Pack the result first
    LE_ASSERT(le_pack_PackResult( &_msgBufPtr, &_msgBufSize, _result ));

    // Pack any "out" parameters
    if (vehicleTypePtr)
    {
        LE_ASSERT(le_pack_PackUint32( &_msgBufPtr, &_msgBufSize,
                                                      *vehicleTypePtr ));
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


static void Handle_le_ecall_SetVIN
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
    char vin[18];
    if (!le_pack_UnpackString( &_msgBufPtr, &_msgBufSize,
                               vin,
                               sizeof(vin),
                               17 ))
    {
        goto error_unpack;
    }

    // Define storage for output parameters

    // Call the function
    le_result_t _result;
    _result  = le_ecall_SetVIN ( 
        vin );

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


static void Handle_le_ecall_GetVIN
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
    size_t vinSize;
    if (!le_pack_UnpackSize( &_msgBufPtr, &_msgBufSize,
                               &vinSize ))
    {
        goto error_unpack;
    }
    if ( (vinSize > 18) &&
         (vinSize < UINT32_MAX) )
    {
        LE_DEBUG("Adjusting vinSize from %zu to 18", vinSize);
        vinSize = 18;
    }
    if (vinSize >= UINT32_MAX)
    {
        vinSize = 0;
    }
    else
    {
        vinSize++;
    }

    // Define storage for output parameters
    char vinBuffer[19];
    char *vin = vinBuffer;
    vin[0] = 0;
    if (!(_requiredOutputs & (1u << 0)))
    {
        vin = NULL;
        vinSize = 0;
    }

    // Call the function
    le_result_t _result;
    _result  = le_ecall_GetVIN ( 
        vin, 
        vinSize );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;
    _msgBufSize = _MAX_MSG_SIZE;

    // Pack the result first
    LE_ASSERT(le_pack_PackResult( &_msgBufPtr, &_msgBufSize, _result ));

    // Pack any "out" parameters
    if (vin)
    {
        LE_ASSERT(le_pack_PackString( &_msgBufPtr, &_msgBufSize,
                                      vin, 18 ));
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


static void Handle_le_ecall_SetPropulsionType
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
    le_ecall_PropulsionTypeBitMask_t propulsionType;
    if (!le_pack_UnpackUint32( &_msgBufPtr, &_msgBufSize,
                                               &propulsionType ))
    {
        goto error_unpack;
    }

    // Define storage for output parameters

    // Call the function
    le_result_t _result;
    _result  = le_ecall_SetPropulsionType ( 
        propulsionType );

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


static void Handle_le_ecall_GetPropulsionType
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
    le_ecall_PropulsionTypeBitMask_t propulsionTypeBuffer;
    le_ecall_PropulsionTypeBitMask_t *propulsionTypePtr = &propulsionTypeBuffer;
    if (!(_requiredOutputs & (1u << 0)))
    {
        propulsionTypePtr = NULL;
    }

    // Call the function
    le_result_t _result;
    _result  = le_ecall_GetPropulsionType ( 
        propulsionTypePtr );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;
    _msgBufSize = _MAX_MSG_SIZE;

    // Pack the result first
    LE_ASSERT(le_pack_PackResult( &_msgBufPtr, &_msgBufSize, _result ));

    // Pack any "out" parameters
    if (propulsionTypePtr)
    {
        LE_ASSERT(le_pack_PackUint32( &_msgBufPtr, &_msgBufSize,
                                                      *propulsionTypePtr ));
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


static void Handle_le_ecall_SetMsdTxMode
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
    le_ecall_MsdTxMode_t mode;
    if (!le_pack_UnpackUint32( &_msgBufPtr, &_msgBufSize,
                                               &mode ))
    {
        goto error_unpack;
    }

    // Define storage for output parameters

    // Call the function
    le_result_t _result;
    _result  = le_ecall_SetMsdTxMode ( 
        mode );

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


static void Handle_le_ecall_GetMsdTxMode
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
    le_ecall_MsdTxMode_t modeBuffer;
    le_ecall_MsdTxMode_t *modePtr = &modeBuffer;
    if (!(_requiredOutputs & (1u << 0)))
    {
        modePtr = NULL;
    }

    // Call the function
    le_result_t _result;
    _result  = le_ecall_GetMsdTxMode ( 
        modePtr );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;
    _msgBufSize = _MAX_MSG_SIZE;

    // Pack the result first
    LE_ASSERT(le_pack_PackResult( &_msgBufPtr, &_msgBufSize, _result ));

    // Pack any "out" parameters
    if (modePtr)
    {
        LE_ASSERT(le_pack_PackUint32( &_msgBufPtr, &_msgBufSize,
                                                      *modePtr ));
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


static void Handle_le_ecall_SetMsdPosition
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
    le_ecall_CallRef_t ecallRef;
    if (!le_pack_UnpackReference( &_msgBufPtr, &_msgBufSize,
                                               &ecallRef ))
    {
        goto error_unpack;
    }
    bool isTrusted;
    if (!le_pack_UnpackBool( &_msgBufPtr, &_msgBufSize,
                                               &isTrusted ))
    {
        goto error_unpack;
    }
    int32_t latitude;
    if (!le_pack_UnpackInt32( &_msgBufPtr, &_msgBufSize,
                                               &latitude ))
    {
        goto error_unpack;
    }
    int32_t longitude;
    if (!le_pack_UnpackInt32( &_msgBufPtr, &_msgBufSize,
                                               &longitude ))
    {
        goto error_unpack;
    }
    int32_t direction;
    if (!le_pack_UnpackInt32( &_msgBufPtr, &_msgBufSize,
                                               &direction ))
    {
        goto error_unpack;
    }

    // Define storage for output parameters

    // Call the function
    le_result_t _result;
    _result  = le_ecall_SetMsdPosition ( 
        ecallRef, 
        isTrusted, 
        latitude, 
        longitude, 
        direction );

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


static void Handle_le_ecall_SetMsdPositionN1
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
    le_ecall_CallRef_t ecallRef;
    if (!le_pack_UnpackReference( &_msgBufPtr, &_msgBufSize,
                                               &ecallRef ))
    {
        goto error_unpack;
    }
    int32_t latitudeDeltaN1;
    if (!le_pack_UnpackInt32( &_msgBufPtr, &_msgBufSize,
                                               &latitudeDeltaN1 ))
    {
        goto error_unpack;
    }
    int32_t longitudeDeltaN1;
    if (!le_pack_UnpackInt32( &_msgBufPtr, &_msgBufSize,
                                               &longitudeDeltaN1 ))
    {
        goto error_unpack;
    }

    // Define storage for output parameters

    // Call the function
    le_result_t _result;
    _result  = le_ecall_SetMsdPositionN1 ( 
        ecallRef, 
        latitudeDeltaN1, 
        longitudeDeltaN1 );

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


static void Handle_le_ecall_SetMsdPositionN2
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
    le_ecall_CallRef_t ecallRef;
    if (!le_pack_UnpackReference( &_msgBufPtr, &_msgBufSize,
                                               &ecallRef ))
    {
        goto error_unpack;
    }
    int32_t latitudeDeltaN2;
    if (!le_pack_UnpackInt32( &_msgBufPtr, &_msgBufSize,
                                               &latitudeDeltaN2 ))
    {
        goto error_unpack;
    }
    int32_t longitudeDeltaN2;
    if (!le_pack_UnpackInt32( &_msgBufPtr, &_msgBufSize,
                                               &longitudeDeltaN2 ))
    {
        goto error_unpack;
    }

    // Define storage for output parameters

    // Call the function
    le_result_t _result;
    _result  = le_ecall_SetMsdPositionN2 ( 
        ecallRef, 
        latitudeDeltaN2, 
        longitudeDeltaN2 );

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


static void Handle_le_ecall_SetMsdPassengersCount
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
    le_ecall_CallRef_t ecallRef;
    if (!le_pack_UnpackReference( &_msgBufPtr, &_msgBufSize,
                                               &ecallRef ))
    {
        goto error_unpack;
    }
    uint32_t paxCount;
    if (!le_pack_UnpackUint32( &_msgBufPtr, &_msgBufSize,
                                               &paxCount ))
    {
        goto error_unpack;
    }

    // Define storage for output parameters

    // Call the function
    le_result_t _result;
    _result  = le_ecall_SetMsdPassengersCount ( 
        ecallRef, 
        paxCount );

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


static void Handle_le_ecall_ImportMsd
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
    le_ecall_CallRef_t ecallRef;
    if (!le_pack_UnpackReference( &_msgBufPtr, &_msgBufSize,
                                               &ecallRef ))
    {
        goto error_unpack;
    }
    size_t msdSize;
    uint8_t msdPtr[140];
    bool msdResult;
    LE_PACK_UNPACKARRAY( &_msgBufPtr, &_msgBufSize,
                         msdPtr, &msdSize,
                         140,
                         le_pack_UnpackUint8,
                         &msdResult );
    if (!msdResult)
    {
        goto error_unpack;
    }

    // Define storage for output parameters

    // Call the function
    le_result_t _result;
    _result  = le_ecall_ImportMsd ( 
        ecallRef, 
        msdPtr, 
        msdSize );

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


static void Handle_le_ecall_ExportMsd
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
    le_ecall_CallRef_t ecallRef;
    if (!le_pack_UnpackReference( &_msgBufPtr, &_msgBufSize,
                                               &ecallRef ))
    {
        goto error_unpack;
    }
    size_t msdSize;
    if (!le_pack_UnpackSize( &_msgBufPtr, &_msgBufSize,
                               &msdSize ))
    {
        goto error_unpack;
    }
    if ( (msdSize > 140) )
    {
        LE_DEBUG("Adjusting msdSize from %zu to 140", msdSize);
        msdSize = 140;
    }

    // Define storage for output parameters
    uint8_t msdBuffer[140];
    uint8_t *msdPtr = msdBuffer;
    size_t *msdSizePtr = &msdSize;
    if (!(_requiredOutputs & (1u << 0)))
    {
        msdPtr = NULL;
    }

    // Call the function
    le_result_t _result;
    _result  = le_ecall_ExportMsd ( 
        ecallRef, 
        msdPtr, 
        &msdSize );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;
    _msgBufSize = _MAX_MSG_SIZE;

    // Pack the result first
    LE_ASSERT(le_pack_PackResult( &_msgBufPtr, &_msgBufSize, _result ));

    // Pack any "out" parameters
    if (msdPtr)
    {
        bool msdResult;
        LE_PACK_PACKARRAY( &_msgBufPtr, &_msgBufSize,
                           msdPtr, (*msdSizePtr),
                           140, le_pack_PackUint8,
                           &msdResult );
        LE_ASSERT(msdResult);
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


static void Handle_le_ecall_SendMsd
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
    le_ecall_CallRef_t ecallRef;
    if (!le_pack_UnpackReference( &_msgBufPtr, &_msgBufSize,
                                               &ecallRef ))
    {
        goto error_unpack;
    }

    // Define storage for output parameters

    // Call the function
    le_result_t _result;
    _result  = le_ecall_SendMsd ( 
        ecallRef );

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


static void Handle_le_ecall_StartAutomatic
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
    le_ecall_CallRef_t ecallRef;
    if (!le_pack_UnpackReference( &_msgBufPtr, &_msgBufSize,
                                               &ecallRef ))
    {
        goto error_unpack;
    }

    // Define storage for output parameters

    // Call the function
    le_result_t _result;
    _result  = le_ecall_StartAutomatic ( 
        ecallRef );

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


static void Handle_le_ecall_StartManual
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
    le_ecall_CallRef_t ecallRef;
    if (!le_pack_UnpackReference( &_msgBufPtr, &_msgBufSize,
                                               &ecallRef ))
    {
        goto error_unpack;
    }

    // Define storage for output parameters

    // Call the function
    le_result_t _result;
    _result  = le_ecall_StartManual ( 
        ecallRef );

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


static void Handle_le_ecall_StartTest
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
    le_ecall_CallRef_t ecallRef;
    if (!le_pack_UnpackReference( &_msgBufPtr, &_msgBufSize,
                                               &ecallRef ))
    {
        goto error_unpack;
    }

    // Define storage for output parameters

    // Call the function
    le_result_t _result;
    _result  = le_ecall_StartTest ( 
        ecallRef );

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


static void Handle_le_ecall_End
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
    le_ecall_CallRef_t ecallRef;
    if (!le_pack_UnpackReference( &_msgBufPtr, &_msgBufSize,
                                               &ecallRef ))
    {
        goto error_unpack;
    }

    // Define storage for output parameters

    // Call the function
    le_result_t _result;
    _result  = le_ecall_End ( 
        ecallRef );

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


static void Handle_le_ecall_GetState
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
    le_ecall_CallRef_t ecallRef;
    if (!le_pack_UnpackReference( &_msgBufPtr, &_msgBufSize,
                                               &ecallRef ))
    {
        goto error_unpack;
    }

    // Define storage for output parameters

    // Call the function
    le_ecall_State_t _result;
    _result  = le_ecall_GetState ( 
        ecallRef );

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


static void Handle_le_ecall_SetPsapNumber
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
    char psap[18];
    if (!le_pack_UnpackString( &_msgBufPtr, &_msgBufSize,
                               psap,
                               sizeof(psap),
                               17 ))
    {
        goto error_unpack;
    }

    // Define storage for output parameters

    // Call the function
    le_result_t _result;
    _result  = le_ecall_SetPsapNumber ( 
        psap );

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


static void Handle_le_ecall_GetPsapNumber
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
    size_t psapSize;
    if (!le_pack_UnpackSize( &_msgBufPtr, &_msgBufSize,
                               &psapSize ))
    {
        goto error_unpack;
    }
    if ( (psapSize > 17) &&
         (psapSize < UINT32_MAX) )
    {
        LE_DEBUG("Adjusting psapSize from %zu to 17", psapSize);
        psapSize = 17;
    }
    if (psapSize >= UINT32_MAX)
    {
        psapSize = 0;
    }
    else
    {
        psapSize++;
    }

    // Define storage for output parameters
    char psapBuffer[18];
    char *psap = psapBuffer;
    psap[0] = 0;
    if (!(_requiredOutputs & (1u << 0)))
    {
        psap = NULL;
        psapSize = 0;
    }

    // Call the function
    le_result_t _result;
    _result  = le_ecall_GetPsapNumber ( 
        psap, 
        psapSize );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;
    _msgBufSize = _MAX_MSG_SIZE;

    // Pack the result first
    LE_ASSERT(le_pack_PackResult( &_msgBufPtr, &_msgBufSize, _result ));

    // Pack any "out" parameters
    if (psap)
    {
        LE_ASSERT(le_pack_PackString( &_msgBufPtr, &_msgBufSize,
                                      psap, 17 ));
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


static void Handle_le_ecall_UseUSimNumbers
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
    _result  = le_ecall_UseUSimNumbers (  );

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


static void Handle_le_ecall_SetNadDeregistrationTime
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
    uint16_t deregTime;
    if (!le_pack_UnpackUint16( &_msgBufPtr, &_msgBufSize,
                                               &deregTime ))
    {
        goto error_unpack;
    }

    // Define storage for output parameters

    // Call the function
    le_result_t _result;
    _result  = le_ecall_SetNadDeregistrationTime ( 
        deregTime );

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


static void Handle_le_ecall_GetNadDeregistrationTime
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
    uint16_t deregTimeBuffer;
    uint16_t *deregTimePtr = &deregTimeBuffer;
    if (!(_requiredOutputs & (1u << 0)))
    {
        deregTimePtr = NULL;
    }

    // Call the function
    le_result_t _result;
    _result  = le_ecall_GetNadDeregistrationTime ( 
        deregTimePtr );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;
    _msgBufSize = _MAX_MSG_SIZE;

    // Pack the result first
    LE_ASSERT(le_pack_PackResult( &_msgBufPtr, &_msgBufSize, _result ));

    // Pack any "out" parameters
    if (deregTimePtr)
    {
        LE_ASSERT(le_pack_PackUint16( &_msgBufPtr, &_msgBufSize,
                                                      *deregTimePtr ));
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


static void Handle_le_ecall_SetIntervalBetweenDialAttempts
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
    uint16_t pause;
    if (!le_pack_UnpackUint16( &_msgBufPtr, &_msgBufSize,
                                               &pause ))
    {
        goto error_unpack;
    }

    // Define storage for output parameters

    // Call the function
    le_result_t _result;
    _result  = le_ecall_SetIntervalBetweenDialAttempts ( 
        pause );

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


static void Handle_le_ecall_GetIntervalBetweenDialAttempts
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
    uint16_t pauseBuffer;
    uint16_t *pausePtr = &pauseBuffer;
    if (!(_requiredOutputs & (1u << 0)))
    {
        pausePtr = NULL;
    }

    // Call the function
    le_result_t _result;
    _result  = le_ecall_GetIntervalBetweenDialAttempts ( 
        pausePtr );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;
    _msgBufSize = _MAX_MSG_SIZE;

    // Pack the result first
    LE_ASSERT(le_pack_PackResult( &_msgBufPtr, &_msgBufSize, _result ));

    // Pack any "out" parameters
    if (pausePtr)
    {
        LE_ASSERT(le_pack_PackUint16( &_msgBufPtr, &_msgBufSize,
                                                      *pausePtr ));
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


static void Handle_le_ecall_SetEraGlonassManualDialAttempts
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
    uint16_t attempts;
    if (!le_pack_UnpackUint16( &_msgBufPtr, &_msgBufSize,
                                               &attempts ))
    {
        goto error_unpack;
    }

    // Define storage for output parameters

    // Call the function
    le_result_t _result;
    _result  = le_ecall_SetEraGlonassManualDialAttempts ( 
        attempts );

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


static void Handle_le_ecall_SetEraGlonassAutoDialAttempts
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
    uint16_t attempts;
    if (!le_pack_UnpackUint16( &_msgBufPtr, &_msgBufSize,
                                               &attempts ))
    {
        goto error_unpack;
    }

    // Define storage for output parameters

    // Call the function
    le_result_t _result;
    _result  = le_ecall_SetEraGlonassAutoDialAttempts ( 
        attempts );

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


static void Handle_le_ecall_SetEraGlonassDialDuration
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
    uint16_t duration;
    if (!le_pack_UnpackUint16( &_msgBufPtr, &_msgBufSize,
                                               &duration ))
    {
        goto error_unpack;
    }

    // Define storage for output parameters

    // Call the function
    le_result_t _result;
    _result  = le_ecall_SetEraGlonassDialDuration ( 
        duration );

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


static void Handle_le_ecall_SetEraGlonassFallbackTime
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
    uint16_t duration;
    if (!le_pack_UnpackUint16( &_msgBufPtr, &_msgBufSize,
                                               &duration ))
    {
        goto error_unpack;
    }

    // Define storage for output parameters

    // Call the function
    le_result_t _result;
    _result  = le_ecall_SetEraGlonassFallbackTime ( 
        duration );

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


static void Handle_le_ecall_SetEraGlonassAutoAnswerTime
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
    uint16_t autoAnswerTime;
    if (!le_pack_UnpackUint16( &_msgBufPtr, &_msgBufSize,
                                               &autoAnswerTime ))
    {
        goto error_unpack;
    }

    // Define storage for output parameters

    // Call the function
    le_result_t _result;
    _result  = le_ecall_SetEraGlonassAutoAnswerTime ( 
        autoAnswerTime );

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


static void Handle_le_ecall_SetEraGlonassMSDMaxTransmissionTime
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
    uint16_t msdMaxTransTime;
    if (!le_pack_UnpackUint16( &_msgBufPtr, &_msgBufSize,
                                               &msdMaxTransTime ))
    {
        goto error_unpack;
    }

    // Define storage for output parameters

    // Call the function
    le_result_t _result;
    _result  = le_ecall_SetEraGlonassMSDMaxTransmissionTime ( 
        msdMaxTransTime );

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


static void Handle_le_ecall_GetEraGlonassManualDialAttempts
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
    uint16_t attemptsBuffer;
    uint16_t *attemptsPtr = &attemptsBuffer;
    if (!(_requiredOutputs & (1u << 0)))
    {
        attemptsPtr = NULL;
    }

    // Call the function
    le_result_t _result;
    _result  = le_ecall_GetEraGlonassManualDialAttempts ( 
        attemptsPtr );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;
    _msgBufSize = _MAX_MSG_SIZE;

    // Pack the result first
    LE_ASSERT(le_pack_PackResult( &_msgBufPtr, &_msgBufSize, _result ));

    // Pack any "out" parameters
    if (attemptsPtr)
    {
        LE_ASSERT(le_pack_PackUint16( &_msgBufPtr, &_msgBufSize,
                                                      *attemptsPtr ));
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


static void Handle_le_ecall_GetEraGlonassAutoDialAttempts
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
    uint16_t attemptsBuffer;
    uint16_t *attemptsPtr = &attemptsBuffer;
    if (!(_requiredOutputs & (1u << 0)))
    {
        attemptsPtr = NULL;
    }

    // Call the function
    le_result_t _result;
    _result  = le_ecall_GetEraGlonassAutoDialAttempts ( 
        attemptsPtr );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;
    _msgBufSize = _MAX_MSG_SIZE;

    // Pack the result first
    LE_ASSERT(le_pack_PackResult( &_msgBufPtr, &_msgBufSize, _result ));

    // Pack any "out" parameters
    if (attemptsPtr)
    {
        LE_ASSERT(le_pack_PackUint16( &_msgBufPtr, &_msgBufSize,
                                                      *attemptsPtr ));
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


static void Handle_le_ecall_GetEraGlonassDialDuration
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
    uint16_t durationBuffer;
    uint16_t *durationPtr = &durationBuffer;
    if (!(_requiredOutputs & (1u << 0)))
    {
        durationPtr = NULL;
    }

    // Call the function
    le_result_t _result;
    _result  = le_ecall_GetEraGlonassDialDuration ( 
        durationPtr );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;
    _msgBufSize = _MAX_MSG_SIZE;

    // Pack the result first
    LE_ASSERT(le_pack_PackResult( &_msgBufPtr, &_msgBufSize, _result ));

    // Pack any "out" parameters
    if (durationPtr)
    {
        LE_ASSERT(le_pack_PackUint16( &_msgBufPtr, &_msgBufSize,
                                                      *durationPtr ));
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


static void Handle_le_ecall_GetEraGlonassFallbackTime
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
    uint16_t durationBuffer;
    uint16_t *durationPtr = &durationBuffer;
    if (!(_requiredOutputs & (1u << 0)))
    {
        durationPtr = NULL;
    }

    // Call the function
    le_result_t _result;
    _result  = le_ecall_GetEraGlonassFallbackTime ( 
        durationPtr );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;
    _msgBufSize = _MAX_MSG_SIZE;

    // Pack the result first
    LE_ASSERT(le_pack_PackResult( &_msgBufPtr, &_msgBufSize, _result ));

    // Pack any "out" parameters
    if (durationPtr)
    {
        LE_ASSERT(le_pack_PackUint16( &_msgBufPtr, &_msgBufSize,
                                                      *durationPtr ));
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


static void Handle_le_ecall_GetEraGlonassAutoAnswerTime
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
    uint16_t autoAnswerTimeBuffer;
    uint16_t *autoAnswerTimePtr = &autoAnswerTimeBuffer;
    if (!(_requiredOutputs & (1u << 0)))
    {
        autoAnswerTimePtr = NULL;
    }

    // Call the function
    le_result_t _result;
    _result  = le_ecall_GetEraGlonassAutoAnswerTime ( 
        autoAnswerTimePtr );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;
    _msgBufSize = _MAX_MSG_SIZE;

    // Pack the result first
    LE_ASSERT(le_pack_PackResult( &_msgBufPtr, &_msgBufSize, _result ));

    // Pack any "out" parameters
    if (autoAnswerTimePtr)
    {
        LE_ASSERT(le_pack_PackUint16( &_msgBufPtr, &_msgBufSize,
                                                      *autoAnswerTimePtr ));
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


static void Handle_le_ecall_GetEraGlonassMSDMaxTransmissionTime
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
    uint16_t msdMaxTransTimeBuffer;
    uint16_t *msdMaxTransTimePtr = &msdMaxTransTimeBuffer;
    if (!(_requiredOutputs & (1u << 0)))
    {
        msdMaxTransTimePtr = NULL;
    }

    // Call the function
    le_result_t _result;
    _result  = le_ecall_GetEraGlonassMSDMaxTransmissionTime ( 
        msdMaxTransTimePtr );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;
    _msgBufSize = _MAX_MSG_SIZE;

    // Pack the result first
    LE_ASSERT(le_pack_PackResult( &_msgBufPtr, &_msgBufSize, _result ));

    // Pack any "out" parameters
    if (msdMaxTransTimePtr)
    {
        LE_ASSERT(le_pack_PackUint16( &_msgBufPtr, &_msgBufSize,
                                                      *msdMaxTransTimePtr ));
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


static void Handle_le_ecall_SetMsdEraGlonassCrashSeverity
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
    le_ecall_CallRef_t ecallRef;
    if (!le_pack_UnpackReference( &_msgBufPtr, &_msgBufSize,
                                               &ecallRef ))
    {
        goto error_unpack;
    }
    uint32_t crashSeverity;
    if (!le_pack_UnpackUint32( &_msgBufPtr, &_msgBufSize,
                                               &crashSeverity ))
    {
        goto error_unpack;
    }

    // Define storage for output parameters

    // Call the function
    le_result_t _result;
    _result  = le_ecall_SetMsdEraGlonassCrashSeverity ( 
        ecallRef, 
        crashSeverity );

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


static void Handle_le_ecall_ResetMsdEraGlonassCrashSeverity
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
    le_ecall_CallRef_t ecallRef;
    if (!le_pack_UnpackReference( &_msgBufPtr, &_msgBufSize,
                                               &ecallRef ))
    {
        goto error_unpack;
    }

    // Define storage for output parameters

    // Call the function
    le_result_t _result;
    _result  = le_ecall_ResetMsdEraGlonassCrashSeverity ( 
        ecallRef );

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


static void Handle_le_ecall_SetMsdEraGlonassDiagnosticResult
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
    le_ecall_CallRef_t ecallRef;
    if (!le_pack_UnpackReference( &_msgBufPtr, &_msgBufSize,
                                               &ecallRef ))
    {
        goto error_unpack;
    }
    le_ecall_DiagnosticResultBitMask_t diagnosticResultMask;
    if (!le_pack_UnpackUint64( &_msgBufPtr, &_msgBufSize,
                                               &diagnosticResultMask ))
    {
        goto error_unpack;
    }

    // Define storage for output parameters

    // Call the function
    le_result_t _result;
    _result  = le_ecall_SetMsdEraGlonassDiagnosticResult ( 
        ecallRef, 
        diagnosticResultMask );

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


static void Handle_le_ecall_ResetMsdEraGlonassDiagnosticResult
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
    le_ecall_CallRef_t ecallRef;
    if (!le_pack_UnpackReference( &_msgBufPtr, &_msgBufSize,
                                               &ecallRef ))
    {
        goto error_unpack;
    }

    // Define storage for output parameters

    // Call the function
    le_result_t _result;
    _result  = le_ecall_ResetMsdEraGlonassDiagnosticResult ( 
        ecallRef );

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


static void Handle_le_ecall_SetMsdEraGlonassCrashInfo
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
    le_ecall_CallRef_t ecallRef;
    if (!le_pack_UnpackReference( &_msgBufPtr, &_msgBufSize,
                                               &ecallRef ))
    {
        goto error_unpack;
    }
    le_ecall_CrashInfoBitMask_t crashInfoMask;
    if (!le_pack_UnpackUint32( &_msgBufPtr, &_msgBufSize,
                                               &crashInfoMask ))
    {
        goto error_unpack;
    }

    // Define storage for output parameters

    // Call the function
    le_result_t _result;
    _result  = le_ecall_SetMsdEraGlonassCrashInfo ( 
        ecallRef, 
        crashInfoMask );

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


static void Handle_le_ecall_ResetMsdEraGlonassCrashInfo
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
    le_ecall_CallRef_t ecallRef;
    if (!le_pack_UnpackReference( &_msgBufPtr, &_msgBufSize,
                                               &ecallRef ))
    {
        goto error_unpack;
    }

    // Define storage for output parameters

    // Call the function
    le_result_t _result;
    _result  = le_ecall_ResetMsdEraGlonassCrashInfo ( 
        ecallRef );

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


static void Handle_le_ecall_SetMsdEraGlonassCoordinateSystemType
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
    le_ecall_CallRef_t ecallRef;
    if (!le_pack_UnpackReference( &_msgBufPtr, &_msgBufSize,
                                               &ecallRef ))
    {
        goto error_unpack;
    }
    le_ecall_MsdCoordinateType_t coordinateSystemType;
    if (!le_pack_UnpackUint32( &_msgBufPtr, &_msgBufSize,
                                               &coordinateSystemType ))
    {
        goto error_unpack;
    }

    // Define storage for output parameters

    // Call the function
    le_result_t _result;
    _result  = le_ecall_SetMsdEraGlonassCoordinateSystemType ( 
        ecallRef, 
        coordinateSystemType );

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


static void Handle_le_ecall_ResetMsdEraGlonassCoordinateSystemType
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
    le_ecall_CallRef_t ecallRef;
    if (!le_pack_UnpackReference( &_msgBufPtr, &_msgBufSize,
                                               &ecallRef ))
    {
        goto error_unpack;
    }

    // Define storage for output parameters

    // Call the function
    le_result_t _result;
    _result  = le_ecall_ResetMsdEraGlonassCoordinateSystemType ( 
        ecallRef );

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


static void Handle_le_ecall_GetTerminationReason
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
    le_ecall_CallRef_t ecallRef;
    if (!le_pack_UnpackReference( &_msgBufPtr, &_msgBufSize,
                                               &ecallRef ))
    {
        goto error_unpack;
    }

    // Define storage for output parameters

    // Call the function
    le_mcc_TerminationReason_t _result;
    _result  = le_ecall_GetTerminationReason ( 
        ecallRef );

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


static void Handle_le_ecall_GetPlatformSpecificTerminationCode
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
    le_ecall_CallRef_t ecallRef;
    if (!le_pack_UnpackReference( &_msgBufPtr, &_msgBufSize,
                                               &ecallRef ))
    {
        goto error_unpack;
    }

    // Define storage for output parameters

    // Call the function
    int32_t _result;
    _result  = le_ecall_GetPlatformSpecificTerminationCode ( 
        ecallRef );

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
        case _MSGID_le_ecall_ForceOnlyMode : Handle_le_ecall_ForceOnlyMode(msgRef); break;
        case _MSGID_le_ecall_ForcePersistentOnlyMode : Handle_le_ecall_ForcePersistentOnlyMode(msgRef); break;
        case _MSGID_le_ecall_ExitOnlyMode : Handle_le_ecall_ExitOnlyMode(msgRef); break;
        case _MSGID_le_ecall_GetConfiguredOperationMode : Handle_le_ecall_GetConfiguredOperationMode(msgRef); break;
        case _MSGID_le_ecall_AddStateChangeHandler : Handle_le_ecall_AddStateChangeHandler(msgRef); break;
        case _MSGID_le_ecall_RemoveStateChangeHandler : Handle_le_ecall_RemoveStateChangeHandler(msgRef); break;
        case _MSGID_le_ecall_Create : Handle_le_ecall_Create(msgRef); break;
        case _MSGID_le_ecall_Delete : Handle_le_ecall_Delete(msgRef); break;
        case _MSGID_le_ecall_SetSystemStandard : Handle_le_ecall_SetSystemStandard(msgRef); break;
        case _MSGID_le_ecall_GetSystemStandard : Handle_le_ecall_GetSystemStandard(msgRef); break;
        case _MSGID_le_ecall_SetMsdVersion : Handle_le_ecall_SetMsdVersion(msgRef); break;
        case _MSGID_le_ecall_GetMsdVersion : Handle_le_ecall_GetMsdVersion(msgRef); break;
        case _MSGID_le_ecall_SetVehicleType : Handle_le_ecall_SetVehicleType(msgRef); break;
        case _MSGID_le_ecall_GetVehicleType : Handle_le_ecall_GetVehicleType(msgRef); break;
        case _MSGID_le_ecall_SetVIN : Handle_le_ecall_SetVIN(msgRef); break;
        case _MSGID_le_ecall_GetVIN : Handle_le_ecall_GetVIN(msgRef); break;
        case _MSGID_le_ecall_SetPropulsionType : Handle_le_ecall_SetPropulsionType(msgRef); break;
        case _MSGID_le_ecall_GetPropulsionType : Handle_le_ecall_GetPropulsionType(msgRef); break;
        case _MSGID_le_ecall_SetMsdTxMode : Handle_le_ecall_SetMsdTxMode(msgRef); break;
        case _MSGID_le_ecall_GetMsdTxMode : Handle_le_ecall_GetMsdTxMode(msgRef); break;
        case _MSGID_le_ecall_SetMsdPosition : Handle_le_ecall_SetMsdPosition(msgRef); break;
        case _MSGID_le_ecall_SetMsdPositionN1 : Handle_le_ecall_SetMsdPositionN1(msgRef); break;
        case _MSGID_le_ecall_SetMsdPositionN2 : Handle_le_ecall_SetMsdPositionN2(msgRef); break;
        case _MSGID_le_ecall_SetMsdPassengersCount : Handle_le_ecall_SetMsdPassengersCount(msgRef); break;
        case _MSGID_le_ecall_ImportMsd : Handle_le_ecall_ImportMsd(msgRef); break;
        case _MSGID_le_ecall_ExportMsd : Handle_le_ecall_ExportMsd(msgRef); break;
        case _MSGID_le_ecall_SendMsd : Handle_le_ecall_SendMsd(msgRef); break;
        case _MSGID_le_ecall_StartAutomatic : Handle_le_ecall_StartAutomatic(msgRef); break;
        case _MSGID_le_ecall_StartManual : Handle_le_ecall_StartManual(msgRef); break;
        case _MSGID_le_ecall_StartTest : Handle_le_ecall_StartTest(msgRef); break;
        case _MSGID_le_ecall_End : Handle_le_ecall_End(msgRef); break;
        case _MSGID_le_ecall_GetState : Handle_le_ecall_GetState(msgRef); break;
        case _MSGID_le_ecall_SetPsapNumber : Handle_le_ecall_SetPsapNumber(msgRef); break;
        case _MSGID_le_ecall_GetPsapNumber : Handle_le_ecall_GetPsapNumber(msgRef); break;
        case _MSGID_le_ecall_UseUSimNumbers : Handle_le_ecall_UseUSimNumbers(msgRef); break;
        case _MSGID_le_ecall_SetNadDeregistrationTime : Handle_le_ecall_SetNadDeregistrationTime(msgRef); break;
        case _MSGID_le_ecall_GetNadDeregistrationTime : Handle_le_ecall_GetNadDeregistrationTime(msgRef); break;
        case _MSGID_le_ecall_SetIntervalBetweenDialAttempts : Handle_le_ecall_SetIntervalBetweenDialAttempts(msgRef); break;
        case _MSGID_le_ecall_GetIntervalBetweenDialAttempts : Handle_le_ecall_GetIntervalBetweenDialAttempts(msgRef); break;
        case _MSGID_le_ecall_SetEraGlonassManualDialAttempts : Handle_le_ecall_SetEraGlonassManualDialAttempts(msgRef); break;
        case _MSGID_le_ecall_SetEraGlonassAutoDialAttempts : Handle_le_ecall_SetEraGlonassAutoDialAttempts(msgRef); break;
        case _MSGID_le_ecall_SetEraGlonassDialDuration : Handle_le_ecall_SetEraGlonassDialDuration(msgRef); break;
        case _MSGID_le_ecall_SetEraGlonassFallbackTime : Handle_le_ecall_SetEraGlonassFallbackTime(msgRef); break;
        case _MSGID_le_ecall_SetEraGlonassAutoAnswerTime : Handle_le_ecall_SetEraGlonassAutoAnswerTime(msgRef); break;
        case _MSGID_le_ecall_SetEraGlonassMSDMaxTransmissionTime : Handle_le_ecall_SetEraGlonassMSDMaxTransmissionTime(msgRef); break;
        case _MSGID_le_ecall_GetEraGlonassManualDialAttempts : Handle_le_ecall_GetEraGlonassManualDialAttempts(msgRef); break;
        case _MSGID_le_ecall_GetEraGlonassAutoDialAttempts : Handle_le_ecall_GetEraGlonassAutoDialAttempts(msgRef); break;
        case _MSGID_le_ecall_GetEraGlonassDialDuration : Handle_le_ecall_GetEraGlonassDialDuration(msgRef); break;
        case _MSGID_le_ecall_GetEraGlonassFallbackTime : Handle_le_ecall_GetEraGlonassFallbackTime(msgRef); break;
        case _MSGID_le_ecall_GetEraGlonassAutoAnswerTime : Handle_le_ecall_GetEraGlonassAutoAnswerTime(msgRef); break;
        case _MSGID_le_ecall_GetEraGlonassMSDMaxTransmissionTime : Handle_le_ecall_GetEraGlonassMSDMaxTransmissionTime(msgRef); break;
        case _MSGID_le_ecall_SetMsdEraGlonassCrashSeverity : Handle_le_ecall_SetMsdEraGlonassCrashSeverity(msgRef); break;
        case _MSGID_le_ecall_ResetMsdEraGlonassCrashSeverity : Handle_le_ecall_ResetMsdEraGlonassCrashSeverity(msgRef); break;
        case _MSGID_le_ecall_SetMsdEraGlonassDiagnosticResult : Handle_le_ecall_SetMsdEraGlonassDiagnosticResult(msgRef); break;
        case _MSGID_le_ecall_ResetMsdEraGlonassDiagnosticResult : Handle_le_ecall_ResetMsdEraGlonassDiagnosticResult(msgRef); break;
        case _MSGID_le_ecall_SetMsdEraGlonassCrashInfo : Handle_le_ecall_SetMsdEraGlonassCrashInfo(msgRef); break;
        case _MSGID_le_ecall_ResetMsdEraGlonassCrashInfo : Handle_le_ecall_ResetMsdEraGlonassCrashInfo(msgRef); break;
        case _MSGID_le_ecall_SetMsdEraGlonassCoordinateSystemType : Handle_le_ecall_SetMsdEraGlonassCoordinateSystemType(msgRef); break;
        case _MSGID_le_ecall_ResetMsdEraGlonassCoordinateSystemType : Handle_le_ecall_ResetMsdEraGlonassCoordinateSystemType(msgRef); break;
        case _MSGID_le_ecall_GetTerminationReason : Handle_le_ecall_GetTerminationReason(msgRef); break;
        case _MSGID_le_ecall_GetPlatformSpecificTerminationCode : Handle_le_ecall_GetPlatformSpecificTerminationCode(msgRef); break;

        default: LE_ERROR("Unknowm msg id = %i", msgPtr->id);
    }

    // Clear the client session ref associated with the current message, since the message
    // has now been processed.
    _ClientSessionRef = 0;
}