
/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */


#include "le_pos_messages.h"
#include "le_pos_server.h"


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
le_msg_ServiceRef_t le_pos_GetServiceRef
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
le_msg_SessionRef_t le_pos_GetClientSessionRef
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
void le_pos_AdvertiseService
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
    _ServerDataPool = le_mem_CreatePool("le_pos_ServerData", sizeof(_ServerData_t));

    // Create safe reference map for handler references.
    // The size of the map should be based on the number of handlers defined for the server.
    // Don't expect that to be more than 2-3, so use 3 as a reasonable guess.
    _HandlerRefMap = le_ref_CreateMap("le_pos_ServerHandlers", 3);

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


static void AsyncResponse_le_pos_AddMovementHandler
(
    le_pos_SampleRef_t positionSampleRef,
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
    _msgPtr->id = _MSGID_le_pos_AddMovementHandler;
    _msgBufPtr = _msgPtr->buffer;
    _msgBufSize = _MAX_MSG_SIZE;

    // Always pack the client context pointer first
    LE_ASSERT(le_pack_PackReference( &_msgBufPtr, &_msgBufSize, serverDataPtr->contextPtr ))

    // Pack the input parameters
    
    LE_ASSERT(le_pack_PackReference( &_msgBufPtr, &_msgBufSize,
                                                  positionSampleRef ));

    // Send the async response to the client
    TRACE("Sending message to client session %p : %ti bytes sent",
          serverDataPtr->clientSessionRef,
          _msgBufPtr-_msgPtr->buffer);

    SendMsgToClient(_msgRef);
}


