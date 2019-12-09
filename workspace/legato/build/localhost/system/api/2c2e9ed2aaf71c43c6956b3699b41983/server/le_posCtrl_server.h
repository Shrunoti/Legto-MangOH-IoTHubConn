

/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */


#ifndef LE_POSCTRL_INTERFACE_H_INCLUDE_GUARD
#define LE_POSCTRL_INTERFACE_H_INCLUDE_GUARD


#include "legato.h"


//--------------------------------------------------------------------------------------------------
/**
 * Get the server service reference
 */
//--------------------------------------------------------------------------------------------------
le_msg_ServiceRef_t le_posCtrl_GetServiceRef
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * Get the client session reference for the current message
 */
//--------------------------------------------------------------------------------------------------
le_msg_SessionRef_t le_posCtrl_GetClientSessionRef
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * Initialize the server and advertise the service.
 */
//--------------------------------------------------------------------------------------------------
void le_posCtrl_AdvertiseService
(
    void
);


//--------------------------------------------------------------------------------------------------
/**
 * Reference type for dealing with Position Service management.
 */
//--------------------------------------------------------------------------------------------------
typedef struct le_posCtrl_Activation* le_posCtrl_ActivationRef_t;



//--------------------------------------------------------------------------------------------------
/**
 * Request activation of the positioning service.
 *
 * @return
 *      - Reference to the service activation request (to be used later for releasing the request).
 *      - NULL if the service request could not be processed.
 *
 */
//--------------------------------------------------------------------------------------------------
le_posCtrl_ActivationRef_t le_posCtrl_Request
(
    void
);



//--------------------------------------------------------------------------------------------------
/**
 * Release the positioning service.
 *
 */
//--------------------------------------------------------------------------------------------------
void le_posCtrl_Release
(
    le_posCtrl_ActivationRef_t ref
        ///< [IN] Reference to a positioning service activation request.
);


#endif // LE_POSCTRL_INTERFACE_H_INCLUDE_GUARD