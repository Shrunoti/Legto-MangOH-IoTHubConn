
/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */


#ifndef LE_RIPIN_MESSAGES_H_INCLUDE_GUARD
#define LE_RIPIN_MESSAGES_H_INCLUDE_GUARD


#include "legato.h"

#define PROTOCOL_ID_STR "aafbc3c57eec1350fdae5eb6fe083e5a"

#ifdef MK_TOOLS_BUILD
    extern const char** le_riPin_ServiceInstanceNamePtr;
    #define SERVICE_INSTANCE_NAME (*le_riPin_ServiceInstanceNamePtr)
#else
    #define SERVICE_INSTANCE_NAME "le_riPin"
#endif


#define _MAX_MSG_SIZE 13

// Define the message type for communicating between client and server
typedef struct __attribute__((packed))
{
    uint32_t id;
    uint8_t buffer[_MAX_MSG_SIZE];
}
_Message_t;

#define _MSGID_le_riPin_AmIOwnerOfRingSignal 0
#define _MSGID_le_riPin_TakeRingSignal 1
#define _MSGID_le_riPin_ReleaseRingSignal 2
#define _MSGID_le_riPin_PulseRingSignal 3


#endif // LE_RIPIN_MESSAGES_H_INCLUDE_GUARD