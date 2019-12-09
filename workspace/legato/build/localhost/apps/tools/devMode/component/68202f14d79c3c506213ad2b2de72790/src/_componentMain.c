/*
 * AUTO-GENERATED _componentMain.c for the devMode component.

 * Don't bother hand-editing this file.
 */

#include "legato.h"
#include "../liblegato/eventLoop.h"
#include "../liblegato/log.h"

#ifdef __cplusplus
extern "C" {
#endif

extern const char* _devMode_le_pm_ServiceInstanceName;
const char** le_pm_ServiceInstanceNamePtr = &_devMode_le_pm_ServiceInstanceName;
void le_pm_ConnectService(void);
extern const char* _devMode_le_updateCtrl_ServiceInstanceName;
const char** le_updateCtrl_ServiceInstanceNamePtr = &_devMode_le_updateCtrl_ServiceInstanceName;
void le_updateCtrl_ConnectService(void);
extern const char* _devMode_le_instStat_ServiceInstanceName;
const char** le_instStat_ServiceInstanceNamePtr = &_devMode_le_instStat_ServiceInstanceName;
void le_instStat_ConnectService(void);
extern const char* _devMode_le_framework_ServiceInstanceName;
const char** le_framework_ServiceInstanceNamePtr = &_devMode_le_framework_ServiceInstanceName;
void le_framework_ConnectService(void);
// Component log session variables.
le_log_SessionRef_t devMode_LogSession;
le_log_Level_t* devMode_LogLevelFilterPtr;

// Component initialization function (COMPONENT_INIT).
void _devMode_COMPONENT_INIT(void);

// Library initialization function.
// Will be called by the dynamic linker loader when the library is loaded.
__attribute__((constructor)) void _devMode_Init(void)
{
    LE_DEBUG("Initializing devMode component library.");

    // Connect client-side IPC interfaces.
    le_pm_ConnectService();
    le_updateCtrl_ConnectService();
    le_instStat_ConnectService();
    le_framework_ConnectService();

    // Register the component with the Log Daemon.
    devMode_LogSession = log_RegComponent("devMode", &devMode_LogLevelFilterPtr);

    //Queue the COMPONENT_INIT function to be called by the event loop
    event_QueueComponentInit(_devMode_COMPONENT_INIT);
}


#ifdef __cplusplus
}
#endif
