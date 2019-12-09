
/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */


#ifndef LE_APPPROC_MESSAGES_H_INCLUDE_GUARD
#define LE_APPPROC_MESSAGES_H_INCLUDE_GUARD


#include "legato.h"

#define PROTOCOL_ID_STR "3a750db42e253ea2307d75519c48afeb"

#ifdef MK_TOOLS_BUILD
    extern const char** le_appProc_ServiceInstanceNamePtr;
    #define SERVICE_INSTANCE_NAME (*le_appProc_ServiceInstanceNamePtr)
#else
    #define SERVICE_INSTANCE_NAME "le_appProc"
#endif


#define _MAX_MSG_SIZE 629

// Define the message type for communicating between client and server
typedef struct __attribute__((packed))
{
    uint32_t id;
    uint8_t buffer[_MAX_MSG_SIZE];
}
_Message_t;

#define _MSGID_le_appProc_Create 0
#define _MSGID_le_appProc_SetStdIn 1
#define _MSGID_le_appProc_SetStdOut 2
#define _MSGID_le_appProc_SetStdErr 3
#define _MSGID_le_appProc_AddStopHandler 4
#define _MSGID_le_appProc_RemoveStopHandler 5
#define _MSGID_le_appProc_AddArg 6
#define _MSGID_le_appProc_ClearArgs 7
#define _MSGID_le_appProc_SetPriority 8
#define _MSGID_le_appProc_ClearPriority 9
#define _MSGID_le_appProc_SetFaultAction 10
#define _MSGID_le_appProc_ClearFaultAction 11
#define _MSGID_le_appProc_Start 12
#define _MSGID_le_appProc_Delete 13


#endif // LE_APPPROC_MESSAGES_H_INCLUDE_GUARD