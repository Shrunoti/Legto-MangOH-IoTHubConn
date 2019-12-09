/*
 * AUTO-GENERATED _componentMain.c for the fwupdate component.

 * Don't bother hand-editing this file.
 */

#include "legato.h"
#include "../liblegato/eventLoop.h"
#include "../liblegato/log.h"

#ifdef __cplusplus
extern "C" {
#endif

extern const char* _fwupdate_le_fwupdate_ServiceInstanceName;
const char** le_fwupdate_ServiceInstanceNamePtr = &_fwupdate_le_fwupdate_ServiceInstanceName;
void le_fwupdate_ConnectService(void);
// Component log session variables.
le_log_SessionRef_t fwupdate_LogSession;
le_log_Level_t* fwupdate_LogLevelFilterPtr;

// Component initialization function (COMPONENT_INIT).
void _fwupdate_COMPONENT_INIT(void);

// Library initialization function.
// Will be called by the dynamic linker loader when the library is loaded.
__attribute__((constructor)) void _fwupdate_Init(void)
{
    LE_DEBUG("Initializing fwupdate component library.");

    // Connect client-side IPC interfaces.
    // 'le_fwupdate' is [manual-start].

    // Register the component with the Log Daemon.
    fwupdate_LogSession = log_RegComponent("fwupdate", &fwupdate_LogLevelFilterPtr);

    //Queue the COMPONENT_INIT function to be called by the event loop
    event_QueueComponentInit(_fwupdate_COMPONENT_INIT);
}


#ifdef __cplusplus
}
#endif
