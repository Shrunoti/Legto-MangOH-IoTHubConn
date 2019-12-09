
/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */


#ifndef LE_FLASH_MESSAGES_H_INCLUDE_GUARD
#define LE_FLASH_MESSAGES_H_INCLUDE_GUARD


#include "legato.h"

#define PROTOCOL_ID_STR "a36a81bca852297786086f08a8884cda"

#ifdef MK_TOOLS_BUILD
    extern const char** le_flash_ServiceInstanceNamePtr;
    #define SERVICE_INSTANCE_NAME (*le_flash_ServiceInstanceNamePtr)
#else
    #define SERVICE_INSTANCE_NAME "le_flash"
#endif


#define _MAX_MSG_SIZE 262168

// Define the message type for communicating between client and server
typedef struct __attribute__((packed))
{
    uint32_t id;
    uint8_t buffer[_MAX_MSG_SIZE];
}
_Message_t;

#define _MSGID_le_flash_AddBadImageDetectionHandler 0
#define _MSGID_le_flash_RemoveBadImageDetectionHandler 1
#define _MSGID_le_flash_RequestAccess 2
#define _MSGID_le_flash_ReleaseAccess 3
#define _MSGID_le_flash_OpenMtd 4
#define _MSGID_le_flash_OpenUbi 5
#define _MSGID_le_flash_OpenUbiVolume 6
#define _MSGID_le_flash_CloseUbiVolume 7
#define _MSGID_le_flash_Close 8
#define _MSGID_le_flash_EraseBlock 9
#define _MSGID_le_flash_Read 10
#define _MSGID_le_flash_Write 11
#define _MSGID_le_flash_GetBlockInformation 12
#define _MSGID_le_flash_GetUbiVolumeInformation 13
#define _MSGID_le_flash_CreateUbi 14
#define _MSGID_le_flash_CreateUbiVolume 15
#define _MSGID_le_flash_DeleteUbiVolume 16


#endif // LE_FLASH_MESSAGES_H_INCLUDE_GUARD