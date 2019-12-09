

/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */


#ifndef LE_CFG_INTERFACE_H_INCLUDE_GUARD
#define LE_CFG_INTERFACE_H_INCLUDE_GUARD


#include "legato.h"

//--------------------------------------------------------------------------------------------------
/**
 * Command reference for async server-side function support.  The interface function receives the
 * reference, and must pass it to the corresponding respond function.
 */
//--------------------------------------------------------------------------------------------------
typedef struct le_cfg_ServerCmd* le_cfg_ServerCmdRef_t;


//--------------------------------------------------------------------------------------------------
/**
 * Get the server service reference
 */
//--------------------------------------------------------------------------------------------------
le_msg_ServiceRef_t le_cfg_GetServiceRef
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * Get the client session reference for the current message
 */
//--------------------------------------------------------------------------------------------------
le_msg_SessionRef_t le_cfg_GetClientSessionRef
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * Initialize the server and advertise the service.
 */
//--------------------------------------------------------------------------------------------------
void le_cfg_AdvertiseService
(
    void
);


//--------------------------------------------------------------------------------------------------
/**
 * Length of the strings used by this API.
 */
//--------------------------------------------------------------------------------------------------
#define LE_CFG_STR_LEN 511

//--------------------------------------------------------------------------------------------------
/**
 * Length of the strings used by this API, including the trailing NULL.
 */
//--------------------------------------------------------------------------------------------------
#define LE_CFG_STR_LEN_BYTES 512

//--------------------------------------------------------------------------------------------------
/**
 * Allowed length of a node name.
 */
//--------------------------------------------------------------------------------------------------
#define LE_CFG_NAME_LEN 127

//--------------------------------------------------------------------------------------------------
/**
 * The node name length, including a trailing NULL.
 */
//--------------------------------------------------------------------------------------------------
#define LE_CFG_NAME_LEN_BYTES 128

//--------------------------------------------------------------------------------------------------
/**
 * Reference to a tree iterator object.
 */
//--------------------------------------------------------------------------------------------------
typedef struct le_cfg_Iterator* le_cfg_IteratorRef_t;


//--------------------------------------------------------------------------------------------------
/**
 * Identifies the data type of node.
 */
//--------------------------------------------------------------------------------------------------
typedef enum
{
    LE_CFG_TYPE_EMPTY = 0,
        ///< A node with no value.
    LE_CFG_TYPE_STRING = 1,
        ///< A string encoded as utf8.
    LE_CFG_TYPE_BOOL = 2,
        ///< Boolean value.
    LE_CFG_TYPE_INT = 3,
        ///< Signed 32-bit.
    LE_CFG_TYPE_FLOAT = 4,
        ///< 64-bit floating point value.
    LE_CFG_TYPE_STEM = 5,
        ///< Non-leaf node, this node is the parent of other nodes.
    LE_CFG_TYPE_DOESNT_EXIST = 6
        ///< Node doesn't exist.
}
le_cfg_nodeType_t;


//--------------------------------------------------------------------------------------------------
/**
 * Reference type used by Add/Remove functions for EVENT 'le_cfg_Change'
 */
//--------------------------------------------------------------------------------------------------
typedef struct le_cfg_ChangeHandler* le_cfg_ChangeHandlerRef_t;


//--------------------------------------------------------------------------------------------------
/**
 * Handler for node change notifications.
 */
//--------------------------------------------------------------------------------------------------
typedef void (*le_cfg_ChangeHandlerFunc_t)
(
    void* contextPtr
        ///<
);
//--------------------------------------------------------------------------------------------------
/**
 * Server-side respond function for le_cfg_CreateReadTxn
 */
//--------------------------------------------------------------------------------------------------
void le_cfg_CreateReadTxnRespond
(
    le_cfg_ServerCmdRef_t _cmdRef,
    le_cfg_IteratorRef_t _result
);

//--------------------------------------------------------------------------------------------------
/**
 * Prototype for server-side async interface function
 */
//--------------------------------------------------------------------------------------------------
void le_cfg_CreateReadTxn
(
    le_cfg_ServerCmdRef_t _cmdRef,
    const char* LE_NONNULL basePath
);

//--------------------------------------------------------------------------------------------------
/**
 * Server-side respond function for le_cfg_CreateWriteTxn
 */
