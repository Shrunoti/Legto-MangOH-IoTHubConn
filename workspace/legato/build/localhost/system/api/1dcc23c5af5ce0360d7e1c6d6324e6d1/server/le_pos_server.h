

/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */


#ifndef LE_POS_INTERFACE_H_INCLUDE_GUARD
#define LE_POS_INTERFACE_H_INCLUDE_GUARD


#include "legato.h"


//--------------------------------------------------------------------------------------------------
/**
 * Get the server service reference
 */
//--------------------------------------------------------------------------------------------------
le_msg_ServiceRef_t le_pos_GetServiceRef
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * Get the client session reference for the current message
 */
//--------------------------------------------------------------------------------------------------
le_msg_SessionRef_t le_pos_GetClientSessionRef
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * Initialize the server and advertise the service.
 */
//--------------------------------------------------------------------------------------------------
void le_pos_AdvertiseService
(
    void
);


//--------------------------------------------------------------------------------------------------
/**
 *  Fix position states.
 */
//--------------------------------------------------------------------------------------------------
typedef enum
{
    LE_POS_STATE_NO_FIX = 0,
        ///< The position is not fixed.
    LE_POS_STATE_FIX_2D = 1,
        ///< 2-Dimensional position fix.
    LE_POS_STATE_FIX_3D = 2,
        ///< 3-Dimensional position fix.
    LE_POS_STATE_FIX_ESTIMATED = 3,
        ///< Estimated (i.e. forward predicted) position fix.
    LE_POS_STATE_UNKNOWN = 4
        ///< Unknown state.
}
le_pos_FixState_t;


//--------------------------------------------------------------------------------------------------
/**
 *  Resolution values.
 */
//--------------------------------------------------------------------------------------------------
typedef enum
{
    LE_POS_RES_METER = 0,
        ///< Resolution in meter.
    LE_POS_RES_DECIMETER = 1,
        ///< Resolution in decimeter.
    LE_POS_RES_CENTIMETER = 2,
        ///< Resolution in centimeter.
    LE_POS_RES_MILLIMETER = 3,
        ///< Resolution in millimeter.
    LE_POS_RES_UNKNOWN = 4
        ///< Unknown resolution.
}
le_pos_Resolution_t;


//--------------------------------------------------------------------------------------------------
/**
 *  Reference type for dealing with Position samples.
 */
//--------------------------------------------------------------------------------------------------
typedef struct le_pos_Sample* le_pos_SampleRef_t;


//--------------------------------------------------------------------------------------------------
/**
 * Reference type used by Add/Remove functions for EVENT 'le_pos_Movement'
 */
//--------------------------------------------------------------------------------------------------
typedef struct le_pos_MovementHandler* le_pos_MovementHandlerRef_t;


//--------------------------------------------------------------------------------------------------
/**
 * Handler for Movement changes.
 *
 */
//--------------------------------------------------------------------------------------------------
typedef void (*le_pos_MovementHandlerFunc_t)
(
    le_pos_SampleRef_t positionSampleRef,
        ///< Position's sample reference
    void* contextPtr
        ///<
);


//--------------------------------------------------------------------------------------------------
/**
 * Add handler function for EVENT 'le_pos_Movement'
 *
 * This event provides information on movement changes.
 *
 */
//--------------------------------------------------------------------------------------------------
le_pos_MovementHandlerRef_t le_pos_AddMovementHandler
(
    uint32_t horizontalMagnitude,
        ///< [IN] Horizontal magnitude in meters.
        ///<       0 means that I don't care about
        ///<      changes in the latitude and longitude.
    uint32_t verticalMagnitude,
        ///< [IN] Vertical magnitude in meters.
        ///<       0 means that I don't care about
        ///<       changes in the altitude.
    le_pos_MovementHandlerFunc_t handlerPtr,
        ///< [IN]
    void* contextPtr
        ///< [IN]
);



//--------------------------------------------------------------------------------------------------
/**
 * Remove handler function for EVENT 'le_pos_Movement'
 */
//--------------------------------------------------------------------------------------------------
void le_pos_RemoveMovementHandler
(
    le_pos_MovementHandlerRef_t handlerRef
        ///< [IN]
);



