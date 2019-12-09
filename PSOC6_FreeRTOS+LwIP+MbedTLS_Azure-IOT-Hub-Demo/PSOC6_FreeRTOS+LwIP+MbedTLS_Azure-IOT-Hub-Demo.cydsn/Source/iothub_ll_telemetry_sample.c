// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

// CAVEAT: This sample is to demonstrate azure IoT client concepts only and is not a guide design principles or style
// Checking of return codes and error values shall be omitted for brevity.  Please practice sound engineering practices
// when writing production code.

#include <stdio.h>
#include <stdlib.h>

#include "lwip/init.h"
#include "lwip/mem.h"
#include "lwip/sys.h"
#include "lwip/udp.h"
#include "lwip/tcp.h"
#include "lwip/tcpip.h"
#include "lwip/opt.h"
#include "lwip/debug.h"
#include "lwip/ip_addr.h"
#include "netif/slipif.h"
#include "lwip/sio.h"

#include "project.h"
#include "FreeRTOS.h"
#include "task.h"

#define LWIP_PORT_INIT_SLIP_IPADDR(addr)   IP4_ADDR((addr), 192, 168, 65, 233)
#define LWIP_PORT_INIT_SLIP_GW(addr)       IP4_ADDR((addr), 192, 168, 65, 1)
#define LWIP_PORT_INIT_SLIP_NETMASK(addr)  IP4_ADDR((addr), 255, 255, 255, 0)

#define SLIP_ADDRS &ipaddr_slip, &netmask_slip, &gw_slip,

ip4_addr_t ipaddr_slip, netmask_slip, gw_slip;
u8_t num_slip = 0;
struct netif slipif;

#include "iothub.h"
#include "iothub_device_client_ll.h"
#include "iothub_client_options.h"
#include "iothub_message.h"
#include "azure_c_shared_utility/threadapi.h"
#include "azure_c_shared_utility/crt_abstractions.h"
#include "azure_c_shared_utility/shared_util_options.h"

#ifdef SET_TRUSTED_CERT_IN_SAMPLES
#include "certs.h"
#endif // SET_TRUSTED_CERT_IN_SAMPLES

/* This sample uses the _LL APIs of iothub_client for example purposes.
Simply changing the using the convenience layer (functions not having _LL)
and removing calls to _DoWork will yield the same results. */

// The protocol you wish to use should be uncommented
//
//#define SAMPLE_MQTT
//#define SAMPLE_MQTT_OVER_WEBSOCKETS
#define SAMPLE_AMQP
//#define SAMPLE_AMQP_OVER_WEBSOCKETS
//#define SAMPLE_HTTP

#ifdef SAMPLE_MQTT
    #include "iothubtransportmqtt.h"
#endif // SAMPLE_MQTT
#ifdef SAMPLE_MQTT_OVER_WEBSOCKETS
    #include "iothubtransportmqtt_websockets.h"
#endif // SAMPLE_MQTT_OVER_WEBSOCKETS
#ifdef SAMPLE_AMQP
    #include "iothubtransportamqp.h"
#endif // SAMPLE_AMQP
#ifdef SAMPLE_AMQP_OVER_WEBSOCKETS
    #include "iothubtransportamqp_websockets.h"
#endif // SAMPLE_AMQP_OVER_WEBSOCKETS
#ifdef SAMPLE_HTTP
    #include "iothubtransporthttp.h"
#endif // SAMPLE_HTTP

#ifdef SET_TRUSTED_CERT_IN_SAMPLES
#include "certs.h"
#endif // SET_TRUSTED_CERT_IN_SAMPLES

/* Paste in the your iothub connection string  */
static const char* connectionString = "HostName=IoTSDKPortTesting.azure-devices.net;DeviceId=dev-test;SharedAccessKey=//qJeu0wPiLEpy9PbeibmA8ILx8afrpbZUmtXPke5pM=";

#define MESSAGE_COUNT        5
static bool g_continueRunning = true;
static size_t g_message_count_send_confirmations = 0;

unsigned int memfreeHeap, memfreeStack;

static void send_confirm_callback(IOTHUB_CLIENT_CONFIRMATION_RESULT result, void* userContextCallback)
{
    (void)userContextCallback;
    // When a message is sent this callback will get envoked
    g_message_count_send_confirmations++;
    (void)printf("Confirmation callback received for message %zu with result %s\r\n", g_message_count_send_confirmations, ENUM_TO_STRING(IOTHUB_CLIENT_CONFIRMATION_RESULT, result));
}

static void connection_status_callback(IOTHUB_CLIENT_CONNECTION_STATUS result, IOTHUB_CLIENT_CONNECTION_STATUS_REASON reason, void* user_context)
{
    (void)reason;
    (void)user_context;
    // This sample DOES NOT take into consideration network outages.
    if (result == IOTHUB_CLIENT_CONNECTION_AUTHENTICATED)
    {
        (void)printf("The device client is connected to iothub\r\n");
    }
    else
    {
        (void)printf("The device client has been disconnected\r\n");
    }
}

