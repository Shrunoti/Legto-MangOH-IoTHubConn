
/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */


#ifndef LE_BOOTREASON_MESSAGES_H_INCLUDE_GUARD
#define LE_BOOTREASON_MESSAGES_H_INCLUDE_GUARD


#include "legato.h"

#define PROTOCOL_ID_STR "2d79668511adea4d675f5d77b9f2d3ea"

#ifdef MK_TOOLS_BUILD
    extern const char** le_bootReason_ServiceInstanceNamePtr;
    #define SERVICE_INSTANCE_NAME (*le_bootReason_ServiceInstanceNamePtr)
#else
    #define SERVICE_INSTANCE_NAME "le_bootReason"
#endif


#define _MAX_MSG_SIZE 13

// Define the message type for communicating between client and server
typedef struct __attribute__((packed))
{
    uint32_t id;
    uint8_t buffer[_MAX_MSG_SIZE];
}
_Message_t;

#define _MSGID_le_bootReason_WasTimer 0
#define _MSGID_le_bootReason_WasGpio 1
#define _MSGID_le_bootReason_WasAdc 2


#endif // LE_BOOTREASON_MESSAGES_H_INCLUDE_GUARD