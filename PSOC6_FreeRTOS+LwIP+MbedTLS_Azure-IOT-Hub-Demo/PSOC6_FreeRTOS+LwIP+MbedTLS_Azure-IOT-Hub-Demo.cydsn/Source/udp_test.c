/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "project.h"

#include "FreeRTOS.h"
//#include "portable.h"
//#include "portmacro.h"
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


uint8_t i2cReadBuffer [60];

#define LWIP_PORT_INIT_SLIP_IPADDR(addr)   IP4_ADDR((addr), 192, 168,   1, 5)
#define LWIP_PORT_INIT_SLIP_GW(addr)       IP4_ADDR((addr), 192, 168,   1, 1)
#define LWIP_PORT_INIT_SLIP_NETMASK(addr)  IP4_ADDR((addr), 255, 255, 255, 0)
#define UDP_PORT	10000

#define SLIP_ADDRS &ipaddr_slip, &netmask_slip, &gw_slip,
#define MAX_COMMAND_LENGTH	10 // Including null terminator
#define MAX_RETURN_LENGTH	25

#define RESP_PREFIX	"Device Response: "

static struct udp_pcb *udpecho_raw_pcb;
static struct tcp_pcb *tcpecho_raw_pcb;
char retBuf[MAX_RETURN_LENGTH];
char retError[] = "API Error!";
char respPrefix[] = RESP_PREFIX;

// Macros for convenience
#define ZERO_OUT_RET_BUFS() \
	memset(retBuf, 0, sizeof(retBuf));


#define ALLOCATE_CREATE_PBUF(pbuf, bufSrc) \
		pbuf = pbuf_alloc(PBUF_TRANSPORT, sizeof(respPrefix) + strlen(bufSrc) , PBUF_RAM); \
		memcpy(pbuf->payload, respPrefix , sizeof(respPrefix) - 1); \
		memcpy(&(pbuf->payload[sizeof(respPrefix) - 1]), bufSrc, strlen(bufSrc) + 1)

ip4_addr_t ipaddr_slip, netmask_slip, gw_slip;
u8_t num_slip = 0;
struct netif slipif;

struct udp_pcb *upcb;
struct pbuf *p;

TaskHandle_t xHandle = NULL;

unsigned int memfreeHeap;
unsigned int memfreeStack;

static void udpecho_raw_recv_cb(void *arg, struct tcp_pcb *upcb, struct pbuf *p,
                 const ip_addr_t *addr, u16_t port)
{
    
}

static void tcpecho_raw_recv_cb(void *arg, struct udp_pcb *upcb, struct pbuf *p,
                 const ip_addr_t *addr, u16_t port)
{
    
}

void tcpecho_raw_init_cb(void)
{
    tcpecho_raw_pcb = tcp_new_ip_type(IPADDR_TYPE_ANY);
    if(tcpecho_raw_pcb != NULL)
    {
        err_t err;
        err = tcp_bind(tcpecho_raw_pcb, IP_ANY_TYPE, UDP_PORT);
        if (err == ERR_OK)
        {
          //  tcp_recv(tcpecho_raw_pcb, tcpecho_raw_recv_cb);
        }
    }
}

void udpecho_raw_init_cb(void)
{
    udpecho_raw_pcb = udp_new_ip_type(IPADDR_TYPE_ANY);
    if (udpecho_raw_pcb != NULL)
    {
        err_t err;
        err = udp_bind(udpecho_raw_pcb, IP_ANY_TYPE, UDP_PORT );
        if (err == ERR_OK)
        {
         //   udp_recv (udpecho_raw_pcb, udpecho_raw_recv_cb, NULL);
            
          //  printf("CALLBACK SET\r\n");
        }
    }  
}

void lwip_test()
{
   // (void)arg;
    lwip_init();
    udpecho_raw_init_cb();
   // tcpecho_raw_init_cb();
    
    LWIP_PORT_INIT_SLIP_IPADDR(&ipaddr_slip);
    LWIP_PORT_INIT_SLIP_GW(&gw_slip);
    LWIP_PORT_INIT_SLIP_NETMASK(&netmask_slip);
    
    netif_add(&slipif, SLIP_ADDRS &num_slip, slipif_init, ip_input);
    netif_set_default (&slipif);
    netif_set_up(&slipif);
    netif_set_link_up(&slipif);

    ALLOCATE_CREATE_PBUF(p, "LWIP TEST");
    
    while(1)
    {
        //memfreeHeap = xPortGetFreeHeapSize();
        //memfreeStack = uxTaskGetStackHighWaterMark(&xHandle);
        
        printf("memory free : %u %u", memfreeHeap, memfreeStack);
        udp_send(udpecho_raw_pcb, p);
        vTaskDelay(2000);
    }
}

void lwip_test2()
{

    
    
    while(1)
    {
        ALLOCATE_CREATE_PBUF(p, "LWIP TEST2");
        //udp_send(udpecho_raw_pcb, p);
    }
      //vTaskDelete(&xHandle);
}

//uint8_t ucHeap[configTOTAL_HEAP_SIZE] __attribute__ ((section (".my_heap")));

int main(void)
{
    //UART_1_Start();
    __enable_irq(); /* Enable global interrupts. */

    xTaskCreate(lwip_test, "LWIP_TEST", 6144, 0, 1, &xHandle);
    //xTaskCreate(lwip_test2, "LWIP_TEST2",1000,0,19,0);
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



/* [] END OF FILE */
