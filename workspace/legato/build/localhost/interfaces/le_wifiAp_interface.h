

/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */

/**
 * @page c_le_wifi_ap WiFi Access Point Service
 *
 * @ref le_wifiAp_interface.h "API Reference"
 *
 * <HR>
 *
 * This WiFi Service API provides Wifi Access Point setup.
 * Please note that if there is only one wifi hardware the WiFi Access Point
 * cannot be used at the same time as the WiFi Client service.
 *
 * @section le_wifi_binding IPC interfaces binding
 *
 *
 * Here's a code sample binding to WiFi service:
 * @verbatim
   bindings:
   {
      clientExe.clientComponent.le_wifiAp -> wifiService.le_wifiAp
   }
   @endverbatim
 *
 *
 *
 * @section le_wifiAp_connect_to_ap Setting parameters for the Access Point
 *
 * Note that these parameters must be set before the access point is started.
 * See each function for its default value.
 *
 * To set the SSID for the Access Point use the following function:
 * - le_wifiAp_SetSsid()
 *
 * To set the pass phrase prior for the Access Point use the function:
 * Also see le_wifiAp_SetPreSharedKey().
 * - le_wifiAp_SetPassPhrase()
 *
 * Instead of setting the pass phrase, the Pre Shared Key (PSK), can be set explicitly.
 * To set the Pre Shared Key prior for the Access Point start use the function:
 * - le_wifiAp_SetPreSharedKey()
 *
 * Sets the security protocol to use.
 * - le_wifiAp_SetSecurityProtocol()
 *
 * Sets is the Access Point should show up in network scans or not.
 * - le_wifiAp_SetDiscoverable()
 *
 * Sets which channel to use.
 * - le_wifiAp_SetChannel()
 *
 *
 * @snippet "apps/sample/wifiApTest/wifiApTestComponent/wifiApTest.c" SetCred
 *
 * @section le_wifiAp_Start Starting the WiFi Access Point
 *
 * The WiFi Access Point is started with the function le_wifiAp_Start(). Unless values have been changed, default values will be used:
 * - le_wifiAp_Start():
 *
 * To subscribe to wifi events le_wifiAp_AddNewEventHandler() is to be called.
 * - le_wifiAp_AddNewEventHandler()
 *
 *
 * @snippet "apps/sample/wifiApTest/wifiApTestComponent/wifiApTest.c"  Subscribe
 *
 * <HR>
 *
 * Copyright (C) Sierra Wireless Inc.
 */
/**
 * @file le_wifiAp_interface.h
 *
 * Legato @ref c_le_wifiAp include file.
 *
 * Copyright (C) Sierra Wireless Inc.
 */

#ifndef LE_WIFIAP_INTERFACE_H_INCLUDE_GUARD
#define LE_WIFIAP_INTERFACE_H_INCLUDE_GUARD


#include "legato.h"

// Interface specific includes
#include "le_wifiDefs_interface.h"


//--------------------------------------------------------------------------------------------------
/**
 * Type for handler called when a server disconnects.
 */
