
/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */


#ifndef LE_SMSINBOX1_MESSAGES_H_INCLUDE_GUARD
#define LE_SMSINBOX1_MESSAGES_H_INCLUDE_GUARD


#include "legato.h"

#define PROTOCOL_ID_STR "4b0ed65a271c653dec4c0487697753be"

#ifdef MK_TOOLS_BUILD
    extern const char** le_smsInbox1_ServiceInstanceNamePtr;
    #define SERVICE_INSTANCE_NAME (*le_smsInbox1_ServiceInstanceNamePtr)
#else
    #define SERVICE_INSTANCE_NAME "le_smsInbox1"
#endif


#define _MAX_MSG_SIZE 196

// Define the message type for communicating between client and server
typedef struct __attribute__((packed))
{
    uint32_t id;
    uint8_t buffer[_MAX_MSG_SIZE];
}
_Message_t;

#define _MSGID_le_smsInbox1_Open 0
#define _MSGID_le_smsInbox1_Close 1
#define _MSGID_le_smsInbox1_AddRxMessageHandler 2
#define _MSGID_le_smsInbox1_RemoveRxMessageHandler 3
#define _MSGID_le_smsInbox1_DeleteMsg 4
#define _MSGID_le_smsInbox1_GetImsi 5
#define _MSGID_le_smsInbox1_GetFormat 6
#define _MSGID_le_smsInbox1_GetSenderTel 7
#define _MSGID_le_smsInbox1_GetTimeStamp 8
#define _MSGID_le_smsInbox1_GetMsgLen 9
#define _MSGID_le_smsInbox1_GetText 10
#define _MSGID_le_smsInbox1_GetBinary 11
#define _MSGID_le_smsInbox1_GetPdu 12
#define _MSGID_le_smsInbox1_GetFirst 13
#define _MSGID_le_smsInbox1_GetNext 14
#define _MSGID_le_smsInbox1_IsUnread 15
#define _MSGID_le_smsInbox1_MarkRead 16
#define _MSGID_le_smsInbox1_MarkUnread 17
#define _MSGID_le_smsInbox1_SetMaxMessages 18
#define _MSGID_le_smsInbox1_GetMaxMessages 19


#endif // LE_SMSINBOX1_MESSAGES_H_INCLUDE_GUARD