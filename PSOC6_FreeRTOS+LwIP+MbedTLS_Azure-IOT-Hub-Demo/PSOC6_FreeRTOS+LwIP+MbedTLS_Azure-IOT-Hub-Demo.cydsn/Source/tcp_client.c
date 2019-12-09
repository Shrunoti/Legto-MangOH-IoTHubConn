#include <stdlib.h>
#include <string.h>
#include "lwip/sockets.h"

#include "project.h"
#include "FreeRTOS.h"
#include "task.h"

#include "lwip/init.h"
#include "lwip/mem.h"
#include "lwip/sys.h"
#include "lwip/udp.h"
#include "lwip/tcp.h"
#include "lwip/opt.h"
#include "lwip/debug.h"
#include "lwip/ip_addr.h"
#include "netif/slipif.h"
#include "lwip/sio.h"
#include "lwip/tcpip.h"

#define SERVER_PORT_NUM 6000
#define SERVER_IP_ADDRESS "192.168.1.4"

#define LWIP_PORT_INIT_SLIP_IPADDR(addr)   IP4_ADDR((addr), 192, 168,   1, 250)
#define LWIP_PORT_INIT_SLIP_GW(addr)       IP4_ADDR((addr), 192, 168,   1, 1)
#define LWIP_PORT_INIT_SLIP_NETMASK(addr)  IP4_ADDR((addr), 255, 255, 255, 0)
#define UDP_PORT	10000

ip4_addr_t ipaddr_slip, netmask_slip, gw_slip;
u8_t num_slip = 0;
struct netif slipif;

unsigned int memfreeHeap, memfreeStack;

#define SLIP_ADDRS &ipaddr_slip, &netmask_slip, &gw_slip,

void tcp_test()
{
    struct sockaddr_in address;
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    char *hello = "Hello from client";
    char buffer[1024] = {0};    
    
    tcpip_init(NULL, NULL);
    
    LWIP_PORT_INIT_SLIP_IPADDR(&ipaddr_slip);
    LWIP_PORT_INIT_SLIP_GW(&gw_slip);
    LWIP_PORT_INIT_SLIP_NETMASK(&netmask_slip);
    
    netif_add(&slipif, SLIP_ADDRS &num_slip, slipif_init, ip_input);
    netif_set_default (&slipif);
    netif_set_up(&slipif);
    netif_set_link_up(&slipif);

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        exit(2);
    }

    memset(&serv_addr, '0', sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(SERVER_IP_ADDRESS);
    serv_addr.sin_port = htons(SERVER_PORT_NUM);

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\n Connection Failed \n");
        exit(2);
    }
    
    send(sock , hello , strlen(hello) , 0 );
    printf("Client: Hello message sent\r\n");
    
    memset(buffer, 0, 1024);
    valread = recv( sock , buffer, 1024, 0);
    printf("Client: %s\r\n",buffer ); 
    
    vTaskDelete(NULL);
}

int main(void)
{
    __enable_irq();
    
    xTaskCreate(tcp_test, "TCP_TEST", 2048, 0, 1, 0);
    //memfreeHeap = xPortGetFreeHeapSize();
    //memfreeStack = uxTaskGetStackHighWaterMark(&xHandle);
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
