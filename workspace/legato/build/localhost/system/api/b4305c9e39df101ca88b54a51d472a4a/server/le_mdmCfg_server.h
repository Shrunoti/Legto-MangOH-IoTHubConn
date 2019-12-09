

/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */


#ifndef LE_MDMCFG_INTERFACE_H_INCLUDE_GUARD
#define LE_MDMCFG_INTERFACE_H_INCLUDE_GUARD


#include "legato.h"


//--------------------------------------------------------------------------------------------------
/**
 * Get the server service reference
 */
//--------------------------------------------------------------------------------------------------
le_msg_ServiceRef_t le_mdmCfg_GetServiceRef
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * Get the client session reference for the current message
 */
//--------------------------------------------------------------------------------------------------
le_msg_SessionRef_t le_mdmCfg_GetClientSessionRef
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * Initialize the server and advertise the service.
 */
//--------------------------------------------------------------------------------------------------
void le_mdmCfg_AdvertiseService
(
    void
);



//--------------------------------------------------------------------------------------------------
/**
 * Store the modem current configuration.
 *
 * @return
 *    - LE_OK on success
 *    - LE_FAULT for other failures
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_mdmCfg_StoreCurrentConfiguration
(
    void
);



//--------------------------------------------------------------------------------------------------
/**
 * Restore previously saved the modem configuration.
 *
 * @return
 *    - LE_OK on success
 *    - LE_FAULT for other failures
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_mdmCfg_RestoreSavedConfiguration
(
    void
);


#endif // LE_MDMCFG_INTERFACE_H_INCLUDE_GUARD