
/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */


#ifndef LE_IMA_MESSAGES_H_INCLUDE_GUARD
#define LE_IMA_MESSAGES_H_INCLUDE_GUARD


#include "legato.h"

#define PROTOCOL_ID_STR "32b128a43637f60e028aba17f6513daa"

#ifdef MK_TOOLS_BUILD
    extern const char** le_ima_ServiceInstanceNamePtr;
    #define SERVICE_INSTANCE_NAME (*le_ima_ServiceInstanceNamePtr)
#else
    #define SERVICE_INSTANCE_NAME "le_ima"
#endif


#define _MAX_MSG_SIZE 527

// Define the message type for communicating between client and server
typedef struct __attribute__((packed))
{
    uint32_t id;
    uint8_t buffer[_MAX_MSG_SIZE];
}
_Message_t;

#define _MSGID_le_ima_ImportCert 0


#endif // LE_IMA_MESSAGES_H_INCLUDE_GUARD