//--------------------------------------------------------------------------------------------------
void le_cfg_CreateWriteTxnRespond
(
    le_cfg_ServerCmdRef_t _cmdRef,
    le_cfg_IteratorRef_t _result
);

//--------------------------------------------------------------------------------------------------
/**
 * Prototype for server-side async interface function
 */
//--------------------------------------------------------------------------------------------------
void le_cfg_CreateWriteTxn
(
    le_cfg_ServerCmdRef_t _cmdRef,
    const char* LE_NONNULL basePath
);

//--------------------------------------------------------------------------------------------------
/**
 * Server-side respond function for le_cfg_CommitTxn
 */
//--------------------------------------------------------------------------------------------------
void le_cfg_CommitTxnRespond
(
    le_cfg_ServerCmdRef_t _cmdRef
);

//--------------------------------------------------------------------------------------------------
/**
 * Prototype for server-side async interface function
 */
//--------------------------------------------------------------------------------------------------
void le_cfg_CommitTxn
(
    le_cfg_ServerCmdRef_t _cmdRef,
    le_cfg_IteratorRef_t iteratorRef
);

//--------------------------------------------------------------------------------------------------
/**
 * Server-side respond function for le_cfg_CancelTxn
 */
//--------------------------------------------------------------------------------------------------
void le_cfg_CancelTxnRespond
(
    le_cfg_ServerCmdRef_t _cmdRef
);

//--------------------------------------------------------------------------------------------------
/**
 * Prototype for server-side async interface function
 */
//--------------------------------------------------------------------------------------------------
void le_cfg_CancelTxn
(
    le_cfg_ServerCmdRef_t _cmdRef,
    le_cfg_IteratorRef_t iteratorRef
);

//--------------------------------------------------------------------------------------------------
/**
 * Server-side respond function for le_cfg_GoToNode
 */
//--------------------------------------------------------------------------------------------------
void le_cfg_GoToNodeRespond
(
    le_cfg_ServerCmdRef_t _cmdRef
);

//--------------------------------------------------------------------------------------------------
/**
 * Prototype for server-side async interface function
 */
//--------------------------------------------------------------------------------------------------
void le_cfg_GoToNode
(
    le_cfg_ServerCmdRef_t _cmdRef,
    le_cfg_IteratorRef_t iteratorRef,
    const char* LE_NONNULL newPath
);

//--------------------------------------------------------------------------------------------------
/**
 * Server-side respond function for le_cfg_GoToParent
 */
//--------------------------------------------------------------------------------------------------
void le_cfg_GoToParentRespond
(
    le_cfg_ServerCmdRef_t _cmdRef,
    le_result_t _result
);

//--------------------------------------------------------------------------------------------------
/**
 * Prototype for server-side async interface function
 */
//--------------------------------------------------------------------------------------------------
void le_cfg_GoToParent
(
    le_cfg_ServerCmdRef_t _cmdRef,
    le_cfg_IteratorRef_t iteratorRef
);

//--------------------------------------------------------------------------------------------------
/**
 * Server-side respond function for le_cfg_GoToFirstChild
 */
//--------------------------------------------------------------------------------------------------
void le_cfg_GoToFirstChildRespond
(
    le_cfg_ServerCmdRef_t _cmdRef,
    le_result_t _result
);

//--------------------------------------------------------------------------------------------------
/**
 * Prototype for server-side async interface function
 */
//--------------------------------------------------------------------------------------------------
void le_cfg_GoToFirstChild
(
    le_cfg_ServerCmdRef_t _cmdRef,
    le_cfg_IteratorRef_t iteratorRef
);

//--------------------------------------------------------------------------------------------------
/**
 * Server-side respond function for le_cfg_GoToNextSibling
 */
//--------------------------------------------------------------------------------------------------
void le_cfg_GoToNextSiblingRespond
(
    le_cfg_ServerCmdRef_t _cmdRef,
    le_result_t _result
);

//--------------------------------------------------------------------------------------------------
/**
 * Prototype for server-side async interface function
 */
//--------------------------------------------------------------------------------------------------
void le_cfg_GoToNextSibling
(
    le_cfg_ServerCmdRef_t _cmdRef,
    le_cfg_IteratorRef_t iteratorRef
);

