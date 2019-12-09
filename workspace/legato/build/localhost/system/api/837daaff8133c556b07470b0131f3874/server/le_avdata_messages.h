
/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */


#ifndef LE_AVDATA_MESSAGES_H_INCLUDE_GUARD
#define LE_AVDATA_MESSAGES_H_INCLUDE_GUARD


#include "legato.h"

#define PROTOCOL_ID_STR "d042ee9f795ba3ff64aacf91d970b8fc"

#ifdef MK_TOOLS_BUILD
    extern const char** le_avdata_ServiceInstanceNamePtr;
    #define SERVICE_INSTANCE_NAME (*le_avdata_ServiceInstanceNamePtr)
#else
    #define SERVICE_INSTANCE_NAME "le_avdata"
#endif


#define _MAX_MSG_SIZE 798

// Define the message type for communicating between client and server
typedef struct __attribute__((packed))
{
    uint32_t id;
    uint8_t buffer[_MAX_MSG_SIZE];
}
_Message_t;

#define _MSGID_le_avdata_AddResourceEventHandler 0
#define _MSGID_le_avdata_RemoveResourceEventHandler 1
#define _MSGID_le_avdata_CreateResource 2
#define _MSGID_le_avdata_SetNamespace 3
#define _MSGID_le_avdata_SetNull 4
#define _MSGID_le_avdata_GetInt 5
#define _MSGID_le_avdata_SetInt 6
#define _MSGID_le_avdata_GetFloat 7
#define _MSGID_le_avdata_SetFloat 8
#define _MSGID_le_avdata_GetBool 9
#define _MSGID_le_avdata_SetBool 10
#define _MSGID_le_avdata_GetString 11
#define _MSGID_le_avdata_SetString 12
#define _MSGID_le_avdata_GetBoolArg 13
#define _MSGID_le_avdata_GetFloatArg 14
#define _MSGID_le_avdata_GetIntArg 15
#define _MSGID_le_avdata_GetStringArg 16
#define _MSGID_le_avdata_GetStringArgLength 17
#define _MSGID_le_avdata_ReplyExecResult 18
#define _MSGID_le_avdata_Push 19
#define _MSGID_le_avdata_PushStream 20
#define _MSGID_le_avdata_CreateRecord 21
#define _MSGID_le_avdata_DeleteRecord 22
#define _MSGID_le_avdata_RecordInt 23
#define _MSGID_le_avdata_RecordFloat 24
#define _MSGID_le_avdata_RecordBool 25
#define _MSGID_le_avdata_RecordString 26
#define _MSGID_le_avdata_PushRecord 27
#define _MSGID_le_avdata_AddSessionStateHandler 28
#define _MSGID_le_avdata_RemoveSessionStateHandler 29
#define _MSGID_le_avdata_RequestSession 30
#define _MSGID_le_avdata_ReleaseSession 31


#endif // LE_AVDATA_MESSAGES_H_INCLUDE_GUARD