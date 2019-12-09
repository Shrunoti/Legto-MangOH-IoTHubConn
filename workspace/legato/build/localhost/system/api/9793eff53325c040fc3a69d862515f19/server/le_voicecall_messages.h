
/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */


#ifndef LE_VOICECALL_MESSAGES_H_INCLUDE_GUARD
#define LE_VOICECALL_MESSAGES_H_INCLUDE_GUARD


#include "legato.h"

#define PROTOCOL_ID_STR "181e5028027647a1335449aaac136e5c"

#ifdef MK_TOOLS_BUILD
    extern const char** le_voicecall_ServiceInstanceNamePtr;
    #define SERVICE_INSTANCE_NAME (*le_voicecall_ServiceInstanceNamePtr)
#else
    #define SERVICE_INSTANCE_NAME "le_voicecall"
#endif


#define _MAX_MSG_SIZE 37

// Define the message type for communicating between client and server
typedef struct __attribute__((packed))
{
    uint32_t id;
    uint8_t buffer[_MAX_MSG_SIZE];
}
_Message_t;

#define _MSGID_le_voicecall_AddStateHandler 0
#define _MSGID_le_voicecall_RemoveStateHandler 1
#define _MSGID_le_voicecall_Start 2
#define _MSGID_le_voicecall_End 3
#define _MSGID_le_voicecall_Delete 4
#define _MSGID_le_voicecall_Answer 5
#define _MSGID_le_voicecall_GetTerminationReason 6
#define _MSGID_le_voicecall_GetTxAudioStream 7
#define _MSGID_le_voicecall_GetRxAudioStream 8


#endif // LE_VOICECALL_MESSAGES_H_INCLUDE_GUARD