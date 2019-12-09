

/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */

/**
 * @page c_rtc User Timebase
 *
 * @ref le_rtc_interface.h "API Reference" <br>
 * @ref howToSetUserTimebase how-to
 * <HR>
 *
 * This API provides definitions to read and set the @e user timebase for a target's real time clock (RTC).
 *
 * Usually, the time daemon automatically updates the system time (i.e., gets time value
 * when calling le_clk_GetAbsoluteTime() or using Linux @c date command). In some cases, the RTC
 * time value may require a user timebase value based on other domains (e.g., GPS) to offset
 * differences between the battery-powered RTC and the system time.
 *
 * If you're synchronizng with cellular network time only, the time daemon will update the
 * system time (the time you get when calling le_clk_GetAbsoluteTime() or using linux 'date'
 * command) automatically.
 *
 * @section c_rtc_disableDaemon Disable Time Daemon
 *
 * If you need to set the system time from a saved user timebase value, first disable the time
 * daemon by commenting out the
 * @c TIME_SERVICES="qcom_time" line in @c /etc/time_service.conf to prevent the system time from being
 * updated if the modem receives a new time from a cellular network.
 *
 * @section c_rtc_getSet Get/Set Time Value
 *
 * You use le_rtc_GetUserTime() to read the current RTC time, and le_rtc_SetUserTime()
 * to set the user timebase adjustment (plus/minus in milliseconds).
 *
 * The time stored in the user timebase is incremented on a millisecond basis;
 * Unix time is incremented by the second. When storing Unix time in the user timebase, it
 * must be multiplied by 1000, and must be divided by 1000 when retrieved.
 *
 * Typically, it's sufficient to use Unix epoch time x 1000 as the time base.
 *
 * <HR>
 *
 * Copyright (C) Sierra Wireless Inc.
 */
/**
 * @file le_rtc_interface.h
 *
 * Legato @ref c_rtc include file.
 *
 * Copyright (C) Sierra Wireless Inc.
 */

#ifndef LE_RTC_INTERFACE_H_INCLUDE_GUARD
#define LE_RTC_INTERFACE_H_INCLUDE_GUARD


#include "legato.h"


//--------------------------------------------------------------------------------------------------
/**
 * Type for handler called when a server disconnects.
 */
//--------------------------------------------------------------------------------------------------
typedef void (*le_rtc_DisconnectHandler_t)(void *);

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
void le_rtc_ConnectService
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
le_result_t le_rtc_TryConnectService
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
void le_rtc_SetServerDisconnectHandler
(
    le_rtc_DisconnectHandler_t disconnectHandler,
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
void le_rtc_DisconnectService
(
    void
);


//--------------------------------------------------------------------------------------------------
/**
 * Get the current time from the RTC
 *
 * @return
 *      - LE_OK            Function succeeded.
 *      - LE_FAULT         Function failed.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_rtc_GetUserTime
(
    uint64_t* millisecondsPastGpsEpochPtr
        ///< [OUT]
);

//--------------------------------------------------------------------------------------------------
/**
 * Set the RTC with the given time string
 *
 * @return
 *      - LE_OK            Function succeeded.
 *      - LE_FAULT         Function failed.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_rtc_SetUserTime
(
    uint64_t millisecondsPastGpsEpoch
        ///< [IN]
);

#endif // LE_RTC_INTERFACE_H_INCLUDE_GUARD