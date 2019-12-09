/*
 * AUTO-GENERATED _componentMain.c for the sdirTool component.

 * Don't bother hand-editing this file.
 */

#include "legato.h"
#include "../liblegato/eventLoop.h"
#include "../liblegato/log.h"

#ifdef __cplusplus
extern "C" {
#endif

extern const char* _sdirTool_le_cfg_ServiceInstanceName;
const char** le_cfg_ServiceInstanceNamePtr = &_sdirTool_le_cfg_ServiceInstanceName;
void le_cfg_ConnectService(void);
// Component log session variables.
le_log_SessionRef_t sdirTool_LogSession;
le_log_Level_t* sdirTool_LogLevelFilterPtr;

// Component initialization function (COMPONENT_INIT).
void _sdirTool_COMPONENT_INIT(void);

// Library initialization function.
// Will be called by the dynamic linker loader when the library is loaded.
__attribute__((constructor)) void _sdirTool_Init(void)
{
    LE_DEBUG("Initializing sdirTool component library.");

    // Connect client-side IPC interfaces.
    // 'le_cfg' is [manual-start].

    // Register the component with the Log Daemon.
    sdirTool_LogSession = log_RegComponent("sdirTool", &sdirTool_LogLevelFilterPtr);

    //Queue the COMPONENT_INIT function to be called by the event loop
    event_QueueComponentInit(_sdirTool_COMPONENT_INIT);
}


#ifdef __cplusplus
}
#endif
