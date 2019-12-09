

/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */

/**
 * @page c_audio Audio
 *
 * @ref le_audio_interface.h "API Reference" <br>
 * @ref platformConstraintsAudio Constraints
 *
 * The Audio API handles audio interfaces including play and record supported formats.
 *
 * A Legato device can use several audio interfaces. You choose the input and output
 * interfaces to tie together. The Audio stream related to a particular interface is
 * represented with an 'Audio Stream Reference'.
 *
 * You can create your own audio path by connecting several audio streams together using
 * audio connectors.
 *
 * An audio path can support more than two audio interfaces. You can have a basic output audio
 * path of a voice call to connect the Modem Voice Received interface with the Speaker
 * interface, and at the same time, the Modem Voice Received interface can be also connected to a
 * Recorder Device interface.
 *
 * @section le_audio_binding IPC interfaces binding
 *
 * All the functions of this API are provided by the @b audioService.
 *
 * @note The functions that are highly dependent on the initialization of the lower layers of the
 * @b audioService will return LE_UNAVAILABLE error code if those layers failed to initialize.
 * Recovery can be attempted by restarting the service.
 *
 * Here's a code sample binding to audio services:
 * @verbatim
   bindings:
   {
      clientExe.clientComponent.le_audio -> audioService.le_audio
   }
   @endverbatim
 *
 * @section le_audio_configuration Configure the Audio
 *
 * The audio profile can be set with the le_audio_SetProfile() function.
 *
 * @warning Ensure to check the number of supported audio profiles for your specific platform.
 *
 * An audio profile can be retrieved with le_audio_GetProfile() and set with le_audio_SetProfile().
 *
 * Then, the following functions let you enable or disable the audio settings on the selected audio
 * interface:
 * - le_audio_EnableNoiseSuppressor()/le_audio_DisableNoiseSuppressor(): Noise Suppressor.
 * - le_audio_EnableEchoCanceller()/ le_audio_DisableEchoCanceller(): Echo Canceller.
 * - le_audio_EnableFirFilter()/ le_audio_DisableFirFilter(): downlink FIR Filter (Finite Impulse
 *   Response).
 * - le_audio_EnableIirFilter()/ le_audio_DisableIirFilter(): downlink IIR Filter (Infinite Impulse
 *   Response).
 * - le_audio_EnableAutomaticGainControl()/ le_audio_DisableAutomaticGainControl(): automatic
 * gain on the selected audio stream.
 *
 * - le_audio_IsNoiseSuppressorEnabled(): To get the status of Noise Suppressor.
 * - le_audio_IsEchoCancellerEnabled(): To get the status of Echo Canceller.
 *
 * To configure the encoding format, use le_audio_GetEncodingFormat() and le_audio_SetEncodingFormat().
 *
 * To configure gain settings, use le_audio_GetGain() and le_audio_SetGain().
 *
 * PCM has the following configuration get/set functions:
 * - le_audio_GetPcmSamplingRate() and le_audio_SetPcmSamplingRate() for Hz rate.
 * - le_audio_GetPcmSamplingResolution() and le_audio_SetPcmSamplingResolution() for bit/sample settings.
 * - le_audio_GetPcmCompanding() and le_audio_SetPcmCompanding() for signal settings.
 * - le_audio_GetSamplePcmChannelNumber() and le_audio_SetSamplePcmChannelNumber() for channel numbers.
 *
 * To Mute/Unmute the Call Waiting Tone to alert for an incoming voice call when a voice call is
 * already in progress, use le_audio_MuteCallWaitingTone() and le_audio_UnmuteCallWaitingTone().
 *
 * @note The Call Waiting Supplementary Service must be enabled.
 *
 * @section le_audio_interfaces Open/Close an Audio Interface
 *
 * The following functions let you select the desired interface attributes:
 * - le_audio_OpenMic(): returns an Audio Stream Reference of the analog audio signal coming from
 *                       the microphone input.
 * - le_audio_OpenSpeaker(): returns an Audio Stream Reference of the analog audio signal routed
 *                           to the Speaker output.
 * - le_audio_OpenUsbRx(): returns an Audio Stream Reference of the digitized audio signal coming
 *                         from an external device connected via USB Audio Class.
 * - le_audio_OpenUsbTx(): returns an Audio Stream Reference of the digitized audio signal routed
 *                         to an external device connected via USB Audio Class.
 * - le_audio_OpenPcmRx(): it returns an Audio Stream Reference of the digitized audio signal
 *                            coming from an external device connected via the PCM interface.
 * - le_audio_OpenPcmTx(): it returns an Audio Stream Reference of the digitized audio signal
 *                            routed to an external device connected via the PCM interface.
 * - le_audio_OpenI2sRx(): it returns an Audio Stream Reference of the digitized audio signal
 *                            coming from an external device connected via the I2S interface.
 * - le_audio_OpenI2sTx(): it returns an Audio Stream Reference of the digitized audio signal
 *                            routed to an external device connected via the I2S interface.
 * - le_audio_OpenModemVoiceRx(): returns an Audio Stream Reference of the digitized audio signal
 *                                coming from a voice call. The audio format is negotiated with the
 *                                network when the call is established.
 * - le_audio_OpenModemVoiceTx(): returns an Audio Stream Reference of the digitized audio signal
 *                                routed to a voice call. The audio format is negotiated with the
 *                                network when the call is established.
 *
 * Multiple users can own the same stream at the same time.
 *
 * le_audio_GetDefaultPcmTimeSlot() can be called to get the default PCM time slot used on the
 * current platform.
 *
 * le_audio_GetDefaultI2sMode() can be called to get the default I2s channel mode slot used on
 * the current platform.
 *
 * Call le_audio_Close() to release it. If
 * several users own the same, corresponding stream reference, the interface will
 * close only after the last user releases the audio stream.
 *
 * You can configure the PCM interface with the le_audio_SetPcmSamplingRate(),
 * le_audio_SetPcmSamplingResolution() and le_audio_SetPcmCompanding() functions.
 * This function must be called before activating an audio path with the PCM interface, in other
 * words you must call this function before connecting the PCM Stream to a connector.
 *
 * In addition, the le_audio_GetPcmSamplingRate(), le_audio_GetPcmSamplingResolution() and
 * le_audio_GetPcmCompanding() functions allows you to retrieve the PCM interface configuration.
 *
 * @note Opening a Legato audio stream doesn’t necessarily interact with the audio HW interface.
 * On most platforms, it only allocates a context in memory. The audio path becomes active when the
 * streams are plugged into a connector.
 *
 *
 * @section le_audio_streams Control an Audio Stream
 *
 * Once the users get an Audio Stream reference, they can control it with the following functions:
 *
 * - le_audio_SetGain(): adjust the gain of an audio stream (gain value is specific to the
 *                       platform).
 * - le_audio_GetGain(): retrieve the gain of an audio stream (gain value is specific to the
 *                       platform).
 * - le_audio_Mute(): mute an audio stream.
 * - le_audio_Unmute(): unmute an audio stream.
 *
 * @note Multimedia (playback and recording) must be controlled separately from the main audio path
 * (Microphone/Speaker, I2S, PCM, USB).  Muting/Unmuting a multimedia is done by
 * muting/unmuting the multimedia stream, and not the other connected stream. For example, in case
 * of playback + voice on the speaker, if the user wants to mute all the audio coming out of the
 * speaker, it must mute both the Speaker stream and the playback stream.
 *
 * @warning Ensure to check the list of supported audio streams for these functions on your
 * specific platform.
 *
 * In the case your platform can support other gains in your audio subsystem, you can set or get
 * the value of them with the following functions:
 *
 * - le_audio_SetPlatformSpecificGain(): adjust the value of a platform specific gain in the audio
 *                                       subsystem.
 * - le_audio_GetPlatformSpecificGain(): retrieve the value of a platform specific gain in the
 *                                       audio subsystem.
 *
 * @warning Ensure to check the names of supported gains for your specific platform.
 *
 * @section le_audio_connectors Create Audio connectors
 *
 * You can create your own audio path by connecting several audio streams together.
 *
 * le_audio_CreateConnector() function creates a reference to an audio connector.
 *
 * You can tie an audio stream to a connector by calling the le_audio_Connect()
 * function.
 *
 * You can remove an audio stream from a connector by calling the le_audio_Disconnect()
 * function.
 *
 * When finished with it, delete it using the le_audio_DeleteConnector() function.
 *
 * The following image shows how to connect a Player stream to play a file towards a remote end
 * during a voice call.
 * @image html AudioConnector.png
 *
 * A sample code that implements audio connectors and audio streams during voice call can be found in
 * \b audioMccTest.c file (please refer to @ref c_audioMcc page).
 *
 * @note It is recommended to set the connectors before the connected streams are used.
 * In particular, a connector using Modem Voice Rx or Tx interfaces has to be created before dialing
 * or answering the call if the application needs to customize the audio path.
 *
 * @section le_audio_pb_playback Playback
 *
 * An audio file can be played to any active output interfaces.
 *
 * Open a player interface by calling:
 * - le_audio_OpenPlayer(): returns an Audio Stream Reference for file playback.
 *                          An audio file can be played on the local audio interface like
 *                          Speaker, USB Tx, PCM Tx, I2S Tx or on the remote audio
 *                          interface Modem Voice Tx depending the kind of connector
 *                          (input or output) is tied to.
 *
 * - le_audio_PlayFile(): plays a specified file. WAVE (Waveform audio file) and
 * AMR (Adaptive Multi Rate) are supported.
 * AMR is an audio compression format optimized for speech coding. Two codecs are supported:
 *     - AMR_NB (AMR Narrowband) codec that encodes narrowband (200–3400 Hz) signals at variable bit
 * rates ranging from 4.75 to 12.2 kbit/s. It was adopted as the standard speech codec by 3GPP.
 *     - AMR-WB (AMR Wideband) is an ITU-T standard speech codec which improved speech quality due to a
 * wider speech bandwidth of 50–7000 Hz.
 *
 * - le_audio_PlaySamples(): initiates a playback using an audio flow. A pipe has
 * to be opened first, then the PCM samples are sent through the opened pipe.
 * A play can be done only on a connected stream. For instance, the
 * "I2S Tx", "Modem Voice Rx" and "Player" must be previously connected before playing a file.
 * If there are no more PCM samples to be played, the playback must be stopped by calling
 * le_audio_Stop().
 *
 * @section le_audio_pb_rec Record
 *
 *  * Audio file recording can be done from any active input interface.
 *
 * Open a "File Recording" interface by calling:
 * - le_audio_OpenRecorder(): returns an Audio Stream Reference for file recording.
 *                            The local audio interface like Microphone, USB Rx, PCM Rx,
 *                            I2S Rx is recorded into an audio file; or the Modem Voice Rx
 *                            remote audio interface is recorded into an audio file,
 *                            depending the kind of connector (input or output) is tied to.
 * - le_audio_RecordFile(): records in a specified file.
 * - le_audio_SetEncodingFormat(): sets the encoding format. The same formats as the
 * player are supported.
 * - le_audio_GetEncodingFormat(): gets the encoding format.
 * - le_audio_GetSamples(): records the audio PCM samples.  A pipe has
 * to be opened first, then the PCM samples are sent through the opened pipe.
 * Recording can only be done on a connected stream. For example, the
 * "I2S Rx", "Modem Voice Tx" and "Recorder" must be previously connected before recording a file.
 * If there are no more PCM samples to be retrieved, the recording must be stopped by calling
 * le_audio_Stop().
 *
 * A PCM configuration must be set with:
 * - le_audio_SetSamplePcmChannelNumber(): sets the channel number of a PCM
 * sample.
 * - le_audio_SetSamplePcmSamplingRate(): sets the sampling rate of a PCM
 * sample.
 * - le_audio_SetSamplePcmSamplingResolution(): sets the sampling resolution
 * (in bits per sample) of a PCM sample.
 *
 * The PCM samples configuration can be retrieved with:
 *      - le_audio_GetSamplePcmChannelNumber(): gets the channel number of a PCM
 * sample.
 *      - le_audio_GetSamplePcmSamplingRate(): gets the sampling rate of a PCM
 * sample.
 *      - le_audio_GetSamplePcmSamplingResolution(): can be called to get the sampling resolution
 * (in bits per sample) of a PCM sample.
 * The default configuration is PCM 16-bit audio @ 8KHz one channel.
 *
 * An AMR configuration must be set with:
 *      - le_audio_SetSampleAmrMode(): sets the AMR mode (NB/WB, bitrate).
 *      - le_audio_SetSampleAmrDtx(): can be called to activate/deactivate the Discontinuous
 * Transmission (DTX) to reduce bandwidth usage during silence periods.
 * The AMR configuration can be retrieved with:
 *      - le_audio_GetSampleAmrMode(): gets the AMR mode.
 *      - le_audio_GetSampleAmrDtx(): gets the Discontinuous
 * Transmission (DTX).
 *
 * To stop a play/record call le_audio_Stop():
 * - The playback/record stops playing/recording, and the read/write position indicator associated
 * with the file stream is rewound to the beginning of the file. A new file can be played/recorded
 * using le_audio_PlayFile()/le_audio_PlaySamples()/le_audio_RecordFile()/le_audio_GetSamples().
 * - le_audio_Pause(): can be called to pause a play/record. The file playing/recording is put on
 * hold, the read/write position indicator of the file is not moved.
 * - le_audio_Resume(): can be called to resume a paused play/record. The file playing/recording
 * continues at the file's position indicator held after the pause.
 * - le_audio_Flush(): can be called to flush the remaining audio samples before sending
 * them to the audio driver.
 *
 * You can also register a handler function for media-related notifications like errors or audio
 * events.
 *
 * le_audio_AddMediaHandler() function installs a handler for player/recorder stream
 * notifications.
 *
 * le_audio_RemoveMediaHandler() function removes the player/recorder handler function.
 *
 * @note The @c LE_AUDIO_MEDIA_NO_MORE_SAMPLES event indicates when all samples put into the pipe
 * by the user's App have been sent to the audio driver (see le_audio_PlaySamples()).
 *
 * A sample code that implements audio playback and capture can be found in \b audioPlaybackRec.c
 * file (please refer to @ref c_audioCapturePlayback page).
 *
 * @section le_audio_dtmf DTMF
 *
 * The le_audio_PlayDtmf() function allows the application to play one or several DTMF on a playback
 * stream. The duration and the pause of the DTMFs must also be specified with the input parameters.
 *
 * The le_audio_PlaySignallingDtmf() function allows the application to ask the Mobile Network to
 * generate on the remote audio party the DTMFs. Compared with le_audio_PlayDtmf(),
 * le_audio_PlaySignallingDtmf() function may offer a better signal quality, but the the duration
 * and the pause timings may be less accurate.
 *
 * The application must register a handler function to detect incoming DTMF characters on a specific
 * input audio stream. The le_audio_AddDtmfDetectorHandler() function installs a handler for DTMF
 * detection.
 *
 * The le_audio_RemoveDtmfDetectorHandler() function uninstalls the handler function.
 *
 * The DTMFs are: 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, *, #, A, B, C, D. Not case sensitive.
 *
 * @note The DTMF decoding works only on an active audio path.
 *
 * A sample code that implements DTMF playback and decoding can be found in \b audioDtmfTest.c file
 * (please refer to @ref c_audiodtmf page).
 *
 * <HR>
 *
 * Copyright (C) Sierra Wireless Inc.
 */
