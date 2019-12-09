
/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */


#include "le_flash_messages.h"
#include "le_flash_server.h"


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
le_msg_ServiceRef_t le_flash_GetServiceRef
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
le_msg_SessionRef_t le_flash_GetClientSessionRef
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
void le_flash_AdvertiseService
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
    _ServerDataPool = le_mem_CreatePool("le_flash_ServerData", sizeof(_ServerData_t));

    // Create safe reference map for handler references.
    // The size of the map should be based on the number of handlers defined for the server.
    // Don't expect that to be more than 2-3, so use 3 as a reasonable guess.
    _HandlerRefMap = le_ref_CreateMap("le_flash_ServerHandlers", 3);

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


static void AsyncResponse_le_flash_AddBadImageDetectionHandler
(
    const char* LE_NONNULL imageName,
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
    _msgPtr->id = _MSGID_le_flash_AddBadImageDetectionHandler;
    _msgBufPtr = _msgPtr->buffer;
    _msgBufSize = _MAX_MSG_SIZE;

    // Always pack the client context pointer first
    LE_ASSERT(le_pack_PackReference( &_msgBufPtr, &_msgBufSize, serverDataPtr->contextPtr ))

    // Pack the input parameters
    
    LE_ASSERT(le_pack_PackString( &_msgBufPtr, &_msgBufSize,
                                  imageName, 33 ));

    // Send the async response to the client
    TRACE("Sending message to client session %p : %ti bytes sent",
          serverDataPtr->clientSessionRef,
          _msgBufPtr-_msgPtr->buffer);

    SendMsgToClient(_msgRef);
}