//--------------------------------------------------------------------------------------------------
typedef void (*le_wifiAp_DisconnectHandler_t)(void *);

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
void le_wifiAp_ConnectService
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
le_result_t le_wifiAp_TryConnectService
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
void le_wifiAp_SetServerDisconnectHandler
(
    le_wifiAp_DisconnectHandler_t disconnectHandler,
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
void le_wifiAp_DisconnectService
(
    void
);


//--------------------------------------------------------------------------------------------------
/**
 * WiFi Access Point Events.
 */
//--------------------------------------------------------------------------------------------------
typedef enum
{
    LE_WIFIAP_EVENT_CLIENT_CONNECTED = 0,
        ///< Client connected to WiFi Access Point.
    LE_WIFIAP_EVENT_CLIENT_DISCONNECTED = 1
        ///< Client disconnected from WiFi Access Point.
}
le_wifiAp_Event_t;


//--------------------------------------------------------------------------------------------------
/**
 * WiFi Access Point Security levels.
 */
//--------------------------------------------------------------------------------------------------
typedef enum
{
    LE_WIFIAP_SECURITY_NONE = 0,
        ///< WiFi Access Point is open and has no password.
    LE_WIFIAP_SECURITY_WPA2 = 1
        ///< WiFi Access Point has WPA2 activated.
}
le_wifiAp_SecurityProtocol_t;


//--------------------------------------------------------------------------------------------------
/**
 * Reference type used by Add/Remove functions for EVENT 'le_wifiAp_NewEvent'
 */
//--------------------------------------------------------------------------------------------------
typedef struct le_wifiAp_NewEventHandler* le_wifiAp_NewEventHandlerRef_t;


//--------------------------------------------------------------------------------------------------
/**
 * Handler for WiFi Access Point changes
 */
//--------------------------------------------------------------------------------------------------
typedef void (*le_wifiAp_EventHandlerFunc_t)
(
    le_wifiAp_Event_t event,
        ///< Handles the wifi events
    void* contextPtr
        ///<
);

//--------------------------------------------------------------------------------------------------
/**
 * Add handler function for EVENT 'le_wifiAp_NewEvent'
 *
 * These events provide information on WiFi Access Point
 *
 */
//--------------------------------------------------------------------------------------------------
le_wifiAp_NewEventHandlerRef_t le_wifiAp_AddNewEventHandler
(
    le_wifiAp_EventHandlerFunc_t handlerPtr,
        ///< [IN]
    void* contextPtr
        ///< [IN]
);

//--------------------------------------------------------------------------------------------------
/**
 * Remove handler function for EVENT 'le_wifiAp_NewEvent'
 */
//--------------------------------------------------------------------------------------------------
void le_wifiAp_RemoveNewEventHandler
(
    le_wifiAp_NewEventHandlerRef_t handlerRef
        ///< [IN]
);

//--------------------------------------------------------------------------------------------------
/**
 * This function starts the WIFI Access Point.
 * @note that all settings, if to be used, such as security, username, password must set prior to
 * starting the Access Point.
 *
 * @return
 *      - LE_FAULT if the function failed.
 *      - LE_OK if the function succeeded.
 *
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_wifiAp_Start
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * This function stops the WIFI Access Point.
 *
 * @return
 *      - LE_FAULT if the function failed.
 *      - LE_OK if the function succeeded.
 *
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_wifiAp_Stop
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * Set the Service set identification (SSID) of the AccessPoint
 * Default value is "LEGATO Access Point"
 * @note that the SSID does not have to be human readable ASCII values, but often has.
 *
 * @return
 *      - LE_BAD_PARAMETER if some parameter is invalid.
 *      - LE_OK if the function succeeded.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_wifiAp_SetSsid
(
    const uint8_t* ssidPtr,
        ///< [IN] The SSID to set as a octet array.
    size_t ssidSize
        ///< [IN]
);

//--------------------------------------------------------------------------------------------------
/**
 * Set the Security protocol to use.
 * Default value is SECURITY_WPA2.
 * @note that the SSID does not have to be human readable ASCII values, but often has.
 *
 * @return
 *      - LE_BAD_PARAMETER if some parameter is invalid.
 *      - LE_OK if the function succeeded.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_wifiAp_SetSecurityProtocol
(
    le_wifiAp_SecurityProtocol_t securityProtocol
        ///< [IN] The security protocol to use.
);

//--------------------------------------------------------------------------------------------------
/**
 * Set the passphrase used to generate the PSK.
 *
 * @note If the PSK is to be set directly, please use le_wifiAp_SetPreSharedKey()
 *
 * @return
 *      - LE_BAD_PARAMETER if parameter is invalid.
 *      - LE_OK if the function succeeded.
 *
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_wifiAp_SetPassPhrase
(
    const char* LE_NONNULL passPhrase
        ///< [IN] pass-phrase for PSK
);

//--------------------------------------------------------------------------------------------------
/**
 * Set the Pre Shared Key, PSK.
 * There is no default value, since le_wifiAp_SetPassPhrase is used as default.
 * @note the difference between le_wifiAp_SetPassPhrase() and this function
 *
 * @return
 *      - LE_BAD_PARAMETER if parameter is invalid.
 *      - LE_OK if the function succeeded.
 *
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_wifiAp_SetPreSharedKey
(
    const char* LE_NONNULL preSharedKey
        ///< [IN] PSK. Note the difference between PSK and Pass Phrase.
);

//--------------------------------------------------------------------------------------------------
/**
 * Set if the Access Point should announce its presence.
 * Default value is TRUE.
 * If the value is set to FALSE, the Access Point will be hidden.
 *
 * @return LE_OK if the function succeeded.
 *
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_wifiAp_SetDiscoverable
(
    bool discoverable
        ///< [IN] If TRUE the Access Point shows up on scans, else it is hidden.
);

//--------------------------------------------------------------------------------------------------
/**
 * Set which WiFi Channel to use.
 * Default value is 1.
 * Some legal restrictions for values 12 - 14 might apply for your region.
 *
 * @return
 *      - LE_OUT_OF_RANGE if requested channel number is out of range.
 *      - LE_OK if the function succeeded.
 *
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_wifiAp_SetChannel
(
    int8_t channelNumber
        ///< [IN] the channel number must be between 1 and 14.
);

//--------------------------------------------------------------------------------------------------
/**
 * Set number of maximally allowed clients to connect to the Access Point at the same time.
 *
 * @return
 *      - LE_OUT_OF_RANGE if requested number of users exceeds the capabilities of the Access Point.
 *      - LE_OK if the function succeeded.
 *
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_wifiAp_SetMaxNumberOfClients
(
    int8_t maxNumberOfClient
        ///< [IN] the maximum number of clients
);

//--------------------------------------------------------------------------------------------------
/**
 * Defines the IP adresses range for the host AP.
 *
 * @return
 *      - LE_BAD_PARAMETER if at least one of the given IP addresses is invalid.
 *      - LE_FAULT if a system call failed.
 *      - LE_OK if the function succeeded.
 *
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_wifiAp_SetIpRange
(
    const char* LE_NONNULL ipAp,
        ///< [IN] the IP address of the Access Point.
    const char* LE_NONNULL ipStart,
        ///< [IN] the start IP address of the Access Point.
    const char* LE_NONNULL ipStop
        ///< [IN] the stop IP address of the Access Point.
);

#endif // LE_WIFIAP_INTERFACE_H_INCLUDE_GUARD