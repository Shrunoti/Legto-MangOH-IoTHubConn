
/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */


#ifndef WDOG_MESSAGES_H_INCLUDE_GUARD
#define WDOG_MESSAGES_H_INCLUDE_GUARD


#include "legato.h"

#define PROTOCOL_ID_STR "80408175972a85f9cd0abf02df19f2f3"

#ifdef MK_TOOLS_BUILD
    extern const char** wdog_ServiceInstanceNamePtr;
    #define SERVICE_INSTANCE_NAME (*wdog_ServiceInstanceNamePtr)
#else
    #define SERVICE_INSTANCE_NAME "wdog"
#endif


#define _MAX_MSG_SIZE 12

// Define the message type for communicating between client and server
typedef struct __attribute__((packed))
{
    uint32_t id;
    uint8_t buffer[_MAX_MSG_SIZE];
}
_Message_t;

#define _MSGID_wdog_WatchdogTimedOut 0


#endif // WDOG_MESSAGES_H_INCLUDE_GUARD