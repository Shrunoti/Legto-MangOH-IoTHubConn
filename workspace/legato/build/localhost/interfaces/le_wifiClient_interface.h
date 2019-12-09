

/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */

/**
 * @page c_le_wifi_client WiFi Client Service
 *
 * @ref le_wifiClient_interface.h "API Reference"
 *
 * <HR>
 *
 * This API provides WiFi Client setup.
 * Please note that the WiFi Client cannot be used at the same time as the WiFi Access Points
 * service, due to the sharing of same wifi hardware.
 *
 * @section le_wifi_binding IPC interfaces binding
 *
 *
 * Here's a code sample binding to WiFi service:
 * @verbatim
   bindings:
   {
      clientExe.clientComponent.le_wifiClient -> wifiService.le_wifiClient
   }
   @endverbatim
 *
 * @section le_wifiClient_Start Starting the WiFi Client
 *
 * First of all the function le_wifiClient_Start() must be called to start the WiFi Service.
 * - le_wifiClient_Start(): returns LE_OK if the call went ok.
 *   If WiFi Access Point is active, this will fail.
 *
 *
 * To subscribe to wifi events le_wifiClient_AddNewEventHandler() is to be called.
 * - le_wifiClient_AddNewEventHandler(): returns the handler reference if the call went ok.
 *
 * @code
 *
 * static void EventHandler
 * (
 *     le_wifiClient_Event_t clientEvent,
 *     void *contextPtr
 * )
 * {
 *     switch( clientEvent )
 *     {
 *          case LE_WIFICLIENT_EVENT_CONNECTED:
 *          {
 *              LE_INFO("WiFi Client Connected.");
 *          }
 *          break;
 *          case LE_WIFICLIENT_EVENT_DISCONNECTED:
 *          {
 *              LE_INFO("WiFi Client Disconnected.");
 *          }
 *          break;
 *          case LE_WIFICLIENT_EVENT_SCAN_DONE:
 *          {
 *              LE_INFO("WiFi Client Scan is done.");
 *              MyHandleScanResult();
 *          }
 *          break;
 *     }
 * }
 *
 * le_wifiClient_NewEventHandler WiFiEventHandlerRef = NULL;
 *
 * static void MyInit
 * (
 *     void
 * )
 * {
 *     le_result_t result = le_wifiClient_start();
 *
 *     if ( LE_OK == result )
 *     {
 *         LE_INFO("WiFi Client started.");
 *         WiFiEventHandlerRef = le_wifiClient_AddNewEventHandler( EventHandler, NULL );
 *     }
 *     else if ( LE_BUSY == result )
 *     {
 *         LE_INFO("ERROR: WiFi Client already started.");
 *     }
 *     else
 *     {
 *         LE_INFO("ERROR: WiFi Client not started.");
 *     }
 *
 * }
 *
 * @endcode
 *
 *
 * @section le_wifiClient_scan Scanning Access Points with WiFi Client
 *
 * To start a scan for Access Points, the le_wifiClient_Scan() should be called.
 * - le_wifiClient_Scan(): returns the LE_OK if the call went ok.
 *
 *
 * @section le_wifiClient_scan_result Processing the WiFi scan results
 *
 * Once the scan results are available, the event LE_WIFICLIENT_EVENT_SCAN_DONE is received.
 * The found Access Points can then be gotten with
 * - le_wifiClient_GetFirstAccessPoint():  returns the Access Point if found. Else NULL.
 * - le_wifiClient_GetNextAccessPoint(): returns the next Access Point if found. Else NULL.
 *
 * The Access Points SSID, Service Set Identifier, is not a string.
 * It does however often contain human readable ASCII values.
 * It can be read with the following function:
 * - le_wifiClient_GetSsid() : returns the LE_OK if the SSID was read ok.
 *
 * The Access Points signal strength can be read with the following function:
 * - le_wifiClient_GetSignalStrength() : returns the signal strength in dBm of the AccessPoint
 *
 * @code
 *
 * static void MyHandleScanResult
 * (
 *     void
 * )
 * {
 *     uint8 ssid[MAX_SSID_BYTES];
 *     le_wifiClient_AccessPointRef_t accessPointRef = le_wifiClient_GetFirstAccessPoint();
 *
 *     while( NULL != accessPointRef )
 *     {
 *          result = le_wifiClient_GetSsid( accessPointRef, ssid, MAX_SSID_BYTES );
 *          if (( result == LE_OK ) && ( memcmp( ssid, "MySSID", 6) == 0 ))
 *          {
 *               LE_INFO("WiFi Client found.");
 *               break;
 *          }
 *          accessPointRef = le_wifiClient_GetNextAccessPoint();
 *     }
 * }
 *
 * @endcode
 *
 * @section le_wifiClient_connect_to_ap Connecting to Access Point
 *
 * First of all, an Access Point reference should be created using the SSID of the target Access
 * Point. Use the following function to create a reference:
 * - le_wifiClient_Create(): returns Access Point reference
 *
 * To set the pass phrase prior for the Access Point use the function:
 * - le_wifiClient_SetPassphrase(): returns the function execution status.
 *
 * WPA-Enterprise requires a username and password to authenticate.
 * To set them use the function:
 * - le_wifiClient_SetUserCredentials():  returns the function execution status.
 *
 * If an Access Point is hidden, it does not announce its presence and will not show up in scan.
 * So, the SSID of this Access Point must be known in advance. Then, use the following function to
 * allow connections to hidden Access Points:
 * le_wifiClient_SetHiddenNetworkAttribute(): returns the function execution status.
 *
 * Finally and when the Access Point parameters have been configured, use the following function to
 * attempt a connection:
 * - le_wifiClient_Connect():  returns the function execution status.
 *
 * @code
 *
 * static void MyConnectTo
 * (
 *     le_wifiClient_AccessPointRef_t accessPointRef
 * )
 * {
 *     le_result_t result;
 *     le_wifiClient_SetPassphrase ( accessPointRef, "Secret1" );
 *     result = le_wifiClient_Connect( accessPointRef );
 *     if (result == LE_OK)
 *     {
 *          LE_INFO("Connecting to AP.");
 *     }
 * }
 *
 * @endcode
 *
 * <HR>
 *
 * Copyright (C) Sierra Wireless Inc.
 */
