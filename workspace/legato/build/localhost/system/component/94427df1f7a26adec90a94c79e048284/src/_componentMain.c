/*
 * AUTO-GENERATED _componentMain.c for the avcDaemon component.

 * Don't bother hand-editing this file.
 */

#include "legato.h"
#include "../liblegato/eventLoop.h"
#include "../liblegato/log.h"

#ifdef __cplusplus
extern "C" {
#endif

extern const char* _avcDaemon_le_instStat_ServiceInstanceName;
const char** le_instStat_ServiceInstanceNamePtr = &_avcDaemon_le_instStat_ServiceInstanceName;
void le_instStat_ConnectService(void);
extern const char* _avcDaemon_le_appRemove_ServiceInstanceName;
const char** le_appRemove_ServiceInstanceNamePtr = &_avcDaemon_le_appRemove_ServiceInstanceName;
void le_appRemove_ConnectService(void);
extern const char* _avcDaemon_le_appCtrl_ServiceInstanceName;
const char** le_appCtrl_ServiceInstanceNamePtr = &_avcDaemon_le_appCtrl_ServiceInstanceName;
void le_appCtrl_ConnectService(void);
extern const char* _avcDaemon_le_cfg_ServiceInstanceName;
const char** le_cfg_ServiceInstanceNamePtr = &_avcDaemon_le_cfg_ServiceInstanceName;
void le_cfg_ConnectService(void);
extern const char* _avcDaemon_le_data_ServiceInstanceName;
const char** le_data_ServiceInstanceNamePtr = &_avcDaemon_le_data_ServiceInstanceName;
void le_data_ConnectService(void);
extern const char* _avcDaemon_le_ulpm_ServiceInstanceName;
const char** le_ulpm_ServiceInstanceNamePtr = &_avcDaemon_le_ulpm_ServiceInstanceName;
void le_ulpm_ConnectService(void);
extern const char* _avcDaemon_le_info_ServiceInstanceName;
const char** le_info_ServiceInstanceNamePtr = &_avcDaemon_le_info_ServiceInstanceName;
void le_info_ConnectService(void);
extern const char* _avcDaemon_le_ips_ServiceInstanceName;
const char** le_ips_ServiceInstanceNamePtr = &_avcDaemon_le_ips_ServiceInstanceName;
void le_ips_ConnectService(void);
extern const char* _avcDaemon_le_mdc_ServiceInstanceName;
const char** le_mdc_ServiceInstanceNamePtr = &_avcDaemon_le_mdc_ServiceInstanceName;
void le_mdc_ConnectService(void);
extern const char* _avcDaemon_le_mrc_ServiceInstanceName;
const char** le_mrc_ServiceInstanceNamePtr = &_avcDaemon_le_mrc_ServiceInstanceName;
void le_mrc_ConnectService(void);
extern const char* _avcDaemon_le_sim_ServiceInstanceName;
const char** le_sim_ServiceInstanceNamePtr = &_avcDaemon_le_sim_ServiceInstanceName;
void le_sim_ConnectService(void);
extern const char* _avcDaemon_le_sms_ServiceInstanceName;
const char** le_sms_ServiceInstanceNamePtr = &_avcDaemon_le_sms_ServiceInstanceName;
void le_sms_ConnectService(void);
extern const char* _avcDaemon_le_temp_ServiceInstanceName;
const char** le_temp_ServiceInstanceNamePtr = &_avcDaemon_le_temp_ServiceInstanceName;
void le_temp_ConnectService(void);
extern const char* _avcDaemon_le_gnss_ServiceInstanceName;
const char** le_gnss_ServiceInstanceNamePtr = &_avcDaemon_le_gnss_ServiceInstanceName;
void le_gnss_ConnectService(void);
extern const char* _avcDaemon_le_pos_ServiceInstanceName;
const char** le_pos_ServiceInstanceNamePtr = &_avcDaemon_le_pos_ServiceInstanceName;
void le_pos_ConnectService(void);
extern const char* _avcDaemon_le_appInfo_ServiceInstanceName;
const char** le_appInfo_ServiceInstanceNamePtr = &_avcDaemon_le_appInfo_ServiceInstanceName;
void le_appInfo_ConnectService(void);
extern const char* _avcDaemon_le_fwupdate_ServiceInstanceName;
const char** le_fwupdate_ServiceInstanceNamePtr = &_avcDaemon_le_fwupdate_ServiceInstanceName;
void le_fwupdate_ConnectService(void);
extern const char* _avcDaemon_le_update_ServiceInstanceName;
const char** le_update_ServiceInstanceNamePtr = &_avcDaemon_le_update_ServiceInstanceName;
void le_update_ConnectService(void);
extern const char* _avcDaemon_le_framework_ServiceInstanceName;
const char** le_framework_ServiceInstanceNamePtr = &_avcDaemon_le_framework_ServiceInstanceName;
void le_framework_ConnectService(void);
extern const char* _avcDaemon_le_secStore_ServiceInstanceName;
const char** le_secStore_ServiceInstanceNamePtr = &_avcDaemon_le_secStore_ServiceInstanceName;
void le_secStore_ConnectService(void);
extern const char* _avcDaemon_le_cellnet_ServiceInstanceName;
const char** le_cellnet_ServiceInstanceNamePtr = &_avcDaemon_le_cellnet_ServiceInstanceName;
void le_cellnet_ConnectService(void);
extern const char* _avcDaemon_le_avc_ServiceInstanceName;
const char** le_avc_ServiceInstanceNamePtr = &_avcDaemon_le_avc_ServiceInstanceName;
void le_avc_AdvertiseService(void);
extern const char* _avcDaemon_le_avdata_ServiceInstanceName;
const char** le_avdata_ServiceInstanceNamePtr = &_avcDaemon_le_avdata_ServiceInstanceName;
void le_avdata_AdvertiseService(void);
// Component log session variables.
le_log_SessionRef_t avcDaemon_LogSession;
le_log_Level_t* avcDaemon_LogLevelFilterPtr;

// Component initialization function (COMPONENT_INIT).
void _avcDaemon_COMPONENT_INIT(void);

// Library initialization function.
// Will be called by the dynamic linker loader when the library is loaded.
__attribute__((constructor)) void _avcDaemon_Init(void)
{
    LE_DEBUG("Initializing avcDaemon component library.");

    // Advertise server-side IPC interfaces.
    le_avc_AdvertiseService();
    le_avdata_AdvertiseService();

    // Connect client-side IPC interfaces.
    le_instStat_ConnectService();
    le_appRemove_ConnectService();
    le_appCtrl_ConnectService();
    le_cfg_ConnectService();
    le_data_ConnectService();
    le_ulpm_ConnectService();
    le_info_ConnectService();
    le_ips_ConnectService();
    le_mdc_ConnectService();
    le_mrc_ConnectService();
    le_sim_ConnectService();
    le_sms_ConnectService();
    le_temp_ConnectService();
    le_gnss_ConnectService();
    le_pos_ConnectService();
    le_appInfo_ConnectService();
    le_fwupdate_ConnectService();
    le_update_ConnectService();
    le_framework_ConnectService();
    le_secStore_ConnectService();
    le_cellnet_ConnectService();

    // Register the component with the Log Daemon.
    avcDaemon_LogSession = log_RegComponent("avcDaemon", &avcDaemon_LogLevelFilterPtr);

    //Queue the COMPONENT_INIT function to be called by the event loop
    event_QueueComponentInit(_avcDaemon_COMPONENT_INIT);
}


#ifdef __cplusplus
}
#endif
