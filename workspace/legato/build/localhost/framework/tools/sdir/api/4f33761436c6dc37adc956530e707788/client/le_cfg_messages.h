
/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */


#ifndef LE_CFG_MESSAGES_H_INCLUDE_GUARD
#define LE_CFG_MESSAGES_H_INCLUDE_GUARD


#include "legato.h"

#define PROTOCOL_ID_STR "26b40ad3a107d48e4227dff8903b9601"

#ifdef MK_TOOLS_BUILD
    extern const char** le_cfg_ServiceInstanceNamePtr;
    #define SERVICE_INSTANCE_NAME (*le_cfg_ServiceInstanceNamePtr)
#else
    #define SERVICE_INSTANCE_NAME "le_cfg"
#endif


#define _MAX_MSG_SIZE 1561

// Define the message type for communicating between client and server
typedef struct __attribute__((packed))
{
    uint32_t id;
    uint8_t buffer[_MAX_MSG_SIZE];
}
_Message_t;

#define _MSGID_le_cfg_CreateReadTxn 0
#define _MSGID_le_cfg_CreateWriteTxn 1
#define _MSGID_le_cfg_CommitTxn 2
#define _MSGID_le_cfg_CancelTxn 3
#define _MSGID_le_cfg_GoToNode 4
#define _MSGID_le_cfg_GoToParent 5
#define _MSGID_le_cfg_GoToFirstChild 6
#define _MSGID_le_cfg_GoToNextSibling 7
#define _MSGID_le_cfg_GetPath 8
#define _MSGID_le_cfg_GetNodeType 9
#define _MSGID_le_cfg_GetNodeName 10
#define _MSGID_le_cfg_AddChangeHandler 11
#define _MSGID_le_cfg_RemoveChangeHandler 12
#define _MSGID_le_cfg_DeleteNode 13
#define _MSGID_le_cfg_IsEmpty 14
#define _MSGID_le_cfg_SetEmpty 15
#define _MSGID_le_cfg_NodeExists 16
#define _MSGID_le_cfg_GetString 17
#define _MSGID_le_cfg_SetString 18
#define _MSGID_le_cfg_GetInt 19
#define _MSGID_le_cfg_SetInt 20
#define _MSGID_le_cfg_GetFloat 21
#define _MSGID_le_cfg_SetFloat 22
#define _MSGID_le_cfg_GetBool 23
#define _MSGID_le_cfg_SetBool 24
#define _MSGID_le_cfg_QuickDeleteNode 25
#define _MSGID_le_cfg_QuickSetEmpty 26
#define _MSGID_le_cfg_QuickGetString 27
#define _MSGID_le_cfg_QuickSetString 28
#define _MSGID_le_cfg_QuickGetInt 29
#define _MSGID_le_cfg_QuickSetInt 30
#define _MSGID_le_cfg_QuickGetFloat 31
#define _MSGID_le_cfg_QuickSetFloat 32
#define _MSGID_le_cfg_QuickGetBool 33
#define _MSGID_le_cfg_QuickSetBool 34


#endif // LE_CFG_MESSAGES_H_INCLUDE_GUARD