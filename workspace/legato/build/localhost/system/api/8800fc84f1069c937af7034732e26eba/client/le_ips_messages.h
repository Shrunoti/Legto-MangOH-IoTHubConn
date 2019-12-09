
/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */


#ifndef LE_IPS_MESSAGES_H_INCLUDE_GUARD
#define LE_IPS_MESSAGES_H_INCLUDE_GUARD


#include "legato.h"

#define PROTOCOL_ID_STR "90ad953c761aa71f85c44f94ee91aff1"

#ifdef MK_TOOLS_BUILD
    extern const char** le_ips_ServiceInstanceNamePtr;
    #define SERVICE_INSTANCE_NAME (*le_ips_ServiceInstanceNamePtr)
#else
    #define SERVICE_INSTANCE_NAME "le_ips"
#endif


#define _MAX_MSG_SIZE 20

// Define the message type for communicating between client and server
typedef struct __attribute__((packed))
{
    uint32_t id;
    uint8_t buffer[_MAX_MSG_SIZE];
}
_Message_t;

#define _MSGID_le_ips_AddThresholdEventHandler 0
#define _MSGID_le_ips_RemoveThresholdEventHandler 1
#define _MSGID_le_ips_GetInputVoltage 2
#define _MSGID_le_ips_SetVoltageThresholds 3
#define _MSGID_le_ips_GetVoltageThresholds 4
#define _MSGID_le_ips_GetPowerSource 5
#define _MSGID_le_ips_GetBatteryLevel 6
#define _MSGID_le_ips_SetBatteryLevel 7


#endif // LE_IPS_MESSAGES_H_INCLUDE_GUARD