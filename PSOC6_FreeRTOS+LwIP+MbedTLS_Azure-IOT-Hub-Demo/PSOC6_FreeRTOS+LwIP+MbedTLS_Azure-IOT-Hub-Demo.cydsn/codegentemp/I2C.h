/***************************************************************************//**
* \file I2C.c
* \version 2.0
*
*  This file provides constants and parameter values for the I2C component.
*
********************************************************************************
* \copyright
* Copyright 2016-2017, Cypress Semiconductor Corporation. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(I2C_CY_SCB_I2C_PDL_H)
#define I2C_CY_SCB_I2C_PDL_H

#include "cyfitter.h"
#include "scb/cy_scb_i2c.h"

#if defined(__cplusplus)
extern "C" {
#endif

/***************************************
*   Initial Parameter Constants
****************************************/

#define I2C_MODE               (0x1U)
#define I2C_MODE_SLAVE_MASK    (0x1U)
#define I2C_MODE_MASTER_MASK   (0x2U)

#define I2C_ENABLE_SLAVE       (0UL != (I2C_MODE & I2C_MODE_SLAVE_MASK))
#define I2C_ENABLE_MASTER      (0UL != (I2C_MODE & I2C_MODE_MASTER_MASK))
#define I2C_MANUAL_SCL_CONTROL (0U)


/***************************************
*        Function Prototypes
***************************************/
/**
* \addtogroup group_general
* @{
*/
/* Component only APIs. */
void I2C_Start(void);

/* Basic functions. */
__STATIC_INLINE cy_en_scb_i2c_status_t I2C_Init(cy_stc_scb_i2c_config_t const *config);
__STATIC_INLINE void I2C_DeInit (void);
__STATIC_INLINE void I2C_Enable (void);
__STATIC_INLINE void I2C_Disable(void);

/* Data rate configuration functions. */
__STATIC_INLINE uint32_t I2C_SetDataRate(uint32_t dataRateHz, uint32_t scbClockHz);
__STATIC_INLINE uint32_t I2C_GetDataRate(uint32_t scbClockHz);

/* Register callbacks. */
__STATIC_INLINE void I2C_RegisterEventCallback(cy_cb_scb_i2c_handle_events_t callback);
#if (I2C_ENABLE_SLAVE)
__STATIC_INLINE void I2C_RegisterAddrCallback (cy_cb_scb_i2c_handle_addr_t callback);
#endif /* (I2C_ENABLE_SLAVE) */

/* Configuration functions. */
#if (I2C_ENABLE_SLAVE)
__STATIC_INLINE void     I2C_SlaveSetAddress(uint8_t addr);
__STATIC_INLINE uint32_t I2C_SlaveGetAddress(void);
__STATIC_INLINE void     I2C_SlaveSetAddressMask(uint8_t addrMask);
__STATIC_INLINE uint32_t I2C_SlaveGetAddressMask(void);
#endif /* (I2C_ENABLE_SLAVE) */

#if (I2C_ENABLE_MASTER)
__STATIC_INLINE void I2C_MasterSetLowPhaseDutyCycle (uint32_t clockCycles);
__STATIC_INLINE void I2C_MasterSetHighPhaseDutyCycle(uint32_t clockCycles);
#endif /* (I2C_ENABLE_MASTER) */

/* Bus status. */
__STATIC_INLINE bool     I2C_IsBusBusy(void);

/* Slave functions. */
#if (I2C_ENABLE_SLAVE)
__STATIC_INLINE uint32_t I2C_SlaveGetStatus(void);

__STATIC_INLINE void     I2C_SlaveConfigReadBuf(uint8_t *buffer, uint32_t size);
__STATIC_INLINE void     I2C_SlaveAbortRead(void);
__STATIC_INLINE uint32_t I2C_SlaveGetReadTransferCount(void);
__STATIC_INLINE uint32_t I2C_SlaveClearReadStatus(void);

__STATIC_INLINE void     I2C_SlaveConfigWriteBuf(uint8_t *buffer, uint32_t size);
__STATIC_INLINE void     I2C_SlaveAbortWrite(void);
__STATIC_INLINE uint32_t I2C_SlaveGetWriteTransferCount(void);
__STATIC_INLINE uint32_t I2C_SlaveClearWriteStatus(void);
#endif /* (I2C_ENABLE_SLAVE) */

