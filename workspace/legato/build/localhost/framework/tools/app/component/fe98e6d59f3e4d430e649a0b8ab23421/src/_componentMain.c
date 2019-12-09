/*
 * AUTO-GENERATED _componentMain.c for the appCtrl component.

 * Don't bother hand-editing this file.
 */

#include "legato.h"
#include "../liblegato/eventLoop.h"
#include "../liblegato/log.h"

#ifdef __cplusplus
extern "C" {
#endif

extern const char* _appCtrl_le_appCtrl_ServiceInstanceName;
const char** le_appCtrl_ServiceInstanceNamePtr = &_appCtrl_le_appCtrl_ServiceInstanceName;
void le_appCtrl_ConnectService(void);
extern const char* _appCtrl_le_framework_ServiceInstanceName;
const char** le_framework_ServiceInstanceNamePtr = &_appCtrl_le_framework_ServiceInstanceName;
void le_framework_ConnectService(void);
extern const char* _appCtrl_le_appInfo_ServiceInstanceName;
const char** le_appInfo_ServiceInstanceNamePtr = &_appCtrl_le_appInfo_ServiceInstanceName;
void le_appInfo_ConnectService(void);
extern const char* _appCtrl_le_cfg_ServiceInstanceName;
const char** le_cfg_ServiceInstanceNamePtr = &_appCtrl_le_cfg_ServiceInstanceName;
void le_cfg_ConnectService(void);
extern const char* _appCtrl_le_appProc_ServiceInstanceName;
const char** le_appProc_ServiceInstanceNamePtr = &_appCtrl_le_appProc_ServiceInstanceName;
void le_appProc_ConnectService(void);
// Component log session variables.
le_log_SessionRef_t appCtrl_LogSession;
le_log_Level_t* appCtrl_LogLevelFilterPtr;

// Component initialization function (COMPONENT_INIT).
void _appCtrl_COMPONENT_INIT(void);

// Library initialization function.
// Will be called by the dynamic linker loader when the library is loaded.
__attribute__((constructor)) void _appCtrl_Init(void)
{
    LE_DEBUG("Initializing appCtrl component library.");

    // Connect client-side IPC interfaces.
    // 'le_appCtrl' is [manual-start].
    // 'le_framework' is [manual-start].
    // 'le_appInfo' is [manual-start].
    // 'le_cfg' is [manual-start].
    // 'le_appProc' is [manual-start].

    // Register the component with the Log Daemon.
    appCtrl_LogSession = log_RegComponent("appCtrl", &appCtrl_LogLevelFilterPtr);

    //Queue the COMPONENT_INIT function to be called by the event loop
    event_QueueComponentInit(_appCtrl_COMPONENT_INIT);
}


#ifdef __cplusplus
}
#endif
