
/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */


#ifndef LE_INSTSTAT_MESSAGES_H_INCLUDE_GUARD
#define LE_INSTSTAT_MESSAGES_H_INCLUDE_GUARD


#include "legato.h"

#define PROTOCOL_ID_STR "e160a983e92b4310ae8af3fbeff75a1b"

#ifdef MK_TOOLS_BUILD
    extern const char** le_instStat_ServiceInstanceNamePtr;
    #define SERVICE_INSTANCE_NAME (*le_instStat_ServiceInstanceNamePtr)
#else
    #define SERVICE_INSTANCE_NAME "le_instStat"
#endif


#define _MAX_MSG_SIZE 59

// Define the message type for communicating between client and server
typedef struct __attribute__((packed))
{
    uint32_t id;
    uint8_t buffer[_MAX_MSG_SIZE];
}
_Message_t;

#define _MSGID_le_instStat_AddAppInstallEventHandler 0
#define _MSGID_le_instStat_RemoveAppInstallEventHandler 1
#define _MSGID_le_instStat_AddAppUninstallEventHandler 2
#define _MSGID_le_instStat_RemoveAppUninstallEventHandler 3


#endif // LE_INSTSTAT_MESSAGES_H_INCLUDE_GUARD