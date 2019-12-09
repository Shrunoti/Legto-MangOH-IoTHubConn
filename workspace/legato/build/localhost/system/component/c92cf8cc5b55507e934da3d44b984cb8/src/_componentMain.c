/*
 * AUTO-GENERATED _componentMain.c for the appCfg component.

 * Don't bother hand-editing this file.
 */

#include "legato.h"
#include "../liblegato/eventLoop.h"
#include "../liblegato/log.h"

#ifdef __cplusplus
extern "C" {
#endif

extern const char* _appCfg_le_cfg_ServiceInstanceName;
const char** le_cfg_ServiceInstanceNamePtr = &_appCfg_le_cfg_ServiceInstanceName;
void le_cfg_ConnectService(void);
// Component log session variables.
le_log_SessionRef_t appCfg_LogSession;
le_log_Level_t* appCfg_LogLevelFilterPtr;

// Component initialization function (COMPONENT_INIT).
void _appCfg_COMPONENT_INIT(void);

// Library initialization function.
// Will be called by the dynamic linker loader when the library is loaded.
__attribute__((constructor)) void _appCfg_Init(void)
{
    LE_DEBUG("Initializing appCfg component library.");

    // Connect client-side IPC interfaces.
    le_cfg_ConnectService();

    // Register the component with the Log Daemon.
    appCfg_LogSession = log_RegComponent("appCfg", &appCfg_LogLevelFilterPtr);

    //Queue the COMPONENT_INIT function to be called by the event loop
    event_QueueComponentInit(_appCfg_COMPONENT_INIT);
}


#ifdef __cplusplus
}
#endif
