/*
 * AUTO-GENERATED _componentMain.c for the sbtrace component.

 * Don't bother hand-editing this file.
 */

#include "legato.h"
#include "../liblegato/eventLoop.h"
#include "../liblegato/log.h"

#ifdef __cplusplus
extern "C" {
#endif

extern const char* _sbtrace_le_appCtrl_ServiceInstanceName;
const char** le_appCtrl_ServiceInstanceNamePtr = &_sbtrace_le_appCtrl_ServiceInstanceName;
void le_appCtrl_ConnectService(void);
// Component log session variables.
le_log_SessionRef_t sbtrace_LogSession;
le_log_Level_t* sbtrace_LogLevelFilterPtr;

// Component initialization function (COMPONENT_INIT).
void _sbtrace_COMPONENT_INIT(void);

// Library initialization function.
// Will be called by the dynamic linker loader when the library is loaded.
__attribute__((constructor)) void _sbtrace_Init(void)
{
    LE_DEBUG("Initializing sbtrace component library.");

    // Connect client-side IPC interfaces.
    // 'le_appCtrl' is [manual-start].

    // Register the component with the Log Daemon.
    sbtrace_LogSession = log_RegComponent("sbtrace", &sbtrace_LogLevelFilterPtr);

    //Queue the COMPONENT_INIT function to be called by the event loop
    event_QueueComponentInit(_sbtrace_COMPONENT_INIT);
}


#ifdef __cplusplus
}
#endif
