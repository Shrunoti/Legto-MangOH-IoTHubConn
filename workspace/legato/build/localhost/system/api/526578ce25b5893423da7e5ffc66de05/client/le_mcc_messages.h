
/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */


#ifndef LE_MCC_MESSAGES_H_INCLUDE_GUARD
#define LE_MCC_MESSAGES_H_INCLUDE_GUARD


#include "legato.h"

#define PROTOCOL_ID_STR "bf43202a0369b12bb827e1bc4724bb41"

#ifdef MK_TOOLS_BUILD
    extern const char** le_mcc_ServiceInstanceNamePtr;
    #define SERVICE_INSTANCE_NAME (*le_mcc_ServiceInstanceNamePtr)
#else
    #define SERVICE_INSTANCE_NAME "le_mcc"
#endif


#define _MAX_MSG_SIZE 37

// Define the message type for communicating between client and server
typedef struct __attribute__((packed))
{
    uint32_t id;
    uint8_t buffer[_MAX_MSG_SIZE];
}
_Message_t;

#define _MSGID_le_mcc_Create 0
#define _MSGID_le_mcc_Delete 1
#define _MSGID_le_mcc_Start 2
#define _MSGID_le_mcc_IsConnected 3
#define _MSGID_le_mcc_GetRemoteTel 4
#define _MSGID_le_mcc_GetTerminationReason 5
#define _MSGID_le_mcc_GetPlatformSpecificTerminationCode 6
#define _MSGID_le_mcc_Answer 7
#define _MSGID_le_mcc_HangUp 8
#define _MSGID_le_mcc_HangUpAll 9
#define _MSGID_le_mcc_GetCallerIdRestrict 10
#define _MSGID_le_mcc_SetCallerIdRestrict 11
#define _MSGID_le_mcc_AddCallEventHandler 12
#define _MSGID_le_mcc_RemoveCallEventHandler 13
#define _MSGID_le_mcc_SetCallWaitingService 14
#define _MSGID_le_mcc_GetCallWaitingService 15
#define _MSGID_le_mcc_ActivateCall 16
#define _MSGID_le_mcc_SetAmrWbCapability 17
#define _MSGID_le_mcc_GetAmrWbCapability 18


#endif // LE_MCC_MESSAGES_H_INCLUDE_GUARD