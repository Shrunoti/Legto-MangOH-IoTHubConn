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

#define LWIP_PORT_INIT_SLIP_IPADDR(addr)   IP4_ADDR((addr), 192, 168,   1, 4)
#define LWIP_PORT_INIT_SLIP_GW(addr)       IP4_ADDR((addr), 192, 168,   1, 1)
#define LWIP_PORT_INIT_SLIP_NETMASK(addr)  IP4_ADDR((addr), 255, 255, 255, 0)
#define UDP_PORT	10000

ip4_addr_t ipaddr_slip, netmask_slip, gw_slip;
u8_t num_slip = 0;
struct netif slipif;

unsigned int memfreeHeap, memfreeStack;

volatile uint8_t g_SlaveDataReady = 0;

#define SLIP_ADDRS &ipaddr_slip, &netmask_slip, &gw_slip,

void SwInterruptIsr()
{   
    Cy_GPIO_ClearInterrupt(Pin_1_PORT, Pin_1_NUM);
    
    g_SlaveDataReady = 1;
    
	NVIC_ClearPendingIRQ(SysInt_1_cfg.intrSrc);
}

void tcp_test()
{
    int listenfd = 0, connfd = 0, n = 0;
    struct sockaddr_in serv_addr, client_addr;

    char *hello = "Hello from server";
    char sendBuff[1024];
    
    Cy_SysInt_Init(&SysInt_1_cfg, SwInterruptIsr); 
    NVIC_ClearPendingIRQ(SysInt_1_cfg.intrSrc);
    NVIC_EnableIRQ((IRQn_Type)SysInt_1_cfg.intrSrc);    

    tcpip_init(NULL, NULL);
    
    LWIP_PORT_INIT_SLIP_IPADDR(&ipaddr_slip);
    LWIP_PORT_INIT_SLIP_GW(&gw_slip);
    LWIP_PORT_INIT_SLIP_NETMASK(&netmask_slip);
    
    netif_add(&slipif, SLIP_ADDRS &num_slip, slipif_init, ip_input);
    netif_set_default (&slipif);
    netif_set_up(&slipif);
    netif_set_link_up(&slipif);

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    memset(&serv_addr, '0', sizeof(serv_addr));
    memset(sendBuff, '0', sizeof(sendBuff));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(SERVER_IP_ADDRESS);
    serv_addr.sin_port = htons(SERVER_PORT_NUM);

    bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

    listen(listenfd, 5);

    while(1)
    {
        connfd = accept(listenfd, (struct sockaddr*)NULL, NULL);
        if(connfd >= 0)
        {
                printf("Server: Client connected\r\n");
                memset(sendBuff, 0, 1024);
                while ((n = recv(connfd, sendBuff, sizeof(sendBuff), 0)) <= 0);
                sendBuff[n] = 0;
                printf("Server: %s\r\n", sendBuff);
                send(connfd, hello, strlen(hello), 0);
        }
        close(connfd);
     }
}

int main(void)
{
    __enable_irq();
    
    xTaskCreate(tcp_test, "TCP_TEST", 4096, 0, 1, 0);
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