static void Handle_le_flash_AddBadImageDetectionHandler
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
    le_flash_BadImageDetectionHandlerRef_t _result;
    _result  = le_flash_AddBadImageDetectionHandler ( AsyncResponse_le_flash_AddBadImageDetectionHandler, 
        contextPtr );

    if (_result)
    {
        // Put the handler reference result and a pointer to the associated remove function
        // into the server data object.  This function pointer is needed in case the client
        // is closed and the handlers need to be removed.
        serverDataPtr->handlerRef = (le_event_HandlerRef_t)_result;
        serverDataPtr->removeHandlerFunc =
            (RemoveHandlerFunc_t)le_flash_RemoveBadImageDetectionHandler;

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


static void Handle_le_flash_RemoveBadImageDetectionHandler
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
    le_flash_BadImageDetectionHandlerRef_t handlerRef;
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
    handlerRef = (le_flash_BadImageDetectionHandlerRef_t)serverDataPtr->handlerRef;
    le_mem_Release(serverDataPtr);

    // Define storage for output parameters

    // Call the function
    le_flash_RemoveBadImageDetectionHandler ( 
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


static void Handle_le_flash_RequestAccess
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
    _result  = le_flash_RequestAccess (  );

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


static void Handle_le_flash_ReleaseAccess
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
    _result  = le_flash_ReleaseAccess (  );

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


static void Handle_le_flash_OpenMtd
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
    char partitionName[52];
    if (!le_pack_UnpackString( &_msgBufPtr, &_msgBufSize,
                               partitionName,
                               sizeof(partitionName),
                               51 ))
    {
        goto error_unpack;
    }
    le_flash_OpenMode_t mode;
    if (!le_pack_UnpackUint32( &_msgBufPtr, &_msgBufSize,
                                               &mode ))
    {
        goto error_unpack;
    }

    // Define storage for output parameters
    le_flash_PartitionRef_t partitionRefBuffer;
    le_flash_PartitionRef_t *partitionRefPtr = &partitionRefBuffer;
    if (!(_requiredOutputs & (1u << 0)))
    {
        partitionRefPtr = NULL;
    }

    // Call the function
    le_result_t _result;
    _result  = le_flash_OpenMtd ( 
        partitionName, 
        mode, 
        partitionRefPtr );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;
    _msgBufSize = _MAX_MSG_SIZE;

    // Pack the result first
    LE_ASSERT(le_pack_PackResult( &_msgBufPtr, &_msgBufSize, _result ));

    // Pack any "out" parameters
    if (partitionRefPtr)
    {
        LE_ASSERT(le_pack_PackReference( &_msgBufPtr, &_msgBufSize,
                                                      *partitionRefPtr ));
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


static void Handle_le_flash_OpenUbi
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
    char partitionName[52];
    if (!le_pack_UnpackString( &_msgBufPtr, &_msgBufSize,
                               partitionName,
                               sizeof(partitionName),
                               51 ))
    {
        goto error_unpack;
    }
    le_flash_OpenMode_t mode;
    if (!le_pack_UnpackUint32( &_msgBufPtr, &_msgBufSize,
                                               &mode ))
    {
        goto error_unpack;
    }

    // Define storage for output parameters
    le_flash_PartitionRef_t partitionRefBuffer;
    le_flash_PartitionRef_t *partitionRefPtr = &partitionRefBuffer;
    if (!(_requiredOutputs & (1u << 0)))
    {
        partitionRefPtr = NULL;
    }

    // Call the function
    le_result_t _result;
    _result  = le_flash_OpenUbi ( 
        partitionName, 
        mode, 
        partitionRefPtr );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;
    _msgBufSize = _MAX_MSG_SIZE;

    // Pack the result first
    LE_ASSERT(le_pack_PackResult( &_msgBufPtr, &_msgBufSize, _result ));

    // Pack any "out" parameters
    if (partitionRefPtr)
    {
        LE_ASSERT(le_pack_PackReference( &_msgBufPtr, &_msgBufSize,
                                                      *partitionRefPtr ));
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


static void Handle_le_flash_OpenUbiVolume
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
    le_flash_PartitionRef_t partitionRef;
    if (!le_pack_UnpackReference( &_msgBufPtr, &_msgBufSize,
                                               &partitionRef ))
    {
        goto error_unpack;
    }
    char volumeName[52];
    if (!le_pack_UnpackString( &_msgBufPtr, &_msgBufSize,
                               volumeName,
                               sizeof(volumeName),
                               51 ))
    {
        goto error_unpack;
    }
    int32_t volumeSize;
    if (!le_pack_UnpackInt32( &_msgBufPtr, &_msgBufSize,
                                               &volumeSize ))
    {
        goto error_unpack;
    }

    // Define storage for output parameters

    // Call the function
    le_result_t _result;
    _result  = le_flash_OpenUbiVolume ( 
        partitionRef, 
        volumeName, 
        volumeSize );

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


static void Handle_le_flash_CloseUbiVolume
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
    le_flash_PartitionRef_t partitionRef;
    if (!le_pack_UnpackReference( &_msgBufPtr, &_msgBufSize,
                                               &partitionRef ))
    {
        goto error_unpack;
    }

    // Define storage for output parameters

    // Call the function
    le_result_t _result;
    _result  = le_flash_CloseUbiVolume ( 
        partitionRef );

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


static void Handle_le_flash_Close
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
    le_flash_PartitionRef_t partitionRef;
    if (!le_pack_UnpackReference( &_msgBufPtr, &_msgBufSize,
                                               &partitionRef ))
    {
        goto error_unpack;
    }

    // Define storage for output parameters

    // Call the function
    le_result_t _result;
    _result  = le_flash_Close ( 
        partitionRef );

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


static void Handle_le_flash_EraseBlock
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
    le_flash_PartitionRef_t partitionRef;
    if (!le_pack_UnpackReference( &_msgBufPtr, &_msgBufSize,
                                               &partitionRef ))
    {
        goto error_unpack;
    }
    uint32_t blockIndex;
    if (!le_pack_UnpackUint32( &_msgBufPtr, &_msgBufSize,
                                               &blockIndex ))
    {
        goto error_unpack;
    }

    // Define storage for output parameters

    // Call the function
    le_result_t _result;
    _result  = le_flash_EraseBlock ( 
        partitionRef, 
        blockIndex );

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


static void Handle_le_flash_Read
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
    le_flash_PartitionRef_t partitionRef;
    if (!le_pack_UnpackReference( &_msgBufPtr, &_msgBufSize,
                                               &partitionRef ))
    {
        goto error_unpack;
    }
    uint32_t blockIndex;
    if (!le_pack_UnpackUint32( &_msgBufPtr, &_msgBufSize,
                                               &blockIndex ))
    {
        goto error_unpack;
    }
    size_t readDataSize;
    if (!le_pack_UnpackSize( &_msgBufPtr, &_msgBufSize,
                               &readDataSize ))
    {
        goto error_unpack;
    }
    if ( (readDataSize > 262144) )
    {
        LE_DEBUG("Adjusting readDataSize from %zu to 262144", readDataSize);
        readDataSize = 262144;
    }

    // Define storage for output parameters
    uint8_t readDataBuffer[262144];
    uint8_t *readDataPtr = readDataBuffer;
    size_t *readDataSizePtr = &readDataSize;
    if (!(_requiredOutputs & (1u << 0)))
    {
        readDataPtr = NULL;
    }

    // Call the function
    le_result_t _result;
    _result  = le_flash_Read ( 
        partitionRef, 
        blockIndex, 
        readDataPtr, 
        &readDataSize );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;
    _msgBufSize = _MAX_MSG_SIZE;

    // Pack the result first
    LE_ASSERT(le_pack_PackResult( &_msgBufPtr, &_msgBufSize, _result ));

    // Pack any "out" parameters
    if (readDataPtr)
    {
        bool readDataResult;
        LE_PACK_PACKARRAY( &_msgBufPtr, &_msgBufSize,
                           readDataPtr, (*readDataSizePtr),
                           262144, le_pack_PackUint8,
                           &readDataResult );
        LE_ASSERT(readDataResult);
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


static void Handle_le_flash_Write
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
    le_flash_PartitionRef_t partitionRef;
    if (!le_pack_UnpackReference( &_msgBufPtr, &_msgBufSize,
                                               &partitionRef ))
    {
        goto error_unpack;
    }
    uint32_t blockIndex;
    if (!le_pack_UnpackUint32( &_msgBufPtr, &_msgBufSize,
                                               &blockIndex ))
    {
        goto error_unpack;
    }
    size_t writeDataSize;
    uint8_t writeDataPtr[262144];
    bool writeDataResult;
    LE_PACK_UNPACKARRAY( &_msgBufPtr, &_msgBufSize,
                         writeDataPtr, &writeDataSize,
                         262144,
                         le_pack_UnpackUint8,
                         &writeDataResult );
    if (!writeDataResult)
    {
        goto error_unpack;
    }

    // Define storage for output parameters

    // Call the function
    le_result_t _result;
    _result  = le_flash_Write ( 
        partitionRef, 
        blockIndex, 
        writeDataPtr, 
        writeDataSize );

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


static void Handle_le_flash_GetBlockInformation
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
    le_flash_PartitionRef_t partitionRef;
    if (!le_pack_UnpackReference( &_msgBufPtr, &_msgBufSize,
                                               &partitionRef ))
    {
        goto error_unpack;
    }

    // Define storage for output parameters
    uint32_t badBlocksNumberBuffer;
    uint32_t *badBlocksNumberPtr = &badBlocksNumberBuffer;
    if (!(_requiredOutputs & (1u << 0)))
    {
        badBlocksNumberPtr = NULL;
    }
    uint32_t eraseBlocksNumberBuffer;
    uint32_t *eraseBlocksNumberPtr = &eraseBlocksNumberBuffer;
    if (!(_requiredOutputs & (1u << 1)))
    {
        eraseBlocksNumberPtr = NULL;
    }
    uint32_t eraseBlockSizeBuffer;
    uint32_t *eraseBlockSizePtr = &eraseBlockSizeBuffer;
    if (!(_requiredOutputs & (1u << 2)))
    {
        eraseBlockSizePtr = NULL;
    }
    uint32_t pageSizeBuffer;
    uint32_t *pageSizePtr = &pageSizeBuffer;
    if (!(_requiredOutputs & (1u << 3)))
    {
        pageSizePtr = NULL;
    }

    // Call the function
    le_result_t _result;
    _result  = le_flash_GetBlockInformation ( 
        partitionRef, 
        badBlocksNumberPtr, 
        eraseBlocksNumberPtr, 
        eraseBlockSizePtr, 
        pageSizePtr );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;
    _msgBufSize = _MAX_MSG_SIZE;

    // Pack the result first
    LE_ASSERT(le_pack_PackResult( &_msgBufPtr, &_msgBufSize, _result ));

    // Pack any "out" parameters
    if (badBlocksNumberPtr)
    {
        LE_ASSERT(le_pack_PackUint32( &_msgBufPtr, &_msgBufSize,
                                                      *badBlocksNumberPtr ));
    }
    if (eraseBlocksNumberPtr)
    {
        LE_ASSERT(le_pack_PackUint32( &_msgBufPtr, &_msgBufSize,
                                                      *eraseBlocksNumberPtr ));
    }
    if (eraseBlockSizePtr)
    {
        LE_ASSERT(le_pack_PackUint32( &_msgBufPtr, &_msgBufSize,
                                                      *eraseBlockSizePtr ));
    }
    if (pageSizePtr)
    {
        LE_ASSERT(le_pack_PackUint32( &_msgBufPtr, &_msgBufSize,
                                                      *pageSizePtr ));
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


static void Handle_le_flash_GetUbiVolumeInformation
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
    le_flash_PartitionRef_t partitionRef;
    if (!le_pack_UnpackReference( &_msgBufPtr, &_msgBufSize,
                                               &partitionRef ))
    {
        goto error_unpack;
    }

    // Define storage for output parameters
    uint32_t freeBlockNumberBuffer;
    uint32_t *freeBlockNumberPtr = &freeBlockNumberBuffer;
    if (!(_requiredOutputs & (1u << 0)))
    {
        freeBlockNumberPtr = NULL;
    }
    uint32_t allocatedBlockNumberBuffer;
    uint32_t *allocatedBlockNumberPtr = &allocatedBlockNumberBuffer;
    if (!(_requiredOutputs & (1u << 1)))
    {
        allocatedBlockNumberPtr = NULL;
    }
    uint32_t sizeInBytesBuffer;
    uint32_t *sizeInBytesPtr = &sizeInBytesBuffer;
    if (!(_requiredOutputs & (1u << 2)))
    {
        sizeInBytesPtr = NULL;
    }

    // Call the function
    le_result_t _result;
    _result  = le_flash_GetUbiVolumeInformation ( 
        partitionRef, 
        freeBlockNumberPtr, 
        allocatedBlockNumberPtr, 
        sizeInBytesPtr );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;
    _msgBufSize = _MAX_MSG_SIZE;

    // Pack the result first
    LE_ASSERT(le_pack_PackResult( &_msgBufPtr, &_msgBufSize, _result ));

    // Pack any "out" parameters
    if (freeBlockNumberPtr)
    {
        LE_ASSERT(le_pack_PackUint32( &_msgBufPtr, &_msgBufSize,
                                                      *freeBlockNumberPtr ));
    }
    if (allocatedBlockNumberPtr)
    {
        LE_ASSERT(le_pack_PackUint32( &_msgBufPtr, &_msgBufSize,
                                                      *allocatedBlockNumberPtr ));
    }
    if (sizeInBytesPtr)
    {
        LE_ASSERT(le_pack_PackUint32( &_msgBufPtr, &_msgBufSize,
                                                      *sizeInBytesPtr ));
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


static void Handle_le_flash_CreateUbi
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
    char partitionName[52];
    if (!le_pack_UnpackString( &_msgBufPtr, &_msgBufSize,
                               partitionName,
                               sizeof(partitionName),
                               51 ))
    {
        goto error_unpack;
    }
    bool isForcedCreate;
    if (!le_pack_UnpackBool( &_msgBufPtr, &_msgBufSize,
                                               &isForcedCreate ))
    {
        goto error_unpack;
    }

    // Define storage for output parameters
    le_flash_PartitionRef_t partitionRefBuffer;
    le_flash_PartitionRef_t *partitionRefPtr = &partitionRefBuffer;
    if (!(_requiredOutputs & (1u << 0)))
    {
        partitionRefPtr = NULL;
    }

    // Call the function
    le_result_t _result;
    _result  = le_flash_CreateUbi ( 
        partitionName, 
        isForcedCreate, 
        partitionRefPtr );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;
    _msgBufSize = _MAX_MSG_SIZE;

    // Pack the result first
    LE_ASSERT(le_pack_PackResult( &_msgBufPtr, &_msgBufSize, _result ));

    // Pack any "out" parameters
    if (partitionRefPtr)
    {
        LE_ASSERT(le_pack_PackReference( &_msgBufPtr, &_msgBufSize,
                                                      *partitionRefPtr ));
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


static void Handle_le_flash_CreateUbiVolume
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
    le_flash_PartitionRef_t partitionRef;
    if (!le_pack_UnpackReference( &_msgBufPtr, &_msgBufSize,
                                               &partitionRef ))
    {
        goto error_unpack;
    }
    bool isForcedCreate;
    if (!le_pack_UnpackBool( &_msgBufPtr, &_msgBufSize,
                                               &isForcedCreate ))
    {
        goto error_unpack;
    }
    uint32_t volumeID;
    if (!le_pack_UnpackUint32( &_msgBufPtr, &_msgBufSize,
                                               &volumeID ))
    {
        goto error_unpack;
    }
    le_flash_UbiVolumeType_t volumeType;
    if (!le_pack_UnpackUint32( &_msgBufPtr, &_msgBufSize,
                                               &volumeType ))
    {
        goto error_unpack;
    }
    char volumeName[52];
    if (!le_pack_UnpackString( &_msgBufPtr, &_msgBufSize,
                               volumeName,
                               sizeof(volumeName),
                               51 ))
    {
        goto error_unpack;
    }
    int32_t volumeSize;
    if (!le_pack_UnpackInt32( &_msgBufPtr, &_msgBufSize,
                                               &volumeSize ))
    {
        goto error_unpack;
    }

    // Define storage for output parameters

    // Call the function
    le_result_t _result;
    _result  = le_flash_CreateUbiVolume ( 
        partitionRef, 
        isForcedCreate, 
        volumeID, 
        volumeType, 
        volumeName, 
        volumeSize );

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


static void Handle_le_flash_DeleteUbiVolume
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
    le_flash_PartitionRef_t partitionRef;
    if (!le_pack_UnpackReference( &_msgBufPtr, &_msgBufSize,
                                               &partitionRef ))
    {
        goto error_unpack;
    }
    char volumeName[52];
    if (!le_pack_UnpackString( &_msgBufPtr, &_msgBufSize,
                               volumeName,
                               sizeof(volumeName),
                               51 ))
    {
        goto error_unpack;
    }

    // Define storage for output parameters

    // Call the function
    le_result_t _result;
    _result  = le_flash_DeleteUbiVolume ( 
        partitionRef, 
        volumeName );

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
        case _MSGID_le_flash_AddBadImageDetectionHandler : Handle_le_flash_AddBadImageDetectionHandler(msgRef); break;
        case _MSGID_le_flash_RemoveBadImageDetectionHandler : Handle_le_flash_RemoveBadImageDetectionHandler(msgRef); break;
        case _MSGID_le_flash_RequestAccess : Handle_le_flash_RequestAccess(msgRef); break;
        case _MSGID_le_flash_ReleaseAccess : Handle_le_flash_ReleaseAccess(msgRef); break;
        case _MSGID_le_flash_OpenMtd : Handle_le_flash_OpenMtd(msgRef); break;
        case _MSGID_le_flash_OpenUbi : Handle_le_flash_OpenUbi(msgRef); break;
        case _MSGID_le_flash_OpenUbiVolume : Handle_le_flash_OpenUbiVolume(msgRef); break;
        case _MSGID_le_flash_CloseUbiVolume : Handle_le_flash_CloseUbiVolume(msgRef); break;
        case _MSGID_le_flash_Close : Handle_le_flash_Close(msgRef); break;
        case _MSGID_le_flash_EraseBlock : Handle_le_flash_EraseBlock(msgRef); break;
        case _MSGID_le_flash_Read : Handle_le_flash_Read(msgRef); break;
        case _MSGID_le_flash_Write : Handle_le_flash_Write(msgRef); break;
        case _MSGID_le_flash_GetBlockInformation : Handle_le_flash_GetBlockInformation(msgRef); break;
        case _MSGID_le_flash_GetUbiVolumeInformation : Handle_le_flash_GetUbiVolumeInformation(msgRef); break;
        case _MSGID_le_flash_CreateUbi : Handle_le_flash_CreateUbi(msgRef); break;
        case _MSGID_le_flash_CreateUbiVolume : Handle_le_flash_CreateUbiVolume(msgRef); break;
        case _MSGID_le_flash_DeleteUbiVolume : Handle_le_flash_DeleteUbiVolume(msgRef); break;

        default: LE_ERROR("Unknowm msg id = %i", msgPtr->id);
    }

    // Clear the client session ref associated with the current message, since the message
    // has now been processed.
    _ClientSessionRef = 0;
}