static void Handle_le_pos_AddMovementHandler
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
    uint32_t horizontalMagnitude;
    if (!le_pack_UnpackUint32( &_msgBufPtr, &_msgBufSize,
                                               &horizontalMagnitude ))
    {
        goto error_unpack;
    }
    uint32_t verticalMagnitude;
    if (!le_pack_UnpackUint32( &_msgBufPtr, &_msgBufSize,
                                               &verticalMagnitude ))
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
    le_pos_MovementHandlerRef_t _result;
    _result  = le_pos_AddMovementHandler ( 
        horizontalMagnitude, 
        verticalMagnitude, AsyncResponse_le_pos_AddMovementHandler, 
        contextPtr );

    if (_result)
    {
        // Put the handler reference result and a pointer to the associated remove function
        // into the server data object.  This function pointer is needed in case the client
        // is closed and the handlers need to be removed.
        serverDataPtr->handlerRef = (le_event_HandlerRef_t)_result;
        serverDataPtr->removeHandlerFunc =
            (RemoveHandlerFunc_t)le_pos_RemoveMovementHandler;

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


static void Handle_le_pos_RemoveMovementHandler
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
    le_pos_MovementHandlerRef_t handlerRef;
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
    handlerRef = (le_pos_MovementHandlerRef_t)serverDataPtr->handlerRef;
    le_mem_Release(serverDataPtr);

    // Define storage for output parameters

    // Call the function
    le_pos_RemoveMovementHandler ( 
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


static void Handle_le_pos_Get2DLocation
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
    int32_t latitudeBuffer;
    int32_t *latitudePtr = &latitudeBuffer;
    if (!(_requiredOutputs & (1u << 0)))
    {
        latitudePtr = NULL;
    }
    int32_t longitudeBuffer;
    int32_t *longitudePtr = &longitudeBuffer;
    if (!(_requiredOutputs & (1u << 1)))
    {
        longitudePtr = NULL;
    }
    int32_t hAccuracyBuffer;
    int32_t *hAccuracyPtr = &hAccuracyBuffer;
    if (!(_requiredOutputs & (1u << 2)))
    {
        hAccuracyPtr = NULL;
    }

    // Call the function
    le_result_t _result;
    _result  = le_pos_Get2DLocation ( 
        latitudePtr, 
        longitudePtr, 
        hAccuracyPtr );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;
    _msgBufSize = _MAX_MSG_SIZE;

    // Pack the result first
    LE_ASSERT(le_pack_PackResult( &_msgBufPtr, &_msgBufSize, _result ));

    // Pack any "out" parameters
    if (latitudePtr)
    {
        LE_ASSERT(le_pack_PackInt32( &_msgBufPtr, &_msgBufSize,
                                                      *latitudePtr ));
    }
    if (longitudePtr)
    {
        LE_ASSERT(le_pack_PackInt32( &_msgBufPtr, &_msgBufSize,
                                                      *longitudePtr ));
    }
    if (hAccuracyPtr)
    {
        LE_ASSERT(le_pack_PackInt32( &_msgBufPtr, &_msgBufSize,
                                                      *hAccuracyPtr ));
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


static void Handle_le_pos_Get3DLocation
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
    int32_t latitudeBuffer;
    int32_t *latitudePtr = &latitudeBuffer;
    if (!(_requiredOutputs & (1u << 0)))
    {
        latitudePtr = NULL;
    }
    int32_t longitudeBuffer;
    int32_t *longitudePtr = &longitudeBuffer;
    if (!(_requiredOutputs & (1u << 1)))
    {
        longitudePtr = NULL;
    }
    int32_t hAccuracyBuffer;
    int32_t *hAccuracyPtr = &hAccuracyBuffer;
    if (!(_requiredOutputs & (1u << 2)))
    {
        hAccuracyPtr = NULL;
    }
    int32_t altitudeBuffer;
    int32_t *altitudePtr = &altitudeBuffer;
    if (!(_requiredOutputs & (1u << 3)))
    {
        altitudePtr = NULL;
    }
    int32_t vAccuracyBuffer;
    int32_t *vAccuracyPtr = &vAccuracyBuffer;
    if (!(_requiredOutputs & (1u << 4)))
    {
        vAccuracyPtr = NULL;
    }

    // Call the function
    le_result_t _result;
    _result  = le_pos_Get3DLocation ( 
        latitudePtr, 
        longitudePtr, 
        hAccuracyPtr, 
        altitudePtr, 
        vAccuracyPtr );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;
    _msgBufSize = _MAX_MSG_SIZE;

    // Pack the result first
    LE_ASSERT(le_pack_PackResult( &_msgBufPtr, &_msgBufSize, _result ));

    // Pack any "out" parameters
    if (latitudePtr)
    {
        LE_ASSERT(le_pack_PackInt32( &_msgBufPtr, &_msgBufSize,
                                                      *latitudePtr ));
    }
    if (longitudePtr)
    {
        LE_ASSERT(le_pack_PackInt32( &_msgBufPtr, &_msgBufSize,
                                                      *longitudePtr ));
    }
    if (hAccuracyPtr)
    {
        LE_ASSERT(le_pack_PackInt32( &_msgBufPtr, &_msgBufSize,
                                                      *hAccuracyPtr ));
    }
    if (altitudePtr)
    {
        LE_ASSERT(le_pack_PackInt32( &_msgBufPtr, &_msgBufSize,
                                                      *altitudePtr ));
    }
    if (vAccuracyPtr)
    {
        LE_ASSERT(le_pack_PackInt32( &_msgBufPtr, &_msgBufSize,
                                                      *vAccuracyPtr ));
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


static void Handle_le_pos_GetTime
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
    uint16_t hoursBuffer;
    uint16_t *hoursPtr = &hoursBuffer;
    if (!(_requiredOutputs & (1u << 0)))
    {
        hoursPtr = NULL;
    }
    uint16_t minutesBuffer;
    uint16_t *minutesPtr = &minutesBuffer;
    if (!(_requiredOutputs & (1u << 1)))
    {
        minutesPtr = NULL;
    }
    uint16_t secondsBuffer;
    uint16_t *secondsPtr = &secondsBuffer;
    if (!(_requiredOutputs & (1u << 2)))
    {
        secondsPtr = NULL;
    }
    uint16_t millisecondsBuffer;
    uint16_t *millisecondsPtr = &millisecondsBuffer;
    if (!(_requiredOutputs & (1u << 3)))
    {
        millisecondsPtr = NULL;
    }

    // Call the function
    le_result_t _result;
    _result  = le_pos_GetTime ( 
        hoursPtr, 
        minutesPtr, 
        secondsPtr, 
        millisecondsPtr );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;
    _msgBufSize = _MAX_MSG_SIZE;

    // Pack the result first
    LE_ASSERT(le_pack_PackResult( &_msgBufPtr, &_msgBufSize, _result ));

    // Pack any "out" parameters
    if (hoursPtr)
    {
        LE_ASSERT(le_pack_PackUint16( &_msgBufPtr, &_msgBufSize,
                                                      *hoursPtr ));
    }
    if (minutesPtr)
    {
        LE_ASSERT(le_pack_PackUint16( &_msgBufPtr, &_msgBufSize,
                                                      *minutesPtr ));
    }
    if (secondsPtr)
    {
        LE_ASSERT(le_pack_PackUint16( &_msgBufPtr, &_msgBufSize,
                                                      *secondsPtr ));
    }
    if (millisecondsPtr)
    {
        LE_ASSERT(le_pack_PackUint16( &_msgBufPtr, &_msgBufSize,
                                                      *millisecondsPtr ));
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


static void Handle_le_pos_GetDate
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
    uint16_t yearBuffer;
    uint16_t *yearPtr = &yearBuffer;
    if (!(_requiredOutputs & (1u << 0)))
    {
        yearPtr = NULL;
    }
    uint16_t monthBuffer;
    uint16_t *monthPtr = &monthBuffer;
    if (!(_requiredOutputs & (1u << 1)))
    {
        monthPtr = NULL;
    }
    uint16_t dayBuffer;
    uint16_t *dayPtr = &dayBuffer;
    if (!(_requiredOutputs & (1u << 2)))
    {
        dayPtr = NULL;
    }

    // Call the function
    le_result_t _result;
    _result  = le_pos_GetDate ( 
        yearPtr, 
        monthPtr, 
        dayPtr );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;
    _msgBufSize = _MAX_MSG_SIZE;

    // Pack the result first
    LE_ASSERT(le_pack_PackResult( &_msgBufPtr, &_msgBufSize, _result ));

    // Pack any "out" parameters
    if (yearPtr)
    {
        LE_ASSERT(le_pack_PackUint16( &_msgBufPtr, &_msgBufSize,
                                                      *yearPtr ));
    }
    if (monthPtr)
    {
        LE_ASSERT(le_pack_PackUint16( &_msgBufPtr, &_msgBufSize,
                                                      *monthPtr ));
    }
    if (dayPtr)
    {
        LE_ASSERT(le_pack_PackUint16( &_msgBufPtr, &_msgBufSize,
                                                      *dayPtr ));
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


static void Handle_le_pos_GetMotion
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
    uint32_t hSpeedBuffer;
    uint32_t *hSpeedPtr = &hSpeedBuffer;
    if (!(_requiredOutputs & (1u << 0)))
    {
        hSpeedPtr = NULL;
    }
    uint32_t hSpeedAccuracyBuffer;
    uint32_t *hSpeedAccuracyPtr = &hSpeedAccuracyBuffer;
    if (!(_requiredOutputs & (1u << 1)))
    {
        hSpeedAccuracyPtr = NULL;
    }
    int32_t vSpeedBuffer;
    int32_t *vSpeedPtr = &vSpeedBuffer;
    if (!(_requiredOutputs & (1u << 2)))
    {
        vSpeedPtr = NULL;
    }
    int32_t vSpeedAccuracyBuffer;
    int32_t *vSpeedAccuracyPtr = &vSpeedAccuracyBuffer;
    if (!(_requiredOutputs & (1u << 3)))
    {
        vSpeedAccuracyPtr = NULL;
    }

    // Call the function
    le_result_t _result;
    _result  = le_pos_GetMotion ( 
        hSpeedPtr, 
        hSpeedAccuracyPtr, 
        vSpeedPtr, 
        vSpeedAccuracyPtr );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;
    _msgBufSize = _MAX_MSG_SIZE;

    // Pack the result first
    LE_ASSERT(le_pack_PackResult( &_msgBufPtr, &_msgBufSize, _result ));

    // Pack any "out" parameters
    if (hSpeedPtr)
    {
        LE_ASSERT(le_pack_PackUint32( &_msgBufPtr, &_msgBufSize,
                                                      *hSpeedPtr ));
    }
    if (hSpeedAccuracyPtr)
    {
        LE_ASSERT(le_pack_PackUint32( &_msgBufPtr, &_msgBufSize,
                                                      *hSpeedAccuracyPtr ));
    }
    if (vSpeedPtr)
    {
        LE_ASSERT(le_pack_PackInt32( &_msgBufPtr, &_msgBufSize,
                                                      *vSpeedPtr ));
    }
    if (vSpeedAccuracyPtr)
    {
        LE_ASSERT(le_pack_PackInt32( &_msgBufPtr, &_msgBufSize,
                                                      *vSpeedAccuracyPtr ));
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


static void Handle_le_pos_GetHeading
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
    uint32_t headingBuffer;
    uint32_t *headingPtr = &headingBuffer;
    if (!(_requiredOutputs & (1u << 0)))
    {
        headingPtr = NULL;
    }
    uint32_t headingAccuracyBuffer;
    uint32_t *headingAccuracyPtr = &headingAccuracyBuffer;
    if (!(_requiredOutputs & (1u << 1)))
    {
        headingAccuracyPtr = NULL;
    }

    // Call the function
    le_result_t _result;
    _result  = le_pos_GetHeading ( 
        headingPtr, 
        headingAccuracyPtr );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;
    _msgBufSize = _MAX_MSG_SIZE;

    // Pack the result first
    LE_ASSERT(le_pack_PackResult( &_msgBufPtr, &_msgBufSize, _result ));

    // Pack any "out" parameters
    if (headingPtr)
    {
        LE_ASSERT(le_pack_PackUint32( &_msgBufPtr, &_msgBufSize,
                                                      *headingPtr ));
    }
    if (headingAccuracyPtr)
    {
        LE_ASSERT(le_pack_PackUint32( &_msgBufPtr, &_msgBufSize,
                                                      *headingAccuracyPtr ));
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


static void Handle_le_pos_GetDirection
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
    uint32_t directionBuffer;
    uint32_t *directionPtr = &directionBuffer;
    if (!(_requiredOutputs & (1u << 0)))
    {
        directionPtr = NULL;
    }
    uint32_t directionAccuracyBuffer;
    uint32_t *directionAccuracyPtr = &directionAccuracyBuffer;
    if (!(_requiredOutputs & (1u << 1)))
    {
        directionAccuracyPtr = NULL;
    }

    // Call the function
    le_result_t _result;
    _result  = le_pos_GetDirection ( 
        directionPtr, 
        directionAccuracyPtr );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;
    _msgBufSize = _MAX_MSG_SIZE;

    // Pack the result first
    LE_ASSERT(le_pack_PackResult( &_msgBufPtr, &_msgBufSize, _result ));

    // Pack any "out" parameters
    if (directionPtr)
    {
        LE_ASSERT(le_pack_PackUint32( &_msgBufPtr, &_msgBufSize,
                                                      *directionPtr ));
    }
    if (directionAccuracyPtr)
    {
        LE_ASSERT(le_pack_PackUint32( &_msgBufPtr, &_msgBufSize,
                                                      *directionAccuracyPtr ));
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


static void Handle_le_pos_GetFixState
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
    le_pos_FixState_t stateBuffer;
    le_pos_FixState_t *statePtr = &stateBuffer;
    if (!(_requiredOutputs & (1u << 0)))
    {
        statePtr = NULL;
    }

    // Call the function
    le_result_t _result;
    _result  = le_pos_GetFixState ( 
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


static void Handle_le_pos_sample_Get2DLocation
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
    le_pos_SampleRef_t positionSampleRef;
    if (!le_pack_UnpackReference( &_msgBufPtr, &_msgBufSize,
                                               &positionSampleRef ))
    {
        goto error_unpack;
    }

    // Define storage for output parameters
    int32_t latitudeBuffer;
    int32_t *latitudePtr = &latitudeBuffer;
    if (!(_requiredOutputs & (1u << 0)))
    {
        latitudePtr = NULL;
    }
    int32_t longitudeBuffer;
    int32_t *longitudePtr = &longitudeBuffer;
    if (!(_requiredOutputs & (1u << 1)))
    {
        longitudePtr = NULL;
    }
    int32_t horizontalAccuracyBuffer;
    int32_t *horizontalAccuracyPtr = &horizontalAccuracyBuffer;
    if (!(_requiredOutputs & (1u << 2)))
    {
        horizontalAccuracyPtr = NULL;
    }

    // Call the function
    le_result_t _result;
    _result  = le_pos_sample_Get2DLocation ( 
        positionSampleRef, 
        latitudePtr, 
        longitudePtr, 
        horizontalAccuracyPtr );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;
    _msgBufSize = _MAX_MSG_SIZE;

    // Pack the result first
    LE_ASSERT(le_pack_PackResult( &_msgBufPtr, &_msgBufSize, _result ));

    // Pack any "out" parameters
    if (latitudePtr)
    {
        LE_ASSERT(le_pack_PackInt32( &_msgBufPtr, &_msgBufSize,
                                                      *latitudePtr ));
    }
    if (longitudePtr)
    {
        LE_ASSERT(le_pack_PackInt32( &_msgBufPtr, &_msgBufSize,
                                                      *longitudePtr ));
    }
    if (horizontalAccuracyPtr)
    {
        LE_ASSERT(le_pack_PackInt32( &_msgBufPtr, &_msgBufSize,
                                                      *horizontalAccuracyPtr ));
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


static void Handle_le_pos_sample_GetTime
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
    le_pos_SampleRef_t positionSampleRef;
    if (!le_pack_UnpackReference( &_msgBufPtr, &_msgBufSize,
                                               &positionSampleRef ))
    {
        goto error_unpack;
    }

    // Define storage for output parameters
    uint16_t hoursBuffer;
    uint16_t *hoursPtr = &hoursBuffer;
    if (!(_requiredOutputs & (1u << 0)))
    {
        hoursPtr = NULL;
    }
    uint16_t minutesBuffer;
    uint16_t *minutesPtr = &minutesBuffer;
    if (!(_requiredOutputs & (1u << 1)))
    {
        minutesPtr = NULL;
    }
    uint16_t secondsBuffer;
    uint16_t *secondsPtr = &secondsBuffer;
    if (!(_requiredOutputs & (1u << 2)))
    {
        secondsPtr = NULL;
    }
    uint16_t millisecondsBuffer;
    uint16_t *millisecondsPtr = &millisecondsBuffer;
    if (!(_requiredOutputs & (1u << 3)))
    {
        millisecondsPtr = NULL;
    }

    // Call the function
    le_result_t _result;
    _result  = le_pos_sample_GetTime ( 
        positionSampleRef, 
        hoursPtr, 
        minutesPtr, 
        secondsPtr, 
        millisecondsPtr );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;
    _msgBufSize = _MAX_MSG_SIZE;

    // Pack the result first
    LE_ASSERT(le_pack_PackResult( &_msgBufPtr, &_msgBufSize, _result ));

    // Pack any "out" parameters
    if (hoursPtr)
    {
        LE_ASSERT(le_pack_PackUint16( &_msgBufPtr, &_msgBufSize,
                                                      *hoursPtr ));
    }
    if (minutesPtr)
    {
        LE_ASSERT(le_pack_PackUint16( &_msgBufPtr, &_msgBufSize,
                                                      *minutesPtr ));
    }
    if (secondsPtr)
    {
        LE_ASSERT(le_pack_PackUint16( &_msgBufPtr, &_msgBufSize,
                                                      *secondsPtr ));
    }
    if (millisecondsPtr)
    {
        LE_ASSERT(le_pack_PackUint16( &_msgBufPtr, &_msgBufSize,
                                                      *millisecondsPtr ));
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


static void Handle_le_pos_sample_GetDate
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
    le_pos_SampleRef_t positionSampleRef;
    if (!le_pack_UnpackReference( &_msgBufPtr, &_msgBufSize,
                                               &positionSampleRef ))
    {
        goto error_unpack;
    }

    // Define storage for output parameters
    uint16_t yearBuffer;
    uint16_t *yearPtr = &yearBuffer;
    if (!(_requiredOutputs & (1u << 0)))
    {
        yearPtr = NULL;
    }
    uint16_t monthBuffer;
    uint16_t *monthPtr = &monthBuffer;
    if (!(_requiredOutputs & (1u << 1)))
    {
        monthPtr = NULL;
    }
    uint16_t dayBuffer;
    uint16_t *dayPtr = &dayBuffer;
    if (!(_requiredOutputs & (1u << 2)))
    {
        dayPtr = NULL;
    }

    // Call the function
    le_result_t _result;
    _result  = le_pos_sample_GetDate ( 
        positionSampleRef, 
        yearPtr, 
        monthPtr, 
        dayPtr );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;
    _msgBufSize = _MAX_MSG_SIZE;

    // Pack the result first
    LE_ASSERT(le_pack_PackResult( &_msgBufPtr, &_msgBufSize, _result ));

    // Pack any "out" parameters
    if (yearPtr)
    {
        LE_ASSERT(le_pack_PackUint16( &_msgBufPtr, &_msgBufSize,
                                                      *yearPtr ));
    }
    if (monthPtr)
    {
        LE_ASSERT(le_pack_PackUint16( &_msgBufPtr, &_msgBufSize,
                                                      *monthPtr ));
    }
    if (dayPtr)
    {
        LE_ASSERT(le_pack_PackUint16( &_msgBufPtr, &_msgBufSize,
                                                      *dayPtr ));
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


static void Handle_le_pos_sample_GetAltitude
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
    le_pos_SampleRef_t positionSampleRef;
    if (!le_pack_UnpackReference( &_msgBufPtr, &_msgBufSize,
                                               &positionSampleRef ))
    {
        goto error_unpack;
    }

    // Define storage for output parameters
    int32_t altitudeBuffer;
    int32_t *altitudePtr = &altitudeBuffer;
    if (!(_requiredOutputs & (1u << 0)))
    {
        altitudePtr = NULL;
    }
    int32_t altitudeAccuracyBuffer;
    int32_t *altitudeAccuracyPtr = &altitudeAccuracyBuffer;
    if (!(_requiredOutputs & (1u << 1)))
    {
        altitudeAccuracyPtr = NULL;
    }

    // Call the function
    le_result_t _result;
    _result  = le_pos_sample_GetAltitude ( 
        positionSampleRef, 
        altitudePtr, 
        altitudeAccuracyPtr );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;
    _msgBufSize = _MAX_MSG_SIZE;

    // Pack the result first
    LE_ASSERT(le_pack_PackResult( &_msgBufPtr, &_msgBufSize, _result ));

    // Pack any "out" parameters
    if (altitudePtr)
    {
        LE_ASSERT(le_pack_PackInt32( &_msgBufPtr, &_msgBufSize,
                                                      *altitudePtr ));
    }
    if (altitudeAccuracyPtr)
    {
        LE_ASSERT(le_pack_PackInt32( &_msgBufPtr, &_msgBufSize,
                                                      *altitudeAccuracyPtr ));
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


static void Handle_le_pos_sample_GetHorizontalSpeed
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
    le_pos_SampleRef_t positionSampleRef;
    if (!le_pack_UnpackReference( &_msgBufPtr, &_msgBufSize,
                                               &positionSampleRef ))
    {
        goto error_unpack;
    }

    // Define storage for output parameters
    uint32_t hSpeedBuffer;
    uint32_t *hSpeedPtr = &hSpeedBuffer;
    if (!(_requiredOutputs & (1u << 0)))
    {
        hSpeedPtr = NULL;
    }
    uint32_t hSpeedAccuracyBuffer;
    uint32_t *hSpeedAccuracyPtr = &hSpeedAccuracyBuffer;
    if (!(_requiredOutputs & (1u << 1)))
    {
        hSpeedAccuracyPtr = NULL;
    }

    // Call the function
    le_result_t _result;
    _result  = le_pos_sample_GetHorizontalSpeed ( 
        positionSampleRef, 
        hSpeedPtr, 
        hSpeedAccuracyPtr );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;
    _msgBufSize = _MAX_MSG_SIZE;

    // Pack the result first
    LE_ASSERT(le_pack_PackResult( &_msgBufPtr, &_msgBufSize, _result ));

    // Pack any "out" parameters
    if (hSpeedPtr)
    {
        LE_ASSERT(le_pack_PackUint32( &_msgBufPtr, &_msgBufSize,
                                                      *hSpeedPtr ));
    }
    if (hSpeedAccuracyPtr)
    {
        LE_ASSERT(le_pack_PackUint32( &_msgBufPtr, &_msgBufSize,
                                                      *hSpeedAccuracyPtr ));
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


static void Handle_le_pos_sample_GetVerticalSpeed
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
    le_pos_SampleRef_t positionSampleRef;
    if (!le_pack_UnpackReference( &_msgBufPtr, &_msgBufSize,
                                               &positionSampleRef ))
    {
        goto error_unpack;
    }

    // Define storage for output parameters
    int32_t vspeedBuffer;
    int32_t *vspeedPtr = &vspeedBuffer;
    if (!(_requiredOutputs & (1u << 0)))
    {
        vspeedPtr = NULL;
    }
    int32_t vspeedAccuracyBuffer;
    int32_t *vspeedAccuracyPtr = &vspeedAccuracyBuffer;
    if (!(_requiredOutputs & (1u << 1)))
    {
        vspeedAccuracyPtr = NULL;
    }

    // Call the function
    le_result_t _result;
    _result  = le_pos_sample_GetVerticalSpeed ( 
        positionSampleRef, 
        vspeedPtr, 
        vspeedAccuracyPtr );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;
    _msgBufSize = _MAX_MSG_SIZE;

    // Pack the result first
    LE_ASSERT(le_pack_PackResult( &_msgBufPtr, &_msgBufSize, _result ));

    // Pack any "out" parameters
    if (vspeedPtr)
    {
        LE_ASSERT(le_pack_PackInt32( &_msgBufPtr, &_msgBufSize,
                                                      *vspeedPtr ));
    }
    if (vspeedAccuracyPtr)
    {
        LE_ASSERT(le_pack_PackInt32( &_msgBufPtr, &_msgBufSize,
                                                      *vspeedAccuracyPtr ));
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


static void Handle_le_pos_sample_GetHeading
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
    le_pos_SampleRef_t positionSampleRef;
    if (!le_pack_UnpackReference( &_msgBufPtr, &_msgBufSize,
                                               &positionSampleRef ))
    {
        goto error_unpack;
    }

    // Define storage for output parameters
    uint32_t headingBuffer;
    uint32_t *headingPtr = &headingBuffer;
    if (!(_requiredOutputs & (1u << 0)))
    {
        headingPtr = NULL;
    }
    uint32_t headingAccuracyBuffer;
    uint32_t *headingAccuracyPtr = &headingAccuracyBuffer;
    if (!(_requiredOutputs & (1u << 1)))
    {
        headingAccuracyPtr = NULL;
    }

    // Call the function
    le_result_t _result;
    _result  = le_pos_sample_GetHeading ( 
        positionSampleRef, 
        headingPtr, 
        headingAccuracyPtr );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;
    _msgBufSize = _MAX_MSG_SIZE;

    // Pack the result first
    LE_ASSERT(le_pack_PackResult( &_msgBufPtr, &_msgBufSize, _result ));

    // Pack any "out" parameters
    if (headingPtr)
    {
        LE_ASSERT(le_pack_PackUint32( &_msgBufPtr, &_msgBufSize,
                                                      *headingPtr ));
    }
    if (headingAccuracyPtr)
    {
        LE_ASSERT(le_pack_PackUint32( &_msgBufPtr, &_msgBufSize,
                                                      *headingAccuracyPtr ));
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


static void Handle_le_pos_sample_GetDirection
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
    le_pos_SampleRef_t positionSampleRef;
    if (!le_pack_UnpackReference( &_msgBufPtr, &_msgBufSize,
                                               &positionSampleRef ))
    {
        goto error_unpack;
    }

    // Define storage for output parameters
    uint32_t directionBuffer;
    uint32_t *directionPtr = &directionBuffer;
    if (!(_requiredOutputs & (1u << 0)))
    {
        directionPtr = NULL;
    }
    uint32_t directionAccuracyBuffer;
    uint32_t *directionAccuracyPtr = &directionAccuracyBuffer;
    if (!(_requiredOutputs & (1u << 1)))
    {
        directionAccuracyPtr = NULL;
    }

    // Call the function
    le_result_t _result;
    _result  = le_pos_sample_GetDirection ( 
        positionSampleRef, 
        directionPtr, 
        directionAccuracyPtr );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;
    _msgBufSize = _MAX_MSG_SIZE;

    // Pack the result first
    LE_ASSERT(le_pack_PackResult( &_msgBufPtr, &_msgBufSize, _result ));

    // Pack any "out" parameters
    if (directionPtr)
    {
        LE_ASSERT(le_pack_PackUint32( &_msgBufPtr, &_msgBufSize,
                                                      *directionPtr ));
    }
    if (directionAccuracyPtr)
    {
        LE_ASSERT(le_pack_PackUint32( &_msgBufPtr, &_msgBufSize,
                                                      *directionAccuracyPtr ));
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


static void Handle_le_pos_sample_GetFixState
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
    le_pos_SampleRef_t positionSampleRef;
    if (!le_pack_UnpackReference( &_msgBufPtr, &_msgBufSize,
                                               &positionSampleRef ))
    {
        goto error_unpack;
    }

    // Define storage for output parameters
    le_pos_FixState_t stateBuffer;
    le_pos_FixState_t *statePtr = &stateBuffer;
    if (!(_requiredOutputs & (1u << 0)))
    {
        statePtr = NULL;
    }

    // Call the function
    le_result_t _result;
    _result  = le_pos_sample_GetFixState ( 
        positionSampleRef, 
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


static void Handle_le_pos_sample_Release
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
    le_pos_SampleRef_t positionSampleRef;
    if (!le_pack_UnpackReference( &_msgBufPtr, &_msgBufSize,
                                               &positionSampleRef ))
    {
        goto error_unpack;
    }

    // Define storage for output parameters

    // Call the function
    le_pos_sample_Release ( 
        positionSampleRef );

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


static void Handle_le_pos_SetAcquisitionRate
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
    uint32_t acquisitionRate;
    if (!le_pack_UnpackUint32( &_msgBufPtr, &_msgBufSize,
                                               &acquisitionRate ))
    {
        goto error_unpack;
    }

    // Define storage for output parameters

    // Call the function
    le_result_t _result;
    _result  = le_pos_SetAcquisitionRate ( 
        acquisitionRate );

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


static void Handle_le_pos_GetAcquisitionRate
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
    _result  = le_pos_GetAcquisitionRate (  );

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


static void Handle_le_pos_SetDistanceResolution
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
    le_pos_Resolution_t resolution;
    if (!le_pack_UnpackUint32( &_msgBufPtr, &_msgBufSize,
                                               &resolution ))
    {
        goto error_unpack;
    }

    // Define storage for output parameters

    // Call the function
    le_result_t _result;
    _result  = le_pos_SetDistanceResolution ( 
        resolution );

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
        case _MSGID_le_pos_AddMovementHandler : Handle_le_pos_AddMovementHandler(msgRef); break;
        case _MSGID_le_pos_RemoveMovementHandler : Handle_le_pos_RemoveMovementHandler(msgRef); break;
        case _MSGID_le_pos_Get2DLocation : Handle_le_pos_Get2DLocation(msgRef); break;
        case _MSGID_le_pos_Get3DLocation : Handle_le_pos_Get3DLocation(msgRef); break;
        case _MSGID_le_pos_GetTime : Handle_le_pos_GetTime(msgRef); break;
        case _MSGID_le_pos_GetDate : Handle_le_pos_GetDate(msgRef); break;
        case _MSGID_le_pos_GetMotion : Handle_le_pos_GetMotion(msgRef); break;
        case _MSGID_le_pos_GetHeading : Handle_le_pos_GetHeading(msgRef); break;
        case _MSGID_le_pos_GetDirection : Handle_le_pos_GetDirection(msgRef); break;
        case _MSGID_le_pos_GetFixState : Handle_le_pos_GetFixState(msgRef); break;
        case _MSGID_le_pos_sample_Get2DLocation : Handle_le_pos_sample_Get2DLocation(msgRef); break;
        case _MSGID_le_pos_sample_GetTime : Handle_le_pos_sample_GetTime(msgRef); break;
        case _MSGID_le_pos_sample_GetDate : Handle_le_pos_sample_GetDate(msgRef); break;
        case _MSGID_le_pos_sample_GetAltitude : Handle_le_pos_sample_GetAltitude(msgRef); break;
        case _MSGID_le_pos_sample_GetHorizontalSpeed : Handle_le_pos_sample_GetHorizontalSpeed(msgRef); break;
        case _MSGID_le_pos_sample_GetVerticalSpeed : Handle_le_pos_sample_GetVerticalSpeed(msgRef); break;
        case _MSGID_le_pos_sample_GetHeading : Handle_le_pos_sample_GetHeading(msgRef); break;
        case _MSGID_le_pos_sample_GetDirection : Handle_le_pos_sample_GetDirection(msgRef); break;
        case _MSGID_le_pos_sample_GetFixState : Handle_le_pos_sample_GetFixState(msgRef); break;
        case _MSGID_le_pos_sample_Release : Handle_le_pos_sample_Release(msgRef); break;
        case _MSGID_le_pos_SetAcquisitionRate : Handle_le_pos_SetAcquisitionRate(msgRef); break;
        case _MSGID_le_pos_GetAcquisitionRate : Handle_le_pos_GetAcquisitionRate(msgRef); break;
        case _MSGID_le_pos_SetDistanceResolution : Handle_le_pos_SetDistanceResolution(msgRef); break;

        default: LE_ERROR("Unknowm msg id = %i", msgPtr->id);
    }

    // Clear the client session ref associated with the current message, since the message
    // has now been processed.
    _ClientSessionRef = 0;
}