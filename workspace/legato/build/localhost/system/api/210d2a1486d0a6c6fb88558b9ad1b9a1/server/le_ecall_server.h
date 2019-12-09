

/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */


#ifndef LE_ECALL_INTERFACE_H_INCLUDE_GUARD
#define LE_ECALL_INTERFACE_H_INCLUDE_GUARD


#include "legato.h"

// Interface specific includes
#include "le_mdmDefs_server.h"
#include "le_mcc_server.h"


//--------------------------------------------------------------------------------------------------
/**
 * Get the server service reference
 */
//--------------------------------------------------------------------------------------------------
le_msg_ServiceRef_t le_ecall_GetServiceRef
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * Get the client session reference for the current message
 */
//--------------------------------------------------------------------------------------------------
le_msg_SessionRef_t le_ecall_GetClientSessionRef
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * Initialize the server and advertise the service.
 */
//--------------------------------------------------------------------------------------------------
void le_ecall_AdvertiseService
(
    void
);


//--------------------------------------------------------------------------------------------------
/**
 * MSD message length in bytes.
 */
//--------------------------------------------------------------------------------------------------
#define LE_ECALL_MSD_MAX_LEN 140

//--------------------------------------------------------------------------------------------------
/**
 * Vehicle Identification Number (VIN) string LENGTH. (without nulltermination)
 */
//--------------------------------------------------------------------------------------------------
#define LE_ECALL_VIN_MAX_LEN 17

//--------------------------------------------------------------------------------------------------
/**
 * Vehicle Identification Number (VIN) string BYTES. One extra byte is added for the null
 * character.
 */
//--------------------------------------------------------------------------------------------------
#define LE_ECALL_VIN_MAX_BYTES 18

//--------------------------------------------------------------------------------------------------
/**
 * Reference returned by Create function and used by other functions
 */
//--------------------------------------------------------------------------------------------------
typedef struct le_ecall_Call* le_ecall_CallRef_t;


//--------------------------------------------------------------------------------------------------
/**
 *  Configured operation modes.
 */
//--------------------------------------------------------------------------------------------------
typedef enum
{
    LE_ECALL_NORMAL_MODE = 0,
        ///< Normal mode.
    LE_ECALL_ONLY_MODE = 1,
        ///< eCall only mode according to U/SIM operation mode or forced
        ///< by application through the le_ecall_ForceOnlyMode() function.
    LE_ECALL_FORCED_PERSISTENT_ONLY_MODE = 2
        ///< Persistent eCall only mode.
}
le_ecall_OpMode_t;


//--------------------------------------------------------------------------------------------------
/**
 *  Configure which standard to follow for the eCall, either PAN European eCall or ERA GLONASS.
 */
//--------------------------------------------------------------------------------------------------
typedef enum
{
    LE_ECALL_PAN_EUROPEAN = 0,
        ///< PAN-EUROPEAN standard
    LE_ECALL_ERA_GLONASS = 1
        ///< ERA GLONASS standard
}
le_ecall_SystemStandard_t;


//--------------------------------------------------------------------------------------------------
/**
 *  eCall session states.
 */
//--------------------------------------------------------------------------------------------------
typedef enum
{
    LE_ECALL_STATE_UNKNOWN = 0,
        ///< Unknown state.
    LE_ECALL_STATE_STARTED = 1,
        ///< eCall session started.
    LE_ECALL_STATE_CONNECTED = 2,
        ///< Emergency call is established.
    LE_ECALL_STATE_DISCONNECTED = 3,
        ///< Emergency call is disconnected.
    LE_ECALL_STATE_WAITING_PSAP_START_IND = 4,
        ///< Waiting for PSAP start indication.
    LE_ECALL_STATE_PSAP_START_IND_RECEIVED = 5,
        ///< PSAP start indication received.
    LE_ECALL_STATE_MSD_TX_STARTED = 6,
        ///< MSD transmission is started.
    LE_ECALL_STATE_LLNACK_RECEIVED = 7,
        ///< LL-NACK received.
    LE_ECALL_STATE_LLACK_RECEIVED = 8,
        ///< LL-ACK received.
    LE_ECALL_STATE_MSD_TX_COMPLETED = 9,
        ///< MSD transmission is complete.
    LE_ECALL_STATE_MSD_TX_FAILED = 10,
        ///< MSD transmission has failed.
    LE_ECALL_STATE_ALACK_RECEIVED_POSITIVE = 11,
        ///< AL-ACK received.
    LE_ECALL_STATE_ALACK_RECEIVED_CLEAR_DOWN = 12,
        ///< AL-ACK clear-down received.
    LE_ECALL_STATE_STOPPED = 13,
        ///< eCall session has been stopped by PSAP
        ///< or IVS le_ecall_End().
    LE_ECALL_STATE_RESET = 14,
        ///< eCall session has lost synchronization and starts over.
    LE_ECALL_STATE_COMPLETED = 15,
        ///< eCall session completed. The modem successfully completed
        ///< the MSD transmission and received two AL-ACKs (positive).
        ///< Can now switch to voice path.
    LE_ECALL_STATE_FAILED = 16,
        ///< Unsuccessful eCall session.
    LE_ECALL_STATE_END_OF_REDIAL_PERIOD = 17,
        ///< End of the redial period.
    LE_ECALL_STATE_TIMEOUT_T2 = 18,
        ///< Timeout for Timer T2 (see CEN-EN 16062, annex A)
    LE_ECALL_STATE_TIMEOUT_T3 = 19,
        ///< Timeout for Timer T3 (see CEN-EN 16062, annex A)
    LE_ECALL_STATE_TIMEOUT_T5 = 20,
        ///< Timeout for Timer T5 (see CEN-EN 16062, annex A)
    LE_ECALL_STATE_TIMEOUT_T6 = 21,
        ///< Timeout for Timer T6 (see CEN-EN 16062, annex A)
    LE_ECALL_STATE_TIMEOUT_T7 = 22,
        ///< Timeout for Timer T7 (see CEN-EN 16062, annex A)
    LE_ECALL_STATE_TIMEOUT_T9 = 23,
        ///< Timeout for Timer T9 (see CEN-EN 16062, annex A)
    LE_ECALL_STATE_TIMEOUT_T10 = 24
        ///< Timeout for Timer T10 (see CEN-EN 16062, annex A)
}
le_ecall_State_t;


//--------------------------------------------------------------------------------------------------
/**
 *  eCall MSD transmission mode.
 */
//--------------------------------------------------------------------------------------------------
typedef enum
{
    LE_ECALL_TX_MODE_PULL = 0,
        ///< Pull mode (modem/host waits for MSD request from PSAP to send MSD).
    LE_ECALL_TX_MODE_PUSH = 1
        ///< Push mode (modem/host sends MSD to PSAP right after eCall is connected).
}
le_ecall_MsdTxMode_t;


