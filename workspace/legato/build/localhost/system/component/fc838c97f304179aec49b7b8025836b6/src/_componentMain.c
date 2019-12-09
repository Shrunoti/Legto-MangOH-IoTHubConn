/*
 * AUTO-GENERATED _componentMain.c for the watchdogChain component.

 * Don't bother hand-editing this file.
 */

#include "legato.h"
#include "../liblegato/eventLoop.h"
#include "../liblegato/log.h"

#ifdef __cplusplus
extern "C" {
#endif

extern const char* _watchdogChain_le_wdog_ServiceInstanceName;
const char** le_wdog_ServiceInstanceNamePtr = &_watchdogChain_le_wdog_ServiceInstanceName;
void le_wdog_ConnectService(void);
// Component log session variables.
le_log_SessionRef_t watchdogChain_LogSession;
le_log_Level_t* watchdogChain_LogLevelFilterPtr;

// Component initialization function (COMPONENT_INIT).
void _watchdogChain_COMPONENT_INIT(void);

// Library initialization function.
// Will be called by the dynamic linker loader when the library is loaded.
__attribute__((constructor)) void _watchdogChain_Init(void)
{
    LE_DEBUG("Initializing watchdogChain component library.");

    // Connect client-side IPC interfaces.
    // 'le_wdog' is [manual-start].

    // Register the component with the Log Daemon.
    watchdogChain_LogSession = log_RegComponent("watchdogChain", &watchdogChain_LogLevelFilterPtr);

    //Queue the COMPONENT_INIT function to be called by the event loop
    event_QueueComponentInit(_watchdogChain_COMPONENT_INIT);
}


#ifdef __cplusplus
}
#endif
