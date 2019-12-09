
/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */


#ifndef LE_APPREMOVE_MESSAGES_H_INCLUDE_GUARD
#define LE_APPREMOVE_MESSAGES_H_INCLUDE_GUARD


#include "legato.h"

#define PROTOCOL_ID_STR "13c5300b39cc08587796fd4b47183f92"

#ifdef MK_TOOLS_BUILD
    extern const char** le_appRemove_ServiceInstanceNamePtr;
    #define SERVICE_INSTANCE_NAME (*le_appRemove_ServiceInstanceNamePtr)
#else
    #define SERVICE_INSTANCE_NAME "le_appRemove"
#endif


#define _MAX_MSG_SIZE 63

// Define the message type for communicating between client and server
typedef struct __attribute__((packed))
{
    uint32_t id;
    uint8_t buffer[_MAX_MSG_SIZE];
}
_Message_t;

#define _MSGID_le_appRemove_Remove 0


#endif // LE_APPREMOVE_MESSAGES_H_INCLUDE_GUARD