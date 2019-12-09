
/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */


#ifndef LE_MDMCFG_MESSAGES_H_INCLUDE_GUARD
#define LE_MDMCFG_MESSAGES_H_INCLUDE_GUARD


#include "legato.h"

#define PROTOCOL_ID_STR "fcf3b5cece06794472fb938f2c621c50"

#ifdef MK_TOOLS_BUILD
    extern const char** le_mdmCfg_ServiceInstanceNamePtr;
    #define SERVICE_INSTANCE_NAME (*le_mdmCfg_ServiceInstanceNamePtr)
#else
    #define SERVICE_INSTANCE_NAME "le_mdmCfg"
#endif


#define _MAX_MSG_SIZE 12

// Define the message type for communicating between client and server
typedef struct __attribute__((packed))
{
    uint32_t id;
    uint8_t buffer[_MAX_MSG_SIZE];
}
_Message_t;

#define _MSGID_le_mdmCfg_StoreCurrentConfiguration 0
#define _MSGID_le_mdmCfg_RestoreSavedConfiguration 1


#endif // LE_MDMCFG_MESSAGES_H_INCLUDE_GUARD