//--------------------------------------------------------------------------------------------------
/**
 * Server-side respond function for le_cfg_GetPath
 */
//--------------------------------------------------------------------------------------------------
void le_cfg_GetPathRespond
(
    le_cfg_ServerCmdRef_t _cmdRef,
    le_result_t _result,
    const char* pathBuffer
);

//--------------------------------------------------------------------------------------------------
/**
 * Prototype for server-side async interface function
 */
//--------------------------------------------------------------------------------------------------
void le_cfg_GetPath
(
    le_cfg_ServerCmdRef_t _cmdRef,
    le_cfg_IteratorRef_t iteratorRef,
    const char* LE_NONNULL path,
    size_t pathBufferSize
);

//--------------------------------------------------------------------------------------------------
/**
 * Server-side respond function for le_cfg_GetNodeType
 */
//--------------------------------------------------------------------------------------------------
void le_cfg_GetNodeTypeRespond
(
    le_cfg_ServerCmdRef_t _cmdRef,
    le_cfg_nodeType_t _result
);

//--------------------------------------------------------------------------------------------------
/**
 * Prototype for server-side async interface function
 */
//--------------------------------------------------------------------------------------------------
void le_cfg_GetNodeType
(
    le_cfg_ServerCmdRef_t _cmdRef,
    le_cfg_IteratorRef_t iteratorRef,
    const char* LE_NONNULL path
);

//--------------------------------------------------------------------------------------------------
/**
 * Server-side respond function for le_cfg_GetNodeName
 */
//--------------------------------------------------------------------------------------------------
void le_cfg_GetNodeNameRespond
(
    le_cfg_ServerCmdRef_t _cmdRef,
    le_result_t _result,
    const char* name
);

//--------------------------------------------------------------------------------------------------
/**
 * Prototype for server-side async interface function
 */
//--------------------------------------------------------------------------------------------------
void le_cfg_GetNodeName
(
    le_cfg_ServerCmdRef_t _cmdRef,
    le_cfg_IteratorRef_t iteratorRef,
    const char* LE_NONNULL path,
    size_t nameSize
);



//--------------------------------------------------------------------------------------------------
/**
 * Add handler function for EVENT 'le_cfg_Change'
 *
 * This event provides information on changes to the given node object, or any of it's children,
 * where a change could be either a read, write, create or delete operation.
 */
//--------------------------------------------------------------------------------------------------
le_cfg_ChangeHandlerRef_t le_cfg_AddChangeHandler
(
    const char* LE_NONNULL newPath,
        ///< [IN] Path to the object to watch.
    le_cfg_ChangeHandlerFunc_t handlerPtr,
        ///< [IN] Handler to receive change notification
    void* contextPtr
        ///< [IN]
);



//--------------------------------------------------------------------------------------------------
/**
 * Remove handler function for EVENT 'le_cfg_Change'
 */
//--------------------------------------------------------------------------------------------------
void le_cfg_RemoveChangeHandler
(
    le_cfg_ChangeHandlerRef_t handlerRef
        ///< [IN]
);

//--------------------------------------------------------------------------------------------------
/**
 * Server-side respond function for le_cfg_DeleteNode
 */
//--------------------------------------------------------------------------------------------------
void le_cfg_DeleteNodeRespond
(
    le_cfg_ServerCmdRef_t _cmdRef
);

//--------------------------------------------------------------------------------------------------
/**
 * Prototype for server-side async interface function
 */
//--------------------------------------------------------------------------------------------------
void le_cfg_DeleteNode
(
    le_cfg_ServerCmdRef_t _cmdRef,
    le_cfg_IteratorRef_t iteratorRef,
    const char* LE_NONNULL path
);

//--------------------------------------------------------------------------------------------------
/**
 * Server-side respond function for le_cfg_IsEmpty
 */
//--------------------------------------------------------------------------------------------------
void le_cfg_IsEmptyRespond
(
    le_cfg_ServerCmdRef_t _cmdRef,
    bool _result
);

//--------------------------------------------------------------------------------------------------
/**
 * Prototype for server-side async interface function
 */
//--------------------------------------------------------------------------------------------------
void le_cfg_IsEmpty
(
    le_cfg_ServerCmdRef_t _cmdRef,
    le_cfg_IteratorRef_t iteratorRef,
    const char* LE_NONNULL path
);

