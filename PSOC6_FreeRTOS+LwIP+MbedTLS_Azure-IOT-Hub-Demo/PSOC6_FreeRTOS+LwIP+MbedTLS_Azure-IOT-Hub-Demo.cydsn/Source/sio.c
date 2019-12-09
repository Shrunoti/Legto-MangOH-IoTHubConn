
#include "lwip/sio.h"
#include "arch/sys_arch.h"
#include "lwip/sys.h"
#include "project.h"
#include <string.h>

//#define SIO_I2C

static uint8_t TxBuffer[1024];
static uint8_t RxBuffer[1024];

static uint32_t TxBufferIdx = 0;
static uint32_t RxBufferIdx = 0;

static uint32_t RxBuffLen = 0;

static volatile uint8_t MasterWriteComplete = 0;

#ifdef SIO_I2C
/*******************************************************************************
* Function Name: HandleEventsSlave
****************************************************************************//**
*
* Handles slave events write and read completion events. 
*
* \param event
* reports slave events.
* ref uint32_t
*
* \return
*  None
*
*******************************************************************************/
static void HandleEventsSlave(uint32_t event)
{
    /* Check write complete event */
    if (0UL != (CY_SCB_I2C_SLAVE_WR_CMPLT_EVENT & event))
    {   
        /* Check for errors */
        if (0UL == (CY_SCB_I2C_SLAVE_ERR_EVENT & event))
        {
            MasterWriteComplete = 1;
        }
        else
        {
            MasterWriteComplete = 0;
        }
    }
    
    /* Check write complete event */
    if (0UL != (CY_SCB_I2C_SLAVE_RD_CMPLT_EVENT & event))
    {
        Cy_GPIO_Write(Pin_1_0_PORT, Pin_1_0_NUM, 1);
        
        TxBufferIdx = 0;
        
        /* Configure read buffer for the next read */
        Cy_SCB_I2C_SlaveConfigReadBuf(I2C_HW, TxBuffer, 4096, &I2C_context);
    }
}
#endif

/**
 * Opens a serial device for communication.
 *
 * @param devnum device number
 * @return handle to serial device if successful, NULL otherwise
 */
sio_fd_t sio_open(u8_t devnum)
{
    (void) devnum;
    cy_en_scb_i2c_status_t initI2Cstatus;
    cy_en_sysint_status_t sysI2Cstatus;
    
    UART_1_Start();
    
#ifdef SIO_I2C   
    initI2Cstatus = Cy_SCB_I2C_Init(I2C_HW, &I2C_config, &I2C_context);
    if(initI2Cstatus != CY_SCB_I2C_SUCCESS)
    {
        // HandleError();
    }
    
    /* Initialize and enable I2C Interrupt in slave mode. If initialization fails process error. */
    sysI2Cstatus = Cy_SysInt_Init(&I2C_SCB_IRQ_cfg, &I2C_Interrupt);
    if(sysI2Cstatus != CY_SYSINT_SUCCESS)
    {
        // HandleError();
    }
    
    Cy_SCB_I2C_SlaveConfigReadBuf(I2C_HW, TxBuffer, 4096, &I2C_context);
    Cy_SCB_I2C_SlaveConfigWriteBuf(I2C_HW, RxBuffer, 4096, &I2C_context);
    
    /* Register Callback function for interrupt */
    Cy_SCB_I2C_RegisterEventCallback(I2C_HW, (cy_cb_scb_i2c_handle_events_t) HandleEventsSlave, &I2C_context);
    
    /*  Enable interrupt and I2C block. */
    NVIC_EnableIRQ((IRQn_Type) I2C_SCB_IRQ_cfg.intrSrc);
    Cy_SCB_I2C_Enable(I2C_HW);
    
    return I2C_HW;
#else
    UART_2_Start();
    return UART_2_HW;
#endif        
}

/**
 * Sends a single character to the serial device.
 *
 * @param c character to send
 * @param fd serial device handle
 *
 * @note This function will block until the character can be sent.
 */
