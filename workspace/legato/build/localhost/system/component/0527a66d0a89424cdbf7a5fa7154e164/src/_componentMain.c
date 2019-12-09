/*
 * AUTO-GENERATED _componentMain.c for the voiceCallService component.

 * Don't bother hand-editing this file.
 */

#include "legato.h"
#include "../liblegato/eventLoop.h"
#include "../liblegato/log.h"

#ifdef __cplusplus
extern "C" {
#endif

extern const char* _voiceCallService_le_mcc_ServiceInstanceName;
const char** le_mcc_ServiceInstanceNamePtr = &_voiceCallService_le_mcc_ServiceInstanceName;
void le_mcc_ConnectService(void);
extern const char* _voiceCallService_le_audio_ServiceInstanceName;
const char** le_audio_ServiceInstanceNamePtr = &_voiceCallService_le_audio_ServiceInstanceName;
void le_audio_ConnectService(void);
extern const char* _voiceCallService_le_voicecall_ServiceInstanceName;
const char** le_voicecall_ServiceInstanceNamePtr = &_voiceCallService_le_voicecall_ServiceInstanceName;
void le_voicecall_AdvertiseService(void);
// Component log session variables.
le_log_SessionRef_t voiceCallService_LogSession;
le_log_Level_t* voiceCallService_LogLevelFilterPtr;

// Component initialization function (COMPONENT_INIT).
void _voiceCallService_COMPONENT_INIT(void);

// Library initialization function.
// Will be called by the dynamic linker loader when the library is loaded.
__attribute__((constructor)) void _voiceCallService_Init(void)
{
    LE_DEBUG("Initializing voiceCallService component library.");

    // Advertise server-side IPC interfaces.
    le_voicecall_AdvertiseService();

    // Connect client-side IPC interfaces.
    le_mcc_ConnectService();
    le_audio_ConnectService();

    // Register the component with the Log Daemon.
    voiceCallService_LogSession = log_RegComponent("voiceCallService", &voiceCallService_LogLevelFilterPtr);

    //Queue the COMPONENT_INIT function to be called by the event loop
    event_QueueComponentInit(_voiceCallService_COMPONENT_INIT);
}


#ifdef __cplusplus
}
#endif
