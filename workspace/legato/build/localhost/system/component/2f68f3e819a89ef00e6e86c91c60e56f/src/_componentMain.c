/*
 * AUTO-GENERATED _componentMain.c for the powerMgr component.

 * Don't bother hand-editing this file.
 */

#include "legato.h"
#include "../liblegato/eventLoop.h"
#include "../liblegato/log.h"

#ifdef __cplusplus
extern "C" {
#endif

extern const char* _powerMgr_le_framework_ServiceInstanceName;
const char** le_framework_ServiceInstanceNamePtr = &_powerMgr_le_framework_ServiceInstanceName;
void le_framework_ConnectService(void);
extern const char* _powerMgr_le_pm_ServiceInstanceName;
const char** le_pm_ServiceInstanceNamePtr = &_powerMgr_le_pm_ServiceInstanceName;
void le_pm_AdvertiseService(void);
extern const char* _powerMgr_le_ulpm_ServiceInstanceName;
const char** le_ulpm_ServiceInstanceNamePtr = &_powerMgr_le_ulpm_ServiceInstanceName;
void le_ulpm_AdvertiseService(void);
extern const char* _powerMgr_le_bootReason_ServiceInstanceName;
const char** le_bootReason_ServiceInstanceNamePtr = &_powerMgr_le_bootReason_ServiceInstanceName;
void le_bootReason_AdvertiseService(void);
// Component log session variables.
le_log_SessionRef_t powerMgr_LogSession;
le_log_Level_t* powerMgr_LogLevelFilterPtr;

// Component initialization function (COMPONENT_INIT).
void _powerMgr_COMPONENT_INIT(void);

// Library initialization function.
// Will be called by the dynamic linker loader when the library is loaded.
__attribute__((constructor)) void _powerMgr_Init(void)
{
    LE_DEBUG("Initializing powerMgr component library.");

    // Advertise server-side IPC interfaces.
    le_pm_AdvertiseService();
    le_ulpm_AdvertiseService();
    le_bootReason_AdvertiseService();

    // Connect client-side IPC interfaces.
    le_framework_ConnectService();

    // Register the component with the Log Daemon.
    powerMgr_LogSession = log_RegComponent("powerMgr", &powerMgr_LogLevelFilterPtr);

    //Queue the COMPONENT_INIT function to be called by the event loop
    event_QueueComponentInit(_powerMgr_COMPONENT_INIT);
}


#ifdef __cplusplus
}
#endif
