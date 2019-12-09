

/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */


#ifndef LE_ULPM_INTERFACE_H_INCLUDE_GUARD
#define LE_ULPM_INTERFACE_H_INCLUDE_GUARD


#include "legato.h"


//--------------------------------------------------------------------------------------------------
/**
 * Get the server service reference
 */
//--------------------------------------------------------------------------------------------------
le_msg_ServiceRef_t le_ulpm_GetServiceRef
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * Get the client session reference for the current message
 */
//--------------------------------------------------------------------------------------------------
le_msg_SessionRef_t le_ulpm_GetClientSessionRef
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * Initialize the server and advertise the service.
 */
//--------------------------------------------------------------------------------------------------
void le_ulpm_AdvertiseService
(
    void
);


//--------------------------------------------------------------------------------------------------
/**
 * Maximum version length of mcu firmware.
 */
//--------------------------------------------------------------------------------------------------
#define LE_ULPM_MAX_VERS_LEN 8

//--------------------------------------------------------------------------------------------------
/**
 * State of gpio pin. This state will be used to exit from low power state.
 */
//--------------------------------------------------------------------------------------------------
typedef enum
{
    LE_ULPM_GPIO_LOW = 0,
        ///< Gpio voltage level low.
    LE_ULPM_GPIO_HIGH = 1,
        ///< Gpio voltage level high.
    LE_ULPM_GPIO_RISING = 2,
        ///< Gpio edge rising.
    LE_ULPM_GPIO_FALLING = 3,
        ///< Gpio edge falling.
    LE_ULPM_GPIO_BOTH = 4,
        ///< Gpio edge either rising or falling.
    LE_ULPM_GPIO_OFF = 5
        ///< Gpio off.
}
le_ulpm_GpioState_t;



//--------------------------------------------------------------------------------------------------
/**
 * Configure the system to boot based on a state change of a given GPIO.
 *
 * @return
 *      - LE_OK if specified gpio is configured as boot source.
 *      - LE_NOT_PERMITTED if the process lacks sufficient permissions to configure the GPIO as a
 *        boot source.
 *      - LE_UNSUPPORTED if the device lacks the ability to boot based on the given GPIO.
 *      - LE_BAD_PARAMETER if the state parameter was rejected.
 *      - LE_FAULT if there is a non-specific failure.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_ulpm_BootOnGpio
(
    uint32_t gpioNum,
        ///< [IN] Gpio number to boot.
    le_ulpm_GpioState_t state
        ///< [IN] State which should cause boot.
);



//--------------------------------------------------------------------------------------------------
/**
 * Boot after expiration of timer interval.
 *
 * @return
 *      - LE_OK if timer is configured as boot source.
 *      - LE_NOT_PERMITTED if the process lacks sufficient permissions to configure the timer as a
 *        boot source.
 *      - LE_UNSUPPORTED if the device lacks the ability to boot based on a timer.
 *      - LE_BAD_PARAMETER if the state parameter was rejected.
 *      - LE_FAULT if there is a non-specific failure.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_ulpm_BootOnTimer
(
    uint32_t expiryVal
        ///< [IN] Expiration time(in second) to boot. This is relative time from
        ///< modem/app processor shutdown.
);



//--------------------------------------------------------------------------------------------------
/**
 * Configure and enable an ADC as a boot source.
 *
 * It is possible to specify a single range of operation or two ranges of operation with a
 * non-operational range in between.  When bootAboveAdcReading is less than bootBelowAdcReading,
 * then a single range bootAboveReading to bootBelowReading is the configured operational range.
 * However if bootAboveAdcReading is greater than bootBelowAdcReading, then there are two
 * operational ranges.  The first is any reading less than bootBelowAdcReading and the second is any
 * reading greater than bootAboveAdcReading.
 *
 * @return
 *      - LE_OK on success
 *      - LE_NOT_PERMITTED if the process lacks sufficient permissions to configure the adc as a
 *        boot source.
 *      - LE_OVERFLOW if the provided bootAboveAdcReading or bootBelowAdcReading are too large to
 *        convert to fit in the internal string buffer.
 *      - LE_BAD_PARAMETER if the pollIntervalInMs, bootAboveAdcReading or bootBelowAdcReading
 *        parameter were rejected.
 *      - LE_UNSUPPORTED if the device does not support using the given adc as a boot source.
 *      - LE_FAULT if there is a non-specific failure.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_ulpm_BootOnAdc
(
    uint32_t adcNum,
        ///< [IN] Number of the ADC to configure
    uint32_t pollIntervalInMs,
        ///< [IN] How frequently to poll the ADC while sleeping
    double bootAboveAdcReading,
        ///< [IN] Reading above which the system should boot
    double bootBelowAdcReading
        ///< [IN] Reading below which the system should boot
);



//--------------------------------------------------------------------------------------------------
/**
 * Get the ultra low power manager firmware version.
 *
 * @return
 *      - LE_OK on success
 *      - LE_OVERFLOW if version string to big to fit in provided buffer
 *      - LE_FAULT for any other errors
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_ulpm_GetFirmwareVersion
(
    char* version,
        ///< [OUT] Firmware version string
    size_t versionSize
        ///< [IN]
);



//--------------------------------------------------------------------------------------------------
/**
 * Initiate shutting down of app processor/modem etc.
 *
 * @return
 *      - LE_OK if entry to ultra low power mode initiates properly.
 *      - LE_NOT_POSSIBLE if shutting is not possible now. Try again.
 *      - LE_NOT_PERMITTED if the process lacks sufficient permissions to perform a shutdown.
 *      - LE_UNSUPPORTED if the device lacks the ability to shutdown via ULPM.
 *      - LE_FAULT if there is a non-specific failure.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_ulpm_ShutDown
(
    void
);


#endif // LE_ULPM_INTERFACE_H_INCLUDE_GUARD