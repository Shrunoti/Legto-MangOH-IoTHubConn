
/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */


#ifndef LE_GPIOPIN61_MESSAGES_H_INCLUDE_GUARD
#define LE_GPIOPIN61_MESSAGES_H_INCLUDE_GUARD


#include "legato.h"

#define PROTOCOL_ID_STR "eb4b6dc575d64f27484e7657275f11fd"

#ifdef MK_TOOLS_BUILD
    extern const char** le_gpioPin61_ServiceInstanceNamePtr;
    #define SERVICE_INSTANCE_NAME (*le_gpioPin61_ServiceInstanceNamePtr)
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

#define _MSGID_le_gpioPin61_SetInput 0
#define _MSGID_le_gpioPin61_SetPushPullOutput 1
#define _MSGID_le_gpioPin61_SetTriStateOutput 2
#define _MSGID_le_gpioPin61_SetOpenDrainOutput 3
#define _MSGID_le_gpioPin61_EnablePullUp 4
#define _MSGID_le_gpioPin61_EnablePullDown 5
#define _MSGID_le_gpioPin61_DisableResistors 6
#define _MSGID_le_gpioPin61_Activate 7
#define _MSGID_le_gpioPin61_Deactivate 8
#define _MSGID_le_gpioPin61_SetHighZ 9
#define _MSGID_le_gpioPin61_Read 10
#define _MSGID_le_gpioPin61_AddChangeEventHandler 11
#define _MSGID_le_gpioPin61_RemoveChangeEventHandler 12
#define _MSGID_le_gpioPin61_SetEdgeSense 13
#define _MSGID_le_gpioPin61_DisableEdgeSense 14
#define _MSGID_le_gpioPin61_IsOutput 15
#define _MSGID_le_gpioPin61_IsInput 16
#define _MSGID_le_gpioPin61_GetEdgeSense 17
#define _MSGID_le_gpioPin61_GetPolarity 18
#define _MSGID_le_gpioPin61_IsActive 19
#define _MSGID_le_gpioPin61_GetPullUpDown 20


#endif // LE_GPIOPIN61_MESSAGES_H_INCLUDE_GUARD