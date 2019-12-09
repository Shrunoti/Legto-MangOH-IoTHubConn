/*
 * AUTO-GENERATED _componentMain.c for the config component.

 * Don't bother hand-editing this file.
 */

#include "legato.h"
#include "../liblegato/eventLoop.h"
#include "../liblegato/log.h"

#ifdef __cplusplus
extern "C" {
#endif

extern const char* _config_le_cfg_ServiceInstanceName;
const char** le_cfg_ServiceInstanceNamePtr = &_config_le_cfg_ServiceInstanceName;
void le_cfg_ConnectService(void);
extern const char* _config_le_cfgAdmin_ServiceInstanceName;
const char** le_cfgAdmin_ServiceInstanceNamePtr = &_config_le_cfgAdmin_ServiceInstanceName;
void le_cfgAdmin_ConnectService(void);
// Component log session variables.
le_log_SessionRef_t config_LogSession;
le_log_Level_t* config_LogLevelFilterPtr;

// Component initialization function (COMPONENT_INIT).
void _config_COMPONENT_INIT(void);

// Library initialization function.
// Will be called by the dynamic linker loader when the library is loaded.
__attribute__((constructor)) void _config_Init(void)
{
    LE_DEBUG("Initializing config component library.");

    // Connect client-side IPC interfaces.
    le_cfg_ConnectService();
    le_cfgAdmin_ConnectService();

    // Register the component with the Log Daemon.
    config_LogSession = log_RegComponent("config", &config_LogLevelFilterPtr);

    //Queue the COMPONENT_INIT function to be called by the event loop
    event_QueueComponentInit(_config_COMPONENT_INIT);
}


#ifdef __cplusplus
}
#endif
