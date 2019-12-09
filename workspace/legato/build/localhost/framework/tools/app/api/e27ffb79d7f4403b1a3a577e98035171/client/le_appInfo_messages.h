
/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */


#ifndef LE_APPINFO_MESSAGES_H_INCLUDE_GUARD
#define LE_APPINFO_MESSAGES_H_INCLUDE_GUARD


#include "legato.h"

#define PROTOCOL_ID_STR "092c2f7994c22cecc1e5a2608e1595b0"

#ifdef MK_TOOLS_BUILD
    extern const char** le_appInfo_ServiceInstanceNamePtr;
    #define SERVICE_INSTANCE_NAME (*le_appInfo_ServiceInstanceNamePtr)
#else
    #define SERVICE_INSTANCE_NAME "le_appInfo"
#endif


#define _MAX_MSG_SIZE 114

// Define the message type for communicating between client and server
typedef struct __attribute__((packed))
{
    uint32_t id;
    uint8_t buffer[_MAX_MSG_SIZE];
}
_Message_t;

#define _MSGID_le_appInfo_GetState 0
#define _MSGID_le_appInfo_GetProcState 1
#define _MSGID_le_appInfo_GetName 2
#define _MSGID_le_appInfo_GetHash 3


#endif // LE_APPINFO_MESSAGES_H_INCLUDE_GUARD