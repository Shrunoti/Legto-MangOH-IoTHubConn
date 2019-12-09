
/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */


#ifndef LE_APPCTRL_MESSAGES_H_INCLUDE_GUARD
#define LE_APPCTRL_MESSAGES_H_INCLUDE_GUARD


#include "legato.h"

#define PROTOCOL_ID_STR "bbc8a1041b89cad83df1e4bfb1c9a067"

#ifdef MK_TOOLS_BUILD
    extern const char** le_appCtrl_ServiceInstanceNamePtr;
    #define SERVICE_INSTANCE_NAME (*le_appCtrl_ServiceInstanceNamePtr)
#else
    #define SERVICE_INSTANCE_NAME "le_appCtrl"
#endif


#define _MAX_MSG_SIZE 537

// Define the message type for communicating between client and server
typedef struct __attribute__((packed))
{
    uint32_t id;
    uint8_t buffer[_MAX_MSG_SIZE];
}
_Message_t;

#define _MSGID_le_appCtrl_GetRef 0
#define _MSGID_le_appCtrl_ReleaseRef 1
#define _MSGID_le_appCtrl_SetRun 2
#define _MSGID_le_appCtrl_Import 3
#define _MSGID_le_appCtrl_SetDevicePerm 4
#define _MSGID_le_appCtrl_AddTraceAttachHandler 5
#define _MSGID_le_appCtrl_RemoveTraceAttachHandler 6
#define _MSGID_le_appCtrl_TraceUnblock 7
#define _MSGID_le_appCtrl_Start 8
#define _MSGID_le_appCtrl_Stop 9


#endif // LE_APPCTRL_MESSAGES_H_INCLUDE_GUARD