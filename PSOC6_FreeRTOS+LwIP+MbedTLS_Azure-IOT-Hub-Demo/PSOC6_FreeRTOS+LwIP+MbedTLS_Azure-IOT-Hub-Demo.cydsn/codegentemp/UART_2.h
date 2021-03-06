/***************************************************************************//**
* \file UART_2.h
* \version 2.0
*
*  This file provides constants and parameter values for the UART component.
*
********************************************************************************
* \copyright
* Copyright 2016-2017, Cypress Semiconductor Corporation. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(UART_2_CY_SCB_UART_PDL_H)
#define UART_2_CY_SCB_UART_PDL_H

#include "cyfitter.h"
#include "scb/cy_scb_uart.h"

#if defined(__cplusplus)
extern "C" {
#endif

/***************************************
*   Initial Parameter Constants
****************************************/

#define UART_2_DIRECTION  (3U)
#define UART_2_ENABLE_RTS (0U)
#define UART_2_ENABLE_CTS (0U)

/* UART direction enum */
#define UART_2_RX    (0x1U)
#define UART_2_TX    (0x2U)

#define UART_2_ENABLE_RX  (0UL != (UART_2_DIRECTION & UART_2_RX))
#define UART_2_ENABLE_TX  (0UL != (UART_2_DIRECTION & UART_2_TX))


/***************************************
*        Function Prototypes
***************************************/
/**
* \addtogroup group_general
* @{
*/
/* Component specific functions. */
void UART_2_Start(void);

/* Basic functions */
__STATIC_INLINE cy_en_scb_uart_status_t UART_2_Init(cy_stc_scb_uart_config_t const *config);
__STATIC_INLINE void UART_2_DeInit(void);
__STATIC_INLINE void UART_2_Enable(void);
__STATIC_INLINE void UART_2_Disable(void);

/* Register callback. */
__STATIC_INLINE void UART_2_RegisterCallback(cy_cb_scb_uart_handle_events_t callback);

/* Configuration change. */
#if (UART_2_ENABLE_CTS)
__STATIC_INLINE void UART_2_EnableCts(void);
__STATIC_INLINE void UART_2_DisableCts(void);
#endif /* (UART_2_ENABLE_CTS) */

#if (UART_2_ENABLE_RTS)
__STATIC_INLINE void     UART_2_SetRtsFifoLevel(uint32_t level);
__STATIC_INLINE uint32_t UART_2_GetRtsFifoLevel(void);
#endif /* (UART_2_ENABLE_RTS) */

__STATIC_INLINE void UART_2_EnableSkipStart(void);
__STATIC_INLINE void UART_2_DisableSkipStart(void);

#if (UART_2_ENABLE_RX)
/* Low level: Receive direction. */
__STATIC_INLINE uint32_t UART_2_Get(void);
__STATIC_INLINE uint32_t UART_2_GetArray(void *buffer, uint32_t size);
__STATIC_INLINE void     UART_2_GetArrayBlocking(void *buffer, uint32_t size);
__STATIC_INLINE uint32_t UART_2_GetRxFifoStatus(void);
__STATIC_INLINE void     UART_2_ClearRxFifoStatus(uint32_t clearMask);
__STATIC_INLINE uint32_t UART_2_GetNumInRxFifo(void);
__STATIC_INLINE void     UART_2_ClearRxFifo(void);
#endif /* (UART_2_ENABLE_RX) */

#if (UART_2_ENABLE_TX)
/* Low level: Transmit direction. */
__STATIC_INLINE uint32_t UART_2_Put(uint32_t data);
__STATIC_INLINE uint32_t UART_2_PutArray(void *buffer, uint32_t size);
__STATIC_INLINE void     UART_2_PutArrayBlocking(void *buffer, uint32_t size);
__STATIC_INLINE void     UART_2_PutString(char_t const string[]);
__STATIC_INLINE void     UART_2_SendBreakBlocking(uint32_t breakWidth);
__STATIC_INLINE uint32_t UART_2_GetTxFifoStatus(void);
__STATIC_INLINE void     UART_2_ClearTxFifoStatus(uint32_t clearMask);
__STATIC_INLINE uint32_t UART_2_GetNumInTxFifo(void);
__STATIC_INLINE bool     UART_2_IsTxComplete(void);
__STATIC_INLINE void     UART_2_ClearTxFifo(void);
#endif /* (UART_2_ENABLE_TX) */

