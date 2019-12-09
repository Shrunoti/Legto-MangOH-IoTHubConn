

/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */

/**
 * @page c_smsInbox SMS Inbox
 *
 * @ref le_smsInbox1_interface.h "API Reference"
 *
 * <HR>
 *
 * 3rd-party customer applications need to access to SMS messages at anytime without worrying how
 * the resources are managed.
 *
 * Legato provides an SMS Inbox service to allow apps to receive SMS messages through their own,
 * private message box, without:
 * - having to manage SIM or modem memory;
 * - conflicting with other applications that also receive SMS messages;
 * - missing messages while being updated or restarted.
 *
 *
 * The SMS Inbox Service handles the resource arbitration for the user app: the message
 * reception is always guaranteed, for example the user app doesn't have to worried about freeing
 * space in the SIM (or device's storage area) when it is full.
 *
 * In fact, at device's startup or when a SIM is inserted, the SIM content is copied into the "Inbox
 * Message Storage" folder allocated in the root file system of the device. Then, the process frees
 * automatically the SIM content.
 * Moreover, every new received SMS message is automatically copied into the "Inbox
 * Message Storage" folder and deleted from the SIM.
 * This mechanism keeps the SIM always empty in order to guarantee the reception of SMS messages.
 *
 * This process is the same when the SMS message storage is the device's storage area (ME - Mobile
 * Equipment).
 *
 * The message box is a persistent storage area. All files are saved into the file system in the
 * directory /mnt/flash/smsInbox.
 *
 * The creation of SMS inboxes is done based on the message box configuration settings
 * (cf. @subpage le_smsInbox_configdb section). This way, the message box contents will be kept up
 * to date automatically by the SMS Inbox Service, even when the user app is slow to start, is
 * stopped while it is being updated, or is being restarted to recover from a fault.
 *
 * A message box works as a circular buffer, when the message box is filled, the older messages
 * are deleted to free space for new messages. But, the application can also explicitly delete
 * messages if it doesn't need them anymore.
 *
 * @section le_smsInbox_binding IPC interfaces binding
 *
 * All the functions of this API are provided by the @b smsInboxService application service.
 *
 * Here's a code sample binding to SMS Inbox services:
 * @verbatim
   bindings:
   {
      clientExe.clientComponent.le_smsInbox1 -> smsInboxService.le_smsInbox1
   }
   @endverbatim
 *
 * @note By default, smsInboxService starts manually. To start it automatically, the user can remove
 * the option from the smsInboxService.adef file.
 *
 * A second message box (named le_smsInbox2) can be used by another application. These 2 message
 * boxes are used independently. All functions of this second message box are prefixed by
 * le_smsInbox2 (instead of le_msmInbox1).
 * The user can implement other message boxes based on le_smsInbox1 and le_smsInbox2 model.
 *
 * @section le_smsInbox_init Initialise a message box
 * Use the API le_smsInbox1_Open() to open a message box for access.
 *
 * @section le_smsInbox_receiving Receiving a message
 * To receive messages, register a handler function to obtain incoming
 * messages. Use le_smsInbox1_AddRxMessageHandler() to register that handler.
 *
 * The handler must satisfy the following prototype:
 *  @code
 * typedef void (*le_smsInbox1_RxMessageHandlerFunc_t)
 * (
 *   uint32_t msgId,
 *   void*    contextPtr
 * )
 * @endcode
 *
 * If a succession of messages is received, a new Message reference is created for each, and
 * the handler is called for each new message.
 *
 * Uninstall the handler function by calling le_smsInbox1_RemoveRxMessageHandler().
 *
 * @note le_smsInbox1_RemoveRxMessageHandler() function does not delete the Message Object.
 *       The caller has to delete it with le_smsInbox1_DeleteMsg().
 *
 * Use the following APIs to retrieve message information and data from the message object:
 * - le_smsInbox1_GetImsi() - get the IMSI of the message receiver SIM if it applies.
 * - le_smsInbox1_GetFormat() - determine if it is a binary or a text message.
 * - le_smsInbox1_GetSenderTel() - get the sender's Telephone number.
 * - le_smsInbox1_GetTimeStamp() - get the timestamp sets by the Service Center.
 * - le_smsInbox1_GetMsgLen() - get the message content length.
 * - le_smsInbox1_GetText() - get the message text.
 * - le_smsInbox1_GetBinary() - get the message binary content.
 * - le_smsInbox1_GetPdu() - get the PDU message content.
 *
 * @note For incoming SMS Inbox, format returned by le_smsInbox1_GetFormat() is never
 * LE_SMSINBOX1_FORMAT_PDU.
 *
 * @section le_smsInbox_listing Getting received messages
 *
 * Call le_smsInbox1_GetFirst() to get the first message from the inbox folder, and then call
 * le_smsInbox1_GetNext() to get the next message.
 *
 * Call le_smsInbox1_IsUnread() to know whether the message has been read or not. The message is
 * marked as "read" when one of those APIs is called: le_smsInbox1_GetImsi(),
 * le_smsInbox1_GetFormat(), le_smsInbox1_GetSenderTel(), le_smsInbox1_GetTimeStamp(),
 * le_smsInbox1_GetMsgLen(), le_smsInbox1_GetText(), le_smsInbox1_GetBinary(), le_smsInbox1_GetPdu().
 *
 * To finish, you can also modify the received status of a message with le_smsInbox1_MarkRead() and
 * le_smsInbox1_MarkUnread().
 *
 * @note The message status is tied to the client app.
 *
 * @section le_smsInbox1_deleting Deleting a message
 *
 * le_smsInbox1_DeleteMsg() deletes the message from the folder. Message is identified with
 * le_smsInbox1_MsgRef_t object. The function returns an error if the message is not found.
 *
 * @section le_smsInbox_end Close a message box
 * Use the API le_smsInbox1_Close() to close a message box (the message box is still exist and can be
 * re-opened and retrieve later all the messages).
 *
 * @section le_smsInbox_configdb Configuration tree
 *
 * @section le_smsInbox_MaxMessage() Messagebox Database Configuration
 *
 * Use le_smsInbox1_SetMaxMessages() to set the maximum number of messages for message box.
 * Use le_smsInbox1_GetMaxMessages() to get the maximum number of messages for message box.
 *
 * @copydoc le_smsInbox_configdbPage_Hide
 *
 * <HR>
 *
 * Copyright (C) Sierra Wireless Inc.
 */
