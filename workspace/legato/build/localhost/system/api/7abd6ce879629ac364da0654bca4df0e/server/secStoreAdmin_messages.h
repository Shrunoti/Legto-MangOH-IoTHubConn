
/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */


#ifndef SECSTOREADMIN_MESSAGES_H_INCLUDE_GUARD
#define SECSTOREADMIN_MESSAGES_H_INCLUDE_GUARD


#include "legato.h"

#define PROTOCOL_ID_STR "4995e42beb8aa21c2fabf4414840024d"

#ifdef MK_TOOLS_BUILD
    extern const char** secStoreAdmin_ServiceInstanceNamePtr;
    #define SERVICE_INSTANCE_NAME (*secStoreAdmin_ServiceInstanceNamePtr)
#else
    #define SERVICE_INSTANCE_NAME "secStoreAdmin"
#endif


#define _MAX_MSG_SIZE 8723

// Define the message type for communicating between client and server
typedef struct __attribute__((packed))
{
    uint32_t id;
    uint8_t buffer[_MAX_MSG_SIZE];
}
_Message_t;

#define _MSGID_secStoreAdmin_CreateIter 0
#define _MSGID_secStoreAdmin_DeleteIter 1
#define _MSGID_secStoreAdmin_Next 2
#define _MSGID_secStoreAdmin_GetEntry 3
#define _MSGID_secStoreAdmin_Write 4
#define _MSGID_secStoreAdmin_Read 5
#define _MSGID_secStoreAdmin_CopyMetaTo 6
#define _MSGID_secStoreAdmin_Delete 7
#define _MSGID_secStoreAdmin_GetSize 8
#define _MSGID_secStoreAdmin_GetTotalSpace 9


#endif // SECSTOREADMIN_MESSAGES_H_INCLUDE_GUARD