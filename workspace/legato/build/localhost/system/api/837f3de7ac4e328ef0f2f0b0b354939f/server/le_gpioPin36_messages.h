
/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */


#ifndef LE_GPIOPIN36_MESSAGES_H_INCLUDE_GUARD
#define LE_GPIOPIN36_MESSAGES_H_INCLUDE_GUARD


#include "legato.h"

#define PROTOCOL_ID_STR "eb4b6dc575d64f27484e7657275f11fd"

#ifdef MK_TOOLS_BUILD
    extern const char** le_gpioPin36_ServiceInstanceNamePtr;
    #define SERVICE_INSTANCE_NAME (*le_gpioPin36_ServiceInstanceNamePtr)
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

#define _MSGID_le_gpioPin36_SetInput 0
#define _MSGID_le_gpioPin36_SetPushPullOutput 1
#define _MSGID_le_gpioPin36_SetTriStateOutput 2
#define _MSGID_le_gpioPin36_SetOpenDrainOutput 3
#define _MSGID_le_gpioPin36_EnablePullUp 4
#define _MSGID_le_gpioPin36_EnablePullDown 5
#define _MSGID_le_gpioPin36_DisableResistors 6
#define _MSGID_le_gpioPin36_Activate 7
#define _MSGID_le_gpioPin36_Deactivate 8
#define _MSGID_le_gpioPin36_SetHighZ 9
#define _MSGID_le_gpioPin36_Read 10
#define _MSGID_le_gpioPin36_AddChangeEventHandler 11
#define _MSGID_le_gpioPin36_RemoveChangeEventHandler 12
#define _MSGID_le_gpioPin36_SetEdgeSense 13
#define _MSGID_le_gpioPin36_DisableEdgeSense 14
#define _MSGID_le_gpioPin36_IsOutput 15
#define _MSGID_le_gpioPin36_IsInput 16
#define _MSGID_le_gpioPin36_GetEdgeSense 17
#define _MSGID_le_gpioPin36_GetPolarity 18
#define _MSGID_le_gpioPin36_IsActive 19
#define _MSGID_le_gpioPin36_GetPullUpDown 20


#endif // LE_GPIOPIN36_MESSAGES_H_INCLUDE_GUARD