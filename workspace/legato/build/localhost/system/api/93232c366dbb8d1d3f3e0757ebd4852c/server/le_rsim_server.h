

/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */


#ifndef LE_RSIM_INTERFACE_H_INCLUDE_GUARD
#define LE_RSIM_INTERFACE_H_INCLUDE_GUARD


#include "legato.h"


//--------------------------------------------------------------------------------------------------
/**
 * Get the server service reference
 */
//--------------------------------------------------------------------------------------------------
le_msg_ServiceRef_t le_rsim_GetServiceRef
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * Get the client session reference for the current message
 */
//--------------------------------------------------------------------------------------------------
le_msg_SessionRef_t le_rsim_GetClientSessionRef
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * Initialize the server and advertise the service.
 */
//--------------------------------------------------------------------------------------------------
void le_rsim_AdvertiseService
(
    void
);


//--------------------------------------------------------------------------------------------------
/**
 * Remote SIM maximal message size
 *
 * @note
 *  The maximal message size of 276 bytes is based on the longest possible SAP message.
 *  This message is a @c TRANSFER_APDU_RESP composed of a 4-byte SAP header, a 8-byte ResultCode,
 *  a 4-byte ResponseAPDU header, 256 bytes of APDU data, 2 status bytes and 2 padding bytes.
 */
//--------------------------------------------------------------------------------------------------
#define LE_RSIM_MAX_MSG_SIZE 276

//--------------------------------------------------------------------------------------------------
/**
 ** Remote SIM minimal message size
 **
 ** @note
 **  This minimal length is set to ensure that the communication with the remote SIM server
 **  can be effective.
 */
//--------------------------------------------------------------------------------------------------
#define LE_RSIM_MIN_MSG_SIZE 200

//--------------------------------------------------------------------------------------------------
/**
 * Reference type used by Add/Remove functions for EVENT 'le_rsim_Message'
 */
//--------------------------------------------------------------------------------------------------
typedef struct le_rsim_MessageHandler* le_rsim_MessageHandlerRef_t;


//--------------------------------------------------------------------------------------------------
/**
 * Handler for message notification
 */
//--------------------------------------------------------------------------------------------------
typedef void (*le_rsim_MessageHandlerFunc_t)
(
    const uint8_t* messagePtr,
        ///< Message to notify
    size_t messageSize,
        ///<
    void* contextPtr
        ///<
);

//--------------------------------------------------------------------------------------------------
/**
 * Handler for message sending result
 */
//--------------------------------------------------------------------------------------------------
typedef void (*le_rsim_CallbackHandlerFunc_t)
(
    uint8_t messageId,
        ///< Message identifier
    le_result_t result,
        ///< Sending result
    void* contextPtr
        ///<
);


//--------------------------------------------------------------------------------------------------
/**
 * Add handler function for EVENT 'le_rsim_Message'
 *
 * This event is used to notify and send a message
 */
//--------------------------------------------------------------------------------------------------
le_rsim_MessageHandlerRef_t le_rsim_AddMessageHandler
(
    le_rsim_MessageHandlerFunc_t handlerPtr,
        ///< [IN]
    void* contextPtr
        ///< [IN]
);



//--------------------------------------------------------------------------------------------------
/**
 * Remove handler function for EVENT 'le_rsim_Message'
 */
//--------------------------------------------------------------------------------------------------
void le_rsim_RemoveMessageHandler
(
    le_rsim_MessageHandlerRef_t handlerRef
        ///< [IN]
);



//--------------------------------------------------------------------------------------------------
/**
 * This function is called by the application to send a message to the Legato remote SIM service
 *
 * @return
 *  - LE_OK             Function succeeded
 *  - LE_BAD_PARAMETER  Message is too long
 *
 * @warning The sending process is asynchronous: only the message length is checked by this function
 * before returning a result. A callback function should be passed as a parameter in order to be
 * notified of the message sending result.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_rsim_SendMessage
(
    const uint8_t* messagePtr,
        ///< [IN] Message to send
    size_t messageSize,
        ///< [IN]
    le_rsim_CallbackHandlerFunc_t callbackPtr,
        ///< [IN] Callback for sending result
    void* contextPtr
        ///< [IN]
);


#endif // LE_RSIM_INTERFACE_H_INCLUDE_GUARD