/* Master interrupt processing functions. */
#if (I2C_ENABLE_MASTER)
__STATIC_INLINE uint32_t I2C_MasterGetStatus(void);

__STATIC_INLINE cy_en_scb_i2c_status_t I2C_MasterRead(cy_stc_scb_i2c_master_xfer_config_t *xferConfig);
__STATIC_INLINE void I2C_MasterAbortRead(void);
__STATIC_INLINE cy_en_scb_i2c_status_t I2C_MasterWrite(cy_stc_scb_i2c_master_xfer_config_t *xferConfig);
__STATIC_INLINE void I2C_MasterAbortWrite(void);
__STATIC_INLINE uint32_t I2C_MasterGetTransferCount(void);

/* Master manual processing functions. */
__STATIC_INLINE cy_en_scb_i2c_status_t I2C_MasterSendStart(uint32_t address, cy_en_scb_i2c_direction_t bitRnW, uint32_t timeoutMs);
__STATIC_INLINE cy_en_scb_i2c_status_t I2C_MasterSendReStart(uint32_t address, cy_en_scb_i2c_direction_t bitRnW, uint32_t timeoutMs);
__STATIC_INLINE cy_en_scb_i2c_status_t I2C_MasterSendStop(uint32_t timeoutMs);
__STATIC_INLINE cy_en_scb_i2c_status_t I2C_MasterReadByte(cy_en_scb_i2c_command_t ackNack, uint8_t *byte, uint32_t timeoutMs);
__STATIC_INLINE cy_en_scb_i2c_status_t I2C_MasterWriteByte(uint8_t byte, uint32_t timeoutMs);
#endif /* (I2C_ENABLE_MASTER) */

/* Interrupt handler. */
__STATIC_INLINE void I2C_Interrupt(void);
/** @} group_general */


/***************************************
*    Variables with External Linkage
***************************************/
/**
* \addtogroup group_globals
* @{
*/
extern uint8_t I2C_initVar;
extern cy_stc_scb_i2c_config_t const I2C_config;
extern cy_stc_scb_i2c_context_t I2C_context;
/** @} group_globals */


/***************************************
*         Preprocessor Macros
***************************************/
/**
* \addtogroup group_macros
* @{
*/
/** The pointer to the base address of the SCB instance */
#define I2C_HW     ((CySCB_Type *) I2C_SCB__HW)

/** The desired data rate in Hz */
#define I2C_DATA_RATE_HZ      (400000U)

/** The frequency of the clock used by the Component in Hz */
#define I2C_CLK_FREQ_HZ       (12500000U)

/** The number of Component clocks used by the master to generate the SCL
* low phase. This number is calculated by GUI based on the selected data rate.
*/
#define I2C_LOW_PHASE_DUTY_CYCLE   (0U)

/** The number of Component clocks used by the master to generate the SCL
* high phase. This number is calculated by GUI based on the selected data rate.
*/
#define I2C_HIGH_PHASE_DUTY_CYCLE  (0U)
/** @} group_macros */


/***************************************
*    In-line Function Implementation
***************************************/

