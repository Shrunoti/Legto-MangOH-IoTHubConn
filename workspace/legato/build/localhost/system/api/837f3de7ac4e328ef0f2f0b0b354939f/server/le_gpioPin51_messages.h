
/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */


#ifndef LE_GPIOPIN51_MESSAGES_H_INCLUDE_GUARD
#define LE_GPIOPIN51_MESSAGES_H_INCLUDE_GUARD


#include "legato.h"

#define PROTOCOL_ID_STR "eb4b6dc575d64f27484e7657275f11fd"

#ifdef MK_TOOLS_BUILD
    extern const char** le_gpioPin51_ServiceInstanceNamePtr;
    #define SERVICE_INSTANCE_NAME (*le_gpioPin51_ServiceInstanceNamePtr)
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

#define _MSGID_le_gpioPin51_SetInput 0
#define _MSGID_le_gpioPin51_SetPushPullOutput 1
#define _MSGID_le_gpioPin51_SetTriStateOutput 2
#define _MSGID_le_gpioPin51_SetOpenDrainOutput 3
#define _MSGID_le_gpioPin51_EnablePullUp 4
#define _MSGID_le_gpioPin51_EnablePullDown 5
#define _MSGID_le_gpioPin51_DisableResistors 6
#define _MSGID_le_gpioPin51_Activate 7
#define _MSGID_le_gpioPin51_Deactivate 8
#define _MSGID_le_gpioPin51_SetHighZ 9
#define _MSGID_le_gpioPin51_Read 10
#define _MSGID_le_gpioPin51_AddChangeEventHandler 11
#define _MSGID_le_gpioPin51_RemoveChangeEventHandler 12
#define _MSGID_le_gpioPin51_SetEdgeSense 13
#define _MSGID_le_gpioPin51_DisableEdgeSense 14
#define _MSGID_le_gpioPin51_IsOutput 15
#define _MSGID_le_gpioPin51_IsInput 16
#define _MSGID_le_gpioPin51_GetEdgeSense 17
#define _MSGID_le_gpioPin51_GetPolarity 18
#define _MSGID_le_gpioPin51_IsActive 19
#define _MSGID_le_gpioPin51_GetPullUpDown 20


#endif // LE_GPIOPIN51_MESSAGES_H_INCLUDE_GUARD