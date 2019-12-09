/*
 * AUTO-GENERATED _componentMain.c for the modemDaemon component.

 * Don't bother hand-editing this file.
 */

#include "legato.h"
#include "../liblegato/eventLoop.h"
#include "../liblegato/log.h"

#ifdef __cplusplus
extern "C" {
#endif

extern const char* _modemDaemon_le_cfg_ServiceInstanceName;
const char** le_cfg_ServiceInstanceNamePtr = &_modemDaemon_le_cfg_ServiceInstanceName;
void le_cfg_ConnectService(void);
extern const char* _modemDaemon_le_pm_ServiceInstanceName;
const char** le_pm_ServiceInstanceNamePtr = &_modemDaemon_le_pm_ServiceInstanceName;
void le_pm_ConnectService(void);
extern const char* _modemDaemon_le_info_ServiceInstanceName;
const char** le_info_ServiceInstanceNamePtr = &_modemDaemon_le_info_ServiceInstanceName;
void le_info_AdvertiseService(void);
extern const char* _modemDaemon_le_sms_ServiceInstanceName;
const char** le_sms_ServiceInstanceNamePtr = &_modemDaemon_le_sms_ServiceInstanceName;
void le_sms_AdvertiseService(void);
extern const char* _modemDaemon_le_mrc_ServiceInstanceName;
const char** le_mrc_ServiceInstanceNamePtr = &_modemDaemon_le_mrc_ServiceInstanceName;
void le_mrc_AdvertiseService(void);
extern const char* _modemDaemon_le_sim_ServiceInstanceName;
const char** le_sim_ServiceInstanceNamePtr = &_modemDaemon_le_sim_ServiceInstanceName;
void le_sim_AdvertiseService(void);
extern const char* _modemDaemon_le_mdc_ServiceInstanceName;
const char** le_mdc_ServiceInstanceNamePtr = &_modemDaemon_le_mdc_ServiceInstanceName;
void le_mdc_AdvertiseService(void);
extern const char* _modemDaemon_le_mcc_ServiceInstanceName;
const char** le_mcc_ServiceInstanceNamePtr = &_modemDaemon_le_mcc_ServiceInstanceName;
void le_mcc_AdvertiseService(void);
extern const char* _modemDaemon_le_ecall_ServiceInstanceName;
const char** le_ecall_ServiceInstanceNamePtr = &_modemDaemon_le_ecall_ServiceInstanceName;
void le_ecall_AdvertiseService(void);
extern const char* _modemDaemon_le_ips_ServiceInstanceName;
const char** le_ips_ServiceInstanceNamePtr = &_modemDaemon_le_ips_ServiceInstanceName;
void le_ips_AdvertiseService(void);
extern const char* _modemDaemon_le_temp_ServiceInstanceName;
const char** le_temp_ServiceInstanceNamePtr = &_modemDaemon_le_temp_ServiceInstanceName;
void le_temp_AdvertiseService(void);
extern const char* _modemDaemon_le_antenna_ServiceInstanceName;
const char** le_antenna_ServiceInstanceNamePtr = &_modemDaemon_le_antenna_ServiceInstanceName;
void le_antenna_AdvertiseService(void);
extern const char* _modemDaemon_le_riPin_ServiceInstanceName;
const char** le_riPin_ServiceInstanceNamePtr = &_modemDaemon_le_riPin_ServiceInstanceName;
void le_riPin_AdvertiseService(void);
extern const char* _modemDaemon_le_adc_ServiceInstanceName;
const char** le_adc_ServiceInstanceNamePtr = &_modemDaemon_le_adc_ServiceInstanceName;
void le_adc_AdvertiseService(void);
extern const char* _modemDaemon_le_rtc_ServiceInstanceName;
const char** le_rtc_ServiceInstanceNamePtr = &_modemDaemon_le_rtc_ServiceInstanceName;
void le_rtc_AdvertiseService(void);
extern const char* _modemDaemon_le_mdmCfg_ServiceInstanceName;
const char** le_mdmCfg_ServiceInstanceNamePtr = &_modemDaemon_le_mdmCfg_ServiceInstanceName;
void le_mdmCfg_AdvertiseService(void);
extern const char* _modemDaemon_le_lpt_ServiceInstanceName;
const char** le_lpt_ServiceInstanceNamePtr = &_modemDaemon_le_lpt_ServiceInstanceName;
void le_lpt_AdvertiseService(void);
// Component log session variables.
le_log_SessionRef_t modemDaemon_LogSession;
le_log_Level_t* modemDaemon_LogLevelFilterPtr;

// Component initialization function (COMPONENT_INIT).
void _modemDaemon_COMPONENT_INIT(void);

// Library initialization function.
// Will be called by the dynamic linker loader when the library is loaded.
__attribute__((constructor)) void _modemDaemon_Init(void)
{
    LE_DEBUG("Initializing modemDaemon component library.");

    // Advertise server-side IPC interfaces.
    le_info_AdvertiseService();
    le_sms_AdvertiseService();
    le_mrc_AdvertiseService();
    le_sim_AdvertiseService();
    le_mdc_AdvertiseService();
    le_mcc_AdvertiseService();
    le_ecall_AdvertiseService();
    le_ips_AdvertiseService();
    le_temp_AdvertiseService();
    le_antenna_AdvertiseService();
    le_riPin_AdvertiseService();
    le_adc_AdvertiseService();
    le_rtc_AdvertiseService();
    le_mdmCfg_AdvertiseService();
    le_lpt_AdvertiseService();

    // Connect client-side IPC interfaces.
    le_cfg_ConnectService();
    le_pm_ConnectService();

    // Register the component with the Log Daemon.
    modemDaemon_LogSession = log_RegComponent("modemDaemon", &modemDaemon_LogLevelFilterPtr);

    //Queue the COMPONENT_INIT function to be called by the event loop
    event_QueueComponentInit(_modemDaemon_COMPONENT_INIT);
}


#ifdef __cplusplus
}
#endif
