
/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */


#ifndef LE_PM_MESSAGES_H_INCLUDE_GUARD
#define LE_PM_MESSAGES_H_INCLUDE_GUARD


#include "legato.h"

#define PROTOCOL_ID_STR "39f21a4932ef87d302a8f31dfe5a0af7"

#ifdef MK_TOOLS_BUILD
    extern const char** le_pm_ServiceInstanceNamePtr;
    #define SERVICE_INSTANCE_NAME (*le_pm_ServiceInstanceNamePtr)
#else
    #define SERVICE_INSTANCE_NAME "le_pm"
#endif


#define _MAX_MSG_SIZE 51

// Define the message type for communicating between client and server
typedef struct __attribute__((packed))
{
    uint32_t id;
    uint8_t buffer[_MAX_MSG_SIZE];
}
_Message_t;

#define _MSGID_le_pm_NewWakeupSource 0
#define _MSGID_le_pm_StayAwake 1
#define _MSGID_le_pm_Relax 2
#define _MSGID_le_pm_ForceRelaxAndDestroyAllWakeupSource 3


#endif // LE_PM_MESSAGES_H_INCLUDE_GUARD