/*
 * AUTO-GENERATED _componentMain.c for the rSimDaemon component.

 * Don't bother hand-editing this file.
 */

#include "legato.h"
#include "../liblegato/eventLoop.h"
#include "../liblegato/log.h"

#ifdef __cplusplus
extern "C" {
#endif

extern const char* _rSimDaemon_le_rsim_ServiceInstanceName;
const char** le_rsim_ServiceInstanceNamePtr = &_rSimDaemon_le_rsim_ServiceInstanceName;
void le_rsim_AdvertiseService(void);
// Component log session variables.
le_log_SessionRef_t rSimDaemon_LogSession;
le_log_Level_t* rSimDaemon_LogLevelFilterPtr;

// Component initialization function (COMPONENT_INIT).
void _rSimDaemon_COMPONENT_INIT(void);

// Library initialization function.
// Will be called by the dynamic linker loader when the library is loaded.
__attribute__((constructor)) void _rSimDaemon_Init(void)
{
    LE_DEBUG("Initializing rSimDaemon component library.");

    // Advertise server-side IPC interfaces.
    le_rsim_AdvertiseService();

    // Register the component with the Log Daemon.
    rSimDaemon_LogSession = log_RegComponent("rSimDaemon", &rSimDaemon_LogLevelFilterPtr);

    //Queue the COMPONENT_INIT function to be called by the event loop
    event_QueueComponentInit(_rSimDaemon_COMPONENT_INIT);
}


#ifdef __cplusplus
}
#endif
