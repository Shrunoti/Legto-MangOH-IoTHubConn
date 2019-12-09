

/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */

/**
 * @page c_port Port Service
 *
 * @ref le_port_interface.h "API Reference"
 *
 * This service manages a list of serial links (physical or emulated). It also manages the
 * link modes (AT command and data mode).
 *
 * @section le_port_binding IPC interfaces binding
 *
 * All the functions of this API are provided by the @b portService.
 *
 * Here's a code sample binding to the port service:
 * @verbatim
   bindings:
   {
      portTest.portTestComp.le_port -> portService.le_port
   }
   @endverbatim
 *
 * @section port_request Request Device
 *
 * le_port_Request() must be called to open a configured device. If the device was not opened, it
 * opens the device.
 *
 * @section port_SwitchMode Switch Mode
 *
 * le_port_SetDataMode() must be called to switch the device into data mode.
 * le_port_SetCommandMode() must be called to switch the device into command mode.
 *
 * @section port_Release Release Device
 *
 * le_port_Release() must be called to release the device.
 *
 * @section port_config Configuration
 *
 * @todo Configuration documentation to be written.
 *
 * @section port_GetPortReference Get Port Reference
 *
 * le_port_GetPortReference() must be called to get the port object reference regarding to a given
 * reference coming from the AT server.
 *
 * @todo The port service is not implemented yet.
 *
 * <HR>
 *
 * Copyright (C) Sierra Wireless Inc.
 */
/**
 * @file le_port_interface.h
 *
 * Legato @ref c_port include file.
 *
 * Copyright (C) Sierra Wireless Inc.
 */
/**
 * Include le_atServer.api for atServer type definitions.
 */

#ifndef LE_PORT_INTERFACE_H_INCLUDE_GUARD
#define LE_PORT_INTERFACE_H_INCLUDE_GUARD


#include "legato.h"

// Interface specific includes
#include "le_atServer_interface.h"


//--------------------------------------------------------------------------------------------------
/**
 * Type for handler called when a server disconnects.
 */
//--------------------------------------------------------------------------------------------------
typedef void (*le_port_DisconnectHandler_t)(void *);

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
void le_port_ConnectService
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
le_result_t le_port_TryConnectService
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
void le_port_SetServerDisconnectHandler
(
    le_port_DisconnectHandler_t disconnectHandler,
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
void le_port_DisconnectService
(
    void
);


//--------------------------------------------------------------------------------------------------
/**
 * Maximum length of the device name.
 */
//--------------------------------------------------------------------------------------------------
#define LE_PORT_MAX_LEN_DEVICE_NAME 50

//--------------------------------------------------------------------------------------------------
/**
 * Reference type for referring to the device.
 */
//--------------------------------------------------------------------------------------------------
typedef struct le_port_Device* le_port_DeviceRef_t;


//--------------------------------------------------------------------------------------------------
/**
 * This function requests to open a configured device. If the device was not opened, it opens
 * the device.
 *
 * @return
 *      - Reference to the device.
 *      - NULL if the device is not available.
 */
//--------------------------------------------------------------------------------------------------
le_port_DeviceRef_t le_port_Request
(
    const char* LE_NONNULL deviceName
        ///< [IN] Device name to be requested.
);

//--------------------------------------------------------------------------------------------------
/**
 * This function switches the device into data mode.
 *
 * @return
 *      - LE_OK            Function succeeded.
 *      - LE_FAULT         Function failed.
 *      - LE_BAD_PARAMETER Invalid parameter.
 *      - LE_UNAVAILABLE   JSON parsing is not completed.
 *      - LE_DUPLICATE     Device already opened in data mode.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_port_SetDataMode
(
    le_port_DeviceRef_t devRef,
        ///< [IN] Device reference.
    int* fdPtr
        ///< [OUT] File descriptor of the device.
);

//--------------------------------------------------------------------------------------------------
/**
 * This function switches the device into AT command mode and returns AT server device reference.
 *
 * @return
 *      - LE_OK            Function succeeded.
 *      - LE_FAULT         Function failed.
 *      - LE_BAD_PARAMETER Invalid parameter.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_port_SetCommandMode
(
    le_port_DeviceRef_t devRef,
        ///< [IN] Device reference.
    le_atServer_DeviceRef_t* atServerDevRefPtr
        ///< [OUT] AT server device reference.
);

//--------------------------------------------------------------------------------------------------
/**
 * This function closes the device and releases the resources.
 *
 * @return
 *      - LE_OK            Function succeeded.
 *      - LE_FAULT         Function failed.
 *      - LE_BAD_PARAMETER Invalid parameter.
 *      - LE_UNAVAILABLE   JSON parsing is not completed.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_port_Release
(
    le_port_DeviceRef_t devRef
        ///< [IN] Device reference.
);

//--------------------------------------------------------------------------------------------------
/**
 * This function gets the device reference regarding to a given reference coming from the AT server.
 *
 * @return
 *      - LE_OK            Function succeeded.
 *      - LE_FAULT         Function failed.
 *      - LE_BAD_PARAMETER Invalid parameter.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_port_GetPortReference
(
    le_atServer_DeviceRef_t atServerDevRef,
        ///< [IN] Device reference from AT server.
    le_port_DeviceRef_t* devRefPtr
        ///< [OUT] Device reference from port service.
);

#endif // LE_PORT_INTERFACE_H_INCLUDE_GUARD