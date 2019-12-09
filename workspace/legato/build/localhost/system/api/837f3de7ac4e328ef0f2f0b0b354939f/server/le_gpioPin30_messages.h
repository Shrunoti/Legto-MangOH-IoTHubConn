
/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */


#ifndef LE_GPIOPIN30_MESSAGES_H_INCLUDE_GUARD
#define LE_GPIOPIN30_MESSAGES_H_INCLUDE_GUARD


#include "legato.h"

#define PROTOCOL_ID_STR "eb4b6dc575d64f27484e7657275f11fd"

#ifdef MK_TOOLS_BUILD
    extern const char** le_gpioPin30_ServiceInstanceNamePtr;
    #define SERVICE_INSTANCE_NAME (*le_gpioPin30_ServiceInstanceNamePtr)
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

#define _MSGID_le_gpioPin30_SetInput 0
#define _MSGID_le_gpioPin30_SetPushPullOutput 1
#define _MSGID_le_gpioPin30_SetTriStateOutput 2
#define _MSGID_le_gpioPin30_SetOpenDrainOutput 3
#define _MSGID_le_gpioPin30_EnablePullUp 4
#define _MSGID_le_gpioPin30_EnablePullDown 5
#define _MSGID_le_gpioPin30_DisableResistors 6
#define _MSGID_le_gpioPin30_Activate 7
#define _MSGID_le_gpioPin30_Deactivate 8
#define _MSGID_le_gpioPin30_SetHighZ 9
#define _MSGID_le_gpioPin30_Read 10
#define _MSGID_le_gpioPin30_AddChangeEventHandler 11
#define _MSGID_le_gpioPin30_RemoveChangeEventHandler 12
#define _MSGID_le_gpioPin30_SetEdgeSense 13
#define _MSGID_le_gpioPin30_DisableEdgeSense 14
#define _MSGID_le_gpioPin30_IsOutput 15
#define _MSGID_le_gpioPin30_IsInput 16
#define _MSGID_le_gpioPin30_GetEdgeSense 17
#define _MSGID_le_gpioPin30_GetPolarity 18
#define _MSGID_le_gpioPin30_IsActive 19
#define _MSGID_le_gpioPin30_GetPullUpDown 20


#endif // LE_GPIOPIN30_MESSAGES_H_INCLUDE_GUARD