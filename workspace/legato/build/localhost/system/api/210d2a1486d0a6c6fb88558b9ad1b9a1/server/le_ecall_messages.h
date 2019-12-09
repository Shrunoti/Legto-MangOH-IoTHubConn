
/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */


#ifndef LE_ECALL_MESSAGES_H_INCLUDE_GUARD
#define LE_ECALL_MESSAGES_H_INCLUDE_GUARD


#include "legato.h"

#define PROTOCOL_ID_STR "7dd2ab731abae99c7141df6e32dd6b95"

#ifdef MK_TOOLS_BUILD
    extern const char** le_ecall_ServiceInstanceNamePtr;
    #define SERVICE_INSTANCE_NAME (*le_ecall_ServiceInstanceNamePtr)
#else
    #define SERVICE_INSTANCE_NAME "le_ecall"
#endif


#define _MAX_MSG_SIZE 160

// Define the message type for communicating between client and server
typedef struct __attribute__((packed))
{
    uint32_t id;
    uint8_t buffer[_MAX_MSG_SIZE];
}
_Message_t;

#define _MSGID_le_ecall_ForceOnlyMode 0
#define _MSGID_le_ecall_ForcePersistentOnlyMode 1
#define _MSGID_le_ecall_ExitOnlyMode 2
#define _MSGID_le_ecall_GetConfiguredOperationMode 3
#define _MSGID_le_ecall_AddStateChangeHandler 4
#define _MSGID_le_ecall_RemoveStateChangeHandler 5
#define _MSGID_le_ecall_Create 6
#define _MSGID_le_ecall_Delete 7
#define _MSGID_le_ecall_SetSystemStandard 8
#define _MSGID_le_ecall_GetSystemStandard 9
#define _MSGID_le_ecall_SetMsdVersion 10
#define _MSGID_le_ecall_GetMsdVersion 11
#define _MSGID_le_ecall_SetVehicleType 12
#define _MSGID_le_ecall_GetVehicleType 13
#define _MSGID_le_ecall_SetVIN 14
#define _MSGID_le_ecall_GetVIN 15
#define _MSGID_le_ecall_SetPropulsionType 16
#define _MSGID_le_ecall_GetPropulsionType 17
#define _MSGID_le_ecall_SetMsdTxMode 18
#define _MSGID_le_ecall_GetMsdTxMode 19
#define _MSGID_le_ecall_SetMsdPosition 20
#define _MSGID_le_ecall_SetMsdPositionN1 21
#define _MSGID_le_ecall_SetMsdPositionN2 22
#define _MSGID_le_ecall_SetMsdPassengersCount 23
#define _MSGID_le_ecall_ImportMsd 24
#define _MSGID_le_ecall_ExportMsd 25
#define _MSGID_le_ecall_SendMsd 26
#define _MSGID_le_ecall_StartAutomatic 27
#define _MSGID_le_ecall_StartManual 28
#define _MSGID_le_ecall_StartTest 29
#define _MSGID_le_ecall_End 30
#define _MSGID_le_ecall_GetState 31
#define _MSGID_le_ecall_SetPsapNumber 32
#define _MSGID_le_ecall_GetPsapNumber 33
#define _MSGID_le_ecall_UseUSimNumbers 34
#define _MSGID_le_ecall_SetNadDeregistrationTime 35
#define _MSGID_le_ecall_GetNadDeregistrationTime 36
#define _MSGID_le_ecall_SetIntervalBetweenDialAttempts 37
#define _MSGID_le_ecall_GetIntervalBetweenDialAttempts 38
#define _MSGID_le_ecall_SetEraGlonassManualDialAttempts 39
#define _MSGID_le_ecall_SetEraGlonassAutoDialAttempts 40
#define _MSGID_le_ecall_SetEraGlonassDialDuration 41
#define _MSGID_le_ecall_SetEraGlonassFallbackTime 42
#define _MSGID_le_ecall_SetEraGlonassAutoAnswerTime 43
#define _MSGID_le_ecall_SetEraGlonassMSDMaxTransmissionTime 44
#define _MSGID_le_ecall_GetEraGlonassManualDialAttempts 45
#define _MSGID_le_ecall_GetEraGlonassAutoDialAttempts 46
#define _MSGID_le_ecall_GetEraGlonassDialDuration 47
#define _MSGID_le_ecall_GetEraGlonassFallbackTime 48
#define _MSGID_le_ecall_GetEraGlonassAutoAnswerTime 49
#define _MSGID_le_ecall_GetEraGlonassMSDMaxTransmissionTime 50
#define _MSGID_le_ecall_SetMsdEraGlonassCrashSeverity 51
#define _MSGID_le_ecall_ResetMsdEraGlonassCrashSeverity 52
#define _MSGID_le_ecall_SetMsdEraGlonassDiagnosticResult 53
#define _MSGID_le_ecall_ResetMsdEraGlonassDiagnosticResult 54
#define _MSGID_le_ecall_SetMsdEraGlonassCrashInfo 55
#define _MSGID_le_ecall_ResetMsdEraGlonassCrashInfo 56
#define _MSGID_le_ecall_SetMsdEraGlonassCoordinateSystemType 57
#define _MSGID_le_ecall_ResetMsdEraGlonassCoordinateSystemType 58
#define _MSGID_le_ecall_GetTerminationReason 59
#define _MSGID_le_ecall_GetPlatformSpecificTerminationCode 60


#endif // LE_ECALL_MESSAGES_H_INCLUDE_GUARD