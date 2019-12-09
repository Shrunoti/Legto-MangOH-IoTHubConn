
// Startup code for the executable 'audioDaemon'.
// This is a generated file, do not edit.

#include "legato.h"
#include "../liblegato/eventLoop.h"
#include "../liblegato/log.h"
#include <dlfcn.h>


// Define IPC API interface names.
LE_SHARED const char* _watchdogChain_le_wdog_ServiceInstanceName = "audioDaemon.watchdogChain.le_wdog";
LE_SHARED const char* _audio_le_audio_ServiceInstanceName = "le_audio";
LE_SHARED const char* _audio_le_pm_ServiceInstanceName = "audioDaemon.audio.le_pm";

// Define default component's log session variables.
LE_SHARED le_log_SessionRef_t audioDaemon_exe_LogSession;
LE_SHARED le_log_Level_t* audioDaemon_exe_LogLevelFilterPtr;

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

    audioDaemon_exe_LogSession = log_RegComponent("audioDaemon_exe", &audioDaemon_exe_LogLevelFilterPtr);

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
    LoadLib("libComponent_le_pa_audio_default.so");
    LoadLib("libComponent_le_pa_pcm_default.so");
    LoadLib("libComponent_le_pa_amr_default.so");
    LoadLib("libComponent_watchdogChain.so");
    LoadLib("libComponent_audio.so");

    // Set the Signal Fault handler
    le_sig_InstallShowStackHandler();

    // Set the Signal Term handler
    le_sig_InstallDefaultTermHandler();

    LE_DEBUG("== Starting Event Processing Loop ==");
    le_event_RunLoop();
    LE_FATAL("== SHOULDN'T GET HERE! ==");
}