/*******************************************************************************
* Function Name: I2C_Init
****************************************************************************//**
*
* Invokes the Cy_SCB_I2C_Init() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE cy_en_scb_i2c_status_t I2C_Init(cy_stc_scb_i2c_config_t const *config)
{
    return Cy_SCB_I2C_Init(I2C_HW, config, &I2C_context);
}


/*******************************************************************************
*  Function Name: I2C_DeInit
****************************************************************************//**
*
* Invokes the Cy_SCB_I2C_DeInit() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void I2C_DeInit(void)
{
    Cy_SCB_I2C_DeInit(I2C_HW);
}


/*******************************************************************************
* Function Name: I2C_Enable
****************************************************************************//**
*
* Invokes the Cy_SCB_I2C_Enable() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void I2C_Enable(void)
{
    Cy_SCB_I2C_Enable(I2C_HW);
}


/*******************************************************************************
* Function Name: I2C_Disable
****************************************************************************//**
*
* Invokes the Cy_SCB_I2C_Disable() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void I2C_Disable(void)
{
    Cy_SCB_I2C_Disable(I2C_HW, &I2C_context);
}


/*******************************************************************************
* Function Name: I2C_SetDataRate
****************************************************************************//**
*
* Invokes the Cy_SCB_I2C_SetDataRate() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t I2C_SetDataRate(uint32_t dataRateHz, uint32_t scbClockHz)
{
    return Cy_SCB_I2C_SetDataRate(I2C_HW, dataRateHz, scbClockHz);
}


/*******************************************************************************
* Function Name: I2C_GetDataRate
****************************************************************************//**
*
* Invokes the Cy_SCB_I2C_GetDataRate() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t I2C_GetDataRate(uint32_t scbClockHz)
{
    return Cy_SCB_I2C_GetDataRate(I2C_HW, scbClockHz);
}


/*******************************************************************************
* Function Name: I2C_RegisterEventCallback
****************************************************************************//**
*
* Invokes the Cy_SCB_I2C_RegisterEventCallback() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void I2C_RegisterEventCallback(cy_cb_scb_i2c_handle_events_t callback)
{
    Cy_SCB_I2C_RegisterEventCallback(I2C_HW, callback, &I2C_context);
}


#if (I2C_ENABLE_SLAVE)
/*******************************************************************************
* Function Name: I2C_RegisterAddrCallback
****************************************************************************//**
*
* Invokes the Cy_SCB_I2C_RegisterAddrCallback() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void I2C_RegisterAddrCallback(cy_cb_scb_i2c_handle_addr_t callback)
{
    Cy_SCB_I2C_RegisterAddrCallback(I2C_HW, callback, &I2C_context);
}
#endif /* (I2C_ENABLE_SLAVE) */


/*******************************************************************************
* Function Name: I2C_IsBusBusy
****************************************************************************//**
*
* Invokes the Cy_SCB_I2C_IsBusBusy() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE bool I2C_IsBusBusy(void)
{
    return Cy_SCB_I2C_IsBusBusy(I2C_HW);
}


#if (I2C_ENABLE_SLAVE)
/*******************************************************************************
* Function Name: I2C_SlaveSetAddress
****************************************************************************//**
*
* Invokes the Cy_SCB_I2C_SlaveGetAddress() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void I2C_SlaveSetAddress(uint8_t addr)
{
    Cy_SCB_I2C_SlaveSetAddress(I2C_HW, addr);
}


/*******************************************************************************
* Function Name: I2C_SlaveGetAddress
****************************************************************************//**
*
* Invokes the Cy_SCB_I2C_SlaveGetAddress() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t I2C_SlaveGetAddress(void)
{
    return Cy_SCB_I2C_SlaveGetAddress(I2C_HW);
}


/*******************************************************************************
* Function Name: I2C_SlaveSetAddressMask
****************************************************************************//**
*
* Invokes the Cy_SCB_I2C_SlaveSetAddressMask() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void I2C_SlaveSetAddressMask(uint8_t addrMask)
{
    Cy_SCB_I2C_SlaveSetAddressMask(I2C_HW, addrMask);
}


/*******************************************************************************
* Function Name: I2C_SlaveGetAddressMask
****************************************************************************//**
*
* Invokes the Cy_SCB_I2C_SlaveGetAddressMask() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t I2C_SlaveGetAddressMask(void)
{
    return Cy_SCB_I2C_SlaveGetAddressMask(I2C_HW);
}
#endif /* (I2C_ENABLE_SLAVE) */

#if (I2C_ENABLE_MASTER)
/*******************************************************************************
* Function Name: I2C_MasterSetLowPhaseDutyCycle
****************************************************************************//**
*
* Invokes the Cy_SCB_I2C_MasterSetLowPhaseDutyCycle() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void I2C_MasterSetLowPhaseDutyCycle(uint32_t clockCycles)
{
    Cy_SCB_I2C_MasterSetLowPhaseDutyCycle(I2C_HW, clockCycles);
}


/*******************************************************************************
* Function Name: I2C_MasterSetHighPhaseDutyCycle
****************************************************************************//**
*
* Invokes the Cy_SCB_I2C_MasterSetHighPhaseDutyCycle() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void I2C_MasterSetHighPhaseDutyCycle(uint32_t clockCycles)
{
    Cy_SCB_I2C_MasterSetHighPhaseDutyCycle(I2C_HW, clockCycles);
}
#endif /* (I2C_ENABLE_MASTER) */


