
/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */


#ifndef LE_ADC_MESSAGES_H_INCLUDE_GUARD
#define LE_ADC_MESSAGES_H_INCLUDE_GUARD


#include "legato.h"

#define PROTOCOL_ID_STR "cca082d3d4d6b9c7c11466ab2ea74e4f"

#ifdef MK_TOOLS_BUILD
    extern const char** le_adc_ServiceInstanceNamePtr;
    #define SERVICE_INSTANCE_NAME (*le_adc_ServiceInstanceNamePtr)
#else
    #define SERVICE_INSTANCE_NAME "le_adc"
#endif


#define _MAX_MSG_SIZE 50

// Define the message type for communicating between client and server
typedef struct __attribute__((packed))
{
    uint32_t id;
    uint8_t buffer[_MAX_MSG_SIZE];
}
_Message_t;

#define _MSGID_le_adc_ReadValue 0


#endif // LE_ADC_MESSAGES_H_INCLUDE_GUARD