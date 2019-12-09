

/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */

/**
 * @page c_sms SMS
 *
 * @ref le_sms_interface.h "API Reference"
 *
 * <HR>
 *
 * This file contains data structures and prototypes definitions for high level SMS APIs.
 *
 * SMS is a common way to communicate in the M2M world.
 *
 * It's an easy, fast way to send a small amount of data (e.g., sensor values for gas telemetry).
 * Usually, the radio module requests small power resources to send or receive a message.
 * It's often a good way to wake-up a device that was disconnected from the network or that was
 * operating in low power mode.
 *
 * @section le_sms_binding IPC interfaces binding
 *
 * All the functions of this API are provided by the @b modemService.
 *
 * Here's a code sample binding to modem services:
 * @verbatim
   bindings:
   {
      clientExe.clientComponent.le_sms -> modemService.le_sms
   }
   @endverbatim
 *
 * @section le_sms_ops_creating_msg Creating a Message object
 * There are 3 kinds of supported messages: text messages, binary messages, and PDU messages.
 *
 * You must create a Message object by calling @c le_sms_Create() before using the message
 * APIs. It automatically allocates needed resources for the Message object, which is referenced by
 * @c le_sms_MsgRef_t type.
 *
 * When the Message object is no longer needed, call @c le_sms_Delete() to free all
 *  allocated resources associated with the object.
 *
 * @section le_sms_ops_deleting_msg Deleting a Message object
 * To delete a Message object, call le_sms_Delete(). This frees all the
 * resources allocated for the Message object. If several users own the Message object
 * (e.g., several handler functions registered for SMS message reception), the
 * Message object will be deleted only after the last user deletes the Message object.
 *
 * @section le_sms_ops_sending Sending a message
 * To send a message, create an @c le_sms_MsgRef_t object by calling the
 * @c le_sms_Create() function. Then, set all the needed parameters for the message:
 * - Destination telephone number with le_sms_SetDestination();
 * -  Text content with le_sms_SetText(), the total length are set as well with this API, maximum
 * 160 characters as only the 7-bit alphabet is supported.
 * - Binary content with le_sms_SetBinary(), total length is set with this API, maximum 140 bytes.
 * - PDU content with le_sms_SetPDU(), total length is set with this API, max 36 (header) + 140
 *  (payload) bytes long.
 * - UCS2 content (16-bit format) with le_sms_SetUCS2(), total length is set with this API, maximum
 *  70 characters (140 bytes).
 *
 * - A specific timeout value can be used with le_sms_SetTimeout() API.
 *
 * After the Msg object is ready, call @c le_sms_Send().
 *
 * @c le_sms_Send() is a blocking function, it will return once the Modem has given back a
 * positive or negative answer to the sending operation. The return of @c le_sms_Send() API
 * provides definitive status of the sending operation.
 * TP-Validity-Period(TP-VP) parameter value indicates the time period for which the short message
 * is valid, i.e. for how long the Service Center (SC) shall guarantee its existence in the SC
 * memory before delivery to the recipient has been carried out. The default validity period(TP-VP)
 * is set to 7 days for MO SMS.
 *
 * When a message sending has failed and returned LE_FAULT, call le_sms_GetErrorCode() to retrieve
 * the 3GPP message error code or le_sms_Get3GPP2ErrorCode() to retrieve the 3GPP2 message error
 * code. If LE_SMS_ERROR_3GPP_PLATFORM_SPECIFIC or LE_SMS_ERROR_3GPP2_PLATFORM_SPECIFIC values is
 * returned, call le_sms_GetPlatformSpecificErrorCode() to retrieve the platform specific error
 * code.
 *
 * Please refer to @ref platformConstraintsSpecificErrorCodes for platform
 * specific error code description.
 *
 * Please refer to @ref c_smsSampleMO page to get an example of SMS message sending.
 *
 * @section le_sms_ops_async_sending Sending asynchronously a message
 *
 * To send an asynchronous message, le_sms_SendAsync() API can be called instead of le_sms_Send()
 *  and a specific timeout value can be used with le_sms_SetTimeout() API.
 * The default validity period(TP-VP) is set to 7 days for MO SMS.
 *
 * A text message can be sent with one simple function: le_sms_SendText(). You only have to pass
 *  the three following parameters:
 * - the destination telephone number.
 * - the text message, the total length are set as well with this function, maximum 160
 * characters as only the 7-bit alphabet is supported.
 * - the callback function to get a notification indicating the sending result: LE_SMS_SENT,
 *  LE_SMS_SENDING_FAILED or LE_SMS_SENDING_TIMEOUT.
 * The default validity period(TP-VP) is set to 7 days for MO SMS.
 *
 * A PDU message can be sent using the le_sms_SendPdu() functions. The parameters to give are:
 * - the PDU content, total length is set with this API, maximum 176 bytes long = 36 (header) +
 *  140 (payload).
 * - the callback function to get a notification indicating the sending result: LE_SMS_SENT,
 *  LE_SMS_SENDING_FAILED or LE_SMS_SENDING_TIMEOUT.
 * The default validity period(TP-VP) is set to 7 days for MO SMS.
 *
 * When a message sending has failed, call le_sms_GetErrorCode() to retrieve the 3GPP message error
 * code or le_sms_Get3GPP2ErrorCode() to retrieve the 3GPP2 message error code.
 * If LE_SMS_ERROR_3GPP_PLATFORM_SPECIFIC or LE_SMS_ERROR_3GPP2_PLATFORM_SPECIFIC values is
 * returned, call le_sms_GetPlatformSpecificErrorCode() to retrieve the platform specific error
 * code.
 *
 * Message object is never deleted regardless of the sending result. Caller has to
 * delete it. Message object once used for sending the message can not be reused to
 * send another message regardless of success or failure. New object has to be created
 * for new message.
 *
 * @section le_sms_ops_receiving Receiving a message
 * To receive SMS messages, register a handler function to obtain incoming
 * messages. Use @c le_sms_AddRxMessageHandler() to register that handler.
 *
 * The handler must satisfy the following prototype:
 * @c typedef void (*le_sms_RxMessageHandlerFunc_t)(le_sms_MsgRef_t msg).
 *
 * When a new incoming message is received, a Message object is automatically created and the
 * handler is called. This Message object is Read-Only, any calls of a le_sms_SetXXX API will
 * return a LE_NOT_PERMITTED error.
 *
 * Use the following APIs to retrieve message information and data from the Message
 * object:
 * - le_sms_GetFormat() - determine if it is a binary or a text message.
 * - le_sms_GetSenderTel() - get the sender's Telephone number.
 * - le_sms_GetTimeStamp() - get the timestamp sets by the Service Center.
 * - le_sms_GetUserdataLen() - get the message content (text, binary or UCS2) length.
 * - le_sms_GetPDULen() - get the PDU message length.
 * - le_sms_GetText() - get the message text.
 * - le_sms_GetUCS2() - get the UCS2 message content (16-bit format).
 * - le_sms_GetBinary() - get the message binary content.
 * - le_sms_GetPDU() - get the message PDU data.
 * - le_sms_GetType() - get the message type.
 *
 * @note  - If two (or more) registered handler functions exist, they are
 * all called and get a different message object reference.
 *
 * @note - For incoming SMS, if the returned format is LE_SMS_FORMAT_PDU, the PDU length can be
 * retrieved by calling le_sms_GetPDULen() and the content can be read by le_sms_GetPDU().
 *
 * If a succession of messages is received, a new Message object is created for each, and
 * the handler is called for each new message.
 *
 * Uninstall the handler function by calling @c le_sms_RemoveRxMessageHandler().
 * @note @c le_sms_RemoveRxMessageHandler() API does not delete the Message Object.
 *  The caller has to delete it.
 *
 * Please refer to @ref c_smsSampleMT page to get an example of SMS message reception handling.
 *
 * @section le_sms_ops_sms_storage Receiving a full SMS storage indication
 * To receive a SMS full storage status, the application has to register a handler function.
 * Use @c le_sms_AddFullStorageEventHandler() to register that handler.
 *
 * The handler must satisfy the following prototype:
 * @c typedef void (*le_sms_FullStorageEventFunc_t)(le_sms_Storage_t storage).
 *
 * Uninstall the handler function by calling @c le_sms_RemoveFullStorageEventHandler().
 *
 * Please refer to @ref c_smsSampleMT page to get an example of SMS storage indication
 * handling.
 *
 * @section le_sms_ops_listing Listing  messages recorded in storage area
 *
 * Call @c le_sms_CreateRxMsgList() to create a List object that lists the received
 * messages present in the storage area, which is referenced by @c le_sms_MsgListRef_t
 * type.
 *
 * If messages are not present, the le_sms_CreateRxMsgList() returns NULL.
 *
 * Once the list is available, call @c le_sms_GetFirst() to get the first
 * message from the list, and then call @c le_sms_GetNext() API to get the next message.
 *
 * Call @c le_sms_DeleteList() to free all allocated
 *  resources associated with the List object.
 *
 * Call @c le_sms_GetStatus() to read the status of a message (Received
 * Read, Received Unread).
 *
 * To finish, you can also modify the received status of a message with
 * @c le_sms_MarkRead() and @c le_sms_MarkUnread().
 *
 * @section le_sms_ops_deleting Deleting a message from the storage area
 *
 * @c le_sms_DeleteFromStorage() deletes the message from the storage area. Message is
 * identified with @c le_sms_MsgRef_t object. The API returns an error if the deletion cannot be
 * performed or if it is a broadcast or a non stored message.
 *
 * @note If several users own the Message object on new reception
 * (e.g., several handler functions registered for SMS message reception), the
 * Message will be deleted from the storage area only after the last user deletes
 * the Message object reference (not necessary from storage). API returns always LE_OK in this case.
 *
 * @note If one client creates a list and deletes all sms from storage, other clients won’t see sms
 *  stored If they have not created a sms list too. Sms List creation locks and
 *   delays sms deletion from storage until all references have been deleted.
 *
 * @section le_sms_ops_broadcast SMS Cell Broadcast
 *
 * The Cell Broadcast service permits a number of unacknowledged general messages to be broadcast
 * to all receivers within a particular region. Cell Broadcast messages are broadcast to defined
 * geographical areas known as cell broadcast areas. These areas may comprise of one or more cells,
 * or may comprise the entire PLMN.
 *
 * GSM or UMTS SMS cell broadcast service can be activated or deactivated with
 * le_sms_ActivateCellBroadcast() and le_sms_DeactivateCellBroadcast() APIs.
 *
 * CDMA cell broadcast service can be activated or deactivated with
 * le_sms_ActivateCdmaCellBroadcast() and le_sms_DeactivateCdmaCellBroadcast() APIs.
 *
 * Cell broadcast message receptions are notify by the SMS handler like a SMS message reception,
 * but there are neither stored in SIM nor in the modem. So le_sms_DeleteFromStorage()
 * can't be used but the message reference shall be delete with le_sms_Delete().
 *
 * - le_sms_GetFormat() - determine if it is a binary or a text message.
 * - le_sms_GetUserdataLen() - get the message content (text, binary or UCS2) length.
 * - le_sms_GetPDULen() - get the PDU message received length.
 * - le_sms_GetText() - get the message text.
 * - le_sms_GetBinary() - get the message binary content.
 * - le_sms_GetUCS2() - get the UCS2 message content (16-bit format).
 * - le_sms_GetPDU() - get the message PDU data received length.
 * - le_sms_GetCellBroadcastId() - get the message identifier received (3GPP 23.41).
 * - le_sms_GetCellBroadcastSerialNumber() get the message Serial Number received (3GPP 23.41).
 *
 * A sample code that implements a function for SMS Cell Broadcast reception can be found in
 * \b smsCBTest.c file (please refer to @ref c_smsCbSample page).
 *
 * @b Serial @b Number
 *
 * Cell Broadcast Serial Number parameter is a 16-bit integer which identifies a particular
 * CBS message from the source and type indicated by the Message Identifier and is altered every
 * time the CBS message with a given Message Identifier is changed.
 *
 * The two bytes of the Serial Number field are divided into a 2-bit Geographical Scope (GS)
 * indicator, a 10-bit Message Code and a 4-bit Update Number as shown below:
 *
 * - GS code (bit 14 and 15): The Geographical Scope (GS) indicates the geographical area over
 * which the Message Code is unique, and the display mode.
 *
 * - Message Code (bit 4 to 13) : The Message Code differentiates between CBS messages from
 * the same source and type (i.e. with the same Message Identifier). Message Codes are for
 * allocation by PLMN operators. The Message Code identifies different message themes.
 * For example, let the value for the Message Identifier be "Automotive Association" (= source),
 * "Traffic Reports" (= type). Then "Crash on A1 J5" could be one value for the message code,
 * "Cow on A32 J4" could be another, and "Slow vehicle on M3 J3" yet another.
 *
 * - Update Number (bit 0 to 3) : The Update Number indicates a change of the message content of
 * the same CBS message, i.e. the CBS message with the same Message Identifier, Geographical
 * Scope, and Message Code.
 *
 * Serial Number fields meaning are defined in the 3GPP 23.041 (9.4.1.2.1 Serial Number).
 *
 * @b Message @b Identifier
 *
 * Message Identifier parameter identifies the source and type of the CBS message. For example,
 * "Automotive Association" (= source), "Traffic Reports" (= type) could correspond to one value.
 * A number of CBS messages may originate from the same source and/or be of the same type.
 * These will be distinguished by the Serial Number.
 *
 * Message identifier meaning ranges are defined in the 3GPP 23.041 (9.4.1.2.2 Message Identifier).
 *
 * @section le_sms_ops_broadcast_configuration SMS Cell Broadcast configuration
 *
 * GSM or UMTS Cell broadcast Message identifiers range can be added or removed with
 *  le_sms_AddCellBroadcastIds() and le_sms_RemoveCellBroadcastIds() APIs. All Message identifiers
 *  can be removed with le_sms_ClearCellBroadcastIds() API.
 *
 * CDMA Cell broadcast Service categories can be added or removed with
 *  le_sms_AddCdmaCellBroadcastServices() and le_sms_RemoveCdmaCellBroadcastServices() APIs. All
 * Service categories can be removed with le_sms_ClearCdmaCellBroadcastServices() API.
 *
 * @section le_sms_ops_statusReport SMS Status Report
 *
 * SMS Status Report may be sent by the SMS Center (SMSC) to inform the originating device about the
 * final outcome of the message delivery.
 *
 * SMS Status Report can be activated or deactivated for outgoing messages with
 * le_sms_EnableStatusReport() and le_sms_DisableStatusReport(). The current activation state can
 * be retrieved with le_sms_IsStatusReportEnabled().
 *
 * The reception of a SMS Status Report is notified by the SMS handler like a SMS message reception,
 * but the message is neither stored in SIM nor in the modem. So le_sms_DeleteFromStorage()
 * can't be used, but the message reference shall be delete with le_sms_Delete(). Received SMS
 * Status Reports are identified by a specific type: @ref LE_SMS_TYPE_STATUS_REPORT.
 *
 * The different elements of the SMS Status Report can be retrieved with the following APIs:
 * - le_sms_GetTpMr() gives the Message Reference, defined in 3GPP TS 23.040 section 9.2.3.6.
 * - le_sms_GetTpRa() gives the Recipient Address, defined in 3GPP TS 23.040 section 9.2.3.14, and
 * the Recipient Address Type of Address, defined in 3GPP TS 24.011 section 8.2.5.2.
 * - le_sms_GetTpScTs() gives the Service Centre Time Stamp, defined in 3GPP TS 23.040
 * section 9.2.3.11.
 * - le_sms_GetTpDt() gives the Discharge Time, defined in 3GPP TS 23.040 section 9.2.3.13.
 * - le_sms_GetTpSt() gives the Status, defined in 3GPP TS 23.040 section 9.2.3.15.
 *
 * @section le_sms_ops_configuration SMS configuration
 *
 *  Modem SMS Center Address can be set or get with le_sms_SetSmsCenterAddress() and
 *   le_sms_GetSmsCenterAddress() functions
 *
 * @section le_sms_ops_storage_configuration Preferred SMS storage configuration
 *
 *  Preferred SMS storage for incoming messages can be set or get with le_sms_SetPreferredStorage()
 *  and le_sms_GetPreferredStorage() functions.
 *
 * @section le_sms_ops_statistics SMS statistics
 *
 * The number of SMS successfully sent or received through the Legato API can be counted.
 * This feature is activated by default. le_sms_GetCount() allows retrieving the message count
 * for each SMS type (cf. @ref le_sms_Type_t).
 *
 * le_sms_StopCount() stops the message counting and le_sms_StartCount() restarts it.
 * le_sms_ResetCount() can be used to reset the message counters.
 *
 * @note The activation state of this feature is persistent even after a reboot of the platform.
 *
 * @section le_sms_ops_samples Sample codes
 * A sample code that implements a function for Mobile Originated SMS message can be found in
 * \b smsMO.c file (please refer to @ref c_smsSampleMO page).
 *
 * A sample code that implements a function for Mobile Terminated SMS message can be found in
 * \b smsMT.c file (please refer to @ref c_smsSampleMT page).
 *
 * These two samples can be easily compiled and run into the \b sms app, to install and use
 * this app:
 *
 *   @verbatim
   $ make ar7
   $ bin/instapp  build/ar7/bin/samples/sms.ar7 <ipaddress>
   @endverbatim
 * where ipaddress is the address of your target device.
 *
 * Then on your target, just run:
 *   @verbatim
   $ app start sms
   @endverbatim
 *
 * The sample replies to the sender by the message "Message from <phone number> received". Where
 * "phone number" is the sender's phone number.
 *
 * Sample code for that application can be seen in the following pages:
 * - @subpage c_smsSampleMO <br>
 * - @subpage c_smsSampleMT
 *
 * <HR>
 *
 * Copyright (C) Sierra Wireless Inc.
 */
