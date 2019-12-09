/*
 * AUTO-GENERATED _componentMain.c for the updateDaemon component.

 * Don't bother hand-editing this file.
 */

#include "legato.h"
#include "../liblegato/eventLoop.h"
#include "../liblegato/log.h"

#ifdef __cplusplus
extern "C" {
#endif

extern const char* _updateDaemon_le_cfg_ServiceInstanceName;
const char** le_cfg_ServiceInstanceNamePtr = &_updateDaemon_le_cfg_ServiceInstanceName;
void le_cfg_ConnectService(void);
extern const char* _updateDaemon_le_cfgAdmin_ServiceInstanceName;
const char** le_cfgAdmin_ServiceInstanceNamePtr = &_updateDaemon_le_cfgAdmin_ServiceInstanceName;
void le_cfgAdmin_ConnectService(void);
extern const char* _updateDaemon_le_ima_ServiceInstanceName;
const char** le_ima_ServiceInstanceNamePtr = &_updateDaemon_le_ima_ServiceInstanceName;
void le_ima_ConnectService(void);
extern const char* _updateDaemon_le_fwupdate_ServiceInstanceName;
const char** le_fwupdate_ServiceInstanceNamePtr = &_updateDaemon_le_fwupdate_ServiceInstanceName;
void le_fwupdate_ConnectService(void);
extern const char* _updateDaemon_le_appCtrl_ServiceInstanceName;
const char** le_appCtrl_ServiceInstanceNamePtr = &_updateDaemon_le_appCtrl_ServiceInstanceName;
void le_appCtrl_ConnectService(void);
extern const char* _updateDaemon_le_framework_ServiceInstanceName;
const char** le_framework_ServiceInstanceNamePtr = &_updateDaemon_le_framework_ServiceInstanceName;
void le_framework_ConnectService(void);
extern const char* _updateDaemon_le_update_ServiceInstanceName;
const char** le_update_ServiceInstanceNamePtr = &_updateDaemon_le_update_ServiceInstanceName;
void le_update_AdvertiseService(void);
extern const char* _updateDaemon_le_appRemove_ServiceInstanceName;
const char** le_appRemove_ServiceInstanceNamePtr = &_updateDaemon_le_appRemove_ServiceInstanceName;
void le_appRemove_AdvertiseService(void);
extern const char* _updateDaemon_le_instStat_ServiceInstanceName;
const char** le_instStat_ServiceInstanceNamePtr = &_updateDaemon_le_instStat_ServiceInstanceName;
void le_instStat_AdvertiseService(void);
extern const char* _updateDaemon_le_updateCtrl_ServiceInstanceName;
const char** le_updateCtrl_ServiceInstanceNamePtr = &_updateDaemon_le_updateCtrl_ServiceInstanceName;
void le_updateCtrl_AdvertiseService(void);
extern const char* _updateDaemon_updateDaemonWdog_ServiceInstanceName;
const char** updateDaemonWdog_ServiceInstanceNamePtr = &_updateDaemon_updateDaemonWdog_ServiceInstanceName;
void updateDaemonWdog_AdvertiseService(void);
// Component log session variables.
le_log_SessionRef_t updateDaemon_LogSession;
le_log_Level_t* updateDaemon_LogLevelFilterPtr;

// Component initialization function (COMPONENT_INIT).
void _updateDaemon_COMPONENT_INIT(void);

// Library initialization function.
// Will be called by the dynamic linker loader when the library is loaded.
__attribute__((constructor)) void _updateDaemon_Init(void)
{
    LE_DEBUG("Initializing updateDaemon component library.");

    // Advertise server-side IPC interfaces.
    le_update_AdvertiseService();
    le_appRemove_AdvertiseService();
    le_instStat_AdvertiseService();
    le_updateCtrl_AdvertiseService();
    updateDaemonWdog_AdvertiseService();

    // Connect client-side IPC interfaces.
    le_cfg_ConnectService();
    le_cfgAdmin_ConnectService();
    // 'le_ima' is [manual-start].
    // 'le_fwupdate' is [manual-start].
    // 'le_appCtrl' is [manual-start].
    // 'le_framework' is [manual-start].

    // Register the component with the Log Daemon.
    updateDaemon_LogSession = log_RegComponent("updateDaemon", &updateDaemon_LogLevelFilterPtr);

    //Queue the COMPONENT_INIT function to be called by the event loop
    event_QueueComponentInit(_updateDaemon_COMPONENT_INIT);
}


#ifdef __cplusplus
}
#endif
