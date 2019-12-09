
/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */


#ifndef LE_CFGADMIN_MESSAGES_H_INCLUDE_GUARD
#define LE_CFGADMIN_MESSAGES_H_INCLUDE_GUARD


#include "legato.h"

#define PROTOCOL_ID_STR "2aac298e89c905c985fd3b9437fb9f53"

#ifdef MK_TOOLS_BUILD
    extern const char** le_cfgAdmin_ServiceInstanceNamePtr;
    #define SERVICE_INSTANCE_NAME (*le_cfgAdmin_ServiceInstanceNamePtr)
#else
    #define SERVICE_INSTANCE_NAME "le_cfgAdmin"
#endif


#define _MAX_MSG_SIZE 1048

// Define the message type for communicating between client and server
typedef struct __attribute__((packed))
{
    uint32_t id;
    uint8_t buffer[_MAX_MSG_SIZE];
}
_Message_t;

#define _MSGID_le_cfgAdmin_ImportTree 0
#define _MSGID_le_cfgAdmin_ExportTree 1
#define _MSGID_le_cfgAdmin_DeleteTree 2
#define _MSGID_le_cfgAdmin_CreateTreeIterator 3
#define _MSGID_le_cfgAdmin_ReleaseTreeIterator 4
#define _MSGID_le_cfgAdmin_GetTreeName 5
#define _MSGID_le_cfgAdmin_NextTree 6


#endif // LE_CFGADMIN_MESSAGES_H_INCLUDE_GUARD