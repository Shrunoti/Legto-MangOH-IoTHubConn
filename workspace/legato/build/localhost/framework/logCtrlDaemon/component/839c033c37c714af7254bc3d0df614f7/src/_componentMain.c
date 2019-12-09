/*
 * AUTO-GENERATED _componentMain.c for the logDaemon component.

 * Don't bother hand-editing this file.
 */

#include "legato.h"
#include "../liblegato/eventLoop.h"
#include "../liblegato/log.h"

#ifdef __cplusplus
extern "C" {
#endif

extern const char* _logDaemon_logFd_ServiceInstanceName;
const char** logFd_ServiceInstanceNamePtr = &_logDaemon_logFd_ServiceInstanceName;
void logFd_AdvertiseService(void);
extern const char* _logDaemon_logDaemonWdog_ServiceInstanceName;
const char** logDaemonWdog_ServiceInstanceNamePtr = &_logDaemon_logDaemonWdog_ServiceInstanceName;
void logDaemonWdog_AdvertiseService(void);
// Component log session variables.
le_log_SessionRef_t logDaemon_LogSession;
le_log_Level_t* logDaemon_LogLevelFilterPtr;

// Component initialization function (COMPONENT_INIT).
void _logDaemon_COMPONENT_INIT(void);

// Library initialization function.
// Will be called by the dynamic linker loader when the library is loaded.
__attribute__((constructor)) void _logDaemon_Init(void)
{
    LE_DEBUG("Initializing logDaemon component library.");

    // Advertise server-side IPC interfaces.
    logFd_AdvertiseService();
    logDaemonWdog_AdvertiseService();

    // Register the component with the Log Daemon.
    logDaemon_LogSession = log_RegComponent("logDaemon", &logDaemon_LogLevelFilterPtr);

    //Queue the COMPONENT_INIT function to be called by the event loop
    event_QueueComponentInit(_logDaemon_COMPONENT_INIT);
}


#ifdef __cplusplus
}
#endif
