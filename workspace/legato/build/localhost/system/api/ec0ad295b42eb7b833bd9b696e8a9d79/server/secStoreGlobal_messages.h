
/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */


#ifndef SECSTOREGLOBAL_MESSAGES_H_INCLUDE_GUARD
#define SECSTOREGLOBAL_MESSAGES_H_INCLUDE_GUARD


#include "legato.h"

#define PROTOCOL_ID_STR "98c2a4dd482018a94c642390891be24f"

#ifdef MK_TOOLS_BUILD
    extern const char** secStoreGlobal_ServiceInstanceNamePtr;
    #define SERVICE_INSTANCE_NAME (*secStoreGlobal_ServiceInstanceNamePtr)
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

#define _MSGID_secStoreGlobal_Write 0
#define _MSGID_secStoreGlobal_Read 1
#define _MSGID_secStoreGlobal_Delete 2


#endif // SECSTOREGLOBAL_MESSAGES_H_INCLUDE_GUARD