void sio_send(u8_t c, sio_fd_t fd)
{    
#ifdef SIO_I2C    
    if (TxBufferIdx == 0)
    {
        /* clear the buffer */
        memset(TxBuffer, 0, 4096);   
    }
    
    if(TxBufferIdx == 4095)
    {
        /* reset buffer index */
        TxBufferIdx = 0;
        /* Configure buffer again */
        /* For the first time Cy_SCB_I2C_SlaveConfigReadBuf is called by sio_open() */
        Cy_SCB_I2C_SlaveConfigReadBuf(I2C_HW, TxBuffer, 4096, &I2C_context);
    }
    
    TxBuffer[TxBufferIdx++] = c;
    Cy_SCB_UART_Put(UART_1_HW, c);
#else
    Cy_SCB_UART_Put(fd, c);
    Cy_SCB_UART_Put(UART_1_HW, c);
#endif    
}

/**
 * Receives a single character from the serial device.
 *
 * @param fd serial device handle
 *
 * @note This function will block until a character is received.
 */
u8_t sio_recv(sio_fd_t fd)
{
	u8_t c;    
    c = Cy_SCB_UART_Get(fd);    
    return c;
}

/**
 * Reads from the serial device.
 *
 * @param fd serial device handle
 * @param data pointer to data buffer for receiving
 * @param len maximum length (in bytes) of data to receive
 * @return number of bytes actually received - may be 0 if aborted by sio_read_abort
 *
 * @note This function will block until data can be received. The blocking
 * can be cancelled by calling sio_read_abort().
 */
u32_t sio_read(sio_fd_t fd, u8_t *data, u32_t len)
{   
#ifdef SIO_I2C    
    if(MasterWriteComplete)
    {    
        RxBuffLen = Cy_SCB_I2C_SlaveGetWriteTransferCount(I2C_HW, &I2C_context);
        
        if((RxBufferIdx == RxBuffLen) && (RxBuffLen != 0))
        {
            RxBufferIdx = 0;
            RxBuffLen = 0;
            MasterWriteComplete = 0;
            /* Configure buffer again */
            /* For the first time Cy_SCB_I2C_SlaveConfigReadBuf is called by sio_open() */
            Cy_SCB_I2C_SlaveConfigWriteBuf(I2C_HW, RxBuffer, 4096, &I2C_context);        
        }
        
        if(RxBuffLen > 0)
        {
            *data = RxBuffer[RxBufferIdx++];
            Cy_SCB_UART_PutArrayBlocking(UART_1_HW, data, len);
            return len;
        }
    }
	return 0;
#else
    Cy_SCB_UART_GetArrayBlocking(fd, data, len);
    Cy_SCB_UART_PutArrayBlocking(UART_1_HW, data, len);        
    return len;
#endif    
}

/**
 * Tries to read from the serial device. Same as sio_read but returns
 * immediately if no data is available and never blocks.
 *
 * @param fd serial device handle
 * @param data pointer to data buffer for receiving
 * @param len maximum length (in bytes) of data to receive
 * @return number of bytes actually received
 */
u32_t sio_tryread(sio_fd_t fd, u8_t *data, u32_t len)
{
    Cy_SCB_UART_GetArray(fd, data, len);
    return len;
}

/**
 * Writes to the serial device.
 *
 * @param fd serial device handle
 * @param data pointer to data to send
 * @param len length (in bytes) of data to send
 * @return number of bytes actually sent
 *
 * @note This function will block until all data can be sent.
 */
u32_t sio_write(sio_fd_t fd, u8_t *data, u32_t len)
{
#ifdef SIO_I2C   
    if(TxBufferIdx == 4095)
    {
        TxBufferIdx = 0;
        /* Configure buffer again */
        /* For the first time Cy_SCB_I2C_SlaveConfigReadBuf is called by sio_open() */
        Cy_SCB_I2C_SlaveConfigReadBuf(fd, TxBuffer, 4096, &I2C_context);        
    }
    
    memcpy((TxBuffer+TxBufferIdx), data, len);
    
    TxBufferIdx += len;

    Cy_SCB_UART_PutArray(UART_1_HW, data, len);
#else
    len = Cy_SCB_UART_PutArray(fd, data, len);
    Cy_SCB_UART_PutArray(UART_1_HW, data, len);
#endif
    return len;
}

/**
 * Aborts a blocking sio_read() call.
 *
 * @param fd serial device handle
 */
void sio_read_abort(sio_fd_t fd)
{
    Cy_SCB_UART_AbortReceive(fd, &UART_1_context);
}