/**
 * @file le_audio_interface.h
 *
 * Legato @ref c_audio include file.
 *
 * Copyright (C) Sierra Wireless Inc.
 */
/**
 * @page c_audioMcc Sample code of audio settings for a dialing call
 *
 * @include "apps/test/audio/audioMcc/audioMccComp/audioMccTest.c"
 */
/**
 * @page c_audiodtmf Sample code of dtmf playback and decoding
 *
 * @include "apps/test/audio/dtmf/dtmfTestComp/audioDtmfTest.c"
 */
/**
 * @page c_audioCapturePlayback Sample code of audio playback and capture
 *
 * @include "apps/test/audio/audioPlaybackRec/audioPlaybackRecComp/audioPlaybackRec.c"
 */

#ifndef LE_AUDIO_INTERFACE_H_INCLUDE_GUARD
#define LE_AUDIO_INTERFACE_H_INCLUDE_GUARD


#include "legato.h"


//--------------------------------------------------------------------------------------------------
/**
 * Type for handler called when a server disconnects.
 */
//--------------------------------------------------------------------------------------------------
typedef void (*le_audio_DisconnectHandler_t)(void *);

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
void le_audio_ConnectService
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
le_result_t le_audio_TryConnectService
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
void le_audio_SetServerDisconnectHandler
(
    le_audio_DisconnectHandler_t disconnectHandler,
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
void le_audio_DisconnectService
(
    void
);


//--------------------------------------------------------------------------------------------------
/**
 * Maximum DTMF string length.
 */
//--------------------------------------------------------------------------------------------------
#define LE_AUDIO_DTMF_MAX_LEN 100

//--------------------------------------------------------------------------------------------------
/**
 * Maximum DTMF string length.
 * One extra byte is added for the null character.
 */
//--------------------------------------------------------------------------------------------------
#define LE_AUDIO_DTMF_MAX_BYTES 101

//--------------------------------------------------------------------------------------------------
/**
 * No file descriptor define
 */
//--------------------------------------------------------------------------------------------------
#define LE_AUDIO_NO_FD -1

//--------------------------------------------------------------------------------------------------
/**
 * Maximum gain name string length.
 */
//--------------------------------------------------------------------------------------------------
#define LE_AUDIO_GAIN_NAME_MAX_LEN 100

//--------------------------------------------------------------------------------------------------
/**
 * Maximum gain name string length.
 * One extra byte is added for the null character.
 */
//--------------------------------------------------------------------------------------------------
#define LE_AUDIO_GAIN_NAME_MAX_BYTES 101

//--------------------------------------------------------------------------------------------------
/**
 * Audio recording/playback events.
 */
//--------------------------------------------------------------------------------------------------
typedef enum
{
    LE_AUDIO_MEDIA_ENDED = 0,
        ///< Playback has reached the end of file.
    LE_AUDIO_MEDIA_NO_MORE_SAMPLES = 1,
        ///< No more audio samples to play.
    LE_AUDIO_MEDIA_ERROR = 2,
        ///< Recording/playback has encountered an error.
    LE_AUDIO_MEDIA_MAX = 3
        ///< Enumerate max value.
}
le_audio_MediaEvent_t;


//--------------------------------------------------------------------------------------------------
/**
 * Companding.
 */
//--------------------------------------------------------------------------------------------------
typedef enum
{
    LE_AUDIO_COMPANDING_ALAW = 0,
        ///< A-law PCM (i.e. Europe and most of the rest of the world).
    LE_AUDIO_COMPANDING_ULAW = 1,
        ///< MU-law PCM (i.e. North America and Japan).
    LE_AUDIO_COMPANDING_NONE = 2
        ///< Linear PCM.
}
le_audio_Companding_t;


//--------------------------------------------------------------------------------------------------
/**
 * I2S channel mode.
 */
//--------------------------------------------------------------------------------------------------
typedef enum
{
    LE_AUDIO_I2S_LEFT = 0,
        ///< Left channel.
    LE_AUDIO_I2S_RIGHT = 1,
        ///< Right channel.
    LE_AUDIO_I2S_MONO = 2,
        ///< Mono mode.
    LE_AUDIO_I2S_STEREO = 3,
        ///< Stereo mode.
    LE_AUDIO_I2S_REVERSE = 4
        ///< Reverse mode (left & right reversed).
}
le_audio_I2SChannel_t;


//--------------------------------------------------------------------------------------------------
/**
 * Audio format.
 */
//--------------------------------------------------------------------------------------------------
typedef enum
{
    LE_AUDIO_WAVE = 0,
        ///< Waveform audio file
    LE_AUDIO_AMR = 1,
        ///< Adaptive Multi Rate
    LE_AUDIO_FORMAT_MAX = 2
        ///<
}
le_audio_Format_t;


//--------------------------------------------------------------------------------------------------
/**
 * AMR mode.
 */
//--------------------------------------------------------------------------------------------------
typedef enum
{
    LE_AUDIO_AMR_NONE = 0,
        ///< Not set
    LE_AUDIO_AMR_NB_4_75_KBPS = 1,
        ///< AMR Narrowband in 4.75kbps
    LE_AUDIO_AMR_NB_5_15_KBPS = 2,
        ///< AMR Narrowband in 5.15kbps
    LE_AUDIO_AMR_NB_5_9_KBPS = 3,
        ///< AMR Narrowband in 5.9kbps
    LE_AUDIO_AMR_NB_6_7_KBPS = 4,
        ///< AMR Narrowband in 6.7kbps
    LE_AUDIO_AMR_NB_7_4_KBPS = 5,
        ///< AMR Narrowband in 7.4kbps (default value)
    LE_AUDIO_AMR_NB_7_95_KBPS = 6,
        ///< AMR Narrowband in 7.95kbps
    LE_AUDIO_AMR_NB_10_2_KBPS = 7,
        ///< AMR Narrowband in 10.3kbps
    LE_AUDIO_AMR_NB_12_2_KBPS = 8,
        ///< AMR Narrowband in 12.2kbps
    LE_AUDIO_AMR_WB_6_6_KBPS = 9,
        ///< AMR Wideband in 6.6kbps
    LE_AUDIO_AMR_WB_8_85_KBPS = 10,
        ///< AMR Wideband in 8.85kbps
    LE_AUDIO_AMR_WB_12_65_KBPS = 11,
        ///< AMR Wideband in 12.65kbps
    LE_AUDIO_AMR_WB_14_25_KBPS = 12,
        ///< AMR Wideband in 14.25kbps
    LE_AUDIO_AMR_WB_15_85_KBPS = 13,
        ///< AMR Wideband in 15.85kbps
    LE_AUDIO_AMR_WB_18_25_KBPS = 14,
        ///< AMR Wideband in 18.25kbps
    LE_AUDIO_AMR_WB_19_85_KBPS = 15,
        ///< AMR Wideband in 19.85kbps
    LE_AUDIO_AMR_WB_23_05_KBPS = 16,
        ///< AMR Wideband in 23.05kbps
    LE_AUDIO_AMR_WB_23_85_KBPS = 17
        ///< AMR Wideband in 23.85kbps
}
le_audio_AmrMode_t;


//--------------------------------------------------------------------------------------------------
/**
 * Reference type for Audio Stream
 */
//--------------------------------------------------------------------------------------------------
typedef struct le_audio_Stream* le_audio_StreamRef_t;


//--------------------------------------------------------------------------------------------------
/**
 * Reference type for Audio Connector
 */
//--------------------------------------------------------------------------------------------------
typedef struct le_audio_Connector* le_audio_ConnectorRef_t;


//--------------------------------------------------------------------------------------------------
/**
 * Reference type used by Add/Remove functions for EVENT 'le_audio_Media'
 */
//--------------------------------------------------------------------------------------------------
typedef struct le_audio_MediaHandler* le_audio_MediaHandlerRef_t;


//--------------------------------------------------------------------------------------------------
/**
 * Reference type used by Add/Remove functions for EVENT 'le_audio_DtmfDetector'
 */
//--------------------------------------------------------------------------------------------------
typedef struct le_audio_DtmfDetectorHandler* le_audio_DtmfDetectorHandlerRef_t;


//--------------------------------------------------------------------------------------------------
/**
 * Handler for audio stream event.
 *
 */
//--------------------------------------------------------------------------------------------------
typedef void (*le_audio_MediaHandlerFunc_t)
(
    le_audio_StreamRef_t streamRef,
        ///< Audio stream reference.
    le_audio_MediaEvent_t event,
        ///< Event value.
    void* contextPtr
        ///<
);

//--------------------------------------------------------------------------------------------------
/**
 * Handler for DTMF decoding.
 *
 */
//--------------------------------------------------------------------------------------------------
typedef void (*le_audio_DtmfDetectorHandlerFunc_t)
(
    le_audio_StreamRef_t streamRef,
        ///< Audio stream reference.
    char dtmf,
        ///< Detected DTMF.
    void* contextPtr
        ///<
);

//--------------------------------------------------------------------------------------------------
/**
 * Open the Microphone.
 *
 * @return Reference to the input audio stream, NULL if the function fails.
 */
//--------------------------------------------------------------------------------------------------
le_audio_StreamRef_t le_audio_OpenMic
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * Open the Speaker-phone.
 *
 * @return Reference to the output audio stream, NULL if the function fails.
 */
//--------------------------------------------------------------------------------------------------
le_audio_StreamRef_t le_audio_OpenSpeaker
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * Open the received audio stream of an USB audio class.
 *
 * @return Reference to the input audio stream, NULL if the function fails.
 */
//--------------------------------------------------------------------------------------------------
le_audio_StreamRef_t le_audio_OpenUsbRx
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * Open the transmitted audio stream of an USB audio class.
 *
 * @return Reference to the output audio stream, NULL if the function fails.
 */
//--------------------------------------------------------------------------------------------------
le_audio_StreamRef_t le_audio_OpenUsbTx
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * Open the received audio stream of the PCM interface.
 *
 * @return Reference to the input audio stream, NULL if the function fails.
 */
//--------------------------------------------------------------------------------------------------
le_audio_StreamRef_t le_audio_OpenPcmRx
(
    uint32_t timeslot
        ///< [IN] The time slot number.
);

//--------------------------------------------------------------------------------------------------
/**
 * Open the transmitted audio stream of the PCM interface.
 *
 * @return Reference to the output audio stream, NULL if the function fails.
 */
//--------------------------------------------------------------------------------------------------
le_audio_StreamRef_t le_audio_OpenPcmTx
(
    uint32_t timeslot
        ///< [IN] The time slot number.
);

//--------------------------------------------------------------------------------------------------
/**
 * Open the received audio stream of the I2S interface.
 *
 * @return Reference to the input audio stream, NULL if the function fails.
 */
//--------------------------------------------------------------------------------------------------
le_audio_StreamRef_t le_audio_OpenI2sRx
(
    le_audio_I2SChannel_t mode
        ///< [IN] The channel mode.
);

//--------------------------------------------------------------------------------------------------
/**
 * Open the transmitted audio stream of the I2S interface.
 *
 * @return Reference to the output audio stream, NULL if the function fails.
 */
//--------------------------------------------------------------------------------------------------
le_audio_StreamRef_t le_audio_OpenI2sTx
(
    le_audio_I2SChannel_t mode
        ///< [IN] The channel mode.
);

//--------------------------------------------------------------------------------------------------
/**
 * Open the audio stream for playback.
 *
 * @return Reference to the audio stream, NULL if the function fails.
 */
//--------------------------------------------------------------------------------------------------
le_audio_StreamRef_t le_audio_OpenPlayer
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * Open the audio stream for recording.
 *
 * @return Reference to the audio stream, NULL if the function fails.
 */
//--------------------------------------------------------------------------------------------------
le_audio_StreamRef_t le_audio_OpenRecorder
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * Open the received audio stream of a voice call.
 *
 * @return Reference to the input audio stream, NULL if the function fails.
 */
//--------------------------------------------------------------------------------------------------
le_audio_StreamRef_t le_audio_OpenModemVoiceRx
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * Open the transmitted audio stream of a voice call.
 *
 * @return Reference to the output audio stream, NULL if the function fails.
 */
//--------------------------------------------------------------------------------------------------
le_audio_StreamRef_t le_audio_OpenModemVoiceTx
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * Add handler function for EVENT 'le_audio_Media'
 *
 * This event provides information on player / recorder stream events.
 *
 */
//--------------------------------------------------------------------------------------------------
le_audio_MediaHandlerRef_t le_audio_AddMediaHandler
(
    le_audio_StreamRef_t streamRef,
        ///< [IN] The audio stream reference.
    le_audio_MediaHandlerFunc_t handlerPtr,
        ///< [IN]
    void* contextPtr
        ///< [IN]
);

//--------------------------------------------------------------------------------------------------
/**
 * Remove handler function for EVENT 'le_audio_Media'
 */
//--------------------------------------------------------------------------------------------------
void le_audio_RemoveMediaHandler
(
    le_audio_MediaHandlerRef_t handlerRef
        ///< [IN]
);

//--------------------------------------------------------------------------------------------------
/**
 * Close an audio stream.
 * If several users own the stream reference, the interface closes only after
 * the last user closes the audio stream.
 *
 * @note If the caller is passing a bad reference into this function, it is a fatal error, the
 *       function will not return.
 */
//--------------------------------------------------------------------------------------------------
void le_audio_Close
(
    le_audio_StreamRef_t streamRef
        ///< [IN] Audio stream reference.
);

//--------------------------------------------------------------------------------------------------
/**
 * Set the Gain value of an input or output stream.
 *
 * @return LE_BAD_PARAMETER The audio stream reference is invalid.
 * @return LE_OUT_OF_RANGE  The gain value is out of range.
 * @return LE_UNAVAILABLE   The audio service initialization failed.
 * @return LE_FAULT         On any other failure.
 * @return LE_OK            The function succeeded.
 *
 * @note If the caller is passing a bad reference into this function, it is a fatal error, the
 *       function will not return.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_audio_SetGain
(
    le_audio_StreamRef_t streamRef,
        ///< [IN] Audio stream reference.
    int32_t gain
        ///< [IN] Gain value (specific to the platform)
);

//--------------------------------------------------------------------------------------------------
/**
 * Get the Gain value of an input or output stream.
 *
 * @return LE_BAD_PARAMETER The audio stream reference is invalid.
 * @return LE_UNAVAILABLE   The audio service initialization failed.
 * @return LE_FAULT         On any other failure.
 * @return LE_OK            The function succeeded.
 *
 * @note If the caller is passing a bad reference into this function, it is a fatal error, the
 *       function will not return.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_audio_GetGain
(
    le_audio_StreamRef_t streamRef,
        ///< [IN] Audio stream reference.
    int32_t* gainPtr
        ///< [OUT] Gain value (specific to the platform)
);

//--------------------------------------------------------------------------------------------------
/**
 * Mute an audio stream.
 *
 * @return LE_BAD_PARAMETER The audio stream reference is invalid.
 * @return LE_UNAVAILABLE   The audio service initialization failed.
 * @return LE_FAULT         On any other failure.
 * @return LE_OK            The function succeeded.
 *
 * @note If the caller is passing a bad reference into this function, it is a fatal error, the
 *       function will not return.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_audio_Mute
(
    le_audio_StreamRef_t streamRef
        ///< [IN] Audio stream reference.
);

//--------------------------------------------------------------------------------------------------
/**
 * Unmute an audio stream.
 *
 * @return LE_BAD_PARAMETER The audio stream reference is invalid.
 * @return LE_UNAVAILABLE   The audio service initialization failed.
 * @return LE_FAULT         On any other failure.
 * @return LE_OK            The function succeeded.
 *
 * @note If the caller is passing a bad reference into this function, it is a fatal error, the
 *       function will not return.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_audio_Unmute
(
    le_audio_StreamRef_t streamRef
        ///< [IN] Audio stream reference.
);

//--------------------------------------------------------------------------------------------------
/**
 * Create an audio connector reference.
 *
 * @return Reference to the audio connector, NULL if the function fails.
 */
//--------------------------------------------------------------------------------------------------
le_audio_ConnectorRef_t le_audio_CreateConnector
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * Delete an audio connector reference.
 *
 * @note If the caller is passing a bad reference into this function, it is a fatal error, the
 *       function will not return.
 */
//--------------------------------------------------------------------------------------------------
void le_audio_DeleteConnector
(
    le_audio_ConnectorRef_t connectorRef
        ///< [IN] Connector reference.
);

//--------------------------------------------------------------------------------------------------
/**
 * Connect an audio stream to the connector reference.
 *
 * @return LE_BUSY          There are insufficient DSP resources available.
 * @return LE_UNAVAILABLE   The audio service initialization failed.
 * @return LE_BAD_PARAMETER The connector and/or the audio stream references are invalid.
 * @return LE_FAULT         On any other failure.
 * @return LE_OK            The function succeeded.
 *
 * @note If the caller is passing a bad reference into this function, it is a fatal error, the
 *       function will not return.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_audio_Connect
(
    le_audio_ConnectorRef_t connectorRef,
        ///< [IN] Connector reference.
    le_audio_StreamRef_t streamRef
        ///< [IN] Audio stream reference.
);

//--------------------------------------------------------------------------------------------------
/**
 * Disconnect an audio stream from the connector reference.
 *
 * @note If the caller is passing a bad reference into this function, it is a fatal error, the
 *       function will not return.
 */
//--------------------------------------------------------------------------------------------------
void le_audio_Disconnect
(
    le_audio_ConnectorRef_t connectorRef,
        ///< [IN] Connector reference.
    le_audio_StreamRef_t streamRef
        ///< [IN] Audio stream reference.
);

//--------------------------------------------------------------------------------------------------
/**
 * Add handler function for EVENT 'le_audio_DtmfDetector'
 *
 * This event provides information on DTMF decoding for the specified streamRef
 *
 */
//--------------------------------------------------------------------------------------------------
le_audio_DtmfDetectorHandlerRef_t le_audio_AddDtmfDetectorHandler
(
    le_audio_StreamRef_t streamRef,
        ///< [IN] Audio stream reference.
    le_audio_DtmfDetectorHandlerFunc_t handlerPtr,
        ///< [IN]
    void* contextPtr
        ///< [IN]
);

//--------------------------------------------------------------------------------------------------
/**
 * Remove handler function for EVENT 'le_audio_DtmfDetector'
 */
//--------------------------------------------------------------------------------------------------
void le_audio_RemoveDtmfDetectorHandler
(
    le_audio_DtmfDetectorHandlerRef_t handlerRef
        ///< [IN]
);

//--------------------------------------------------------------------------------------------------
/**
 * This function must be called to enable the Noise Suppressor.
 *
 * @return LE_BAD_PARAMETER The audio stream reference is invalid.
 * @return LE_UNAVAILABLE   The audio service initialization failed.
 * @return LE_FAULT         On any other failure.
 * @return LE_OK            Function succeeded.
 *
 * @note The process exits, if an invalid audio stream reference is given.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_audio_EnableNoiseSuppressor
(
    le_audio_StreamRef_t streamRef
        ///< [IN] Audio stream reference.
);

//--------------------------------------------------------------------------------------------------
/**
 * This function must be called to disable the Noise Suppressor.
 *
 * @return LE_BAD_PARAMETER The audio stream reference is invalid.
 * @return LE_UNAVAILABLE   The audio service initialization failed.
 * @return LE_FAULT         On any other failure.
 * @return LE_OK            Function succeeded.
 *
 * @note The process exits, if an invalid audio stream reference is given.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_audio_DisableNoiseSuppressor
(
    le_audio_StreamRef_t streamRef
        ///< [IN] Audio stream reference.
);

//--------------------------------------------------------------------------------------------------
/**
 * This function must be called to enable the Echo Canceller.
 *
 * @return LE_BAD_PARAMETER The audio stream reference is invalid.
 * @return LE_UNAVAILABLE   The audio service initialization failed.
 * @return LE_FAULT         On any other failure.
 * @return LE_OK            Function succeeded.
 *
 * @note The process exits, if an invalid audio stream reference is given.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_audio_EnableEchoCanceller
(
    le_audio_StreamRef_t streamRef
        ///< [IN] Audio stream reference.
);

//--------------------------------------------------------------------------------------------------
/**
 * This function must be called to disable the Echo Canceller.
 *
 * @return LE_BAD_PARAMETER The audio stream reference is invalid.
 * @return LE_UNAVAILABLE   The audio service initialization failed.
 * @return LE_FAULT         On any other failure.
 * @return LE_OK            Function succeeded.
 *
 * @note The process exits, if an invalid audio stream reference is given.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_audio_DisableEchoCanceller
(
    le_audio_StreamRef_t streamRef
        ///< [IN] Audio stream reference.
);

//--------------------------------------------------------------------------------------------------
/**
 * This function must be called to get the status of Noise Suppressor.
 *
 * @return LE_BAD_PARAMETER The audio stream reference is invalid.
 * @return LE_UNAVAILABLE   The audio service initialization failed.
 * @return LE_FAULT         On any other failure.
 * @return LE_OK            Function succeeded.
 *
 * @note The process exits, if an invalid audio stream reference is given.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_audio_IsNoiseSuppressorEnabled
(
    le_audio_StreamRef_t streamRef,
        ///< [IN] Audio stream reference.
    bool* statusPtr
        ///< [OUT] true if NS is enabled, false otherwise
);

//--------------------------------------------------------------------------------------------------
/**
 * This function must be called to get the status of Echo Canceller.
 *
 * @return LE_BAD_PARAMETER The audio stream reference is invalid.
 * @return LE_UNAVAILABLE   The audio service initialization failed.
 * @return LE_FAULT         On any other failure.
 * @return LE_OK            Function succeeded.
 *
 * @note The process exits, if an invalid audio stream reference is given.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_audio_IsEchoCancellerEnabled
(
    le_audio_StreamRef_t streamRef,
        ///< [IN] Audio stream reference.
    bool* statusPtr
        ///< [OUT] true if EC is enabled, false otherwise
);

//--------------------------------------------------------------------------------------------------
/**
 * This function must be called to enable the FIR (Finite Impulse Response) filter.
 *
 * @return LE_BAD_PARAMETER The audio stream reference is invalid.
 * @return LE_UNAVAILABLE   The audio service initialization failed.
 * @return LE_FAULT         On any other failure.
 * @return LE_OK            Function succeeded.
 *
 * @note The process exits, if an invalid audio stream reference is given.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_audio_EnableFirFilter
(
    le_audio_StreamRef_t streamRef
        ///< [IN] Audio stream reference.
);

//--------------------------------------------------------------------------------------------------
/**
 * This function must be called to disable the FIR (Finite Impulse Response) filter.
 *
 * @return LE_BAD_PARAMETER The audio stream reference is invalid.
 * @return LE_UNAVAILABLE   The audio service initialization failed.
 * @return LE_FAULT         On any other failure.
 * @return LE_OK            Function succeeded.
 *
 * @note The process exits, if an invalid audio stream reference is given.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_audio_DisableFirFilter
(
    le_audio_StreamRef_t streamRef
        ///< [IN] Audio stream reference.
);

//--------------------------------------------------------------------------------------------------
/**
 * This function must be called to enable the IIR (Infinite Impulse Response) filter.
 *
 * @return LE_BAD_PARAMETER The audio stream reference is invalid.
 * @return LE_UNAVAILABLE   The audio service initialization failed.
 * @return LE_FAULT         On any other failure.
 * @return LE_OK            Function succeeded.
 *
 * @note The process exits, if an invalid audio stream reference is given.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_audio_EnableIirFilter
(
    le_audio_StreamRef_t streamRef
        ///< [IN] Audio stream reference.
);

//--------------------------------------------------------------------------------------------------
/**
 * This function must be called to disable the IIR (Infinite Impulse Response) filter.
 *
 * @return LE_BAD_PARAMETER The audio stream reference is invalid.
 * @return LE_UNAVAILABLE   The audio service initialization failed.
 * @return LE_FAULT         On any other failure.
 * @return LE_OK            Function succeeded.
 *
 * @note The process exits, if an invalid audio stream reference is given.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_audio_DisableIirFilter
(
    le_audio_StreamRef_t streamRef
        ///< [IN] Audio stream reference.
);

//--------------------------------------------------------------------------------------------------
/**
 * This function must be called to enable the automatic gain control on the selected audio stream.
 *
 * @return LE_BAD_PARAMETER The audio stream reference is invalid.
 * @return LE_UNAVAILABLE   The audio service initialization failed.
 * @return LE_FAULT         On any other failure.
 * @return LE_OK            Function succeeded.
 *
 * @note The process exits, if an invalid audio stream reference is given.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_audio_EnableAutomaticGainControl
(
    le_audio_StreamRef_t streamRef
        ///< [IN] Audio stream reference.
);

//--------------------------------------------------------------------------------------------------
/**
 * This function must be called to disable the automatic gain control on the selected audio stream.
 *
 * @return LE_BAD_PARAMETER The audio stream reference is invalid.
 * @return LE_UNAVAILABLE   The audio service initialization failed.
 * @return LE_FAULT         On any other failure.
 * @return LE_OK            Function succeeded.
 *
 * @note The process exits, if an invalid audio stream reference is given.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_audio_DisableAutomaticGainControl
(
    le_audio_StreamRef_t streamRef
        ///< [IN] Audio stream reference.
);

//--------------------------------------------------------------------------------------------------
/**
 * This function must be called to set the audio profile.
 *
 * @return LE_UNAVAILABLE   On audio service initialization failure.
 * @return LE_FAULT         On any other failure.
 * @return LE_OK            Function succeeded.
 *
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_audio_SetProfile
(
    uint32_t profile
        ///< [IN] Audio profile.
);

//--------------------------------------------------------------------------------------------------
/**
 * This function must be called to get the audio profile in use.
 *
 * @return LE_UNAVAILABLE   On audio service initialization failure.
 * @return LE_FAULT         On any other failure.
 * @return LE_OK            Function succeeded.
 *
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_audio_GetProfile
(
    uint32_t* profilePtr
        ///< [OUT] Audio profile.
);

//--------------------------------------------------------------------------------------------------
/**
 * Configure the PCM Sampling Rate.
 *
 * @return LE_FAULT         Function failed.
 * @return LE_OUT_OF_RANGE  Your platform does not support the setting's value.
 * @return LE_BUSY          PCM interface is already active.
 * @return LE_OK            Function succeeded.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_audio_SetPcmSamplingRate
(
    uint32_t rate
        ///< [IN] Sampling rate in Hz.
);

//--------------------------------------------------------------------------------------------------
/**
 * Configure the PCM Sampling Resolution.
 *
 * @return LE_FAULT         Function failed.
 * @return LE_OUT_OF_RANGE  Your platform does not support the setting's value.
 * @return LE_BUSY          PCM interface is already active.
 * @return LE_OK            Function succeeded.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_audio_SetPcmSamplingResolution
(
    uint32_t bitsPerSample
        ///< [IN] Sampling resolution (bits/sample).
);

//--------------------------------------------------------------------------------------------------
/**
 * Configure the PCM Companding.
 *
 * @return LE_FAULT         Function failed.
 * @return LE_OUT_OF_RANGE  Your platform does not support the setting's value.
 * @return LE_BUSY          PCM interface is already active.
 * @return LE_OK            Function succeeded.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_audio_SetPcmCompanding
(
    le_audio_Companding_t companding
        ///< [IN] Companding.
);

//--------------------------------------------------------------------------------------------------
/**
 * Retrieve the PCM Sampling Rate.
 *
 * @return The sampling rate in Hz.
 */
//--------------------------------------------------------------------------------------------------
uint32_t le_audio_GetPcmSamplingRate
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * Retrieve the PCM Sampling Resolution.
 *
 * @return The sampling resolution (bits/sample).
 */
//--------------------------------------------------------------------------------------------------
uint32_t le_audio_GetPcmSamplingResolution
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * Retrieve the PCM Companding.
 *
 * @return The PCM companding.
 */
//--------------------------------------------------------------------------------------------------
le_audio_Companding_t le_audio_GetPcmCompanding
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * Get the default PCM time slot used on the current platform.
 *
 * @return the time slot number.
 */
//--------------------------------------------------------------------------------------------------
uint32_t le_audio_GetDefaultPcmTimeSlot
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * Get the default I2S channel mode used on the current platform.
 *
 * @return the I2S channel mode.
 */
//--------------------------------------------------------------------------------------------------
le_audio_I2SChannel_t le_audio_GetDefaultI2sMode
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * Play a file on a playback stream.
 *
 * @return LE_FAULT         Function failed.
 * @return LE_BAD_PARAMETER Audio stream reference is invalid.
 * @return LE_BUSY          Player interface is already active.
 * @return LE_OK            Function succeeded.
 *
 * @note
 *  - The fd is closed by the IPC API. To play again the same file, the fd parameter can be set
 *    to LE_AUDIO_NO_FD: in this case, the previous file descriptor is re-used.
 *    If the fd as to be kept on its side, the application should duplicate the fd (e.g., using
 *    dup() ) before calling the API.
 *    In that case, the old and new file descriptors refer to the same open file description and
 *    thus share file offset. So, once a playback has reached the end of file, the application must
 *    reset the file offset by using lseek on the duplicated descriptor to start the playback from
 *    the beginning.
 *
 * @note
 * -  Calling le_audio_PlayFile(<..>, LE_AUDIO_NO_FD) will rewind the audio file to the
 *    beginning when a playback is already in progress.
 *
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_audio_PlayFile
(
    le_audio_StreamRef_t streamRef,
        ///< [IN] Audio stream reference.
    int fd
        ///< [IN] File descriptor.
);

//--------------------------------------------------------------------------------------------------
/**
 * Initiate a playback sending samples over a pipe.
 *
 * @return LE_FAULT         Function failed.
 * @return LE_BUSY          Player interface is already active.
 * @return LE_OK            Function succeeded.
 *
 * @note The fd is closed by the IPC API. To use again the same pipe, the fd parameter can be set
 * to LE_AUDIO_NO_FD: in this case, the previous file descriptor is re-used.
 * If the fd as to be kept on its side, the application should duplicate the fd (e.g., using dup() )
 * before calling the API.
 *
 * @note Playback initiated with this function must be stopped by calling le_audio_Stop().
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_audio_PlaySamples
(
    le_audio_StreamRef_t streamRef,
        ///< [IN] Audio stream reference.
    int fd
        ///< [IN] File descriptor.
);

//--------------------------------------------------------------------------------------------------
/**
 * Record a file on a recorder stream.
 *
 * @return LE_FAULT         Function failed.
 * @return LE_BAD_PARAMETER The audio stream reference is invalid.
 * @return LE_BUSY          Recorder interface is already active.
 * @return LE_OK            Function succeeded.
 *
 * @note the fd is closed by the API. To record again the same file, the fd parameter can be set to
 * LE_AUDIO_NO_FD: in this case, the previous file descriptor is re-used.
 * If the fd as to be kept on its side, the application should duplicate the fd (e.g., using dup() )
 * before calling the API.
 *
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_audio_RecordFile
(
    le_audio_StreamRef_t streamRef,
        ///< [IN] Audio stream reference.
    int fd
        ///< [IN] File descriptor.
);

//--------------------------------------------------------------------------------------------------
/**
 * Get samples from a recorder stream.
 *
 * @return LE_FAULT         Function failed.
 * @return LE_BUSY          Recorder interface is already active.
 * @return LE_OK            Function succeeded.
 *
 * @note The fd is closed by the API. To use again the same pipe, the fd parameter can be set to
 * LE_AUDIO_NO_FD: in this case, the previous file descriptor is re-used.
 * If the fd as to be kept on its side, the application should duplicate the fd (e.g., using dup() )
 * before calling the API.
 *
 * @note When using this function recording must be stopped by calling le_audio_Stop().
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_audio_GetSamples
(
    le_audio_StreamRef_t streamRef,
        ///< [IN] Audio stream reference.
    int fd
        ///< [IN] File descriptor.
);

//--------------------------------------------------------------------------------------------------
/**
 * Stop the file playback/recording.
 *
 * @return LE_FAULT         Function failed.
 * @return LE_OK            Function succeeded.
 *
 * @note The used file descriptor is not deallocated, but is is rewound to the beginning.
 *
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_audio_Stop
(
    le_audio_StreamRef_t streamRef
        ///< [IN] Audio stream reference.
);

//--------------------------------------------------------------------------------------------------
/**
 * Pause the file playback/recording.
 *
 * @return LE_FAULT         Function failed.
 * @return LE_OK            Function succeeded.
 *
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_audio_Pause
(
    le_audio_StreamRef_t streamRef
        ///< [IN] Audio stream reference.
);

//--------------------------------------------------------------------------------------------------
/**
 * Flush the remaining audio samples.
 *
 * @return LE_FAULT         Function failed.
 * @return LE_OK            Function succeeded.
 *
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_audio_Flush
(
    le_audio_StreamRef_t streamRef
        ///< [IN] Audio stream reference.
);

//--------------------------------------------------------------------------------------------------
/**
 * Resume a file playback/recording (need to be in pause state).
 *
 * @return LE_FAULT         Function failed.
 * @return LE_OK            Function succeeded.
 *
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_audio_Resume
(
    le_audio_StreamRef_t streamRef
        ///< [IN] Audio stream reference.
);

//--------------------------------------------------------------------------------------------------
/**
 * Set the channel number of a PCM sample.
 *
 * @return LE_FAULT         Function failed.
 * @return LE_OK            Function succeeded.
 *
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_audio_SetSamplePcmChannelNumber
(
    le_audio_StreamRef_t streamRef,
        ///< [IN] Audio stream reference.
    uint32_t nbChannel
        ///< [IN] Channel Number
);

//--------------------------------------------------------------------------------------------------
/**
 * Get the channel number of a PCM sample.
 *
 * @return LE_FAULT         Function failed.
 * @return LE_OK            Function succeeded.
 *
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_audio_GetSamplePcmChannelNumber
(
    le_audio_StreamRef_t streamRef,
        ///< [IN] Audio stream reference.
    uint32_t* nbChannelPtr
        ///< [OUT] Channel Number
);

//--------------------------------------------------------------------------------------------------
/**
 * Set the PCM sampling rate of a PCM sample.
 *
 * @return LE_FAULT         Function failed.
 * @return LE_OK            Function succeeded.
 *
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_audio_SetSamplePcmSamplingRate
(
    le_audio_StreamRef_t streamRef,
        ///< [IN] Audio stream reference.
    uint32_t rate
        ///< [IN] PCM sampling Rate.
);

//--------------------------------------------------------------------------------------------------
/**
 * Get the PCM sampling rate of a PCM sample.
 *
 * @return LE_FAULT         Function failed.
 * @return LE_OK            Function succeeded.
 *
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_audio_GetSamplePcmSamplingRate
(
    le_audio_StreamRef_t streamRef,
        ///< [IN] Audio stream reference.
    uint32_t* ratePtr
        ///< [OUT] PCM sampling Rate.
);

//--------------------------------------------------------------------------------------------------
/**
 * Set the sampling resolution (in bits per sample) of a PCM sample.
 *
 * @return LE_FAULT         Function failed.
 * @return LE_OK            Function succeeded.
 *
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_audio_SetSamplePcmSamplingResolution
(
    le_audio_StreamRef_t streamRef,
        ///< [IN] Audio stream reference.
    uint32_t samplingRes
        ///< [IN] Sampling resolution (in bits per sample).
);

//--------------------------------------------------------------------------------------------------
/**
 * Get the sampling resolution (in bits per sample) of a PCM sample.
 *
 * @return LE_FAULT         Function failed.
 * @return LE_OK            Function succeeded.
 *
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_audio_GetSamplePcmSamplingResolution
(
    le_audio_StreamRef_t streamRef,
        ///< [IN] Audio stream reference.
    uint32_t* samplingResPtr
        ///< [OUT] Sampling resolution (in bits per sample).
);

//--------------------------------------------------------------------------------------------------
/**
 * This function must be called to play a DTMF on a specific audio stream.
 *
 * @return LE_FORMAT_ERROR  The DTMF characters are invalid.
 * @return LE_BUSY          A DTMF playback is already in progress on the playback stream.
 * @return LE_FAULT         Function failed to play the DTMFs.
 * @return LE_OK            Function succeeded.
 *
 * @note If the DTMF string is too long (max DTMF_MAX_LEN characters), it is a fatal
 *       error, the function will not return.
 * @note The process exits, if an invalid audio stream reference is given.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_audio_PlayDtmf
(
    le_audio_StreamRef_t streamRef,
        ///< [IN] Audio stream reference.
    const char* LE_NONNULL dtmf,
        ///< [IN] DTMFs to play.
    uint32_t duration,
        ///< [IN] DTMF duration in milliseconds.
    uint32_t pause
        ///< [IN] Pause duration between tones in milliseconds.
);

//--------------------------------------------------------------------------------------------------
/**
 * This function must be called to ask to the Mobile Network to generate on the remote audio party
 * the DTMFs.
 *
 * @return LE_FORMAT_ERROR  The DTMF characters are invalid.
 * @return LE_BUSY          A DTMF playback is in progress.
 * @return LE_FAULT         Function failed.
 * @return LE_OK            Function succeeded.
 *
 * @note If the DTMF string is too long (max DTMF_MAX_LEN characters), it is a fatal
 *       error, the function will not return.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_audio_PlaySignallingDtmf
(
    const char* LE_NONNULL dtmf,
        ///< [IN] DTMFs to play.
    uint32_t duration,
        ///< [IN] DTMF duration in milliseconds.
    uint32_t pause
        ///< [IN] Pause duration between tones in milliseconds.
);

//--------------------------------------------------------------------------------------------------
/**
 * Set the encoding format of a recorder stream.
 *
 * @return LE_FAULT         Function failed.
 * @return LE_OK            Function succeeded.
 *
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_audio_SetEncodingFormat
(
    le_audio_StreamRef_t streamRef,
        ///< [IN] Audio stream reference.
    le_audio_Format_t format
        ///< [IN] Encoding format.
);

//--------------------------------------------------------------------------------------------------
/**
 * Get the encoding format of a recorder stream.
 *
 * @return LE_FAULT         Function failed.
 * @return LE_OK            Function succeeded.
 *
 * @note A client calling this function with either an invalid
 * streamRef or null out pointer parameter will be killed and the
 * function will not return.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_audio_GetEncodingFormat
(
    le_audio_StreamRef_t streamRef,
        ///< [IN] Audio stream reference.
    le_audio_Format_t* formatPtr
        ///< [OUT] Encoding format.
);

//--------------------------------------------------------------------------------------------------
/**
 * Set the AMR mode for AMR encoder.
 *
 * @return LE_FAULT         Function failed.
 * @return LE_OK            Function succeeded.
 *
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_audio_SetSampleAmrMode
(
    le_audio_StreamRef_t streamRef,
        ///< [IN] Audio stream reference.
    le_audio_AmrMode_t mode
        ///< [IN] AMR mode.
);

//--------------------------------------------------------------------------------------------------
/**
 * Get the AMR mode for AMR encoder.
 *
 * @return LE_FAULT         Function failed.
 * @return LE_OK            Function succeeded.
 *
 * @note A client calling this function with a null out pointer
 * parameter will be killed and the function will not return.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_audio_GetSampleAmrMode
(
    le_audio_StreamRef_t streamRef,
        ///< [IN] Audio stream reference.
    le_audio_AmrMode_t* modePtr
        ///< [OUT] AMR mode.
);

//--------------------------------------------------------------------------------------------------
/**
 * Set the AMR discontinuous transmission (DTX). The DTX is activated by default.
 *
 * @return LE_FAULT         Function failed.
 * @return LE_OK            Function succeeded.
 *
 * @note If the caller is passing a bad reference into this function, it is a fatal error, the
 *       function will not return.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_audio_SetSampleAmrDtx
(
    le_audio_StreamRef_t streamRef,
        ///< [IN] Audio stream reference.
    bool dtx
        ///< [IN] DTX.
);

//--------------------------------------------------------------------------------------------------
/**
 * Get the AMR discontinuous transmission (DTX) value.
 *
 * @return LE_FAULT         Function failed.
 * @return LE_OK            Function succeeded.
 *
 * @note If the caller is passing a bad reference into this function, it is a fatal error, the
 *       function will not return.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_audio_GetSampleAmrDtx
(
    le_audio_StreamRef_t streamRef,
        ///< [IN] Audio stream reference.
    bool* dtxPtr
        ///< [OUT] DTX.
);

//--------------------------------------------------------------------------------------------------
/**
 * Set the value of a platform specific gain in the audio subsystem.
 *
 * @return LE_BAD_PARAMETER The pointer to the name of the platform specific gain is invalid.
 * @return LE_NOT_FOUND     The specified gain's name is not recognized in your audio subsystem.
 * @return LE_OUT_OF_RANGE  The gain parameter is out of range.
 * @return LE_UNAVAILABLE   The audio service initialization failed.
 * @return LE_FAULT         On any other failure.
 * @return LE_OK            The function succeeded.
 *
 * @note If the caller is passing a bad reference into this function, it is a fatal error, the
 *       function will not return.
 * @warning Ensure to check the names of supported gains for your specific platform.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_audio_SetPlatformSpecificGain
(
    const char* LE_NONNULL gainName,
        ///< [IN] Name of the platform specific gain.
    int32_t gain
        ///< [IN] The gain value (specific to the platform)
);

//--------------------------------------------------------------------------------------------------
/**
 * Get the value of a platform specific gain in the audio subsystem.
 *
 * @return LE_BAD_PARAMETER The pointer to the name of the platform specific gain is invalid.
 * @return LE_NOT_FOUND     The specified gain's name is not recognized in your audio subsystem.
 * @return LE_UNAVAILABLE   The audio service initialization failed.
 * @return LE_FAULT         On any other failure.
 * @return LE_OK            The function succeeded.
 *
 * @note If the caller is passing a bad reference into this function, it is a fatal error, the
 *       function will not return.
 * @warning Ensure to check the names of supported gains for your specific platform.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_audio_GetPlatformSpecificGain
(
    const char* LE_NONNULL gainName,
        ///< [IN] Name of the platform specific gain.
    int32_t* gainPtr
        ///< [OUT] The gain value (specific to the platform)
);

//--------------------------------------------------------------------------------------------------
/**
 * Mute the Call Waiting Tone.
 *
 * @return LE_UNAVAILABLE   On audio service initialization failure.
 * @return LE_FAULT         On any other failure.
 * @return LE_OK            The function succeeded.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_audio_MuteCallWaitingTone
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * Unmute the Call Waiting Tone.
 *
 * @return LE_UNAVAILABLE   On audio service initialization failure.
 * @return LE_FAULT         On any other failure.
 * @return LE_OK            The function succeeded.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_audio_UnmuteCallWaitingTone
(
    void
);

#endif // LE_AUDIO_INTERFACE_H_INCLUDE_GUARD