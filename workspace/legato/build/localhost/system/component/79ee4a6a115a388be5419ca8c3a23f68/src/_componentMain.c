/*
 * AUTO-GENERATED _componentMain.c for the atServer component.

 * Don't bother hand-editing this file.
 */

#include "legato.h"
#include "../liblegato/eventLoop.h"
#include "../liblegato/log.h"

#ifdef __cplusplus
extern "C" {
#endif

extern const char* _atServer_le_atClient_ServiceInstanceName;
const char** le_atClient_ServiceInstanceNamePtr = &_atServer_le_atClient_ServiceInstanceName;
void le_atClient_ConnectService(void);
extern const char* _atServer_le_atServer_ServiceInstanceName;
const char** le_atServer_ServiceInstanceNamePtr = &_atServer_le_atServer_ServiceInstanceName;
void le_atServer_AdvertiseService(void);
// Component log session variables.
le_log_SessionRef_t atServer_LogSession;
le_log_Level_t* atServer_LogLevelFilterPtr;

// Component initialization function (COMPONENT_INIT).
void _atServer_COMPONENT_INIT(void);

// Library initialization function.
// Will be called by the dynamic linker loader when the library is loaded.
__attribute__((constructor)) void _atServer_Init(void)
{
    LE_DEBUG("Initializing atServer component library.");

    // Advertise server-side IPC interfaces.
    le_atServer_AdvertiseService();

    // Connect client-side IPC interfaces.
    le_atClient_ConnectService();

    // Register the component with the Log Daemon.
    atServer_LogSession = log_RegComponent("atServer", &atServer_LogLevelFilterPtr);

    //Queue the COMPONENT_INIT function to be called by the event loop
    event_QueueComponentInit(_atServer_COMPONENT_INIT);
}


#ifdef __cplusplus
}
#endif
