

/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */


#ifndef LE_SUP_CTRL_INTERFACE_H_INCLUDE_GUARD
#define LE_SUP_CTRL_INTERFACE_H_INCLUDE_GUARD


#include "legato.h"

//--------------------------------------------------------------------------------------------------
/**
 * Command reference for async server-side function support.  The interface function receives the
 * reference, and must pass it to the corresponding respond function.
 */
//--------------------------------------------------------------------------------------------------
typedef struct le_sup_ctrl_ServerCmd* le_sup_ctrl_ServerCmdRef_t;

// Interface specific includes
#include "le_limit_server.h"


//--------------------------------------------------------------------------------------------------
/**
 * Get the server service reference
 */
//--------------------------------------------------------------------------------------------------
le_msg_ServiceRef_t le_sup_ctrl_GetServiceRef
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * Get the client session reference for the current message
 */
//--------------------------------------------------------------------------------------------------
le_msg_SessionRef_t le_sup_ctrl_GetClientSessionRef
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * Initialize the server and advertise the service.
 */
//--------------------------------------------------------------------------------------------------
void le_sup_ctrl_AdvertiseService
(
    void
);


//--------------------------------------------------------------------------------------------------
/**
 */
//--------------------------------------------------------------------------------------------------
typedef struct le_sup_ctrl_App* le_sup_ctrl_AppRef_t;

//--------------------------------------------------------------------------------------------------
/**
 * Server-side respond function for le_sup_ctrl_GetAppRef
 */
//--------------------------------------------------------------------------------------------------
void le_sup_ctrl_GetAppRefRespond
(
    le_sup_ctrl_ServerCmdRef_t _cmdRef,
    le_sup_ctrl_AppRef_t _result
);

//--------------------------------------------------------------------------------------------------
/**
 * Prototype for server-side async interface function
 */
//--------------------------------------------------------------------------------------------------
void le_sup_ctrl_GetAppRef
(
    le_sup_ctrl_ServerCmdRef_t _cmdRef,
    const char* LE_NONNULL appName
);

//--------------------------------------------------------------------------------------------------
/**
 * Server-side respond function for le_sup_ctrl_ReleaseAppRef
 */
//--------------------------------------------------------------------------------------------------
void le_sup_ctrl_ReleaseAppRefRespond
(
    le_sup_ctrl_ServerCmdRef_t _cmdRef
);

//--------------------------------------------------------------------------------------------------
/**
 * Prototype for server-side async interface function
 */
//--------------------------------------------------------------------------------------------------
void le_sup_ctrl_ReleaseAppRef
(
    le_sup_ctrl_ServerCmdRef_t _cmdRef,
    le_sup_ctrl_AppRef_t appRef
);

//--------------------------------------------------------------------------------------------------
/**
 * Server-side respond function for le_sup_ctrl_SetRun
 */
//--------------------------------------------------------------------------------------------------
void le_sup_ctrl_SetRunRespond
(
    le_sup_ctrl_ServerCmdRef_t _cmdRef
);

//--------------------------------------------------------------------------------------------------
/**
 * Prototype for server-side async interface function
 */
//--------------------------------------------------------------------------------------------------
void le_sup_ctrl_SetRun
(
    le_sup_ctrl_ServerCmdRef_t _cmdRef,
    le_sup_ctrl_AppRef_t appRef,
    const char* LE_NONNULL procName,
    bool run
);

//--------------------------------------------------------------------------------------------------
/**
 * Server-side respond function for le_sup_ctrl_StartApp
 */
//--------------------------------------------------------------------------------------------------
void le_sup_ctrl_StartAppRespond
(
    le_sup_ctrl_ServerCmdRef_t _cmdRef,
    le_result_t _result
);

//--------------------------------------------------------------------------------------------------
/**
 * Prototype for server-side async interface function
 */
//--------------------------------------------------------------------------------------------------
void le_sup_ctrl_StartApp
(
    le_sup_ctrl_ServerCmdRef_t _cmdRef,
    const char* LE_NONNULL appName
);

//--------------------------------------------------------------------------------------------------
/**
 * Server-side respond function for le_sup_ctrl_StopApp
 */
//--------------------------------------------------------------------------------------------------
void le_sup_ctrl_StopAppRespond
(
    le_sup_ctrl_ServerCmdRef_t _cmdRef,
    le_result_t _result
);

//--------------------------------------------------------------------------------------------------
/**
 * Prototype for server-side async interface function
 */
//--------------------------------------------------------------------------------------------------
void le_sup_ctrl_StopApp
(
    le_sup_ctrl_ServerCmdRef_t _cmdRef,
    const char* LE_NONNULL appName
);

//--------------------------------------------------------------------------------------------------
/**
 * Server-side respond function for le_sup_ctrl_StopLegato
 */
//--------------------------------------------------------------------------------------------------
void le_sup_ctrl_StopLegatoRespond
(
    le_sup_ctrl_ServerCmdRef_t _cmdRef,
    le_result_t _result
);

//--------------------------------------------------------------------------------------------------
/**
 * Prototype for server-side async interface function
 */
//--------------------------------------------------------------------------------------------------
void le_sup_ctrl_StopLegato
(
    le_sup_ctrl_ServerCmdRef_t _cmdRef
);

//--------------------------------------------------------------------------------------------------
/**
 * Server-side respond function for le_sup_ctrl_RestartLegato
 */
//--------------------------------------------------------------------------------------------------
void le_sup_ctrl_RestartLegatoRespond
(
    le_sup_ctrl_ServerCmdRef_t _cmdRef,
    le_result_t _result
);

//--------------------------------------------------------------------------------------------------
/**
 * Prototype for server-side async interface function
 */
//--------------------------------------------------------------------------------------------------
void le_sup_ctrl_RestartLegato
(
    le_sup_ctrl_ServerCmdRef_t _cmdRef,
    bool manualRestart
);


#endif // LE_SUP_CTRL_INTERFACE_H_INCLUDE_GUARD