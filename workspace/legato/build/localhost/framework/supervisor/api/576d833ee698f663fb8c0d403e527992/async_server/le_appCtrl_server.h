

/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */


#ifndef LE_APPCTRL_INTERFACE_H_INCLUDE_GUARD
#define LE_APPCTRL_INTERFACE_H_INCLUDE_GUARD


#include "legato.h"

//--------------------------------------------------------------------------------------------------
/**
 * Command reference for async server-side function support.  The interface function receives the
 * reference, and must pass it to the corresponding respond function.
 */
//--------------------------------------------------------------------------------------------------
typedef struct le_appCtrl_ServerCmd* le_appCtrl_ServerCmdRef_t;

// Interface specific includes
#include "le_limit_server.h"


//--------------------------------------------------------------------------------------------------
/**
 * Get the server service reference
 */
//--------------------------------------------------------------------------------------------------
le_msg_ServiceRef_t le_appCtrl_GetServiceRef
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * Get the client session reference for the current message
 */
//--------------------------------------------------------------------------------------------------
le_msg_SessionRef_t le_appCtrl_GetClientSessionRef
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * Initialize the server and advertise the service.
 */
//--------------------------------------------------------------------------------------------------
void le_appCtrl_AdvertiseService
(
    void
);


//--------------------------------------------------------------------------------------------------
/**
 */
//--------------------------------------------------------------------------------------------------
typedef struct le_appCtrl_App* le_appCtrl_AppRef_t;


//--------------------------------------------------------------------------------------------------
/**
 * Reference type used by Add/Remove functions for EVENT 'le_appCtrl_TraceAttach'
 */
//--------------------------------------------------------------------------------------------------
typedef struct le_appCtrl_TraceAttachHandler* le_appCtrl_TraceAttachHandlerRef_t;


//--------------------------------------------------------------------------------------------------
/**
 * Handler for attaching to a process that is to be traced.  The process is blocked allowing the
 * tracer to attach to it.  The tracer must call TraceUnblock() to unblock the traced process.
 */
//--------------------------------------------------------------------------------------------------
typedef void (*le_appCtrl_TraceAttachHandlerFunc_t)
(
    le_appCtrl_AppRef_t appRef,
        ///< Ref to the app.
    int32_t pid,
        ///< PID of a process that can be attached to.
    const char* LE_NONNULL procName,
        ///< Name of the process name.
    void* contextPtr
        ///<
);
//--------------------------------------------------------------------------------------------------
/**
 * Server-side respond function for le_appCtrl_GetRef
 */
//--------------------------------------------------------------------------------------------------
void le_appCtrl_GetRefRespond
(
    le_appCtrl_ServerCmdRef_t _cmdRef,
    le_appCtrl_AppRef_t _result
);

//--------------------------------------------------------------------------------------------------
/**
 * Prototype for server-side async interface function
 */
//--------------------------------------------------------------------------------------------------
void le_appCtrl_GetRef
(
    le_appCtrl_ServerCmdRef_t _cmdRef,
    const char* LE_NONNULL appName
);

//--------------------------------------------------------------------------------------------------
/**
 * Server-side respond function for le_appCtrl_ReleaseRef
 */
//--------------------------------------------------------------------------------------------------
void le_appCtrl_ReleaseRefRespond
(
    le_appCtrl_ServerCmdRef_t _cmdRef
);

//--------------------------------------------------------------------------------------------------
/**
 * Prototype for server-side async interface function
 */
//--------------------------------------------------------------------------------------------------
void le_appCtrl_ReleaseRef
(
    le_appCtrl_ServerCmdRef_t _cmdRef,
    le_appCtrl_AppRef_t appRef
);

//--------------------------------------------------------------------------------------------------
/**
 * Server-side respond function for le_appCtrl_SetRun
 */
//--------------------------------------------------------------------------------------------------
void le_appCtrl_SetRunRespond
(
    le_appCtrl_ServerCmdRef_t _cmdRef
);

//--------------------------------------------------------------------------------------------------
/**
 * Prototype for server-side async interface function
 */
//--------------------------------------------------------------------------------------------------
void le_appCtrl_SetRun
(
    le_appCtrl_ServerCmdRef_t _cmdRef,
    le_appCtrl_AppRef_t appRef,
    const char* LE_NONNULL procName,
    bool run
);