//--------------------------------------------------------------------------------------------------
/**
 * Server-side respond function for le_cfg_SetEmpty
 */
//--------------------------------------------------------------------------------------------------
void le_cfg_SetEmptyRespond
(
    le_cfg_ServerCmdRef_t _cmdRef
);

//--------------------------------------------------------------------------------------------------
/**
 * Prototype for server-side async interface function
 */
//--------------------------------------------------------------------------------------------------
void le_cfg_SetEmpty
(
    le_cfg_ServerCmdRef_t _cmdRef,
    le_cfg_IteratorRef_t iteratorRef,
    const char* LE_NONNULL path
);

//--------------------------------------------------------------------------------------------------
/**
 * Server-side respond function for le_cfg_NodeExists
 */
//--------------------------------------------------------------------------------------------------
void le_cfg_NodeExistsRespond
(
    le_cfg_ServerCmdRef_t _cmdRef,
    bool _result
);

//--------------------------------------------------------------------------------------------------
/**
 * Prototype for server-side async interface function
 */
//--------------------------------------------------------------------------------------------------
void le_cfg_NodeExists
(
    le_cfg_ServerCmdRef_t _cmdRef,
    le_cfg_IteratorRef_t iteratorRef,
    const char* LE_NONNULL path
);

//--------------------------------------------------------------------------------------------------
/**
 * Server-side respond function for le_cfg_GetString
 */
//--------------------------------------------------------------------------------------------------
void le_cfg_GetStringRespond
(
    le_cfg_ServerCmdRef_t _cmdRef,
    le_result_t _result,
    const char* value
);

//--------------------------------------------------------------------------------------------------
/**
 * Prototype for server-side async interface function
 */
//--------------------------------------------------------------------------------------------------
void le_cfg_GetString
(
    le_cfg_ServerCmdRef_t _cmdRef,
    le_cfg_IteratorRef_t iteratorRef,
    const char* LE_NONNULL path,
    size_t valueSize,
    const char* LE_NONNULL defaultValue
);

//--------------------------------------------------------------------------------------------------
/**
 * Server-side respond function for le_cfg_SetString
 */
//--------------------------------------------------------------------------------------------------
void le_cfg_SetStringRespond
(
    le_cfg_ServerCmdRef_t _cmdRef
);

//--------------------------------------------------------------------------------------------------
/**
 * Prototype for server-side async interface function
 */
//--------------------------------------------------------------------------------------------------
void le_cfg_SetString
(
    le_cfg_ServerCmdRef_t _cmdRef,
    le_cfg_IteratorRef_t iteratorRef,
    const char* LE_NONNULL path,
    const char* LE_NONNULL value
);

//--------------------------------------------------------------------------------------------------
/**
 * Server-side respond function for le_cfg_GetInt
 */
//--------------------------------------------------------------------------------------------------
void le_cfg_GetIntRespond
(
    le_cfg_ServerCmdRef_t _cmdRef,
    int32_t _result
);

//--------------------------------------------------------------------------------------------------
/**
 * Prototype for server-side async interface function
 */
//--------------------------------------------------------------------------------------------------
void le_cfg_GetInt
(
    le_cfg_ServerCmdRef_t _cmdRef,
    le_cfg_IteratorRef_t iteratorRef,
    const char* LE_NONNULL path,
    int32_t defaultValue
);

//--------------------------------------------------------------------------------------------------
/**
 * Server-side respond function for le_cfg_SetInt
 */
//--------------------------------------------------------------------------------------------------
void le_cfg_SetIntRespond
(
    le_cfg_ServerCmdRef_t _cmdRef
);

//--------------------------------------------------------------------------------------------------
/**
 * Prototype for server-side async interface function
 */
//--------------------------------------------------------------------------------------------------
void le_cfg_SetInt
(
    le_cfg_ServerCmdRef_t _cmdRef,
    le_cfg_IteratorRef_t iteratorRef,
    const char* LE_NONNULL path,
    int32_t value
);

//--------------------------------------------------------------------------------------------------
/**
 * Server-side respond function for le_cfg_GetFloat
 */
//--------------------------------------------------------------------------------------------------
void le_cfg_GetFloatRespond
(
    le_cfg_ServerCmdRef_t _cmdRef,
    double _result
);

