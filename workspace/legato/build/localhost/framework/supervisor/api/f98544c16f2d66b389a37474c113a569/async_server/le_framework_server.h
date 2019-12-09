

/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */


#ifndef LE_FRAMEWORK_INTERFACE_H_INCLUDE_GUARD
#define LE_FRAMEWORK_INTERFACE_H_INCLUDE_GUARD


#include "legato.h"

//--------------------------------------------------------------------------------------------------
/**
 * Command reference for async server-side function support.  The interface function receives the
 * reference, and must pass it to the corresponding respond function.
 */
//--------------------------------------------------------------------------------------------------
typedef struct le_framework_ServerCmd* le_framework_ServerCmdRef_t;


//--------------------------------------------------------------------------------------------------
/**
 * Get the server service reference
 */
//--------------------------------------------------------------------------------------------------
le_msg_ServiceRef_t le_framework_GetServiceRef
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * Get the client session reference for the current message
 */
//--------------------------------------------------------------------------------------------------
le_msg_SessionRef_t le_framework_GetClientSessionRef
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * Initialize the server and advertise the service.
 */
//--------------------------------------------------------------------------------------------------
void le_framework_AdvertiseService
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * Server-side respond function for le_framework_Stop
 */
//--------------------------------------------------------------------------------------------------
void le_framework_StopRespond
(
    le_framework_ServerCmdRef_t _cmdRef,
    le_result_t _result
);

//--------------------------------------------------------------------------------------------------
/**
 * Prototype for server-side async interface function
 */
//--------------------------------------------------------------------------------------------------
void le_framework_Stop
(
    le_framework_ServerCmdRef_t _cmdRef
);

//--------------------------------------------------------------------------------------------------
/**
 * Server-side respond function for le_framework_Restart
 */
//--------------------------------------------------------------------------------------------------
void le_framework_RestartRespond
(
    le_framework_ServerCmdRef_t _cmdRef,
    le_result_t _result
);

//--------------------------------------------------------------------------------------------------
/**
 * Prototype for server-side async interface function
 */
//--------------------------------------------------------------------------------------------------
void le_framework_Restart
(
    le_framework_ServerCmdRef_t _cmdRef,
    bool manualRestart
);

//--------------------------------------------------------------------------------------------------
/**
 * Server-side respond function for le_framework_IsStopping
 */
//--------------------------------------------------------------------------------------------------
void le_framework_IsStoppingRespond
(
    le_framework_ServerCmdRef_t _cmdRef,
    bool _result
);

//--------------------------------------------------------------------------------------------------
/**
 * Prototype for server-side async interface function
 */
//--------------------------------------------------------------------------------------------------
void le_framework_IsStopping
(
    le_framework_ServerCmdRef_t _cmdRef
);

//--------------------------------------------------------------------------------------------------
/**
 * Server-side respond function for le_framework_IsReadOnly
 */
//--------------------------------------------------------------------------------------------------
void le_framework_IsReadOnlyRespond
(
    le_framework_ServerCmdRef_t _cmdRef,
    bool _result
);

//--------------------------------------------------------------------------------------------------
/**
 * Prototype for server-side async interface function
 */
//--------------------------------------------------------------------------------------------------
void le_framework_IsReadOnly
(
    le_framework_ServerCmdRef_t _cmdRef
);

//--------------------------------------------------------------------------------------------------
/**
 * Server-side respond function for le_framework_NotifyExpectedReboot
 */
//--------------------------------------------------------------------------------------------------
void le_framework_NotifyExpectedRebootRespond
(
    le_framework_ServerCmdRef_t _cmdRef
);

//--------------------------------------------------------------------------------------------------
/**
 * Prototype for server-side async interface function
 */
//--------------------------------------------------------------------------------------------------
void le_framework_NotifyExpectedReboot
(
    le_framework_ServerCmdRef_t _cmdRef
);


#endif // LE_FRAMEWORK_INTERFACE_H_INCLUDE_GUARD