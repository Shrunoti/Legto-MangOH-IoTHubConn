
/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */


#include "le_gnss_messages.h"
#include "le_gnss_server.h"


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
le_msg_ServiceRef_t le_gnss_GetServiceRef
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
le_msg_SessionRef_t le_gnss_GetClientSessionRef
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
void le_gnss_AdvertiseService
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
    _ServerDataPool = le_mem_CreatePool("le_gnss_ServerData", sizeof(_ServerData_t));

    // Create safe reference map for handler references.
    // The size of the map should be based on the number of handlers defined for the server.
    // Don't expect that to be more than 2-3, so use 3 as a reasonable guess.
    _HandlerRefMap = le_ref_CreateMap("le_gnss_ServerHandlers", 3);

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


static void Handle_le_gnss_SetConstellation
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
    le_gnss_ConstellationBitMask_t constellationMask;
    if (!le_pack_UnpackUint32( &_msgBufPtr, &_msgBufSize,
                                               &constellationMask ))
    {
        goto error_unpack;
    }

    // Define storage for output parameters

    // Call the function
    le_result_t _result;
    _result  = le_gnss_SetConstellation ( 
        constellationMask );

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


static void Handle_le_gnss_GetConstellation
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
    le_gnss_ConstellationBitMask_t constellationMaskBuffer;
    le_gnss_ConstellationBitMask_t *constellationMaskPtr = &constellationMaskBuffer;
    if (!(_requiredOutputs & (1u << 0)))
    {
        constellationMaskPtr = NULL;
    }

    // Call the function
    le_result_t _result;
    _result  = le_gnss_GetConstellation ( 
        constellationMaskPtr );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;
    _msgBufSize = _MAX_MSG_SIZE;

    // Pack the result first
    LE_ASSERT(le_pack_PackResult( &_msgBufPtr, &_msgBufSize, _result ));

    // Pack any "out" parameters
    if (constellationMaskPtr)
    {
        LE_ASSERT(le_pack_PackUint32( &_msgBufPtr, &_msgBufSize,
                                                      *constellationMaskPtr ));
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


static void Handle_le_gnss_SetConstellationArea
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
    le_gnss_Constellation_t satConstellation;
    if (!le_pack_UnpackUint32( &_msgBufPtr, &_msgBufSize,
                                               &satConstellation ))
    {
        goto error_unpack;
    }
    le_gnss_ConstellationArea_t constellationArea;
    if (!le_pack_UnpackUint32( &_msgBufPtr, &_msgBufSize,
                                               &constellationArea ))
    {
        goto error_unpack;
    }

    // Define storage for output parameters

    // Call the function
    le_result_t _result;
    _result  = le_gnss_SetConstellationArea ( 
        satConstellation, 
        constellationArea );

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


static void Handle_le_gnss_GetConstellationArea
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
    le_gnss_Constellation_t satConstellation;
    if (!le_pack_UnpackUint32( &_msgBufPtr, &_msgBufSize,
                                               &satConstellation ))
    {
        goto error_unpack;
    }

    // Define storage for output parameters
    le_gnss_ConstellationArea_t constellationAreaBuffer;
    le_gnss_ConstellationArea_t *constellationAreaPtr = &constellationAreaBuffer;
    if (!(_requiredOutputs & (1u << 0)))
    {
        constellationAreaPtr = NULL;
    }

    // Call the function
    le_result_t _result;
    _result  = le_gnss_GetConstellationArea ( 
        satConstellation, 
        constellationAreaPtr );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;
    _msgBufSize = _MAX_MSG_SIZE;

    // Pack the result first
    LE_ASSERT(le_pack_PackResult( &_msgBufPtr, &_msgBufSize, _result ));

    // Pack any "out" parameters
    if (constellationAreaPtr)
    {
        LE_ASSERT(le_pack_PackUint32( &_msgBufPtr, &_msgBufSize,
                                                      *constellationAreaPtr ));
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


static void Handle_le_gnss_EnableExtendedEphemerisFile
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
    _result  = le_gnss_EnableExtendedEphemerisFile (  );

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


static void Handle_le_gnss_DisableExtendedEphemerisFile
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
    _result  = le_gnss_DisableExtendedEphemerisFile (  );

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


static void Handle_le_gnss_LoadExtendedEphemerisFile
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
    le_result_t _result;
    _result  = le_gnss_LoadExtendedEphemerisFile ( 
        fd );

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


static void Handle_le_gnss_GetExtendedEphemerisValidity
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
    uint64_t startTimeBuffer;
    uint64_t *startTimePtr = &startTimeBuffer;
    if (!(_requiredOutputs & (1u << 0)))
    {
        startTimePtr = NULL;
    }
    uint64_t stopTimeBuffer;
    uint64_t *stopTimePtr = &stopTimeBuffer;
    if (!(_requiredOutputs & (1u << 1)))
    {
        stopTimePtr = NULL;
    }

    // Call the function
    le_result_t _result;
    _result  = le_gnss_GetExtendedEphemerisValidity ( 
        startTimePtr, 
        stopTimePtr );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;
    _msgBufSize = _MAX_MSG_SIZE;

    // Pack the result first
    LE_ASSERT(le_pack_PackResult( &_msgBufPtr, &_msgBufSize, _result ));

    // Pack any "out" parameters
    if (startTimePtr)
    {
        LE_ASSERT(le_pack_PackUint64( &_msgBufPtr, &_msgBufSize,
                                                      *startTimePtr ));
    }
    if (stopTimePtr)
    {
        LE_ASSERT(le_pack_PackUint64( &_msgBufPtr, &_msgBufSize,
                                                      *stopTimePtr ));
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


static void Handle_le_gnss_InjectUtcTime
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
    uint64_t timeUtc;
    if (!le_pack_UnpackUint64( &_msgBufPtr, &_msgBufSize,
                                               &timeUtc ))
    {
        goto error_unpack;
    }
    uint32_t timeUnc;
    if (!le_pack_UnpackUint32( &_msgBufPtr, &_msgBufSize,
                                               &timeUnc ))
    {
        goto error_unpack;
    }

    // Define storage for output parameters

    // Call the function
    le_result_t _result;
    _result  = le_gnss_InjectUtcTime ( 
        timeUtc, 
        timeUnc );

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


static void Handle_le_gnss_Start
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
    _result  = le_gnss_Start (  );

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


static void Handle_le_gnss_Stop
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
    _result  = le_gnss_Stop (  );

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


static void Handle_le_gnss_ForceHotRestart
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
    _result  = le_gnss_ForceHotRestart (  );

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


static void Handle_le_gnss_ForceWarmRestart
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
    _result  = le_gnss_ForceWarmRestart (  );

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


static void Handle_le_gnss_ForceColdRestart
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
    _result  = le_gnss_ForceColdRestart (  );

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


static void Handle_le_gnss_ForceFactoryRestart
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
    _result  = le_gnss_ForceFactoryRestart (  );

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


static void Handle_le_gnss_GetTtff
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
    uint32_t ttffBuffer;
    uint32_t *ttffPtr = &ttffBuffer;
    if (!(_requiredOutputs & (1u << 0)))
    {
        ttffPtr = NULL;
    }

    // Call the function
    le_result_t _result;
    _result  = le_gnss_GetTtff ( 
        ttffPtr );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;
    _msgBufSize = _MAX_MSG_SIZE;

    // Pack the result first
    LE_ASSERT(le_pack_PackResult( &_msgBufPtr, &_msgBufSize, _result ));

    // Pack any "out" parameters
    if (ttffPtr)
    {
        LE_ASSERT(le_pack_PackUint32( &_msgBufPtr, &_msgBufSize,
                                                      *ttffPtr ));
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


static void Handle_le_gnss_Enable
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
    _result  = le_gnss_Enable (  );

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


static void Handle_le_gnss_Disable
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
    _result  = le_gnss_Disable (  );

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


static void Handle_le_gnss_SetAcquisitionRate
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
    uint32_t rate;
    if (!le_pack_UnpackUint32( &_msgBufPtr, &_msgBufSize,
                                               &rate ))
    {
        goto error_unpack;
    }

    // Define storage for output parameters

    // Call the function
    le_result_t _result;
    _result  = le_gnss_SetAcquisitionRate ( 
        rate );

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


static void Handle_le_gnss_GetAcquisitionRate
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
    uint32_t rateBuffer;
    uint32_t *ratePtr = &rateBuffer;
    if (!(_requiredOutputs & (1u << 0)))
    {
        ratePtr = NULL;
    }

    // Call the function
    le_result_t _result;
    _result  = le_gnss_GetAcquisitionRate ( 
        ratePtr );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;
    _msgBufSize = _MAX_MSG_SIZE;

    // Pack the result first
    LE_ASSERT(le_pack_PackResult( &_msgBufPtr, &_msgBufSize, _result ));

    // Pack any "out" parameters
    if (ratePtr)
    {
        LE_ASSERT(le_pack_PackUint32( &_msgBufPtr, &_msgBufSize,
                                                      *ratePtr ));
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


static void AsyncResponse_le_gnss_AddPositionHandler
(
    le_gnss_SampleRef_t positionSampleRef,
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
    _msgPtr->id = _MSGID_le_gnss_AddPositionHandler;
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


static void Handle_le_gnss_AddPositionHandler
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
    le_gnss_PositionHandlerRef_t _result;
    _result  = le_gnss_AddPositionHandler ( AsyncResponse_le_gnss_AddPositionHandler, 
        contextPtr );

    if (_result)
    {
        // Put the handler reference result and a pointer to the associated remove function
        // into the server data object.  This function pointer is needed in case the client
        // is closed and the handlers need to be removed.
        serverDataPtr->handlerRef = (le_event_HandlerRef_t)_result;
        serverDataPtr->removeHandlerFunc =
            (RemoveHandlerFunc_t)le_gnss_RemovePositionHandler;

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


static void Handle_le_gnss_RemovePositionHandler
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
    le_gnss_PositionHandlerRef_t handlerRef;
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
    handlerRef = (le_gnss_PositionHandlerRef_t)serverDataPtr->handlerRef;
    le_mem_Release(serverDataPtr);

    // Define storage for output parameters

    // Call the function
    le_gnss_RemovePositionHandler ( 
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


static void Handle_le_gnss_GetPositionState
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
    le_gnss_SampleRef_t positionSampleRef;
    if (!le_pack_UnpackReference( &_msgBufPtr, &_msgBufSize,
                                               &positionSampleRef ))
    {
        goto error_unpack;
    }

    // Define storage for output parameters
    le_gnss_FixState_t stateBuffer;
    le_gnss_FixState_t *statePtr = &stateBuffer;
    if (!(_requiredOutputs & (1u << 0)))
    {
        statePtr = NULL;
    }

    // Call the function
    le_result_t _result;
    _result  = le_gnss_GetPositionState ( 
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


static void Handle_le_gnss_GetLocation
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
    le_gnss_SampleRef_t positionSampleRef;
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
    int32_t hAccuracyBuffer;
    int32_t *hAccuracyPtr = &hAccuracyBuffer;
    if (!(_requiredOutputs & (1u << 2)))
    {
        hAccuracyPtr = NULL;
    }

    // Call the function
    le_result_t _result;
    _result  = le_gnss_GetLocation ( 
        positionSampleRef, 
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


static void Handle_le_gnss_GetAltitude
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
    le_gnss_SampleRef_t positionSampleRef;
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
    int32_t vAccuracyBuffer;
    int32_t *vAccuracyPtr = &vAccuracyBuffer;
    if (!(_requiredOutputs & (1u << 1)))
    {
        vAccuracyPtr = NULL;
    }

    // Call the function
    le_result_t _result;
    _result  = le_gnss_GetAltitude ( 
        positionSampleRef, 
        altitudePtr, 
        vAccuracyPtr );

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


static void Handle_le_gnss_GetTime
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
    le_gnss_SampleRef_t positionSampleRef;
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
    _result  = le_gnss_GetTime ( 
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


static void Handle_le_gnss_GetGpsTime
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
    le_gnss_SampleRef_t positionSampleRef;
    if (!le_pack_UnpackReference( &_msgBufPtr, &_msgBufSize,
                                               &positionSampleRef ))
    {
        goto error_unpack;
    }

    // Define storage for output parameters
    uint32_t gpsWeekBuffer;
    uint32_t *gpsWeekPtr = &gpsWeekBuffer;
    if (!(_requiredOutputs & (1u << 0)))
    {
        gpsWeekPtr = NULL;
    }
    uint32_t gpsTimeOfWeekBuffer;
    uint32_t *gpsTimeOfWeekPtr = &gpsTimeOfWeekBuffer;
    if (!(_requiredOutputs & (1u << 1)))
    {
        gpsTimeOfWeekPtr = NULL;
    }

    // Call the function
    le_result_t _result;
    _result  = le_gnss_GetGpsTime ( 
        positionSampleRef, 
        gpsWeekPtr, 
        gpsTimeOfWeekPtr );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;
    _msgBufSize = _MAX_MSG_SIZE;

    // Pack the result first
    LE_ASSERT(le_pack_PackResult( &_msgBufPtr, &_msgBufSize, _result ));

    // Pack any "out" parameters
    if (gpsWeekPtr)
    {
        LE_ASSERT(le_pack_PackUint32( &_msgBufPtr, &_msgBufSize,
                                                      *gpsWeekPtr ));
    }
    if (gpsTimeOfWeekPtr)
    {
        LE_ASSERT(le_pack_PackUint32( &_msgBufPtr, &_msgBufSize,
                                                      *gpsTimeOfWeekPtr ));
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


static void Handle_le_gnss_GetEpochTime
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
    le_gnss_SampleRef_t positionSampleRef;
    if (!le_pack_UnpackReference( &_msgBufPtr, &_msgBufSize,
                                               &positionSampleRef ))
    {
        goto error_unpack;
    }

    // Define storage for output parameters
    uint64_t millisecondsBuffer;
    uint64_t *millisecondsPtr = &millisecondsBuffer;
    if (!(_requiredOutputs & (1u << 0)))
    {
        millisecondsPtr = NULL;
    }

    // Call the function
    le_result_t _result;
    _result  = le_gnss_GetEpochTime ( 
        positionSampleRef, 
        millisecondsPtr );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;
    _msgBufSize = _MAX_MSG_SIZE;

    // Pack the result first
    LE_ASSERT(le_pack_PackResult( &_msgBufPtr, &_msgBufSize, _result ));

    // Pack any "out" parameters
    if (millisecondsPtr)
    {
        LE_ASSERT(le_pack_PackUint64( &_msgBufPtr, &_msgBufSize,
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


static void Handle_le_gnss_GetTimeAccuracy
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
    le_gnss_SampleRef_t positionSampleRef;
    if (!le_pack_UnpackReference( &_msgBufPtr, &_msgBufSize,
                                               &positionSampleRef ))
    {
        goto error_unpack;
    }

    // Define storage for output parameters
    uint32_t timeAccuracyBuffer;
    uint32_t *timeAccuracyPtr = &timeAccuracyBuffer;
    if (!(_requiredOutputs & (1u << 0)))
    {
        timeAccuracyPtr = NULL;
    }

    // Call the function
    le_result_t _result;
    _result  = le_gnss_GetTimeAccuracy ( 
        positionSampleRef, 
        timeAccuracyPtr );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;
    _msgBufSize = _MAX_MSG_SIZE;

    // Pack the result first
    LE_ASSERT(le_pack_PackResult( &_msgBufPtr, &_msgBufSize, _result ));

    // Pack any "out" parameters
    if (timeAccuracyPtr)
    {
        LE_ASSERT(le_pack_PackUint32( &_msgBufPtr, &_msgBufSize,
                                                      *timeAccuracyPtr ));
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


static void Handle_le_gnss_GetGpsLeapSeconds
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
    le_gnss_SampleRef_t positionSampleRef;
    if (!le_pack_UnpackReference( &_msgBufPtr, &_msgBufSize,
                                               &positionSampleRef ))
    {
        goto error_unpack;
    }

    // Define storage for output parameters
    uint8_t leapSecondsBuffer;
    uint8_t *leapSecondsPtr = &leapSecondsBuffer;
    if (!(_requiredOutputs & (1u << 0)))
    {
        leapSecondsPtr = NULL;
    }

    // Call the function
    le_result_t _result;
    _result  = le_gnss_GetGpsLeapSeconds ( 
        positionSampleRef, 
        leapSecondsPtr );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;
    _msgBufSize = _MAX_MSG_SIZE;

    // Pack the result first
    LE_ASSERT(le_pack_PackResult( &_msgBufPtr, &_msgBufSize, _result ));

    // Pack any "out" parameters
    if (leapSecondsPtr)
    {
        LE_ASSERT(le_pack_PackUint8( &_msgBufPtr, &_msgBufSize,
                                                      *leapSecondsPtr ));
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


static void Handle_le_gnss_GetDate
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
    le_gnss_SampleRef_t positionSampleRef;
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
    _result  = le_gnss_GetDate ( 
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


static void Handle_le_gnss_GetHorizontalSpeed
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
    le_gnss_SampleRef_t positionSampleRef;
    if (!le_pack_UnpackReference( &_msgBufPtr, &_msgBufSize,
                                               &positionSampleRef ))
    {
        goto error_unpack;
    }

    // Define storage for output parameters
    uint32_t hspeedBuffer;
    uint32_t *hspeedPtr = &hspeedBuffer;
    if (!(_requiredOutputs & (1u << 0)))
    {
        hspeedPtr = NULL;
    }
    uint32_t hspeedAccuracyBuffer;
    uint32_t *hspeedAccuracyPtr = &hspeedAccuracyBuffer;
    if (!(_requiredOutputs & (1u << 1)))
    {
        hspeedAccuracyPtr = NULL;
    }

    // Call the function
    le_result_t _result;
    _result  = le_gnss_GetHorizontalSpeed ( 
        positionSampleRef, 
        hspeedPtr, 
        hspeedAccuracyPtr );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;
    _msgBufSize = _MAX_MSG_SIZE;

    // Pack the result first
    LE_ASSERT(le_pack_PackResult( &_msgBufPtr, &_msgBufSize, _result ));

    // Pack any "out" parameters
    if (hspeedPtr)
    {
        LE_ASSERT(le_pack_PackUint32( &_msgBufPtr, &_msgBufSize,
                                                      *hspeedPtr ));
    }
    if (hspeedAccuracyPtr)
    {
        LE_ASSERT(le_pack_PackUint32( &_msgBufPtr, &_msgBufSize,
                                                      *hspeedAccuracyPtr ));
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


static void Handle_le_gnss_GetVerticalSpeed
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
    le_gnss_SampleRef_t positionSampleRef;
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
    _result  = le_gnss_GetVerticalSpeed ( 
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


static void Handle_le_gnss_GetDirection
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
    le_gnss_SampleRef_t positionSampleRef;
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
    _result  = le_gnss_GetDirection ( 
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


static void Handle_le_gnss_GetSatellitesInfo
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
    le_gnss_SampleRef_t positionSampleRef;
    if (!le_pack_UnpackReference( &_msgBufPtr, &_msgBufSize,
                                               &positionSampleRef ))
    {
        goto error_unpack;
    }
    size_t satIdSize;
    if (!le_pack_UnpackSize( &_msgBufPtr, &_msgBufSize,
                               &satIdSize ))
    {
        goto error_unpack;
    }
    if ( (satIdSize > 80) )
    {
        LE_DEBUG("Adjusting satIdSize from %zu to 80", satIdSize);
        satIdSize = 80;
    }
    size_t satConstSize;
    if (!le_pack_UnpackSize( &_msgBufPtr, &_msgBufSize,
                               &satConstSize ))
    {
        goto error_unpack;
    }
    if ( (satConstSize > 80) )
    {
        LE_DEBUG("Adjusting satConstSize from %zu to 80", satConstSize);
        satConstSize = 80;
    }
    size_t satUsedSize;
    if (!le_pack_UnpackSize( &_msgBufPtr, &_msgBufSize,
                               &satUsedSize ))
    {
        goto error_unpack;
    }
    if ( (satUsedSize > 80) )
    {
        LE_DEBUG("Adjusting satUsedSize from %zu to 80", satUsedSize);
        satUsedSize = 80;
    }
    size_t satSnrSize;
    if (!le_pack_UnpackSize( &_msgBufPtr, &_msgBufSize,
                               &satSnrSize ))
    {
        goto error_unpack;
    }
    if ( (satSnrSize > 80) )
    {
        LE_DEBUG("Adjusting satSnrSize from %zu to 80", satSnrSize);
        satSnrSize = 80;
    }
    size_t satAzimSize;
    if (!le_pack_UnpackSize( &_msgBufPtr, &_msgBufSize,
                               &satAzimSize ))
    {
        goto error_unpack;
    }
    if ( (satAzimSize > 80) )
    {
        LE_DEBUG("Adjusting satAzimSize from %zu to 80", satAzimSize);
        satAzimSize = 80;
    }
    size_t satElevSize;
    if (!le_pack_UnpackSize( &_msgBufPtr, &_msgBufSize,
                               &satElevSize ))
    {
        goto error_unpack;
    }
    if ( (satElevSize > 80) )
    {
        LE_DEBUG("Adjusting satElevSize from %zu to 80", satElevSize);
        satElevSize = 80;
    }

    // Define storage for output parameters
    uint16_t satIdBuffer[80];
    uint16_t *satIdPtr = satIdBuffer;
    size_t *satIdSizePtr = &satIdSize;
    if (!(_requiredOutputs & (1u << 0)))
    {
        satIdPtr = NULL;
    }
    le_gnss_Constellation_t satConstBuffer[80];
    le_gnss_Constellation_t *satConstPtr = satConstBuffer;
    size_t *satConstSizePtr = &satConstSize;
    if (!(_requiredOutputs & (1u << 1)))
    {
        satConstPtr = NULL;
    }
    bool satUsedBuffer[80];
    bool *satUsedPtr = satUsedBuffer;
    size_t *satUsedSizePtr = &satUsedSize;
    if (!(_requiredOutputs & (1u << 2)))
    {
        satUsedPtr = NULL;
    }
    uint8_t satSnrBuffer[80];
    uint8_t *satSnrPtr = satSnrBuffer;
    size_t *satSnrSizePtr = &satSnrSize;
    if (!(_requiredOutputs & (1u << 3)))
    {
        satSnrPtr = NULL;
    }
    uint16_t satAzimBuffer[80];
    uint16_t *satAzimPtr = satAzimBuffer;
    size_t *satAzimSizePtr = &satAzimSize;
    if (!(_requiredOutputs & (1u << 4)))
    {
        satAzimPtr = NULL;
    }
    uint8_t satElevBuffer[80];
    uint8_t *satElevPtr = satElevBuffer;
    size_t *satElevSizePtr = &satElevSize;
    if (!(_requiredOutputs & (1u << 5)))
    {
        satElevPtr = NULL;
    }

    // Call the function
    le_result_t _result;
    _result  = le_gnss_GetSatellitesInfo ( 
        positionSampleRef, 
        satIdPtr, 
        &satIdSize, 
        satConstPtr, 
        &satConstSize, 
        satUsedPtr, 
        &satUsedSize, 
        satSnrPtr, 
        &satSnrSize, 
        satAzimPtr, 
        &satAzimSize, 
        satElevPtr, 
        &satElevSize );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;
    _msgBufSize = _MAX_MSG_SIZE;

    // Pack the result first
    LE_ASSERT(le_pack_PackResult( &_msgBufPtr, &_msgBufSize, _result ));

    // Pack any "out" parameters
    if (satIdPtr)
    {
        bool satIdResult;
        LE_PACK_PACKARRAY( &_msgBufPtr, &_msgBufSize,
                           satIdPtr, (*satIdSizePtr),
                           80, le_pack_PackUint16,
                           &satIdResult );
        LE_ASSERT(satIdResult);
    }
    if (satConstPtr)
    {
        bool satConstResult;
        LE_PACK_PACKARRAY( &_msgBufPtr, &_msgBufSize,
                           satConstPtr, (*satConstSizePtr),
                           80, le_pack_PackUint32,
                           &satConstResult );
        LE_ASSERT(satConstResult);
    }
    if (satUsedPtr)
    {
        bool satUsedResult;
        LE_PACK_PACKARRAY( &_msgBufPtr, &_msgBufSize,
                           satUsedPtr, (*satUsedSizePtr),
                           80, le_pack_PackBool,
                           &satUsedResult );
        LE_ASSERT(satUsedResult);
    }
    if (satSnrPtr)
    {
        bool satSnrResult;
        LE_PACK_PACKARRAY( &_msgBufPtr, &_msgBufSize,
                           satSnrPtr, (*satSnrSizePtr),
                           80, le_pack_PackUint8,
                           &satSnrResult );
        LE_ASSERT(satSnrResult);
    }
    if (satAzimPtr)
    {
        bool satAzimResult;
        LE_PACK_PACKARRAY( &_msgBufPtr, &_msgBufSize,
                           satAzimPtr, (*satAzimSizePtr),
                           80, le_pack_PackUint16,
                           &satAzimResult );
        LE_ASSERT(satAzimResult);
    }
    if (satElevPtr)
    {
        bool satElevResult;
        LE_PACK_PACKARRAY( &_msgBufPtr, &_msgBufSize,
                           satElevPtr, (*satElevSizePtr),
                           80, le_pack_PackUint8,
                           &satElevResult );
        LE_ASSERT(satElevResult);
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


static void Handle_le_gnss_GetSbasConstellationCategory
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
    uint16_t satId;
    if (!le_pack_UnpackUint16( &_msgBufPtr, &_msgBufSize,
                                               &satId ))
    {
        goto error_unpack;
    }

    // Define storage for output parameters

    // Call the function
    le_gnss_SbasConstellationCategory_t _result;
    _result  = le_gnss_GetSbasConstellationCategory ( 
        satId );

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


static void Handle_le_gnss_GetSatellitesStatus
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
    le_gnss_SampleRef_t positionSampleRef;
    if (!le_pack_UnpackReference( &_msgBufPtr, &_msgBufSize,
                                               &positionSampleRef ))
    {
        goto error_unpack;
    }

    // Define storage for output parameters
    uint8_t satsInViewCountBuffer;
    uint8_t *satsInViewCountPtr = &satsInViewCountBuffer;
    if (!(_requiredOutputs & (1u << 0)))
    {
        satsInViewCountPtr = NULL;
    }
    uint8_t satsTrackingCountBuffer;
    uint8_t *satsTrackingCountPtr = &satsTrackingCountBuffer;
    if (!(_requiredOutputs & (1u << 1)))
    {
        satsTrackingCountPtr = NULL;
    }
    uint8_t satsUsedCountBuffer;
    uint8_t *satsUsedCountPtr = &satsUsedCountBuffer;
    if (!(_requiredOutputs & (1u << 2)))
    {
        satsUsedCountPtr = NULL;
    }

    // Call the function
    le_result_t _result;
    _result  = le_gnss_GetSatellitesStatus ( 
        positionSampleRef, 
        satsInViewCountPtr, 
        satsTrackingCountPtr, 
        satsUsedCountPtr );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;
    _msgBufSize = _MAX_MSG_SIZE;

    // Pack the result first
    LE_ASSERT(le_pack_PackResult( &_msgBufPtr, &_msgBufSize, _result ));

    // Pack any "out" parameters
    if (satsInViewCountPtr)
    {
        LE_ASSERT(le_pack_PackUint8( &_msgBufPtr, &_msgBufSize,
                                                      *satsInViewCountPtr ));
    }
    if (satsTrackingCountPtr)
    {
        LE_ASSERT(le_pack_PackUint8( &_msgBufPtr, &_msgBufSize,
                                                      *satsTrackingCountPtr ));
    }
    if (satsUsedCountPtr)
    {
        LE_ASSERT(le_pack_PackUint8( &_msgBufPtr, &_msgBufSize,
                                                      *satsUsedCountPtr ));
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


static void Handle_le_gnss_GetDop
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
    le_gnss_SampleRef_t positionSampleRef;
    if (!le_pack_UnpackReference( &_msgBufPtr, &_msgBufSize,
                                               &positionSampleRef ))
    {
        goto error_unpack;
    }

    // Define storage for output parameters
    uint16_t hdopBuffer;
    uint16_t *hdopPtr = &hdopBuffer;
    if (!(_requiredOutputs & (1u << 0)))
    {
        hdopPtr = NULL;
    }
    uint16_t vdopBuffer;
    uint16_t *vdopPtr = &vdopBuffer;
    if (!(_requiredOutputs & (1u << 1)))
    {
        vdopPtr = NULL;
    }
    uint16_t pdopBuffer;
    uint16_t *pdopPtr = &pdopBuffer;
    if (!(_requiredOutputs & (1u << 2)))
    {
        pdopPtr = NULL;
    }

    // Call the function
    le_result_t _result;
    _result  = le_gnss_GetDop ( 
        positionSampleRef, 
        hdopPtr, 
        vdopPtr, 
        pdopPtr );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;
    _msgBufSize = _MAX_MSG_SIZE;

    // Pack the result first
    LE_ASSERT(le_pack_PackResult( &_msgBufPtr, &_msgBufSize, _result ));

    // Pack any "out" parameters
    if (hdopPtr)
    {
        LE_ASSERT(le_pack_PackUint16( &_msgBufPtr, &_msgBufSize,
                                                      *hdopPtr ));
    }
    if (vdopPtr)
    {
        LE_ASSERT(le_pack_PackUint16( &_msgBufPtr, &_msgBufSize,
                                                      *vdopPtr ));
    }
    if (pdopPtr)
    {
        LE_ASSERT(le_pack_PackUint16( &_msgBufPtr, &_msgBufSize,
                                                      *pdopPtr ));
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


static void Handle_le_gnss_GetDilutionOfPrecision
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
    le_gnss_SampleRef_t positionSampleRef;
    if (!le_pack_UnpackReference( &_msgBufPtr, &_msgBufSize,
                                               &positionSampleRef ))
    {
        goto error_unpack;
    }
    le_gnss_DopType_t dopType;
    if (!le_pack_UnpackUint32( &_msgBufPtr, &_msgBufSize,
                                               &dopType ))
    {
        goto error_unpack;
    }

    // Define storage for output parameters
    uint16_t dopBuffer;
    uint16_t *dopPtr = &dopBuffer;
    if (!(_requiredOutputs & (1u << 0)))
    {
        dopPtr = NULL;
    }

    // Call the function
    le_result_t _result;
    _result  = le_gnss_GetDilutionOfPrecision ( 
        positionSampleRef, 
        dopType, 
        dopPtr );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;
    _msgBufSize = _MAX_MSG_SIZE;

    // Pack the result first
    LE_ASSERT(le_pack_PackResult( &_msgBufPtr, &_msgBufSize, _result ));

    // Pack any "out" parameters
    if (dopPtr)
    {
        LE_ASSERT(le_pack_PackUint16( &_msgBufPtr, &_msgBufSize,
                                                      *dopPtr ));
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


static void Handle_le_gnss_GetAltitudeOnWgs84
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
    le_gnss_SampleRef_t positionSampleRef;
    if (!le_pack_UnpackReference( &_msgBufPtr, &_msgBufSize,
                                               &positionSampleRef ))
    {
        goto error_unpack;
    }

    // Define storage for output parameters
    int32_t altitudeOnWgs84Buffer;
    int32_t *altitudeOnWgs84Ptr = &altitudeOnWgs84Buffer;
    if (!(_requiredOutputs & (1u << 0)))
    {
        altitudeOnWgs84Ptr = NULL;
    }

    // Call the function
    le_result_t _result;
    _result  = le_gnss_GetAltitudeOnWgs84 ( 
        positionSampleRef, 
        altitudeOnWgs84Ptr );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;
    _msgBufSize = _MAX_MSG_SIZE;

    // Pack the result first
    LE_ASSERT(le_pack_PackResult( &_msgBufPtr, &_msgBufSize, _result ));

    // Pack any "out" parameters
    if (altitudeOnWgs84Ptr)
    {
        LE_ASSERT(le_pack_PackInt32( &_msgBufPtr, &_msgBufSize,
                                                      *altitudeOnWgs84Ptr ));
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


static void Handle_le_gnss_GetMagneticDeviation
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
    le_gnss_SampleRef_t positionSampleRef;
    if (!le_pack_UnpackReference( &_msgBufPtr, &_msgBufSize,
                                               &positionSampleRef ))
    {
        goto error_unpack;
    }

    // Define storage for output parameters
    int32_t magneticDeviationBuffer;
    int32_t *magneticDeviationPtr = &magneticDeviationBuffer;
    if (!(_requiredOutputs & (1u << 0)))
    {
        magneticDeviationPtr = NULL;
    }

    // Call the function
    le_result_t _result;
    _result  = le_gnss_GetMagneticDeviation ( 
        positionSampleRef, 
        magneticDeviationPtr );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;
    _msgBufSize = _MAX_MSG_SIZE;

    // Pack the result first
    LE_ASSERT(le_pack_PackResult( &_msgBufPtr, &_msgBufSize, _result ));

    // Pack any "out" parameters
    if (magneticDeviationPtr)
    {
        LE_ASSERT(le_pack_PackInt32( &_msgBufPtr, &_msgBufSize,
                                                      *magneticDeviationPtr ));
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


static void Handle_le_gnss_GetLastSampleRef
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
    le_gnss_SampleRef_t _result;
    _result  = le_gnss_GetLastSampleRef (  );

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


static void Handle_le_gnss_ReleaseSampleRef
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
    le_gnss_SampleRef_t positionSampleRef;
    if (!le_pack_UnpackReference( &_msgBufPtr, &_msgBufSize,
                                               &positionSampleRef ))
    {
        goto error_unpack;
    }

    // Define storage for output parameters

    // Call the function
    le_gnss_ReleaseSampleRef ( 
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


static void Handle_le_gnss_SetSuplAssistedMode
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
    le_gnss_AssistedMode_t assistedMode;
    if (!le_pack_UnpackUint32( &_msgBufPtr, &_msgBufSize,
                                               &assistedMode ))
    {
        goto error_unpack;
    }

    // Define storage for output parameters

    // Call the function
    le_result_t _result;
    _result  = le_gnss_SetSuplAssistedMode ( 
        assistedMode );

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


static void Handle_le_gnss_GetSuplAssistedMode
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
    le_gnss_AssistedMode_t assistedModeBuffer;
    le_gnss_AssistedMode_t *assistedModePtr = &assistedModeBuffer;
    if (!(_requiredOutputs & (1u << 0)))
    {
        assistedModePtr = NULL;
    }

    // Call the function
    le_result_t _result;
    _result  = le_gnss_GetSuplAssistedMode ( 
        assistedModePtr );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;
    _msgBufSize = _MAX_MSG_SIZE;

    // Pack the result first
    LE_ASSERT(le_pack_PackResult( &_msgBufPtr, &_msgBufSize, _result ));

    // Pack any "out" parameters
    if (assistedModePtr)
    {
        LE_ASSERT(le_pack_PackUint32( &_msgBufPtr, &_msgBufSize,
                                                      *assistedModePtr ));
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


static void Handle_le_gnss_SetSuplServerUrl
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
    char suplServerUrl[257];
    if (!le_pack_UnpackString( &_msgBufPtr, &_msgBufSize,
                               suplServerUrl,
                               sizeof(suplServerUrl),
                               256 ))
    {
        goto error_unpack;
    }

    // Define storage for output parameters

    // Call the function
    le_result_t _result;
    _result  = le_gnss_SetSuplServerUrl ( 
        suplServerUrl );

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


static void Handle_le_gnss_InjectSuplCertificate
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
    uint8_t suplCertificateId;
    if (!le_pack_UnpackUint8( &_msgBufPtr, &_msgBufSize,
                                               &suplCertificateId ))
    {
        goto error_unpack;
    }
    uint16_t suplCertificateLen;
    if (!le_pack_UnpackUint16( &_msgBufPtr, &_msgBufSize,
                                               &suplCertificateLen ))
    {
        goto error_unpack;
    }
    char suplCertificate[2001];
    if (!le_pack_UnpackString( &_msgBufPtr, &_msgBufSize,
                               suplCertificate,
                               sizeof(suplCertificate),
                               2000 ))
    {
        goto error_unpack;
    }

    // Define storage for output parameters

    // Call the function
    le_result_t _result;
    _result  = le_gnss_InjectSuplCertificate ( 
        suplCertificateId, 
        suplCertificateLen, 
        suplCertificate );

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


static void Handle_le_gnss_DeleteSuplCertificate
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
    uint8_t suplCertificateId;
    if (!le_pack_UnpackUint8( &_msgBufPtr, &_msgBufSize,
                                               &suplCertificateId ))
    {
        goto error_unpack;
    }

    // Define storage for output parameters

    // Call the function
    le_result_t _result;
    _result  = le_gnss_DeleteSuplCertificate ( 
        suplCertificateId );

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


static void Handle_le_gnss_SetNmeaSentences
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
    le_gnss_NmeaBitMask_t nmeaMask;
    if (!le_pack_UnpackUint32( &_msgBufPtr, &_msgBufSize,
                                               &nmeaMask ))
    {
        goto error_unpack;
    }

    // Define storage for output parameters

    // Call the function
    le_result_t _result;
    _result  = le_gnss_SetNmeaSentences ( 
        nmeaMask );

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


static void Handle_le_gnss_GetNmeaSentences
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
    le_gnss_NmeaBitMask_t nmeaMaskPtrBuffer;
    le_gnss_NmeaBitMask_t *nmeaMaskPtrPtr = &nmeaMaskPtrBuffer;
    if (!(_requiredOutputs & (1u << 0)))
    {
        nmeaMaskPtrPtr = NULL;
    }

    // Call the function
    le_result_t _result;
    _result  = le_gnss_GetNmeaSentences ( 
        nmeaMaskPtrPtr );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;
    _msgBufSize = _MAX_MSG_SIZE;

    // Pack the result first
    LE_ASSERT(le_pack_PackResult( &_msgBufPtr, &_msgBufSize, _result ));

    // Pack any "out" parameters
    if (nmeaMaskPtrPtr)
    {
        LE_ASSERT(le_pack_PackUint32( &_msgBufPtr, &_msgBufSize,
                                                      *nmeaMaskPtrPtr ));
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


static void Handle_le_gnss_GetState
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
    le_gnss_State_t _result;
    _result  = le_gnss_GetState (  );

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


static void Handle_le_gnss_SetMinElevation
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
    uint8_t minElevation;
    if (!le_pack_UnpackUint8( &_msgBufPtr, &_msgBufSize,
                                               &minElevation ))
    {
        goto error_unpack;
    }

    // Define storage for output parameters

    // Call the function
    le_result_t _result;
    _result  = le_gnss_SetMinElevation ( 
        minElevation );

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


static void Handle_le_gnss_GetMinElevation
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
    uint8_t minElevationPtrBuffer;
    uint8_t *minElevationPtrPtr = &minElevationPtrBuffer;
    if (!(_requiredOutputs & (1u << 0)))
    {
        minElevationPtrPtr = NULL;
    }

    // Call the function
    le_result_t _result;
    _result  = le_gnss_GetMinElevation ( 
        minElevationPtrPtr );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;
    _msgBufSize = _MAX_MSG_SIZE;

    // Pack the result first
    LE_ASSERT(le_pack_PackResult( &_msgBufPtr, &_msgBufSize, _result ));

    // Pack any "out" parameters
    if (minElevationPtrPtr)
    {
        LE_ASSERT(le_pack_PackUint8( &_msgBufPtr, &_msgBufSize,
                                                      *minElevationPtrPtr ));
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


static void Handle_le_gnss_SetDopResolution
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
    le_gnss_Resolution_t resolution;
    if (!le_pack_UnpackUint32( &_msgBufPtr, &_msgBufSize,
                                               &resolution ))
    {
        goto error_unpack;
    }

    // Define storage for output parameters

    // Call the function
    le_result_t _result;
    _result  = le_gnss_SetDopResolution ( 
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
        case _MSGID_le_gnss_SetConstellation : Handle_le_gnss_SetConstellation(msgRef); break;
        case _MSGID_le_gnss_GetConstellation : Handle_le_gnss_GetConstellation(msgRef); break;
        case _MSGID_le_gnss_SetConstellationArea : Handle_le_gnss_SetConstellationArea(msgRef); break;
        case _MSGID_le_gnss_GetConstellationArea : Handle_le_gnss_GetConstellationArea(msgRef); break;
        case _MSGID_le_gnss_EnableExtendedEphemerisFile : Handle_le_gnss_EnableExtendedEphemerisFile(msgRef); break;
        case _MSGID_le_gnss_DisableExtendedEphemerisFile : Handle_le_gnss_DisableExtendedEphemerisFile(msgRef); break;
        case _MSGID_le_gnss_LoadExtendedEphemerisFile : Handle_le_gnss_LoadExtendedEphemerisFile(msgRef); break;
        case _MSGID_le_gnss_GetExtendedEphemerisValidity : Handle_le_gnss_GetExtendedEphemerisValidity(msgRef); break;
        case _MSGID_le_gnss_InjectUtcTime : Handle_le_gnss_InjectUtcTime(msgRef); break;
        case _MSGID_le_gnss_Start : Handle_le_gnss_Start(msgRef); break;
        case _MSGID_le_gnss_Stop : Handle_le_gnss_Stop(msgRef); break;
        case _MSGID_le_gnss_ForceHotRestart : Handle_le_gnss_ForceHotRestart(msgRef); break;
        case _MSGID_le_gnss_ForceWarmRestart : Handle_le_gnss_ForceWarmRestart(msgRef); break;
        case _MSGID_le_gnss_ForceColdRestart : Handle_le_gnss_ForceColdRestart(msgRef); break;
        case _MSGID_le_gnss_ForceFactoryRestart : Handle_le_gnss_ForceFactoryRestart(msgRef); break;
        case _MSGID_le_gnss_GetTtff : Handle_le_gnss_GetTtff(msgRef); break;
        case _MSGID_le_gnss_Enable : Handle_le_gnss_Enable(msgRef); break;
        case _MSGID_le_gnss_Disable : Handle_le_gnss_Disable(msgRef); break;
        case _MSGID_le_gnss_SetAcquisitionRate : Handle_le_gnss_SetAcquisitionRate(msgRef); break;
        case _MSGID_le_gnss_GetAcquisitionRate : Handle_le_gnss_GetAcquisitionRate(msgRef); break;
        case _MSGID_le_gnss_AddPositionHandler : Handle_le_gnss_AddPositionHandler(msgRef); break;
        case _MSGID_le_gnss_RemovePositionHandler : Handle_le_gnss_RemovePositionHandler(msgRef); break;
        case _MSGID_le_gnss_GetPositionState : Handle_le_gnss_GetPositionState(msgRef); break;
        case _MSGID_le_gnss_GetLocation : Handle_le_gnss_GetLocation(msgRef); break;
        case _MSGID_le_gnss_GetAltitude : Handle_le_gnss_GetAltitude(msgRef); break;
        case _MSGID_le_gnss_GetTime : Handle_le_gnss_GetTime(msgRef); break;
        case _MSGID_le_gnss_GetGpsTime : Handle_le_gnss_GetGpsTime(msgRef); break;
        case _MSGID_le_gnss_GetEpochTime : Handle_le_gnss_GetEpochTime(msgRef); break;
        case _MSGID_le_gnss_GetTimeAccuracy : Handle_le_gnss_GetTimeAccuracy(msgRef); break;
        case _MSGID_le_gnss_GetGpsLeapSeconds : Handle_le_gnss_GetGpsLeapSeconds(msgRef); break;
        case _MSGID_le_gnss_GetDate : Handle_le_gnss_GetDate(msgRef); break;
        case _MSGID_le_gnss_GetHorizontalSpeed : Handle_le_gnss_GetHorizontalSpeed(msgRef); break;
        case _MSGID_le_gnss_GetVerticalSpeed : Handle_le_gnss_GetVerticalSpeed(msgRef); break;
        case _MSGID_le_gnss_GetDirection : Handle_le_gnss_GetDirection(msgRef); break;
        case _MSGID_le_gnss_GetSatellitesInfo : Handle_le_gnss_GetSatellitesInfo(msgRef); break;
        case _MSGID_le_gnss_GetSbasConstellationCategory : Handle_le_gnss_GetSbasConstellationCategory(msgRef); break;
        case _MSGID_le_gnss_GetSatellitesStatus : Handle_le_gnss_GetSatellitesStatus(msgRef); break;
        case _MSGID_le_gnss_GetDop : Handle_le_gnss_GetDop(msgRef); break;
        case _MSGID_le_gnss_GetDilutionOfPrecision : Handle_le_gnss_GetDilutionOfPrecision(msgRef); break;
        case _MSGID_le_gnss_GetAltitudeOnWgs84 : Handle_le_gnss_GetAltitudeOnWgs84(msgRef); break;
        case _MSGID_le_gnss_GetMagneticDeviation : Handle_le_gnss_GetMagneticDeviation(msgRef); break;
        case _MSGID_le_gnss_GetLastSampleRef : Handle_le_gnss_GetLastSampleRef(msgRef); break;
        case _MSGID_le_gnss_ReleaseSampleRef : Handle_le_gnss_ReleaseSampleRef(msgRef); break;
        case _MSGID_le_gnss_SetSuplAssistedMode : Handle_le_gnss_SetSuplAssistedMode(msgRef); break;
        case _MSGID_le_gnss_GetSuplAssistedMode : Handle_le_gnss_GetSuplAssistedMode(msgRef); break;
        case _MSGID_le_gnss_SetSuplServerUrl : Handle_le_gnss_SetSuplServerUrl(msgRef); break;
        case _MSGID_le_gnss_InjectSuplCertificate : Handle_le_gnss_InjectSuplCertificate(msgRef); break;
        case _MSGID_le_gnss_DeleteSuplCertificate : Handle_le_gnss_DeleteSuplCertificate(msgRef); break;
        case _MSGID_le_gnss_SetNmeaSentences : Handle_le_gnss_SetNmeaSentences(msgRef); break;
        case _MSGID_le_gnss_GetNmeaSentences : Handle_le_gnss_GetNmeaSentences(msgRef); break;
        case _MSGID_le_gnss_GetState : Handle_le_gnss_GetState(msgRef); break;
        case _MSGID_le_gnss_SetMinElevation : Handle_le_gnss_SetMinElevation(msgRef); break;
        case _MSGID_le_gnss_GetMinElevation : Handle_le_gnss_GetMinElevation(msgRef); break;
        case _MSGID_le_gnss_SetDopResolution : Handle_le_gnss_SetDopResolution(msgRef); break;

        default: LE_ERROR("Unknowm msg id = %i", msgPtr->id);
    }

    // Clear the client session ref associated with the current message, since the message
    // has now been processed.
    _ClientSessionRef = 0;
}