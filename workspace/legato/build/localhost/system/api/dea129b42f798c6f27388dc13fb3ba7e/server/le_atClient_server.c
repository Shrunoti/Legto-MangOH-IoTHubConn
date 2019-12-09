
/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */


#include "le_atClient_messages.h"
#include "le_atClient_server.h"


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
le_msg_ServiceRef_t le_atClient_GetServiceRef
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
le_msg_SessionRef_t le_atClient_GetClientSessionRef
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
void le_atClient_AdvertiseService
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
    _ServerDataPool = le_mem_CreatePool("le_atClient_ServerData", sizeof(_ServerData_t));

    // Create safe reference map for handler references.
    // The size of the map should be based on the number of handlers defined for the server.
    // Don't expect that to be more than 2-3, so use 3 as a reasonable guess.
    _HandlerRefMap = le_ref_CreateMap("le_atClient_ServerHandlers", 3);

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


static void Handle_le_atClient_Start
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
    le_atClient_DeviceRef_t _result;
    _result  = le_atClient_Start ( 
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


static void Handle_le_atClient_Stop
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
    le_atClient_DeviceRef_t device;
    if (!le_pack_UnpackReference( &_msgBufPtr, &_msgBufSize,
                                               &device ))
    {
        goto error_unpack;
    }

    // Define storage for output parameters

    // Call the function
    le_result_t _result;
    _result  = le_atClient_Stop ( 
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


static void Handle_le_atClient_Create
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
    le_atClient_CmdRef_t _result;
    _result  = le_atClient_Create (  );

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


static void Handle_le_atClient_Delete
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
    le_atClient_CmdRef_t cmdRef;
    if (!le_pack_UnpackReference( &_msgBufPtr, &_msgBufSize,
                                               &cmdRef ))
    {
        goto error_unpack;
    }

    // Define storage for output parameters

    // Call the function
    le_result_t _result;
    _result  = le_atClient_Delete ( 
        cmdRef );

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


static void Handle_le_atClient_SetCommand
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
    le_atClient_CmdRef_t cmdRef;
    if (!le_pack_UnpackReference( &_msgBufPtr, &_msgBufSize,
                                               &cmdRef ))
    {
        goto error_unpack;
    }
    char command[512];
    if (!le_pack_UnpackString( &_msgBufPtr, &_msgBufSize,
                               command,
                               sizeof(command),
                               511 ))
    {
        goto error_unpack;
    }

    // Define storage for output parameters

    // Call the function
    le_result_t _result;
    _result  = le_atClient_SetCommand ( 
        cmdRef, 
        command );

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


static void Handle_le_atClient_SetIntermediateResponse
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
    le_atClient_CmdRef_t cmdRef;
    if (!le_pack_UnpackReference( &_msgBufPtr, &_msgBufSize,
                                               &cmdRef ))
    {
        goto error_unpack;
    }
    char intermediate[513];
    if (!le_pack_UnpackString( &_msgBufPtr, &_msgBufSize,
                               intermediate,
                               sizeof(intermediate),
                               512 ))
    {
        goto error_unpack;
    }

    // Define storage for output parameters

    // Call the function
    le_result_t _result;
    _result  = le_atClient_SetIntermediateResponse ( 
        cmdRef, 
        intermediate );

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


static void Handle_le_atClient_SetFinalResponse
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
    le_atClient_CmdRef_t cmdRef;
    if (!le_pack_UnpackReference( &_msgBufPtr, &_msgBufSize,
                                               &cmdRef ))
    {
        goto error_unpack;
    }
    char response[513];
    if (!le_pack_UnpackString( &_msgBufPtr, &_msgBufSize,
                               response,
                               sizeof(response),
                               512 ))
    {
        goto error_unpack;
    }

    // Define storage for output parameters

    // Call the function
    le_result_t _result;
    _result  = le_atClient_SetFinalResponse ( 
        cmdRef, 
        response );

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


static void Handle_le_atClient_SetText
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
    le_atClient_CmdRef_t cmdRef;
    if (!le_pack_UnpackReference( &_msgBufPtr, &_msgBufSize,
                                               &cmdRef ))
    {
        goto error_unpack;
    }
    char text[4096];
    if (!le_pack_UnpackString( &_msgBufPtr, &_msgBufSize,
                               text,
                               sizeof(text),
                               4095 ))
    {
        goto error_unpack;
    }

    // Define storage for output parameters

    // Call the function
    le_result_t _result;
    _result  = le_atClient_SetText ( 
        cmdRef, 
        text );

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


static void Handle_le_atClient_SetTimeout
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
    le_atClient_CmdRef_t cmdRef;
    if (!le_pack_UnpackReference( &_msgBufPtr, &_msgBufSize,
                                               &cmdRef ))
    {
        goto error_unpack;
    }
    uint32_t timer;
    if (!le_pack_UnpackUint32( &_msgBufPtr, &_msgBufSize,
                                               &timer ))
    {
        goto error_unpack;
    }

    // Define storage for output parameters

    // Call the function
    le_result_t _result;
    _result  = le_atClient_SetTimeout ( 
        cmdRef, 
        timer );

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


static void Handle_le_atClient_SetDevice
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
    le_atClient_CmdRef_t cmdRef;
    if (!le_pack_UnpackReference( &_msgBufPtr, &_msgBufSize,
                                               &cmdRef ))
    {
        goto error_unpack;
    }
    le_atClient_DeviceRef_t devRef;
    if (!le_pack_UnpackReference( &_msgBufPtr, &_msgBufSize,
                                               &devRef ))
    {
        goto error_unpack;
    }

    // Define storage for output parameters

    // Call the function
    le_result_t _result;
    _result  = le_atClient_SetDevice ( 
        cmdRef, 
        devRef );

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


static void Handle_le_atClient_Send
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
    le_atClient_CmdRef_t cmdRef;
    if (!le_pack_UnpackReference( &_msgBufPtr, &_msgBufSize,
                                               &cmdRef ))
    {
        goto error_unpack;
    }

    // Define storage for output parameters

    // Call the function
    le_result_t _result;
    _result  = le_atClient_Send ( 
        cmdRef );

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


static void Handle_le_atClient_GetFirstIntermediateResponse
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
    le_atClient_CmdRef_t cmdRef;
    if (!le_pack_UnpackReference( &_msgBufPtr, &_msgBufSize,
                                               &cmdRef ))
    {
        goto error_unpack;
    }
    size_t intermediateRspSize;
    if (!le_pack_UnpackSize( &_msgBufPtr, &_msgBufSize,
                               &intermediateRspSize ))
    {
        goto error_unpack;
    }
    if ( (intermediateRspSize > 512) &&
         (intermediateRspSize < UINT32_MAX) )
    {
        LE_DEBUG("Adjusting intermediateRspSize from %zu to 512", intermediateRspSize);
        intermediateRspSize = 512;
    }
    if (intermediateRspSize >= UINT32_MAX)
    {
        intermediateRspSize = 0;
    }
    else
    {
        intermediateRspSize++;
    }

    // Define storage for output parameters
    char intermediateRspBuffer[513];
    char *intermediateRsp = intermediateRspBuffer;
    intermediateRsp[0] = 0;
    if (!(_requiredOutputs & (1u << 0)))
    {
        intermediateRsp = NULL;
        intermediateRspSize = 0;
    }

    // Call the function
    le_result_t _result;
    _result  = le_atClient_GetFirstIntermediateResponse ( 
        cmdRef, 
        intermediateRsp, 
        intermediateRspSize );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;
    _msgBufSize = _MAX_MSG_SIZE;

    // Pack the result first
    LE_ASSERT(le_pack_PackResult( &_msgBufPtr, &_msgBufSize, _result ));

    // Pack any "out" parameters
    if (intermediateRsp)
    {
        LE_ASSERT(le_pack_PackString( &_msgBufPtr, &_msgBufSize,
                                      intermediateRsp, 512 ));
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


static void Handle_le_atClient_GetNextIntermediateResponse
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
    le_atClient_CmdRef_t cmdRef;
    if (!le_pack_UnpackReference( &_msgBufPtr, &_msgBufSize,
                                               &cmdRef ))
    {
        goto error_unpack;
    }
    size_t intermediateRspSize;
    if (!le_pack_UnpackSize( &_msgBufPtr, &_msgBufSize,
                               &intermediateRspSize ))
    {
        goto error_unpack;
    }
    if ( (intermediateRspSize > 512) &&
         (intermediateRspSize < UINT32_MAX) )
    {
        LE_DEBUG("Adjusting intermediateRspSize from %zu to 512", intermediateRspSize);
        intermediateRspSize = 512;
    }
    if (intermediateRspSize >= UINT32_MAX)
    {
        intermediateRspSize = 0;
    }
    else
    {
        intermediateRspSize++;
    }

    // Define storage for output parameters
    char intermediateRspBuffer[513];
    char *intermediateRsp = intermediateRspBuffer;
    intermediateRsp[0] = 0;
    if (!(_requiredOutputs & (1u << 0)))
    {
        intermediateRsp = NULL;
        intermediateRspSize = 0;
    }

    // Call the function
    le_result_t _result;
    _result  = le_atClient_GetNextIntermediateResponse ( 
        cmdRef, 
        intermediateRsp, 
        intermediateRspSize );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;
    _msgBufSize = _MAX_MSG_SIZE;

    // Pack the result first
    LE_ASSERT(le_pack_PackResult( &_msgBufPtr, &_msgBufSize, _result ));

    // Pack any "out" parameters
    if (intermediateRsp)
    {
        LE_ASSERT(le_pack_PackString( &_msgBufPtr, &_msgBufSize,
                                      intermediateRsp, 512 ));
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


static void Handle_le_atClient_GetFinalResponse
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
    le_atClient_CmdRef_t cmdRef;
    if (!le_pack_UnpackReference( &_msgBufPtr, &_msgBufSize,
                                               &cmdRef ))
    {
        goto error_unpack;
    }
    size_t finalRspSize;
    if (!le_pack_UnpackSize( &_msgBufPtr, &_msgBufSize,
                               &finalRspSize ))
    {
        goto error_unpack;
    }
    if ( (finalRspSize > 512) &&
         (finalRspSize < UINT32_MAX) )
    {
        LE_DEBUG("Adjusting finalRspSize from %zu to 512", finalRspSize);
        finalRspSize = 512;
    }
    if (finalRspSize >= UINT32_MAX)
    {
        finalRspSize = 0;
    }
    else
    {
        finalRspSize++;
    }

    // Define storage for output parameters
    char finalRspBuffer[513];
    char *finalRsp = finalRspBuffer;
    finalRsp[0] = 0;
    if (!(_requiredOutputs & (1u << 0)))
    {
        finalRsp = NULL;
        finalRspSize = 0;
    }

    // Call the function
    le_result_t _result;
    _result  = le_atClient_GetFinalResponse ( 
        cmdRef, 
        finalRsp, 
        finalRspSize );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;
    _msgBufSize = _MAX_MSG_SIZE;

    // Pack the result first
    LE_ASSERT(le_pack_PackResult( &_msgBufPtr, &_msgBufSize, _result ));

    // Pack any "out" parameters
    if (finalRsp)
    {
        LE_ASSERT(le_pack_PackString( &_msgBufPtr, &_msgBufSize,
                                      finalRsp, 512 ));
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


static void Handle_le_atClient_SetCommandAndSend
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
    le_atClient_DeviceRef_t devRef;
    if (!le_pack_UnpackReference( &_msgBufPtr, &_msgBufSize,
                                               &devRef ))
    {
        goto error_unpack;
    }
    char command[512];
    if (!le_pack_UnpackString( &_msgBufPtr, &_msgBufSize,
                               command,
                               sizeof(command),
                               511 ))
    {
        goto error_unpack;
    }
    char interResp[513];
    if (!le_pack_UnpackString( &_msgBufPtr, &_msgBufSize,
                               interResp,
                               sizeof(interResp),
                               512 ))
    {
        goto error_unpack;
    }
    char finalResp[513];
    if (!le_pack_UnpackString( &_msgBufPtr, &_msgBufSize,
                               finalResp,
                               sizeof(finalResp),
                               512 ))
    {
        goto error_unpack;
    }
    uint32_t timeout;
    if (!le_pack_UnpackUint32( &_msgBufPtr, &_msgBufSize,
                                               &timeout ))
    {
        goto error_unpack;
    }

    // Define storage for output parameters
    le_atClient_CmdRef_t cmdRefBuffer;
    le_atClient_CmdRef_t *cmdRefPtr = &cmdRefBuffer;
    if (!(_requiredOutputs & (1u << 0)))
    {
        cmdRefPtr = NULL;
    }

    // Call the function
    le_result_t _result;
    _result  = le_atClient_SetCommandAndSend ( 
        cmdRefPtr, 
        devRef, 
        command, 
        interResp, 
        finalResp, 
        timeout );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;
    _msgBufSize = _MAX_MSG_SIZE;

    // Pack the result first
    LE_ASSERT(le_pack_PackResult( &_msgBufPtr, &_msgBufSize, _result ));

    // Pack any "out" parameters
    if (cmdRefPtr)
    {
        LE_ASSERT(le_pack_PackReference( &_msgBufPtr, &_msgBufSize,
                                                      *cmdRefPtr ));
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


static void AsyncResponse_le_atClient_AddUnsolicitedResponseHandler
(
    const char* LE_NONNULL unsolicitedRsp,
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
    _msgPtr->id = _MSGID_le_atClient_AddUnsolicitedResponseHandler;
    _msgBufPtr = _msgPtr->buffer;
    _msgBufSize = _MAX_MSG_SIZE;

    // Always pack the client context pointer first
    LE_ASSERT(le_pack_PackReference( &_msgBufPtr, &_msgBufSize, serverDataPtr->contextPtr ))

    // Pack the input parameters
    
    LE_ASSERT(le_pack_PackString( &_msgBufPtr, &_msgBufSize,
                                  unsolicitedRsp, 256 ));

    // Send the async response to the client
    TRACE("Sending message to client session %p : %ti bytes sent",
          serverDataPtr->clientSessionRef,
          _msgBufPtr-_msgPtr->buffer);

    SendMsgToClient(_msgRef);
}


static void Handle_le_atClient_AddUnsolicitedResponseHandler
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
    char unsolRsp[513];
    if (!le_pack_UnpackString( &_msgBufPtr, &_msgBufSize,
                               unsolRsp,
                               sizeof(unsolRsp),
                               512 ))
    {
        goto error_unpack;
    }
    le_atClient_DeviceRef_t devRef;
    if (!le_pack_UnpackReference( &_msgBufPtr, &_msgBufSize,
                                               &devRef ))
    {
        goto error_unpack;
    }
    void *contextPtr;
    if (!le_pack_UnpackReference( &_msgBufPtr, &_msgBufSize, &contextPtr ))
    {
        goto error_unpack;
    }
    uint32_t lineCount;
    if (!le_pack_UnpackUint32( &_msgBufPtr, &_msgBufSize,
                                               &lineCount ))
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
    le_atClient_UnsolicitedResponseHandlerRef_t _result;
    _result  = le_atClient_AddUnsolicitedResponseHandler ( 
        unsolRsp, 
        devRef, AsyncResponse_le_atClient_AddUnsolicitedResponseHandler, 
        contextPtr, 
        lineCount );

    if (_result)
    {
        // Put the handler reference result and a pointer to the associated remove function
        // into the server data object.  This function pointer is needed in case the client
        // is closed and the handlers need to be removed.
        serverDataPtr->handlerRef = (le_event_HandlerRef_t)_result;
        serverDataPtr->removeHandlerFunc =
            (RemoveHandlerFunc_t)le_atClient_RemoveUnsolicitedResponseHandler;

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


static void Handle_le_atClient_RemoveUnsolicitedResponseHandler
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
    le_atClient_UnsolicitedResponseHandlerRef_t handlerRef;
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
    handlerRef = (le_atClient_UnsolicitedResponseHandlerRef_t)serverDataPtr->handlerRef;
    le_mem_Release(serverDataPtr);

    // Define storage for output parameters

    // Call the function
    le_atClient_RemoveUnsolicitedResponseHandler ( 
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
        case _MSGID_le_atClient_Start : Handle_le_atClient_Start(msgRef); break;
        case _MSGID_le_atClient_Stop : Handle_le_atClient_Stop(msgRef); break;
        case _MSGID_le_atClient_Create : Handle_le_atClient_Create(msgRef); break;
        case _MSGID_le_atClient_Delete : Handle_le_atClient_Delete(msgRef); break;
        case _MSGID_le_atClient_SetCommand : Handle_le_atClient_SetCommand(msgRef); break;
        case _MSGID_le_atClient_SetIntermediateResponse : Handle_le_atClient_SetIntermediateResponse(msgRef); break;
        case _MSGID_le_atClient_SetFinalResponse : Handle_le_atClient_SetFinalResponse(msgRef); break;
        case _MSGID_le_atClient_SetText : Handle_le_atClient_SetText(msgRef); break;
        case _MSGID_le_atClient_SetTimeout : Handle_le_atClient_SetTimeout(msgRef); break;
        case _MSGID_le_atClient_SetDevice : Handle_le_atClient_SetDevice(msgRef); break;
        case _MSGID_le_atClient_Send : Handle_le_atClient_Send(msgRef); break;
        case _MSGID_le_atClient_GetFirstIntermediateResponse : Handle_le_atClient_GetFirstIntermediateResponse(msgRef); break;
        case _MSGID_le_atClient_GetNextIntermediateResponse : Handle_le_atClient_GetNextIntermediateResponse(msgRef); break;
        case _MSGID_le_atClient_GetFinalResponse : Handle_le_atClient_GetFinalResponse(msgRef); break;
        case _MSGID_le_atClient_SetCommandAndSend : Handle_le_atClient_SetCommandAndSend(msgRef); break;
        case _MSGID_le_atClient_AddUnsolicitedResponseHandler : Handle_le_atClient_AddUnsolicitedResponseHandler(msgRef); break;
        case _MSGID_le_atClient_RemoveUnsolicitedResponseHandler : Handle_le_atClient_RemoveUnsolicitedResponseHandler(msgRef); break;

        default: LE_ERROR("Unknowm msg id = %i", msgPtr->id);
    }

    // Clear the client session ref associated with the current message, since the message
    // has now been processed.
    _ClientSessionRef = 0;
}