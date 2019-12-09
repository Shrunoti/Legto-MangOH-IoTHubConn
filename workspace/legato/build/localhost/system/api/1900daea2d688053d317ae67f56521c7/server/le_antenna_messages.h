
/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */


#ifndef LE_ANTENNA_MESSAGES_H_INCLUDE_GUARD
#define LE_ANTENNA_MESSAGES_H_INCLUDE_GUARD


#include "legato.h"

#define PROTOCOL_ID_STR "c6ad7def5425943d0e3b97ad84593ed5"

#ifdef MK_TOOLS_BUILD
    extern const char** le_antenna_ServiceInstanceNamePtr;
    #define SERVICE_INSTANCE_NAME (*le_antenna_ServiceInstanceNamePtr)
#else
    #define SERVICE_INSTANCE_NAME "le_antenna"
#endif


#define _MAX_MSG_SIZE 20

// Define the message type for communicating between client and server
typedef struct __attribute__((packed))
{
    uint32_t id;
    uint8_t buffer[_MAX_MSG_SIZE];
}
_Message_t;

#define _MSGID_le_antenna_Request 0
#define _MSGID_le_antenna_GetType 1
#define _MSGID_le_antenna_SetShortLimit 2
#define _MSGID_le_antenna_GetShortLimit 3
#define _MSGID_le_antenna_SetOpenLimit 4
#define _MSGID_le_antenna_GetOpenLimit 5
#define _MSGID_le_antenna_AddStatusEventHandler 6
#define _MSGID_le_antenna_RemoveStatusEventHandler 7
#define _MSGID_le_antenna_GetStatus 8
#define _MSGID_le_antenna_SetExternalAdc 9
#define _MSGID_le_antenna_GetExternalAdc 10


#endif // LE_ANTENNA_MESSAGES_H_INCLUDE_GUARD