#if (UART_2_ENABLE_RX)
/* High level: Ring buffer functions. */
__STATIC_INLINE void     UART_2_StartRingBuffer(void *buffer, uint32_t size);
__STATIC_INLINE void     UART_2_StopRingBuffer(void);
__STATIC_INLINE void     UART_2_ClearRingBuffer(void);
__STATIC_INLINE uint32_t UART_2_GetNumInRingBuffer(void);

/* High level: Receive direction functions. */
__STATIC_INLINE cy_en_scb_uart_status_t UART_2_Receive(void *buffer, uint32_t size);
__STATIC_INLINE void     UART_2_AbortReceive(void);
__STATIC_INLINE uint32_t UART_2_GetReceiveStatus(void);
__STATIC_INLINE uint32_t UART_2_GetNumReceived(void);
#endif /* (UART_2_ENABLE_RX) */

#if (UART_2_ENABLE_TX)
/* High level: Transmit direction functions. */
__STATIC_INLINE cy_en_scb_uart_status_t UART_2_Transmit(void *buffer, uint32_t size);
__STATIC_INLINE void     UART_2_AbortTransmit(void);
__STATIC_INLINE uint32_t UART_2_GetTransmitStatus(void);
__STATIC_INLINE uint32_t UART_2_GetNumLeftToTransmit(void);
#endif /* (UART_2_ENABLE_TX) */

/* Interrupt handler */
__STATIC_INLINE void UART_2_Interrupt(void);
/** @} group_general */


/***************************************
*    Variables with External Linkage
***************************************/
/**
* \addtogroup group_globals
* @{
*/
extern uint8_t UART_2_initVar;
extern cy_stc_scb_uart_config_t const UART_2_config;
extern cy_stc_scb_uart_context_t UART_2_context;
/** @} group_globals */


/***************************************
*         Preprocessor Macros
***************************************/
/**
* \addtogroup group_macros
* @{
*/
/** The pointer to the base address of the hardware */
#define UART_2_HW     ((CySCB_Type *) UART_2_SCB__HW)
/** @} group_macros */


/***************************************
*    In-line Function Implementation
***************************************/

/*******************************************************************************
* Function Name: UART_2_Init
****************************************************************************//**
*
* Invokes the Cy_SCB_UART_Init() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE cy_en_scb_uart_status_t UART_2_Init(cy_stc_scb_uart_config_t const *config)
{
   return Cy_SCB_UART_Init(UART_2_HW, config, &UART_2_context);
}


/*******************************************************************************
* Function Name: UART_2_DeInit
****************************************************************************//**
*
* Invokes the Cy_SCB_UART_DeInit() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void UART_2_DeInit(void)
{
    Cy_SCB_UART_DeInit(UART_2_HW);
}


/*******************************************************************************
* Function Name: UART_2_Enable
****************************************************************************//**
*
* Invokes the Cy_SCB_UART_Enable() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void UART_2_Enable(void)
{
    Cy_SCB_UART_Enable(UART_2_HW);
}


/*******************************************************************************
* Function Name: UART_2_Disable
****************************************************************************//**
*
* Invokes the Cy_SCB_UART_Disable() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void UART_2_Disable(void)
{
    Cy_SCB_UART_Disable(UART_2_HW, &UART_2_context);
}


/*******************************************************************************
* Function Name: UART_2_RegisterCallback
****************************************************************************//**
*
* Invokes the Cy_SCB_UART_RegisterCallback() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void UART_2_RegisterCallback(cy_cb_scb_uart_handle_events_t callback)
{
    Cy_SCB_UART_RegisterCallback(UART_2_HW, callback, &UART_2_context);
}


#if (UART_2_ENABLE_CTS)
/*******************************************************************************
* Function Name: UART_2_EnableCts
****************************************************************************//**
*
* Invokes the Cy_SCB_UART_EnableCts() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void UART_2_EnableCts(void)
{
    Cy_SCB_UART_EnableCts(UART_2_HW);
}


/*******************************************************************************
* Function Name: Cy_SCB_UART_DisableCts
****************************************************************************//**
*
* Invokes the Cy_SCB_UART_DisableCts() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void UART_2_DisableCts(void)
{
    Cy_SCB_UART_DisableCts(UART_2_HW);
}
#endif /* (UART_2_ENABLE_CTS) */