/**
 * @interface le_smsInbox_configdbPage_Hide
 *
 * The configuration database for the SMS Inbox Service is stored in the @c smsInboxService config
 * tree:
 * @verbatim
   smsInboxService:/
       smsInbox/
           apps/
               appA<uint> = <# of messages>
               appB<uint> = <# of messages>
   @endverbatim
 *
 * Each app have its own inbox, containing its own list of messages. The maximum number of
 * messages is specified for each inbox. If the message box is not configured, a default number
 * of 10 messages is applied (only for message boxes configured in le_smsInbox_mboxName[]
 * area).
 *
 * The application name is given by the API name provided into the Components.cdef, both must be the
 * same.
 *
 * @todo: Give an indication of memory consumption for 1 message, including the file system overhead
 * (inode, directory entry, etc...).
 *
 *
 */
/**
 * @file le_smsInbox1_interface.h
 *
 * Legato @ref c_smsInbox include file.
 *
 * Copyright (C) Sierra Wireless Inc.
 */

#ifndef LE_SMSINBOX1_INTERFACE_H_INCLUDE_GUARD
#define LE_SMSINBOX1_INTERFACE_H_INCLUDE_GUARD


#include "legato.h"

// Interface specific includes
#include "le_mdmDefs_interface.h"
#include "le_sim_interface.h"
#include "le_sms_interface.h"


//--------------------------------------------------------------------------------------------------
/**
 * Type for handler called when a server disconnects.
 */
//--------------------------------------------------------------------------------------------------
typedef void (*le_smsInbox1_DisconnectHandler_t)(void *);

//--------------------------------------------------------------------------------------------------
/**
 *
 * Connect the current client thread to the service providing this API. Block until the service is
 * available.
 *
 * For each thread that wants to use this API, either ConnectService or TryConnectService must be
 * called before any other functions in this API.  Normally, ConnectService is automatically called
 * for the main thread, but not for any other thread. For details, see @ref apiFilesC_client.
 *
 * This function is created automatically.
 */