void azure()
{
    
    __enable_irq();
    
    tcpip_init(NULL,NULL);
    
    LWIP_PORT_INIT_SLIP_IPADDR(&ipaddr_slip);
    LWIP_PORT_INIT_SLIP_GW(&gw_slip);
    LWIP_PORT_INIT_SLIP_NETMASK(&netmask_slip);
    
    netif_add(&slipif, SLIP_ADDRS &num_slip, slipif_init, ip_input);
    netif_set_default (&slipif);
    netif_set_up(&slipif);
    netif_set_link_up(&slipif);    
    
    IOTHUB_CLIENT_TRANSPORT_PROVIDER protocol;
    IOTHUB_MESSAGE_HANDLE message_handle;
    size_t messages_sent = 0;
    const char* telemetry_msg = "Telemetry_Data_Test";

    // Select the Protocol to use with the connection
#ifdef SAMPLE_MQTT
    protocol = MQTT_Protocol;
#endif // SAMPLE_MQTT
#ifdef SAMPLE_MQTT_OVER_WEBSOCKETS
    protocol = MQTT_WebSocket_Protocol;
#endif // SAMPLE_MQTT_OVER_WEBSOCKETS
#ifdef SAMPLE_AMQP
    protocol = AMQP_Protocol;
#endif // SAMPLE_AMQP
#ifdef SAMPLE_AMQP_OVER_WEBSOCKETS
    protocol = AMQP_Protocol_over_WebSocketsTls;
#endif // SAMPLE_AMQP_OVER_WEBSOCKETS
#ifdef SAMPLE_HTTP
    protocol = HTTP_Protocol;
#endif // SAMPLE_HTTP

    // Used to initialize IoTHub SDK subsystem
    (void)IoTHub_Init();

    IOTHUB_DEVICE_CLIENT_LL_HANDLE device_ll_handle;

    (void)printf("Creating IoTHub Device handle\r\n");
    // Create the iothub handle here
    device_ll_handle = IoTHubDeviceClient_LL_CreateFromConnectionString(connectionString, protocol);
    if (device_ll_handle == NULL)
    {
        (void)printf("Failure creating Iothub device.  Hint: Check you connection string.\r\n");
    }
    else
    {
        // Set any option that are neccessary.
        // For available options please see the iothub_sdk_options.md documentation

        (void)printf("PSOC6---Set_the_option_for_iothub_client_ll---PSOC6\r\n");
        bool traceOn = true;
        IoTHubDeviceClient_LL_SetOption(device_ll_handle, OPTION_LOG_TRACE, &traceOn);

#ifdef SET_TRUSTED_CERT_IN_SAMPLES
        // Setting the Trusted Certificate.  This is only necessary on system with without
        // built in certificate stores.
            IoTHubDeviceClient_LL_SetOption(device_ll_handle, OPTION_TRUSTED_CERT, certificates);
#endif // SET_TRUSTED_CERT_IN_SAMPLES

#if defined SAMPLE_MQTT || defined SAMPLE_MQTT_WS
        //Setting the auto URL Encoder (recommended for MQTT). Please use this option unless
        //you are URL Encoding inputs yourself.
        //ONLY valid for use with MQTT
        //bool urlEncodeOn = true;
        //IoTHubDeviceClient_LL_SetOption(iothub_ll_handle, OPTION_AUTO_URL_ENCODE_DECODE, &urlEncodeOn);
#endif

        // Setting connection status callback to get indication of connection to iothub
//        (void)IoTHubDeviceClient_LL_SetConnectionStatusCallback(device_ll_handle, connection_status_callback, NULL);

        do
        {
            if (messages_sent < MESSAGE_COUNT)
            {
                // Construct the iothub message from a string or a byte array
                message_handle = IoTHubMessage_CreateFromString(telemetry_msg);
                //message_handle = IoTHubMessage_CreateFromByteArray((const unsigned char*)msgText, strlen(msgText)));

                // Set Message property
                /*(void)IoTHubMessage_SetMessageId(message_handle, "MSG_ID");
                (void)IoTHubMessage_SetCorrelationId(message_handle, "CORE_ID");
                (void)IoTHubMessage_SetContentTypeSystemProperty(message_handle, "application%2fjson");
                (void)IoTHubMessage_SetContentEncodingSystemProperty(message_handle, "utf-8");*/

                // Add custom properties to message
                (void)IoTHubMessage_SetProperty(message_handle, "property_key", "property_value");

                (void)printf("Sending message %d to IoTHub\r\n", (int)(messages_sent + 1));
                IoTHubDeviceClient_LL_SendEventAsync(device_ll_handle, message_handle, send_confirm_callback, NULL);

                // The message is copied to the sdk so the we can destroy it
                IoTHubMessage_Destroy(message_handle);

                messages_sent++;
            }
            else if (g_message_count_send_confirmations >= MESSAGE_COUNT)
            {
                // After all messages are all received stop running
                g_continueRunning = false;
            }

            IoTHubDeviceClient_LL_DoWork(device_ll_handle);
            ThreadAPI_Sleep(1);

        } while (g_continueRunning);

        // Clean up the iothub sdk handle
        IoTHubDeviceClient_LL_Destroy(device_ll_handle);
    }
    // Free all the sdk subsystem
    IoTHub_Deinit();

    printf("Press any key to continue");
    getchar();

   // return 0;
}

int main(void)
{
   // UART_Start();
   // Cy_SCB_UART_Put(UART_1_HW, 'c');
    __enable_irq();
    xTaskCreate(azure,"azure",6144,0,1,0);
    //memfreeHeap = xPortGetFreeHeapSize();
        //memfreeStack = uxTaskGetStackHighWaterMark(&xHandle);
        
        printf("memory free : %u %u", memfreeHeap, memfreeStack);
    vTaskStartScheduler();
    while(1);
}

void vApplicationMallocFailedHook (void)
{
    printf("Heap Overflow\r\n");
    while(1);
}

void vApplicationStackOverflowHook( TaskHandle_t xTask, char *pcTaskName )
{
    printf("Stack Overflow\r\n");
    while(1);
}