//--------------------------------------------------------------------------------------------------
/**
 *  eCall MSD vehicle type
 */
//--------------------------------------------------------------------------------------------------
typedef enum
{
    LE_ECALL_MSD_VEHICLE_PASSENGER_M1 = 0,
        ///< Passenger vehicle (Class M1)
    LE_ECALL_MSD_VEHICLE_BUS_M2 = 1,
        ///< Buses and coaches (Class M2)
    LE_ECALL_MSD_VEHICLE_BUS_M3 = 2,
        ///< Buses and coaches (Class M3)
    LE_ECALL_MSD_VEHICLE_COMMERCIAL_N1 = 3,
        ///< Light commercial vehicles (Class N1)
    LE_ECALL_MSD_VEHICLE_HEAVY_N2 = 4,
        ///< Heavy duty vehicles (Class N2)
    LE_ECALL_MSD_VEHICLE_HEAVY_N3 = 5,
        ///< Heavy duty vehicles (Class N3)
    LE_ECALL_MSD_VEHICLE_MOTORCYCLE_L1E = 6,
        ///< Motorcycles (Class L1e)
    LE_ECALL_MSD_VEHICLE_MOTORCYCLE_L2E = 7,
        ///< Motorcycles (Class L2e)
    LE_ECALL_MSD_VEHICLE_MOTORCYCLE_L3E = 8,
        ///< Motorcycles (Class L3e)
    LE_ECALL_MSD_VEHICLE_MOTORCYCLE_L4E = 9,
        ///< Motorcycles (Class L4e)
    LE_ECALL_MSD_VEHICLE_MOTORCYCLE_L5E = 10,
        ///< Motorcycles (Class L5e)
    LE_ECALL_MSD_VEHICLE_MOTORCYCLE_L6E = 11,
        ///< Motorcycles (Class L6e)
    LE_ECALL_MSD_VEHICLE_MOTORCYCLE_L7E = 12
        ///< Motorcycles (Class L7e)
}
le_ecall_MsdVehicleType_t;


//--------------------------------------------------------------------------------------------------
/**
 * Propulsion type Bit Mask
 */
//--------------------------------------------------------------------------------------------------
typedef enum
{
    LE_ECALL_PROPULSION_TYPE_GASOLINE = 0x1,        ///< Gasoline propulsion)
    LE_ECALL_PROPULSION_TYPE_DIESEL = 0x2,        ///< Diesel propulsion
    LE_ECALL_PROPULSION_TYPE_NATURALGAS = 0x4,        ///< Compressed natural gas propulsion
    LE_ECALL_PROPULSION_TYPE_PROPANE = 0x8,        ///< Liquid propane gas propulsion
    LE_ECALL_PROPULSION_TYPE_ELECTRIC = 0x10,        ///< Electric propulsion
    LE_ECALL_PROPULSION_TYPE_HYDROGEN = 0x20,        ///< Hydrogen propulsion
    LE_ECALL_PROPULSION_TYPE_OTHER = 0x40        ///< Other type of propulsions
}
le_ecall_PropulsionTypeBitMask_t;


//--------------------------------------------------------------------------------------------------
/**
 * Diagnostic result Bit Mask (64 bits)
 *
 */
//--------------------------------------------------------------------------------------------------
typedef enum
{
    LE_ECALL_DIAG_RESULT_PRESENT_MIC_CONNECTION_FAILURE = 0x1,        ///< Present Mic connection failure.
    LE_ECALL_DIAG_RESULT_MIC_CONNECTION_FAILURE = 0x2,        ///< Mic connection failure status.
    LE_ECALL_DIAG_RESULT_PRESENT_MIC_FAILURE = 0x4,        ///< Present Mic  failure.
    LE_ECALL_DIAG_RESULT_MIC_FAILURE = 0x8,        ///< Mic failure status.
    LE_ECALL_DIAG_RESULT_PRESENT_RIGHT_SPEAKER_FAILURE = 0x10,        ///< Present Right speaker failure.
    LE_ECALL_DIAG_RESULT_RIGHT_SPEAKER_FAILURE = 0x20,        ///< Right speaker failure status.
    LE_ECALL_DIAG_RESULT_PRESENT_LEFT_SPEAKER_FAILURE = 0x40,        ///< Present Left speaker failure.
    LE_ECALL_DIAG_RESULT_LEFT_SPEAKER_FAILURE = 0x80,        ///< Left speaker failure status.
    LE_ECALL_DIAG_RESULT_PRESENT_SPEAKERS_FAILURE = 0x100,        ///< Present Speaker failure.
    LE_ECALL_DIAG_RESULT_SPEAKERS_FAILURE = 0x200,        ///< Speaker failure status.
    LE_ECALL_DIAG_RESULT_PRESENT_IGNITION_LINE_FAILURE = 0x400,        ///< Present Ignition line failure.
    LE_ECALL_DIAG_RESULT_IGNITION_LINE_FAILURE = 0x800,        ///< Ignition line failure status.
    LE_ECALL_DIAG_RESULT_PRESENT_UIM_FAILURE = 0x1000,        ///< Present UIM failure.
    LE_ECALL_DIAG_RESULT_UIM_FAILURE = 0x2000,        ///< UIM failure status.
    LE_ECALL_DIAG_RESULT_PRESENT_STATUS_INDICATOR_FAILURE = 0x4000,        ///< Present Status indicator failure.
    LE_ECALL_DIAG_RESULT_STATUS_INDICATOR_FAILURE = 0x8000,        ///< Status indicator failure status.
    LE_ECALL_DIAG_RESULT_PRESENT_BATTERY_FAILURE = 0x10000,        ///< Present Battery failure.
    LE_ECALL_DIAG_RESULT_BATTERY_FAILURE = 0x20000,        ///< Battery failure status.
    LE_ECALL_DIAG_RESULT_PRESENT_BATTERY_VOLTAGE_LOW = 0x40000,        ///< Present Battery voltage low failure.
    LE_ECALL_DIAG_RESULT_BATTERY_VOLTAGE_LOW = 0x80000,        ///< Battery voltage low failure status.
    LE_ECALL_DIAG_RESULT_PRESENT_CRASH_SENSOR_FAILURE = 0x100000,        ///< Present Crash sensor failure.
    LE_ECALL_DIAG_RESULT_CRASH_SENSOR_FAILURE = 0x200000,        ///< Crash sensor failure status.
    LE_ECALL_DIAG_RESULT_PRESENT_FIRMWARE_IMAGE_CORRUPTION = 0x400000,        ///< Present Firmware image corruption failure.
    LE_ECALL_DIAG_RESULT_FIRMWARE_IMAGE_CORRUPTION = 0x800000,        ///< Firmware image corruption failure status.
    LE_ECALL_DIAG_RESULT_PRESENT_COMM_MODULE_INTERFACE_FAILURE = 0x1000000,        ///< Present Comm module interface failure.
    LE_ECALL_DIAG_RESULT_COMM_MODULE_INTERFACE_FAILURE = 0x2000000,        ///< Comm module interface failure status.
    LE_ECALL_DIAG_RESULT_PRESENT_GNSS_RECEIVER_FAILURE = 0x4000000,        ///< Present GNSS receiver failure.
    LE_ECALL_DIAG_RESULT_GNSS_RECEIVER_FAILURE = 0x8000000,        ///< GNSS receiver failure status.
    LE_ECALL_DIAG_RESULT_PRESENT_RAIM_PROBLEM = 0x10000000,        ///< Present RAIM problem.
    LE_ECALL_DIAG_RESULT_RAIM_PROBLEM = 0x20000000,        ///< RAIM problem status.
    LE_ECALL_DIAG_RESULT_PRESENT_GNSS_ANTENNA_FAILURE = 0x40000000,        ///< Present GNSS antenna failure.
    LE_ECALL_DIAG_RESULT_GNSS_ANTENNA_FAILURE = 0x80000000,        ///< GNSS antenna failure status.
    LE_ECALL_DIAG_RESULT_PRESENT_COMM_MODULE_FAILURE = 0x100000000,        ///< Present Comm module failure.
    LE_ECALL_DIAG_RESULT_COMM_MODULE_FAILURE = 0x200000000,        ///< Comm module failure status.
    LE_ECALL_DIAG_RESULT_PRESENT_EVENTS_MEMORY_OVERFLOW = 0x400000000,        ///< Present Events memory overflow.
    LE_ECALL_DIAG_RESULT_EVENTS_MEMORY_OVERFLOW = 0x800000000,        ///< Events memory overflow status.
    LE_ECALL_DIAG_RESULT_PRESENT_CRASH_PROFILE_MEMORY_OVERFLOW = 0x1000000000,        ///< Present Crash profile memory overflow.
    LE_ECALL_DIAG_RESULT_CRASH_PROFILE_MEMORY_OVERFLOW = 0x2000000000,        ///< Crash profile memory overflow status.
    LE_ECALL_DIAG_RESULT_PRESENT_OTHER_CRITICAL_FAILURES = 0x4000000000,        ///< Present Other critical failures.
    LE_ECALL_DIAG_RESULT_OTHER_CRITICAL_FAILURES = 0x8000000000,        ///< Other critical failures status.
    LE_ECALL_DIAG_RESULT_PRESENT_OTHER_NOT_CRITICAL_FAILURES = 0x10000000000,        ///< Present Other not critical failures.
    LE_ECALL_DIAG_RESULT_OTHER_NOT_CRITICAL_FAILURES = 0x20000000000        ///< Other not critical failures status.
}
le_ecall_DiagnosticResultBitMask_t;


