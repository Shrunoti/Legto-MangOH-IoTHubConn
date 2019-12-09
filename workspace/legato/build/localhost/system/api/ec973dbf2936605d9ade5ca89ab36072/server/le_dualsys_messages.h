
/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */


#ifndef LE_DUALSYS_MESSAGES_H_INCLUDE_GUARD
#define LE_DUALSYS_MESSAGES_H_INCLUDE_GUARD


#include "legato.h"

#define PROTOCOL_ID_STR "69539775d6f1358ade33034c2fa222c4"

#ifdef MK_TOOLS_BUILD
    extern const char** le_dualsys_ServiceInstanceNamePtr;
    #define SERVICE_INSTANCE_NAME (*le_dualsys_ServiceInstanceNamePtr)
#else
    #define SERVICE_INSTANCE_NAME "le_dualsys"
#endif


#define _MAX_MSG_SIZE 16

// Define the message type for communicating between client and server
typedef struct __attribute__((packed))
{
    uint32_t id;
    uint8_t buffer[_MAX_MSG_SIZE];
}
_Message_t;

#define _MSGID_le_dualsys_DisableSyncBeforeUpdate 0
#define _MSGID_le_dualsys_SetSystem 1
#define _MSGID_le_dualsys_GetCurrentSystem 2


#endif // LE_DUALSYS_MESSAGES_H_INCLUDE_GUARD