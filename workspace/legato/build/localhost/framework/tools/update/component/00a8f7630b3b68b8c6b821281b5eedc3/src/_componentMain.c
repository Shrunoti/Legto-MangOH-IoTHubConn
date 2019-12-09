/*
 * AUTO-GENERATED _componentMain.c for the update component.

 * Don't bother hand-editing this file.
 */

#include "legato.h"
#include "../liblegato/eventLoop.h"
#include "../liblegato/log.h"

#ifdef __cplusplus
extern "C" {
#endif

extern const char* _update_le_update_ServiceInstanceName;
const char** le_update_ServiceInstanceNamePtr = &_update_le_update_ServiceInstanceName;
void le_update_ConnectService(void);
extern const char* _update_le_updateCtrl_ServiceInstanceName;
const char** le_updateCtrl_ServiceInstanceNamePtr = &_update_le_updateCtrl_ServiceInstanceName;
void le_updateCtrl_ConnectService(void);
extern const char* _update_le_appRemove_ServiceInstanceName;
const char** le_appRemove_ServiceInstanceNamePtr = &_update_le_appRemove_ServiceInstanceName;
void le_appRemove_ConnectService(void);
// Component log session variables.
le_log_SessionRef_t update_LogSession;
le_log_Level_t* update_LogLevelFilterPtr;

// Component initialization function (COMPONENT_INIT).
void _update_COMPONENT_INIT(void);

// Library initialization function.
// Will be called by the dynamic linker loader when the library is loaded.
__attribute__((constructor)) void _update_Init(void)
{
    LE_DEBUG("Initializing update component library.");

    // Connect client-side IPC interfaces.
    // 'le_update' is [manual-start].
    // 'le_updateCtrl' is [manual-start].
    // 'le_appRemove' is [manual-start].

    // Register the component with the Log Daemon.
    update_LogSession = log_RegComponent("update", &update_LogLevelFilterPtr);

    //Queue the COMPONENT_INIT function to be called by the event loop
    event_QueueComponentInit(_update_COMPONENT_INIT);
}


#ifdef __cplusplus
}
#endif