//--------------------------------------------------------------------------------------------------
void le_smsInbox1_ConnectService
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 *
 * Try to connect the current client thread to the service providing this API. Return with an error
 * if the service is not available.
 *
 * For each thread that wants to use this API, either ConnectService or TryConnectService must be
 * called before any other functions in this API.  Normally, ConnectService is automatically called
 * for the main thread, but not for any other thread. For details, see @ref apiFilesC_client.
 *
 * This function is created automatically.
 *
 * @return
 *  - LE_OK if the client connected successfully to the service.
 *  - LE_UNAVAILABLE if the server is not currently offering the service to which the client is
 *    bound.
 *  - LE_NOT_PERMITTED if the client interface is not bound to any service (doesn't have a binding).
 *  - LE_COMM_ERROR if the Service Directory cannot be reached.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_smsInbox1_TryConnectService
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * Set handler called when server disconnection is detected.
 *
 * When a server connection is lost, call this handler then exit with LE_FATAL.  If a program wants
 * to continue without exiting, it should call longjmp() from inside the handler.
 */
//--------------------------------------------------------------------------------------------------
void le_smsInbox1_SetServerDisconnectHandler
(
    le_smsInbox1_DisconnectHandler_t disconnectHandler,
    void *contextPtr
);

//--------------------------------------------------------------------------------------------------
/**
 *
 * Disconnect the current client thread from the service providing this API.
 *
 * Normally, this function doesn't need to be called. After this function is called, there's no
 * longer a connection to the service, and the functions in this API can't be used. For details, see
 * @ref apiFilesC_client.
 *
 * This function is created automatically.
 */
//--------------------------------------------------------------------------------------------------
void le_smsInbox1_DisconnectService
(
    void
);


//--------------------------------------------------------------------------------------------------
/**
 * Define unknown format
 */
//--------------------------------------------------------------------------------------------------
#define LE_SMSINBOX1_FORMAT_UNKNOWN -1

//--------------------------------------------------------------------------------------------------
/**
 * Define the name of length of the message box name
 */
//--------------------------------------------------------------------------------------------------
#define LE_SMSINBOX1_MAX_MBOX_NAME_LEN 12

//--------------------------------------------------------------------------------------------------
/**
 * Reference type for referring to open message box sessions.
 */
//--------------------------------------------------------------------------------------------------
typedef struct le_smsInbox1_Session* le_smsInbox1_SessionRef_t;


//--------------------------------------------------------------------------------------------------
/**
 * Reference type used by Add/Remove functions for EVENT 'le_smsInbox1_RxMessage'
 */
//--------------------------------------------------------------------------------------------------
typedef struct le_smsInbox1_RxMessageHandler* le_smsInbox1_RxMessageHandlerRef_t;


//--------------------------------------------------------------------------------------------------
/**
 * Handler for New Message.
 *
 */
//--------------------------------------------------------------------------------------------------
typedef void (*le_smsInbox1_RxMessageHandlerFunc_t)
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
le_smsInbox1_SessionRef_t le_smsInbox1_Open
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * Close a previously open message box.
 *
 */
//--------------------------------------------------------------------------------------------------
void le_smsInbox1_Close
(
    le_smsInbox1_SessionRef_t sessionRef
        ///< [IN] Mailbox session reference.
);

//--------------------------------------------------------------------------------------------------
/**
 * Add handler function for EVENT 'le_smsInbox1_RxMessage'
 *
 * This event provides information on new received messages.
 *
 */
//--------------------------------------------------------------------------------------------------
le_smsInbox1_RxMessageHandlerRef_t le_smsInbox1_AddRxMessageHandler
(
    le_smsInbox1_RxMessageHandlerFunc_t handlerPtr,
        ///< [IN]
    void* contextPtr
        ///< [IN]
);

//--------------------------------------------------------------------------------------------------
/**
 * Remove handler function for EVENT 'le_smsInbox1_RxMessage'
 */
//--------------------------------------------------------------------------------------------------
void le_smsInbox1_RemoveRxMessageHandler
(
    le_smsInbox1_RxMessageHandlerRef_t handlerRef
        ///< [IN]
);

//--------------------------------------------------------------------------------------------------
/**
 * Delete a Message.
 *
 * @note It is valid to delete a non-existent message.
 */
//--------------------------------------------------------------------------------------------------
void le_smsInbox1_DeleteMsg
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
le_result_t le_smsInbox1_GetImsi
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
le_sms_Format_t le_smsInbox1_GetFormat
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
le_result_t le_smsInbox1_GetSenderTel
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
le_result_t le_smsInbox1_GetTimeStamp
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
size_t le_smsInbox1_GetMsgLen
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
le_result_t le_smsInbox1_GetText
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
le_result_t le_smsInbox1_GetBinary
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
le_result_t le_smsInbox1_GetPdu
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
uint32_t le_smsInbox1_GetFirst
(
    le_smsInbox1_SessionRef_t sessionRef
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
uint32_t le_smsInbox1_GetNext
(
    le_smsInbox1_SessionRef_t sessionRef
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
bool le_smsInbox1_IsUnread
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
void le_smsInbox1_MarkRead
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
void le_smsInbox1_MarkUnread
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
le_result_t le_smsInbox1_SetMaxMessages
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
le_result_t le_smsInbox1_GetMaxMessages
(
    uint32_t* maxMessageCountPtrPtr
        ///< [OUT] Maximum number of messages
);

#endif // LE_SMSINBOX1_INTERFACE_H_INCLUDE_GUARD