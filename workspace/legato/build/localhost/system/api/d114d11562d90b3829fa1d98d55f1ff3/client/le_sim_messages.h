
/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */


#ifndef LE_SIM_MESSAGES_H_INCLUDE_GUARD
#define LE_SIM_MESSAGES_H_INCLUDE_GUARD


#include "legato.h"

#define PROTOCOL_ID_STR "407615b08a6c306e1e6fcd1f40a8c9cd"

#ifdef MK_TOOLS_BUILD
    extern const char** le_sim_ServiceInstanceNamePtr;
    #define SERVICE_INSTANCE_NAME (*le_sim_ServiceInstanceNamePtr)
#else
    #define SERVICE_INSTANCE_NAME "le_sim"
#endif


#define _MAX_MSG_SIZE 536

// Define the message type for communicating between client and server
typedef struct __attribute__((packed))
{
    uint32_t id;
    uint8_t buffer[_MAX_MSG_SIZE];
}
_Message_t;

#define _MSGID_le_sim_AddNewStateHandler 0
#define _MSGID_le_sim_RemoveNewStateHandler 1
#define _MSGID_le_sim_AddProfileUpdateHandler 2
#define _MSGID_le_sim_RemoveProfileUpdateHandler 3
#define _MSGID_le_sim_AddSimToolkitEventHandler 4
#define _MSGID_le_sim_RemoveSimToolkitEventHandler 5
#define _MSGID_le_sim_AddIccidChangeHandler 6
#define _MSGID_le_sim_RemoveIccidChangeHandler 7
#define _MSGID_le_sim_GetSelectedCard 8
#define _MSGID_le_sim_SelectCard 9
#define _MSGID_le_sim_GetICCID 10
#define _MSGID_le_sim_GetEID 11
#define _MSGID_le_sim_GetIMSI 12
#define _MSGID_le_sim_IsPresent 13
#define _MSGID_le_sim_IsReady 14
#define _MSGID_le_sim_EnterPIN 15
#define _MSGID_le_sim_ChangePIN 16
#define _MSGID_le_sim_GetRemainingPINTries 17
#define _MSGID_le_sim_GetRemainingPUKTries 18
#define _MSGID_le_sim_Unlock 19
#define _MSGID_le_sim_Lock 20
#define _MSGID_le_sim_Unblock 21
#define _MSGID_le_sim_GetState 22
#define _MSGID_le_sim_GetSubscriberPhoneNumber 23
#define _MSGID_le_sim_GetHomeNetworkOperator 24
#define _MSGID_le_sim_GetHomeNetworkMccMnc 25
#define _MSGID_le_sim_LocalSwapToEmergencyCallSubscription 26
#define _MSGID_le_sim_LocalSwapToCommercialSubscription 27
#define _MSGID_le_sim_IsEmergencyCallSubscriptionSelected 28
#define _MSGID_le_sim_AcceptSimToolkitCommand 29
#define _MSGID_le_sim_RejectSimToolkitCommand 30
#define _MSGID_le_sim_GetSimToolkitRefreshMode 31
#define _MSGID_le_sim_GetSimToolkitRefreshStage 32
#define _MSGID_le_sim_SendApdu 33
#define _MSGID_le_sim_SendCommand 34
#define _MSGID_le_sim_Reset 35
#define _MSGID_le_sim_CreateFPLMNList 36
#define _MSGID_le_sim_AddFPLMNOperator 37
#define _MSGID_le_sim_WriteFPLMNList 38
#define _MSGID_le_sim_ReadFPLMNList 39
#define _MSGID_le_sim_GetFirstFPLMNOperator 40
#define _MSGID_le_sim_GetNextFPLMNOperator 41
#define _MSGID_le_sim_DeleteFPLMNList 42
#define _MSGID_le_sim_OpenLogicalChannel 43
#define _MSGID_le_sim_CloseLogicalChannel 44
#define _MSGID_le_sim_SendApduOnChannel 45
#define _MSGID_le_sim_SetPower 46


#endif // LE_SIM_MESSAGES_H_INCLUDE_GUARD