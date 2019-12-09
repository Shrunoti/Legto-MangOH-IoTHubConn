

/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */


#ifndef LE_TEMP_INTERFACE_H_INCLUDE_GUARD
#define LE_TEMP_INTERFACE_H_INCLUDE_GUARD


#include "legato.h"


//--------------------------------------------------------------------------------------------------
/**
 * Get the server service reference
 */
//--------------------------------------------------------------------------------------------------
le_msg_ServiceRef_t le_temp_GetServiceRef
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * Get the client session reference for the current message
 */
//--------------------------------------------------------------------------------------------------
le_msg_SessionRef_t le_temp_GetClientSessionRef
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * Initialize the server and advertise the service.
 */
//--------------------------------------------------------------------------------------------------
void le_temp_AdvertiseService
(
    void
);


//--------------------------------------------------------------------------------------------------
/**
 * Maximum sensor's name string length.
 */
//--------------------------------------------------------------------------------------------------
#define LE_TEMP_SENSOR_NAME_MAX_LEN 100

//--------------------------------------------------------------------------------------------------
/**
 * Maximum sensor's name string length.
 * One extra byte is added for the null character.
 */
//--------------------------------------------------------------------------------------------------
#define LE_TEMP_SENSOR_NAME_MAX_BYTES 101

//--------------------------------------------------------------------------------------------------
/**
 * Maximum threshold's name string length.
 */
//--------------------------------------------------------------------------------------------------
#define LE_TEMP_THRESHOLD_NAME_MAX_LEN 100

//--------------------------------------------------------------------------------------------------
/**
 * Maximum threshold's name string length.
 * One extra byte is added for the null character.
 */
//--------------------------------------------------------------------------------------------------
#define LE_TEMP_THRESHOLD_NAME_MAX_BYTES 101

//--------------------------------------------------------------------------------------------------
/**
 *  Reference type for a temperature sensor.
 */
//--------------------------------------------------------------------------------------------------
typedef struct le_temp_Sensor* le_temp_SensorRef_t;


//--------------------------------------------------------------------------------------------------
/**
 * Reference type used by Add/Remove functions for EVENT 'le_temp_ThresholdEvent'
 */
//--------------------------------------------------------------------------------------------------
typedef struct le_temp_ThresholdEventHandler* le_temp_ThresholdEventHandlerRef_t;


//--------------------------------------------------------------------------------------------------
/**
 * Handler for Temperature event.
 */
//--------------------------------------------------------------------------------------------------
typedef void (*le_temp_ThresholdEventHandlerFunc_t)
(
    le_temp_SensorRef_t sensor,
        ///< Temperature sensor reference.
    const char* LE_NONNULL threshold,
        ///< Name of the raised threshold.
    void* contextPtr
        ///<
);


//--------------------------------------------------------------------------------------------------
/**
 * Request a temperature sensor reference.
 *
 * @return
 *      - Reference to the temperature sensor.
 *      - NULL when the requested sensor is not supported.
 */
//--------------------------------------------------------------------------------------------------
le_temp_SensorRef_t le_temp_Request
(
    const char* LE_NONNULL sensorName
        ///< [IN] Name of the temperature sensor.
);



//--------------------------------------------------------------------------------------------------
/**
 * Retrieve the temperature sensor's name from its reference.
 *
 * @return
 *      - LE_OK            The function succeeded.
 *      - LE_OVERFLOW      The name length exceed the maximum length.
 *      - LE_FAULT         The function failed.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_temp_GetSensorName
(
    le_temp_SensorRef_t sensor,
        ///< [IN] Temperature sensor reference.
    char* sensorName,
        ///< [OUT] Name of the temperature sensor.
    size_t sensorNameSize
        ///< [IN]
);



//--------------------------------------------------------------------------------------------------
/**
 * Add handler function for EVENT 'le_temp_ThresholdEvent'
 *
 * This event provides information on the threshold reached.
 *
 */
//--------------------------------------------------------------------------------------------------
le_temp_ThresholdEventHandlerRef_t le_temp_AddThresholdEventHandler
(
    le_temp_ThresholdEventHandlerFunc_t ThresholdEventHandlerPtr,
        ///< [IN]
    void* contextPtr
        ///< [IN]
);



//--------------------------------------------------------------------------------------------------
/**
 * Remove handler function for EVENT 'le_temp_ThresholdEvent'
 */
//--------------------------------------------------------------------------------------------------
void le_temp_RemoveThresholdEventHandler
(
    le_temp_ThresholdEventHandlerRef_t handlerRef
        ///< [IN]
);



//--------------------------------------------------------------------------------------------------
/**
 * Get the temperature in degree Celsius.
 *
 * @return
 *      - LE_OK            The function succeeded.
 *      - LE_FAULT         The function failed to get the temperature.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_temp_GetTemperature
(
    le_temp_SensorRef_t sensor,
        ///< [IN] Temperature sensor reference.
    int32_t* temperaturePtr
        ///< [OUT] Temperature in degree Celsius.
);



//--------------------------------------------------------------------------------------------------
/**
 * Set the temperature threshold in degree Celsius. This function does not start the temperature
 * monitoring, call le_temp_StartMonitoring() to start it.
 *
 * @return
 *      - LE_OK            The function succeeded.
 *      - LE_FAULT         The function failed.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_temp_SetThreshold
(
    le_temp_SensorRef_t sensor,
        ///< [IN] Temperature sensor reference.
    const char* LE_NONNULL threshold,
        ///< [IN] Name of the threshold.
    int32_t temperature
        ///< [IN] Temperature threshold in degree Celsius.
);



//--------------------------------------------------------------------------------------------------
/**
 * Get the temperature threshold in degree Celsius.
 *
 * @return
 *      - LE_OK            The function succeeded.
 *      - LE_FAULT         The function failed.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_temp_GetThreshold
(
    le_temp_SensorRef_t sensor,
        ///< [IN] Temperature sensor reference.
    const char* LE_NONNULL threshold,
        ///< [IN] Name of the threshold.
    int32_t* temperaturePtr
        ///< [OUT] Temperature threshold in degree Celsius.
);



//--------------------------------------------------------------------------------------------------
/**
 * Start the temperature monitoring with the temperature thresholds configured by
 * le_temp_SetThreshold() function.
 *
 * @return
 *      - LE_OK            The function succeeded.
 *      - LE_BAD_PARAMETER There is a mismatch with the configured threshold values.
 *      - LE_FAULT         The function failed to apply the thresholds.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_temp_StartMonitoring
(
    void
);


#endif // LE_TEMP_INTERFACE_H_INCLUDE_GUARD