/*
 * AUTO-GENERATED _componentMain.c for the gnss component.

 * Don't bother hand-editing this file.
 */

#include "legato.h"
#include "../liblegato/eventLoop.h"
#include "../liblegato/log.h"

#ifdef __cplusplus
extern "C" {
#endif

extern const char* _gnss_le_gnss_ServiceInstanceName;
const char** le_gnss_ServiceInstanceNamePtr = &_gnss_le_gnss_ServiceInstanceName;
void le_gnss_ConnectService(void);
extern const char* _gnss_le_pos_ServiceInstanceName;
const char** le_pos_ServiceInstanceNamePtr = &_gnss_le_pos_ServiceInstanceName;
void le_pos_ConnectService(void);
extern const char* _gnss_le_posCtrl_ServiceInstanceName;
const char** le_posCtrl_ServiceInstanceNamePtr = &_gnss_le_posCtrl_ServiceInstanceName;
void le_posCtrl_ConnectService(void);
// Component log session variables.
le_log_SessionRef_t gnss_LogSession;
le_log_Level_t* gnss_LogLevelFilterPtr;

// Component initialization function (COMPONENT_INIT).
void _gnss_COMPONENT_INIT(void);

// Library initialization function.
// Will be called by the dynamic linker loader when the library is loaded.
__attribute__((constructor)) void _gnss_Init(void)
{
    LE_DEBUG("Initializing gnss component library.");

    // Connect client-side IPC interfaces.
    le_gnss_ConnectService();
    le_pos_ConnectService();
    le_posCtrl_ConnectService();

    // Register the component with the Log Daemon.
    gnss_LogSession = log_RegComponent("gnss", &gnss_LogLevelFilterPtr);

    //Queue the COMPONENT_INIT function to be called by the event loop
    event_QueueComponentInit(_gnss_COMPONENT_INIT);
}


#ifdef __cplusplus
}
#endif
