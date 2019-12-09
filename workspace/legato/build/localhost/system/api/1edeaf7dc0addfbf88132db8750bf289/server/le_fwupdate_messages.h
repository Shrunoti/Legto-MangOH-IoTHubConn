
/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */


#ifndef LE_FWUPDATE_MESSAGES_H_INCLUDE_GUARD
#define LE_FWUPDATE_MESSAGES_H_INCLUDE_GUARD


#include "legato.h"

#define PROTOCOL_ID_STR "a8f730857c018907842f8ee02858e26f"

#ifdef MK_TOOLS_BUILD
    extern const char** le_fwupdate_ServiceInstanceNamePtr;
    #define SERVICE_INSTANCE_NAME (*le_fwupdate_ServiceInstanceNamePtr)
#else
    #define SERVICE_INSTANCE_NAME "le_fwupdate"
#endif


#define _MAX_MSG_SIZE 272

// Define the message type for communicating between client and server
typedef struct __attribute__((packed))
{
    uint32_t id;
    uint8_t buffer[_MAX_MSG_SIZE];
}
_Message_t;

#define _MSGID_le_fwupdate_Download 0
#define _MSGID_le_fwupdate_InitDownload 1
#define _MSGID_le_fwupdate_GetResumePosition 2
#define _MSGID_le_fwupdate_GetUpdateStatus 3
#define _MSGID_le_fwupdate_GetFirmwareVersion 4
#define _MSGID_le_fwupdate_GetBootloaderVersion 5
#define _MSGID_le_fwupdate_GetAppBootloaderVersion 6
#define _MSGID_le_fwupdate_IsSystemMarkedGood 7
#define _MSGID_le_fwupdate_Install 8
#define _MSGID_le_fwupdate_MarkGood 9
#define _MSGID_le_fwupdate_InstallAndMarkGood 10


#endif // LE_FWUPDATE_MESSAGES_H_INCLUDE_GUARD