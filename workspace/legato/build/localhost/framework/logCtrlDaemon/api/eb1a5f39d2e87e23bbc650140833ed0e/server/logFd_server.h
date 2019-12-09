

/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */


#ifndef LOGFD_INTERFACE_H_INCLUDE_GUARD
#define LOGFD_INTERFACE_H_INCLUDE_GUARD


#include "legato.h"

// Interface specific includes
#include "le_limit_server.h"


//--------------------------------------------------------------------------------------------------
/**
 * Get the server service reference
 */
//--------------------------------------------------------------------------------------------------
le_msg_ServiceRef_t logFd_GetServiceRef
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * Get the client session reference for the current message
 */
//--------------------------------------------------------------------------------------------------
le_msg_SessionRef_t logFd_GetClientSessionRef
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * Initialize the server and advertise the service.
 */
//--------------------------------------------------------------------------------------------------
void logFd_AdvertiseService
(
    void
);



//--------------------------------------------------------------------------------------------------
/**
 * Registers an application process' standard error for logging.  Messages from this file descriptor
 * will be logged at LE_LOG_ERR level.
 *
 * @return
 *      LE_OK if successful.
 *      LE_FAULT if there was an error.
 */
//--------------------------------------------------------------------------------------------------
void logFd_StdErr
(
    int fd,
        ///< [IN] stderr file descriptor.
    const char* LE_NONNULL appName,
        ///< [IN] Name of the application.
    const char* LE_NONNULL procName,
        ///< [IN] Name of the process.
    int32_t pid
        ///< [IN] PID of the process.
);



//--------------------------------------------------------------------------------------------------
/**
 * Registers an application process' standard out for logging.  Messages from this file descriptor
 * will be logged at LE_LOG_INFO level.
 *
 * @return
 *      LE_OK if successful.
 *      LE_FAULT if there was an error.
 */
//--------------------------------------------------------------------------------------------------
void logFd_StdOut
(
    int fd,
        ///< [IN] stdout file descriptor.
    const char* LE_NONNULL appName,
        ///< [IN] Name of the application.
    const char* LE_NONNULL procName,
        ///< [IN] Name of the process.
    int32_t pid
        ///< [IN] PID of the process.
);


#endif // LOGFD_INTERFACE_H_INCLUDE_GUARD