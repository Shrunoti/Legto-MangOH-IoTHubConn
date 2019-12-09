
/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */


#include "le_ips_messages.h"
#include "le_ips_server.h"


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
le_msg_ServiceRef_t le_ips_GetServiceRef
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
le_msg_SessionRef_t le_ips_GetClientSessionRef
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
void le_ips_AdvertiseService
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
    _ServerDataPool = le_mem_CreatePool("le_ips_ServerData", sizeof(_ServerData_t));

    // Create safe reference map for handler references.
    // The size of the map should be based on the number of handlers defined for the server.
    // Don't expect that to be more than 2-3, so use 3 as a reasonable guess.
    _HandlerRefMap = le_ref_CreateMap("le_ips_ServerHandlers", 3);

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


static void AsyncResponse_le_ips_AddThresholdEventHandler
(
    le_ips_ThresholdStatus_t event,
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
    _msgPtr->id = _MSGID_le_ips_AddThresholdEventHandler;
    _msgBufPtr = _msgPtr->buffer;
    _msgBufSize = _MAX_MSG_SIZE;

    // Always pack the client context pointer first
    LE_ASSERT(le_pack_PackReference( &_msgBufPtr, &_msgBufSize, serverDataPtr->contextPtr ))

    // Pack the input parameters
    
    LE_ASSERT(le_pack_PackUint32( &_msgBufPtr, &_msgBufSize,
                                                  event ));

    // Send the async response to the client
    TRACE("Sending message to client session %p : %ti bytes sent",
          serverDataPtr->clientSessionRef,
          _msgBufPtr-_msgPtr->buffer);

    SendMsgToClient(_msgRef);
}


