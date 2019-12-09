/*
 * AUTO-GENERATED _componentMain.c for the watchdogDaemon component.

 * Don't bother hand-editing this file.
 */

#include "legato.h"
#include "../liblegato/eventLoop.h"
#include "../liblegato/log.h"

#ifdef __cplusplus
extern "C" {
#endif

extern const char* _watchdogDaemon_le_cfg_ServiceInstanceName;
const char** le_cfg_ServiceInstanceNamePtr = &_watchdogDaemon_le_cfg_ServiceInstanceName;
void le_cfg_ConnectService(void);
extern const char* _watchdogDaemon_le_instStat_ServiceInstanceName;
const char** le_instStat_ServiceInstanceNamePtr = &_watchdogDaemon_le_instStat_ServiceInstanceName;
void le_instStat_ConnectService(void);
extern const char* _watchdogDaemon_le_appInfo_ServiceInstanceName;
const char** le_appInfo_ServiceInstanceNamePtr = &_watchdogDaemon_le_appInfo_ServiceInstanceName;
void le_appInfo_ConnectService(void);
extern const char* _watchdogDaemon_configTreeWdog_ServiceInstanceName;
const char** configTreeWdog_ServiceInstanceNamePtr = &_watchdogDaemon_configTreeWdog_ServiceInstanceName;
void configTreeWdog_ConnectService(void);
extern const char* _watchdogDaemon_logDaemonWdog_ServiceInstanceName;
const char** logDaemonWdog_ServiceInstanceNamePtr = &_watchdogDaemon_logDaemonWdog_ServiceInstanceName;
void logDaemonWdog_ConnectService(void);
extern const char* _watchdogDaemon_updateDaemonWdog_ServiceInstanceName;
const char** updateDaemonWdog_ServiceInstanceNamePtr = &_watchdogDaemon_updateDaemonWdog_ServiceInstanceName;
void updateDaemonWdog_ConnectService(void);
extern const char* _watchdogDaemon_wdog_ServiceInstanceName;
const char** wdog_ServiceInstanceNamePtr = &_watchdogDaemon_wdog_ServiceInstanceName;
void wdog_ConnectService(void);
extern const char* _watchdogDaemon_supervisorWdog_ServiceInstanceName;
const char** supervisorWdog_ServiceInstanceNamePtr = &_watchdogDaemon_supervisorWdog_ServiceInstanceName;
void supervisorWdog_ConnectService(void);
extern const char* _watchdogDaemon_le_wdog_ServiceInstanceName;
const char** le_wdog_ServiceInstanceNamePtr = &_watchdogDaemon_le_wdog_ServiceInstanceName;
void le_wdog_AdvertiseService(void);
// Component log session variables.
le_log_SessionRef_t watchdogDaemon_LogSession;
le_log_Level_t* watchdogDaemon_LogLevelFilterPtr;

// Component initialization function (COMPONENT_INIT).
void _watchdogDaemon_COMPONENT_INIT(void);

// Library initialization function.
// Will be called by the dynamic linker loader when the library is loaded.
__attribute__((constructor)) void _watchdogDaemon_Init(void)
{
    LE_DEBUG("Initializing watchdogDaemon component library.");

    // Advertise server-side IPC interfaces.
    le_wdog_AdvertiseService();

    // Connect client-side IPC interfaces.
    le_cfg_ConnectService();
    le_instStat_ConnectService();
    // 'le_appInfo' is [manual-start].
    configTreeWdog_ConnectService();
    logDaemonWdog_ConnectService();
    updateDaemonWdog_ConnectService();
    // 'wdog' is [manual-start].
    // 'supervisorWdog' is [manual-start].

    // Register the component with the Log Daemon.
    watchdogDaemon_LogSession = log_RegComponent("watchdogDaemon", &watchdogDaemon_LogLevelFilterPtr);

    //Queue the COMPONENT_INIT function to be called by the event loop
    event_QueueComponentInit(_watchdogDaemon_COMPONENT_INIT);
}


#ifdef __cplusplus
}
#endif
