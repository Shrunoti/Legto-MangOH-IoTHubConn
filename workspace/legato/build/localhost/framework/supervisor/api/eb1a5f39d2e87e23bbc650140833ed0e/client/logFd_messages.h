
/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */


#ifndef LOGFD_MESSAGES_H_INCLUDE_GUARD
#define LOGFD_MESSAGES_H_INCLUDE_GUARD


#include "legato.h"

#define PROTOCOL_ID_STR "f871a40b4c7617a93b47f874956f1e69"

#ifdef MK_TOOLS_BUILD
    extern const char** logFd_ServiceInstanceNamePtr;
    #define SERVICE_INSTANCE_NAME (*logFd_ServiceInstanceNamePtr)
#else
    #define SERVICE_INSTANCE_NAME "logFd"
#endif


#define _MAX_MSG_SIZE 114

// Define the message type for communicating between client and server
typedef struct __attribute__((packed))
{
    uint32_t id;
    uint8_t buffer[_MAX_MSG_SIZE];
}
_Message_t;

#define _MSGID_logFd_StdErr 0
#define _MSGID_logFd_StdOut 1


#endif // LOGFD_MESSAGES_H_INCLUDE_GUARD