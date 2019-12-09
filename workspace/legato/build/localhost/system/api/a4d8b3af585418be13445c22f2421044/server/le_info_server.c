
/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */


#include "le_info_messages.h"
#include "le_info_server.h"


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
le_msg_ServiceRef_t le_info_GetServiceRef
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
le_msg_SessionRef_t le_info_GetClientSessionRef
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
void le_info_AdvertiseService
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
    _ServerDataPool = le_mem_CreatePool("le_info_ServerData", sizeof(_ServerData_t));

    // Create safe reference map for handler references.
    // The size of the map should be based on the number of handlers defined for the server.
    // Don't expect that to be more than 2-3, so use 3 as a reasonable guess.
    _HandlerRefMap = le_ref_CreateMap("le_info_ServerHandlers", 3);

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


static void Handle_le_info_GetImei
(
    le_msg_MessageRef_t _msgRef

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
    size_t imeiSize;
    if (!le_pack_UnpackSize( &_msgBufPtr, &_msgBufSize,
                               &imeiSize ))
    {
        goto error_unpack;
    }
    if ( (imeiSize > 15) &&
         (imeiSize < UINT32_MAX) )
    {
        LE_DEBUG("Adjusting imeiSize from %zu to 15", imeiSize);
        imeiSize = 15;
    }
    if (imeiSize >= UINT32_MAX)
    {
        imeiSize = 0;
    }
    else
    {
        imeiSize++;
    }

    // Define storage for output parameters
    char imeiBuffer[16];
    char *imei = imeiBuffer;
    imei[0] = 0;
    if (!(_requiredOutputs & (1u << 0)))
    {
        imei = NULL;
        imeiSize = 0;
    }

    // Call the function
    le_result_t _result;
    _result  = le_info_GetImei ( 
        imei, 
        imeiSize );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;
    _msgBufSize = _MAX_MSG_SIZE;

    // Pack the result first
    LE_ASSERT(le_pack_PackResult( &_msgBufPtr, &_msgBufSize, _result ));

    // Pack any "out" parameters
    if (imei)
    {
        LE_ASSERT(le_pack_PackString( &_msgBufPtr, &_msgBufSize,
                                      imei, 15 ));
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


static void Handle_le_info_GetImeiSv
(
    le_msg_MessageRef_t _msgRef

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
    size_t imeiSvSize;
    if (!le_pack_UnpackSize( &_msgBufPtr, &_msgBufSize,
                               &imeiSvSize ))
    {
        goto error_unpack;
    }
    if ( (imeiSvSize > 255) &&
         (imeiSvSize < UINT32_MAX) )
    {
        LE_DEBUG("Adjusting imeiSvSize from %zu to 255", imeiSvSize);
        imeiSvSize = 255;
    }
    if (imeiSvSize >= UINT32_MAX)
    {
        imeiSvSize = 0;
    }
    else
    {
        imeiSvSize++;
    }

    // Define storage for output parameters
    char imeiSvBuffer[256];
    char *imeiSv = imeiSvBuffer;
    imeiSv[0] = 0;
    if (!(_requiredOutputs & (1u << 0)))
    {
        imeiSv = NULL;
        imeiSvSize = 0;
    }

    // Call the function
    le_result_t _result;
    _result  = le_info_GetImeiSv ( 
        imeiSv, 
        imeiSvSize );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;
    _msgBufSize = _MAX_MSG_SIZE;

    // Pack the result first
    LE_ASSERT(le_pack_PackResult( &_msgBufPtr, &_msgBufSize, _result ));

    // Pack any "out" parameters
    if (imeiSv)
    {
        LE_ASSERT(le_pack_PackString( &_msgBufPtr, &_msgBufSize,
                                      imeiSv, 255 ));
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


static void Handle_le_info_GetFirmwareVersion
(
    le_msg_MessageRef_t _msgRef

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
    size_t versionSize;
    if (!le_pack_UnpackSize( &_msgBufPtr, &_msgBufSize,
                               &versionSize ))
    {
        goto error_unpack;
    }
    if ( (versionSize > 256) &&
         (versionSize < UINT32_MAX) )
    {
        LE_DEBUG("Adjusting versionSize from %zu to 256", versionSize);
        versionSize = 256;
    }
    if (versionSize >= UINT32_MAX)
    {
        versionSize = 0;
    }
    else
    {
        versionSize++;
    }

    // Define storage for output parameters
    char versionBuffer[257];
    char *version = versionBuffer;
    version[0] = 0;
    if (!(_requiredOutputs & (1u << 0)))
    {
        version = NULL;
        versionSize = 0;
    }

    // Call the function
    le_result_t _result;
    _result  = le_info_GetFirmwareVersion ( 
        version, 
        versionSize );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;
    _msgBufSize = _MAX_MSG_SIZE;

    // Pack the result first
    LE_ASSERT(le_pack_PackResult( &_msgBufPtr, &_msgBufSize, _result ));

    // Pack any "out" parameters
    if (version)
    {
        LE_ASSERT(le_pack_PackString( &_msgBufPtr, &_msgBufSize,
                                      version, 256 ));
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


static void Handle_le_info_GetResetInformation
(
    le_msg_MessageRef_t _msgRef

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
    size_t resetSpecificInfoStrSize;
    if (!le_pack_UnpackSize( &_msgBufPtr, &_msgBufSize,
                               &resetSpecificInfoStrSize ))
    {
        goto error_unpack;
    }
    if ( (resetSpecificInfoStrSize > 50) &&
         (resetSpecificInfoStrSize < UINT32_MAX) )
    {
        LE_DEBUG("Adjusting resetSpecificInfoStrSize from %zu to 50", resetSpecificInfoStrSize);
        resetSpecificInfoStrSize = 50;
    }
    if (resetSpecificInfoStrSize >= UINT32_MAX)
    {
        resetSpecificInfoStrSize = 0;
    }
    else
    {
        resetSpecificInfoStrSize++;
    }

    // Define storage for output parameters
    le_info_Reset_t resetBuffer;
    le_info_Reset_t *resetPtr = &resetBuffer;
    if (!(_requiredOutputs & (1u << 0)))
    {
        resetPtr = NULL;
    }
    char resetSpecificInfoStrBuffer[51];
    char *resetSpecificInfoStr = resetSpecificInfoStrBuffer;
    resetSpecificInfoStr[0] = 0;
    if (!(_requiredOutputs & (1u << 1)))
    {
        resetSpecificInfoStr = NULL;
        resetSpecificInfoStrSize = 0;
    }

    // Call the function
    le_result_t _result;
    _result  = le_info_GetResetInformation ( 
        resetPtr, 
        resetSpecificInfoStr, 
        resetSpecificInfoStrSize );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;
    _msgBufSize = _MAX_MSG_SIZE;

    // Pack the result first
    LE_ASSERT(le_pack_PackResult( &_msgBufPtr, &_msgBufSize, _result ));

    // Pack any "out" parameters
    if (resetPtr)
    {
        LE_ASSERT(le_pack_PackUint32( &_msgBufPtr, &_msgBufSize,
                                                      *resetPtr ));
    }
    if (resetSpecificInfoStr)
    {
        LE_ASSERT(le_pack_PackString( &_msgBufPtr, &_msgBufSize,
                                      resetSpecificInfoStr, 50 ));
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


static void Handle_le_info_GetBootloaderVersion
(
    le_msg_MessageRef_t _msgRef

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
    size_t versionSize;
    if (!le_pack_UnpackSize( &_msgBufPtr, &_msgBufSize,
                               &versionSize ))
    {
        goto error_unpack;
    }
    if ( (versionSize > 256) &&
         (versionSize < UINT32_MAX) )
    {
        LE_DEBUG("Adjusting versionSize from %zu to 256", versionSize);
        versionSize = 256;
    }
    if (versionSize >= UINT32_MAX)
    {
        versionSize = 0;
    }
    else
    {
        versionSize++;
    }

    // Define storage for output parameters
    char versionBuffer[257];
    char *version = versionBuffer;
    version[0] = 0;
    if (!(_requiredOutputs & (1u << 0)))
    {
        version = NULL;
        versionSize = 0;
    }

    // Call the function
    le_result_t _result;
    _result  = le_info_GetBootloaderVersion ( 
        version, 
        versionSize );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;
    _msgBufSize = _MAX_MSG_SIZE;

    // Pack the result first
    LE_ASSERT(le_pack_PackResult( &_msgBufPtr, &_msgBufSize, _result ));

    // Pack any "out" parameters
    if (version)
    {
        LE_ASSERT(le_pack_PackString( &_msgBufPtr, &_msgBufSize,
                                      version, 256 ));
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


static void Handle_le_info_GetDeviceModel
(
    le_msg_MessageRef_t _msgRef

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
    size_t modelPtrSize;
    if (!le_pack_UnpackSize( &_msgBufPtr, &_msgBufSize,
                               &modelPtrSize ))
    {
        goto error_unpack;
    }
    if ( (modelPtrSize > 256) &&
         (modelPtrSize < UINT32_MAX) )
    {
        LE_DEBUG("Adjusting modelPtrSize from %zu to 256", modelPtrSize);
        modelPtrSize = 256;
    }
    if (modelPtrSize >= UINT32_MAX)
    {
        modelPtrSize = 0;
    }
    else
    {
        modelPtrSize++;
    }

    // Define storage for output parameters
    char modelPtrBuffer[257];
    char *modelPtr = modelPtrBuffer;
    modelPtr[0] = 0;
    if (!(_requiredOutputs & (1u << 0)))
    {
        modelPtr = NULL;
        modelPtrSize = 0;
    }

    // Call the function
    le_result_t _result;
    _result  = le_info_GetDeviceModel ( 
        modelPtr, 
        modelPtrSize );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;
    _msgBufSize = _MAX_MSG_SIZE;

    // Pack the result first
    LE_ASSERT(le_pack_PackResult( &_msgBufPtr, &_msgBufSize, _result ));

    // Pack any "out" parameters
    if (modelPtr)
    {
        LE_ASSERT(le_pack_PackString( &_msgBufPtr, &_msgBufSize,
                                      modelPtr, 256 ));
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


static void Handle_le_info_GetMeid
(
    le_msg_MessageRef_t _msgRef

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
    size_t meidStrSize;
    if (!le_pack_UnpackSize( &_msgBufPtr, &_msgBufSize,
                               &meidStrSize ))
    {
        goto error_unpack;
    }
    if ( (meidStrSize > 32) &&
         (meidStrSize < UINT32_MAX) )
    {
        LE_DEBUG("Adjusting meidStrSize from %zu to 32", meidStrSize);
        meidStrSize = 32;
    }
    if (meidStrSize >= UINT32_MAX)
    {
        meidStrSize = 0;
    }
    else
    {
        meidStrSize++;
    }

    // Define storage for output parameters
    char meidStrBuffer[33];
    char *meidStr = meidStrBuffer;
    meidStr[0] = 0;
    if (!(_requiredOutputs & (1u << 0)))
    {
        meidStr = NULL;
        meidStrSize = 0;
    }

    // Call the function
    le_result_t _result;
    _result  = le_info_GetMeid ( 
        meidStr, 
        meidStrSize );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;
    _msgBufSize = _MAX_MSG_SIZE;

    // Pack the result first
    LE_ASSERT(le_pack_PackResult( &_msgBufPtr, &_msgBufSize, _result ));

    // Pack any "out" parameters
    if (meidStr)
    {
        LE_ASSERT(le_pack_PackString( &_msgBufPtr, &_msgBufSize,
                                      meidStr, 32 ));
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


static void Handle_le_info_GetEsn
(
    le_msg_MessageRef_t _msgRef

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
    size_t esnStrSize;
    if (!le_pack_UnpackSize( &_msgBufPtr, &_msgBufSize,
                               &esnStrSize ))
    {
        goto error_unpack;
    }
    if ( (esnStrSize > 32) &&
         (esnStrSize < UINT32_MAX) )
    {
        LE_DEBUG("Adjusting esnStrSize from %zu to 32", esnStrSize);
        esnStrSize = 32;
    }
    if (esnStrSize >= UINT32_MAX)
    {
        esnStrSize = 0;
    }
    else
    {
        esnStrSize++;
    }

    // Define storage for output parameters
    char esnStrBuffer[33];
    char *esnStr = esnStrBuffer;
    esnStr[0] = 0;
    if (!(_requiredOutputs & (1u << 0)))
    {
        esnStr = NULL;
        esnStrSize = 0;
    }

    // Call the function
    le_result_t _result;
    _result  = le_info_GetEsn ( 
        esnStr, 
        esnStrSize );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;
    _msgBufSize = _MAX_MSG_SIZE;

    // Pack the result first
    LE_ASSERT(le_pack_PackResult( &_msgBufPtr, &_msgBufSize, _result ));

    // Pack any "out" parameters
    if (esnStr)
    {
        LE_ASSERT(le_pack_PackString( &_msgBufPtr, &_msgBufSize,
                                      esnStr, 32 ));
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


static void Handle_le_info_GetMdn
(
    le_msg_MessageRef_t _msgRef

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
    size_t mdnStrSize;
    if (!le_pack_UnpackSize( &_msgBufPtr, &_msgBufSize,
                               &mdnStrSize ))
    {
        goto error_unpack;
    }
    if ( (mdnStrSize > 32) &&
         (mdnStrSize < UINT32_MAX) )
    {
        LE_DEBUG("Adjusting mdnStrSize from %zu to 32", mdnStrSize);
        mdnStrSize = 32;
    }
    if (mdnStrSize >= UINT32_MAX)
    {
        mdnStrSize = 0;
    }
    else
    {
        mdnStrSize++;
    }

    // Define storage for output parameters
    char mdnStrBuffer[33];
    char *mdnStr = mdnStrBuffer;
    mdnStr[0] = 0;
    if (!(_requiredOutputs & (1u << 0)))
    {
        mdnStr = NULL;
        mdnStrSize = 0;
    }

    // Call the function
    le_result_t _result;
    _result  = le_info_GetMdn ( 
        mdnStr, 
        mdnStrSize );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;
    _msgBufSize = _MAX_MSG_SIZE;

    // Pack the result first
    LE_ASSERT(le_pack_PackResult( &_msgBufPtr, &_msgBufSize, _result ));

    // Pack any "out" parameters
    if (mdnStr)
    {
        LE_ASSERT(le_pack_PackString( &_msgBufPtr, &_msgBufSize,
                                      mdnStr, 32 ));
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


static void Handle_le_info_GetPrlVersion
(
    le_msg_MessageRef_t _msgRef

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
    uint16_t prlVersionBuffer;
    uint16_t *prlVersionPtr = &prlVersionBuffer;
    if (!(_requiredOutputs & (1u << 0)))
    {
        prlVersionPtr = NULL;
    }

    // Call the function
    le_result_t _result;
    _result  = le_info_GetPrlVersion ( 
        prlVersionPtr );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;
    _msgBufSize = _MAX_MSG_SIZE;

    // Pack the result first
    LE_ASSERT(le_pack_PackResult( &_msgBufPtr, &_msgBufSize, _result ));

    // Pack any "out" parameters
    if (prlVersionPtr)
    {
        LE_ASSERT(le_pack_PackUint16( &_msgBufPtr, &_msgBufSize,
                                                      *prlVersionPtr ));
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


static void Handle_le_info_GetPrlOnlyPreference
(
    le_msg_MessageRef_t _msgRef

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
    bool prlOnlyPreferenceBuffer;
    bool *prlOnlyPreferencePtr = &prlOnlyPreferenceBuffer;
    if (!(_requiredOutputs & (1u << 0)))
    {
        prlOnlyPreferencePtr = NULL;
    }

    // Call the function
    le_result_t _result;
    _result  = le_info_GetPrlOnlyPreference ( 
        prlOnlyPreferencePtr );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;
    _msgBufSize = _MAX_MSG_SIZE;

    // Pack the result first
    LE_ASSERT(le_pack_PackResult( &_msgBufPtr, &_msgBufSize, _result ));

    // Pack any "out" parameters
    if (prlOnlyPreferencePtr)
    {
        LE_ASSERT(le_pack_PackBool( &_msgBufPtr, &_msgBufSize,
                                                      *prlOnlyPreferencePtr ));
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


static void Handle_le_info_GetMin
(
    le_msg_MessageRef_t _msgRef

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
    size_t msisdnStrSize;
    if (!le_pack_UnpackSize( &_msgBufPtr, &_msgBufSize,
                               &msisdnStrSize ))
    {
        goto error_unpack;
    }
    if ( (msisdnStrSize > 32) &&
         (msisdnStrSize < UINT32_MAX) )
    {
        LE_DEBUG("Adjusting msisdnStrSize from %zu to 32", msisdnStrSize);
        msisdnStrSize = 32;
    }
    if (msisdnStrSize >= UINT32_MAX)
    {
        msisdnStrSize = 0;
    }
    else
    {
        msisdnStrSize++;
    }

    // Define storage for output parameters
    char msisdnStrBuffer[33];
    char *msisdnStr = msisdnStrBuffer;
    msisdnStr[0] = 0;
    if (!(_requiredOutputs & (1u << 0)))
    {
        msisdnStr = NULL;
        msisdnStrSize = 0;
    }

    // Call the function
    le_result_t _result;
    _result  = le_info_GetMin ( 
        msisdnStr, 
        msisdnStrSize );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;
    _msgBufSize = _MAX_MSG_SIZE;

    // Pack the result first
    LE_ASSERT(le_pack_PackResult( &_msgBufPtr, &_msgBufSize, _result ));

    // Pack any "out" parameters
    if (msisdnStr)
    {
        LE_ASSERT(le_pack_PackString( &_msgBufPtr, &_msgBufSize,
                                      msisdnStr, 32 ));
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


static void Handle_le_info_GetNai
(
    le_msg_MessageRef_t _msgRef

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
    size_t naiStrSize;
    if (!le_pack_UnpackSize( &_msgBufPtr, &_msgBufSize,
                               &naiStrSize ))
    {
        goto error_unpack;
    }
    if ( (naiStrSize > 72) &&
         (naiStrSize < UINT32_MAX) )
    {
        LE_DEBUG("Adjusting naiStrSize from %zu to 72", naiStrSize);
        naiStrSize = 72;
    }
    if (naiStrSize >= UINT32_MAX)
    {
        naiStrSize = 0;
    }
    else
    {
        naiStrSize++;
    }

    // Define storage for output parameters
    char naiStrBuffer[73];
    char *naiStr = naiStrBuffer;
    naiStr[0] = 0;
    if (!(_requiredOutputs & (1u << 0)))
    {
        naiStr = NULL;
        naiStrSize = 0;
    }

    // Call the function
    le_result_t _result;
    _result  = le_info_GetNai ( 
        naiStr, 
        naiStrSize );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;
    _msgBufSize = _MAX_MSG_SIZE;

    // Pack the result first
    LE_ASSERT(le_pack_PackResult( &_msgBufPtr, &_msgBufSize, _result ));

    // Pack any "out" parameters
    if (naiStr)
    {
        LE_ASSERT(le_pack_PackString( &_msgBufPtr, &_msgBufSize,
                                      naiStr, 72 ));
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


static void Handle_le_info_GetManufacturerName
(
    le_msg_MessageRef_t _msgRef

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
    size_t mfrNameStrSize;
    if (!le_pack_UnpackSize( &_msgBufPtr, &_msgBufSize,
                               &mfrNameStrSize ))
    {
        goto error_unpack;
    }
    if ( (mfrNameStrSize > 128) &&
         (mfrNameStrSize < UINT32_MAX) )
    {
        LE_DEBUG("Adjusting mfrNameStrSize from %zu to 128", mfrNameStrSize);
        mfrNameStrSize = 128;
    }
    if (mfrNameStrSize >= UINT32_MAX)
    {
        mfrNameStrSize = 0;
    }
    else
    {
        mfrNameStrSize++;
    }

    // Define storage for output parameters
    char mfrNameStrBuffer[129];
    char *mfrNameStr = mfrNameStrBuffer;
    mfrNameStr[0] = 0;
    if (!(_requiredOutputs & (1u << 0)))
    {
        mfrNameStr = NULL;
        mfrNameStrSize = 0;
    }

    // Call the function
    le_result_t _result;
    _result  = le_info_GetManufacturerName ( 
        mfrNameStr, 
        mfrNameStrSize );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;
    _msgBufSize = _MAX_MSG_SIZE;

    // Pack the result first
    LE_ASSERT(le_pack_PackResult( &_msgBufPtr, &_msgBufSize, _result ));

    // Pack any "out" parameters
    if (mfrNameStr)
    {
        LE_ASSERT(le_pack_PackString( &_msgBufPtr, &_msgBufSize,
                                      mfrNameStr, 128 ));
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


static void Handle_le_info_GetPriId
(
    le_msg_MessageRef_t _msgRef

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
    size_t priIdPnStrSize;
    if (!le_pack_UnpackSize( &_msgBufPtr, &_msgBufSize,
                               &priIdPnStrSize ))
    {
        goto error_unpack;
    }
    if ( (priIdPnStrSize > 16) &&
         (priIdPnStrSize < UINT32_MAX) )
    {
        LE_DEBUG("Adjusting priIdPnStrSize from %zu to 16", priIdPnStrSize);
        priIdPnStrSize = 16;
    }
    if (priIdPnStrSize >= UINT32_MAX)
    {
        priIdPnStrSize = 0;
    }
    else
    {
        priIdPnStrSize++;
    }
    size_t priIdRevStrSize;
    if (!le_pack_UnpackSize( &_msgBufPtr, &_msgBufSize,
                               &priIdRevStrSize ))
    {
        goto error_unpack;
    }
    if ( (priIdRevStrSize > 16) &&
         (priIdRevStrSize < UINT32_MAX) )
    {
        LE_DEBUG("Adjusting priIdRevStrSize from %zu to 16", priIdRevStrSize);
        priIdRevStrSize = 16;
    }
    if (priIdRevStrSize >= UINT32_MAX)
    {
        priIdRevStrSize = 0;
    }
    else
    {
        priIdRevStrSize++;
    }

    // Define storage for output parameters
    char priIdPnStrBuffer[17];
    char *priIdPnStr = priIdPnStrBuffer;
    priIdPnStr[0] = 0;
    if (!(_requiredOutputs & (1u << 0)))
    {
        priIdPnStr = NULL;
        priIdPnStrSize = 0;
    }
    char priIdRevStrBuffer[17];
    char *priIdRevStr = priIdRevStrBuffer;
    priIdRevStr[0] = 0;
    if (!(_requiredOutputs & (1u << 1)))
    {
        priIdRevStr = NULL;
        priIdRevStrSize = 0;
    }

    // Call the function
    le_result_t _result;
    _result  = le_info_GetPriId ( 
        priIdPnStr, 
        priIdPnStrSize, 
        priIdRevStr, 
        priIdRevStrSize );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;
    _msgBufSize = _MAX_MSG_SIZE;

    // Pack the result first
    LE_ASSERT(le_pack_PackResult( &_msgBufPtr, &_msgBufSize, _result ));

    // Pack any "out" parameters
    if (priIdPnStr)
    {
        LE_ASSERT(le_pack_PackString( &_msgBufPtr, &_msgBufSize,
                                      priIdPnStr, 16 ));
    }
    if (priIdRevStr)
    {
        LE_ASSERT(le_pack_PackString( &_msgBufPtr, &_msgBufSize,
                                      priIdRevStr, 16 ));
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


static void Handle_le_info_GetCarrierPri
(
    le_msg_MessageRef_t _msgRef

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
    size_t capriNameStrSize;
    if (!le_pack_UnpackSize( &_msgBufPtr, &_msgBufSize,
                               &capriNameStrSize ))
    {
        goto error_unpack;
    }
    if ( (capriNameStrSize > 16) &&
         (capriNameStrSize < UINT32_MAX) )
    {
        LE_DEBUG("Adjusting capriNameStrSize from %zu to 16", capriNameStrSize);
        capriNameStrSize = 16;
    }
    if (capriNameStrSize >= UINT32_MAX)
    {
        capriNameStrSize = 0;
    }
    else
    {
        capriNameStrSize++;
    }
    size_t capriRevStrSize;
    if (!le_pack_UnpackSize( &_msgBufPtr, &_msgBufSize,
                               &capriRevStrSize ))
    {
        goto error_unpack;
    }
    if ( (capriRevStrSize > 12) &&
         (capriRevStrSize < UINT32_MAX) )
    {
        LE_DEBUG("Adjusting capriRevStrSize from %zu to 12", capriRevStrSize);
        capriRevStrSize = 12;
    }
    if (capriRevStrSize >= UINT32_MAX)
    {
        capriRevStrSize = 0;
    }
    else
    {
        capriRevStrSize++;
    }

    // Define storage for output parameters
    char capriNameStrBuffer[17];
    char *capriNameStr = capriNameStrBuffer;
    capriNameStr[0] = 0;
    if (!(_requiredOutputs & (1u << 0)))
    {
        capriNameStr = NULL;
        capriNameStrSize = 0;
    }
    char capriRevStrBuffer[13];
    char *capriRevStr = capriRevStrBuffer;
    capriRevStr[0] = 0;
    if (!(_requiredOutputs & (1u << 1)))
    {
        capriRevStr = NULL;
        capriRevStrSize = 0;
    }

    // Call the function
    le_result_t _result;
    _result  = le_info_GetCarrierPri ( 
        capriNameStr, 
        capriNameStrSize, 
        capriRevStr, 
        capriRevStrSize );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;
    _msgBufSize = _MAX_MSG_SIZE;

    // Pack the result first
    LE_ASSERT(le_pack_PackResult( &_msgBufPtr, &_msgBufSize, _result ));

    // Pack any "out" parameters
    if (capriNameStr)
    {
        LE_ASSERT(le_pack_PackString( &_msgBufPtr, &_msgBufSize,
                                      capriNameStr, 16 ));
    }
    if (capriRevStr)
    {
        LE_ASSERT(le_pack_PackString( &_msgBufPtr, &_msgBufSize,
                                      capriRevStr, 12 ));
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


static void Handle_le_info_GetPlatformSerialNumber
(
    le_msg_MessageRef_t _msgRef

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
    size_t platformSerialNumberStrSize;
    if (!le_pack_UnpackSize( &_msgBufPtr, &_msgBufSize,
                               &platformSerialNumberStrSize ))
    {
        goto error_unpack;
    }
    if ( (platformSerialNumberStrSize > 14) &&
         (platformSerialNumberStrSize < UINT32_MAX) )
    {
        LE_DEBUG("Adjusting platformSerialNumberStrSize from %zu to 14", platformSerialNumberStrSize);
        platformSerialNumberStrSize = 14;
    }
    if (platformSerialNumberStrSize >= UINT32_MAX)
    {
        platformSerialNumberStrSize = 0;
    }
    else
    {
        platformSerialNumberStrSize++;
    }

    // Define storage for output parameters
    char platformSerialNumberStrBuffer[15];
    char *platformSerialNumberStr = platformSerialNumberStrBuffer;
    platformSerialNumberStr[0] = 0;
    if (!(_requiredOutputs & (1u << 0)))
    {
        platformSerialNumberStr = NULL;
        platformSerialNumberStrSize = 0;
    }

    // Call the function
    le_result_t _result;
    _result  = le_info_GetPlatformSerialNumber ( 
        platformSerialNumberStr, 
        platformSerialNumberStrSize );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;
    _msgBufSize = _MAX_MSG_SIZE;

    // Pack the result first
    LE_ASSERT(le_pack_PackResult( &_msgBufPtr, &_msgBufSize, _result ));

    // Pack any "out" parameters
    if (platformSerialNumberStr)
    {
        LE_ASSERT(le_pack_PackString( &_msgBufPtr, &_msgBufSize,
                                      platformSerialNumberStr, 14 ));
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


static void Handle_le_info_GetRfDeviceStatus
(
    le_msg_MessageRef_t _msgRef

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
    size_t manufacturedIdSize;
    if (!le_pack_UnpackSize( &_msgBufPtr, &_msgBufSize,
                               &manufacturedIdSize ))
    {
        goto error_unpack;
    }
    if ( (manufacturedIdSize > 16) )
    {
        LE_DEBUG("Adjusting manufacturedIdSize from %zu to 16", manufacturedIdSize);
        manufacturedIdSize = 16;
    }
    size_t productIdSize;
    if (!le_pack_UnpackSize( &_msgBufPtr, &_msgBufSize,
                               &productIdSize ))
    {
        goto error_unpack;
    }
    if ( (productIdSize > 16) )
    {
        LE_DEBUG("Adjusting productIdSize from %zu to 16", productIdSize);
        productIdSize = 16;
    }
    size_t statusSize;
    if (!le_pack_UnpackSize( &_msgBufPtr, &_msgBufSize,
                               &statusSize ))
    {
        goto error_unpack;
    }
    if ( (statusSize > 16) )
    {
        LE_DEBUG("Adjusting statusSize from %zu to 16", statusSize);
        statusSize = 16;
    }

    // Define storage for output parameters
    uint16_t manufacturedIdBuffer[16];
    uint16_t *manufacturedIdPtr = manufacturedIdBuffer;
    size_t *manufacturedIdSizePtr = &manufacturedIdSize;
    if (!(_requiredOutputs & (1u << 0)))
    {
        manufacturedIdPtr = NULL;
    }
    uint8_t productIdBuffer[16];
    uint8_t *productIdPtr = productIdBuffer;
    size_t *productIdSizePtr = &productIdSize;
    if (!(_requiredOutputs & (1u << 1)))
    {
        productIdPtr = NULL;
    }
    bool statusBuffer[16];
    bool *statusPtr = statusBuffer;
    size_t *statusSizePtr = &statusSize;
    if (!(_requiredOutputs & (1u << 2)))
    {
        statusPtr = NULL;
    }

    // Call the function
    le_result_t _result;
    _result  = le_info_GetRfDeviceStatus ( 
        manufacturedIdPtr, 
        &manufacturedIdSize, 
        productIdPtr, 
        &productIdSize, 
        statusPtr, 
        &statusSize );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;
    _msgBufSize = _MAX_MSG_SIZE;

    // Pack the result first
    LE_ASSERT(le_pack_PackResult( &_msgBufPtr, &_msgBufSize, _result ));

    // Pack any "out" parameters
    if (manufacturedIdPtr)
    {
        bool manufacturedIdResult;
        LE_PACK_PACKARRAY( &_msgBufPtr, &_msgBufSize,
                           manufacturedIdPtr, (*manufacturedIdSizePtr),
                           16, le_pack_PackUint16,
                           &manufacturedIdResult );
        LE_ASSERT(manufacturedIdResult);
    }
    if (productIdPtr)
    {
        bool productIdResult;
        LE_PACK_PACKARRAY( &_msgBufPtr, &_msgBufSize,
                           productIdPtr, (*productIdSizePtr),
                           16, le_pack_PackUint8,
                           &productIdResult );
        LE_ASSERT(productIdResult);
    }
    if (statusPtr)
    {
        bool statusResult;
        LE_PACK_PACKARRAY( &_msgBufPtr, &_msgBufSize,
                           statusPtr, (*statusSizePtr),
                           16, le_pack_PackBool,
                           &statusResult );
        LE_ASSERT(statusResult);
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


static void Handle_le_info_GetSku
(
    le_msg_MessageRef_t _msgRef

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
    size_t skuIdStrSize;
    if (!le_pack_UnpackSize( &_msgBufPtr, &_msgBufSize,
                               &skuIdStrSize ))
    {
        goto error_unpack;
    }
    if ( (skuIdStrSize > 32) &&
         (skuIdStrSize < UINT32_MAX) )
    {
        LE_DEBUG("Adjusting skuIdStrSize from %zu to 32", skuIdStrSize);
        skuIdStrSize = 32;
    }
    if (skuIdStrSize >= UINT32_MAX)
    {
        skuIdStrSize = 0;
    }
    else
    {
        skuIdStrSize++;
    }

    // Define storage for output parameters
    char skuIdStrBuffer[33];
    char *skuIdStr = skuIdStrBuffer;
    skuIdStr[0] = 0;
    if (!(_requiredOutputs & (1u << 0)))
    {
        skuIdStr = NULL;
        skuIdStrSize = 0;
    }

    // Call the function
    le_result_t _result;
    _result  = le_info_GetSku ( 
        skuIdStr, 
        skuIdStrSize );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;
    _msgBufSize = _MAX_MSG_SIZE;

    // Pack the result first
    LE_ASSERT(le_pack_PackResult( &_msgBufPtr, &_msgBufSize, _result ));

    // Pack any "out" parameters
    if (skuIdStr)
    {
        LE_ASSERT(le_pack_PackString( &_msgBufPtr, &_msgBufSize,
                                      skuIdStr, 32 ));
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


static void Handle_le_info_GetExpectedResetsCount
(
    le_msg_MessageRef_t _msgRef

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
    uint64_t resetsCountPtrBuffer;
    uint64_t *resetsCountPtrPtr = &resetsCountPtrBuffer;
    if (!(_requiredOutputs & (1u << 0)))
    {
        resetsCountPtrPtr = NULL;
    }

    // Call the function
    le_result_t _result;
    _result  = le_info_GetExpectedResetsCount ( 
        resetsCountPtrPtr );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;
    _msgBufSize = _MAX_MSG_SIZE;

    // Pack the result first
    LE_ASSERT(le_pack_PackResult( &_msgBufPtr, &_msgBufSize, _result ));

    // Pack any "out" parameters
    if (resetsCountPtrPtr)
    {
        LE_ASSERT(le_pack_PackUint64( &_msgBufPtr, &_msgBufSize,
                                                      *resetsCountPtrPtr ));
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


static void Handle_le_info_GetUnexpectedResetsCount
(
    le_msg_MessageRef_t _msgRef

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
    uint64_t resetsCountPtrBuffer;
    uint64_t *resetsCountPtrPtr = &resetsCountPtrBuffer;
    if (!(_requiredOutputs & (1u << 0)))
    {
        resetsCountPtrPtr = NULL;
    }

    // Call the function
    le_result_t _result;
    _result  = le_info_GetUnexpectedResetsCount ( 
        resetsCountPtrPtr );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;
    _msgBufSize = _MAX_MSG_SIZE;

    // Pack the result first
    LE_ASSERT(le_pack_PackResult( &_msgBufPtr, &_msgBufSize, _result ));

    // Pack any "out" parameters
    if (resetsCountPtrPtr)
    {
        LE_ASSERT(le_pack_PackUint64( &_msgBufPtr, &_msgBufSize,
                                                      *resetsCountPtrPtr ));
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
        case _MSGID_le_info_GetImei : Handle_le_info_GetImei(msgRef); break;
        case _MSGID_le_info_GetImeiSv : Handle_le_info_GetImeiSv(msgRef); break;
        case _MSGID_le_info_GetFirmwareVersion : Handle_le_info_GetFirmwareVersion(msgRef); break;
        case _MSGID_le_info_GetResetInformation : Handle_le_info_GetResetInformation(msgRef); break;
        case _MSGID_le_info_GetBootloaderVersion : Handle_le_info_GetBootloaderVersion(msgRef); break;
        case _MSGID_le_info_GetDeviceModel : Handle_le_info_GetDeviceModel(msgRef); break;
        case _MSGID_le_info_GetMeid : Handle_le_info_GetMeid(msgRef); break;
        case _MSGID_le_info_GetEsn : Handle_le_info_GetEsn(msgRef); break;
        case _MSGID_le_info_GetMdn : Handle_le_info_GetMdn(msgRef); break;
        case _MSGID_le_info_GetPrlVersion : Handle_le_info_GetPrlVersion(msgRef); break;
        case _MSGID_le_info_GetPrlOnlyPreference : Handle_le_info_GetPrlOnlyPreference(msgRef); break;
        case _MSGID_le_info_GetMin : Handle_le_info_GetMin(msgRef); break;
        case _MSGID_le_info_GetNai : Handle_le_info_GetNai(msgRef); break;
        case _MSGID_le_info_GetManufacturerName : Handle_le_info_GetManufacturerName(msgRef); break;
        case _MSGID_le_info_GetPriId : Handle_le_info_GetPriId(msgRef); break;
        case _MSGID_le_info_GetCarrierPri : Handle_le_info_GetCarrierPri(msgRef); break;
        case _MSGID_le_info_GetPlatformSerialNumber : Handle_le_info_GetPlatformSerialNumber(msgRef); break;
        case _MSGID_le_info_GetRfDeviceStatus : Handle_le_info_GetRfDeviceStatus(msgRef); break;
        case _MSGID_le_info_GetSku : Handle_le_info_GetSku(msgRef); break;
        case _MSGID_le_info_GetExpectedResetsCount : Handle_le_info_GetExpectedResetsCount(msgRef); break;
        case _MSGID_le_info_GetUnexpectedResetsCount : Handle_le_info_GetUnexpectedResetsCount(msgRef); break;

        default: LE_ERROR("Unknowm msg id = %i", msgPtr->id);
    }

    // Clear the client session ref associated with the current message, since the message
    // has now been processed.
    _ClientSessionRef = 0;
}