
/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */


#ifndef LE_SUP_CTRL_MESSAGES_H_INCLUDE_GUARD
#define LE_SUP_CTRL_MESSAGES_H_INCLUDE_GUARD


#include "legato.h"

#define PROTOCOL_ID_STR "bb17ec49ba32b1f1cfa05c1fa0000719"

#ifdef MK_TOOLS_BUILD
    extern const char** le_sup_ctrl_ServiceInstanceNamePtr;
    #define SERVICE_INSTANCE_NAME (*le_sup_ctrl_ServiceInstanceNamePtr)
#else
    #define SERVICE_INSTANCE_NAME "le_sup_ctrl"
#endif


#define _MAX_MSG_SIZE 64

// Define the message type for communicating between client and server
typedef struct __attribute__((packed))
{
    uint32_t id;
    uint8_t buffer[_MAX_MSG_SIZE];
}
_Message_t;

#define _MSGID_le_sup_ctrl_GetAppRef 0
#define _MSGID_le_sup_ctrl_ReleaseAppRef 1
#define _MSGID_le_sup_ctrl_SetRun 2
#define _MSGID_le_sup_ctrl_StartApp 3
#define _MSGID_le_sup_ctrl_StopApp 4
#define _MSGID_le_sup_ctrl_StopLegato 5
#define _MSGID_le_sup_ctrl_RestartLegato 6


#endif // LE_SUP_CTRL_MESSAGES_H_INCLUDE_GUARD