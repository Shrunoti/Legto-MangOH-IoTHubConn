

/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */

/**
 * @page c_bootReason Boot Reason Query
 *
 * @ref le_bootReason_interface.h "API Reference"
 *
 * <HR>
 *
 * This API can be used to determine the reason for the last OS boot when using @ref c_ulpm
 * (e.g., power on due to pre-configured time setty or GPIO toggled externally).
 *
 * Boot may occur due to exit from ultra low power mode or press on Power button. Exit from low
 * power state may occur through various sources (e.g., GPIO pins or timers that are specific to the
 * device hardware).
 *
 * @section bootReason_example Sample Code
 *
 * This C code sample sows an API calling sequence to get boot-reason:
 *
 *  @code
 *
 *  void CheckMDMBootReason
 *  (
 *      void
 *  )
 *  {
 *      if (le_bootReason_WasTimer())
 *      {
 *           LE_INFO("Timer boot");
 *      }
 *      else if (le_bootReason_WasGpio(<gpioNum>))
 *      {
 *           LE_INFO("GPIO<gpioNum> boot");
 *      }
 *      else
 *      {
 *          LE_INFO("Power-on");
 *      }
 *  }
 *
 *  @endcode
 *
 * <HR>
 *
 * Copyright (C) Sierra Wireless Inc.
 */
/**
 * @file le_bootReason_interface.h
 *
 * Legato @ref c_bootReason include file.
 *
 * Copyright (C) Sierra Wireless Inc.
 */

#ifndef LE_BOOTREASON_INTERFACE_H_INCLUDE_GUARD
#define LE_BOOTREASON_INTERFACE_H_INCLUDE_GUARD


#include "legato.h"


//--------------------------------------------------------------------------------------------------
/**
 * Type for handler called when a server disconnects.
 */
//--------------------------------------------------------------------------------------------------
typedef void (*le_bootReason_DisconnectHandler_t)(void *);

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
void le_bootReason_ConnectService
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
le_result_t le_bootReason_TryConnectService
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
void le_bootReason_SetServerDisconnectHandler
(
    le_bootReason_DisconnectHandler_t disconnectHandler,
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
void le_bootReason_DisconnectService
(
    void
);


//--------------------------------------------------------------------------------------------------
/**
 * Checks whether boot-reason was timer expiry.
 *
 * @return
 *      - TRUE if boot-reason was timer expiry.
 *      - FALSE otherwise.
 */
//--------------------------------------------------------------------------------------------------
bool le_bootReason_WasTimer
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * Checks whether boot-reason was specific gpio change. GPIO number is specified in parameter.
 *
 * @return
 *      - TRUE if boot-reason was specified gpio change.
 *      - FALSE otherwise.
 *
 * @note The process exits if invalid gpio number is passed.  Check corresponding device documents
 * for valid list of gpio.
 */
//--------------------------------------------------------------------------------------------------
bool le_bootReason_WasGpio
(
    uint32_t gpioNum
        ///< [IN] GPIO number.
);

//--------------------------------------------------------------------------------------------------
/**
 * Checks whether boot reason was due to the specified ADC having a reading above or below the
 * configured limits.
 *
 * @return
 *      true if boot reason was due to the given ADC or false otherwise.
 *
 * @note
 *      The process exits if an invalid ADC number is passed. Check corresponding device documents
 *      for valid list of ADC numbers.
 */
//--------------------------------------------------------------------------------------------------
bool le_bootReason_WasAdc
(
    uint32_t adcNum
        ///< [IN] ADC number
);

#endif // LE_BOOTREASON_INTERFACE_H_INCLUDE_GUARD