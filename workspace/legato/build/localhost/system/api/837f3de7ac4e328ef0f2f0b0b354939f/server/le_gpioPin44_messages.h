
/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */


#ifndef LE_GPIOPIN44_MESSAGES_H_INCLUDE_GUARD
#define LE_GPIOPIN44_MESSAGES_H_INCLUDE_GUARD


#include "legato.h"

#define PROTOCOL_ID_STR "eb4b6dc575d64f27484e7657275f11fd"

#ifdef MK_TOOLS_BUILD
    extern const char** le_gpioPin44_ServiceInstanceNamePtr;
    #define SERVICE_INSTANCE_NAME (*le_gpioPin44_ServiceInstanceNamePtr)
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

#define _MSGID_le_gpioPin44_SetInput 0
#define _MSGID_le_gpioPin44_SetPushPullOutput 1
#define _MSGID_le_gpioPin44_SetTriStateOutput 2
#define _MSGID_le_gpioPin44_SetOpenDrainOutput 3
#define _MSGID_le_gpioPin44_EnablePullUp 4
#define _MSGID_le_gpioPin44_EnablePullDown 5
#define _MSGID_le_gpioPin44_DisableResistors 6
#define _MSGID_le_gpioPin44_Activate 7
#define _MSGID_le_gpioPin44_Deactivate 8
#define _MSGID_le_gpioPin44_SetHighZ 9
#define _MSGID_le_gpioPin44_Read 10
#define _MSGID_le_gpioPin44_AddChangeEventHandler 11
#define _MSGID_le_gpioPin44_RemoveChangeEventHandler 12
#define _MSGID_le_gpioPin44_SetEdgeSense 13
#define _MSGID_le_gpioPin44_DisableEdgeSense 14
#define _MSGID_le_gpioPin44_IsOutput 15
#define _MSGID_le_gpioPin44_IsInput 16
#define _MSGID_le_gpioPin44_GetEdgeSense 17
#define _MSGID_le_gpioPin44_GetPolarity 18
#define _MSGID_le_gpioPin44_IsActive 19
#define _MSGID_le_gpioPin44_GetPullUpDown 20


#endif // LE_GPIOPIN44_MESSAGES_H_INCLUDE_GUARD