//--------------------------------------------------------------------------------------------------
/**
 * Crash information Bit Mask (16 bits) indicating the type of road accident.
 *
 */
//--------------------------------------------------------------------------------------------------
typedef enum
{
    LE_ECALL_CRASH_INFO_PRESENT_CRASH_FRONT = 0x1,        ///< Present Front collision indicator.
    LE_ECALL_CRASH_INFO_CRASH_FRONT = 0x2,        ///< Front collision indicator.
    LE_ECALL_CRASH_INFO_PRESENT_CRASH_LEFT = 0x4,        ///< Present Left collision indicator.
    LE_ECALL_CRASH_INFO_CRASH_LEFT = 0x8,        ///< Left collision indicator.
    LE_ECALL_CRASH_INFO_PRESENT_CRASH_RIGHT = 0x10,        ///< Present Right collision indicator
    LE_ECALL_CRASH_INFO_CRASH_RIGHT = 0x20,        ///< Right collision indicator
    LE_ECALL_CRASH_INFO_PRESENT_CRASH_REAR = 0x40,        ///< Present indicator
    LE_ECALL_CRASH_INFO_CRASH_REAR = 0x80,        ///< Rear impact indicator.
    LE_ECALL_CRASH_INFO_PRESENT_CRASH_ROLLOVER = 0x100,        ///< Present Rollover indicator.
    LE_ECALL_CRASH_INFO_CRASH_ROLLOVER = 0x200,        ///< Rollover indicator.
    LE_ECALL_CRASH_INFO_PRESENT_CRASH_SIDE = 0x400,        ///< Present Side collision indicator.
    LE_ECALL_CRASH_INFO_CRASH_SIDE = 0x800,        ///< Side collision indicator.
    LE_ECALL_CRASH_INFO_PRESENT_CRASH_FRONT_OR_SIDE = 0x1000,        ///< Present Front or side collision indicator.
    LE_ECALL_CRASH_INFO_CRASH_FRONT_OR_SIDE = 0x2000,        ///< Front or side collision indicator.
    LE_ECALL_CRASH_INFO_PRESENT_CRASH_ANOTHER_TYPE = 0x4000,        ///< Present Other types of damage indicator.
    LE_ECALL_CRASH_INFO_CRASH_ANOTHER_TYPE = 0x8000        ///< Other types of damage indicator.
}
le_ecall_CrashInfoBitMask_t;


//--------------------------------------------------------------------------------------------------
/**
 *  eCall MSD coordinate system type
 */
//--------------------------------------------------------------------------------------------------
typedef enum
{
    LE_ECALL_MSD_COORDINATE_SYSTEM_TYPE_ABSENT = 0,
        ///< Coordinate system type is absent
    LE_ECALL_MSD_COORDINATE_SYSTEM_TYPE_WGS84 = 1,
        ///< Coordinate system type WGS-84
    LE_ECALL_MSD_COORDINATE_SYSTEM_TYPE_PZ90 = 2
        ///< Coordinate system type PZ-90
}
le_ecall_MsdCoordinateType_t;


//--------------------------------------------------------------------------------------------------
/**
 * Reference type used by Add/Remove functions for EVENT 'le_ecall_StateChange'
 */
//--------------------------------------------------------------------------------------------------
typedef struct le_ecall_StateChangeHandler* le_ecall_StateChangeHandlerRef_t;


//--------------------------------------------------------------------------------------------------
/**
 * Handler for eCall state changes.
 *
 */
