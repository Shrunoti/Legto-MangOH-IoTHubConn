/*
 * AUTO-GENERATED _componentMain.c for the configTree component.

 * Don't bother hand-editing this file.
 */

#include "legato.h"
#include "../liblegato/eventLoop.h"
#include "../liblegato/log.h"

#ifdef __cplusplus
extern "C" {
#endif

extern const char* _configTree_le_cfg_ServiceInstanceName;
const char** le_cfg_ServiceInstanceNamePtr = &_configTree_le_cfg_ServiceInstanceName;
void le_cfg_AdvertiseService(void);
extern const char* _configTree_le_cfgAdmin_ServiceInstanceName;
const char** le_cfgAdmin_ServiceInstanceNamePtr = &_configTree_le_cfgAdmin_ServiceInstanceName;
void le_cfgAdmin_AdvertiseService(void);
extern const char* _configTree_configTreeWdog_ServiceInstanceName;
const char** configTreeWdog_ServiceInstanceNamePtr = &_configTree_configTreeWdog_ServiceInstanceName;
void configTreeWdog_AdvertiseService(void);
// Component log session variables.
le_log_SessionRef_t configTree_LogSession;
le_log_Level_t* configTree_LogLevelFilterPtr;

// Component initialization function (COMPONENT_INIT).
void _configTree_COMPONENT_INIT(void);

// Library initialization function.
// Will be called by the dynamic linker loader when the library is loaded.
__attribute__((constructor)) void _configTree_Init(void)
{
    LE_DEBUG("Initializing configTree component library.");

    // Advertise server-side IPC interfaces.
    le_cfg_AdvertiseService();
    le_cfgAdmin_AdvertiseService();
    configTreeWdog_AdvertiseService();

    // Register the component with the Log Daemon.
    configTree_LogSession = log_RegComponent("configTree", &configTree_LogLevelFilterPtr);

    //Queue the COMPONENT_INIT function to be called by the event loop
    event_QueueComponentInit(_configTree_COMPONENT_INIT);
}


#ifdef __cplusplus
}
#endif
