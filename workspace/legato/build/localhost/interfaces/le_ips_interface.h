

/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */

/**
 * @page c_ips Input Power Supply Monitoring API
 *
 * @ref le_ips_interface.h "API Reference"
 *
 * <HR>
 *
 * The IPS API is used to get monitoring information related to the platform power supply and set
 * warning and critical thresholds.
 *
 * @section le_ips_binding IPC interfaces binding
 *
 * All the functions of this API are provided by the @b modemService.
 *
 * Here's a code sample binding to modem services:
 * @verbatim
   bindings:
   {
      clientExe.clientComponent.le_ips -> modemService.le_ips
   }
   @endverbatim
 *
 * @section le_ips_powerMonitoring Input power supply monitoring
 *
 * This functionality allows to retrieve data related to the platform power supply, so that
 * the application can use the information to diagnose why the modem isn't fully functioning.
 *
 * The application can retrieve the following power supply information:
 * - le_ips_GetInputVoltage() API gives the Platform voltage input.
 * - le_ips_GetPowerSource() API gives the power source used by the Platform:
 *     - @ref LE_IPS_POWER_SOURCE_EXTERNAL for an external power source
 *     - @ref LE_IPS_POWER_SOURCE_BATTERY for a battery.
 * - le_ips_GetBatteryLevel() API gives the Platform battery level.
 *
 * In case the device is powered by an external battery monitored by an application, this
 * application can set the battery level with the le_ips_SetBatteryLevel() API. This value will
 * then be used when the battery level is requested through the le_ips_GetBatteryLevel() API.
 *
 * @note The value set by le_ips_SetBatteryLevel() will be reported by le_ips_GetBatteryLevel()
 * until Legato is restarted.
 *
 * @section le_ips_voltageThresholds Platform input voltage thresholds
 *
 * @warning When a critical event occurs, some platform may automatically switch off.
 *
 * @warning On some platforms, the thresholds parameters are persistent and a platform reboot is
 * required for thresholds change takes effect.
 *
 * Four thresholds are set to decide the state: the @e critical, @e warning, @e normal and
 * <em>high critical</em> platform input voltage thresholds.
 *
 * if the platform input voltage decreases below the:
 *  - "High critical threshold - 1 " but still higher than "Warning threshold",
 * @c LE_IPS_VOLTAGE_NORMAL event occurs.
 *  - "Warning threshold" but still higher than "Critical threshold",
 * @c LE_IPS_VOLTAGE_WARNING event occurs.
 *  - "Critical threshold", a @c LE_IPS_VOLTAGE_CRITICAL event occurs.
 *
 * if the platform input voltage goes up and it reaches the:
 *  - "Normal threshold", a @c LE_IPS_VOLTAGE_NORMAL event occurs.
 *  - "High critical thresholds", a @c LE_IPS_HI_VOLTAGE_CRITICAL event occurs.
 *
 * @note The threshold values range is platform dependent.
 *
 * - le_ips_SetVoltageThresholds() API allows the application to set platform input voltage
 *  thresholds.
 * - le_ips_GetVoltageThresholds() API allows the application to get platform input voltage
 * thresholds.
 * - le_ips_AddThresholdEventHandler() API adds a handler to notify when the platform input voltage
 *  threshold is reached.
 * - le_ips_RemoveThresholdEventHandler() API removes the platform input voltage handler.
 *
 * <HR>
 *
 * Copyright (C) Sierra Wireless Inc.
 */
/**
 * @file le_ips_interface.h
 *
 * Legato @ref c_ips include file.
 *
 * Copyright (C) Sierra Wireless Inc.
 */

#ifndef LE_IPS_INTERFACE_H_INCLUDE_GUARD
#define LE_IPS_INTERFACE_H_INCLUDE_GUARD


#include "legato.h"


//--------------------------------------------------------------------------------------------------
/**
 * Type for handler called when a server disconnects.
 */
