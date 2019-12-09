
/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */


#ifndef LE_UPDATE_MESSAGES_H_INCLUDE_GUARD
#define LE_UPDATE_MESSAGES_H_INCLUDE_GUARD


#include "legato.h"

#define PROTOCOL_ID_STR "17cd65c5c26efb8f22650432980df70f"

#ifdef MK_TOOLS_BUILD
    extern const char** le_update_ServiceInstanceNamePtr;
    #define SERVICE_INSTANCE_NAME (*le_update_ServiceInstanceNamePtr)
#else
    #define SERVICE_INSTANCE_NAME "le_update"
#endif


#define _MAX_MSG_SIZE 52

// Define the message type for communicating between client and server
typedef struct __attribute__((packed))
{
    uint32_t id;
    uint8_t buffer[_MAX_MSG_SIZE];
}
_Message_t;

#define _MSGID_le_update_AddProgressHandler 0
#define _MSGID_le_update_RemoveProgressHandler 1
#define _MSGID_le_update_Start 2
#define _MSGID_le_update_Install 3
#define _MSGID_le_update_End 4
#define _MSGID_le_update_GetErrorCode 5
#define _MSGID_le_update_GetCurrentSysIndex 6
#define _MSGID_le_update_GetSystemHash 7
#define _MSGID_le_update_GetPreviousSystemIndex 8


#endif // LE_UPDATE_MESSAGES_H_INCLUDE_GUARD