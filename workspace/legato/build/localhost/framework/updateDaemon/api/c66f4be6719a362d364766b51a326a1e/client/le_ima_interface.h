

/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */

/**
 * @page c_ima IMA Key Management API
 *
 * @ref le_ima_interface.h "API Reference"
 *
 * To run a signed executable in IMA-enabled linux, public part of the signing key (i.e. public
 * certificate) must be imported in linux keyring. For security reason, no other apps or framework
 * daemons except startSystem and supervisor has the privilege to import public certificate. This
 * API provides a way to request supervisor to do this.
 *
 * @warning This API should only be made available to privileged users (e.g. framework daemons,
 *          platformServices) as it allows to import certificate in linux keyring. Importing public
 *          certificate will allow to run the executable-files (signed by private key) in
 *          IMA-enabled linux.
 *
 * Example of importing certificate in keyring
 * @code
 *
 * #define CERT_PATH "/legato/system/current/ima_pub.cert"
 *
 * void ImportPubCert(void)
 * {
 *     le_result_t result = le_ima_ImportCert(CERT_PATH);
 *
 *     if (LE_OK != result)
 *     {
 *         LE_DEBUG("Imported certificate '%s'", CERT_PATH);
 *     }
 *     else
 *     {
 *         LE_ERROR("Failed to imported certificate '%s'", CERT_PATH);
 *     }
 * }
 * @endcode
 *
 * @section legatoSupervisorIma_binding Binding
 *
 * You can use a definition @c .adef file to bind your client-side app and component to the
 * server-side IMA key management service.
 *
 * This code sample shows how to bind an app to this service:
 * @code
 * bindings
 * {
 *     myExe.myComp.le_ima -> <root>.le_ima
 * }
 * @endcode
 *
 * See @ref defFilesAdef for details on binding APIs to an app.
 *
 * Copyright (C) Sierra Wireless Inc.
 */

#ifndef LE_IMA_INTERFACE_H_INCLUDE_GUARD
#define LE_IMA_INTERFACE_H_INCLUDE_GUARD


#include "legato.h"

// Interface specific includes
#include "le_limit_interface.h"


//--------------------------------------------------------------------------------------------------
/**
 * Type for handler called when a server disconnects.
 */
//--------------------------------------------------------------------------------------------------
typedef void (*le_ima_DisconnectHandler_t)(void *);

//--------------------------------------------------------------------------------------------------
/**
 *
 * Connect the current client thread to the service providing this API. Block until the service is
 * available.
 *
 * For each thread that wants to use this API, either ConnectService or TryConnectService must be
 * called before any other functions in this API.  Normally, ConnectService is automatically called
 * for the main thread, but not for any other thread. For details, see @ref apiFilesC_client.
 *
 * This function is created automatically.
 */
//--------------------------------------------------------------------------------------------------
void le_ima_ConnectService
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 *
 * Try to connect the current client thread to the service providing this API. Return with an error
 * if the service is not available.
 *
 * For each thread that wants to use this API, either ConnectService or TryConnectService must be
 * called before any other functions in this API.  Normally, ConnectService is automatically called
 * for the main thread, but not for any other thread. For details, see @ref apiFilesC_client.
 *
 * This function is created automatically.
 *
 * @return
 *  - LE_OK if the client connected successfully to the service.
 *  - LE_UNAVAILABLE if the server is not currently offering the service to which the client is
 *    bound.
 *  - LE_NOT_PERMITTED if the client interface is not bound to any service (doesn't have a binding).
 *  - LE_COMM_ERROR if the Service Directory cannot be reached.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_ima_TryConnectService
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * Set handler called when server disconnection is detected.
 *
 * When a server connection is lost, call this handler then exit with LE_FATAL.  If a program wants
 * to continue without exiting, it should call longjmp() from inside the handler.
 */
//--------------------------------------------------------------------------------------------------
void le_ima_SetServerDisconnectHandler
(
    le_ima_DisconnectHandler_t disconnectHandler,
    void *contextPtr
);

//--------------------------------------------------------------------------------------------------
/**
 *
 * Disconnect the current client thread from the service providing this API.
 *
 * Normally, this function doesn't need to be called. After this function is called, there's no
 * longer a connection to the service, and the functions in this API can't be used. For details, see
 * @ref apiFilesC_client.
 *
 * This function is created automatically.
 */
//--------------------------------------------------------------------------------------------------
void le_ima_DisconnectService
(
    void
);


//--------------------------------------------------------------------------------------------------
/**
 * Import a public certificate in linux keyring.
 *
 * @return
 *     - LE_OK if successful
 *     - LE_FAULT if there is a failure.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_ima_ImportCert
(
    const char* LE_NONNULL certPath
        ///< [IN] Path to the certificate to import.
);

#endif // LE_IMA_INTERFACE_H_INCLUDE_GUARD