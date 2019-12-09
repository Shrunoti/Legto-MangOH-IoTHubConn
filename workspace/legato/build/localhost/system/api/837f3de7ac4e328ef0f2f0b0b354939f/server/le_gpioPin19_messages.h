
/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */


#ifndef LE_GPIOPIN19_MESSAGES_H_INCLUDE_GUARD
#define LE_GPIOPIN19_MESSAGES_H_INCLUDE_GUARD


#include "legato.h"

#define PROTOCOL_ID_STR "eb4b6dc575d64f27484e7657275f11fd"

#ifdef MK_TOOLS_BUILD
    extern const char** le_gpioPin19_ServiceInstanceNamePtr;
    #define SERVICE_INSTANCE_NAME (*le_gpioPin19_ServiceInstanceNamePtr)
#else
    #define SERVICE_INSTANCE_NAME "le_gpio"
#endif


#define _MAX_MSG_SIZE 24

// Define the message type for communicating between client and server
typedef struct __attribute__((packed))
{
    uint32_t id;
    uint8_t buffer[_MAX_MSG_SIZE];
}
_Message_t;

#define _MSGID_le_gpioPin19_SetInput 0
#define _MSGID_le_gpioPin19_SetPushPullOutput 1
#define _MSGID_le_gpioPin19_SetTriStateOutput 2
#define _MSGID_le_gpioPin19_SetOpenDrainOutput 3
#define _MSGID_le_gpioPin19_EnablePullUp 4
#define _MSGID_le_gpioPin19_EnablePullDown 5
#define _MSGID_le_gpioPin19_DisableResistors 6
#define _MSGID_le_gpioPin19_Activate 7
#define _MSGID_le_gpioPin19_Deactivate 8
#define _MSGID_le_gpioPin19_SetHighZ 9
#define _MSGID_le_gpioPin19_Read 10
#define _MSGID_le_gpioPin19_AddChangeEventHandler 11
#define _MSGID_le_gpioPin19_RemoveChangeEventHandler 12
#define _MSGID_le_gpioPin19_SetEdgeSense 13
#define _MSGID_le_gpioPin19_DisableEdgeSense 14
#define _MSGID_le_gpioPin19_IsOutput 15
#define _MSGID_le_gpioPin19_IsInput 16
#define _MSGID_le_gpioPin19_GetEdgeSense 17
#define _MSGID_le_gpioPin19_GetPolarity 18
#define _MSGID_le_gpioPin19_IsActive 19
#define _MSGID_le_gpioPin19_GetPullUpDown 20


#endif // LE_GPIOPIN19_MESSAGES_H_INCLUDE_GUARD