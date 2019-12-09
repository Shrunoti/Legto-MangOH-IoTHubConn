
// Startup code for the executable 'gpioService'.
// This is a generated file, do not edit.

#include "legato.h"
#include "../liblegato/eventLoop.h"
#include "../liblegato/log.h"
#include <dlfcn.h>


// Define IPC API interface names.
LE_SHARED const char* _watchdogChain_le_wdog_ServiceInstanceName = "gpioService.watchdogChain.le_wdog";
LE_SHARED const char* _sysfsGpio_le_gpioPin1_ServiceInstanceName = "le_gpioPin1";
LE_SHARED const char* _sysfsGpio_le_gpioPin2_ServiceInstanceName = "le_gpioPin2";
LE_SHARED const char* _sysfsGpio_le_gpioPin3_ServiceInstanceName = "le_gpioPin3";
LE_SHARED const char* _sysfsGpio_le_gpioPin4_ServiceInstanceName = "le_gpioPin4";
LE_SHARED const char* _sysfsGpio_le_gpioPin5_ServiceInstanceName = "le_gpioPin5";
LE_SHARED const char* _sysfsGpio_le_gpioPin6_ServiceInstanceName = "le_gpioPin6";
LE_SHARED const char* _sysfsGpio_le_gpioPin7_ServiceInstanceName = "le_gpioPin7";
LE_SHARED const char* _sysfsGpio_le_gpioPin8_ServiceInstanceName = "le_gpioPin8";
LE_SHARED const char* _sysfsGpio_le_gpioPin9_ServiceInstanceName = "le_gpioPin9";
LE_SHARED const char* _sysfsGpio_le_gpioPin10_ServiceInstanceName = "le_gpioPin10";
LE_SHARED const char* _sysfsGpio_le_gpioPin11_ServiceInstanceName = "le_gpioPin11";
LE_SHARED const char* _sysfsGpio_le_gpioPin12_ServiceInstanceName = "le_gpioPin12";
LE_SHARED const char* _sysfsGpio_le_gpioPin13_ServiceInstanceName = "le_gpioPin13";
LE_SHARED const char* _sysfsGpio_le_gpioPin14_ServiceInstanceName = "le_gpioPin14";
LE_SHARED const char* _sysfsGpio_le_gpioPin15_ServiceInstanceName = "le_gpioPin15";
LE_SHARED const char* _sysfsGpio_le_gpioPin16_ServiceInstanceName = "le_gpioPin16";
LE_SHARED const char* _sysfsGpio_le_gpioPin17_ServiceInstanceName = "le_gpioPin17";
LE_SHARED const char* _sysfsGpio_le_gpioPin18_ServiceInstanceName = "le_gpioPin18";
LE_SHARED const char* _sysfsGpio_le_gpioPin19_ServiceInstanceName = "le_gpioPin19";
LE_SHARED const char* _sysfsGpio_le_gpioPin20_ServiceInstanceName = "le_gpioPin20";
LE_SHARED const char* _sysfsGpio_le_gpioPin21_ServiceInstanceName = "le_gpioPin21";
LE_SHARED const char* _sysfsGpio_le_gpioPin22_ServiceInstanceName = "le_gpioPin22";
LE_SHARED const char* _sysfsGpio_le_gpioPin23_ServiceInstanceName = "le_gpioPin23";
LE_SHARED const char* _sysfsGpio_le_gpioPin24_ServiceInstanceName = "le_gpioPin24";
LE_SHARED const char* _sysfsGpio_le_gpioPin25_ServiceInstanceName = "le_gpioPin25";
LE_SHARED const char* _sysfsGpio_le_gpioPin26_ServiceInstanceName = "le_gpioPin26";
LE_SHARED const char* _sysfsGpio_le_gpioPin27_ServiceInstanceName = "le_gpioPin27";
LE_SHARED const char* _sysfsGpio_le_gpioPin28_ServiceInstanceName = "le_gpioPin28";
LE_SHARED const char* _sysfsGpio_le_gpioPin29_ServiceInstanceName = "le_gpioPin29";
LE_SHARED const char* _sysfsGpio_le_gpioPin30_ServiceInstanceName = "le_gpioPin30";
LE_SHARED const char* _sysfsGpio_le_gpioPin31_ServiceInstanceName = "le_gpioPin31";
LE_SHARED const char* _sysfsGpio_le_gpioPin32_ServiceInstanceName = "le_gpioPin32";
LE_SHARED const char* _sysfsGpio_le_gpioPin33_ServiceInstanceName = "le_gpioPin33";
LE_SHARED const char* _sysfsGpio_le_gpioPin34_ServiceInstanceName = "le_gpioPin34";
LE_SHARED const char* _sysfsGpio_le_gpioPin35_ServiceInstanceName = "le_gpioPin35";
LE_SHARED const char* _sysfsGpio_le_gpioPin36_ServiceInstanceName = "le_gpioPin36";
LE_SHARED const char* _sysfsGpio_le_gpioPin37_ServiceInstanceName = "le_gpioPin37";
LE_SHARED const char* _sysfsGpio_le_gpioPin38_ServiceInstanceName = "le_gpioPin38";
LE_SHARED const char* _sysfsGpio_le_gpioPin39_ServiceInstanceName = "le_gpioPin39";
LE_SHARED const char* _sysfsGpio_le_gpioPin40_ServiceInstanceName = "le_gpioPin40";
LE_SHARED const char* _sysfsGpio_le_gpioPin41_ServiceInstanceName = "le_gpioPin41";
LE_SHARED const char* _sysfsGpio_le_gpioPin42_ServiceInstanceName = "le_gpioPin42";
LE_SHARED const char* _sysfsGpio_le_gpioPin43_ServiceInstanceName = "le_gpioPin43";
LE_SHARED const char* _sysfsGpio_le_gpioPin44_ServiceInstanceName = "le_gpioPin44";
LE_SHARED const char* _sysfsGpio_le_gpioPin45_ServiceInstanceName = "le_gpioPin45";
LE_SHARED const char* _sysfsGpio_le_gpioPin46_ServiceInstanceName = "le_gpioPin46";
LE_SHARED const char* _sysfsGpio_le_gpioPin47_ServiceInstanceName = "le_gpioPin47";
LE_SHARED const char* _sysfsGpio_le_gpioPin48_ServiceInstanceName = "le_gpioPin48";
LE_SHARED const char* _sysfsGpio_le_gpioPin49_ServiceInstanceName = "le_gpioPin49";
LE_SHARED const char* _sysfsGpio_le_gpioPin50_ServiceInstanceName = "le_gpioPin50";
LE_SHARED const char* _sysfsGpio_le_gpioPin51_ServiceInstanceName = "le_gpioPin51";
LE_SHARED const char* _sysfsGpio_le_gpioPin52_ServiceInstanceName = "le_gpioPin52";
LE_SHARED const char* _sysfsGpio_le_gpioPin53_ServiceInstanceName = "le_gpioPin53";
LE_SHARED const char* _sysfsGpio_le_gpioPin54_ServiceInstanceName = "le_gpioPin54";
LE_SHARED const char* _sysfsGpio_le_gpioPin55_ServiceInstanceName = "le_gpioPin55";
LE_SHARED const char* _sysfsGpio_le_gpioPin56_ServiceInstanceName = "le_gpioPin56";
LE_SHARED const char* _sysfsGpio_le_gpioPin57_ServiceInstanceName = "le_gpioPin57";
LE_SHARED const char* _sysfsGpio_le_gpioPin58_ServiceInstanceName = "le_gpioPin58";
LE_SHARED const char* _sysfsGpio_le_gpioPin59_ServiceInstanceName = "le_gpioPin59";
LE_SHARED const char* _sysfsGpio_le_gpioPin60_ServiceInstanceName = "le_gpioPin60";
LE_SHARED const char* _sysfsGpio_le_gpioPin61_ServiceInstanceName = "le_gpioPin61";
LE_SHARED const char* _sysfsGpio_le_gpioPin62_ServiceInstanceName = "le_gpioPin62";
LE_SHARED const char* _sysfsGpio_le_gpioPin63_ServiceInstanceName = "le_gpioPin63";
LE_SHARED const char* _sysfsGpio_le_gpioPin64_ServiceInstanceName = "le_gpioPin64";
LE_SHARED const char* _sysfsGpio_le_cfg_ServiceInstanceName = "gpioService.sysfsGpio.le_cfg";

// Define default component's log session variables.
LE_SHARED le_log_SessionRef_t gpioService_exe_LogSession;
LE_SHARED le_log_Level_t* gpioService_exe_LogLevelFilterPtr;

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

    gpioService_exe_LogSession = log_RegComponent("gpioService_exe", &gpioService_exe_LogLevelFilterPtr);

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
    LoadLib("libComponent_sysfsGpio.so");

    // Set the Signal Fault handler
    le_sig_InstallShowStackHandler();

    // Set the Signal Term handler
    le_sig_InstallDefaultTermHandler();

    LE_DEBUG("== Starting Event Processing Loop ==");
    le_event_RunLoop();
    LE_FATAL("== SHOULDN'T GET HERE! ==");
}
