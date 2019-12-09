/*
 * AUTO-GENERATED _componentMain.c for the portDaemon component.

 * Don't bother hand-editing this file.
 */

#include "legato.h"
#include "../liblegato/eventLoop.h"
#include "../liblegato/log.h"

#ifdef __cplusplus
extern "C" {
#endif

extern const char* _portDaemon_le_atServer_ServiceInstanceName;
const char** le_atServer_ServiceInstanceNamePtr = &_portDaemon_le_atServer_ServiceInstanceName;
void le_atServer_ConnectService(void);
extern const char* _portDaemon_le_port_ServiceInstanceName;
const char** le_port_ServiceInstanceNamePtr = &_portDaemon_le_port_ServiceInstanceName;
void le_port_AdvertiseService(void);
// Component log session variables.
le_log_SessionRef_t portDaemon_LogSession;
le_log_Level_t* portDaemon_LogLevelFilterPtr;

// Component initialization function (COMPONENT_INIT).
void _portDaemon_COMPONENT_INIT(void);

// Library initialization function.
// Will be called by the dynamic linker loader when the library is loaded.
__attribute__((constructor)) void _portDaemon_Init(void)
{
    LE_DEBUG("Initializing portDaemon component library.");

    // Advertise server-side IPC interfaces.
    le_port_AdvertiseService();

    // Connect client-side IPC interfaces.
    le_atServer_ConnectService();

    // Register the component with the Log Daemon.
    portDaemon_LogSession = log_RegComponent("portDaemon", &portDaemon_LogLevelFilterPtr);

    //Queue the COMPONENT_INIT function to be called by the event loop
    event_QueueComponentInit(_portDaemon_COMPONENT_INIT);
}


#ifdef __cplusplus
}
#endif