#if (UART_2_ENABLE_RTS)
/*******************************************************************************
* Function Name: UART_2_SetRtsFifoLevel
****************************************************************************//**
*
* Invokes the Cy_SCB_UART_SetRtsFifoLevel() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void UART_2_SetRtsFifoLevel(uint32_t level)
{
    Cy_SCB_UART_SetRtsFifoLevel(UART_2_HW, level);
}


/*******************************************************************************
* Function Name: UART_2_GetRtsFifoLevel
****************************************************************************//**
*
* Invokes the Cy_SCB_UART_GetRtsFifoLevel() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t UART_2_GetRtsFifoLevel(void)
{
    return Cy_SCB_UART_GetRtsFifoLevel(UART_2_HW);
}
#endif /* (UART_2_ENABLE_RTS) */


/*******************************************************************************
* Function Name: UART_2_EnableSkipStart
****************************************************************************//**
*
* Invokes the Cy_SCB_UART_EnableSkipStart() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void UART_2_EnableSkipStart(void)
{
    Cy_SCB_UART_EnableSkipStart(UART_2_HW);
}


/*******************************************************************************
* Function Name: UART_2_DisableSkipStart
****************************************************************************//**
*
* Invokes the Cy_SCB_UART_DisableSkipStart() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void UART_2_DisableSkipStart(void)
{
    Cy_SCB_UART_DisableSkipStart(UART_2_HW);
}


#if (UART_2_ENABLE_RX)
/*******************************************************************************
* Function Name: UART_2_Get
****************************************************************************//**
*
* Invokes the Cy_SCB_UART_Get() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t UART_2_Get(void)
{
    return Cy_SCB_UART_Get(UART_2_HW);
}


/*******************************************************************************
* Function Name: UART_2_GetArray
****************************************************************************//**
*
* Invokes the Cy_SCB_UART_GetArray() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t UART_2_GetArray(void *buffer, uint32_t size)
{
    return Cy_SCB_UART_GetArray(UART_2_HW, buffer, size);
}


/*******************************************************************************
* Function Name: UART_2_GetArrayBlocking
****************************************************************************//**
*
* Invokes the Cy_SCB_UART_GetArrayBlocking() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void UART_2_GetArrayBlocking(void *buffer, uint32_t size)
{
    Cy_SCB_UART_GetArrayBlocking(UART_2_HW, buffer, size);
}


/*******************************************************************************
* Function Name: UART_2_GetRxFifoStatus
****************************************************************************//**
*
* Invokes the Cy_SCB_UART_GetRxFifoStatus() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t UART_2_GetRxFifoStatus(void)
{
    return Cy_SCB_UART_GetRxFifoStatus(UART_2_HW);
}


/*******************************************************************************
* Function Name: UART_2_ClearRxFifoStatus
****************************************************************************//**
*
* Invokes the Cy_SCB_UART_ClearRxFifoStatus() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void UART_2_ClearRxFifoStatus(uint32_t clearMask)
{
    Cy_SCB_UART_ClearRxFifoStatus(UART_2_HW, clearMask);
}


/*******************************************************************************
* Function Name: UART_2_GetNumInRxFifo
****************************************************************************//**
*
* Invokes the Cy_SCB_UART_GetNumInRxFifo() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t UART_2_GetNumInRxFifo(void)
{
    return Cy_SCB_UART_GetNumInRxFifo(UART_2_HW);
}


/*******************************************************************************
* Function Name: UART_2_ClearRxFifo
****************************************************************************//**
*
* Invokes the Cy_SCB_UART_ClearRxFifo() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void UART_2_ClearRxFifo(void)
{
    Cy_SCB_UART_ClearRxFifo(UART_2_HW);
}
#endif /* (UART_2_ENABLE_RX) */