//--------------------------------------------------------------------------------------------------
typedef void (*le_ecall_StateChangeHandlerFunc_t)
(
    le_ecall_CallRef_t ecallRef,
        ///< eCall reference
    le_ecall_State_t state,
        ///< eCall state
    void* contextPtr
        ///<
);


//--------------------------------------------------------------------------------------------------
/**
 * This function configures the eCall operation mode to eCall only, only emergency number can be
 * used to start an eCall session. The modem doesn't try to register on the Cellular network.
 * This function forces the modem to behave as eCall only mode whatever U/SIM operation mode.
 * The change doesn't persist over power cycles.
 * This function can be called before making an eCall.
 *
 * @return
 *      - LE_OK on success
 *      - LE_FAULT for other failures
 *      - LE_UNSUPPORTED Not supported on this platform
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_ecall_ForceOnlyMode
(
    void
);



//--------------------------------------------------------------------------------------------------
/**
 * Same as le_ecall_ForceOnlyMode(), but the change persists over power cycles.
 *
 * @return
 *      - LE_OK on success
 *      - LE_FAULT for other failures
 *      - LE_UNSUPPORTED Not supported on this platform
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_ecall_ForcePersistentOnlyMode
(
    void
);



//--------------------------------------------------------------------------------------------------
/**
 * This function exits from eCall Only mode. It configures the eCall operation mode to Normal mode,
 * the modem uses the default operation mode at power up (or after U/SIM hotswap). The modem behaves
 * following the U/SIM eCall operation mode; for example the U/SIM can be configured only for eCall,
 * or a combination of eCall and commercial service provision.
 *
 * @return
 *      - LE_OK on success
 *      - LE_FAULT for other failures
 *      - LE_UNSUPPORTED Not supported on this platform
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_ecall_ExitOnlyMode
(
    void
);



//--------------------------------------------------------------------------------------------------
/**
 * Get the configured Operation mode.
 *
 * @return
 *      - LE_OK     on success
 *      - LE_FAULT  for other failures
 *      - LE_UNSUPPORTED Not supported on this platform
 *
 * @note If the caller is passing a bad pointer into this function, it is a fatal error, the
 *       function will not return.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_ecall_GetConfiguredOperationMode
(
    le_ecall_OpMode_t* opModePtr
        ///< [OUT] Operation mode
);



//--------------------------------------------------------------------------------------------------
/**
 * Add handler function for EVENT 'le_ecall_StateChange'
 *
 * This event provides information on eCall state changes.
 *
 */
//--------------------------------------------------------------------------------------------------
le_ecall_StateChangeHandlerRef_t le_ecall_AddStateChangeHandler
(
    le_ecall_StateChangeHandlerFunc_t handlerPtr,
        ///< [IN]
    void* contextPtr
        ///< [IN]
);



//--------------------------------------------------------------------------------------------------
/**
 * Remove handler function for EVENT 'le_ecall_StateChange'
 */
//--------------------------------------------------------------------------------------------------
void le_ecall_RemoveStateChangeHandler
(
    le_ecall_StateChangeHandlerRef_t handlerRef
        ///< [IN]
);



//--------------------------------------------------------------------------------------------------
/**
 * Create a new eCall object
 *
 * The eCall is not actually established at this point. It's still up to the caller to call
 * le_ecall_Start() when ready.
 *
 * @return
 *      - A reference to the new Call object.
 *
 * @note On failure, the process exits; you don't have to worry about checking the returned
 *       reference for validity.
 */
//--------------------------------------------------------------------------------------------------
le_ecall_CallRef_t le_ecall_Create
(
    void
);



//--------------------------------------------------------------------------------------------------
/**
 * Call to free up a call reference.
 *
 * @note This will free the reference, but not necessarily stop an active eCall. If there are
 *       other holders of this reference the eCall will remain active.
 */
//--------------------------------------------------------------------------------------------------
void le_ecall_Delete
(
    le_ecall_CallRef_t ecallRef
        ///< [IN] eCall reference
);



