
/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */


#ifndef LE_ATCLIENT_MESSAGES_H_INCLUDE_GUARD
#define LE_ATCLIENT_MESSAGES_H_INCLUDE_GUARD


#include "legato.h"

#define PROTOCOL_ID_STR "ef7be0a431448f583f2e7a2ec83a5680"

#ifdef MK_TOOLS_BUILD
    extern const char** le_atClient_ServiceInstanceNamePtr;
    #define SERVICE_INSTANCE_NAME (*le_atClient_ServiceInstanceNamePtr)
#else
    #define SERVICE_INSTANCE_NAME "le_atClient"
#endif


#define _MAX_MSG_SIZE 4115

// Define the message type for communicating between client and server
typedef struct __attribute__((packed))
{
    uint32_t id;
    uint8_t buffer[_MAX_MSG_SIZE];
}
_Message_t;

#define _MSGID_le_atClient_Start 0
#define _MSGID_le_atClient_Stop 1
#define _MSGID_le_atClient_Create 2
#define _MSGID_le_atClient_Delete 3
#define _MSGID_le_atClient_SetCommand 4
#define _MSGID_le_atClient_SetIntermediateResponse 5
#define _MSGID_le_atClient_SetFinalResponse 6
#define _MSGID_le_atClient_SetText 7
#define _MSGID_le_atClient_SetTimeout 8
#define _MSGID_le_atClient_SetDevice 9
#define _MSGID_le_atClient_Send 10
#define _MSGID_le_atClient_GetFirstIntermediateResponse 11
#define _MSGID_le_atClient_GetNextIntermediateResponse 12
#define _MSGID_le_atClient_GetFinalResponse 13
#define _MSGID_le_atClient_SetCommandAndSend 14
#define _MSGID_le_atClient_AddUnsolicitedResponseHandler 15
#define _MSGID_le_atClient_RemoveUnsolicitedResponseHandler 16


#endif // LE_ATCLIENT_MESSAGES_H_INCLUDE_GUARD