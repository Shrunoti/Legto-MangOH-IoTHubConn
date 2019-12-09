
/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */


#ifndef LE_WDOG_MESSAGES_H_INCLUDE_GUARD
#define LE_WDOG_MESSAGES_H_INCLUDE_GUARD


#include "legato.h"

#define PROTOCOL_ID_STR "572ba176b790548eed89b7ba6f9f4b14"

#ifdef MK_TOOLS_BUILD
    extern const char** le_wdog_ServiceInstanceNamePtr;
    #define SERVICE_INSTANCE_NAME (*le_wdog_ServiceInstanceNamePtr)
#else
    #define SERVICE_INSTANCE_NAME "le_wdog"
#endif


#define _MAX_MSG_SIZE 20

// Define the message type for communicating between client and server
typedef struct __attribute__((packed))
{
    uint32_t id;
    uint8_t buffer[_MAX_MSG_SIZE];
}
_Message_t;

#define _MSGID_le_wdog_Kick 0
#define _MSGID_le_wdog_Timeout 1
#define _MSGID_le_wdog_GetWatchdogTimeout 2
#define _MSGID_le_wdog_GetMaxWatchdogTimeout 3


#endif // LE_WDOG_MESSAGES_H_INCLUDE_GUARD