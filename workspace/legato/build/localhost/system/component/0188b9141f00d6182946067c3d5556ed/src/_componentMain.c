/*
 * AUTO-GENERATED _componentMain.c for the pmtool component.

 * Don't bother hand-editing this file.
 */

#include "legato.h"
#include "../liblegato/eventLoop.h"
#include "../liblegato/log.h"

#ifdef __cplusplus
extern "C" {
#endif

extern const char* _pmtool_le_ulpm_ServiceInstanceName;
const char** le_ulpm_ServiceInstanceNamePtr = &_pmtool_le_ulpm_ServiceInstanceName;
void le_ulpm_ConnectService(void);
extern const char* _pmtool_le_bootReason_ServiceInstanceName;
const char** le_bootReason_ServiceInstanceNamePtr = &_pmtool_le_bootReason_ServiceInstanceName;
void le_bootReason_ConnectService(void);
extern const char* _pmtool_le_pm_ServiceInstanceName;
const char** le_pm_ServiceInstanceNamePtr = &_pmtool_le_pm_ServiceInstanceName;
void le_pm_ConnectService(void);
// Component log session variables.
le_log_SessionRef_t pmtool_LogSession;
le_log_Level_t* pmtool_LogLevelFilterPtr;

// Component initialization function (COMPONENT_INIT).
void _pmtool_COMPONENT_INIT(void);

// Library initialization function.
// Will be called by the dynamic linker loader when the library is loaded.
__attribute__((constructor)) void _pmtool_Init(void)
{
    LE_DEBUG("Initializing pmtool component library.");

    // Connect client-side IPC interfaces.
    le_ulpm_ConnectService();
    le_bootReason_ConnectService();
    le_pm_ConnectService();

    // Register the component with the Log Daemon.
    pmtool_LogSession = log_RegComponent("pmtool", &pmtool_LogLevelFilterPtr);

    //Queue the COMPONENT_INIT function to be called by the event loop
    event_QueueComponentInit(_pmtool_COMPONENT_INIT);
}


#ifdef __cplusplus
}
#endif
