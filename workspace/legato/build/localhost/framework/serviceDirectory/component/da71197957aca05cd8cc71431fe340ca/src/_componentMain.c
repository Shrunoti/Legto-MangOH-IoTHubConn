/*
 * AUTO-GENERATED _componentMain.c for the serviceDirectory component.

 * Don't bother hand-editing this file.
 */

#include "legato.h"
#include "../liblegato/eventLoop.h"
#include "../liblegato/log.h"

#ifdef __cplusplus
extern "C" {
#endif

// Component log session variables.
le_log_SessionRef_t serviceDirectory_LogSession;
le_log_Level_t* serviceDirectory_LogLevelFilterPtr;

// Component initialization function (COMPONENT_INIT).
void _serviceDirectory_COMPONENT_INIT(void);

// Library initialization function.
// Will be called by the dynamic linker loader when the library is loaded.
__attribute__((constructor)) void _serviceDirectory_Init(void)
{
    LE_DEBUG("Initializing serviceDirectory component library.");

    // Register the component with the Log Daemon.
    serviceDirectory_LogSession = log_RegComponent("serviceDirectory", &serviceDirectory_LogLevelFilterPtr);

    //Queue the COMPONENT_INIT function to be called by the event loop
    event_QueueComponentInit(_serviceDirectory_COMPONENT_INIT);
}


#ifdef __cplusplus
}
#endif
