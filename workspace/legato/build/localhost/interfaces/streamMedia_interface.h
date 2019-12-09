

/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */

/**
 * @page c_streamMedia Stream Media API
 *
 * @ref streamMedia_interface.h "API Reference" <br>
 * <HR>
 *
 * @warning This interface is experimental and unsupported.
 *
 * The Stream Media API handles media streaming interfaces with real-time protocols like RTP.
 *
 * The Real-time Transport Protocol is used to stream audio over IP/Ethernet. It is used to
 * transport any audio stream through an IP or Ethernet network.
 *
 * The RTP protocol is augmented by a control protocol called Real-Time Transport Control Protocol
 * (RTCP), which provides monitoring capabilities on the data delivery and minimum control
 * functionality.
 *
 * More information on the protocol can be found in
 * <a href="https://tools.ietf.org/pdf/rfc3550.pdf">RFC-3550</a>. A description of the RTP/AVP
 * profile, which describes generic audio and video conferences, can be found in
 * <a href="https://tools.ietf.org/pdf/rfc3551.pdf">RFC-3551</a>.
 *
 * @note The RTP interface does not provide the full Audio/Video Profile (AVP) profile : it can only
 * handle a singlecast audio session.
 *
 * Typically, the RTP interface can be used to redirect an audio call coming from an audio interface
 * through an IP network. For example, an IP audio server (Legato platform) would retrieve audio
 * from any audio interface (e.g. a Modem voice call), and connect it to the RTP interface that
 * sends the data through an IP network. The data would be received by an IP client (PC, smartphone,
 * Legato platform) that is able to decode the audio stream.
 *
 * @image html AudioOverIP.png
 *
 * In order to start a RTP session, information on the local and remote IP address are required to
 * set up UDP sockets. The local port corresponds to the UDP socket on which RTP packets will be
 * sent and received, and the remote address and port correspond to the IP address and port on which
 * RTP packets should be sent.
 *
 * @section streamMedia_binding IPC interfaces binding
 *
 * All the functions of this API are provided by the @b streamMedia sample service.
 *
 * Here's a code sample binding to streamMedia service:
 * @verbatim
   bindings:
   {
      clientExe.clientComponent.streamMedia -> streamMedia.streamMedia
   }
   @endverbatim
 *
 * @section streamMedia_interfaces Open/Close an Audio Interface
 *
 * The following functions let you select the desired interface attributes:
 * - streamMedia_OpenAudioRtpRx(): return an Audio Stream Reference of the digitized audio signal
 *                                 coming from a RTP socket.
 * - streamMedia_OpenAudioRtpTx(): return an Audio Stream Reference of the digitized audio signal
 *                                 routed to a RTP socket.
 *
 * @section streamMedia_rtp RTP
 *
 * The streamMedia_OpenAudioRtpRx() and streamMedia_OpenAudioRtpTx() functions allow the
 * application to set up a RTP session.
 *
 * After opening and connecting a RTP interface, it has to be started using streamMedia_Start().
 *
 * You can also register a handler function for RTCP packet reception management.
 *
 * streamMedia_AddRtcpHandler() function installs a handler for RTCP Packet reception.
 *
 * streamMedia_RemoveRtcpHandler() function removes the RTCP handler function.
 *
 * The streamMedia_SendRtcpSdes() and streamMedia_SendRtcpBye() functions allow the
 * application to send RTCP Session Description (SDES) or BYE packet.
 *
 * The RTCP SDES packet can be used to mark the beginning of the RTP session. It is composed of
 * several optional text fields that can provide information about the user's name, email address,
 * etc... See streamMedia_SendRtcpSdes() for more details.
 *
 * The RTCP BYE packet can be used to mark the end of the RTP session. It is possible to specify the
 * reason. See streamMedia_SendRtcpBye() for more details.
 *
 * More information on the RTCP SDES and BYE packets can be found in
 * <a href="https://tools.ietf.org/pdf/rfc3550.pdf">RFC-3550</a>.
 *
 * Before disconnecting and closing a RTP interface, it has to be stopped using
 * streamMedia_Stop(). Then, after disconnecting the interface from the connector,
 * streamMedia_Close() is called to close it.
 *
 * @note The RTP interface requires one more UDP socket in order to send and receive RTCP packets.
 * The RTCP UDP socket port number is automatically set to the local RTP port plus one.
 *
 * @section streamMedia_code Sample code
 *
 * The following samples illustrate the case described in the image above. It consists of two
 * different devices that communicate through RTP over an IP network. One device handles a modem
 * voice call, and the other one handles the microphone and speaker.
 *
 * @warning The following codes show a basic example of how to transmit audio through RTP between
 * two peers. Quality of service is not provided in those samples. They just show how the
 * streamMedia API should work and how it should be used.
 *
 * This first sample code is related to the RTP peer that handles the modem voice call.
 *
 * @code
 *
 * // Peer 1 : connection between a modem voice call and a RTP session.
 *
 * static void MyCallEventHandler
 * (
 *     le_mcc_CallRef_t   callRef,
 *     le_mcc_Event_t     callEvent,
 *     void*              contextPtr
 * )
 * {
 *     if (callEvent == LE_MCC_EVENT_TERMINATED)
 *     {
 *         le_mcc_TerminationReason_t term = le_mcc_GetTerminationReason(callRef);
 *         le_mcc_Delete(callRef);
 *         DisconnectAllAudio(); // Not showed here, would disconnect all audio.
 *     }
 *     else if (callEvent == LE_MCC_EVENT_INCOMING)
 *     {
 *         le_mcc_Answer(callRef);
 *     }
 *     else
 *     {
 *         LE_INFO("Other Call event.%d", callEvent);
 *     }
 * }
 *
 * static void MyRtcpEventHandler
 * (
 *     le_audio_StreamRef_t          streamRef,
 *     streamMedia_RtcpEvent_t    event,
 *     void*                         contextPtr
 * )
 * {
 *     switch (event)
 *     {
 *         case STREAMMEDIA_RTCP_BYE:
 *             DisconnectAllAudio(); // Not showed here, would disconnect all audio.
 *             break;
 *         default:
 *             LE_INFO("Other event");
 *             break;
 *     }
 * }
 *
 * void ConnectModemToRtp
 * (
 *     void
 * )
 * {
 *     static le_audio_ConnectorRef_t  audioInputConnectorRef = NULL;
 *     static le_audio_ConnectorRef_t  audioOutputConnectorRef = NULL;
 *     static le_audio_StreamRef_t     rtpInRef = NULL;
 *     static le_audio_StreamRef_t     rtpOutRef = NULL;
 *     static le_audio_StreamRef_t     mdmRxAudioRef = NULL;
 *     static le_audio_StreamRef_t     mdmTxAudioRef = NULL;
 *     static le_mcc_CallRef_t         callRef = NULL;
 *     const char*                     remoteAddress = "10.40.58.2";
 *     const char*                     destinationNumber = "0123456789";
 *     int                             localPort = 4000;
 *     int                             remotePort = 4000;
 *
 *     rtpInRef = streamMedia_OpenAudioRtpRx(localPort);
 *     rtpOutRef = streamMedia_OpenAudioRtpTx(localPort, remoteAddress, remotePort);
 *
 *     mdmRxAudioRef = le_audio_OpenModemVoiceRx();
 *     mdmTxAudioRef = le_audio_OpenModemVoiceTx();
 *
 *     audioInputConnectorRef = le_audio_CreateConnector();
 *     audioOutputConnectorRef = le_audio_CreateConnector();
 *
 *     le_audio_Connect(audioOutputConnectorRef, mdmTxAudioRef);
 *     le_audio_Connect(audioInputConnectorRef, mdmRxAudioRef);
 *
 *     le_audio_Connect(audioOutputConnectorRef, rtpInRef);
 *     streamMedia_Start(rtpInRef);
 *
 *     le_audio_Connect(audioInputConnectorRef, rtpOutRef);
 *     streamMedia_Start(rtpOutRef);
 *
 *     streamMedia_AddRtcpHandler(rtpInRef, MyRtcpEventHandler, NULL);
 *
 *     le_mcc_AddCallEventHandler(MyCallEventHandler, NULL);
 *     callRef=le_mcc_Create(destinationNumber);
 *     le_mcc_Start(callRef);
 * }
 *
 * @endcode
 *
 * This sample code shows how to connect RTP to microphone and speaker.
 *
 * @code
 *
 * // Peer 2 : connection between a RTP session and microphone and speaker.
 *
 * static void MyRtcpEventHandler
 * (
 *     le_audio_StreamRef_t          streamRef,
 *     streamMedia_RtcpEvent_t    event,
 *     void*                         contextPtr
 * )
 * {
 *     switch (event)
 *     {
 *         case STREAMMEDIA_RTCP_BYE:
 *             DisconnectAllAudio(); // Not showed here, would disconnect all audio.
 *             break;
 *         default:
 *             LE_INFO("Other event");
 *             break;
 *     }
 * }
 *
 * void RecordFileFromRtp
 * (
 *     void
 * )
 * {
 *     static le_audio_ConnectorRef_t  audioInputConnectorRef = NULL;
 *     static le_audio_ConnectorRef_t  audioOutputConnectorRef = NULL;
 *     static le_audio_StreamRef_t     rtpInRef = NULL;
 *     static le_audio_StreamRef_t     rtpOutRef = NULL;
 *     static le_audio_StreamRef_t     micAudioRef = NULL;
 *     static le_audio_StreamRef_t     speakerAudioRef = NULL;
 *     const char*                     remoteAddress = "10.40.58.2";
 *     int                             localPort = 4000;
 *     int                             remotePort = 4000;
 *
 *     rtpInRef = streamMedia_OpenAudioRtpRx(localPort);
 *     rtpOutRef = streamMedia_OpenAudioRtpTx(localPort, remoteAddress, remotePort);
 *
 *     micAudioRef = le_audio_OpenMic();
 *     speakerAudioRef = le_audio_OpenSpeaker();
 *
 *     audioInputConnectorRef = le_audio_CreateConnector();
 *     audioOutputConnectorRef = le_audio_CreateConnector();
 *
 *     le_audio_Connect(audioOutputConnectorRef, speakerAudioRef);
 *     le_audio_Connect(audioInputConnectorRef, micAudioRef);
 *
 *     le_audio_Connect(audioOutputConnectorRef, rtpInRef);
 *     streamMedia_Start(rtpInRef);
 *
 *     le_audio_Connect(audioInputConnectorRef, rtpOutRef);
 *     streamMedia_Start(rtpOutRef);
 *
 *     streamMedia_AddRtcpHandler(rtpInRef, MyRtcpEventHandler, NULL);
 * }
 *
 * @endcode
 *
 * @section streamMedia_warning Known limitations
 *
 * The RTP interface must be connected and disconnected in this specific order :
 * - First connect the RTP reception stream, and then the transmission one.
 * - When disconnecting it, start by the transmission stream, and then disconnect the reception one.
 *
 * The streamMedia API provided by the streamMedia sample application is not suitable for
 * real-time communication applications because of the too high latency.
 * A delay of 500 ms has been measured on a one-sided RTP session (RTP transmission only) on a WP85
 * module. Thus, the "Audio over IP" use case described above shows a latency of more than one
 * second due to the two-sided RTP session (transmission on one module and reception on the other).
 * However, the streamMedia API can be used for any streaming purposes where a delay of one second
 * is acceptable.
 *
 * The streamMedia application has been tested on WP85 and AR8652.
 *
 * <HR>
 *
 * Copyright (C) Sierra Wireless Inc.
 */
