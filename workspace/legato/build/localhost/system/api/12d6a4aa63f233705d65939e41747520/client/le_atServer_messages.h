
/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */


#ifndef LE_ATSERVER_MESSAGES_H_INCLUDE_GUARD
#define LE_ATSERVER_MESSAGES_H_INCLUDE_GUARD


#include "legato.h"

#define PROTOCOL_ID_STR "2527056ddcf5218c3a18f8b922a25fd2"

#ifdef MK_TOOLS_BUILD
    extern const char** le_atServer_ServiceInstanceNamePtr;
    #define SERVICE_INSTANCE_NAME (*le_atServer_ServiceInstanceNamePtr)
#else
    #define SERVICE_INSTANCE_NAME "le_atServer"
#endif


#define _MAX_MSG_SIZE 4119

// Define the message type for communicating between client and server
typedef struct __attribute__((packed))
{
    uint32_t id;
    uint8_t buffer[_MAX_MSG_SIZE];
}
_Message_t;

#define _MSGID_le_atServer_Suspend 0
#define _MSGID_le_atServer_Resume 1
#define _MSGID_le_atServer_Open 2
#define _MSGID_le_atServer_Close 3
#define _MSGID_le_atServer_Create 4
#define _MSGID_le_atServer_Delete 5
#define _MSGID_le_atServer_AddCommandHandler 6
#define _MSGID_le_atServer_RemoveCommandHandler 7
#define _MSGID_le_atServer_GetParameter 8
#define _MSGID_le_atServer_GetCommandName 9
#define _MSGID_le_atServer_GetDevice 10
#define _MSGID_le_atServer_SendIntermediateResponse 11
#define _MSGID_le_atServer_SendFinalResponse 12
#define _MSGID_le_atServer_SendFinalResultCode 13
#define _MSGID_le_atServer_SendUnsolicitedResponse 14
#define _MSGID_le_atServer_EnableEcho 15
#define _MSGID_le_atServer_DisableEcho 16
#define _MSGID_le_atServer_OpenBridge 17
#define _MSGID_le_atServer_CloseBridge 18
#define _MSGID_le_atServer_AddDeviceToBridge 19
#define _MSGID_le_atServer_RemoveDeviceFromBridge 20
#define _MSGID_le_atServer_EnableVerboseErrorCodes 21
#define _MSGID_le_atServer_EnableExtendedErrorCodes 22
#define _MSGID_le_atServer_DisableExtendedErrorCodes 23
#define _MSGID_le_atServer_CreateErrorCode 24
#define _MSGID_le_atServer_DeleteErrorCode 25
#define _MSGID_le_atServer_SetVerboseErrorCode 26
#define _MSGID_le_atServer_GetTextAsync 27


#endif // LE_ATSERVER_MESSAGES_H_INCLUDE_GUARD