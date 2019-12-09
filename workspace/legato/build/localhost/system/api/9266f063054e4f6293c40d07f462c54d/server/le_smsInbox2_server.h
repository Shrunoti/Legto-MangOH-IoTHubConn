

/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */


#ifndef LE_SMSINBOX2_INTERFACE_H_INCLUDE_GUARD
#define LE_SMSINBOX2_INTERFACE_H_INCLUDE_GUARD


#include "legato.h"

// Interface specific includes
#include "le_mdmDefs_server.h"
#include "le_sim_server.h"
#include "le_sms_server.h"


//--------------------------------------------------------------------------------------------------
/**
 * Get the server service reference
 */
//--------------------------------------------------------------------------------------------------
le_msg_ServiceRef_t le_smsInbox2_GetServiceRef
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * Get the client session reference for the current message
 */
//--------------------------------------------------------------------------------------------------
le_msg_SessionRef_t le_smsInbox2_GetClientSessionRef
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * Initialize the server and advertise the service.
 */
//--------------------------------------------------------------------------------------------------
void le_smsInbox2_AdvertiseService
(
    void
);


//--------------------------------------------------------------------------------------------------
/**
 * Define unknown format
 */
//--------------------------------------------------------------------------------------------------
#define LE_SMSINBOX2_FORMAT_UNKNOWN -1

//--------------------------------------------------------------------------------------------------
/**
 * Define the name of length of the message box name
 */
//--------------------------------------------------------------------------------------------------
#define LE_SMSINBOX2_MAX_MBOX_NAME_LEN 12

//--------------------------------------------------------------------------------------------------
/**
 * Reference type for referring to open message box sessions.
 */
//--------------------------------------------------------------------------------------------------
typedef struct le_smsInbox2_Session* le_smsInbox2_SessionRef_t;


//--------------------------------------------------------------------------------------------------
/**
 * Reference type used by Add/Remove functions for EVENT 'le_smsInbox2_RxMessage'
 */
//--------------------------------------------------------------------------------------------------
typedef struct le_smsInbox2_RxMessageHandler* le_smsInbox2_RxMessageHandlerRef_t;


//--------------------------------------------------------------------------------------------------
/**
 * Handler for New Message.
 *
 */
//--------------------------------------------------------------------------------------------------
typedef void (*le_smsInbox2_RxMessageHandlerFunc_t)
(
    uint32_t msgId,
        ///< Message identifier.
    void* contextPtr
        ///<
);


//--------------------------------------------------------------------------------------------------
/**
 * Open a message box.
 *
 * @return
 * Reference on the opened message box session
 */
//--------------------------------------------------------------------------------------------------
le_smsInbox2_SessionRef_t le_smsInbox2_Open
(
    void
);



//--------------------------------------------------------------------------------------------------
/**
 * Close a previously open message box.
 *
 */
//--------------------------------------------------------------------------------------------------
void le_smsInbox2_Close
(
    le_smsInbox2_SessionRef_t sessionRef
        ///< [IN] Mailbox session reference.
);



//--------------------------------------------------------------------------------------------------
/**
 * Add handler function for EVENT 'le_smsInbox2_RxMessage'
 *
 * This event provides information on new received messages.
 *
 */
//--------------------------------------------------------------------------------------------------
le_smsInbox2_RxMessageHandlerRef_t le_smsInbox2_AddRxMessageHandler
(
    le_smsInbox2_RxMessageHandlerFunc_t handlerPtr,
        ///< [IN]
    void* contextPtr
        ///< [IN]
);



//--------------------------------------------------------------------------------------------------
/**
 * Remove handler function for EVENT 'le_smsInbox2_RxMessage'
 */
//--------------------------------------------------------------------------------------------------
void le_smsInbox2_RemoveRxMessageHandler
(
    le_smsInbox2_RxMessageHandlerRef_t handlerRef
        ///< [IN]
);



//--------------------------------------------------------------------------------------------------
/**
 * Delete a Message.
 *
 * @note It is valid to delete a non-existent message.
 */
