
/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */


#ifndef LE_FRAMEWORK_MESSAGES_H_INCLUDE_GUARD
#define LE_FRAMEWORK_MESSAGES_H_INCLUDE_GUARD


#include "legato.h"

#define PROTOCOL_ID_STR "28525bdc36d66247ae720424d0b28b51"

#ifdef MK_TOOLS_BUILD
    extern const char** le_framework_ServiceInstanceNamePtr;
    #define SERVICE_INSTANCE_NAME (*le_framework_ServiceInstanceNamePtr)
#else
    #define SERVICE_INSTANCE_NAME "le_framework"
#endif


#define _MAX_MSG_SIZE 13

// Define the message type for communicating between client and server
typedef struct __attribute__((packed))
{
    uint32_t id;
    uint8_t buffer[_MAX_MSG_SIZE];
}
_Message_t;

#define _MSGID_le_framework_Stop 0
#define _MSGID_le_framework_Restart 1
#define _MSGID_le_framework_IsStopping 2
#define _MSGID_le_framework_IsReadOnly 3
#define _MSGID_le_framework_NotifyExpectedReboot 4


#endif // LE_FRAMEWORK_MESSAGES_H_INCLUDE_GUARD