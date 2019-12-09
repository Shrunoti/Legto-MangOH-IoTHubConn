
/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */


#ifndef LE_RTC_MESSAGES_H_INCLUDE_GUARD
#define LE_RTC_MESSAGES_H_INCLUDE_GUARD


#include "legato.h"

#define PROTOCOL_ID_STR "88750cf71cdf94a8a7d68de367465558"

#ifdef MK_TOOLS_BUILD
    extern const char** le_rtc_ServiceInstanceNamePtr;
    #define SERVICE_INSTANCE_NAME (*le_rtc_ServiceInstanceNamePtr)
#else
    #define SERVICE_INSTANCE_NAME "le_rtc"
#endif


#define _MAX_MSG_SIZE 20

// Define the message type for communicating between client and server
typedef struct __attribute__((packed))
{
    uint32_t id;
    uint8_t buffer[_MAX_MSG_SIZE];
}
_Message_t;

#define _MSGID_le_rtc_GetUserTime 0
#define _MSGID_le_rtc_SetUserTime 1


#endif // LE_RTC_MESSAGES_H_INCLUDE_GUARD