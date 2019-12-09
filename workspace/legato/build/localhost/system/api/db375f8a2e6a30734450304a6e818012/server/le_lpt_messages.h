
/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */


#ifndef LE_LPT_MESSAGES_H_INCLUDE_GUARD
#define LE_LPT_MESSAGES_H_INCLUDE_GUARD


#include "legato.h"

#define PROTOCOL_ID_STR "ddfabab9b938cdcb7564dbd7632f76bc"

#ifdef MK_TOOLS_BUILD
    extern const char** le_lpt_ServiceInstanceNamePtr;
    #define SERVICE_INSTANCE_NAME (*le_lpt_ServiceInstanceNamePtr)
#else
    #define SERVICE_INSTANCE_NAME "le_lpt"
#endif


#define _MAX_MSG_SIZE 20

// Define the message type for communicating between client and server
typedef struct __attribute__((packed))
{
    uint32_t id;
    uint8_t buffer[_MAX_MSG_SIZE];
}
_Message_t;

#define _MSGID_le_lpt_SetEDrxState 0
#define _MSGID_le_lpt_SetRequestedEDrxValue 1
#define _MSGID_le_lpt_GetRequestedEDrxValue 2
#define _MSGID_le_lpt_GetNetworkProvidedEDrxValue 3
#define _MSGID_le_lpt_GetNetworkProvidedPagingTimeWindow 4
#define _MSGID_le_lpt_AddEDrxParamsChangeHandler 5
#define _MSGID_le_lpt_RemoveEDrxParamsChangeHandler 6


#endif // LE_LPT_MESSAGES_H_INCLUDE_GUARD