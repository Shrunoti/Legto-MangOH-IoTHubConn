

/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */

/**
 * @page c_rsim Remote SIM service
 *
 * @ref le_rsim_interface.h "API Reference"
 *
 * <HR>
 *
 * The remote SIM (RSIM) service allows using a remote SIM instead of the internal SIM cards.
 *
 * This service allows a user application to convey APDU requests to the remote SIM and APDU
 * responses to the modem through the RSIM service. The link between the application and the RSIM
 * service is based on the SIM Access Profile (SAP) specification: the user application must
 * therefore implement a remote SIM server supporting this specification to handle the remote
 * SIM card.
 *
 * @startuml{le_rsim_Overview.png}
 *    interface "Remote SIM card" as SIM
 *    SIM <-> [Remote SIM Server application] : APDU
 *    [Remote SIM Server application] <-d-> [Legato remote SIM service] : SAP messages
 *    [Legato remote SIM service] <-r-> Modem : APDU
 *   @enduml
 *
 * @note The remote SIM Server application is not part of the Legato implementation and
 * should be developed by the user.
 *
 * @section le_rsim_binding IPC interfaces binding
 *
 * All the functions of this API are provided by the @b modemService application service.
 *
 * Here's a code sample binding to Data Connection services:
 * @verbatim
   bindings:
   {
      clientExe.clientComponent.le_rsim -> modemService.le_rsim
   }
   @endverbatim
 *
 * @section le_rsim_Communication Communication
 *
 * The communication between the application and the remote SIM service uses the SIM Access Profile
 * (SAP) protocol.
 *
 * The latest <a href="https://www.bluetooth.org/DocMan/handlers/DownloadDoc.ashx?doc_id=158740">
 * V11r00 SAP specification</a> is supported by the remote SIM service. All client-mandatory
 * features and some optional features are supported. The table below summarizes all SAP messages
 * supported by the remote SIM service.
 * <table>
 * <tr><th>Feature                                  <th>Associated SAP messages          <th>Support in SAP client <th>RSIM support
 * <tr><td rowspan="5">Connection management        <td>CONNECT_REQ                      <td rowspan="5">Mandatory <td>Supported
 * <tr>                                             <td>CONNECT_RESP                                               <td>Supported
 * <tr>                                             <td>DISCONNECT_REQ                                             <td>Supported
 * <tr>                                             <td>DISCONNECT_RESP                                            <td>Supported
 * <tr>                                             <td>DISCONNECT_IND                                             <td>Supported
 * <tr><td rowspan="2">Transfer APDU                <td>TRANSFER_APDU_REQ                <td rowspan="2">Mandatory <td>Supported
 * <tr>                                             <td>TRANSFER_APDU_RESP                                         <td>Supported
 * <tr><td rowspan="2">Transfer ATR                 <td>TRANSFER_ATR_REQ                 <td rowspan="2">Mandatory <td>Supported
 * <tr>                                             <td>TRANSFER_ATR_RESP                                          <td>Supported
 * <tr><td rowspan="2">Power SIM off                <td>POWER_SIM_OFF_REQ                <td rowspan="2">Optional  <td>Supported
 * <tr>                                             <td>POWER_SIM_OFF_RESP                                         <td>Supported
 * <tr><td rowspan="2">Power SIM on                 <td>POWER_SIM_ON_REQ                 <td rowspan="2">Mandatory <td>Supported
 * <tr>                                             <td>POWER_SIM_ON_RESP                                          <td>Supported
 * <tr><td rowspan="2">Reset SIM                    <td>RESET_SIM_REQ                    <td rowspan="2">Optional  <td>Supported
 * <tr>                                             <td>RESET_SIM_RESP                                             <td>Supported
 * <tr><td>Report Status                            <td>STATUS_IND                       <td>Mandatory             <td>Supported
 * <tr><td rowspan="2">Transfer Card Reader Status  <td>TRANSFER_CARD_READER_STATUS_REQ  <td rowspan="2">Optional  <td>Not supported
 * <tr>                                             <td>TRANSFER_CARD_READER_STATUS_RESP                           <td>Not supported
 * <tr><td>Error handling                           <td>ERROR_RESP                       <td>Mandatory             <td>Supported
 * <tr><td rowspan="2">Set Transport Protocol       <td>SET_TRANSPORT_PROTOCOL_REQ       <td rowspan="2">Optional  <td>Not supported
 * <tr>                                             <td>SET_TRANSPORT_PROTOCOL_RESP                                <td>Not supported
 * </table>
 *
 * The application must register a handler function with le_rsim_AddMessageHandler() in order
 * to receive the SAP messages sent by the remote SIM service. Registering the handler indicates
 * that the remote SIM server is ready to receive messages and that a remote SIM card is available.
 * The handler can be deregistered through the le_rsim_RemoveMessageHandler() function when it is
 * not needed anymore.
 *
 * The application can send SAP messages to the remote SIM service with the le_rsim_SendMessage()
 * function. Message sending is an asynchronous process: a callback can therefore be passed to
 * le_rsim_SendMessage() in order to receive the sending result for this message.
 *
 * @warning The remote SIM service supports only one remote SIM card and can therefore be
 * connected with only one application.
 *
 * @note
 * - The remote SIM service has to be supported by the modem to be used: check your platform
 * documentation.
 * - The remote SIM card should be selected in order to use the remote SIM service.
 * - As runtime switch is not currently supported, the switch between local and remote SIM card
 * requires a platform reset to take effect.
 *
 * A sample code of a basic remote SIM server is available in the following page:
 * - @subpage c_rsimTest
 *
 * <HR>
 *
 * Copyright (C) Sierra Wireless Inc.
 */
/**
 * @page c_rsimTest Sample code
 *
 * @include "apps/test/modemServices/rsim/rsimIntegrationTest/rsimTestComp/rsimTest.c"
 */
/**
 * @file le_rsim_interface.h
 *
 * Legato @ref c_rsim API.
 *
 * <HR>
 *
 * Copyright (C) Sierra Wireless Inc.
 */

#ifndef LE_RSIM_INTERFACE_H_INCLUDE_GUARD
#define LE_RSIM_INTERFACE_H_INCLUDE_GUARD


#include "legato.h"


//--------------------------------------------------------------------------------------------------
/**
 * Type for handler called when a server disconnects.
 */
//--------------------------------------------------------------------------------------------------
typedef void (*le_rsim_DisconnectHandler_t)(void *);

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
void le_rsim_ConnectService
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
le_result_t le_rsim_TryConnectService
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
void le_rsim_SetServerDisconnectHandler
(
    le_rsim_DisconnectHandler_t disconnectHandler,
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
void le_rsim_DisconnectService
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