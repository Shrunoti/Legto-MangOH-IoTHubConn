/*
 * AUTO-GENERATED _componentMain.c for the cm component.

 * Don't bother hand-editing this file.
 */

#include "legato.h"
#include "../liblegato/eventLoop.h"
#include "../liblegato/log.h"

#ifdef __cplusplus
extern "C" {
#endif

extern const char* _cm_le_cfg_ServiceInstanceName;
const char** le_cfg_ServiceInstanceNamePtr = &_cm_le_cfg_ServiceInstanceName;
void le_cfg_ConnectService(void);
extern const char* _cm_le_ulpm_ServiceInstanceName;
const char** le_ulpm_ServiceInstanceNamePtr = &_cm_le_ulpm_ServiceInstanceName;
void le_ulpm_ConnectService(void);
extern const char* _cm_le_mrc_ServiceInstanceName;
const char** le_mrc_ServiceInstanceNamePtr = &_cm_le_mrc_ServiceInstanceName;
void le_mrc_ConnectService(void);
extern const char* _cm_le_mdc_ServiceInstanceName;
const char** le_mdc_ServiceInstanceNamePtr = &_cm_le_mdc_ServiceInstanceName;
void le_mdc_ConnectService(void);
extern const char* _cm_le_sim_ServiceInstanceName;
const char** le_sim_ServiceInstanceNamePtr = &_cm_le_sim_ServiceInstanceName;
void le_sim_ConnectService(void);
extern const char* _cm_le_sms_ServiceInstanceName;
const char** le_sms_ServiceInstanceNamePtr = &_cm_le_sms_ServiceInstanceName;
void le_sms_ConnectService(void);
extern const char* _cm_le_info_ServiceInstanceName;
const char** le_info_ServiceInstanceNamePtr = &_cm_le_info_ServiceInstanceName;
void le_info_ConnectService(void);
extern const char* _cm_le_adc_ServiceInstanceName;
const char** le_adc_ServiceInstanceNamePtr = &_cm_le_adc_ServiceInstanceName;
void le_adc_ConnectService(void);
extern const char* _cm_le_temp_ServiceInstanceName;
const char** le_temp_ServiceInstanceNamePtr = &_cm_le_temp_ServiceInstanceName;
void le_temp_ConnectService(void);
extern const char* _cm_le_ips_ServiceInstanceName;
const char** le_ips_ServiceInstanceNamePtr = &_cm_le_ips_ServiceInstanceName;
void le_ips_ConnectService(void);
extern const char* _cm_le_rtc_ServiceInstanceName;
const char** le_rtc_ServiceInstanceNamePtr = &_cm_le_rtc_ServiceInstanceName;
void le_rtc_ConnectService(void);
extern const char* _cm_le_cellnet_ServiceInstanceName;
const char** le_cellnet_ServiceInstanceNamePtr = &_cm_le_cellnet_ServiceInstanceName;
void le_cellnet_ConnectService(void);
// Component log session variables.
le_log_SessionRef_t cm_LogSession;
le_log_Level_t* cm_LogLevelFilterPtr;

// Component initialization function (COMPONENT_INIT).
void _cm_COMPONENT_INIT(void);

// Library initialization function.
// Will be called by the dynamic linker loader when the library is loaded.
__attribute__((constructor)) void _cm_Init(void)
{
    LE_DEBUG("Initializing cm component library.");

    // Connect client-side IPC interfaces.
    le_cfg_ConnectService();
    // 'le_ulpm' is [manual-start].
    le_mrc_ConnectService();
    le_mdc_ConnectService();
    le_sim_ConnectService();
    le_sms_ConnectService();
    le_info_ConnectService();
    le_adc_ConnectService();
    le_temp_ConnectService();
    le_ips_ConnectService();
    le_rtc_ConnectService();
    le_cellnet_ConnectService();

    // Register the component with the Log Daemon.
    cm_LogSession = log_RegComponent("cm", &cm_LogLevelFilterPtr);

    //Queue the COMPONENT_INIT function to be called by the event loop
    event_QueueComponentInit(_cm_COMPONENT_INIT);
}


#ifdef __cplusplus
}
#endif