//--------------------------------------------------------------------------------------------------
void le_smsInbox2_DeleteMsg
(
    uint32_t msgId
        ///< [IN] Message identifier.
);



//--------------------------------------------------------------------------------------------------
/**
 * Retrieves the IMSI of the message receiver SIM if it applies.
 *
 * @return
 *  - LE_NOT_FOUND     The message item is not tied to a SIM card, the imsi string is empty.
 *  - LE_OVERFLOW      The imsiPtr buffer was too small for the IMSI.
 *  - LE_BAD_PARAMETER The message reference is invalid.
 *  - LE_FAULT         The function failed.
 *  - LE_OK            The function succeeded.
 *
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_smsInbox2_GetImsi
(
    uint32_t msgId,
        ///< [IN] Message identifier.
    char* imsi,
        ///< [OUT] IMSI.
    size_t imsiSize
        ///< [IN]
);



//--------------------------------------------------------------------------------------------------
/**
 * Get the message format (text, binary or PDU).
 *
 * @return
 *  - Message format.
 *  - FORMAT_UNKNOWN when the message format cannot be identified or the message reference is
 *    invalid.
 * @note For incoming SMS Inbox, format returned by le_smsInbox1_GetFormat() is never
 * LE_SMSINBOX1_FORMAT_PDU.
 */
//--------------------------------------------------------------------------------------------------
le_sms_Format_t le_smsInbox2_GetFormat
(
    uint32_t msgId
        ///< [IN] Message identifier.
);



//--------------------------------------------------------------------------------------------------
/**
 * Get the Sender Identifier.
 *
 * @return
 *  - LE_BAD_PARAMETER The message reference is invalid.
 *  - LE_OVERFLOW      Identifier length exceed the maximum length.
 *  - LE_OK            Function succeeded.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_smsInbox2_GetSenderTel
(
    uint32_t msgId,
        ///< [IN] Message identifier.
    char* tel,
        ///< [OUT] Identifier string.
    size_t telSize
        ///< [IN]
);



//--------------------------------------------------------------------------------------------------
/**
 * Get the Message Time Stamp string (it does not apply for PDU message).
 *
 * @return
 *  - LE_BAD_PARAMETER The message reference is invalid.
 *  - LE_NOT_FOUND     The message is a PDU message.
 *  - LE_OVERFLOW      Timestamp number length exceed the maximum length.
 *  - LE_OK            Function succeeded.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_smsInbox2_GetTimeStamp
(
    uint32_t msgId,
        ///< [IN] Message identifier.
    char* timestamp,
        ///< [OUT] Message time stamp (for text or binary
        ///<  messages). String format:
        ///< "yy/MM/dd,hh:mm:ss+/-zz"
        ///< (Year/Month/Day,Hour:Min:Seconds+/-TimeZone)
    size_t timestampSize
        ///< [IN]
);



//--------------------------------------------------------------------------------------------------
/**
 * Get the message Length value.
 *
 * @return Number of characters for text messages, or the length of the data in bytes for raw
 *         binary and PDU messages.
 */
//--------------------------------------------------------------------------------------------------
size_t le_smsInbox2_GetMsgLen
(
    uint32_t msgId
        ///< [IN] Message identifier.
);