//--------------------------------------------------------------------------------------------------
/**
 * Prototype for server-side async interface function
 */
//--------------------------------------------------------------------------------------------------
void le_cfg_GetFloat
(
    le_cfg_ServerCmdRef_t _cmdRef,
    le_cfg_IteratorRef_t iteratorRef,
    const char* LE_NONNULL path,
    double defaultValue
);

//--------------------------------------------------------------------------------------------------
/**
 * Server-side respond function for le_cfg_SetFloat
 */
//--------------------------------------------------------------------------------------------------
void le_cfg_SetFloatRespond
(
    le_cfg_ServerCmdRef_t _cmdRef
);

//--------------------------------------------------------------------------------------------------
/**
 * Prototype for server-side async interface function
 */
//--------------------------------------------------------------------------------------------------
void le_cfg_SetFloat
(
    le_cfg_ServerCmdRef_t _cmdRef,
    le_cfg_IteratorRef_t iteratorRef,
    const char* LE_NONNULL path,
    double value
);

//--------------------------------------------------------------------------------------------------
/**
 * Server-side respond function for le_cfg_GetBool
 */
//--------------------------------------------------------------------------------------------------
void le_cfg_GetBoolRespond
(
    le_cfg_ServerCmdRef_t _cmdRef,
    bool _result
);

//--------------------------------------------------------------------------------------------------
/**
 * Prototype for server-side async interface function
 */
//--------------------------------------------------------------------------------------------------
void le_cfg_GetBool
(
    le_cfg_ServerCmdRef_t _cmdRef,
    le_cfg_IteratorRef_t iteratorRef,
    const char* LE_NONNULL path,
    bool defaultValue
);

//--------------------------------------------------------------------------------------------------
/**
 * Server-side respond function for le_cfg_SetBool
 */
//--------------------------------------------------------------------------------------------------
void le_cfg_SetBoolRespond
(
    le_cfg_ServerCmdRef_t _cmdRef
);

//--------------------------------------------------------------------------------------------------
/**
 * Prototype for server-side async interface function
 */
//--------------------------------------------------------------------------------------------------
void le_cfg_SetBool
(
    le_cfg_ServerCmdRef_t _cmdRef,
    le_cfg_IteratorRef_t iteratorRef,
    const char* LE_NONNULL path,
    bool value
);

//--------------------------------------------------------------------------------------------------
/**
 * Server-side respond function for le_cfg_QuickDeleteNode
 */
//--------------------------------------------------------------------------------------------------
void le_cfg_QuickDeleteNodeRespond
(
    le_cfg_ServerCmdRef_t _cmdRef
);

//--------------------------------------------------------------------------------------------------
/**
 * Prototype for server-side async interface function
 */
//--------------------------------------------------------------------------------------------------
void le_cfg_QuickDeleteNode
(
    le_cfg_ServerCmdRef_t _cmdRef,
    const char* LE_NONNULL path
);

//--------------------------------------------------------------------------------------------------
/**
 * Server-side respond function for le_cfg_QuickSetEmpty
 */
//--------------------------------------------------------------------------------------------------
void le_cfg_QuickSetEmptyRespond
(
    le_cfg_ServerCmdRef_t _cmdRef
);

//--------------------------------------------------------------------------------------------------
/**
 * Prototype for server-side async interface function
 */
//--------------------------------------------------------------------------------------------------
void le_cfg_QuickSetEmpty
(
    le_cfg_ServerCmdRef_t _cmdRef,
    const char* LE_NONNULL path
);

//--------------------------------------------------------------------------------------------------
/**
 * Server-side respond function for le_cfg_QuickGetString
 */
//--------------------------------------------------------------------------------------------------
void le_cfg_QuickGetStringRespond
(
    le_cfg_ServerCmdRef_t _cmdRef,
    le_result_t _result,
    const char* value
);

//--------------------------------------------------------------------------------------------------
/**
 * Prototype for server-side async interface function
 */
//--------------------------------------------------------------------------------------------------
void le_cfg_QuickGetString
(
    le_cfg_ServerCmdRef_t _cmdRef,
    const char* LE_NONNULL path,
    size_t valueSize,
    const char* LE_NONNULL defaultValue
);

//--------------------------------------------------------------------------------------------------
/**
 * Server-side respond function for le_cfg_QuickSetString
 */
