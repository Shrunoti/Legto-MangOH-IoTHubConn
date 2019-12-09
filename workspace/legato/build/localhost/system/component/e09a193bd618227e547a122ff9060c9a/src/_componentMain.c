/*
 * AUTO-GENERATED _componentMain.c for the smsInboxService component.

 * Don't bother hand-editing this file.
 */

#include "legato.h"
#include "../liblegato/eventLoop.h"
#include "../liblegato/log.h"

#ifdef __cplusplus
extern "C" {
#endif

extern const char* _smsInboxService_le_sim_ServiceInstanceName;
const char** le_sim_ServiceInstanceNamePtr = &_smsInboxService_le_sim_ServiceInstanceName;
void le_sim_ConnectService(void);
extern const char* _smsInboxService_le_sms_ServiceInstanceName;
const char** le_sms_ServiceInstanceNamePtr = &_smsInboxService_le_sms_ServiceInstanceName;
void le_sms_ConnectService(void);
extern const char* _smsInboxService_le_cfg_ServiceInstanceName;
const char** le_cfg_ServiceInstanceNamePtr = &_smsInboxService_le_cfg_ServiceInstanceName;
void le_cfg_ConnectService(void);
extern const char* _smsInboxService_le_smsInbox1_ServiceInstanceName;
const char** le_smsInbox1_ServiceInstanceNamePtr = &_smsInboxService_le_smsInbox1_ServiceInstanceName;
void le_smsInbox1_AdvertiseService(void);
extern const char* _smsInboxService_le_smsInbox2_ServiceInstanceName;
const char** le_smsInbox2_ServiceInstanceNamePtr = &_smsInboxService_le_smsInbox2_ServiceInstanceName;
void le_smsInbox2_AdvertiseService(void);
// Component log session variables.
le_log_SessionRef_t smsInboxService_LogSession;
le_log_Level_t* smsInboxService_LogLevelFilterPtr;

// Component initialization function (COMPONENT_INIT).
void _smsInboxService_COMPONENT_INIT(void);

// Library initialization function.
// Will be called by the dynamic linker loader when the library is loaded.
__attribute__((constructor)) void _smsInboxService_Init(void)
{
    LE_DEBUG("Initializing smsInboxService component library.");

    // Advertise server-side IPC interfaces.
    le_smsInbox1_AdvertiseService();
    le_smsInbox2_AdvertiseService();

    // Connect client-side IPC interfaces.
    le_sim_ConnectService();
    le_sms_ConnectService();
    le_cfg_ConnectService();

    // Register the component with the Log Daemon.
    smsInboxService_LogSession = log_RegComponent("smsInboxService", &smsInboxService_LogLevelFilterPtr);

    //Queue the COMPONENT_INIT function to be called by the event loop
    event_QueueComponentInit(_smsInboxService_COMPONENT_INIT);
}


#ifdef __cplusplus
}
#endif
