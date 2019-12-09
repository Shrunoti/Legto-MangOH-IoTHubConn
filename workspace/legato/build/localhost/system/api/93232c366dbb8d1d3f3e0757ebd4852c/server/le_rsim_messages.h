
/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */


#ifndef LE_RSIM_MESSAGES_H_INCLUDE_GUARD
#define LE_RSIM_MESSAGES_H_INCLUDE_GUARD


#include "legato.h"

#define PROTOCOL_ID_STR "b09e1d24fc5dc8036c1bf572dbd40b21"

#ifdef MK_TOOLS_BUILD
    extern const char** le_rsim_ServiceInstanceNamePtr;
    #define SERVICE_INSTANCE_NAME (*le_rsim_ServiceInstanceNamePtr)
#else
    #define SERVICE_INSTANCE_NAME "le_rsim"
#endif


#define _MAX_MSG_SIZE 296

// Define the message type for communicating between client and server
typedef struct __attribute__((packed))
{
    uint32_t id;
    uint8_t buffer[_MAX_MSG_SIZE];
}
_Message_t;

#define _MSGID_le_rsim_AddMessageHandler 0
#define _MSGID_le_rsim_RemoveMessageHandler 1
#define _MSGID_le_rsim_SendMessage 2


#endif // LE_RSIM_MESSAGES_H_INCLUDE_GUARD