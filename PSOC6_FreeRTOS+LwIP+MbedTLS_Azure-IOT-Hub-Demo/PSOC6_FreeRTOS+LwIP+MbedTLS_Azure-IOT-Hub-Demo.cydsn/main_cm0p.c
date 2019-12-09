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
#include "project.h"
#include "FreeRTOS.h"
#include "task.h"
 
void vApplicationStackOverflowHook(TaskHandle_t *pxTask, signed char *pcTaskName )
{
    while(1);
}
 
void ledTask(void *arg)
{
    (void)arg;
    
    while(1)
    {
        Cy_GPIO_Write(RED_PORT,RED_NUM,!Cy_GPIO_ReadOut(RED_PORT,RED_NUM));
        vTaskDelay(500);
    }
}

void UART_Task(void *arg)
{
    (void)arg;
    static char buff[500];
    UART_1_Start();
    Cy_SCB_UART_PutString(UART_1_HW, "Started UART\n\r");
    char c;
    
	while(1) {
        c = Cy_SCB_UART_Get(UART_1_HW);
        switch(c)
        {
            case 'a':
            Cy_SCB_UART_PutString(UART_1_HW, "Working\n\r");
            break;
            
            case 't':
                Cy_SCB_UART_PutString(UART_1_HW, "********************************************\n\r");
                Cy_SCB_UART_PutString(UART_1_HW, "Task          State   Prio    Stack    Num\n\r"); 
                Cy_SCB_UART_PutString(UART_1_HW, "********************************************\n\r");
                vTaskList(buff);
                Cy_SCB_UART_PutString(UART_1_HW, buff);
                Cy_SCB_UART_PutString(UART_1_HW, "*********************************************\n\r");
   
            break;
        }
        taskYIELD();
    }
}
 
int main(void)
{
    __enable_irq(); /* Enable global interrupts. */
    /* Enable CM4.  CY_CORTEX_M4_APPL_ADDR must be updated if CM4 memory layout is changed. */
    Cy_SysEnableCM4(CY_CORTEX_M4_APPL_ADDR);    
 
    xTaskCreate( ledTask, "LED Task",400,0,1,0);
    xTaskCreate( UART_Task, "UART Task",1024,0,1,0);
    vTaskStartScheduler();
 
    while(1);
}