//--------------------------------------------------------------------------------------------------
/**
 * Get the text Message.
 *
 *
 * @return
 *  - LE_BAD_PARAMETER The message reference is invalid.
 *  - LE_FORMAT_ERROR  Message is not in text format.
 *  - LE_OVERFLOW      Message length exceed the maximum length.
 *  - LE_OK            Function succeeded.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_smsInbox2_GetText
(
    uint32_t msgId,
        ///< [IN] Message identifier.
    char* text,
        ///< [OUT] Message text.
    size_t textSize
        ///< [IN]
);



//--------------------------------------------------------------------------------------------------
/**
 * Get the binary Message.
 *
 *
 * @return
 *  - LE_BAD_PARAMETER The message reference is invalid.
 *  - LE_FORMAT_ERROR  Message is not in binary format.
 *  - LE_OVERFLOW      Message length exceed the maximum length.
 *  - LE_OK            Function succeeded.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_smsInbox2_GetBinary
(
    uint32_t msgId,
        ///< [IN] Message identifier.
    uint8_t* binPtr,
        ///< [OUT] Binary message.
    size_t* binSizePtr
        ///< [INOUT]
);



//--------------------------------------------------------------------------------------------------
/**
 * Get the PDU message.
 *
 * Output parameters are updated with the PDU message content and the length of the PDU message
 * in bytes.
 *
 * @return
 *  - LE_BAD_PARAMETER The message reference is invalid.
 *  - LE_FORMAT_ERROR  Unable to encode the message in PDU.
 *  - LE_OVERFLOW      Message length exceed the maximum length.
 *  - LE_OK            Function succeeded.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_smsInbox2_GetPdu
(
    uint32_t msgId,
        ///< [IN] Message identifier.
    uint8_t* pduPtr,
        ///< [OUT] PDU message.
    size_t* pduSizePtr
        ///< [INOUT]
);



//--------------------------------------------------------------------------------------------------
/**
 * Get the first Message object reference in the inbox message.
 *
 * @return
 *  - 0 No message found (message box parsing is over).
 *  - Message identifier.
 */
//--------------------------------------------------------------------------------------------------
uint32_t le_smsInbox2_GetFirst
(
    le_smsInbox2_SessionRef_t sessionRef
        ///< [IN] Mailbox session reference.
);



//--------------------------------------------------------------------------------------------------
/**
 * Get the next Message object reference in the inbox message.
 *
 * @return
 *  - 0 No message found (message box parsing is over).
 *  - Message identifier.
 */
//--------------------------------------------------------------------------------------------------
uint32_t le_smsInbox2_GetNext
(
    le_smsInbox2_SessionRef_t sessionRef
        ///< [IN] Mailbox session reference.
);



//--------------------------------------------------------------------------------------------------
/**
 * allow to know whether the message has been read or not. The message status is tied to the client
 * app.
 *
 * @return True if the message is unread, false otherwise.
 *
 * @note If the caller is passing a bad message reference into this function, it is a fatal error,
 *        the function will not return.
 */
//--------------------------------------------------------------------------------------------------
bool le_smsInbox2_IsUnread
(
    uint32_t msgId
        ///< [IN] Message identifier.
);



//--------------------------------------------------------------------------------------------------
/**
 * Mark a message as 'read'.
 *
 * @note If the caller is passing a bad message reference into this function, it is a fatal error,
 *        the function will not return.
 */
//--------------------------------------------------------------------------------------------------
void le_smsInbox2_MarkRead
(
    uint32_t msgId
        ///< [IN] Message identifier.
);



//--------------------------------------------------------------------------------------------------
/**
 * Mark a message as 'unread'.
 *
 * @note If the caller is passing a bad message reference into this function, it is a fatal error,
 *        the function will not return.
 */
//--------------------------------------------------------------------------------------------------
void le_smsInbox2_MarkUnread
(
    uint32_t msgId
        ///< [IN] Message identifier.
);



//--------------------------------------------------------------------------------------------------
/**
 * Set the maximum number of messages for message box.
 *
 * @return
 *  - LE_BAD_PARAMETER The message box name is invalid.
 *  - LE_OVERFLOW      Message count exceed the maximum limit.
 *  - LE_OK            Function succeeded.
 *  - LE_FAULT         Function failed.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_smsInbox2_SetMaxMessages
(
    uint32_t maxMessageCount
        ///< [IN] Maximum number of messages
);



//--------------------------------------------------------------------------------------------------
/**
 * Get the maximum number of messages for message box.
 *
 * @return
 *  - LE_BAD_PARAMETER The message box name is invalid.
 *  - LE_OK            Function succeeded.
 *  - LE_FAULT         Function failed.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_smsInbox2_GetMaxMessages
(
    uint32_t* maxMessageCountPtrPtr
        ///< [OUT] Maximum number of messages
);


#endif // LE_SMSINBOX2_INTERFACE_H_INCLUDE_GUARD