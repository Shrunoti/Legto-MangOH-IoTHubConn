
// Startup code for the executable 'avcDaemon'.
// This is a generated file, do not edit.

#include "legato.h"
#include "../liblegato/eventLoop.h"
#include "../liblegato/log.h"
#include <dlfcn.h>


// Define IPC API interface names.
LE_SHARED const char* _appCfg_le_cfg_ServiceInstanceName = "avcDaemon.appCfg.le_cfg";
LE_SHARED const char* _watchdogChain_le_wdog_ServiceInstanceName = "avcDaemon.watchdogChain.le_wdog";
LE_SHARED const char* _avcDaemon_le_avc_ServiceInstanceName = "le_avc";
LE_SHARED const char* _avcDaemon_le_avdata_ServiceInstanceName = "le_avdata";
LE_SHARED const char* _avcDaemon_le_instStat_ServiceInstanceName = "avcDaemon.avcDaemon.le_instStat";
LE_SHARED const char* _avcDaemon_le_appRemove_ServiceInstanceName = "avcDaemon.avcDaemon.le_appRemove";
LE_SHARED const char* _avcDaemon_le_appCtrl_ServiceInstanceName = "avcDaemon.avcDaemon.le_appCtrl";
LE_SHARED const char* _avcDaemon_le_cfg_ServiceInstanceName = "avcDaemon.avcDaemon.le_cfg";
LE_SHARED const char* _avcDaemon_le_data_ServiceInstanceName = "avcDaemon.avcDaemon.le_data";
LE_SHARED const char* _avcDaemon_le_ulpm_ServiceInstanceName = "avcDaemon.avcDaemon.le_ulpm";
LE_SHARED const char* _avcDaemon_le_info_ServiceInstanceName = "avcDaemon.avcDaemon.le_info";
LE_SHARED const char* _avcDaemon_le_ips_ServiceInstanceName = "avcDaemon.avcDaemon.le_ips";
LE_SHARED const char* _avcDaemon_le_mdc_ServiceInstanceName = "avcDaemon.avcDaemon.le_mdc";
LE_SHARED const char* _avcDaemon_le_mrc_ServiceInstanceName = "avcDaemon.avcDaemon.le_mrc";
LE_SHARED const char* _avcDaemon_le_sim_ServiceInstanceName = "avcDaemon.avcDaemon.le_sim";
LE_SHARED const char* _avcDaemon_le_sms_ServiceInstanceName = "avcDaemon.avcDaemon.le_sms";
LE_SHARED const char* _avcDaemon_le_temp_ServiceInstanceName = "avcDaemon.avcDaemon.le_temp";
LE_SHARED const char* _avcDaemon_le_gnss_ServiceInstanceName = "avcDaemon.avcDaemon.le_gnss";
LE_SHARED const char* _avcDaemon_le_pos_ServiceInstanceName = "avcDaemon.avcDaemon.le_pos";
LE_SHARED const char* _avcDaemon_le_appInfo_ServiceInstanceName = "avcDaemon.avcDaemon.le_appInfo";
LE_SHARED const char* _avcDaemon_le_fwupdate_ServiceInstanceName = "avcDaemon.avcDaemon.le_fwupdate";
LE_SHARED const char* _avcDaemon_le_update_ServiceInstanceName = "avcDaemon.avcDaemon.le_update";
LE_SHARED const char* _avcDaemon_le_framework_ServiceInstanceName = "avcDaemon.avcDaemon.le_framework";
LE_SHARED const char* _avcDaemon_le_secStore_ServiceInstanceName = "avcDaemon.avcDaemon.le_secStore";
LE_SHARED const char* _avcDaemon_le_cellnet_ServiceInstanceName = "avcDaemon.avcDaemon.le_cellnet";

// Define default component's log session variables.
LE_SHARED le_log_SessionRef_t avcDaemon_exe_LogSession;
LE_SHARED le_log_Level_t* avcDaemon_exe_LogLevelFilterPtr;

// Loads a library using dlopen().
__attribute__((unused)) static void LoadLib
(
    const char* libName
)
{
    dlopen(libName, RTLD_LAZY | RTLD_GLOBAL);
    const char* errorMsg = dlerror();
    LE_FATAL_IF(errorMsg != NULL,
                "Failed to load library '%s' (%s)",                libName,
                errorMsg);
}


int main(int argc, char* argv[])
{
    // Pass the args to the Command Line Arguments API.
    le_arg_SetArgs((size_t)argc, (char**)argv);
    // Make stdout line buffered so printf shows up in logs without flushing.
    setlinebuf(stdout);

    avcDaemon_exe_LogSession = log_RegComponent("avcDaemon_exe", &avcDaemon_exe_LogLevelFilterPtr);

    // Connect to the log control daemon.
    // Note that there are some rare cases where we don't want the
    // process to try to connect to the Log Control Daemon (e.g.,
    // the Supervisor and the Service Directory shouldn't).
    // The NO_LOG_CONTROL macro can be used to control that.
    #ifndef NO_LOG_CONTROL
        log_ConnectToControlDaemon();
    #else
        LE_DEBUG("Not connecting to the Log Control Daemon.");
    #endif

    // Load dynamic libraries.
    LoadLib("libComponent_le_pa_avc_default.so");
    LoadLib("liblwm2mcore.so");
    LoadLib("libwakaama.so");
    LoadLib("libComponent_appCfg.so");
    LoadLib("libComponent_watchdogChain.so");
    LoadLib("libComponent_avcDaemon.so");

    // Set the Signal Fault handler
    le_sig_InstallShowStackHandler();

    // Set the Signal Term handler
    le_sig_InstallDefaultTermHandler();

    LE_DEBUG("== Starting Event Processing Loop ==");
    le_event_RunLoop();
    LE_FATAL("== SHOULDN'T GET HERE! ==");
}
