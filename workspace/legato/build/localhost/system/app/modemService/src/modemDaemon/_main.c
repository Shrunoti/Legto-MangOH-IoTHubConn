
// Startup code for the executable 'modemDaemon'.
// This is a generated file, do not edit.

#include "legato.h"
#include "../liblegato/eventLoop.h"
#include "../liblegato/log.h"
#include <dlfcn.h>


// Define IPC API interface names.
LE_SHARED const char* _watchdogChain_le_wdog_ServiceInstanceName = "modemDaemon.watchdogChain.le_wdog";
LE_SHARED const char* _modemDaemon_le_info_ServiceInstanceName = "le_info";
LE_SHARED const char* _modemDaemon_le_sms_ServiceInstanceName = "le_sms";
LE_SHARED const char* _modemDaemon_le_mrc_ServiceInstanceName = "le_mrc";
LE_SHARED const char* _modemDaemon_le_sim_ServiceInstanceName = "le_sim";
LE_SHARED const char* _modemDaemon_le_mdc_ServiceInstanceName = "le_mdc";
LE_SHARED const char* _modemDaemon_le_mcc_ServiceInstanceName = "le_mcc";
LE_SHARED const char* _modemDaemon_le_ecall_ServiceInstanceName = "le_ecall";
LE_SHARED const char* _modemDaemon_le_ips_ServiceInstanceName = "le_ips";
LE_SHARED const char* _modemDaemon_le_temp_ServiceInstanceName = "le_temp";
LE_SHARED const char* _modemDaemon_le_antenna_ServiceInstanceName = "le_antenna";
LE_SHARED const char* _modemDaemon_le_riPin_ServiceInstanceName = "le_riPin";
LE_SHARED const char* _modemDaemon_le_adc_ServiceInstanceName = "le_adc";
LE_SHARED const char* _modemDaemon_le_rtc_ServiceInstanceName = "le_rtc";
LE_SHARED const char* _modemDaemon_le_mdmCfg_ServiceInstanceName = "le_mdmCfg";
LE_SHARED const char* _modemDaemon_le_lpt_ServiceInstanceName = "le_lpt";
LE_SHARED const char* _modemDaemon_le_cfg_ServiceInstanceName = "modemDaemon.modemDaemon.le_cfg";
LE_SHARED const char* _modemDaemon_le_pm_ServiceInstanceName = "modemDaemon.modemDaemon.le_pm";

// Define default component's log session variables.
LE_SHARED le_log_SessionRef_t modemDaemon_exe_LogSession;
LE_SHARED le_log_Level_t* modemDaemon_exe_LogLevelFilterPtr;

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

    modemDaemon_exe_LogSession = log_RegComponent("modemDaemon_exe", &modemDaemon_exe_LogLevelFilterPtr);

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
    LoadLib("libComponent_watchdogChain.so");
    LoadLib("libComponent_le_pa_default.so");
    LoadLib("libComponent_le_pa_ecall_default.so");
    LoadLib("libComponent_modemDaemon.so");

    // Set the Signal Fault handler
    le_sig_InstallShowStackHandler();

    // Set the Signal Term handler
    le_sig_InstallDefaultTermHandler();

    LE_DEBUG("== Starting Event Processing Loop ==");
    le_event_RunLoop();
    LE_FATAL("== SHOULDN'T GET HERE! ==");
}