//--------------------------------------------------------------------------------------------------
/**
 * Set the system standard.
 * Default is PAN EUROPEAN
 *
 * @return
 *  - LE_OK on success
 *  - LE_FAULT for other failures
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_ecall_SetSystemStandard
(
    le_ecall_SystemStandard_t systemStandard
        ///< [IN] System standard
);



//--------------------------------------------------------------------------------------------------
/**
 * Get the system standard.
 *
 * @return
 *  - LE_OK on success
 *  - LE_FAULT for other failures
 *  - LE_BAD_PARAMETER parameter is NULL
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_ecall_GetSystemStandard
(
    le_ecall_SystemStandard_t* systemStandardPtr
        ///< [OUT] System Standard
);



//--------------------------------------------------------------------------------------------------
/**
 * Set the MSDs version.
 * Default value is 1
 *
 * @return
 *  - LE_OK on success
 *  - LE_FAULT for other failures
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_ecall_SetMsdVersion
(
    uint32_t msdVersion
        ///< [IN] Msd version
);



//--------------------------------------------------------------------------------------------------
/**
 * Get the MSD version.
 *
 * @return
 *  - LE_OK on success
 *  - LE_FAULT for other failures
 *  - LE_BAD_PARAMETER parameter is NULL
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_ecall_GetMsdVersion
(
    uint32_t* msdVersionPtr
        ///< [OUT] Msd version
);



//--------------------------------------------------------------------------------------------------
/**
 * Set the Vehicled Type
 * Default value is 0
 *
 * @return
 *  - LE_OK on success
 *  - LE_FAULT for other failures
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_ecall_SetVehicleType
(
    le_ecall_MsdVehicleType_t vehicleType
        ///< [IN] Vehicle type
);



//--------------------------------------------------------------------------------------------------
/**
 * Get the Vehicled Type.
 *
 * @return
 *  - LE_OK on success
 *  - LE_BAD_PARAMETER parameter is NULL
 *  - LE_FAULT for other failures
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_ecall_GetVehicleType
(
    le_ecall_MsdVehicleType_t* vehicleTypePtr
        ///< [OUT] Vehicle type
);



//--------------------------------------------------------------------------------------------------
/**
 * Set the VIN (Vehicle Identification Number).
 *
 * @return
 *  - LE_OK on success
 *  - LE_BAD_PARAMETER parameter is NULL.
 *  - LE_FAULT for other failures
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_ecall_SetVIN
(
    const char* LE_NONNULL vin
        ///< [IN] VIN (Vehicle Identification Number)
);



//--------------------------------------------------------------------------------------------------
/**
 * Get the VIN (Vehicle Identification Number).
 *
 * @return
 *  - LE_OK on success
 *  - LE_NOT_FOUND if the value is not set.
 *  - LE_BAD_PARAMETER parameter is NULL or to small
 *  - LE_FAULT for other failures
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_ecall_GetVIN
(
    char* vin,
        ///< [OUT] VIN is gotten with a null termination.
    size_t vinSize
        ///< [IN]
);



//--------------------------------------------------------------------------------------------------
/**
 * Set the propulsion type.
 * Note that a vehicle may have more than one propulsion type.
 *
 * @return
 *  - LE_OK on success
 *  - LE_FAULT for other failures
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_ecall_SetPropulsionType
(
    le_ecall_PropulsionTypeBitMask_t propulsionType
        ///< [IN] bitmask
);



//--------------------------------------------------------------------------------------------------
/**
 * Get the propulsion stored.
 * Note that a vehicle may have more than one propulsion type.
 *
 * @return
 *  - LE_OK on success
 *  - LE_NOT_FOUND if the value is not set.
 *  - LE_FAULT for other failures
 *  - LE_BAD_PARAMETER parameter is NULL
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_ecall_GetPropulsionType
(
    le_ecall_PropulsionTypeBitMask_t* propulsionTypePtr
        ///< [OUT] bitmask
);



//--------------------------------------------------------------------------------------------------
/**
 * Set the push/pull transmission mode.
 *
 * @return
 *  - LE_OK on success
 *  - LE_FAULT for other failures
 *  - LE_UNSUPPORTED Not supported on this platform
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_ecall_SetMsdTxMode
(
    le_ecall_MsdTxMode_t mode
        ///< [IN] Transmission mode
);



//--------------------------------------------------------------------------------------------------
/**
 * Get the push/pull transmission mode.
 *
 * @return
 *      - LE_OK     on success
 *      - LE_FAULT  for other failures
 *      - LE_UNSUPPORTED Not supported on this platform
 *
 * @note If the caller is passing a bad pointer into this function, it is a fatal error, the
 *       function will not return.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_ecall_GetMsdTxMode
(
    le_ecall_MsdTxMode_t* modePtr
        ///< [OUT] Transmission mode
);



//--------------------------------------------------------------------------------------------------
/**
 * Set the position transmitted by the MSD.
 *
 * @return
 *      - LE_OK on success
 *      - LE_DUPLICATE an MSD has been already imported
 *      - LE_BAD_PARAMETER bad input parameter
 *      - LE_FAULT on other failures
 *
 * @note The process exits, if an invalid eCall reference is given
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_ecall_SetMsdPosition
(
    le_ecall_CallRef_t ecallRef,
        ///< [IN] eCall reference
    bool isTrusted,
        ///< [IN] true if the position is accurate, false otherwise
    int32_t latitude,
        ///< [IN] latitude in degrees with 6 decimal places, positive North.
        ///< Maximum value is +90 degrees (+90000000), minimum value is -90
        ///< degrees (-90000000).
    int32_t longitude,
        ///< [IN] longitude in degrees with 6 decimal places, positive East.
        ///< Maximum value is +180 degrees (+180000000), minimum value is
        ///< -180 degrees (-180000000).
    int32_t direction
        ///< [IN] direction of the vehicle from magnetic north (0 to 358, clockwise)
        ///< in 2-degrees unit. Valid range is 0 to 179. If direction of
        ///< travel is invalid or unknown, the value 0xFF shall be used.
);



//--------------------------------------------------------------------------------------------------
/**
 * Set the position Delta N-1 from position set in le_ecall_SetMsdPosition() transmitted by the MSD.
 *
 * @return
 *      - LE_OK on success
 *      - LE_DUPLICATE an MSD has been already imported
 *      - LE_BAD_PARAMETER bad input parameter
 *      - LE_FAULT on other failures
 *
 * @note The process exits, if an invalid eCall reference is given
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_ecall_SetMsdPositionN1
(
    le_ecall_CallRef_t ecallRef,
        ///< [IN] eCall reference
    int32_t latitudeDeltaN1,
        ///< [IN] longitude delta from position set in SetMsdPosition
        ///< 1 Unit = 100 miliarcseconds, which is approximately 3m
        ///< maximum value: 511 = 0 0'51.100'' (+- 1580m)
        ///< minimum value: -512 = -0 0'51.200'' (+- -1583m)
    int32_t longitudeDeltaN1
        ///< [IN] longitude delta from position set in SetMsdPosition
        ///< 1 Unit = 100 miliarcseconds, which is approximately 3m
        ///< maximum value: 511 = 0 0'51.100'' (+-1580m)
        ///< minimum value: -512 = -0 0'51.200'' (+- -1583m) )
);



//--------------------------------------------------------------------------------------------------
/**
 * Set the position Delta N-2 from position set in le_ecall_SetMsdPositionN1() transmitted by the MSD.
 *
 * @return
 *      - LE_OK on success
 *      - LE_DUPLICATE an MSD has been already imported
 *      - LE_BAD_PARAMETER bad input parameter
 *      - LE_FAULT on other failures
 *
 * @note The process exits, if an invalid eCall reference is given
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_ecall_SetMsdPositionN2
(
    le_ecall_CallRef_t ecallRef,
        ///< [IN] eCall reference
    int32_t latitudeDeltaN2,
        ///< [IN] longitude delta from position set in SetMsdPositionN1
        ///< 1 Unit = 100 miliarcseconds, which is approximately 3m
        ///< maximum value: 511 = 0 0'51.100'' (+-1580m)
        ///< minimum value: -512 = -0 0'51.200'' (+- -1583m)
    int32_t longitudeDeltaN2
        ///< [IN] longitude delta from position set in SetMsdPositionN1
        ///< 1 Unit = 100 miliarcseconds, which is approximately 3m
        ///< maximum value: 511 = 0 0'51.100'' (+-1580m)
        ///< minimum value: -512 = -0 0'51.200'' (+- -1583m) )
);



//--------------------------------------------------------------------------------------------------
/**
 * Set the number of passengers transmitted by the MSD.
 *
 * @return
 *      - LE_OK on success
 *      - LE_DUPLICATE an MSD has been already imported
 *      - LE_BAD_PARAMETER bad eCall reference
 *
 * @note The process exits, if an invalid eCall reference is given
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_ecall_SetMsdPassengersCount
(
    le_ecall_CallRef_t ecallRef,
        ///< [IN] eCall reference
    uint32_t paxCount
        ///< [IN] number of passengers
);



//--------------------------------------------------------------------------------------------------
/**
 * Import an already prepared MSD.
 *
 * MSD is transmitted only after an emergency call has been established.
 *
 * @return
 *      - LE_OK on success
 *      - LE_OVERFLOW The imported MSD length exceeds the MSD_MAX_LEN maximum length.
 *      - LE_BAD_PARAMETER bad eCall reference
 *      - LE_FAULT for other failures
 *
 * @note On failure, the process exits; you don't have to worry about checking the returned
 *       reference for validity.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_ecall_ImportMsd
(
    le_ecall_CallRef_t ecallRef,
        ///< [IN] eCall reference
    const uint8_t* msdPtr,
        ///< [IN] the prepared MSD
    size_t msdSize
        ///< [IN]
);



//--------------------------------------------------------------------------------------------------
/**
 * Export the encoded MSD.
 *
 * @return
 *  - LE_OK on success
 *  - LE_OVERFLOW  The encoded MSD length exceeds the user's buffer length.
 *  - LE_NOT_FOUND  No encoded MSD is available.
 *  - LE_BAD_PARAMETER bad eCall reference.
 *  - LE_FAULT for other failures.
 *
 * @note If the caller is passing a bad pointer into this function, it is a fatal error, the
 *       function will not return.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_ecall_ExportMsd
(
    le_ecall_CallRef_t ecallRef,
        ///< [IN] eCall reference
    uint8_t* msdPtr,
        ///< [OUT] the encoded MSD
    size_t* msdSizePtr
        ///< [INOUT]
);



//--------------------------------------------------------------------------------------------------
/**
 * Send the MSD.
 *
 * @return
 *      - LE_OK on success
 *      - LE_BAD_PARAMETER bad eCall reference
 *      - LE_FAULT for other failures
 *      - LE_UNSUPPORTED Not supported on this platform
 *
 * @note On failure, the process exits, so you don't have to worry about checking the returned
 *       reference for validity.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_ecall_SendMsd
(
    le_ecall_CallRef_t ecallRef
        ///< [IN] eCall reference
);



//--------------------------------------------------------------------------------------------------
/**
 * Start an automatic eCall session
 *
 * @return
 *      - LE_OK on success
 *      - LE_BUSY an eCall session is already in progress
 *      - LE_BAD_PARAMETER bad eCall reference
 *      - LE_FAULT for other failures
 *      - LE_UNSUPPORTED Not supported on this platform
 *
 * @note The process exits, if an invalid eCall reference is given
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_ecall_StartAutomatic
(
    le_ecall_CallRef_t ecallRef
        ///< [IN] eCall reference
);



//--------------------------------------------------------------------------------------------------
/**
 * Start a manual eCall session
 *
 * @return
 *      - LE_OK on success
 *      - LE_BUSY an eCall session is already in progress
 *      - LE_BAD_PARAMETER bad eCall reference
 *      - LE_FAULT for other failures
 *      - LE_UNSUPPORTED Not supported on this platform
 *
 * @note The process exits, if an invalid eCall reference is given
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_ecall_StartManual
(
    le_ecall_CallRef_t ecallRef
        ///< [IN] eCall reference
);



//--------------------------------------------------------------------------------------------------
/**
 * Start a test eCall  session
 *
 * @return
 *      - LE_OK on success
 *      - LE_BUSY an eCall session is already in progress
 *      - LE_BAD_PARAMETER bad eCall reference
 *      - LE_FAULT for other failures
 *      - LE_UNSUPPORTED Not supported on this platform
 *
 * @note The process exits, if an invalid eCall reference is given
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_ecall_StartTest
(
    le_ecall_CallRef_t ecallRef
        ///< [IN] eCall reference
);



//--------------------------------------------------------------------------------------------------
/**
 * End the current eCall session
 *
 * @return
 *      - LE_OK on success
 *      - LE_BAD_PARAMETER bad eCall reference
 *      - LE_FAULT for other failures
 *      - LE_UNSUPPORTED Not supported on this platform
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_ecall_End
(
    le_ecall_CallRef_t ecallRef
        ///< [IN] eCall reference
);



//--------------------------------------------------------------------------------------------------
/**
 * Get the current state for the given eCall
 *
 * @return
 *      - The current state for the given eCall
 *
 * @note The process exits, if an invalid eCall reference is given
 */
