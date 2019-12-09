
/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */


#ifndef LE_UPDATECTRL_MESSAGES_H_INCLUDE_GUARD
#define LE_UPDATECTRL_MESSAGES_H_INCLUDE_GUARD


#include "legato.h"

#define PROTOCOL_ID_STR "2068715d5da737c096cc7146ce5f1df6"

#ifdef MK_TOOLS_BUILD
    extern const char** le_updateCtrl_ServiceInstanceNamePtr;
    #define SERVICE_INSTANCE_NAME (*le_updateCtrl_ServiceInstanceNamePtr)
#else
    #define SERVICE_INSTANCE_NAME "le_updateCtrl"
#endif


#define _MAX_MSG_SIZE 13

// Define the message type for communicating between client and server
typedef struct __attribute__((packed))
{
    uint32_t id;
    uint8_t buffer[_MAX_MSG_SIZE];
}
_Message_t;

#define _MSGID_le_updateCtrl_Defer 0
#define _MSGID_le_updateCtrl_Allow 1
#define _MSGID_le_updateCtrl_LockProbation 2
#define _MSGID_le_updateCtrl_UnlockProbation 3
#define _MSGID_le_updateCtrl_MarkGood 4
#define _MSGID_le_updateCtrl_FailProbation 5
#define _MSGID_le_updateCtrl_GetSystemState 6


#endif // LE_UPDATECTRL_MESSAGES_H_INCLUDE_GUARD