

/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */


#ifndef LE_AUDIO_INTERFACE_H_INCLUDE_GUARD
#define LE_AUDIO_INTERFACE_H_INCLUDE_GUARD


#include "legato.h"


//--------------------------------------------------------------------------------------------------
/**
 * Get the server service reference
 */
//--------------------------------------------------------------------------------------------------
le_msg_ServiceRef_t le_audio_GetServiceRef
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * Get the client session reference for the current message
 */
//--------------------------------------------------------------------------------------------------
le_msg_SessionRef_t le_audio_GetClientSessionRef
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * Initialize the server and advertise the service.
 */
//--------------------------------------------------------------------------------------------------
void le_audio_AdvertiseService
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