//--------------------------------------------------------------------------------------------------
/**
 * Server-side respond function for le_appCtrl_Import
 */
//--------------------------------------------------------------------------------------------------
void le_appCtrl_ImportRespond
(
    le_appCtrl_ServerCmdRef_t _cmdRef,
    le_result_t _result
);

//--------------------------------------------------------------------------------------------------
/**
 * Prototype for server-side async interface function
 */
//--------------------------------------------------------------------------------------------------
void le_appCtrl_Import
(
    le_appCtrl_ServerCmdRef_t _cmdRef,
    le_appCtrl_AppRef_t appRef,
    const char* LE_NONNULL path
);

//--------------------------------------------------------------------------------------------------
/**
 * Server-side respond function for le_appCtrl_SetDevicePerm
 */
//--------------------------------------------------------------------------------------------------
void le_appCtrl_SetDevicePermRespond
(
    le_appCtrl_ServerCmdRef_t _cmdRef,
    le_result_t _result
);

//--------------------------------------------------------------------------------------------------
/**
 * Prototype for server-side async interface function
 */
//--------------------------------------------------------------------------------------------------
void le_appCtrl_SetDevicePerm
(
    le_appCtrl_ServerCmdRef_t _cmdRef,
    le_appCtrl_AppRef_t appRef,
    const char* LE_NONNULL path,
    const char* LE_NONNULL permissions
);



//--------------------------------------------------------------------------------------------------
/**
 * Add handler function for EVENT 'le_appCtrl_TraceAttach'
 *
 * Event that indicates the process is blocked and can be attached to.
 */
//--------------------------------------------------------------------------------------------------
le_appCtrl_TraceAttachHandlerRef_t le_appCtrl_AddTraceAttachHandler
(
    le_appCtrl_AppRef_t appRef,
        ///< [IN] Ref to the app.
    le_appCtrl_TraceAttachHandlerFunc_t attachToPidPtr,
        ///< [IN] Attach handler to register.
    void* contextPtr
        ///< [IN]
);



//--------------------------------------------------------------------------------------------------
/**
 * Remove handler function for EVENT 'le_appCtrl_TraceAttach'
 */
//--------------------------------------------------------------------------------------------------
void le_appCtrl_RemoveTraceAttachHandler
(
    le_appCtrl_TraceAttachHandlerRef_t handlerRef
        ///< [IN]
);

//--------------------------------------------------------------------------------------------------
/**
 * Server-side respond function for le_appCtrl_TraceUnblock
 */
//--------------------------------------------------------------------------------------------------
void le_appCtrl_TraceUnblockRespond
(
    le_appCtrl_ServerCmdRef_t _cmdRef
);

//--------------------------------------------------------------------------------------------------
/**
 * Prototype for server-side async interface function
 */
//--------------------------------------------------------------------------------------------------
void le_appCtrl_TraceUnblock
(
    le_appCtrl_ServerCmdRef_t _cmdRef,
    le_appCtrl_AppRef_t appRef,
    int32_t pid
);

//--------------------------------------------------------------------------------------------------
/**
 * Server-side respond function for le_appCtrl_Start
 */
//--------------------------------------------------------------------------------------------------
void le_appCtrl_StartRespond
(
    le_appCtrl_ServerCmdRef_t _cmdRef,
    le_result_t _result
);

//--------------------------------------------------------------------------------------------------
/**
 * Prototype for server-side async interface function
 */
//--------------------------------------------------------------------------------------------------
void le_appCtrl_Start
(
    le_appCtrl_ServerCmdRef_t _cmdRef,
    const char* LE_NONNULL appName
);

//--------------------------------------------------------------------------------------------------
/**
 * Server-side respond function for le_appCtrl_Stop
 */
//--------------------------------------------------------------------------------------------------
void le_appCtrl_StopRespond
(
    le_appCtrl_ServerCmdRef_t _cmdRef,
    le_result_t _result
);

//--------------------------------------------------------------------------------------------------
/**
 * Prototype for server-side async interface function
 */
//--------------------------------------------------------------------------------------------------
void le_appCtrl_Stop
(
    le_appCtrl_ServerCmdRef_t _cmdRef,
    const char* LE_NONNULL appName
);


#endif // LE_APPCTRL_INTERFACE_H_INCLUDE_GUARD