#if (I2C_ENABLE_SLAVE)
/*******************************************************************************
* Function Name: I2C_SlaveGetStatus
****************************************************************************//**
*
* Invokes the Cy_SCB_I2C_SlaveGetStatus() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t I2C_SlaveGetStatus(void)
{
    return Cy_SCB_I2C_SlaveGetStatus(I2C_HW, &I2C_context);
}


/*******************************************************************************
* Function Name: I2C_SlaveConfigReadBuf
****************************************************************************//**
*
* Invokes the Cy_SCB_I2C_SlaveConfigReadBuf() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void I2C_SlaveConfigReadBuf(uint8_t *buffer, uint32_t size)
{
    Cy_SCB_I2C_SlaveConfigReadBuf(I2C_HW, buffer, size, &I2C_context);
}


/*******************************************************************************
* Function Name: I2C_SlaveAbortRead
****************************************************************************//**
*
* Invokes the Cy_SCB_I2C_SlaveAbortRead() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void I2C_SlaveAbortRead(void)
{
    Cy_SCB_I2C_SlaveAbortRead(I2C_HW, &I2C_context);
}


/*******************************************************************************
* Function Name: I2C_SlaveGetReadTransferCount
****************************************************************************//**
*
* Invokes the Cy_SCB_I2C_SlaveGetReadTransferCount() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t I2C_SlaveGetReadTransferCount(void)
{
    return Cy_SCB_I2C_SlaveGetReadTransferCount(I2C_HW, &I2C_context);
}


/*******************************************************************************
* Function Name: I2C_SlaveClearReadStatus
****************************************************************************//**
*
* Invokes the Cy_SCB_I2C_SlaveClearReadStatus() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t I2C_SlaveClearReadStatus(void)
{
    return Cy_SCB_I2C_SlaveClearReadStatus(I2C_HW, &I2C_context);
}


/*******************************************************************************
* Function Name: I2C_SlaveConfigWriteBuf
****************************************************************************//**
*
* Invokes the Cy_SCB_I2C_SlaveConfigWriteBuf() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void I2C_SlaveConfigWriteBuf(uint8_t *buffer, uint32_t size)
{
    Cy_SCB_I2C_SlaveConfigWriteBuf(I2C_HW, buffer, size, &I2C_context);
}


/*******************************************************************************
* Function Name: I2C_SlaveAbortWrite
****************************************************************************//**
*
* Invokes the Cy_SCB_I2C_SlaveAbortWrite() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void I2C_SlaveAbortWrite(void)
{
    Cy_SCB_I2C_SlaveAbortWrite(I2C_HW, &I2C_context);
}


/*******************************************************************************
* Function Name: I2C_SlaveGetWriteTransferCount
****************************************************************************//**
*
* Invokes the Cy_SCB_I2C_SlaveGetWriteTransferCount() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t I2C_SlaveGetWriteTransferCount(void)
{
    return Cy_SCB_I2C_SlaveGetWriteTransferCount(I2C_HW, &I2C_context);
}


/*******************************************************************************
* Function Name: I2C_SlaveClearWriteStatus
****************************************************************************//**
*
* Invokes the Cy_SCB_I2C_SlaveClearWriteStatus() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t I2C_SlaveClearWriteStatus(void)
{
    return Cy_SCB_I2C_SlaveClearWriteStatus(I2C_HW, &I2C_context);
}
#endif /* (I2C_ENABLE_SLAVE) */


#if (I2C_ENABLE_MASTER)
/*******************************************************************************
* Function Name: I2C_MasterGetStatus
****************************************************************************//**
*
* Invokes the Cy_SCB_I2C_MasterGetStatus() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t I2C_MasterGetStatus(void)
{
    return Cy_SCB_I2C_MasterGetStatus(I2C_HW, &I2C_context);
}


/*******************************************************************************
* Function Name: I2C_MasterRead
****************************************************************************//**
*
* Invokes the Cy_SCB_I2C_MasterRead() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE cy_en_scb_i2c_status_t I2C_MasterRead(cy_stc_scb_i2c_master_xfer_config_t *xferConfig)
{
    return Cy_SCB_I2C_MasterRead(I2C_HW, xferConfig, &I2C_context);
}


/*******************************************************************************
* Function Name: I2C_MasterAbortRead
****************************************************************************//**
*
* Invokes the Cy_SCB_I2C_MasterAbortRead() PDL driver function.
*
******************************************************************************/
__STATIC_INLINE void I2C_MasterAbortRead(void)
{
    Cy_SCB_I2C_MasterAbortRead(I2C_HW, &I2C_context);
}