/**
 * @file le_wifiClient_interface.h
 *
 * Legato @ref c_le_wifi_client include file.
 *
 * Copyright (C) Sierra Wireless Inc.
 */

#ifndef LE_WIFICLIENT_INTERFACE_H_INCLUDE_GUARD
#define LE_WIFICLIENT_INTERFACE_H_INCLUDE_GUARD


#include "legato.h"

// Interface specific includes
#include "le_wifiDefs_interface.h"


//--------------------------------------------------------------------------------------------------
/**
 * Type for handler called when a server disconnects.
 */
//--------------------------------------------------------------------------------------------------
typedef void (*le_wifiClient_DisconnectHandler_t)(void *);

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
void le_wifiClient_ConnectService
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
le_result_t le_wifiClient_TryConnectService
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
void le_wifiClient_SetServerDisconnectHandler
(
    le_wifiClient_DisconnectHandler_t disconnectHandler,
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
void le_wifiClient_DisconnectService
(
    void
);


//--------------------------------------------------------------------------------------------------
/**
 * Reference type for AccessPoint that is returned by the WiFi Scan.
 */
//--------------------------------------------------------------------------------------------------
typedef struct le_wifiClient_AccessPoint* le_wifiClient_AccessPointRef_t;


//--------------------------------------------------------------------------------------------------
/**
 * WiFi Client Events.
 */
//--------------------------------------------------------------------------------------------------
typedef enum
{
    LE_WIFICLIENT_EVENT_CONNECTED = 0,
        ///< WiFi Client Connected
    LE_WIFICLIENT_EVENT_DISCONNECTED = 1,
        ///< WiFi Client Disconnected
    LE_WIFICLIENT_EVENT_SCAN_DONE = 2,
        ///< WiFi Scan result for available Access Points available
    LE_WIFICLIENT_EVENT_SCAN_FAILED = 3
        ///< WiFi Scan failed
}
le_wifiClient_Event_t;


//--------------------------------------------------------------------------------------------------
/**
 * WiFi Client Security Protocol for connection
 */
//--------------------------------------------------------------------------------------------------
typedef enum
{
    LE_WIFICLIENT_SECURITY_NONE = 0,
        ///< no security.
    LE_WIFICLIENT_SECURITY_WEP = 1,
        ///< Using WEP.
    LE_WIFICLIENT_SECURITY_WPA_PSK_PERSONAL = 2,
        ///< Using WPA
    LE_WIFICLIENT_SECURITY_WPA2_PSK_PERSONAL = 3,
        ///< Using WPA2
    LE_WIFICLIENT_SECURITY_WPA_EAP_PEAP0_ENTERPRISE = 4,
        ///< Using WPA Enterprise
    LE_WIFICLIENT_SECURITY_WPA2_EAP_PEAP0_ENTERPRISE = 5
        ///< Using WPA2 Enterprise
}
le_wifiClient_SecurityProtocol_t;


//--------------------------------------------------------------------------------------------------
/**
 * Reference type used by Add/Remove functions for EVENT 'le_wifiClient_NewEvent'
 */
//--------------------------------------------------------------------------------------------------
typedef struct le_wifiClient_NewEventHandler* le_wifiClient_NewEventHandlerRef_t;


//--------------------------------------------------------------------------------------------------
/**
 * Handler for WiFi Client changes
 */
//--------------------------------------------------------------------------------------------------
typedef void (*le_wifiClient_NewEventHandlerFunc_t)
(
    le_wifiClient_Event_t event,
        ///< Handles the wifi events
    void* contextPtr
        ///<
);

//--------------------------------------------------------------------------------------------------
/**
 * Add handler function for EVENT 'le_wifiClient_NewEvent'
 *
 * This event provide information on WiFi Client event changes.
 *
 */
//--------------------------------------------------------------------------------------------------
le_wifiClient_NewEventHandlerRef_t le_wifiClient_AddNewEventHandler
(
    le_wifiClient_NewEventHandlerFunc_t handlerPtr,
        ///< [IN]
    void* contextPtr
        ///< [IN]
);

//--------------------------------------------------------------------------------------------------
/**
 * Remove handler function for EVENT 'le_wifiClient_NewEvent'
 */
//--------------------------------------------------------------------------------------------------
void le_wifiClient_RemoveNewEventHandler
(
    le_wifiClient_NewEventHandlerRef_t handlerRef
        ///< [IN]
);

//--------------------------------------------------------------------------------------------------
/**
 * This function starts the WIFI device.
 *
 * @return
 *      - LE_FAULT if the function failed.
 *      - LE_BUSY if the WIFI device is already started.
 *      - LE_OK if the function succeeded.
 *
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_wifiClient_Start
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * This function stops the WIFI device.
 *
 * @return
 *      - LE_FAULT if the function failed.
 *      - LE_DUPLICATE if the WIFI device is already stopped.
 *      - LE_OK if the function succeeded.
 *
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_wifiClient_Stop
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * Start Scanning for WiFi Access points
 * Will result in event LE_WIFICLIENT_EVENT_SCAN_DONE when the scan results are available.
 *
 * @return
 *      - LE_FAULT if the function failed.
 *      - LE_OK if the function succeeded.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_wifiClient_Scan
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * Get the first WiFi Access Point found.
 * Will return the Access Points in the order of found.
 *
 * @return
 *      - WiFi Access Point reference if ok.
 *      - NULL if no Access Point reference available.
 */
//--------------------------------------------------------------------------------------------------
le_wifiClient_AccessPointRef_t le_wifiClient_GetFirstAccessPoint
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * Get the next WiFi Access Point.
 * Will return the Access Points in the order of found.
 * This function must be called in the same context as the GetFirstAccessPoint
 *
 * @return
 *      - WiFi Access Point reference if ok.
 *      - NULL if no Access Point reference available.
 */
//--------------------------------------------------------------------------------------------------
le_wifiClient_AccessPointRef_t le_wifiClient_GetNextAccessPoint
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * Get the signal strength of the AccessPoint
 *
 * @return
 *      - signal strength in dBm. Example -30 = -30dBm
 *      - if no signal available it will return OxFFFF
 */
//--------------------------------------------------------------------------------------------------
int16_t le_wifiClient_GetSignalStrength
(
    le_wifiClient_AccessPointRef_t accessPointRef
        ///< [IN] WiFi Access Point reference.
);

//--------------------------------------------------------------------------------------------------
/**
 * Get the Basic Service set identifier (BSSID) of the AccessPoint
 *
 * @return
 *      - LE_FAULT if the function failed.
 *      - LE_BAD_PARAMETER if some parameter is invalid.
 *      - LE_OK if the function succeeded.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_wifiClient_GetBssid
(
    le_wifiClient_AccessPointRef_t accessPointRef,
        ///< [IN] WiFi Access Point reference.
    char* bssid,
        ///< [OUT] The BSSID
    size_t bssidSize
        ///< [IN]
);

//--------------------------------------------------------------------------------------------------
/**
 * Get the Service set identification (SSID) of the AccessPoint
 * @note that the SSID does not have to be human readable ASCII values, but often has.
 *
 * @return
 *      - LE_FAULT if the function failed.
 *      - LE_BAD_PARAMETER if some parameter is invalid.
 *      - LE_OK if the function succeeded.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_wifiClient_GetSsid
(
    le_wifiClient_AccessPointRef_t accessPointRef,
        ///< [IN] WiFi Access Point reference.
    uint8_t* ssidPtr,
        ///< [OUT] The SSID returned as a octet array.
    size_t* ssidSizePtr
        ///< [INOUT]
);

//--------------------------------------------------------------------------------------------------
/**
 * Set the passphrase used to generate the PSK.
 *
 * @note the difference between le_wifiClient_SetPreSharedKey() and this function
 *
 * @return
 *      - LE_FAULT if the function failed.
 *      - LE_BAD_PARAMETER if parameter is invalid.
 *      - LE_OK if the function succeeded.
 *
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_wifiClient_SetPassphrase
(
    le_wifiClient_AccessPointRef_t accessPointRef,
        ///< [IN] WiFi Access Point reference.
    const char* LE_NONNULL PassPhrase
        ///< [IN] pass-phrase for PSK
);

//--------------------------------------------------------------------------------------------------
/**
 * Set the Pre Shared Key, PSK.
 * @note the difference between le_wifiClient_SetPassphrase() and this function
 *
 * @return
 *      - LE_FAULT if the function failed.
 *      - LE_BAD_PARAMETER if parameter is invalid.
 *      - LE_OK if the function succeeded.
 *
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_wifiClient_SetPreSharedKey
(
    le_wifiClient_AccessPointRef_t accessPointRef,
        ///< [IN] WiFi Access Point reference.
    const char* LE_NONNULL PreSharedKey
        ///< [IN] PSK. Note the difference between PSK and Pass Phrase.
);

//--------------------------------------------------------------------------------------------------
/**
 * Set the security protocol for connection
 *
 * @return
 *      - LE_FAULT if the function failed.
 *      - LE_BAD_PARAMETER if parameter is invalid.
 *      - LE_OK if the function succeeded.
 *
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_wifiClient_SetSecurityProtocol
(
    le_wifiClient_AccessPointRef_t accessPointRef,
        ///< [IN] WiFi Access Point reference.
    le_wifiClient_SecurityProtocol_t securityProtocol
        ///< [IN] Security Mode
);

//--------------------------------------------------------------------------------------------------
/**
 * WPA-Enterprise requires a username and password to authenticate.
 * This function sets these parameters.
 *
 * @return
 *      - LE_FAULT if the function failed.
 *      - LE_BAD_PARAMETER if parameter is invalid.
 *      - LE_OK if the function succeeded.
 *
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_wifiClient_SetUserCredentials
(
    le_wifiClient_AccessPointRef_t accessPointRef,
        ///< [IN] WiFi Access Point reference.
    const char* LE_NONNULL userName,
        ///< [IN] UserName used for WPA-Enterprise.
    const char* LE_NONNULL password
        ///< [IN] Password used for WPA-Enterprise.
);

//--------------------------------------------------------------------------------------------------
/**
 * Set the WEP key (WEP)
 *
 * @return
 *      - LE_FAULT if the function failed.
 *      - LE_OK if the function succeeded.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_wifiClient_SetWepKey
(
    le_wifiClient_AccessPointRef_t accessPointRef,
        ///< [IN] WiFi Access Point reference.
    const char* LE_NONNULL wepKey
        ///< [IN] The WEP key
);

//--------------------------------------------------------------------------------------------------
/**
 * This function specifies whether the target Access Point is hiding its presence from clients or
 * not. When an Access Point is hidden, it cannot be discovered by a scan process.
 *
 * @note By default, this attribute is not set which means that the client is unable to connect to
 * a hidden access point. When enabled, the client will be able to connect to the access point
 * whether it is hidden or not.
 *
 * @return
 *      - LE_BAD_PARAMETER if parameter is invalid.
 *      - LE_OK if the function succeeded.
 *
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_wifiClient_SetHiddenNetworkAttribute
(
    le_wifiClient_AccessPointRef_t accessPointRef,
        ///< [IN] WiFi Access Point reference.
    bool hidden
        ///< [IN] If TRUE, the WIFI client will be able to connect to a hidden access point.
);

//--------------------------------------------------------------------------------------------------
/**
 * This function creates a reference to an Access Point given its SSID.
 * If an Access Point is hidden, it will not show up in the scan. So, its SSID must be known
 * in advance in order to create a reference.
 *
 * @note This function fails if called while scan is running.
 *
 * @return AccessPoint reference to the current
 */
//--------------------------------------------------------------------------------------------------
le_wifiClient_AccessPointRef_t le_wifiClient_Create
(
    const uint8_t* SsidPtr,
        ///< [IN] The SSID as a octet array.
    size_t SsidSize
        ///< [IN]
);

//--------------------------------------------------------------------------------------------------
/**
 * Deletes an accessPointRef.
 *
 * @note The handle becomes invalid after it has been deleted.
 * @return
 *      - LE_BAD_PARAMETER if accessPointRef was not found.
 *      - LE_BUSY if the function was called during a scan.
 *      - LE_OK if the function succeeded.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_wifiClient_Delete
(
    le_wifiClient_AccessPointRef_t accessPointRef
        ///< [IN] WiFi Access Point reference.
);

//--------------------------------------------------------------------------------------------------
/**
 * Connect to the WiFi Access Point.
 * All authentication must be set prior to calling this function.
 *
 * @return
 *      - LE_BAD_PARAMETER if parameter is invalid.
 *      - LE_OK if the function succeeded.
 *
 * @note For PSK credentials see le_wifiClient_SetPassphrase() or le_wifiClient_SetPreSharedKey() .
 * @note For WPA-Enterprise credentials see le_wifiClient_SetUserCredentials()
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_wifiClient_Connect
(
    le_wifiClient_AccessPointRef_t accessPointRef
        ///< [IN] WiFi Access Point reference.
);

//--------------------------------------------------------------------------------------------------
/**
 * Disconnect from the current connected WiFi Access Point.
 *
 * @return
 *      - LE_FAULT if the function failed.
 *      - LE_OK if the function succeeded.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_wifiClient_Disconnect
(
    void
);

#endif // LE_WIFICLIENT_INTERFACE_H_INCLUDE_GUARD