//--------------------------------------------------------------------------------------------------
void le_cfg_QuickSetStringRespond
(
    le_cfg_ServerCmdRef_t _cmdRef
);

//--------------------------------------------------------------------------------------------------
/**
 * Prototype for server-side async interface function
 */
//--------------------------------------------------------------------------------------------------
void le_cfg_QuickSetString
(
    le_cfg_ServerCmdRef_t _cmdRef,
    const char* LE_NONNULL path,
    const char* LE_NONNULL value
);

//--------------------------------------------------------------------------------------------------
/**
 * Server-side respond function for le_cfg_QuickGetInt
 */
//--------------------------------------------------------------------------------------------------
void le_cfg_QuickGetIntRespond
(
    le_cfg_ServerCmdRef_t _cmdRef,
    int32_t _result
);

//--------------------------------------------------------------------------------------------------
/**
 * Prototype for server-side async interface function
 */
//--------------------------------------------------------------------------------------------------
void le_cfg_QuickGetInt
(
    le_cfg_ServerCmdRef_t _cmdRef,
    const char* LE_NONNULL path,
    int32_t defaultValue
);

//--------------------------------------------------------------------------------------------------
/**
 * Server-side respond function for le_cfg_QuickSetInt
 */
//--------------------------------------------------------------------------------------------------
void le_cfg_QuickSetIntRespond
(
    le_cfg_ServerCmdRef_t _cmdRef
);

//--------------------------------------------------------------------------------------------------
/**
 * Prototype for server-side async interface function
 */
//--------------------------------------------------------------------------------------------------
void le_cfg_QuickSetInt
(
    le_cfg_ServerCmdRef_t _cmdRef,
    const char* LE_NONNULL path,
    int32_t value
);

//--------------------------------------------------------------------------------------------------
/**
 * Server-side respond function for le_cfg_QuickGetFloat
 */
//--------------------------------------------------------------------------------------------------
void le_cfg_QuickGetFloatRespond
(
    le_cfg_ServerCmdRef_t _cmdRef,
    double _result
);

//--------------------------------------------------------------------------------------------------
/**
 * Prototype for server-side async interface function
 */
//--------------------------------------------------------------------------------------------------
void le_cfg_QuickGetFloat
(
    le_cfg_ServerCmdRef_t _cmdRef,
    const char* LE_NONNULL path,
    double defaultValue
);

//--------------------------------------------------------------------------------------------------
/**
 * Server-side respond function for le_cfg_QuickSetFloat
 */
//--------------------------------------------------------------------------------------------------
void le_cfg_QuickSetFloatRespond
(
    le_cfg_ServerCmdRef_t _cmdRef
);

//--------------------------------------------------------------------------------------------------
/**
 * Prototype for server-side async interface function
 */
//--------------------------------------------------------------------------------------------------
void le_cfg_QuickSetFloat
(
    le_cfg_ServerCmdRef_t _cmdRef,
    const char* LE_NONNULL path,
    double value
);

//--------------------------------------------------------------------------------------------------
/**
 * Server-side respond function for le_cfg_QuickGetBool
 */
//--------------------------------------------------------------------------------------------------
void le_cfg_QuickGetBoolRespond
(
    le_cfg_ServerCmdRef_t _cmdRef,
    bool _result
);

//--------------------------------------------------------------------------------------------------
/**
 * Prototype for server-side async interface function
 */
//--------------------------------------------------------------------------------------------------
void le_cfg_QuickGetBool
(
    le_cfg_ServerCmdRef_t _cmdRef,
    const char* LE_NONNULL path,
    bool defaultValue
);

//--------------------------------------------------------------------------------------------------
/**
 * Server-side respond function for le_cfg_QuickSetBool
 */
//--------------------------------------------------------------------------------------------------
void le_cfg_QuickSetBoolRespond
(
    le_cfg_ServerCmdRef_t _cmdRef
);

//--------------------------------------------------------------------------------------------------
/**
 * Prototype for server-side async interface function
 */
//--------------------------------------------------------------------------------------------------
void le_cfg_QuickSetBool
(
    le_cfg_ServerCmdRef_t _cmdRef,
    const char* LE_NONNULL path,
    bool value
);


#endif // LE_CFG_INTERFACE_H_INCLUDE_GUARD