/*******************************************************************************
* Function Name: I2C_MasterWrite
****************************************************************************//**
*
* Invokes the Cy_SCB_I2C_MasterWrite() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE cy_en_scb_i2c_status_t I2C_MasterWrite(cy_stc_scb_i2c_master_xfer_config_t *xferConfig)
{
    return Cy_SCB_I2C_MasterWrite(I2C_HW, xferConfig, &I2C_context);
}


/*******************************************************************************
* Function Name: I2C_MasterAbortWrite
****************************************************************************//**
*
* Invokes the Cy_SCB_I2C_MasterAbortWrite() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void I2C_MasterAbortWrite(void)
{
    Cy_SCB_I2C_MasterAbortWrite(I2C_HW, &I2C_context);
}


/*******************************************************************************
* Function Name: I2C_MasterGetTransferCount
****************************************************************************//**
*
* Invokes the Cy_SCB_I2C_MasterGetTransferCount() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t I2C_MasterGetTransferCount(void)
{
    return Cy_SCB_I2C_MasterGetTransferCount(I2C_HW, &I2C_context);
}


/*******************************************************************************
* Function Name: I2C_MasterSendStart
****************************************************************************//**
*
* Invokes the Cy_SCB_I2C_MasterSendStart() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE cy_en_scb_i2c_status_t I2C_MasterSendStart(uint32_t address, cy_en_scb_i2c_direction_t bitRnW, uint32_t timeoutMs)
{
    return Cy_SCB_I2C_MasterSendStart(I2C_HW, address, bitRnW, timeoutMs, &I2C_context);
}


/*******************************************************************************
* Function Name: I2C_MasterSendReStart
****************************************************************************//**
*
* Invokes the Cy_SCB_I2C_MasterSendReStart() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE cy_en_scb_i2c_status_t I2C_MasterSendReStart(uint32_t address, cy_en_scb_i2c_direction_t bitRnW, uint32_t timeoutMs)
{
    return Cy_SCB_I2C_MasterSendReStart(I2C_HW, address, bitRnW, timeoutMs, &I2C_context);
}


/*******************************************************************************
* Function Name: I2C_MasterSendStop
****************************************************************************//**
*
* Invokes the Cy_SCB_I2C_MasterSendStop() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE cy_en_scb_i2c_status_t I2C_MasterSendStop(uint32_t timeoutMs)
{
    return Cy_SCB_I2C_MasterSendStop(I2C_HW, timeoutMs, &I2C_context);
}


/*******************************************************************************
* Function Name: I2C_MasterReadByte
****************************************************************************//**
*
* Invokes the Cy_SCB_I2C_MasterReadByte() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE cy_en_scb_i2c_status_t I2C_MasterReadByte(cy_en_scb_i2c_command_t ackNack, uint8_t *byte, uint32_t timeoutMs)
{
    return Cy_SCB_I2C_MasterReadByte(I2C_HW, ackNack, byte, timeoutMs, &I2C_context);
}


/*******************************************************************************
* Function Name: I2C_MasterWriteByte
****************************************************************************//**
*
* Invokes the Cy_SCB_I2C_MasterWriteByte() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE cy_en_scb_i2c_status_t I2C_MasterWriteByte(uint8_t byte, uint32_t timeoutMs)
{
    return Cy_SCB_I2C_MasterWriteByte(I2C_HW, byte, timeoutMs, &I2C_context);
}
#endif /* (I2C_ENABLE_MASTER) */


/*******************************************************************************
* Function Name: I2C_Interrupt
****************************************************************************//**
*
* Invokes the Cy_SCB_I2C_Interrupt() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void I2C_Interrupt(void)
{
    Cy_SCB_I2C_Interrupt(I2C_HW, &I2C_context);
}

#if defined(__cplusplus)
}
#endif

#endif /* I2C_CY_SCB_I2C_PDL_H */


/* [] END OF FILE */
