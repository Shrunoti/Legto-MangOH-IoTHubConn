
/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */


#ifndef LE_WIFICLIENT_MESSAGES_H_INCLUDE_GUARD
#define LE_WIFICLIENT_MESSAGES_H_INCLUDE_GUARD


#include "legato.h"

#define PROTOCOL_ID_STR "0ca3c398f8ed9d944b4f29c4904fb465"

#ifdef MK_TOOLS_BUILD
    extern const char** le_wifiClient_ServiceInstanceNamePtr;
    #define SERVICE_INSTANCE_NAME (*le_wifiClient_ServiceInstanceNamePtr)
#else
    #define SERVICE_INSTANCE_NAME "le_wifiClient"
#endif


#define _MAX_MSG_SIZE 219

// Define the message type for communicating between client and server
typedef struct __attribute__((packed))
{
    uint32_t id;
    uint8_t buffer[_MAX_MSG_SIZE];
}
_Message_t;

#define _MSGID_le_wifiClient_AddNewEventHandler 0
#define _MSGID_le_wifiClient_RemoveNewEventHandler 1
#define _MSGID_le_wifiClient_Start 2
#define _MSGID_le_wifiClient_Stop 3
#define _MSGID_le_wifiClient_Scan 4
#define _MSGID_le_wifiClient_GetFirstAccessPoint 5
#define _MSGID_le_wifiClient_GetNextAccessPoint 6
#define _MSGID_le_wifiClient_GetSignalStrength 7
#define _MSGID_le_wifiClient_GetBssid 8
#define _MSGID_le_wifiClient_GetSsid 9
#define _MSGID_le_wifiClient_SetPassphrase 10
#define _MSGID_le_wifiClient_SetPreSharedKey 11
#define _MSGID_le_wifiClient_SetSecurityProtocol 12
#define _MSGID_le_wifiClient_SetUserCredentials 13
#define _MSGID_le_wifiClient_SetWepKey 14
#define _MSGID_le_wifiClient_SetHiddenNetworkAttribute 15
#define _MSGID_le_wifiClient_Create 16
#define _MSGID_le_wifiClient_Delete 17
#define _MSGID_le_wifiClient_Connect 18
#define _MSGID_le_wifiClient_Disconnect 19


#endif // LE_WIFICLIENT_MESSAGES_H_INCLUDE_GUARD