//--------------------------------------------------------------------------------------------------
le_ecall_State_t le_ecall_GetState
(
    le_ecall_CallRef_t ecallRef
        ///< [IN] eCall reference
);



//--------------------------------------------------------------------------------------------------
/**
 * Set the Public Safely Answering Point telephone number.
 *
 * @note That PSAP number is not applied to Manually or Automatically initiated eCall. For those
 *       modes, an emergency call is launched.
 *
 * @warning This function doesn't modified the U/SIM content.
 *
 * @return
 *  - LE_OK on success
 *  - LE_FAULT for other failures
 *
 * @note If PSAP number is too long (max LE_MDMDEFS_PHONE_NUM_MAX_LEN digits), it is a fatal error,
 *       the function will not return.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_ecall_SetPsapNumber
(
    const char* LE_NONNULL psap
        ///< [IN] Public Safely Answering Point number
);



//--------------------------------------------------------------------------------------------------
/**
 * Get the Public Safely Answering Point telephone number set with le_ecall_SetPsapNumber()
 * function.
 *
 * @note That PSAP number is not applied to Manually or Automatically initiated eCall. For those
 *       modes, an emergency call is launched.
 *
 * @warning This function doesn't read the U/SIM content.
 *
 * @return
 *  - LE_OK on success
 *  - LE_FAULT on failures or if le_ecall_SetPsapNumber() has never been called before.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_ecall_GetPsapNumber
(
    char* psap,
        ///< [OUT] Public Safely Answering Point telephone number
    size_t psapSize
        ///< [IN]
);



//--------------------------------------------------------------------------------------------------
/**
 * When modem is in ECALL_FORCED_PERSISTENT_ONLY_MODE or ECALL_ONLY_MODE, this function
 * can be called to request the modem to read the number to dial from the FDN/SDN of the U/SIM.
 *
 * @note If FDN directory is updated with new dial numbers, be sure that the SIM card is refreshed.
 *
 * @return
 *  - LE_OK on success
 *  - LE_FAULT for other failures
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_ecall_UseUSimNumbers
(
    void
);



//--------------------------------------------------------------------------------------------------
/**
 * Set the NAD (network access device) deregistration time value. After termination of an emergency
 * call the in-vehicle system remains registered on the network for the period of time, defined by
 * the installation parameter NAD (network access device) deregistration time.
 *
 * @return
 *  - LE_OK on success
 *  - LE_FAULT on failure
 *
 * @note The formula to calculate NAD deregistration time for PAN_EUROPEAN is as below:
 *       ECallConfiguration.nad_deregistration_time = (deregTime+59)/60;
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_ecall_SetNadDeregistrationTime
(
    uint16_t deregTime
        ///< [IN] the NAD (network access device) deregistration time value (in minutes).
);



//--------------------------------------------------------------------------------------------------
/**
 * Get the NAD (network access device) deregistration time value.
 *
 * @return
 *  - LE_OK on success
 *  - LE_FAULT on failure
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_ecall_GetNadDeregistrationTime
(
    uint16_t* deregTimePtr
        ///< [OUT] the NAD (network access device) deregistration time value
);



//--------------------------------------------------------------------------------------------------
/**
 * Set the minimum interval value between dial attempts.
 *
 * @return
 *  - LE_OK on success
 *  - LE_FAULT for other failures
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_ecall_SetIntervalBetweenDialAttempts
(
    uint16_t pause
        ///< [IN] the minimum interval value in seconds
);



//--------------------------------------------------------------------------------------------------
/**
 * Get the minimum interval value between dial attempts.
 *
 * @return
 *  - LE_OK on success
 *  - LE_FAULT for other failures
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_ecall_GetIntervalBetweenDialAttempts
(
    uint16_t* pausePtr
        ///< [OUT] the minimum interval value in seconds
);



//--------------------------------------------------------------------------------------------------
/**
 * Set the ECALL_MANUAL_DIAL_ATTEMPTS value. If a dial attempt under manual emergency call
 * initiation failed, it should be repeated maximally ECALL_MANUAL_DIAL_ATTEMPTS-1 times within
 * the maximal time limit of ECALL_DIAL_DURATION. The default value is 10.
 * Redial attempts stop once the call has been cleared down correctly, or if counter/timer reached
 * their limits.
 *
 * @return
 *  - LE_OK on success
 *  - LE_FAULT on failure
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_ecall_SetEraGlonassManualDialAttempts
(
    uint16_t attempts
        ///< [IN] the ECALL_MANUAL_DIAL_ATTEMPTS value
);



//--------------------------------------------------------------------------------------------------
/**
 * Set the ECALL_AUTO_DIAL_ATTEMPTS value. If a dial attempt under automatic emergency call
 * initiation failed, it should be repeated maximally ECALL_AUTO_DIAL_ATTEMPTS-1 times within
 * the maximal time limit of ECALL_DIAL_DURATION. The default value is 10.
 * Redial attempts stop once the call has been cleared down correctly, or if counter/timer reached
 * their limits.
 *
 * @return
 *  - LE_OK on success
 *  - LE_FAULT on failure
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_ecall_SetEraGlonassAutoDialAttempts
(
    uint16_t attempts
        ///< [IN] the ECALL_AUTO_DIAL_ATTEMPTS value
);



//--------------------------------------------------------------------------------------------------
/**
 * Set the ECALL_DIAL_DURATION time. It's the maximum time the IVS have to connect the emergency
 * call to the PSAP, including all redial attempts.
 * If the call is not connected within this time (or ManualDialAttempts/AutoDialAttempts dial
 * attempts), it will stop.
 *
 * @return
 *  - LE_OK on success
 *  - LE_FAULT on failure
 *  - LE_UNSUPPORTED if the function is not supported by the target
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_ecall_SetEraGlonassDialDuration
(
    uint16_t duration
        ///< [IN] the ECALL_DIAL_DURATION time value (in seconds)
);



//--------------------------------------------------------------------------------------------------
/**
 * Set the ECALL_CCFT time. It's the maximum delay before initiating an an automatic call
 * termination.
 * When the delay is reached and IVS NAD didn't receive a call clear-down indication then the call
 * is immediatly terminated.
 *
 * @note Allowed range of values is 1 to 720 minutes.
 *
 * @return
 *  - LE_OK on success
 *  - LE_FAULT on failure
 *  - LE_UNSUPPORTED if the function is not supported by the target
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_ecall_SetEraGlonassFallbackTime
(
    uint16_t duration
        ///< [IN] the ECALL_CCFT time value (in minutes)
);



//--------------------------------------------------------------------------------------------------
/**
 * Set the ECALL_AUTO_ANSWER_TIME time, the time interval wherein IVDS responds to incoming calls
 * automatically after emergency call completion.
 *
 * @note Default value of auto answer time is 20 minutes. Maximum value is 720 minutes.
 *
 * @return
 *  - LE_OK on success
 *  - LE_FAULT on failure
 *  - LE_UNSUPPORTED if the function is not supported by the target
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_ecall_SetEraGlonassAutoAnswerTime
(
    uint16_t autoAnswerTime
        ///< [IN] The ECALL_AUTO_ANSWER_TIME time value (in minutes)
);



//--------------------------------------------------------------------------------------------------
/**
 * Set the ECALL_MSD_MAX_TRANSMISSION_TIME time, the time period for MSD transmission.
 *
 * @note Default value of MSD transmission time is 20 seconds.
 *
 * @return
 *  - LE_OK on success
 *  - LE_FAULT on failure
 *  - LE_UNSUPPORTED if the function is not supported by the target
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_ecall_SetEraGlonassMSDMaxTransmissionTime
(
    uint16_t msdMaxTransTime
        ///< [IN] The ECALL_MSD_MAX_TRANSMISSION_TIME time value (in seconds)
);



//--------------------------------------------------------------------------------------------------
/**
 * Get the ECALL_MANUAL_DIAL_ATTEMPTS value.
 *
 * @return
 *  - LE_OK on success
 *  - LE_FAULT on failure
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_ecall_GetEraGlonassManualDialAttempts
(
    uint16_t* attemptsPtr
        ///< [OUT] the ECALL_MANUAL_DIAL_ATTEMPTS value
);



//--------------------------------------------------------------------------------------------------
/**
 * Get the ECALL_AUTO_DIAL_ATTEMPTS value.
 *
 * @return
 *  - LE_OK on success
 *  - LE_FAULT on failure
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_ecall_GetEraGlonassAutoDialAttempts
(
    uint16_t* attemptsPtr
        ///< [OUT] the ECALL_AUTO_DIAL_ATTEMPTS value
);



//--------------------------------------------------------------------------------------------------
/**
 * Get the ECALL_DIAL_DURATION time.
 *
 * @return
 *  - LE_OK on success
 *  - LE_FAULT on failure
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_ecall_GetEraGlonassDialDuration
(
    uint16_t* durationPtr
        ///< [OUT] the ECALL_DIAL_DURATION time value (in seconds)
);



//--------------------------------------------------------------------------------------------------
/**
 * Get the ECALL_CCFT time.
 *
 * @return
 *  - LE_OK on success
 *  - LE_FAULT on failure
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_ecall_GetEraGlonassFallbackTime
(
    uint16_t* durationPtr
        ///< [OUT] the ECALL_CCFT time value (in minutes)
);



//--------------------------------------------------------------------------------------------------
/**
 * Get the ECALL_AUTO_ANSWER_TIME time.
 *
 * @return
 *  - LE_OK on success
 *  - LE_FAULT on execution failure
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_ecall_GetEraGlonassAutoAnswerTime
(
    uint16_t* autoAnswerTimePtr
        ///< [OUT] The ECALL_AUTO_ANSWER_TIME time value (in minutes)
);



//--------------------------------------------------------------------------------------------------
/**
 * Get the ECALL_MSD_MAX_TRANSMISSION_TIME time.
 *
 * @return
 *  - LE_OK on success
 *  - LE_FAULT on failure
 *  - LE_UNSUPPORTED if the function is not supported by the target
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_ecall_GetEraGlonassMSDMaxTransmissionTime
(
    uint16_t* msdMaxTransTimePtr
        ///< [OUT] The ECALL_MSD_MAX_TRANSMISSION_TIME time value (in seconds)
);



//--------------------------------------------------------------------------------------------------
/**
 * Set the ERA-GLONASS crash severity parameter.
 *
 * @return
 *  - LE_OK on success
 *  - LE_DUPLICATE an MSD has been already imported
 *  - LE_BAD_PARAMETER bad eCall reference
 *  - LE_FAULT on other failures
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_ecall_SetMsdEraGlonassCrashSeverity
(
    le_ecall_CallRef_t ecallRef,
        ///< [IN] eCall reference
    uint32_t crashSeverity
        ///< [IN] the ERA-GLONASS crash severity parameter
);



//--------------------------------------------------------------------------------------------------
/**
 * Reset the ERA-GLONASS crash severity parameter. Therefore that optional parameter is not included
 * in the MSD message.
 *
 * @return
 *  - LE_OK on success
 *  - LE_DUPLICATE an MSD has been already imported
 *  - LE_BAD_PARAMETER bad eCall reference
 *  - LE_FAULT on other failures
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_ecall_ResetMsdEraGlonassCrashSeverity
(
    le_ecall_CallRef_t ecallRef
        ///< [IN] eCall reference
);



//--------------------------------------------------------------------------------------------------
/**
 * Set the ERA-GLONASS diagnostic result using a bit mask.
 *
 * @return
 *  - LE_OK on success
 *  - LE_DUPLICATE an MSD has been already imported
 *  - LE_BAD_PARAMETER bad eCall reference
 *  - LE_FAULT on other failures
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_ecall_SetMsdEraGlonassDiagnosticResult
(
    le_ecall_CallRef_t ecallRef,
        ///< [IN] eCall reference
    le_ecall_DiagnosticResultBitMask_t diagnosticResultMask
        ///< [IN] ERA-GLONASS diagnostic result bit mask.
);



//--------------------------------------------------------------------------------------------------
/**
 * Reset the ERA-GLONASS diagnostic result bit mask. Optional parameter is not
 * included in the MSD message.
 *
 * @return
 *  - LE_OK on success
 *  - LE_DUPLICATE an MSD has been already imported
 *  - LE_BAD_PARAMETER bad eCall reference
 *  - LE_FAULT on other failures
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_ecall_ResetMsdEraGlonassDiagnosticResult
(
    le_ecall_CallRef_t ecallRef
        ///< [IN] eCall reference
);



//--------------------------------------------------------------------------------------------------
/**
 * Set the ERA-GLONASS crash type bit mask
 *
 * @return
 *  - LE_OK on success
 *  - LE_DUPLICATE an MSD has been already imported
 *  - LE_BAD_PARAMETER bad eCall reference
 *  - LE_FAULT on other failures
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_ecall_SetMsdEraGlonassCrashInfo
(
    le_ecall_CallRef_t ecallRef,
        ///< [IN] eCall reference
    le_ecall_CrashInfoBitMask_t crashInfoMask
        ///< [IN] ERA-GLONASS crash type bit mask.
);



//--------------------------------------------------------------------------------------------------
/**
 * Reset the ERA-GLONASS crash type bit mask. Optional parameter is not included
 * in the MSD message.
 *
 * @return
 *  - LE_OK on success
 *  - LE_DUPLICATE an MSD has been already imported
 *  - LE_BAD_PARAMETER bad eCall reference
 *  - LE_FAULT on other failures
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_ecall_ResetMsdEraGlonassCrashInfo
(
    le_ecall_CallRef_t ecallRef
        ///< [IN] eCall reference
);



//--------------------------------------------------------------------------------------------------
/**
 * Set the ERA-GLONASS coordinate system type.
 *
 * @return
 *  - LE_OK on success
 *  - LE_DUPLICATE an MSD has been already imported
 *  - LE_BAD_PARAMETER bad eCall reference
 *  - LE_FAULT on other failures
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_ecall_SetMsdEraGlonassCoordinateSystemType
(
    le_ecall_CallRef_t ecallRef,
        ///< [IN] eCall reference
    le_ecall_MsdCoordinateType_t coordinateSystemType
        ///< [IN] the ERA-GLONASS coordinate system type
);



//--------------------------------------------------------------------------------------------------
/**
 * Reset the ERA-GLONASS coordinate system type. Therefore that optional parameter is not included
 * in the MSD message.
 *
 * @return
 *  - LE_OK on success
 *  - LE_DUPLICATE an MSD has been already imported
 *  - LE_BAD_PARAMETER bad eCall reference
 *  - LE_FAULT on other failures
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_ecall_ResetMsdEraGlonassCoordinateSystemType
(
    le_ecall_CallRef_t ecallRef
        ///< [IN] eCall reference
);



//--------------------------------------------------------------------------------------------------
/**
 * Called to get the termination reason.
 *
 * @return The termination reason.
 *
 * @note If the caller is passing a bad pointer into this function, it is a fatal error, the
 *       function will not return.
 */
//--------------------------------------------------------------------------------------------------
le_mcc_TerminationReason_t le_ecall_GetTerminationReason
(
    le_ecall_CallRef_t ecallRef
        ///< [IN] eCall reference.
);



//--------------------------------------------------------------------------------------------------
/**
 * Called to get the platform specific termination code.
 *
 * @return The platform specific termination code.
 *
 * @note If the caller is passing a bad pointer into this function, it is a fatal error, the
 *       function will not return.
 */
//--------------------------------------------------------------------------------------------------
int32_t le_ecall_GetPlatformSpecificTerminationCode
(
    le_ecall_CallRef_t ecallRef
        ///< [IN] eCall reference.
);


#endif // LE_ECALL_INTERFACE_H_INCLUDE_GUARD