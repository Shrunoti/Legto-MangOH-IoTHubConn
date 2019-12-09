/*
 * AUTO-GENERATED _componentMain.c for the fwupdateDaemon component.

 * Don't bother hand-editing this file.
 */

#include "legato.h"
#include "../liblegato/eventLoop.h"
#include "../liblegato/log.h"

#ifdef __cplusplus
extern "C" {
#endif

extern const char* _fwupdateDaemon_le_fwupdate_ServiceInstanceName;
const char** le_fwupdate_ServiceInstanceNamePtr = &_fwupdateDaemon_le_fwupdate_ServiceInstanceName;
void le_fwupdate_AdvertiseService(void);
extern const char* _fwupdateDaemon_le_dualsys_ServiceInstanceName;
const char** le_dualsys_ServiceInstanceNamePtr = &_fwupdateDaemon_le_dualsys_ServiceInstanceName;
void le_dualsys_AdvertiseService(void);
extern const char* _fwupdateDaemon_le_flash_ServiceInstanceName;
const char** le_flash_ServiceInstanceNamePtr = &_fwupdateDaemon_le_flash_ServiceInstanceName;
void le_flash_AdvertiseService(void);
// Component log session variables.
le_log_SessionRef_t fwupdateDaemon_LogSession;
le_log_Level_t* fwupdateDaemon_LogLevelFilterPtr;

// Component initialization function (COMPONENT_INIT).
void _fwupdateDaemon_COMPONENT_INIT(void);

// Library initialization function.
// Will be called by the dynamic linker loader when the library is loaded.
__attribute__((constructor)) void _fwupdateDaemon_Init(void)
{
    LE_DEBUG("Initializing fwupdateDaemon component library.");

    // Advertise server-side IPC interfaces.
    le_fwupdate_AdvertiseService();
    le_dualsys_AdvertiseService();
    le_flash_AdvertiseService();

    // Register the component with the Log Daemon.
    fwupdateDaemon_LogSession = log_RegComponent("fwupdateDaemon", &fwupdateDaemon_LogLevelFilterPtr);

    //Queue the COMPONENT_INIT function to be called by the event loop
    event_QueueComponentInit(_fwupdateDaemon_COMPONENT_INIT);
}


#ifdef __cplusplus
}
#endif
