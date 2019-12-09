/*
 * AUTO-GENERATED _componentMain.c for the dcsDaemon component.

 * Don't bother hand-editing this file.
 */

#include "legato.h"
#include "../liblegato/eventLoop.h"
#include "../liblegato/log.h"

#ifdef __cplusplus
extern "C" {
#endif

extern const char* _dcsDaemon_le_mdc_ServiceInstanceName;
const char** le_mdc_ServiceInstanceNamePtr = &_dcsDaemon_le_mdc_ServiceInstanceName;
void le_mdc_ConnectService(void);
extern const char* _dcsDaemon_le_mrc_ServiceInstanceName;
const char** le_mrc_ServiceInstanceNamePtr = &_dcsDaemon_le_mrc_ServiceInstanceName;
void le_mrc_ConnectService(void);
extern const char* _dcsDaemon_le_cellnet_ServiceInstanceName;
const char** le_cellnet_ServiceInstanceNamePtr = &_dcsDaemon_le_cellnet_ServiceInstanceName;
void le_cellnet_ConnectService(void);
extern const char* _dcsDaemon_le_cfg_ServiceInstanceName;
const char** le_cfg_ServiceInstanceNamePtr = &_dcsDaemon_le_cfg_ServiceInstanceName;
void le_cfg_ConnectService(void);
extern const char* _dcsDaemon_le_wifiClient_ServiceInstanceName;
const char** le_wifiClient_ServiceInstanceNamePtr = &_dcsDaemon_le_wifiClient_ServiceInstanceName;
void le_wifiClient_ConnectService(void);
extern const char* _dcsDaemon_le_data_ServiceInstanceName;
const char** le_data_ServiceInstanceNamePtr = &_dcsDaemon_le_data_ServiceInstanceName;
void le_data_AdvertiseService(void);
// Component log session variables.
le_log_SessionRef_t dcsDaemon_LogSession;
le_log_Level_t* dcsDaemon_LogLevelFilterPtr;

// Component initialization function (COMPONENT_INIT).
void _dcsDaemon_COMPONENT_INIT(void);

// Library initialization function.
// Will be called by the dynamic linker loader when the library is loaded.
__attribute__((constructor)) void _dcsDaemon_Init(void)
{
    LE_DEBUG("Initializing dcsDaemon component library.");

    // Advertise server-side IPC interfaces.
    le_data_AdvertiseService();

    // Connect client-side IPC interfaces.
    le_mdc_ConnectService();
    le_mrc_ConnectService();
    le_cellnet_ConnectService();
    le_cfg_ConnectService();
    // 'le_wifiClient' is [manual-start].

    // Register the component with the Log Daemon.
    dcsDaemon_LogSession = log_RegComponent("dcsDaemon", &dcsDaemon_LogLevelFilterPtr);

    //Queue the COMPONENT_INIT function to be called by the event loop
    event_QueueComponentInit(_dcsDaemon_COMPONENT_INIT);
}


#ifdef __cplusplus
}
#endif
