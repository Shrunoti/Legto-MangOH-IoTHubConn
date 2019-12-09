
/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */


#ifndef LE_SECSTORE_MESSAGES_H_INCLUDE_GUARD
#define LE_SECSTORE_MESSAGES_H_INCLUDE_GUARD


#include "legato.h"

#define PROTOCOL_ID_STR "98c2a4dd482018a94c642390891be24f"

#ifdef MK_TOOLS_BUILD
    extern const char** le_secStore_ServiceInstanceNamePtr;
    #define SERVICE_INSTANCE_NAME (*le_secStore_ServiceInstanceNamePtr)
#else
    #define SERVICE_INSTANCE_NAME "le_secStore"
#endif


#define _MAX_MSG_SIZE 8467

// Define the message type for communicating between client and server
typedef struct __attribute__((packed))
{
    uint32_t id;
    uint8_t buffer[_MAX_MSG_SIZE];
}
_Message_t;

#define _MSGID_le_secStore_Write 0
#define _MSGID_le_secStore_Read 1
#define _MSGID_le_secStore_Delete 2


#endif // LE_SECSTORE_MESSAGES_H_INCLUDE_GUARD