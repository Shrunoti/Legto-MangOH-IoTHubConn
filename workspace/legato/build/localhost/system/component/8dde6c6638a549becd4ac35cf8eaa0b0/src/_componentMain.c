/*
 * AUTO-GENERATED _componentMain.c for the secstore component.

 * Don't bother hand-editing this file.
 */

#include "legato.h"
#include "../liblegato/eventLoop.h"
#include "../liblegato/log.h"

#ifdef __cplusplus
extern "C" {
#endif

extern const char* _secstore_secStoreAdmin_ServiceInstanceName;
const char** secStoreAdmin_ServiceInstanceNamePtr = &_secstore_secStoreAdmin_ServiceInstanceName;
void secStoreAdmin_ConnectService(void);
// Component log session variables.
le_log_SessionRef_t secstore_LogSession;
le_log_Level_t* secstore_LogLevelFilterPtr;

// Component initialization function (COMPONENT_INIT).
void _secstore_COMPONENT_INIT(void);

// Library initialization function.
// Will be called by the dynamic linker loader when the library is loaded.
__attribute__((constructor)) void _secstore_Init(void)
{
    LE_DEBUG("Initializing secstore component library.");

    // Connect client-side IPC interfaces.
    secStoreAdmin_ConnectService();

    // Register the component with the Log Daemon.
    secstore_LogSession = log_RegComponent("secstore", &secstore_LogLevelFilterPtr);

    //Queue the COMPONENT_INIT function to be called by the event loop
    event_QueueComponentInit(_secstore_COMPONENT_INIT);
}


#ifdef __cplusplus
}
#endif