#if (UART_2_ENABLE_TX)
/*******************************************************************************
* Function Name: UART_2_Put
****************************************************************************//**
*
* Invokes the Cy_SCB_UART_Put() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t UART_2_Put(uint32_t data)
{
    return Cy_SCB_UART_Put(UART_2_HW,data);
}


/*******************************************************************************
* Function Name: UART_2_PutArray
****************************************************************************//**
*
* Invokes the Cy_SCB_UART_PutArray() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t UART_2_PutArray(void *buffer, uint32_t size)
{
    return Cy_SCB_UART_PutArray(UART_2_HW, buffer, size);
}


/*******************************************************************************
* Function Name: UART_2_PutArrayBlocking
****************************************************************************//**
*
* Invokes the Cy_SCB_UART_PutArrayBlocking() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void UART_2_PutArrayBlocking(void *buffer, uint32_t size)
{
    Cy_SCB_UART_PutArrayBlocking(UART_2_HW, buffer, size);
}


/*******************************************************************************
* Function Name: UART_2_PutString
****************************************************************************//**
*
* Invokes the Cy_SCB_UART_PutString() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void UART_2_PutString(char_t const string[])
{
    Cy_SCB_UART_PutString(UART_2_HW, string);
}


/*******************************************************************************
* Function Name: UART_2_SendBreakBlocking
****************************************************************************//**
*
* Invokes the Cy_SCB_UART_SendBreakBlocking() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void UART_2_SendBreakBlocking(uint32_t breakWidth)
{
    Cy_SCB_UART_SendBreakBlocking(UART_2_HW, breakWidth);
}


/*******************************************************************************
* Function Name: UART_2_GetTxFifoStatus
****************************************************************************//**
*
* Invokes the Cy_SCB_UART_GetTxFifoStatus() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t UART_2_GetTxFifoStatus(void)
{
    return Cy_SCB_UART_GetTxFifoStatus(UART_2_HW);
}


/*******************************************************************************
* Function Name: UART_2_ClearTxFifoStatus
****************************************************************************//**
*
* Invokes the Cy_SCB_UART_ClearTxFifoStatus() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void UART_2_ClearTxFifoStatus(uint32_t clearMask)
{
    Cy_SCB_UART_ClearTxFifoStatus(UART_2_HW, clearMask);
}


/*******************************************************************************
* Function Name: UART_2_GetNumInTxFifo
****************************************************************************//**
*
* Invokes the Cy_SCB_UART_GetNumInTxFifo() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t UART_2_GetNumInTxFifo(void)
{
    return Cy_SCB_UART_GetNumInTxFifo(UART_2_HW);
}


/*******************************************************************************
* Function Name: UART_2_IsTxComplete
****************************************************************************//**
*
* Invokes the Cy_SCB_UART_IsTxComplete() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE bool UART_2_IsTxComplete(void)
{
    return Cy_SCB_UART_IsTxComplete(UART_2_HW);
}


/*******************************************************************************
* Function Name: UART_2_ClearTxFifo
****************************************************************************//**
*
* Invokes the Cy_SCB_UART_ClearTxFifo() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void UART_2_ClearTxFifo(void)
{
    Cy_SCB_UART_ClearTxFifo(UART_2_HW);
}
#endif /* (UART_2_ENABLE_TX) */