//--------------------------------------------------------------------------------------------------
/**
 * Get the 2D location's data (Latitude, Longitude, Horizontal
 * accuracy).
 *
 * @return LE_FAULT         Function failed to get the 2D location's data
 * @return LE_OUT_OF_RANGE  One of the retrieved parameter is invalid (set to INT32_MAX).
 * @return LE_OK            Function succeeded.
 *
 * @note latitudePtr, longitudePtr, hAccuracyPtr can be set to NULL if not needed.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_pos_Get2DLocation
(
    int32_t* latitudePtr,
        ///< [OUT] WGS84 Latitude in degrees, positive North [resolution 1e-6].
    int32_t* longitudePtr,
        ///< [OUT] WGS84 Longitude in degrees, positive East [resolution 1e-6].
    int32_t* hAccuracyPtr
        ///< [OUT] Horizontal position's accuracy in meters by default.
);



//--------------------------------------------------------------------------------------------------
/**
 * Get the 3D location's data (Latitude, Longitude, Altitude,
 * Horizontal accuracy, Vertical accuracy).
 *
 * @return LE_FAULT         Function failed to get the 3D location's data
 * @return LE_OUT_OF_RANGE  One of the retrieved parameter is invalid (set to INT32_MAX).
 * @return LE_OK            Function succeeded.
 *
 * @note latitudePtr, longitudePtr,hAccuracyPtr, altitudePtr, vAccuracyPtr can be set to NULL
 *       if not needed.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_pos_Get3DLocation
(
    int32_t* latitudePtr,
        ///< [OUT] WGS84 Latitude in degrees, positive North [resolution 1e-6].
    int32_t* longitudePtr,
        ///< [OUT] WGS84 Longitude in degrees, positive East [resolution 1e-6].
    int32_t* hAccuracyPtr,
        ///< [OUT] Horizontal position's accuracy in meters by default.
    int32_t* altitudePtr,
        ///< [OUT] Altitude above Mean Sea Level in meters by default.
    int32_t* vAccuracyPtr
        ///< [OUT] Vertical position's accuracy in meters by default.
);



//--------------------------------------------------------------------------------------------------
/**
 * Get the time of the last updated location
 *
 * @return LE_FAULT         Function failed to get the time.
 * @return LE_OUT_OF_RANGE  The retrieved time is invalid (all fields are set to 0).
 * @return LE_OK            Function succeeded.
 *
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_pos_GetTime
(
    uint16_t* hoursPtr,
        ///< [OUT] UTC Hours into the day [range 0..23].
    uint16_t* minutesPtr,
        ///< [OUT] UTC Minutes into the hour [range 0..59].
    uint16_t* secondsPtr,
        ///< [OUT] UTC Seconds into the minute [range 0..59].
    uint16_t* millisecondsPtr
        ///< [OUT] UTC Milliseconds into the second [range 0..999].
);



//--------------------------------------------------------------------------------------------------
/**
 * Get the date of the last updated location
 *
 * @return LE_FAULT         Function failed to get the date.
 * @return LE_OUT_OF_RANGE  The retrieved date is invalid (all fields are set to 0).
 * @return LE_OK            Function succeeded.
 *
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_pos_GetDate
(
    uint16_t* yearPtr,
        ///< [OUT] UTC Year A.D. [e.g. 2014].
    uint16_t* monthPtr,
        ///< [OUT] UTC Month into the year [range 1...12].
    uint16_t* dayPtr
        ///< [OUT] UTC Days into the month [range 1...31].
);



//--------------------------------------------------------------------------------------------------
/**
 * Get the motion's data (Horizontal Speed, Horizontal Speed's
 * accuracy, Vertical Speed, Vertical Speed's accuracy).
 *
 * @return LE_FAULT         Function failed to get the motion's data.
 * @return LE_OUT_OF_RANGE  One of the retrieved parameter is invalid (set to INT32_MAX,
 *                          UINT32_MAX).
 * @return LE_OK            Function succeeded.
 *
 * @note hSpeedPtr, hSpeedAccuracyPtr, vSpeedPtr, vSpeedAccuracyPtr can be set to NULL if not
 *       needed.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_pos_GetMotion
(
    uint32_t* hSpeedPtr,
        ///< [OUT] Horizontal Speed in m/sec.
    uint32_t* hSpeedAccuracyPtr,
        ///< [OUT] Horizontal Speed's accuracy in m/sec.
    int32_t* vSpeedPtr,
        ///< [OUT] Vertical Speed in m/sec, positive up.
    int32_t* vSpeedAccuracyPtr
        ///< [OUT] Vertical Speed's accuracy in m/sec.
);



//--------------------------------------------------------------------------------------------------
/**
 * Get the heading indication.
 *
 * @return LE_FAULT         Function failed to get the heading indication.
 * @return LE_OUT_OF_RANGE  One of the retrieved parameter is invalid (set to UINT32_MAX).
 * @return LE_OK            Function succeeded.
 *
 * @note Heading is given in degrees.
 *       Heading ranges from 0 to 359 degrees, where 0 is True North.
 *
 * @note headingPtr, headingAccuracyPtr can be set to NULL if not needed.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_pos_GetHeading
(
    uint32_t* headingPtr,
        ///< [OUT] Heading in degrees.
        ///< Range: 0 to 359, where 0 is True North.
    uint32_t* headingAccuracyPtr
        ///< [OUT] Heading's accuracy in degrees.
);



//--------------------------------------------------------------------------------------------------
/**
 * Get the direction indication. Direction of movement is the direction that the vehicle or person
 * is actually moving.
 *
 * @return LE_FAULT         Function failed to get the direction indication.
 * @return LE_OUT_OF_RANGE  One of the retrieved parameter is invalid (set to UINT32_MAX).
 * @return LE_OK            Function succeeded.
 *
 * @note Direction is given in degrees.
 *       Direction ranges from 0 to 359 degrees, where 0 is True North.
 *
 * @note directionPtr, directionAccuracyPtr can be set to NULL if not needed.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_pos_GetDirection
(
    uint32_t* directionPtr,
        ///< [OUT] Direction indication in degrees.
        ///< Range: 0 to 359, where 0 is True North.
    uint32_t* directionAccuracyPtr
        ///< [OUT] Direction's accuracy estimate in degrees.
);



//--------------------------------------------------------------------------------------------------
/**
 * Get the position fix state
 *
 * @return LE_FAULT         Function failed to get the position fix state.
 * @return LE_OK            Function succeeded.
 *
 * @note In case the function fails to get the position fix state, a fatal error occurs,
 *       the function will not return.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_pos_GetFixState
(
    le_pos_FixState_t* statePtr
        ///< [OUT] Position fix state.
);



//--------------------------------------------------------------------------------------------------
/**
 * Get the position sample's 2D location (latitude, longitude,
 * horizontal accuracy).
 *
 * @return LE_FAULT         Function failed to find the positionSample.
 * @return LE_OUT_OF_RANGE  One of the retrieved parameter is invalid (set to INT32_MAX).
 * @return LE_OK            Function succeeded.
 *
 * @note If the caller is passing an invalid Position reference into this function,
 *       it is a fatal error, the function will not return.
 *
 * @note latitudePtr, longitudePtr, horizontalAccuracyPtr can be set to NULL if not needed.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_pos_sample_Get2DLocation
(
    le_pos_SampleRef_t positionSampleRef,
        ///< [IN] Position sample's reference.
    int32_t* latitudePtr,
        ///< [OUT] WGS84 Latitude in degrees, positive North [resolution 1e-6].
    int32_t* longitudePtr,
        ///< [OUT] WGS84 Longitude in degrees, positive East [resolution 1e-6].
    int32_t* horizontalAccuracyPtr
        ///< [OUT] Horizontal position's accuracy in meters by default.
);



//--------------------------------------------------------------------------------------------------
/**
 * Get the position sample's time.
 *
 * @return LE_FAULT         Function failed to get the time.
 * @return LE_OUT_OF_RANGE  The retrieved time is invalid (all fields are set to 0).
 * @return LE_OK            Function succeeded.
 *
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_pos_sample_GetTime
(
    le_pos_SampleRef_t positionSampleRef,
        ///< [IN] Position sample's reference.
    uint16_t* hoursPtr,
        ///< [OUT] UTC Hours into the day [range 0..23].
    uint16_t* minutesPtr,
        ///< [OUT] UTC Minutes into the hour [range 0..59].
    uint16_t* secondsPtr,
        ///< [OUT] UTC Seconds into the minute [range 0..59].
    uint16_t* millisecondsPtr
        ///< [OUT] UTC Milliseconds into the second [range 0..999].
);



//--------------------------------------------------------------------------------------------------
/**
 * Get the position sample's date.
 *
 * @return LE_FAULT         Function failed to get the date.
 * @return LE_OUT_OF_RANGE  The retrieved date is invalid (all fields are set to 0).
 * @return LE_OK            Function succeeded.
 *
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_pos_sample_GetDate
(
    le_pos_SampleRef_t positionSampleRef,
        ///< [IN] Position sample's reference.
    uint16_t* yearPtr,
        ///< [OUT] UTC Year A.D. [e.g. 2014].
    uint16_t* monthPtr,
        ///< [OUT] UTC Month into the year [range 1...12].
    uint16_t* dayPtr
        ///< [OUT] UTC Days into the month [range 1...31].
);



//--------------------------------------------------------------------------------------------------
/**
 * Get the position sample's altitude.
 *
 * @return LE_FAULT         Function failed to find the positionSample.
 * @return LE_OUT_OF_RANGE  One of the retrieved parameter is invalid (set to INT32_MAX).
 * @return LE_OK            Function succeeded.
 *
 * @note If the caller is passing an invalid Position reference into this function,
 *       it is a fatal error, the function will not return.
 *
 * @note altitudePtr, altitudeAccuracyPtr can be set to NULL if not needed.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_pos_sample_GetAltitude
(
    le_pos_SampleRef_t positionSampleRef,
        ///< [IN] Position sample's reference.
    int32_t* altitudePtr,
        ///< [OUT] Altitude above Mean Sea Level in meters by default.
    int32_t* altitudeAccuracyPtr
        ///< [OUT] Vertical position's accuracy in meters by default.
);



//--------------------------------------------------------------------------------------------------
/**
 * Get the position sample's horizontal speed.
 *
 * @return LE_FAULT         Function failed to find the positionSample.
 * @return LE_OUT_OF_RANGE  One of the retrieved parameter is invalid (set to INT32_MAX,
 *                          UINT32_MAX).
 * @return LE_OK            Function succeeded.
 *
 * @note If the caller is passing an invalid Position reference into this function,
 *       it is a fatal error, the function will not return.
 *
 * @note hSpeedPtr, hSpeedAccuracyPtr can be set to NULL if not needed.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_pos_sample_GetHorizontalSpeed
(
    le_pos_SampleRef_t positionSampleRef,
        ///< [IN] Position sample's reference.
    uint32_t* hSpeedPtr,
        ///< [OUT] The Horizontal Speed in m/sec.
    uint32_t* hSpeedAccuracyPtr
        ///< [OUT] The Horizontal Speed's accuracy in m/sec.
);



//--------------------------------------------------------------------------------------------------
/**
 * Get the position sample's vertical speed.
 *
 * @return LE_FAULT         The function failed to find the positionSample.
 * @return LE_OUT_OF_RANGE  One of the retrieved parameter is not valid (set to INT32_MAX).
 * @return LE_OK            The function succeeded.
 *
 * @note If the caller is passing an invalid Position reference into this function,
 *       it is a fatal error, the function will not return.
 *
 * @note vSpeedPtr, vSpeedAccuracyPtr can be set to NULL if not needed.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_pos_sample_GetVerticalSpeed
(
    le_pos_SampleRef_t positionSampleRef,
        ///< [IN] Position sample's reference.
    int32_t* vspeedPtr,
        ///< [OUT] The Vertical Speed in m/sec, positive up.
    int32_t* vspeedAccuracyPtr
        ///< [OUT] The Vertical Speed's accuracy in m/sec.
);



//--------------------------------------------------------------------------------------------------
/**
 * Get the position sample's heading. Heading is the direction that the vehicle or person is facing.
 *
 * @return LE_FAULT         Function failed to find the positionSample.
 * @return LE_OUT_OF_RANGE  One of the retrieved parameter is invalid (set to UINT32_MAX).
 * @return LE_OK            TFunction succeeded.
 *
 * @note Heading is given in degrees.
 *       Heading ranges from 0 to 359 degrees, where 0 is True North.
 *
 * @note If the caller is passing an invalid Position reference into this function,
 *       it is a fatal error, the function will not return.
 *
 * @note headingPtr, headingAccuracyPtr can be set to NULL if not needed.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_pos_sample_GetHeading
(
    le_pos_SampleRef_t positionSampleRef,
        ///< [IN] Position sample's reference.
    uint32_t* headingPtr,
        ///< [OUT] Heading in degrees.
        ///< Range: 0 to 359, where 0 is True North.
    uint32_t* headingAccuracyPtr
        ///< [OUT] Heading's accuracy estimate in degrees.
);



//--------------------------------------------------------------------------------------------------
/**
 * Get the position sample's direction. Direction of movement is the direction that the vehicle or
 * person is actually moving.
 *
 * @return LE_FAULT         Function failed to find the positionSample.
 * @return LE_OUT_OF_RANGE  One of the retrieved parameter is invalid (set to UINT32_MAX).
 * @return LE_OK            Function succeeded.
 *
 * @note Direction is given in degrees.
 *       Direction ranges from 0 to 359 degrees, where 0 is True North.
 *
 * @note If the caller is passing an invalid Position reference into this function,
 *       it is a fatal error, the function will not return.
 *
 * @note directionPtr, directionAccuracyPtr can be set to NULL if not needed.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_pos_sample_GetDirection
(
    le_pos_SampleRef_t positionSampleRef,
        ///< [IN] Position sample's reference.
    uint32_t* directionPtr,
        ///< [OUT] Direction indication in degrees.
        ///< Range: 0 to 359, where 0 is True North.
    uint32_t* directionAccuracyPtr
        ///< [OUT] Direction's accuracy estimate in degrees.
);



//--------------------------------------------------------------------------------------------------
/**
 * Get the position sample's fix state.
 *
 * @return LE_FAULT         Function failed to get the position sample's fix state.
 * @return LE_OK            Function succeeded.
 *
 * @note If the caller is passing an invalid Position reference into this function,
 *       it is a fatal error, the function will not return.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_pos_sample_GetFixState
(
    le_pos_SampleRef_t positionSampleRef,
        ///< [IN] Position sample's reference.
    le_pos_FixState_t* statePtr
        ///< [OUT] Position fix state.
);



//--------------------------------------------------------------------------------------------------
/**
 * Release the position sample.
 *
 * @note If the caller is passing an invalid Position reference into this function,
 *       it is a fatal error, the function will not return.
 */
