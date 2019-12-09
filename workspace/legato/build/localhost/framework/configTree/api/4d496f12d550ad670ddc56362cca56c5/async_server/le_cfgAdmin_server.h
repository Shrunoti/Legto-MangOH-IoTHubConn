

/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */


#ifndef LE_CFGADMIN_INTERFACE_H_INCLUDE_GUARD
#define LE_CFGADMIN_INTERFACE_H_INCLUDE_GUARD


#include "legato.h"

//--------------------------------------------------------------------------------------------------
/**
 * Command reference for async server-side function support.  The interface function receives the
 * reference, and must pass it to the corresponding respond function.
 */
//--------------------------------------------------------------------------------------------------
typedef struct le_cfgAdmin_ServerCmd* le_cfgAdmin_ServerCmdRef_t;

// Interface specific includes
#include "le_cfg_server.h"


//--------------------------------------------------------------------------------------------------
/**
 * Get the server service reference
 */
//--------------------------------------------------------------------------------------------------
le_msg_ServiceRef_t le_cfgAdmin_GetServiceRef
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * Get the client session reference for the current message
 */
//--------------------------------------------------------------------------------------------------
le_msg_SessionRef_t le_cfgAdmin_GetClientSessionRef
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * Initialize the server and advertise the service.
 */
//--------------------------------------------------------------------------------------------------
void le_cfgAdmin_AdvertiseService
(
    void
);


//--------------------------------------------------------------------------------------------------
/**
 * Reference to an iterator object that can be used to iterate over the list of trees.
 */
//--------------------------------------------------------------------------------------------------
typedef struct le_cfgAdmin_Iterator* le_cfgAdmin_IteratorRef_t;

//--------------------------------------------------------------------------------------------------
/**
 * Server-side respond function for le_cfgAdmin_ImportTree
 */
//--------------------------------------------------------------------------------------------------
void le_cfgAdmin_ImportTreeRespond
(
    le_cfgAdmin_ServerCmdRef_t _cmdRef,
    le_result_t _result
);

//--------------------------------------------------------------------------------------------------
/**
 * Prototype for server-side async interface function
 */
//--------------------------------------------------------------------------------------------------
void le_cfgAdmin_ImportTree
(
    le_cfgAdmin_ServerCmdRef_t _cmdRef,
    le_cfg_IteratorRef_t iteratorRef,
    const char* LE_NONNULL filePath,
    const char* LE_NONNULL nodePath
);

//--------------------------------------------------------------------------------------------------
/**
 * Server-side respond function for le_cfgAdmin_ExportTree
 */
//--------------------------------------------------------------------------------------------------
void le_cfgAdmin_ExportTreeRespond
(
    le_cfgAdmin_ServerCmdRef_t _cmdRef,
    le_result_t _result
);

//--------------------------------------------------------------------------------------------------
/**
 * Prototype for server-side async interface function
 */
//--------------------------------------------------------------------------------------------------
void le_cfgAdmin_ExportTree
(
    le_cfgAdmin_ServerCmdRef_t _cmdRef,
    le_cfg_IteratorRef_t iteratorRef,
    const char* LE_NONNULL filePath,
    const char* LE_NONNULL nodePath
);

//--------------------------------------------------------------------------------------------------
/**
 * Server-side respond function for le_cfgAdmin_DeleteTree
 */
//--------------------------------------------------------------------------------------------------
void le_cfgAdmin_DeleteTreeRespond
(
    le_cfgAdmin_ServerCmdRef_t _cmdRef
);

//--------------------------------------------------------------------------------------------------
/**
 * Prototype for server-side async interface function
 */
//--------------------------------------------------------------------------------------------------
void le_cfgAdmin_DeleteTree
(
    le_cfgAdmin_ServerCmdRef_t _cmdRef,
    const char* LE_NONNULL treeName
);

//--------------------------------------------------------------------------------------------------
/**
 * Server-side respond function for le_cfgAdmin_CreateTreeIterator
 */
//--------------------------------------------------------------------------------------------------
void le_cfgAdmin_CreateTreeIteratorRespond
(
    le_cfgAdmin_ServerCmdRef_t _cmdRef,
    le_cfgAdmin_IteratorRef_t _result
);

//--------------------------------------------------------------------------------------------------
/**
 * Prototype for server-side async interface function
 */
//--------------------------------------------------------------------------------------------------
void le_cfgAdmin_CreateTreeIterator
(
    le_cfgAdmin_ServerCmdRef_t _cmdRef
);

//--------------------------------------------------------------------------------------------------
/**
 * Server-side respond function for le_cfgAdmin_ReleaseTreeIterator
 */
//--------------------------------------------------------------------------------------------------
void le_cfgAdmin_ReleaseTreeIteratorRespond
(
    le_cfgAdmin_ServerCmdRef_t _cmdRef
);

//--------------------------------------------------------------------------------------------------
/**
 * Prototype for server-side async interface function
 */
//--------------------------------------------------------------------------------------------------
void le_cfgAdmin_ReleaseTreeIterator
(
    le_cfgAdmin_ServerCmdRef_t _cmdRef,
    le_cfgAdmin_IteratorRef_t iteratorRef
);

//--------------------------------------------------------------------------------------------------
/**
 * Server-side respond function for le_cfgAdmin_GetTreeName
 */
//--------------------------------------------------------------------------------------------------
void le_cfgAdmin_GetTreeNameRespond
(
    le_cfgAdmin_ServerCmdRef_t _cmdRef,
    le_result_t _result,
    const char* name
);

//--------------------------------------------------------------------------------------------------
/**
 * Prototype for server-side async interface function
 */
//--------------------------------------------------------------------------------------------------
void le_cfgAdmin_GetTreeName
(
    le_cfgAdmin_ServerCmdRef_t _cmdRef,
    le_cfgAdmin_IteratorRef_t iteratorRef,
    size_t nameSize
);

//--------------------------------------------------------------------------------------------------
/**
 * Server-side respond function for le_cfgAdmin_NextTree
 */
//--------------------------------------------------------------------------------------------------
void le_cfgAdmin_NextTreeRespond
(
    le_cfgAdmin_ServerCmdRef_t _cmdRef,
    le_result_t _result
);

//--------------------------------------------------------------------------------------------------
/**
 * Prototype for server-side async interface function
 */
//--------------------------------------------------------------------------------------------------
void le_cfgAdmin_NextTree
(
    le_cfgAdmin_ServerCmdRef_t _cmdRef,
    le_cfgAdmin_IteratorRef_t iteratorRef
);


#endif // LE_CFGADMIN_INTERFACE_H_INCLUDE_GUARD