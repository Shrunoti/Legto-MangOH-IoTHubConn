

/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */


#ifndef LE_PM_INTERFACE_H_INCLUDE_GUARD
#define LE_PM_INTERFACE_H_INCLUDE_GUARD


#include "legato.h"


//--------------------------------------------------------------------------------------------------
/**
 * Get the server service reference
 */
//--------------------------------------------------------------------------------------------------
le_msg_ServiceRef_t le_pm_GetServiceRef
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * Get the client session reference for the current message
 */
//--------------------------------------------------------------------------------------------------
le_msg_SessionRef_t le_pm_GetClientSessionRef
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * Initialize the server and advertise the service.
 */
//--------------------------------------------------------------------------------------------------
void le_pm_AdvertiseService
(
    void
);


//--------------------------------------------------------------------------------------------------
/**
 * Maximum string length for a wake-up source tag (not including the null-terminator)
 */
//--------------------------------------------------------------------------------------------------
#define LE_PM_TAG_LEN 31

//--------------------------------------------------------------------------------------------------
/**
 * Maximum string length for a wake-up source tag (including the null-terminator)
 */
//--------------------------------------------------------------------------------------------------
#define LE_PM_TAG_LEN_BYTES 32

//--------------------------------------------------------------------------------------------------
/**
 * Option LE_PM_REF_COUNT to manage a reference counted wakeup source
 */
//--------------------------------------------------------------------------------------------------
#define LE_PM_REF_COUNT 1

//--------------------------------------------------------------------------------------------------
/**
 * Reference to wakeup source used by StayAwake and Relax function
 */
//--------------------------------------------------------------------------------------------------
typedef struct le_pm_WakeupSource* le_pm_WakeupSourceRef_t;



//--------------------------------------------------------------------------------------------------
/**
 * Create a wakeup source
 *
 * @return
 *      - Reference to wakeup source (to be used later for acquiring/releasing)
 *
 * @note The process exits if an invalid or existing tag is passed
 */
//--------------------------------------------------------------------------------------------------
le_pm_WakeupSourceRef_t le_pm_NewWakeupSource
(
    uint32_t createOpts,
        ///< [IN] Wakeup source options
    const char* LE_NONNULL wsTag
        ///< [IN] Context-specific wakeup source tag
);



//--------------------------------------------------------------------------------------------------
/**
 * Acquire a wakeup source
 *
 * @return
 *     - LE_OK          if the wakeup source is acquired
 *     - LE_NO_MEMORY   if the wakeup sources limit is reached
 *     - LE_FAULT       for other errors
 *
 * @note The process exits if an invalid reference is passed
 * @note The wakeup sources limit is fixed by the kernel CONFIG_PM_WAKELOCKS_LIMIT configuration
 *       variable
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_pm_StayAwake
(
    le_pm_WakeupSourceRef_t wsRef
        ///< [IN] Reference to a created wakeup source
);



//--------------------------------------------------------------------------------------------------
/**
 * Release a previously acquired wakeup source
 *
 * @return
 *     - LE_OK          if the wakeup source is acquired
 *     - LE_NOT_FOUND   if the wakeup source was not currently acquired
 *     - LE_FAULT       for other errors
 *
 * @note The process exits if an invalid reference is passed
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_pm_Relax
(
    le_pm_WakeupSourceRef_t wsRef
        ///< [IN] Reference to a created wakeup source
);



//--------------------------------------------------------------------------------------------------
/**
 * Release and destroy all acquired wakeup source, kill all clients
 *
 * @return
 *     - LE_OK              if the wakeup source is acquired
 *     - LE_NOT_PERMITTED   if the le_pm_StayAwake() has not failed with LE_NO_MEMORY
 *     - LE_FAULT           for other errors
 *
 * @note The service is available only if le_pm_StayAwake() has returned LE_NO_MEMORY. It should be
 *       used in the way to release and destroy all wakeup sources.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_pm_ForceRelaxAndDestroyAllWakeupSource
(
    void
);


#endif // LE_PM_INTERFACE_H_INCLUDE_GUARD