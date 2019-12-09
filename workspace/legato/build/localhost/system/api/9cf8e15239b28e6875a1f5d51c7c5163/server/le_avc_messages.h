
/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */


#ifndef LE_AVC_MESSAGES_H_INCLUDE_GUARD
#define LE_AVC_MESSAGES_H_INCLUDE_GUARD


#include "legato.h"

#define PROTOCOL_ID_STR "7a818d1ee00573303dc3b4f87c43e029"

#ifdef MK_TOOLS_BUILD
    extern const char** le_avc_ServiceInstanceNamePtr;
    #define SERVICE_INSTANCE_NAME (*le_avc_ServiceInstanceNamePtr)
#else
    #define SERVICE_INSTANCE_NAME "le_avc"
#endif


#define _MAX_MSG_SIZE 537

// Define the message type for communicating between client and server
typedef struct __attribute__((packed))
{
    uint32_t id;
    uint8_t buffer[_MAX_MSG_SIZE];
}
_Message_t;

#define _MSGID_le_avc_AddStatusEventHandler 0
#define _MSGID_le_avc_RemoveStatusEventHandler 1
#define _MSGID_le_avc_AddSessionRequestEventHandler 2
#define _MSGID_le_avc_RemoveSessionRequestEventHandler 3
#define _MSGID_le_avc_AddCommInfoHandler 4
#define _MSGID_le_avc_RemoveCommInfoHandler 5
#define _MSGID_le_avc_StartSession 6
#define _MSGID_le_avc_StopSession 7
#define _MSGID_le_avc_CheckRoute 8
#define _MSGID_le_avc_DeferConnect 9
#define _MSGID_le_avc_AcceptDownload 10
#define _MSGID_le_avc_DeferDownload 11
#define _MSGID_le_avc_AcceptInstall 12
#define _MSGID_le_avc_DeferInstall 13
#define _MSGID_le_avc_AcceptUninstall 14
#define _MSGID_le_avc_DeferUninstall 15
#define _MSGID_le_avc_AcceptReboot 16
#define _MSGID_le_avc_DeferReboot 17
#define _MSGID_le_avc_GetUpdateType 18
#define _MSGID_le_avc_GetAppUpdateName 19
#define _MSGID_le_avc_BlockInstall 20
#define _MSGID_le_avc_UnblockInstall 21
#define _MSGID_le_avc_GetErrorCode 22
#define _MSGID_le_avc_GetSessionType 23
#define _MSGID_le_avc_GetHttpStatus 24
#define _MSGID_le_avc_GetPollingTimer 25
#define _MSGID_le_avc_GetRetryTimers 26
#define _MSGID_le_avc_GetApnConfig 27
#define _MSGID_le_avc_SetApnConfig 28
#define _MSGID_le_avc_SetPollingTimer 29
#define _MSGID_le_avc_SetRetryTimers 30
#define _MSGID_le_avc_GetCredentialStatus 31
#define _MSGID_le_avc_SetUserAgreement 32
#define _MSGID_le_avc_GetUserAgreement 33
#define _MSGID_le_avc_ReadLwm2mResource 34


#endif // LE_AVC_MESSAGES_H_INCLUDE_GUARD