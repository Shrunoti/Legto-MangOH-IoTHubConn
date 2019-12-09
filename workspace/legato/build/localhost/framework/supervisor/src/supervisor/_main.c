
// Startup code for the executable 'supervisor'.
// This is a generated file, do not edit.

#include "legato.h"
#include "../liblegato/eventLoop.h"
#include "../liblegato/log.h"
#include <dlfcn.h>


// Define IPC API interface names.
LE_SHARED const char* _supervisor_le_appCtrl_ServiceInstanceName = "le_appCtrl";
LE_SHARED const char* _supervisor_le_framework_ServiceInstanceName = "le_framework";
LE_SHARED const char* _supervisor_wdog_ServiceInstanceName = "wdog";
LE_SHARED const char* _supervisor_le_appInfo_ServiceInstanceName = "le_appInfo";
LE_SHARED const char* _supervisor_le_appProc_ServiceInstanceName = "le_appProc";
LE_SHARED const char* _supervisor_le_ima_ServiceInstanceName = "le_ima";
LE_SHARED const char* _supervisor_le_sup_ctrl_ServiceInstanceName = "le_sup_ctrl";
LE_SHARED const char* _supervisor_supervisorWdog_ServiceInstanceName = "supervisorWdog";
LE_SHARED const char* _supervisor_le_cfg_ServiceInstanceName = "le_cfg";
LE_SHARED const char* _supervisor_logFd_ServiceInstanceName = "logFd";
LE_SHARED const char* _supervisor_le_instStat_ServiceInstanceName = "le_instStat";

// Define default component's log session variables.
LE_SHARED le_log_SessionRef_t supervisor_exe_LogSession;
LE_SHARED le_log_Level_t* supervisor_exe_LogLevelFilterPtr;

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

    supervisor_exe_LogSession = log_RegComponent("supervisor_exe", &supervisor_exe_LogLevelFilterPtr);

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
    LoadLib("libComponent_supervisor.so");

    // Set the Signal Fault handler
    le_sig_InstallShowStackHandler();

    // Set the Signal Term handler
    le_sig_InstallDefaultTermHandler();

    LE_DEBUG("== Starting Event Processing Loop ==");
    le_event_RunLoop();
    LE_FATAL("== SHOULDN'T GET HERE! ==");
}