#if (UART_2_ENABLE_RX)
/*******************************************************************************
* Function Name: UART_2_StartRingBuffer
****************************************************************************//**
*
* Invokes the Cy_SCB_UART_StartRingBuffer() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void UART_2_StartRingBuffer(void *buffer, uint32_t size)
{
    Cy_SCB_UART_StartRingBuffer(UART_2_HW, buffer, size, &UART_2_context);
}


/*******************************************************************************
* Function Name: UART_2_StopRingBuffer
****************************************************************************//**
*
* Invokes the Cy_SCB_UART_StopRingBuffer() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void UART_2_StopRingBuffer(void)
{
    Cy_SCB_UART_StopRingBuffer(UART_2_HW, &UART_2_context);
}


/*******************************************************************************
* Function Name: UART_2_ClearRingBuffer
****************************************************************************//**
*
* Invokes the Cy_SCB_UART_ClearRingBuffer() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void UART_2_ClearRingBuffer(void)
{
    Cy_SCB_UART_ClearRingBuffer(UART_2_HW, &UART_2_context);
}


/*******************************************************************************
* Function Name: UART_2_GetNumInRingBuffer
****************************************************************************//**
*
* Invokes the Cy_SCB_UART_GetNumInRingBuffer() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t UART_2_GetNumInRingBuffer(void)
{
    return Cy_SCB_UART_GetNumInRingBuffer(UART_2_HW, &UART_2_context);
}


/*******************************************************************************
* Function Name: UART_2_Receive
****************************************************************************//**
*
* Invokes the Cy_SCB_UART_Receive() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE cy_en_scb_uart_status_t UART_2_Receive(void *buffer, uint32_t size)
{
    return Cy_SCB_UART_Receive(UART_2_HW, buffer, size, &UART_2_context);
}


/*******************************************************************************
* Function Name: UART_2_GetReceiveStatus
****************************************************************************//**
*
* Invokes the Cy_SCB_UART_GetReceiveStatus() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t UART_2_GetReceiveStatus(void)
{
    return Cy_SCB_UART_GetReceiveStatus(UART_2_HW, &UART_2_context);
}


/*******************************************************************************
* Function Name: UART_2_AbortReceive
****************************************************************************//**
*
* Invokes the Cy_SCB_UART_AbortReceive() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void UART_2_AbortReceive(void)
{
    Cy_SCB_UART_AbortReceive(UART_2_HW, &UART_2_context);
}


/*******************************************************************************
* Function Name: UART_2_GetNumReceived
****************************************************************************//**
*
* Invokes the Cy_SCB_UART_GetNumReceived() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t UART_2_GetNumReceived(void)
{
    return Cy_SCB_UART_GetNumReceived(UART_2_HW, &UART_2_context);
}
#endif /* (UART_2_ENABLE_RX) */


#if (UART_2_ENABLE_TX)
/*******************************************************************************
* Function Name: UART_2_Transmit
****************************************************************************//**
*
* Invokes the Cy_SCB_UART_Transmit() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE cy_en_scb_uart_status_t UART_2_Transmit(void *buffer, uint32_t size)
{
    return Cy_SCB_UART_Transmit(UART_2_HW, buffer, size, &UART_2_context);
}


/*******************************************************************************
* Function Name: UART_2_GetTransmitStatus
****************************************************************************//**
*
* Invokes the Cy_SCB_UART_GetTransmitStatus() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t UART_2_GetTransmitStatus(void)
{
    return Cy_SCB_UART_GetTransmitStatus(UART_2_HW, &UART_2_context);
}


/*******************************************************************************
* Function Name: UART_2_AbortTransmit
****************************************************************************//**
*
* Invokes the Cy_SCB_UART_AbortTransmit() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void UART_2_AbortTransmit(void)
{
    Cy_SCB_UART_AbortTransmit(UART_2_HW, &UART_2_context);
}


/*******************************************************************************
* Function Name: UART_2_GetNumLeftToTransmit
****************************************************************************//**
*
* Invokes the Cy_SCB_UART_GetNumLeftToTransmit() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t UART_2_GetNumLeftToTransmit(void)
{
    return Cy_SCB_UART_GetNumLeftToTransmit(UART_2_HW, &UART_2_context);
}
#endif /* (UART_2_ENABLE_TX) */


/*******************************************************************************
* Function Name: UART_2_Interrupt
****************************************************************************//**
*
* Invokes the Cy_SCB_UART_Interrupt() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void UART_2_Interrupt(void)
{
    Cy_SCB_UART_Interrupt(UART_2_HW, &UART_2_context);
}

#if defined(__cplusplus)
}
#endif

#endif /* UART_2_CY_SCB_UART_PDL_H */


/* [] END OF FILE */