//--------------------------------------------------------------------------------------------------
void le_pos_sample_Release
(
    le_pos_SampleRef_t positionSampleRef
        ///< [IN] Position sample's reference.
);



//--------------------------------------------------------------------------------------------------
/**
 * Set the acquisition rate.
 *
 * @return
 *    LE_OUT_OF_RANGE    Invalid acquisition rate.
 *    LE_OK              The function succeeded.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_pos_SetAcquisitionRate
(
    uint32_t acquisitionRate
        ///< [IN] Acquisition rate in milliseconds.
);



//--------------------------------------------------------------------------------------------------
/**
 * Retrieve the acquisition rate.
 *
 * @return
 *    Acquisition rate in milliseconds.
 */
//--------------------------------------------------------------------------------------------------
uint32_t le_pos_GetAcquisitionRate
(
    void
);



//--------------------------------------------------------------------------------------------------
/**
 * Set the resolution for the positioning distance values.
 *
 * @return LE_OK               Function succeeded.
 * @return LE_BAD_PARAMETER    Invalid parameter provided.
 *
 * @note The positioning distance values are: the altitude above sea level, the horizontal
 *       position accuracy and the vertical position accuracy. The API sets the same resolution to
 *       all distance values. The resolution change request takes effect immediately.
 *
 * @warning The positioning distance values resolutions are platform dependent. Please refer to
 *          @ref platformConstraintsPositioning_SettingResolution section for full details.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_pos_SetDistanceResolution
(
    le_pos_Resolution_t resolution
        ///< [IN] Resolution.
);


#endif // LE_POS_INTERFACE_H_INCLUDE_GUARD