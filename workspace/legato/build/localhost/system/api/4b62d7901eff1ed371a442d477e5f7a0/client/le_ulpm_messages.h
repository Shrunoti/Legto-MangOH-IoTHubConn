
/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */


#ifndef LE_ULPM_MESSAGES_H_INCLUDE_GUARD
#define LE_ULPM_MESSAGES_H_INCLUDE_GUARD


#include "legato.h"

#define PROTOCOL_ID_STR "fdb586a476e5e0e19b60614aa1ea5d7a"

#ifdef MK_TOOLS_BUILD
    extern const char** le_ulpm_ServiceInstanceNamePtr;
    #define SERVICE_INSTANCE_NAME (*le_ulpm_ServiceInstanceNamePtr)
#else
    #define SERVICE_INSTANCE_NAME "le_ulpm"
#endif


#define _MAX_MSG_SIZE 36

// Define the message type for communicating between client and server
typedef struct __attribute__((packed))
{
    uint32_t id;
    uint8_t buffer[_MAX_MSG_SIZE];
}
_Message_t;

#define _MSGID_le_ulpm_BootOnGpio 0
#define _MSGID_le_ulpm_BootOnTimer 1
#define _MSGID_le_ulpm_BootOnAdc 2
#define _MSGID_le_ulpm_GetFirmwareVersion 3
#define _MSGID_le_ulpm_ShutDown 4


#endif // LE_ULPM_MESSAGES_H_INCLUDE_GUARD