/**
 * @file streamMedia_interface.h
 *
 * Legato @ref sampleApps_streamMedia include file.
 *
 * Copyright (C) Sierra Wireless Inc.
 */

#ifndef STREAMMEDIA_INTERFACE_H_INCLUDE_GUARD
#define STREAMMEDIA_INTERFACE_H_INCLUDE_GUARD


#include "legato.h"

// Interface specific includes
#include "le_audio_interface.h"


//--------------------------------------------------------------------------------------------------
/**
 * Type for handler called when a server disconnects.
 */
//--------------------------------------------------------------------------------------------------
typedef void (*streamMedia_DisconnectHandler_t)(void *);

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
void streamMedia_ConnectService
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
le_result_t streamMedia_TryConnectService
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
void streamMedia_SetServerDisconnectHandler
(
    streamMedia_DisconnectHandler_t disconnectHandler,
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
void streamMedia_DisconnectService
(
    void
);


//--------------------------------------------------------------------------------------------------
/**
 * The maximum length of the textual representation of an IP address.
 */
//--------------------------------------------------------------------------------------------------
#define STREAMMEDIA_MAX_IP_LENGTH 15

//--------------------------------------------------------------------------------------------------
/**
 * The maximum length of the optional RTCP BYE packet reason string length.
 */
//--------------------------------------------------------------------------------------------------
#define STREAMMEDIA_MAX_BYE_REASON_LENGTH 255

//--------------------------------------------------------------------------------------------------
/**
 * The maximum length of a RTCP SDES packet field string length.
 */
//--------------------------------------------------------------------------------------------------
#define STREAMMEDIA_MAX_SDES_FIELD_LENGTH 255

//--------------------------------------------------------------------------------------------------
/**
 * The default RTP port.
 */
//--------------------------------------------------------------------------------------------------
#define STREAMMEDIA_DEFAULT_RTP_PORT 4000

//--------------------------------------------------------------------------------------------------
/**
 * RTCP packet reception events, related to the RTCP packet type. See
 * <a href="https://tools.ietf.org/pdf/rfc3550.pdf">RFC-3550</a> for more details.
 */
//--------------------------------------------------------------------------------------------------
typedef enum
{
    STREAMMEDIA_RTCP_SDES = 0,
        ///< RTCP Session Description packet has been received.
    STREAMMEDIA_RTCP_BYE = 1,
        ///< RTCP BYE packet has been received.
    STREAMMEDIA_RTCP_SR = 2,
        ///< RTCP Sender Report packet has been received.
    STREAMMEDIA_RTCP_RR = 3,
        ///< RTCP Receiver Report packet has been received.
    STREAMMEDIA_RTCP_APP = 4
        ///< RTCP Application-specific packet has been received.
}
streamMedia_RtcpEvent_t;


//--------------------------------------------------------------------------------------------------
/**
 * Reference type used by Add/Remove functions for EVENT 'streamMedia_Rtcp'
 */
//--------------------------------------------------------------------------------------------------
typedef struct streamMedia_RtcpHandler* streamMedia_RtcpHandlerRef_t;


//--------------------------------------------------------------------------------------------------
/**
 * Handler for RTCP packet reception.
 *
 */
//--------------------------------------------------------------------------------------------------
typedef void (*streamMedia_RtcpHandlerFunc_t)
(
    le_audio_StreamRef_t streamRef,
        ///< Audio stream reference.
    streamMedia_RtcpEvent_t event,
        ///< Event value.
    void* contextPtr
        ///<
);

//--------------------------------------------------------------------------------------------------
/**
 * Open the received audio stream of the RTP interface.
 *
 * @return Reference to the input audio stream, NULL if the function fails.
 */
//--------------------------------------------------------------------------------------------------
le_audio_StreamRef_t streamMedia_OpenAudioRtpRx
(
    int32_t localPort
        ///< [IN] The local UDP socket port for the RTP session.
);

//--------------------------------------------------------------------------------------------------
/**
 * Open the transmitted audio stream of the RTP interface.
 *
 * @return Reference to the output audio stream, NULL if the function fails.
 */
//--------------------------------------------------------------------------------------------------
le_audio_StreamRef_t streamMedia_OpenAudioRtpTx
(
    int32_t localPort,
        ///< [IN] The local UDP socket port for the RTP
        ///< session.
    const char* LE_NONNULL remoteIpAddress,
        ///< [IN] The IP address of the remote peer.
    int32_t remotePort
        ///< [IN] The port of the remote peer.
);

//--------------------------------------------------------------------------------------------------
/**
 * Start the RTP stream. This must be called after connecting the RTP interface to start
 * RTP reception or transmission.
 *
 * @return LE_FAULT         Function failed.
 * @return LE_OK            Function succeeded.
 */
//--------------------------------------------------------------------------------------------------
le_result_t streamMedia_Start
(
    le_audio_StreamRef_t streamRef
        ///< [IN] Audio stream reference.
);

//--------------------------------------------------------------------------------------------------
/**
 * Stop the RTP stream.
 *
 * @return LE_FAULT         Function failed.
 * @return LE_OK            Function succeeded.
 */
//--------------------------------------------------------------------------------------------------
le_result_t streamMedia_Stop
(
    le_audio_StreamRef_t streamRef
        ///< [IN] Audio stream reference.
);

//--------------------------------------------------------------------------------------------------
/**
 * Send a RTCP Session Description packet (SDES) using
 * <a href="http://www.pjsip.org/pjmedia/docs/html/group__PJMED__RTCP.htm#
 * ga8ddca87e2b3ab0b02635edd66d7de748">pjmedia_rtcp_build_rtcp_sdes()</a>.
 *
 * @return LE_FAULT         Function failed.
 * @return LE_OK            Function succeeded.
 */
//--------------------------------------------------------------------------------------------------
le_result_t streamMedia_SendRtcpSdes
(
    le_audio_StreamRef_t streamRef,
        ///< [IN] Audio stream reference.
    const char* LE_NONNULL cname,
        ///< [IN] The optional source canonical name.
    const char* LE_NONNULL name,
        ///< [IN] The optional source name.
    const char* LE_NONNULL email,
        ///< [IN] The optional source email.
    const char* LE_NONNULL phone,
        ///< [IN] The optional source phone.
    const char* LE_NONNULL loc,
        ///< [IN] The optional source location.
    const char* LE_NONNULL tool,
        ///< [IN] The optional source tool.
    const char* LE_NONNULL note
        ///< [IN] The optional source note.
);

//--------------------------------------------------------------------------------------------------
/**
 * Send a RTCP BYE packet using
 * <a href="http://www.pjsip.org/pjmedia/docs/html/group__PJMED__RTCP.htm#
 * ga9eb25597d5815cee68fe7fdc3b4cd9e9">pjmedia_rtcp_build_rtcp_bye()</a>.
 *
 * @return LE_FAULT         Function failed.
 * @return LE_OK            Function succeeded.
 */
//--------------------------------------------------------------------------------------------------
le_result_t streamMedia_SendRtcpBye
(
    le_audio_StreamRef_t streamRef,
        ///< [IN] Audio stream reference.
    const char* LE_NONNULL reason
        ///< [IN] The optional BYE reason.
);

//--------------------------------------------------------------------------------------------------
/**
 * Close an RTP stream.
 *
 */
//--------------------------------------------------------------------------------------------------
void streamMedia_Close
(
    le_audio_StreamRef_t streamRef
        ///< [IN] Audio stream reference.
);

//--------------------------------------------------------------------------------------------------
/**
 * Add handler function for EVENT 'streamMedia_Rtcp'
 *
 * This event provides information on RTCP packet type.
 *
 */
//--------------------------------------------------------------------------------------------------
streamMedia_RtcpHandlerRef_t streamMedia_AddRtcpHandler
(
    le_audio_StreamRef_t streamRef,
        ///< [IN] Audio stream reference.
    streamMedia_RtcpHandlerFunc_t handlerPtr,
        ///< [IN]
    void* contextPtr
        ///< [IN]
);

//--------------------------------------------------------------------------------------------------
/**
 * Remove handler function for EVENT 'streamMedia_Rtcp'
 */
//--------------------------------------------------------------------------------------------------
void streamMedia_RemoveRtcpHandler
(
    streamMedia_RtcpHandlerRef_t handlerRef
        ///< [IN]
);

#endif // STREAMMEDIA_INTERFACE_H_INCLUDE_GUARD