

/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */


#ifndef LE_IMA_INTERFACE_H_INCLUDE_GUARD
#define LE_IMA_INTERFACE_H_INCLUDE_GUARD


#include "legato.h"

// Interface specific includes
#include "le_limit_server.h"


//--------------------------------------------------------------------------------------------------
/**
 * Get the server service reference
 */
//--------------------------------------------------------------------------------------------------
le_msg_ServiceRef_t le_ima_GetServiceRef
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * Get the client session reference for the current message
 */
//--------------------------------------------------------------------------------------------------
le_msg_SessionRef_t le_ima_GetClientSessionRef
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * Initialize the server and advertise the service.
 */
//--------------------------------------------------------------------------------------------------
void le_ima_AdvertiseService
(
    void
);



//--------------------------------------------------------------------------------------------------
/**
 * Import a public certificate in linux keyring.
 *
 * @return
 *     - LE_OK if successful
 *     - LE_FAULT if there is a failure.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_ima_ImportCert
(
    const char* LE_NONNULL certPath
        ///< [IN] Path to the certificate to import.
);


#endif // LE_IMA_INTERFACE_H_INCLUDE_GUARD