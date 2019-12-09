
/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */


#ifndef LE_PORT_MESSAGES_H_INCLUDE_GUARD
#define LE_PORT_MESSAGES_H_INCLUDE_GUARD


#include "legato.h"

#define PROTOCOL_ID_STR "c7135da061a6a38d810159fff5799177"

#ifdef MK_TOOLS_BUILD
    extern const char** le_port_ServiceInstanceNamePtr;
    #define SERVICE_INSTANCE_NAME (*le_port_ServiceInstanceNamePtr)
#else
    #define SERVICE_INSTANCE_NAME "le_port"
#endif


#define _MAX_MSG_SIZE 66

// Define the message type for communicating between client and server
typedef struct __attribute__((packed))
{
    uint32_t id;
    uint8_t buffer[_MAX_MSG_SIZE];
}
_Message_t;

#define _MSGID_le_port_Request 0
#define _MSGID_le_port_SetDataMode 1
#define _MSGID_le_port_SetCommandMode 2
#define _MSGID_le_port_Release 3
#define _MSGID_le_port_GetPortReference 4


#endif // LE_PORT_MESSAGES_H_INCLUDE_GUARD