/**
 * @file le_sms_interface.h
 *
 * Legato @ref c_sms include file.
 *
 * Copyright (C) Sierra Wireless Inc.
 */
/**
 * @page c_smsSampleMO Sample code for Mobile Originated SMS message
 *
 * @include "apps/sample/sms/smsClient/smsMO.c"
 */
/**
 * @page c_smsSampleMT Sample code for Mobile Terminated SMS message
 *
 * @include "apps/sample/sms/smsClient/smsMT.c"
 */
/**
 * @page c_smsCbSample Sample code for SMS Cell Broadcast reception
 *
 * @include "apps/test/modemServices/sms/smsIntegrationTest/smsCBTest/smsCBTest.c"
 */

#ifndef LE_SMS_INTERFACE_H_INCLUDE_GUARD
#define LE_SMS_INTERFACE_H_INCLUDE_GUARD


#include "legato.h"

// Interface specific includes
#include "le_mdmDefs_interface.h"


//--------------------------------------------------------------------------------------------------
/**
 * Type for handler called when a server disconnects.
 */
//--------------------------------------------------------------------------------------------------
typedef void (*le_sms_DisconnectHandler_t)(void *);

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
void le_sms_ConnectService
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
le_result_t le_sms_TryConnectService
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
void le_sms_SetServerDisconnectHandler
(
    le_sms_DisconnectHandler_t disconnectHandler,
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
void le_sms_DisconnectService
(
    void
);


//--------------------------------------------------------------------------------------------------
/**
 * Time stamp string length.
 * The string format is "yy/MM/dd,hh:mm:ss+/-zz" (Year/Month/Day,Hour:Min:Seconds+/-TimeZone).
 * One extra byte is added for the null character.
 */
//--------------------------------------------------------------------------------------------------
#define LE_SMS_TIMESTAMP_MAX_LEN 20

//--------------------------------------------------------------------------------------------------
/**
 * Time stamp string length (including the null-terminator).
 */
//--------------------------------------------------------------------------------------------------
#define LE_SMS_TIMESTAMP_MAX_BYTES 21

//--------------------------------------------------------------------------------------------------
/**
 * The text message can be up to 160 characters long.
 * One extra byte is added for the null character.
 */
//--------------------------------------------------------------------------------------------------
#define LE_SMS_TEXT_MAX_LEN 160

//--------------------------------------------------------------------------------------------------
/**
 * Test message string length (including the null-terminator).
 */
//--------------------------------------------------------------------------------------------------
#define LE_SMS_TEXT_MAX_BYTES 161

//--------------------------------------------------------------------------------------------------
/**
 * The raw binary message can be up to 140 bytes long.
 */
//--------------------------------------------------------------------------------------------------
#define LE_SMS_BINARY_MAX_BYTES 140

//--------------------------------------------------------------------------------------------------
/**
 * The UCS2 message can be up to 140 bytes long (70 characters).
 */
//--------------------------------------------------------------------------------------------------
#define LE_SMS_UCS2_MAX_BYTES 140

//--------------------------------------------------------------------------------------------------
/**
 * The UCS2 message can be up to 70 characters (140 bytes long).
 */
//--------------------------------------------------------------------------------------------------
#define LE_SMS_UCS2_MAX_CHARS 70

//--------------------------------------------------------------------------------------------------
/**
 * The PDU payload bytes long.
 */
//--------------------------------------------------------------------------------------------------
#define LE_SMS_PDU_MAX_PAYLOAD 140

//--------------------------------------------------------------------------------------------------
/**
 * The PDU message can be up to 36 (header) + 140 (payload) bytes long.
 */
//--------------------------------------------------------------------------------------------------
#define LE_SMS_PDU_MAX_BYTES 176

//--------------------------------------------------------------------------------------------------
/**
 * Message Format.
 */
//--------------------------------------------------------------------------------------------------
typedef enum
{
    LE_SMS_FORMAT_PDU = 0,
        ///< PDU message format.
    LE_SMS_FORMAT_TEXT = 1,
        ///< Text message format.
    LE_SMS_FORMAT_BINARY = 2,
        ///< Binary message format.
    LE_SMS_FORMAT_UCS2 = 3,
        ///< UCS2 message format.
    LE_SMS_FORMAT_UNKNOWN = 4
        ///< Unknown message format.
}
le_sms_Format_t;


//--------------------------------------------------------------------------------------------------
/**
 * Message Type.
 */
//--------------------------------------------------------------------------------------------------
typedef enum
{
    LE_SMS_TYPE_RX = 0,
        ///< SMS mobile terminated message.
    LE_SMS_TYPE_TX = 1,
        ///< SMS mobile originated message.
    LE_SMS_TYPE_BROADCAST_RX = 2,
        ///< SMS Cell Broadcast message.
    LE_SMS_TYPE_STATUS_REPORT = 3
        ///< SMS Status Report.
}
le_sms_Type_t;


//--------------------------------------------------------------------------------------------------
/**
 * Message Status.
 */
//--------------------------------------------------------------------------------------------------
typedef enum
{
    LE_SMS_RX_READ = 0,
        ///< Message present in the message storage has been read.
    LE_SMS_RX_UNREAD = 1,
        ///< Message present in the message storage has not been read.
    LE_SMS_STORED_SENT = 2,
        ///< Message saved in the message storage has been sent.
    LE_SMS_STORED_UNSENT = 3,
        ///< Message saved in the message storage has not been sent.
    LE_SMS_SENT = 4,
        ///< Message has been sent.
    LE_SMS_SENDING = 5,
        ///< Message has been in the sending pool.
    LE_SMS_UNSENT = 6,
        ///< Message has not been sent.
    LE_SMS_SENDING_FAILED = 7,
        ///< Message sending has Failed.
    LE_SMS_SENDING_TIMEOUT = 8,
        ///< Message sending has Failed due to timeout.
    LE_SMS_STATUS_UNKNOWN = 9
        ///< Unknown message status.
}
le_sms_Status_t;


//--------------------------------------------------------------------------------------------------
/**
 * CDMA Cell broadcast message languages.
 */
//--------------------------------------------------------------------------------------------------
typedef enum
{
    LE_SMS_LANGUAGE_UNKNOWN = 0,
        ///< Unknow or Unspecified language.
    LE_SMS_LANGUAGE_ENGLISH = 1,
        ///< English language.
    LE_SMS_LANGUAGE_FRENCH = 2,
        ///< French language.
    LE_SMS_LANGUAGE_SPANISH = 3,
        ///< Spanish language.
    LE_SMS_LANGUAGE_JAPANESE = 4,
        ///< Japanese language.
    LE_SMS_LANGUAGE_KOREAN = 5,
        ///< Korean language.
    LE_SMS_LANGUAGE_CHINESE = 6,
        ///< Chinese language.
    LE_SMS_LANGUAGE_HEBREW = 7,
        ///< Hebrew language.
    LE_SMS_LANGUAGE_MAX = 8
        ///<
}
le_sms_Languages_t;


//--------------------------------------------------------------------------------------------------
/**
 * CDMA Cell broadcast Service Categories.
 */
//--------------------------------------------------------------------------------------------------
typedef enum
{
    LE_SMS_CDMA_SVC_CAT_UNKNOWN = 0,
        ///< Unknown or Unspecified.
    LE_SMS_CDMA_SVC_CAT_EMERGENCY_BROADCAST = 1,
        ///< Emergency Broadcast.
    LE_SMS_CDMA_SVC_CAT_ADMINISTRATIVE = 2,
        ///< Administrative.
    LE_SMS_CDMA_SVC_CAT_MAINTENANCE = 3,
        ///< Maintenance.
    LE_SMS_CDMA_SVC_CAT_GENERAL_NEWS_LOCAL = 4,
        ///< General News Local.
    LE_SMS_CDMA_SVC_CAT_GENERAL_NEWS_REGIONAL = 5,
        ///< General News Regional.
    LE_SMS_CDMA_SVC_CAT_GENERAL_NEWS_NATIONAL = 6,
        ///< General News National.
    LE_SMS_CDMA_SVC_CAT_GENERAL_NEWS_INTERNATIONAL = 7,
        ///< General News International.
    LE_SMS_CDMA_SVC_CAT_BUSINESS_NEWS_LOCAL = 8,
        ///< Business News Local.
    LE_SMS_CDMA_SVC_CAT_BUSINESS_NEWS_REGIONAL = 9,
        ///< Business News Regional.
    LE_SMS_CDMA_SVC_CAT_BUSINESS_NEWS_NATIONAL = 10,
        ///< Business News National.
    LE_SMS_CDMA_SVC_CAT_BUSINESS_NEWS_INTERNATIONAL = 11,
        ///< Business News International.
    LE_SMS_CDMA_SVC_CAT_SPORTS_NEWS_LOCAL = 12,
        ///< Sports News Local.
    LE_SMS_CDMA_SVC_CAT_SPORTS_NEWS_REGIONAL = 13,
        ///< Sports News Regional.
    LE_SMS_CDMA_SVC_CAT_SPORTS_NEWS_NATIONAL = 14,
        ///< Sports News National.
    LE_SMS_CDMA_SVC_CAT_SPORTS_NEWS_INTERNATIONAL = 15,
        ///< Sports News International.
    LE_SMS_CDMA_SVC_CAT_ENTERTAINMENT_NEWS_LOCAL = 16,
        ///< Entertainment News Local.
    LE_SMS_CDMA_SVC_CAT_ENTERTAINMENT_NEWS_REGIONAL = 17,
        ///< Entertainment News Regional.
    LE_SMS_CDMA_SVC_CAT_ENTERTAINMENT_NEWS_NATIONAL = 18,
        ///< Entertainment News National.
    LE_SMS_CDMA_SVC_CAT_ENTERTAINMENT_NEWS_INTERNATIONAL = 19,
        ///< Entertainment News International.
    LE_SMS_CDMA_SVC_CAT_LOCAL_WEATHER = 20,
        ///< Local weather.
    LE_SMS_CDMA_SVC_CAT_TRAFFIC_REPORTS = 21,
        ///< Area Traffic Reports.
    LE_SMS_CDMA_SVC_CAT_LOCAL_FLIGHT_SCHEDULES = 22,
        ///< Local Airplane Flight Schedules.
    LE_SMS_CDMA_SVC_CAT_RESTAURANTS = 23,
        ///< Restaurants.
    LE_SMS_CDMA_SVC_CAT_LODGINGS = 24,
        ///< Lodgings.
    LE_SMS_CDMA_SVC_CAT_RETAIL_DIRECTORY = 25,
        ///< Retail Directory.
    LE_SMS_CDMA_SVC_CAT_ADVERTISEMENTS = 26,
        ///< Advertisements.
    LE_SMS_CDMA_SVC_CAT_STOCK_QUOTES = 27,
        ///< Stock Quotes.
    LE_SMS_CDMA_SVC_CAT_EMPLOYMENT_OPPORTUNITIES = 28,
        ///< Employment Opportunities.
    LE_SMS_CDMA_SVC_CAT_MEDICAL = 29,
        ///< Medical/Health/Hospitals.
    LE_SMS_CDMA_SVC_CAT_TECHNOLOGY_NEWS = 30,
        ///< Technology News.
    LE_SMS_CDMA_SVC_CAT_MULTI_CAT = 31,
        ///< Multicategory.
    LE_SMS_CDMA_SVC_CAT_CATPT = 32,
        ///< Card Application Toolkit Protocol Teleservice.
    LE_SMS_CDMA_SVC_CAT_MAX = 33
        ///<
}
le_sms_CdmaServiceCat_t;


//--------------------------------------------------------------------------------------------------
/**
 *  SMS storage area.
 */
//--------------------------------------------------------------------------------------------------
typedef enum
{
    LE_SMS_STORAGE_NV = 0,
        ///< Non volatile memory storage.
    LE_SMS_STORAGE_SIM = 1,
        ///< Sim SMS storage.
    LE_SMS_STORAGE_MAX = 2
        ///< Undefined storage.
}
le_sms_Storage_t;


//--------------------------------------------------------------------------------------------------
/**
 * 3GPP2 Message Error code when the message sending has failed.
 */
//--------------------------------------------------------------------------------------------------
typedef enum
{
    LE_SMS_ERROR_ADDR_VACANT = 0,
        ///< The SMS Destination Address is valid but is not.
        ///<  currently allocated to an SMS terminal.
    LE_SMS_ERROR_ADDR_TRANSLATION_FAILURE = 1,
        ///< The SMS Destination Address is invalid.
    LE_SMS_ERROR_NETWORK_RESOURCE_SHORTAGE = 2,
        ///< The network transmission failed due to lack of
        ///<  a network resource or link capacity.
    LE_SMS_ERROR_NETWORK_FAILURE = 3,
        ///< A network node failed, a link failed, or a
        ///< required operation failed.
    LE_SMS_ERROR_INVALID_TELESERVICE_ID = 4,
        ///< The SMS_TeleserviceIdentifier is not known, is
        ///<  not supported, or is not authorized by an
        ///<  addressed functional entity.
    LE_SMS_ERROR_NETWORK_OTHER = 5,
        ///< A network problem other than identified above.
    LE_SMS_ERROR_NO_PAGE_RESPONSE = 6,
        ///< The addressed MS-based SME is known, but it
        ///<  does not respond to a page.
    LE_SMS_ERROR_DEST_BUSY = 7,
        ///< The destination MS-based SME is SMS capable,
        ///<  but is currently engaged in a call, a service,
        ///<  or a call mode that precludes the use of SMS,
        ///<  or the destination SME is congested.
    LE_SMS_ERROR_NO_ACK = 8,
        ///< The destination SME does not acknowledge receipt
        ///<  of the SMS delivery.
    LE_SMS_ERROR_DEST_RESOURCE_SHORTAGE = 9,
        ///< A required terminal resource is not available to
        ///<  process this message.
    LE_SMS_ERROR_SMS_DELIVERY_POSTPONED = 10,
        ///< Delivery is not currently possible.
    LE_SMS_ERROR_DEST_OUT_OF_SERV = 11,
        ///< The addressed destination is out of
        ///<  service for an extended period of time.
    LE_SMS_ERROR_DEST_NOT_AT_ADDR = 12,
        ///< The MS-based SME is no longer at the
        ///< temporary SMS routing address.
    LE_SMS_ERROR_DEST_OTHER = 13,
        ///< A terminal problem other than described above.
    LE_SMS_ERROR_RADIO_IF_RESOURCE_SHORTAGE = 14,
        ///< There is no channel available or there is
        ///< radio congestion at this time.
    LE_SMS_ERROR_RADIO_IF_INCOMPATABILITY = 15,
        ///< The MS for an MS-based SME is operating in a
        ///<  mode that does not support SMS at this time.
    LE_SMS_ERROR_RADIO_IF_OTHER = 16,
        ///< A radio interface problem to an MS-based SME
        ///<  other than described above.
    LE_SMS_ERROR_ENCODING = 17,
        ///< The size of a parameter or field is not
        ///<  what is expected.
    LE_SMS_ERROR_SMS_ORIG_DENIED = 18,
        ///< The originating MIN is not recognized.
    LE_SMS_ERROR_SMS_TERM_DENIED = 19,
        ///< The destination is not authorized to receive
        ///<  the SMS message.
    LE_SMS_ERROR_SUPP_SERV_NOT_SUPP = 20,
        ///< The originating supplementary service is
        ///<  not known or supported.
    LE_SMS_ERROR_SMS_NOT_SUPP = 21,
        ///< The originating supplementary service is
        ///<  not known or supported.
    LE_SMS_ERROR_MISSING_EXPECTED_PARAM = 22,
        ///< An optional parameter that is required
        ///<  for a particular function.
    LE_SMS_ERROR_MISSING_MAND_PARAM = 23,
        ///< A parameter is missing that is mandatory.
        ///< for a particular message.
    LE_SMS_ERROR_UNRECOGNIZED_PARAM_VAL = 24,
        ///< A known parameter has an unknown or
        ///<  unsupported value.
    LE_SMS_ERROR_UNEXPECTED_PARAM_VAL = 25,
        ///< A known parameter has a known but unexpected
        ///<  value.
    LE_SMS_ERROR_USER_DATA_SIZE_ERR = 26,
        ///< The User Data size is too large for access
        ///< technology, transport network, or call
        ///< mode, etc
    LE_SMS_ERROR_GENERAL_OTHER = 27,
        ///< Other general problems.
    LE_SMS_ERROR_3GPP2_PLATFORM_SPECIFIC = 28,
        ///< Platform specific code.
    LE_SMS_ERROR_3GPP2_MAX = 29
        ///< Undefined reason.
}
le_sms_ErrorCode3GPP2_t;


//--------------------------------------------------------------------------------------------------
/**
 * Message Error code when the message sending has failed.
 */
//--------------------------------------------------------------------------------------------------
typedef enum
{
    LE_SMS_RP_ERROR_UNASSIGNED_NUMBER = 0,
        ///< Unassigned (unallocated) number
    LE_SMS_RP_ERROR_OPERATOR_DETERMINED_BARRING = 1,
        ///< Operator determined barring
    LE_SMS_RP_ERROR_CALL_BARRED = 2,
        ///< Call barred
    LE_SMS_RP_ERROR_RESERVED = 3,
        ///< Reserved
    LE_SMS_RP_ERROR_SMS_TRANSFER_REJECTED = 4,
        ///< Short message transfer rejected
    LE_SMS_RP_ERROR_MEMORY_CAP_EXCEEDED = 5,
        ///< Memory capacity exceeded
    LE_SMS_RP_ERROR_DESTINATION_OUT_OF_ORDER = 6,
        ///< Destination out of order
    LE_SMS_RP_ERROR_UNIDENTIFIED_SUBSCRIBER = 7,
        ///< Unidentified subscriber
    LE_SMS_RP_ERROR_FACILITY_REJECTED = 8,
        ///< Facility rejected
    LE_SMS_RP_ERROR_UNKNOWN_SUBSCRIBER = 9,
        ///< Unknown subscriber
    LE_SMS_RP_ERROR_NETWORK_OUT_OF_ORDER = 10,
        ///< Network out of order
    LE_SMS_RP_ERROR_TEMPORARY_FAILURE = 11,
        ///< Temporary failure
    LE_SMS_RP_ERROR_CONGESTION = 12,
        ///< Congestion
    LE_SMS_RP_ERROR_RESOURCES_UNAVAILABLE = 13,
        ///< Resources unavailable, unspecified
    LE_SMS_RP_ERROR_REQUESTED_FACILITY_NOT_SUBSCRIBED = 14,
        ///< Resources facility not subscribed
    LE_SMS_RP_ERROR_REQUESTED_FACILITY_NOT_IMPLEMENTED = 15,
        ///< Resources facility not implemented
    LE_SMS_RP_ERROR_INVALID_SMS_TRANSFER_REFERENCE_VALUE = 16,
        ///< Invalid short message transfer
        ///<  reference value
    LE_SMS_RP_ERROR_SEMANTICALLY_INCORRECT_MESSAGE = 17,
        ///< Sementically incorect message
    LE_SMS_RP_ERROR_INVALID_MANDATORY_INFO = 18,
        ///< Invalid mandatory information
    LE_SMS_RP_ERROR_MESSAGE_TYPE_NOT_IMPLEMENTED = 19,
        ///< Message type nonexistent or not implemented
    LE_SMS_RP_ERROR_MESSAGE_NOT_COMPATABLE_WITH_SMS = 20,
        ///< Message not compatible with short message
        ///<  protocol state
    LE_SMS_RP_ERROR_INFO_ELEMENT_NOT_IMPLEMENTED = 21,
        ///< Information element nonexistent
        ///<   or not implemented
    LE_SMS_RP_ERROR_PROTOCOL_ERROR = 22,
        ///< Protocol error, unspecified
    LE_SMS_RP_ERROR_INTERWORKING = 23,
        ///< Interworking, unspecified
    LE_SMS_TP_ERROR_TELE_INTERWORKING_NOT_SUPPORTED = 24,
        ///< Telematic interworking not supported
    LE_SMS_TP_ERROR_SHORT_MESSAGE_TYPE_0_NOT_SUPPORTED = 25,
        ///< Short Message Type 0 not supported
    LE_SMS_TP_ERROR_SHORT_MESSAGE_CANNOT_BE_REPLACED = 26,
        ///< Cannot replace short message
    LE_SMS_TP_ERROR_UNSPECIFIED_PID_ERROR = 27,
        ///< Unspecified TP-PID error
    LE_SMS_TP_ERROR_DCS_NOT_SUPPORTED = 28,
        ///< Data coding scheme (alphabet)
        ///<  not supported
    LE_SMS_TP_ERROR_MESSAGE_CLASS_NOT_SUPPORTED = 29,
        ///< Message class not supported
    LE_SMS_TP_ERROR_UNSPECIFIED_DCS_ERROR = 30,
        ///< Unspecified TP-DCS error
    LE_SMS_TP_ERROR_COMMAND_CANNOT_BE_ACTIONED = 31,
        ///< Command cannot be actioned
    LE_SMS_TP_ERROR_COMMAND_UNSUPPORTED = 32,
        ///< Command unsupported
    LE_SMS_TP_ERROR_UNSPECIFIED_COMMAND_ERROR = 33,
        ///< Unspecified TP-Command error
    LE_SMS_TP_ERROR_TPDU_NOT_SUPPORTED = 34,
        ///< TPDU not supported
    LE_SMS_TP_ERROR_SC_BUSY = 35,
        ///< SC busy
    LE_SMS_TP_ERROR_NO_SC_SUBSCRIPTION = 36,
        ///< No SC subscription
    LE_SMS_TP_ERROR_SC_SYS_FAILURE = 37,
        ///< SC system failure
    LE_SMS_TP_ERROR_INVALID_SME_ADDRESS = 38,
        ///< Invalid SME address
    LE_SMS_TP_ERROR_DESTINATION_SME_BARRED = 39,
        ///< Destination SME barred
    LE_SMS_TP_ERROR_SM_REJECTED_OR_DUPLICATE = 40,
        ///< SM Rejected-Duplicate SM
    LE_SMS_TP_ERROR_TP_VPF_NOT_SUPPORTED = 41,
        ///< TP-VPF not supported
    LE_SMS_TP_ERROR_TP_VP_NOT_SUPPORTED = 42,
        ///< TP-VP not supporte
    LE_SMS_TP_ERROR_SIM_SMS_STORAGE_FULL = 43,
        ///< (U)SIM SMS storage full
    LE_SMS_TP_ERROR_NO_SMS_STORAGE_CAP_IN_SIM = 44,
        ///< No SMS storage capability in (U)SIM
    LE_SMS_TP_ERROR_MS_ERROR = 45,
        ///< Error in MS
    LE_SMS_TP_ERROR_MEMORY_CAP_EXCEEDED = 46,
        ///< Memory capacity exceeded
    LE_SMS_TP_ERROR_SIM_APP_TOOLKIT_BUSY = 47,
        ///< (U)SIM Application Toolkit busy
    LE_SMS_TP_ERROR_SIM_DATA_DOWNLOAD_ERROR = 48,
        ///< (U)SIM data download error
    LE_SMS_TP_ERROR_UNSPECIFIED_ERROR = 49,
        ///< Unspecified error cause
    LE_SMS_ERROR_3GPP_PLATFORM_SPECIFIC = 50,
        ///< Platform specific code.
    LE_SMS_ERROR_3GPP_MAX = 51
        ///< Undefined reason.
}
le_sms_ErrorCode_t;


//--------------------------------------------------------------------------------------------------
/**
 * Declare a reference type for referring to SMS Message objects.
 */
//--------------------------------------------------------------------------------------------------
typedef struct le_sms_Msg* le_sms_MsgRef_t;


//--------------------------------------------------------------------------------------------------
/**
 * Opaque type for SMS Message Listing.
 */
//--------------------------------------------------------------------------------------------------
typedef struct le_sms_MsgList* le_sms_MsgListRef_t;


//--------------------------------------------------------------------------------------------------
/**
 * Reference type used by Add/Remove functions for EVENT 'le_sms_RxMessage'
 */
//--------------------------------------------------------------------------------------------------
typedef struct le_sms_RxMessageHandler* le_sms_RxMessageHandlerRef_t;


//--------------------------------------------------------------------------------------------------
/**
 * Reference type used by Add/Remove functions for EVENT 'le_sms_FullStorageEvent'
 */
//--------------------------------------------------------------------------------------------------
typedef struct le_sms_FullStorageEventHandler* le_sms_FullStorageEventHandlerRef_t;


//--------------------------------------------------------------------------------------------------
/**
 * Handler for Sending result.
 */
//--------------------------------------------------------------------------------------------------
typedef void (*le_sms_CallbackResultFunc_t)
(
    le_sms_MsgRef_t msgRef,
        ///< Reference to the message object.
    le_sms_Status_t status,
        ///< Status result.
    void* contextPtr
        ///<
);

//--------------------------------------------------------------------------------------------------
/**
 * Handler for New Message.
 *
 */
//--------------------------------------------------------------------------------------------------
typedef void (*le_sms_RxMessageHandlerFunc_t)
(
    le_sms_MsgRef_t msgRef,
        ///< Message reference.
    void* contextPtr
        ///<
);

//--------------------------------------------------------------------------------------------------
/**
 * Handler for full storage indication.
 *
 */
//--------------------------------------------------------------------------------------------------
typedef void (*le_sms_FullStorageHandlerFunc_t)
(
    le_sms_Storage_t storage,
        ///< storage parameter.
    void* contextPtr
        ///<
);

//--------------------------------------------------------------------------------------------------
/**
 * Add handler function for EVENT 'le_sms_RxMessage'
 *
 * This event provides information on new received messages.
 *
 */
//--------------------------------------------------------------------------------------------------
le_sms_RxMessageHandlerRef_t le_sms_AddRxMessageHandler
(
    le_sms_RxMessageHandlerFunc_t handlerPtr,
        ///< [IN]
    void* contextPtr
        ///< [IN]
);

//--------------------------------------------------------------------------------------------------
/**
 * Remove handler function for EVENT 'le_sms_RxMessage'
 */
//--------------------------------------------------------------------------------------------------
void le_sms_RemoveRxMessageHandler
(
    le_sms_RxMessageHandlerRef_t handlerRef
        ///< [IN]
);

//--------------------------------------------------------------------------------------------------
/**
 * Add handler function for EVENT 'le_sms_FullStorageEvent'
 *
 * This event provides information on full storage indication.
 *
 */
//--------------------------------------------------------------------------------------------------
le_sms_FullStorageEventHandlerRef_t le_sms_AddFullStorageEventHandler
(
    le_sms_FullStorageHandlerFunc_t handlerPtr,
        ///< [IN]
    void* contextPtr
        ///< [IN]
);

//--------------------------------------------------------------------------------------------------
/**
 * Remove handler function for EVENT 'le_sms_FullStorageEvent'
 */
//--------------------------------------------------------------------------------------------------
void le_sms_RemoveFullStorageEventHandler
(
    le_sms_FullStorageEventHandlerRef_t handlerRef
        ///< [IN]
);

//--------------------------------------------------------------------------------------------------
/**
 * Create an SMS Message data structure.
 *
 * @return Reference to the new Message object.
 *
 * @note
 *      On failure, the process exits, so you don't have to worry about checking the returned
 *      reference for validity.
 */
//--------------------------------------------------------------------------------------------------
le_sms_MsgRef_t le_sms_Create
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * Set the timeout to send a SMS Message.
 *
 * @return
 * - LE_FAULT         Message is not in UNSENT state or is Read-Only.
 * - LE_OK            Function succeeded.
 *
 * @note
 *      On failure, the process exits, so you don't have to worry about checking the returned
 *      reference for validity.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_sms_SetTimeout
(
    le_sms_MsgRef_t msgRef,
        ///< [IN] Reference to the message object.
    uint32_t timeout
        ///< [IN] Timeout in seconds.
);

//--------------------------------------------------------------------------------------------------
/**
 * Set the Telephone destination number.
 *
 * Telephone number is defined in ITU-T recommendations E.164/E.163.
 * E.164 numbers can have a maximum of fifteen digits and are usually written with a '+' prefix.
 *
 * @return LE_NOT_PERMITTED Message is Read-Only.
 * @return LE_BAD_PARAMETER Telephone destination number length is equal to zero.
 * @return LE_OK            Function succeeded.
 *
 * @note If telephone destination number is too long is too long (max LE_MDMDEFS_PHONE_NUM_MAX_LEN
 *       digits), it is a fatal error, the function will not return.
 *
 * @note If the caller is passing a bad pointer into this function, it is a fatal error, the
 *       function will not return.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_sms_SetDestination
(
    le_sms_MsgRef_t msgRef,
        ///< [IN] Reference to the message object.
    const char* LE_NONNULL dest
        ///< [IN] Telephone number string.
);

//--------------------------------------------------------------------------------------------------
/**
 * This function must be called to set the Text Message content.
 *
 * @return LE_NOT_PERMITTED Message is Read-Only.
 * @return LE_BAD_PARAMETER Text message length is equal to zero.
 * @return LE_OK            Function succeeded.
 *
 * @note Text Message is encoded in ASCII format (ISO8859-15) and characters have to exist in
 *  the GSM 23.038 7 bit alphabet.
 *
 * @note If message is too long (max LE_SMS_TEXT_MAX_LEN digits), it is a fatal error, the
 *       function will not return.
 *
 * @note If the caller is passing a bad pointer into this function, it is a fatal error, the
 *       function will not return.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_sms_SetText
(
    le_sms_MsgRef_t msgRef,
        ///< [IN] Reference to the message object.
    const char* LE_NONNULL text
        ///< [IN] SMS text.
);

//--------------------------------------------------------------------------------------------------
/**
 * Set the binary message content.
 *
 * @return LE_NOT_PERMITTED Message is Read-Only.
 * @return LE_BAD_PARAMETER Length of the data is equal to zero.
 * @return LE_OK            Function succeeded.
 *
 * @note If length of the data is too long (max LE_SMS_BINARY_MAX_BYTES bytes), it is a fatal
 *       error, the function will not return.
 *
 * @note If the caller is passing a bad pointer into this function, it is a fatal error, the
 *       function will not return.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_sms_SetBinary
(
    le_sms_MsgRef_t msgRef,
        ///< [IN] Reference to the message object.
    const uint8_t* binPtr,
        ///< [IN] Binary data.
    size_t binSize
        ///< [IN]
);

//--------------------------------------------------------------------------------------------------
/**
 * Set the UCS2 message content (16-bit format).
 *
 * @return
 *  - LE_NOT_PERMITTED Message is Read-Only.
 *  - LE_BAD_PARAMETER Length of the data is equal to zero.
 *  - LE_OK            Function succeeded.
 *
 * @note If length of the data is too long (max LE_SMS_UCS2_MAX_CHARS), it is a fatal
 *       error, the function will not return.
 *
 * @note If the caller is passing a bad pointer into this function, it is a fatal error, the
 *       function will not return.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_sms_SetUCS2
(
    le_sms_MsgRef_t msgRef,
        ///< [IN] Reference to the message object.
    const uint16_t* ucs2Ptr,
        ///< [IN] UCS2 message.
    size_t ucs2Size
        ///< [IN]
);

//--------------------------------------------------------------------------------------------------
/**
 * Set the PDU message content.
 *
 * @return LE_NOT_PERMITTED Message is Read-Only.
 * @return LE_BAD_PARAMETER Length of the data is equal to zero.
 * @return LE_OK            Function succeeded.
 *
 * @note If length of the data is too long (max LE_SMS_PDU_MAX_BYTES bytes), it is a fatal error,
 *       the function will not return.
 *
 * @note If the caller is passing a bad pointer into this function, it is a fatal error, the
 *       function will not return.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_sms_SetPDU
(
    le_sms_MsgRef_t msgRef,
        ///< [IN] Reference to the message object.
    const uint8_t* pduPtr,
        ///< [IN] PDU message.
    size_t pduSize
        ///< [IN]
);

//--------------------------------------------------------------------------------------------------
/**
 * Send an SMS message.
 *
 * Verifies first if the parameters are valid, then it checks the modem state can support
 * message sending.
 *
 * @return LE_FORMAT_ERROR  Message content is invalid.
 * @return LE_FAULT         Function failed to send the message.
 * @return LE_OK            Function succeeded.
 * @return LE_TIMEOUT       Timeout before the complete sending.
 *
 * @note If the caller is passing a bad pointer into this function, it is a fatal error, the
 *       function will not return.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_sms_Send
(
    le_sms_MsgRef_t msgRef
        ///< [IN] Reference to the message object.
);

//--------------------------------------------------------------------------------------------------
/**
 * Send an asynchronous SMS message.
 *
 * Verifies first if the parameters are valid, then it checks the modem state can support
 * message sending.
 *
 * @return LE_FORMAT_ERROR  Message content is invalid.
 * @return LE_FAULT         Function failed to send the message.
 * @return LE_OK            Function succeeded.
 * @return LE_TIMEOUT       Timeout before the complete sending.
 *
 * @note If the caller is passing a bad pointer into this function, it is a fatal error, the
 *       function will not return.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_sms_SendAsync
(
    le_sms_MsgRef_t msgRef,
        ///< [IN] Reference to the message object.
    le_sms_CallbackResultFunc_t handlerPtr,
        ///< [IN] CallBack for sending result.
    void* contextPtr
        ///< [IN]
);

//--------------------------------------------------------------------------------------------------
/**
 * Get the error code when a 3GPP2 message sending has Failed.
 *
 * @return The error code
 *
 * @note If the caller is passing a bad pointer into this function, it is a fatal error, the
 *       function will not return.
 *
 * @note It is only applicable for 3GPP2 message sending failure, otherwise
 *       LE_SMS_ERROR_3GPP2_MAX is returned.
 */
//--------------------------------------------------------------------------------------------------
le_sms_ErrorCode3GPP2_t le_sms_Get3GPP2ErrorCode
(
    le_sms_MsgRef_t msgRef
        ///< [IN] Reference to the message object.
);

//--------------------------------------------------------------------------------------------------
/**
 * Get the Radio Protocol and the Transfer Protocol error code when a 3GPP message sending has
 * failed.
 *
 * @note If the caller is passing a bad pointer into this function, it is a fatal error, the
 *       function will not return.
 *
 * @note It is only applicable for 3GPP message sending failure, otherwise
 *       LE_SMS_ERROR_3GPP_MAX is returned.
 */
//--------------------------------------------------------------------------------------------------
void le_sms_GetErrorCode
(
    le_sms_MsgRef_t msgRef,
        ///< [IN] Reference to the message object.
    le_sms_ErrorCode_t* rpCausePtr,
        ///< [OUT] Radio Protocol cause code.
    le_sms_ErrorCode_t* tpCausePtr
        ///< [OUT] Transfer Protocol cause code.
);

//--------------------------------------------------------------------------------------------------
/**
 * Called to get the platform specific error code.
 *
 * Refer to @ref platformConstraintsSpecificErrorCodes for platform specific error code description.
 *
 * @return The platform specific error code.
 *
 * @note If the caller is passing a bad pointer into this function, it is a fatal error, the
 *       function will not return.
 */
//--------------------------------------------------------------------------------------------------
int32_t le_sms_GetPlatformSpecificErrorCode
(
    le_sms_MsgRef_t msgRef
        ///< [IN] Reference to the message object.
);

//--------------------------------------------------------------------------------------------------
/**
 * Create and asynchronously send a text message.
 *
 * @return
 *  - le_sms_Msg Reference to the new Message object pooled.
 *  - NULL Not possible to pool a new message.
 *
 * @note If telephone destination number is too long is too long (max LE_MDMDEFS_PHONE_NUM_MAX_LEN
 *       digits), it is a fatal error, the function will not return.
 * @note If message is too long (max LE_SMS_TEXT_MAX_LEN digits), it is a fatal error, the
 *       function will not return.
 */
//--------------------------------------------------------------------------------------------------
le_sms_MsgRef_t le_sms_SendText
(
    const char* LE_NONNULL destStr,
        ///< [IN] Telephone number string.
    const char* LE_NONNULL textStr,
        ///< [IN] SMS text.
    le_sms_CallbackResultFunc_t handlerPtr,
        ///< [IN] CallBack for sending result.
    void* contextPtr
        ///< [IN]
);

//--------------------------------------------------------------------------------------------------
/**
 * Create and asynchronously send a PDU message.
 *
 * @return
 *  - le_sms_Msg Reference to the new Message object pooled.
 *  - NULL Not possible to pool a new message.
 *
 */
//--------------------------------------------------------------------------------------------------
le_sms_MsgRef_t le_sms_SendPdu
(
    const uint8_t* pduPtr,
        ///< [IN] PDU message.
    size_t pduSize,
        ///< [IN]
    le_sms_CallbackResultFunc_t handlerPtr,
        ///< [IN] CallBack for sending result.
    void* contextPtr
        ///< [IN]
);

//--------------------------------------------------------------------------------------------------
/**
 * Delete a Message data structure.
 *
 * It deletes the Message data structure and all the allocated memory is freed. If several
 * users own the Message object (e.g., several handler functions registered for
 * SMS message reception), the Message object will only be deleted if one User
 * owns the Message object.
 *
 * @note If the caller is passing a bad pointer into this function, it is a fatal error, the
 *       function will not return.
 */
//--------------------------------------------------------------------------------------------------
void le_sms_Delete
(
    le_sms_MsgRef_t msgRef
        ///< [IN] Reference to the message object.
);

//--------------------------------------------------------------------------------------------------
/**
 * Get the message format.
 *
 * @return Message format.
 *
 * @note If the caller is passing a bad pointer into this function, it is a fatal error, the
 *       function will not return.
 *
 */
//--------------------------------------------------------------------------------------------------
le_sms_Format_t le_sms_GetFormat
(
    le_sms_MsgRef_t msgRef
        ///< [IN] Reference to the message object.
);

//--------------------------------------------------------------------------------------------------
/**
 * Get the message type.
 *
 * @return Message type.
 *
 * @note If the caller is passing a bad pointer into this function, it is a fatal error, the
 *       function will not return.
 */
//--------------------------------------------------------------------------------------------------
le_sms_Type_t le_sms_GetType
(
    le_sms_MsgRef_t msgRef
        ///< [IN] Reference to the message object.
);

//--------------------------------------------------------------------------------------------------
/**
 * Get the Cell Broadcast Message Identifier.
 *
 * @return
 * - LE_FAULT       Message is not a cell broadcast type.
 * - LE_OK          Function succeeded.
 *
 * @note If the caller is passing a bad pointer into this function, it is a fatal error, the
 *       function will not return.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_sms_GetCellBroadcastId
(
    le_sms_MsgRef_t msgRef,
        ///< [IN] Reference to the message object.
    uint16_t* messageIdPtr
        ///< [OUT] Cell Broadcast Message Identifier.
);

//--------------------------------------------------------------------------------------------------
/**
 * Get the Cell Broadcast Message Serial Number.
 *
 * @return
 * - LE_FAULT       Message is not a cell broadcast type.
 * - LE_OK          Function succeeded.
 *
 * @note If the caller is passing a bad pointer into this function, it is a fatal error, the
 *       function will not return.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_sms_GetCellBroadcastSerialNumber
(
    le_sms_MsgRef_t msgRef,
        ///< [IN] Reference to the message object.
    uint16_t* serialNumberPtr
        ///< [OUT] Cell Broadcast Serial Number.
);

//--------------------------------------------------------------------------------------------------
/**
 * Get the Sender Telephone number.
 *
 * Output parameter is updated with the Telephone number. If the Telephone number string exceeds
 * the value of 'len' parameter,  LE_OVERFLOW error code is returned and 'tel' is filled until
 * 'len-1' characters and a null-character is implicitly appended at the end of 'tel'.
 *
 * @return LE_NOT_PERMITTED Message is not a received message.
 * @return LE_OVERFLOW      Telephone number length exceed the maximum length.
 * @return LE_OK            Function succeeded.
 *
 * @note If the caller is passing a bad pointer into this function, it is a fatal error, the
 *       function will not return.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_sms_GetSenderTel
(
    le_sms_MsgRef_t msgRef,
        ///< [IN] Reference to the message object.
    char* tel,
        ///< [OUT] Telephone number string.
    size_t telSize
        ///< [IN]
);

//--------------------------------------------------------------------------------------------------
/**
 * Get the Service Center Time Stamp string.
 *
 * Output parameter is updated with the Time Stamp string. If the Time Stamp string exceeds the
 * value of 'len' parameter, a LE_OVERFLOW error code is returned and 'timestamp' is filled until
 * 'len-1' characters and a null-character is implicitly appended at the end of 'timestamp'.
 *
 * @return LE_NOT_PERMITTED Message is not a received message.
 * @return LE_OVERFLOW      Timestamp number length exceed the maximum length.
 * @return LE_OK            Function succeeded.
 *
 * @note If the caller is passing a bad pointer into this function, it is a fatal error, the
 *       function will not return.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_sms_GetTimeStamp
(
    le_sms_MsgRef_t msgRef,
        ///< [IN] Reference to the message object.
    char* timestamp,
        ///< [OUT] Message time stamp (in text mode).
        ///<      string format: "yy/MM/dd,hh:mm:ss+/-zz"
        ///<      (Year/Month/Day,Hour:Min:Seconds+/-TimeZone)
        ///< The time zone indicates the difference, expressed
        ///< in quarters of an hours between the local time
        ///<  and GMT.
    size_t timestampSize
        ///< [IN]
);

//--------------------------------------------------------------------------------------------------
/**
 * Get the message Length value.
 *
 * @return Number of characters for text and UCS2 messages, or the length of the data in bytes for
 *  raw binary messages.
 *
 * @note If the caller is passing a bad pointer into this function, it is a fatal error, the
 *       function will not return.
 */
//--------------------------------------------------------------------------------------------------
size_t le_sms_GetUserdataLen
(
    le_sms_MsgRef_t msgRef
        ///< [IN] Reference to the message object.
);

//--------------------------------------------------------------------------------------------------
/**
 * Get the text Message.
 *
 * Output parameter is updated with the text string encoded in ASCII format. If the text string
 * exceeds the value of 'len' parameter, LE_OVERFLOW error code is returned and 'text' is filled
 * until 'len-1' characters and a null-character is implicitly appended at the end of 'text'.
 *
 * @return LE_OVERFLOW      Message length exceed the maximum length.
 * @return LE_OK            Function succeeded.
 *
 * @note If the caller is passing a bad pointer into this function, it is a fatal error, the
 *       function will not return.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_sms_GetText
(
    le_sms_MsgRef_t msgRef,
        ///< [IN] Reference to the message object.
    char* text,
        ///< [OUT] SMS text.
    size_t textSize
        ///< [IN]
);

//--------------------------------------------------------------------------------------------------
/**
 * Get the binary Message.
 *
 * Output parameters are updated with the binary message content and the length of the raw
 * binary message in bytes. If the binary data exceed the value of 'len' input parameter, a
 * LE_OVERFLOW error code is returned and 'raw' is filled until 'len' bytes.
 *
 * @return LE_FORMAT_ERROR  Message is not in binary format
 * @return LE_OVERFLOW      Message length exceed the maximum length.
 * @return LE_OK            Function succeeded.
 *
 * @note If the caller is passing a bad pointer into this function, it is a fatal error, the
 *       function will not return.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_sms_GetBinary
(
    le_sms_MsgRef_t msgRef,
        ///< [IN] Reference to the message object.
    uint8_t* binPtr,
        ///< [OUT] Binary message.
    size_t* binSizePtr
        ///< [INOUT]
);

//--------------------------------------------------------------------------------------------------
/**
 * Get the UCS2 Message (16-bit format).
 *
 * Output parameters are updated with the UCS2 message content and the number of characters. If
 * the UCS2 data exceed the value of the length input parameter, a LE_OVERFLOW error
 * code is returned and 'ucs2Ptr' is filled until of the number of chars specified.
 *
 * @return
 *  - LE_FORMAT_ERROR  Message is not in binary format
 *  - LE_OVERFLOW      Message length exceed the maximum length.
 *  - LE_OK            Function succeeded.
 *
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_sms_GetUCS2
(
    le_sms_MsgRef_t msgRef,
        ///< [IN] Reference to the message object.
    uint16_t* ucs2Ptr,
        ///< [OUT] UCS2 message.
    size_t* ucs2SizePtr
        ///< [INOUT]
);

//--------------------------------------------------------------------------------------------------
/**
 * Get the PDU message.
 *
 * Output parameters are updated with the PDU message content and the length of the PDU message
 * in bytes. If the PDU data exceed the value of 'len' input parameter, a LE_OVERFLOW error code is
 * returned and 'pdu' is filled until 'len' bytes.
 *
 * @return LE_FORMAT_ERROR  Unable to encode the message in PDU (only for outgoing messages).
 * @return LE_OVERFLOW      Message length exceed the maximum length.
 * @return LE_OK            Function succeeded.
 *
 * @note If the caller is passing a bad pointer into this function, it is a fatal error, the
 *       function will not return.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_sms_GetPDU
(
    le_sms_MsgRef_t msgRef,
        ///< [IN] Reference to the message object.
    uint8_t* pduPtr,
        ///< [OUT] PDU message.
    size_t* pduSizePtr
        ///< [INOUT]
);

//--------------------------------------------------------------------------------------------------
/**
 * Get the message Length value.
 *
 * @return Length of the data in bytes of the PDU message.
 *
 * @note If the caller is passing a bad pointer into this function, it is a fatal error, the
 *       function will not return.
 */
//--------------------------------------------------------------------------------------------------
size_t le_sms_GetPDULen
(
    le_sms_MsgRef_t msgRef
        ///< [IN] Reference to the message object.
);

//--------------------------------------------------------------------------------------------------
/**
 * Delete an SMS message from the storage area.
 *
 * Verifies first if the parameter is valid, then it checks the modem state can support
 * message deleting.
 *
 * @return LE_FAULT         Function failed to perform the deletion.
 * @return LE_NO_MEMORY     The message is not present in storage area.
 * @return LE_OK            Function succeeded.
 *
 * @note If the caller is passing a bad pointer into this function, it is a fatal error, the
 *       function will not return.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_sms_DeleteFromStorage
(
    le_sms_MsgRef_t msgRef
        ///< [IN] Reference to the message object.
);

//--------------------------------------------------------------------------------------------------
/**
 * Create an object's reference of the list of received messages
 * saved in the SMS message storage area.
 *
 * @return
 *      Reference to the List object. Null pointer if no messages have been retrieved.
 */
//--------------------------------------------------------------------------------------------------
le_sms_MsgListRef_t le_sms_CreateRxMsgList
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * Delete the list of the Messages retrieved from the message
 * storage.
 *
 * @note
 *      On failure, the process exits, so you don't have to worry about checking the returned
 *      reference for validity.
 */
//--------------------------------------------------------------------------------------------------
void le_sms_DeleteList
(
    le_sms_MsgListRef_t msgListRef
        ///< [IN] Messages list.
);

//--------------------------------------------------------------------------------------------------
/**
 * Get the first Message object reference in the list of messages
 * retrieved with le_sms_CreateRxMsgList().
 *
 * @return NULL              No message found.
 * @return Msg  Message object reference.
 *
 * @note If the caller is passing a bad pointer into this function, it is a fatal error, the
 *       function will not return.
 */
//--------------------------------------------------------------------------------------------------
le_sms_MsgRef_t le_sms_GetFirst
(
    le_sms_MsgListRef_t msgListRef
        ///< [IN] Messages list.
);

//--------------------------------------------------------------------------------------------------
/**
 * Get the next Message object reference in the list of messages
 * retrieved with le_sms_CreateRxMsgList().
 *
 * @return NULL              No message found.
 * @return Msg  Message object reference.
 *
 * @note If the caller is passing a bad pointer into this function, it is a fatal error, the
 *       function will not return.
 */
//--------------------------------------------------------------------------------------------------
le_sms_MsgRef_t le_sms_GetNext
(
    le_sms_MsgListRef_t msgListRef
        ///< [IN] Messages list.
);

//--------------------------------------------------------------------------------------------------
/**
 * Read the Message status (Received Read, Received Unread, Stored
 * Sent, Stored Unsent).
 *
 * @return Status of the message.
 *
 * @note If the caller is passing a bad pointer into this function, it is a fatal error, the
 *       function will not return.
 */
//--------------------------------------------------------------------------------------------------
le_sms_Status_t le_sms_GetStatus
(
    le_sms_MsgRef_t msgRef
        ///< [IN] Messages list.
);

//--------------------------------------------------------------------------------------------------
/**
 * Mark a message as 'read'.
 *
 * @note If the caller is passing a bad pointer into this function, it is a fatal error, the
 *       function will not return.
 */
//--------------------------------------------------------------------------------------------------
void le_sms_MarkRead
(
    le_sms_MsgRef_t msgRef
        ///< [IN] Messages list.
);

//--------------------------------------------------------------------------------------------------
/**
 * Mark a message as 'unread'.
 *
 * @note If the caller is passing a bad pointer into this function, it is a fatal error, the
 *       function will not return.
 */
//--------------------------------------------------------------------------------------------------
void le_sms_MarkUnread
(
    le_sms_MsgRef_t msgRef
        ///< [IN] Messages list.
);

//--------------------------------------------------------------------------------------------------
/**
 * Get the SMS center address.
 *
 * Output parameter is updated with the SMS Service center address. If the Telephone number string exceeds
 * the value of 'len' parameter,  LE_OVERFLOW error code is returned and 'tel' is filled until
 * 'len-1' characters and a null-character is implicitly appended at the end of 'tel'.
 *
 * @return
 *  - LE_FAULT         Service is not available.
 *  - LE_OVERFLOW      Telephone number length exceed the maximum length.
 *  - LE_OK            Function succeeded.
 *
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_sms_GetSmsCenterAddress
(
    char* tel,
        ///< [OUT] SMS center address number string.
    size_t telSize
        ///< [IN]
);

//--------------------------------------------------------------------------------------------------
/**
 * Set the SMS center address.
 *
 * SMS center address number is defined in ITU-T recommendations E.164/E.163.
 * E.164 numbers can have a maximum of fifteen digits and are usually written with a '+' prefix.
 *
 * @return
 *  - LE_FAULT         Service is not available..
 *  - LE_OK            Function succeeded.
 *
 * @note If the SMS center address number is too long (max LE_MDMDEFS_PHONE_NUM_MAX_LEN digits), it
 *       is a fatal error, the function will not return.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_sms_SetSmsCenterAddress
(
    const char* LE_NONNULL tel
        ///< [IN] SMS center address number string.
);

//--------------------------------------------------------------------------------------------------
/**
 * Set the preferred SMS storage for incoming messages.
 * @return
 *  - LE_FAULT         Function failed.
 *  - LE_OK            Function succeeded.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_sms_SetPreferredStorage
(
    le_sms_Storage_t prefStorage
        ///< [IN] storage parameter.
);

//--------------------------------------------------------------------------------------------------
/**
 * Get the preferred SMS storage for incoming messages.
 * @return
 *  - LE_FAULT         Function failed.
 *  - LE_OK            Function succeeded.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_sms_GetPreferredStorage
(
    le_sms_Storage_t* prefStoragePtr
        ///< [OUT] storage parameter.
);

//--------------------------------------------------------------------------------------------------
/**
 * Activate Cell Broadcast message notification
 *
 * @return
 *  - LE_FAULT         Function failed.
 *  - LE_OK            Function succeeded.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_sms_ActivateCellBroadcast
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * Deactivate Cell Broadcast message notification
 *
 * @return
 *  - LE_FAULT         Function failed.
 *  - LE_OK            Function succeeded.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_sms_DeactivateCellBroadcast
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * Activate CDMA Cell Broadcast message notification
 *
 * @return
 *  - LE_FAULT         Function failed.
 *  - LE_OK            Function succeeded.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_sms_ActivateCdmaCellBroadcast
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * Deactivate CDMA Cell Broadcast message notification
 *
 * @return
 *  - LE_FAULT         Function failed.
 *  - LE_OK            Function succeeded.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_sms_DeactivateCdmaCellBroadcast
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * Add Cell Broadcast message Identifiers range.
 *
 * @return
 *  - LE_FAULT         Function failed.
 *  - LE_OK            Function succeeded.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_sms_AddCellBroadcastIds
(
    uint16_t fromId,
        ///< [IN] Starting point of the range of cell broadcast message identifier.
    uint16_t toId
        ///< [IN] Ending point of the range of cell broadcast message identifier.
);

//--------------------------------------------------------------------------------------------------
/**
 * Remove Cell Broadcast message Identifiers range.
 *
 * @return
 *  - LE_FAULT         Function failed.
 *  - LE_OK            Function succeeded.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_sms_RemoveCellBroadcastIds
(
    uint16_t fromId,
        ///< [IN] Starting point of the range of cell broadcast message identifier.
    uint16_t toId
        ///< [IN] Ending point of the range of cell broadcast message identifier.
);

//--------------------------------------------------------------------------------------------------
/**
 * Clear Cell Broadcast message Identifiers.
 *
 * @return
 *  - LE_FAULT         Function failed.
 *  - LE_OK            Function succeeded.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_sms_ClearCellBroadcastIds
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * Add CDMA Cell Broadcast category services.
 *
 * @return
 *  - LE_FAULT         Function failed.
 *  - LE_BAD_PARAMETER Parameter is invalid.
 *  - LE_OK            Function succeeded.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_sms_AddCdmaCellBroadcastServices
(
    le_sms_CdmaServiceCat_t serviceCat,
        ///< [IN] Service category assignment. Reference to 3GPP2 C.R1001-D
        ///< v1.0 Section 9.3.1 Standard Service Category Assignments.
    le_sms_Languages_t language
        ///< [IN] Language Indicator. Reference to
        ///< 3GPP2 C.R1001-D v1.0 Section 9.2.1 Language Indicator
        ///< Value Assignments
);

//--------------------------------------------------------------------------------------------------
/**
 * Remove CDMA Cell Broadcast category services.
 *
 * @return
 *  - LE_FAULT         Function failed.
 *  - LE_BAD_PARAMETER Parameter is invalid.
 *  - LE_OK            Function succeeded.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_sms_RemoveCdmaCellBroadcastServices
(
    le_sms_CdmaServiceCat_t serviceCat,
        ///< [IN] Service category assignment. Reference to 3GPP2 C.R1001-D
        ///< v1.0 Section 9.3.1 Standard Service Category Assignments.
    le_sms_Languages_t language
        ///< [IN] Language Indicator. Reference to
        ///< 3GPP2 C.R1001-D v1.0 Section 9.2.1 Language Indicator
        ///< Value Assignments
);

//--------------------------------------------------------------------------------------------------
/**
 * Clear CDMA Cell Broadcast category services.
 *
 * @return
 *  - LE_FAULT         Function failed.
 *  - LE_OK            Function succeeded.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_sms_ClearCdmaCellBroadcastServices
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * Get the number of messages successfully received or sent since last counter reset.
 *
 * @return
 *  - LE_OK             Function succeeded.
 *  - LE_BAD_PARAMETER  A parameter is invalid.
 *
 * @note If the caller is passing a bad pointer into this function, it is a fatal error, the
 *       function will not return.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_sms_GetCount
(
    le_sms_Type_t messageType,
        ///< [IN] Message type
    int32_t* messageCountPtr
        ///< [OUT] Number of messages
);

//--------------------------------------------------------------------------------------------------
/**
 * Start to count the messages successfully received and sent.
 */
//--------------------------------------------------------------------------------------------------
void le_sms_StartCount
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * Stop to count the messages successfully received and sent.
 */
//--------------------------------------------------------------------------------------------------
void le_sms_StopCount
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * Reset the count of messages successfully received and sent.
 */
//--------------------------------------------------------------------------------------------------
void le_sms_ResetCount
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * Enable SMS Status Report for outgoing messages.
 *
 * @return
 *  - LE_OK             Function succeeded.
 *  - LE_FAULT          Function failed.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_sms_EnableStatusReport
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * Disable SMS Status Report for outgoing messages.
 *
 * @return
 *  - LE_OK             Function succeeded.
 *  - LE_FAULT          Function failed.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_sms_DisableStatusReport
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * Get SMS Status Report activation state.
 *
 * @return
 *  - LE_OK             Function succeeded.
 *  - LE_BAD_PARAMETER  Parameter is invalid.
 *  - LE_FAULT          Function failed.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_sms_IsStatusReportEnabled
(
    bool* enabledPtr
        ///< [OUT] True when SMS Status Report is enabled, false otherwise.
);

//--------------------------------------------------------------------------------------------------
/**
 * Get TP-Message-Reference of a message. Message type should be either a SMS Status Report or an
 * outgoing SMS.
 * TP-Message-Reference is defined in 3GPP TS 23.040 section 9.2.3.6.
 *
 * @return
 *  - LE_OK             Function succeeded.
 *  - LE_BAD_PARAMETER  Parameter is invalid.
 *  - LE_FAULT          Function failed.
 *  - LE_UNAVAILABLE    Outgoing SMS message is not sent.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_sms_GetTpMr
(
    le_sms_MsgRef_t msgRef,
        ///< [IN] Reference to the message object.
    uint8_t* tpMrPtr
        ///< [OUT] 3GPP TS 23.040 TP-Message-Reference.
);

//--------------------------------------------------------------------------------------------------
/**
 * Get TP-Recipient-Address of SMS Status Report.
 * TP-Recipient-Address is defined in 3GPP TS 23.040 section 9.2.3.14.
 * TP-Recipient-Address Type-of-Address is defined in 3GPP TS 24.011 section 8.2.5.2.
 *
 * @return
 *  - LE_OK             Function succeeded.
 *  - LE_BAD_PARAMETER  A parameter is invalid.
 *  - LE_OVERFLOW       The Recipient Address length exceeds the length of the provided buffer.
 *  - LE_FAULT          Function failed.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_sms_GetTpRa
(
    le_sms_MsgRef_t msgRef,
        ///< [IN] Reference to the message object.
    uint8_t* toraPtr,
        ///< [OUT] 3GPP TS 24.011 TP-Recipient-Address
        ///< Type-of-Address.
    char* ra,
        ///< [OUT] 3GPP TS 23.040 TP-Recipient-Address.
    size_t raSize
        ///< [IN]
);

//--------------------------------------------------------------------------------------------------
/**
 * Get TP-Service-Centre-Time-Stamp of SMS Status Report.
 * TP-Service-Centre-Time-Stamp is defined in 3GPP TS 23.040 section 9.2.3.11.
 *
 * @return
 *  - LE_OK             Function succeeded.
 *  - LE_BAD_PARAMETER  A parameter is invalid.
 *  - LE_OVERFLOW       The SC Timestamp length exceeds the length of the provided buffer.
 *  - LE_FAULT          Function failed.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_sms_GetTpScTs
(
    le_sms_MsgRef_t msgRef,
        ///< [IN] Reference to the message object.
    char* scts,
        ///< [OUT] 3GPP TS 23.040 TP-Service-Centre-Time-Stamp.
    size_t sctsSize
        ///< [IN]
);

//--------------------------------------------------------------------------------------------------
/**
 * Get TP-Discharge-Time of SMS Status Report.
 * TP-Discharge-Time is defined in 3GPP TS 23.040 section 9.2.3.13.
 *
 * @return
 *  - LE_OK             Function succeeded.
 *  - LE_BAD_PARAMETER  A parameter is invalid.
 *  - LE_OVERFLOW       The Discharge Time length exceeds the length of the provided buffer.
 *  - LE_FAULT          Function failed.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_sms_GetTpDt
(
    le_sms_MsgRef_t msgRef,
        ///< [IN] Reference to the message object.
    char* dt,
        ///< [OUT] 3GPP TS 23.040 TP-Discharge-Time.
    size_t dtSize
        ///< [IN]
);

//--------------------------------------------------------------------------------------------------
/**
 * Get TP-Status of SMS Status Report.
 * TP-Status is defined in 3GPP TS 23.040 section 9.2.3.15.
 *
 * @return
 *  - LE_OK             Function succeeded.
 *  - LE_BAD_PARAMETER  A parameter is invalid.
 *  - LE_FAULT          Function failed.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_sms_GetTpSt
(
    le_sms_MsgRef_t msgRef,
        ///< [IN] Reference to the message object.
    uint8_t* stPtr
        ///< [OUT] 3GPP TS 23.040 TP-Status.
);

#endif // LE_SMS_INTERFACE_H_INCLUDE_GUARD