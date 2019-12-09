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
#include "lwip/dhcp.h"

#define SERVER_PORT_NUM 5671
#define SERVER_IP_ADDRESS "192.168.2.1"

ip4_addr_t ipaddr_slip, netmask_slip, gw_slip;
u8_t num_slip = 0;
struct netif slipif;

unsigned int memfreeHeap, memfreeStack;

void print_ip(char *msg, ip_addr_t *ip)
{
	printf("%s", msg);
	printf("%d.%d.%d.%d\n\r", ip4_addr1(ip), ip4_addr2(ip),
			ip4_addr3(ip), ip4_addr4(ip));
}

void print_ip_settings(ip_addr_t *ip, ip_addr_t *mask, ip_addr_t *gw)
{
	print_ip("Board IP: ", ip);
	print_ip("Netmask : ", mask);
	print_ip("Gateway : ", gw);
}

void app_thread()
{
    struct sockaddr_in address;
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    char *hello = "Hello from client";
    char buffer[1024] = {0};   
    
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

void network_thread()
{        
    struct netif *netif;
    /* the mac address of the board. this should be unique per board */
    unsigned char mac_ethernet_address[] = { 0x00, 0x0a, 0x35, 0x00, 0x01, 0x02 };
    ip_addr_t ipaddr, netmask, gw;
    
#if LWIP_DHCP==1
    int mscnt = 0;
#endif

    netif = &slipif;

#if LWIP_DHCP==0
    /* initliaze IP addresses to be used */
    IP4_ADDR(&ipaddr,  192, 168, 1, 250);
    IP4_ADDR(&netmask, 255, 255, 255,  0);
    IP4_ADDR(&gw,      192, 168, 1, 1);
#endif

    /* print out IP settings of the board */

#if LWIP_DHCP==0
    print_ip_settings(&ipaddr, &netmask, &gw);
#endif

#if LWIP_DHCP==1
	ipaddr.addr = 0;
	gw.addr = 0;
	netmask.addr = 0;
#endif

    /* Add network interface to the netif_list, and set it as default */
    
    netif_add(netif, &ipaddr, &netmask, &gw, &num_slip, slipif_init, ip_input);
        
    netif_set_default(netif);

    /* specify that the network if is up */
    netif_set_up(netif);
    
    netif_set_link_up(&slipif);

#if LWIP_DHCP==1
    dhcp_start(netif);
    while (1) {
		vTaskDelay(DHCP_FINE_TIMER_MSECS / configTICK_PERIOD_MS);
		dhcp_fine_tmr();
		mscnt += DHCP_FINE_TIMER_MSECS;
		if (mscnt >= DHCP_COARSE_TIMER_SECS*1000) {
			dhcp_coarse_tmr();
			mscnt = 0;
		}
	}
#else
    sys_thread_new("application thread", app_thread, 0, 2048, 1);
    vTaskDelete(NULL);
#endif
    return;
}

int main_thread()
{
#if LWIP_DHCP==1
    int mscnt = 0;
#endif    
    tcpip_init(NULL, NULL);
    sys_thread_new("network thread", network_thread, 0 , 2048, 1); 

#if LWIP_DHCP==1
    while (1) {
	vTaskDelay(DHCP_FINE_TIMER_MSECS / configTICK_PERIOD_MS);
		if (slipif.ip_addr.addr) {
			printf("DHCP request success\r\n");
			print_ip_settings(&(slipif.ip_addr), &(slipif.netmask), &(slipif.gw));
            
			sys_thread_new("application thread", app_thread, 0, 2048, 1);
			break;
		}
		mscnt += DHCP_FINE_TIMER_MSECS;
		if (mscnt >= 10000) {
			printf("ERROR: DHCP request timed out\r\n");
			printf("Configuring default IP of 192.168.1.10\r\n");
			IP4_ADDR(&(slipif.ip_addr),  192, 168, 1, 250);
			IP4_ADDR(&(slipif.netmask), 255, 255, 255,  0);
			IP4_ADDR(&(slipif.gw),  192, 168, 1, 1);
			print_ip_settings(&(slipif.ip_addr), &(slipif.netmask), &(slipif.gw));

			sys_thread_new("application thread", app_thread, 0, 2048, 1);
			break;
		}
	}
#endif
    vTaskDelete(NULL);   
    return 0;
}

//int main(void)
//{
//    __enable_irq();
    
//    sys_thread_new("main thread", main_thread, 0, 1024, 1);
    //memfreeHeap = xPortGetFreeHeapSize();
    //memfreeStack = uxTaskGetStackHighWaterMark(&xHandle);
//    vTaskStartScheduler();
//    while(1);
//}

//void vApplicationMallocFailedHook (void)
//{
//    printf("Heap Overflow\r\n");
//    while(1);
//}

//void vApplicationStackOverflowHook( TaskHandle_t xTask, char *pcTaskName )
//{
//    printf("Stack Overflow\r\n");
//    while(1);
//}
