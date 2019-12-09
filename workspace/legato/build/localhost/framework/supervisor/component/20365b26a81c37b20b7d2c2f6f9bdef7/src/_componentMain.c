/*
 * AUTO-GENERATED _componentMain.c for the supervisor component.

 * Don't bother hand-editing this file.
 */

#include "legato.h"
#include "../liblegato/eventLoop.h"
#include "../liblegato/log.h"

#ifdef __cplusplus
extern "C" {
#endif

extern const char* _supervisor_le_cfg_ServiceInstanceName;
const char** le_cfg_ServiceInstanceNamePtr = &_supervisor_le_cfg_ServiceInstanceName;
void le_cfg_ConnectService(void);
extern const char* _supervisor_logFd_ServiceInstanceName;
const char** logFd_ServiceInstanceNamePtr = &_supervisor_logFd_ServiceInstanceName;
void logFd_ConnectService(void);
extern const char* _supervisor_le_instStat_ServiceInstanceName;
const char** le_instStat_ServiceInstanceNamePtr = &_supervisor_le_instStat_ServiceInstanceName;
void le_instStat_ConnectService(void);
extern const char* _supervisor_le_appCtrl_ServiceInstanceName;
const char** le_appCtrl_ServiceInstanceNamePtr = &_supervisor_le_appCtrl_ServiceInstanceName;
void le_appCtrl_AdvertiseService(void);
extern const char* _supervisor_le_framework_ServiceInstanceName;
const char** le_framework_ServiceInstanceNamePtr = &_supervisor_le_framework_ServiceInstanceName;
void le_framework_AdvertiseService(void);
extern const char* _supervisor_wdog_ServiceInstanceName;
const char** wdog_ServiceInstanceNamePtr = &_supervisor_wdog_ServiceInstanceName;
void wdog_AdvertiseService(void);
extern const char* _supervisor_le_appInfo_ServiceInstanceName;
const char** le_appInfo_ServiceInstanceNamePtr = &_supervisor_le_appInfo_ServiceInstanceName;
void le_appInfo_AdvertiseService(void);
extern const char* _supervisor_le_appProc_ServiceInstanceName;
const char** le_appProc_ServiceInstanceNamePtr = &_supervisor_le_appProc_ServiceInstanceName;
void le_appProc_AdvertiseService(void);
extern const char* _supervisor_le_ima_ServiceInstanceName;
const char** le_ima_ServiceInstanceNamePtr = &_supervisor_le_ima_ServiceInstanceName;
void le_ima_AdvertiseService(void);
extern const char* _supervisor_le_sup_ctrl_ServiceInstanceName;
const char** le_sup_ctrl_ServiceInstanceNamePtr = &_supervisor_le_sup_ctrl_ServiceInstanceName;
void le_sup_ctrl_AdvertiseService(void);
extern const char* _supervisor_supervisorWdog_ServiceInstanceName;
const char** supervisorWdog_ServiceInstanceNamePtr = &_supervisor_supervisorWdog_ServiceInstanceName;
void supervisorWdog_AdvertiseService(void);
// Component log session variables.
le_log_SessionRef_t supervisor_LogSession;
le_log_Level_t* supervisor_LogLevelFilterPtr;

// Component initialization function (COMPONENT_INIT).
void _supervisor_COMPONENT_INIT(void);

// Library initialization function.
// Will be called by the dynamic linker loader when the library is loaded.
__attribute__((constructor)) void _supervisor_Init(void)
{
    LE_DEBUG("Initializing supervisor component library.");

    // Advertise server-side IPC interfaces.
    // 'le_appCtrl' is [manual-start].
    // 'le_framework' is [manual-start].
    // 'wdog' is [manual-start].
    // 'le_appInfo' is [manual-start].
    // 'le_appProc' is [manual-start].
    // 'le_ima' is [manual-start].
    // 'le_sup_ctrl' is [manual-start].
    // 'supervisorWdog' is [manual-start].

    // Connect client-side IPC interfaces.
    // 'le_cfg' is [manual-start].
    // 'logFd' is [manual-start].
    // 'le_instStat' is [manual-start].

    // Register the component with the Log Daemon.
    supervisor_LogSession = log_RegComponent("supervisor", &supervisor_LogLevelFilterPtr);

    //Queue the COMPONENT_INIT function to be called by the event loop
    event_QueueComponentInit(_supervisor_COMPONENT_INIT);
}


#ifdef __cplusplus
}
#endif
