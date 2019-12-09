
/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */


#ifndef CONFIGTREEWDOG_MESSAGES_H_INCLUDE_GUARD
#define CONFIGTREEWDOG_MESSAGES_H_INCLUDE_GUARD


#include "legato.h"

#define PROTOCOL_ID_STR "5f75fa14e4d6040284a6d7f2ca05dade"

#ifdef MK_TOOLS_BUILD
    extern const char** configTreeWdog_ServiceInstanceNamePtr;
    #define SERVICE_INSTANCE_NAME (*configTreeWdog_ServiceInstanceNamePtr)
#else
    #define SERVICE_INSTANCE_NAME "frameworkWdog"
#endif


#define _MAX_MSG_SIZE 20

// Define the message type for communicating between client and server
typedef struct __attribute__((packed))
{
    uint32_t id;
    uint8_t buffer[_MAX_MSG_SIZE];
}
_Message_t;

#define _MSGID_configTreeWdog_AddKickEventHandler 0
#define _MSGID_configTreeWdog_RemoveKickEventHandler 1


#endif // CONFIGTREEWDOG_MESSAGES_H_INCLUDE_GUARD