//--------------------------------------------------------------------------------------------------
typedef void (*le_ips_DisconnectHandler_t)(void *);

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
void le_ips_ConnectService
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
le_result_t le_ips_TryConnectService
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
void le_ips_SetServerDisconnectHandler
(
    le_ips_DisconnectHandler_t disconnectHandler,
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
void le_ips_DisconnectService
(
    void
);


//--------------------------------------------------------------------------------------------------
/**
 * Platform input voltage event type.
 */
//--------------------------------------------------------------------------------------------------
typedef enum
{
    LE_IPS_VOLTAGE_HI_CRITICAL = 0,
        ///< High Critical input voltage threshold is reached.
    LE_IPS_VOLTAGE_NORMAL = 1,
        ///< Normal input voltage threshold is reached.
    LE_IPS_VOLTAGE_WARNING = 2,
        ///< Warning input voltage threshold is reached.
    LE_IPS_VOLTAGE_CRITICAL = 3
        ///< Critical input voltage threshold is reached.
}
le_ips_ThresholdStatus_t;


//--------------------------------------------------------------------------------------------------
/**
 * Platform power source type.
 */
//--------------------------------------------------------------------------------------------------
typedef enum
{
    LE_IPS_POWER_SOURCE_EXTERNAL = 0,
        ///< Platform is powered by an external source.
    LE_IPS_POWER_SOURCE_BATTERY = 1
        ///< Platform is powered by a battery.
}
le_ips_PowerSource_t;


//--------------------------------------------------------------------------------------------------
/**
 * Reference type used by Add/Remove functions for EVENT 'le_ips_ThresholdEvent'
 */
//--------------------------------------------------------------------------------------------------
typedef struct le_ips_ThresholdEventHandler* le_ips_ThresholdEventHandlerRef_t;


//--------------------------------------------------------------------------------------------------
/**
 * Handler for Platform input voltage event.
 */
//--------------------------------------------------------------------------------------------------
typedef void (*le_ips_ThresholdEventHandlerFunc_t)
(
    le_ips_ThresholdStatus_t event,
        ///< Input voltage threshold event reached.
    void* contextPtr
        ///<
);

//--------------------------------------------------------------------------------------------------
/**
 * Add handler function for EVENT 'le_ips_ThresholdEvent'
 *
 * This event provides information on Threshold reached.
 *
 */
//--------------------------------------------------------------------------------------------------
le_ips_ThresholdEventHandlerRef_t le_ips_AddThresholdEventHandler
(
    le_ips_ThresholdEventHandlerFunc_t handlerPtr,
        ///< [IN]
    void* contextPtr
        ///< [IN]
);

//--------------------------------------------------------------------------------------------------
/**
 * Remove handler function for EVENT 'le_ips_ThresholdEvent'
 */
//--------------------------------------------------------------------------------------------------
void le_ips_RemoveThresholdEventHandler
(
    le_ips_ThresholdEventHandlerRef_t handlerRef
        ///< [IN]
);

//--------------------------------------------------------------------------------------------------
/**
 * Get the Platform input voltage in [mV].
 *
 * @return
 *      - LE_OK            The function succeeded.
 *      - LE_FAULT         The function failed to get the value.
 *
 * @note If the caller is passing a bad pointer into this function, it is a fatal error, the
 *       function will not return.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_ips_GetInputVoltage
(
    uint32_t* inputVoltagePtr
        ///< [OUT] [OUT] The input voltage in [mV]
);

//--------------------------------------------------------------------------------------------------
/**
 * Set the Platform warning and critical input voltage thresholds in [mV].
 *  When thresholds input voltage are reached, a input voltage event is triggered.
 *
 * @return
 *      - LE_OK            The function succeeded.
 *      - LE_BAD_PARAMETER The hiCriticalVolt threshold is equal or lower than the (normalVolt+1)
 *                           threshold.
 *                         The warningVolt threshold is equal to or higher than the normalVolt
 *                           threshold.
 *                         The warningVolt threshold is equal to or lower than the criticalVolt
 *                           threshold.
 *      - LE_FAULT         The function failed to set the thresholds.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_ips_SetVoltageThresholds
(
    uint16_t criticalVolt,
        ///< [IN] [IN] The critical input voltage threshold in [mV].
    uint16_t warningVolt,
        ///< [IN] [IN] The warning input voltage threshold in [mV].
    uint16_t normalVolt,
        ///< [IN] [IN] The normal input voltage threshold in [mV].
    uint16_t hiCriticalVolt
        ///< [IN] [IN] The high critical input voltage threshold in [mV].
);

//--------------------------------------------------------------------------------------------------
/**
 * Get the Platform warning and critical input voltage thresholds in [mV].
 *
 * @return
 *      - LE_OK            The function succeeded.
 *      - LE_FAULT         The function failed to get the thresholds.
 *
 * @note If the caller is passing a bad pointer into this function, it is a fatal error, the
 *       function will not return.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_ips_GetVoltageThresholds
(
    uint16_t* criticalVoltPtr,
        ///< [OUT] [OUT] The critical input voltage threshold in [mV].
    uint16_t* warningVoltPtr,
        ///< [OUT] [OUT] The warning input voltage threshold in [mV].
    uint16_t* normalVoltPtr,
        ///< [OUT] [OUT] The normal input voltage threshold in [mV].
    uint16_t* hiCriticalVoltPtr
        ///< [OUT] [IN] The high critical input voltage threshold in [mV].
);

//--------------------------------------------------------------------------------------------------
/**
 * Get the Platform power source.
 *
 * @return
 *      - LE_OK            The function succeeded.
 *      - LE_FAULT         The function failed to get the value.
 *
 * @note If the caller is passing a bad pointer into this function, it is a fatal error, the
 *       function will not return.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_ips_GetPowerSource
(
    le_ips_PowerSource_t* powerSourcePtr
        ///< [OUT] [OUT] The power source.
);

//--------------------------------------------------------------------------------------------------
/**
 * Get the Platform battery level in percent:
 *  - 0: battery is exhausted or platform does not have a battery connected
 *  - 1 to 100: percentage of battery capacity remaining
 *
 * @return
 *      - LE_OK            The function succeeded.
 *      - LE_FAULT         The function failed to get the value.
 *
 * @note If the caller is passing a bad pointer into this function, it is a fatal error, the
 *       function will not return.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_ips_GetBatteryLevel
(
    uint8_t* batteryLevelPtr
        ///< [OUT] [OUT] The battery level in percent.
);

//--------------------------------------------------------------------------------------------------
/**
 * Set the Platform battery level in percent.
 * This is useful when an external battery is used and its level is provided by the application
 * monitoring it.
 *
 * @note The battery level set through this API will be the value reported by
 * le_ips_GetBatteryLevel() until Legato is restarted.
 *
 * @return
 *      - LE_OK            The function succeeded.
 *      - LE_BAD_PARAMETER Incorrect battery level provided.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_ips_SetBatteryLevel
(
    uint8_t batteryLevel
        ///< [IN] [IN] The battery level in percent.
);

#endif // LE_IPS_INTERFACE_H_INCLUDE_GUARD