static void Handle_le_ips_AddThresholdEventHandler
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
    le_ips_ThresholdEventHandlerRef_t _result;
    _result  = le_ips_AddThresholdEventHandler ( AsyncResponse_le_ips_AddThresholdEventHandler, 
        contextPtr );

    if (_result)
    {
        // Put the handler reference result and a pointer to the associated remove function
        // into the server data object.  This function pointer is needed in case the client
        // is closed and the handlers need to be removed.
        serverDataPtr->handlerRef = (le_event_HandlerRef_t)_result;
        serverDataPtr->removeHandlerFunc =
            (RemoveHandlerFunc_t)le_ips_RemoveThresholdEventHandler;

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


static void Handle_le_ips_RemoveThresholdEventHandler
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
    le_ips_ThresholdEventHandlerRef_t handlerRef;
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
    handlerRef = (le_ips_ThresholdEventHandlerRef_t)serverDataPtr->handlerRef;
    le_mem_Release(serverDataPtr);

    // Define storage for output parameters

    // Call the function
    le_ips_RemoveThresholdEventHandler ( 
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


static void Handle_le_ips_GetInputVoltage
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
    uint32_t inputVoltageBuffer;
    uint32_t *inputVoltagePtr = &inputVoltageBuffer;
    if (!(_requiredOutputs & (1u << 0)))
    {
        inputVoltagePtr = NULL;
    }

    // Call the function
    le_result_t _result;
    _result  = le_ips_GetInputVoltage ( 
        inputVoltagePtr );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;
    _msgBufSize = _MAX_MSG_SIZE;

    // Pack the result first
    LE_ASSERT(le_pack_PackResult( &_msgBufPtr, &_msgBufSize, _result ));

    // Pack any "out" parameters
    if (inputVoltagePtr)
    {
        LE_ASSERT(le_pack_PackUint32( &_msgBufPtr, &_msgBufSize,
                                                      *inputVoltagePtr ));
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


static void Handle_le_ips_SetVoltageThresholds
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
    uint16_t criticalVolt;
    if (!le_pack_UnpackUint16( &_msgBufPtr, &_msgBufSize,
                                               &criticalVolt ))
    {
        goto error_unpack;
    }
    uint16_t warningVolt;
    if (!le_pack_UnpackUint16( &_msgBufPtr, &_msgBufSize,
                                               &warningVolt ))
    {
        goto error_unpack;
    }
    uint16_t normalVolt;
    if (!le_pack_UnpackUint16( &_msgBufPtr, &_msgBufSize,
                                               &normalVolt ))
    {
        goto error_unpack;
    }
    uint16_t hiCriticalVolt;
    if (!le_pack_UnpackUint16( &_msgBufPtr, &_msgBufSize,
                                               &hiCriticalVolt ))
    {
        goto error_unpack;
    }

    // Define storage for output parameters

    // Call the function
    le_result_t _result;
    _result  = le_ips_SetVoltageThresholds ( 
        criticalVolt, 
        warningVolt, 
        normalVolt, 
        hiCriticalVolt );

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


static void Handle_le_ips_GetVoltageThresholds
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
    uint16_t criticalVoltBuffer;
    uint16_t *criticalVoltPtr = &criticalVoltBuffer;
    if (!(_requiredOutputs & (1u << 0)))
    {
        criticalVoltPtr = NULL;
    }
    uint16_t warningVoltBuffer;
    uint16_t *warningVoltPtr = &warningVoltBuffer;
    if (!(_requiredOutputs & (1u << 1)))
    {
        warningVoltPtr = NULL;
    }
    uint16_t normalVoltBuffer;
    uint16_t *normalVoltPtr = &normalVoltBuffer;
    if (!(_requiredOutputs & (1u << 2)))
    {
        normalVoltPtr = NULL;
    }
    uint16_t hiCriticalVoltBuffer;
    uint16_t *hiCriticalVoltPtr = &hiCriticalVoltBuffer;
    if (!(_requiredOutputs & (1u << 3)))
    {
        hiCriticalVoltPtr = NULL;
    }

    // Call the function
    le_result_t _result;
    _result  = le_ips_GetVoltageThresholds ( 
        criticalVoltPtr, 
        warningVoltPtr, 
        normalVoltPtr, 
        hiCriticalVoltPtr );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;
    _msgBufSize = _MAX_MSG_SIZE;

    // Pack the result first
    LE_ASSERT(le_pack_PackResult( &_msgBufPtr, &_msgBufSize, _result ));

    // Pack any "out" parameters
    if (criticalVoltPtr)
    {
        LE_ASSERT(le_pack_PackUint16( &_msgBufPtr, &_msgBufSize,
                                                      *criticalVoltPtr ));
    }
    if (warningVoltPtr)
    {
        LE_ASSERT(le_pack_PackUint16( &_msgBufPtr, &_msgBufSize,
                                                      *warningVoltPtr ));
    }
    if (normalVoltPtr)
    {
        LE_ASSERT(le_pack_PackUint16( &_msgBufPtr, &_msgBufSize,
                                                      *normalVoltPtr ));
    }
    if (hiCriticalVoltPtr)
    {
        LE_ASSERT(le_pack_PackUint16( &_msgBufPtr, &_msgBufSize,
                                                      *hiCriticalVoltPtr ));
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


static void Handle_le_ips_GetPowerSource
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
    le_ips_PowerSource_t powerSourceBuffer;
    le_ips_PowerSource_t *powerSourcePtr = &powerSourceBuffer;
    if (!(_requiredOutputs & (1u << 0)))
    {
        powerSourcePtr = NULL;
    }

    // Call the function
    le_result_t _result;
    _result  = le_ips_GetPowerSource ( 
        powerSourcePtr );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;
    _msgBufSize = _MAX_MSG_SIZE;

    // Pack the result first
    LE_ASSERT(le_pack_PackResult( &_msgBufPtr, &_msgBufSize, _result ));

    // Pack any "out" parameters
    if (powerSourcePtr)
    {
        LE_ASSERT(le_pack_PackUint32( &_msgBufPtr, &_msgBufSize,
                                                      *powerSourcePtr ));
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


static void Handle_le_ips_GetBatteryLevel
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
    uint8_t batteryLevelBuffer;
    uint8_t *batteryLevelPtr = &batteryLevelBuffer;
    if (!(_requiredOutputs & (1u << 0)))
    {
        batteryLevelPtr = NULL;
    }

    // Call the function
    le_result_t _result;
    _result  = le_ips_GetBatteryLevel ( 
        batteryLevelPtr );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;
    _msgBufSize = _MAX_MSG_SIZE;

    // Pack the result first
    LE_ASSERT(le_pack_PackResult( &_msgBufPtr, &_msgBufSize, _result ));

    // Pack any "out" parameters
    if (batteryLevelPtr)
    {
        LE_ASSERT(le_pack_PackUint8( &_msgBufPtr, &_msgBufSize,
                                                      *batteryLevelPtr ));
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


static void Handle_le_ips_SetBatteryLevel
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
    uint8_t batteryLevel;
    if (!le_pack_UnpackUint8( &_msgBufPtr, &_msgBufSize,
                                               &batteryLevel ))
    {
        goto error_unpack;
    }

    // Define storage for output parameters

    // Call the function
    le_result_t _result;
    _result  = le_ips_SetBatteryLevel ( 
        batteryLevel );

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
        case _MSGID_le_ips_AddThresholdEventHandler : Handle_le_ips_AddThresholdEventHandler(msgRef); break;
        case _MSGID_le_ips_RemoveThresholdEventHandler : Handle_le_ips_RemoveThresholdEventHandler(msgRef); break;
        case _MSGID_le_ips_GetInputVoltage : Handle_le_ips_GetInputVoltage(msgRef); break;
        case _MSGID_le_ips_SetVoltageThresholds : Handle_le_ips_SetVoltageThresholds(msgRef); break;
        case _MSGID_le_ips_GetVoltageThresholds : Handle_le_ips_GetVoltageThresholds(msgRef); break;
        case _MSGID_le_ips_GetPowerSource : Handle_le_ips_GetPowerSource(msgRef); break;
        case _MSGID_le_ips_GetBatteryLevel : Handle_le_ips_GetBatteryLevel(msgRef); break;
        case _MSGID_le_ips_SetBatteryLevel : Handle_le_ips_SetBatteryLevel(msgRef); break;

        default: LE_ERROR("Unknowm msg id = %i", msgPtr->id);
    }

    // Clear the client session ref associated with the current message, since the message
    // has now been processed.
    _ClientSessionRef = 0;
}