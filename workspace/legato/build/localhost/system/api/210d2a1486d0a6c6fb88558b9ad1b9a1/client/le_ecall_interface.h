

/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */

/**
 * @page c_ecall eCall
 *
 * @ref le_ecall_interface.h "API Reference" <br>
 * @ref legatoServicesModemECall <br>
 * @ref sampleApps_eCall sample app <br>
 * @ref le_ecall_samples <br>
 *
 * eCall is a technology initiative intended to bring rapid assistance to auto
 * accidents anywhere in the European Union. When a serious vehicle accident occurs,
 * sensors automatically trigger an eCall. When activated, the in-vehicle system (IVS)
 * establishes a 112-voice connection.
 *
 * The Mobile Network Operator handles the eCall like any other 112 call and routes the call to the
 * most appropriate emergency response centre - Public Safety Answering Point (PSAP).
 *
 * At the same time, a digital "minimum set of data" (MSD) message is sent over the voice call using
 * in-band modem signals. The MSD includes accident information like time, location, driving
 * direction, and vehicle description.
 *
 * The eCall can also be activated manually. The mobile network operator identifies that the 112
 * call is an eCall from the “eCall flag” inserted by the vehicle’s communication module.
 *
 * This API applies for both PAN-EUROPEAN and ERA-GLONASS standards.
 *
 * @note eCall service is only available on automotive products.
 *
 * @section le_ecall_binding IPC interfaces binding
 *
 * All the functions of this API are provided by the @b modemService service.
 *
 * Here's a code sample binding to modem services:
 * @verbatim
   bindings:
   {
      clientExe.clientComponent.le_ecall -> modemService.le_ecall
   }
   @endverbatim
 *
 * @section le_ecall_operation Operation modes
 *
 * The modem can be configured to operate in three different operation modes:
 * - le_ecall_ForceOnlyMode(): this function configures the eCall operation mode to eCall only, only
 *   emergency number can be used to start an eCall session. The modem doesn't try to register on
 *   the Cellular network. This function forces the modem to behave as eCall only mode whatever
 *   U/SIM operation mode. The change doesn't persist over power cycles.
 *   This function can be called before making an eCall.
 *
 * - le_ecall_ForcePersistentOnlyMode(): Same as le_ecall_ForceOnlyMode(), but the change persists
 *   over power cycles.
 *
 * - le_ecall_ExitOnlyMode(): this function exits from eCall Only mode. It configures the eCall
 *   operation mode to Normal mode, the modem uses the default operation mode at power up (or after
 *   U/SIM hotswap). The modem behaves following the U/SIM eCall operation mode; for example the
 *   U/SIM can be configured only for eCall, or a combination of eCall and commercial service
 *   provision.
 *
 * - le_ecall_GetConfiguredOperationMode(): this function allows the user to retrieve the configured
 *   Operation mode.
 *   The configured operation mode can be:
 *   - @c LE_ECALL_NORMAL_MODE : normal mode. The modem behaves following the U/SIM eCall operation
 *                               mode.
 *   - @c LE_ECALL_ONLY_MODE : eCall only mode according to U/SIM operation mode or forced by
 *                             application through the le_ecall_ForceOnlyMode() function.
 *   - @c LE_ECALL_FORCED_PERSISTENT_ONLY_MODE : persistent eCall only mode.
 *
 * @section le_ecall_session eCall Session
 *
 * An eCall session is started by creating an eCall object with the le_ecall_Create() function.
 * The eCall session can then be stopped with le_ecall_End().
 *
 * The eCall type and the kind of activation are specified using different functions to start the
 * eCall session:
 * - le_ecall_StartManual(): initiate a manual eCall session (triggered by a passenger)
 * - le_ecall_StartAutomatic(): initiate an automatic eCall session (automatically triggered by
 * the IVS in case of accident)
 * - le_ecall_StartTest(): initiate a test eCall session (to test the communication between the
 * IVS and the PSAP)
 *
 * @warning An application must wait for the end of the ongoing eCall before triggering another
 * one with the @c le_ecall_StartXxx() functions. An eCall is ended when:
 * - the application successfully ended it with le_ecall_End() returning @c LE_OK
 * - or the event @ref LE_ECALL_STATE_STOPPED is received
 * - or the event @ref LE_ECALL_STATE_ALACK_RECEIVED_CLEAR_DOWN is received
 * - or the event @ref LE_ECALL_STATE_DISCONNECTED is received and no redial is launched (see
 * @ref le_ecall_redial section for more information about the redial process).
 *
 * When the eCall object is no longer needed, call le_ecall_Delete() to free all allocated
 * resources associated with the object.
 *
 * The current state of an eCall session can be queried using le_ecall_GetState().
 * Alternatively, an application can register a handler be notified when the session state
 * changes. The handler can be managed using le_ecall_AddStateChangeHandler() and
 * le_ecall_RemoveStateChangeHandler().
 *
 * An application can also call le_ecall_GetTerminationReason() to retrieve the reason of the call
 * termination when call state is @ref LE_ECALL_STATE_DISCONNECTED, and also
 * le_ecall_GetPlatformSpecificTerminationCode() to get platform specific termination code (refer
 * to your platform documentation for further details).
 *
 * @section le_ecall_concurrency Concurrency
 *
 * If another application tries to use the eCall service while a session is already in progress, the
 * le_ecall_StartManual(), le_ecall_StartAutomatic(), le_ecall_StartTest() functions will return a
 * @c LE_BUSY error. The eCall session in progress won't be interrupted or disturbed. The
 * application can follow the session progress with 'state' functions like le_ecall_GetState() and
 * le_ecall_AddStateChangeHandler().
 * A manual eCall can't interrupt an automatic eCall, and an automatic eCall can't interrupt a
 * manual eCall.
 *
 * @section le_ecall_msd Minimum Set of Data (MSD)
 *
 * The dynamic values of the MSD can be set with:
 * - le_ecall_SetMsdPosition() sets the position of the vehicle.
 * - le_ecall_SetMsdPositionN1() sets the first delta position of the vehicle.
 * - le_ecall_SetMsdPositionN2() sets the second delta position of the vehicle.
 * - le_ecall_SetMsdPassengersCount() sets the number of passengers.
 *
 * The MSD is automatically encoded with the values previously set.
 *
 * @warning Those functions return a LE_DUPLICATE error when the MSD has been already imported with
 * le_ecall_ImportMsd() function.
 *
 * The MSD transmission mode can be set or get with:
 *  - le_ecall_SetMsdTxMode()
 *  - le_ecall_GetMsdTxMode()
 *
 * The transmission mode can be:
 * - @c LE_ECALL_TX_MODE_PUSH : the MSD is pushed by the IVS
 * - @c LE_ECALL_TX_MODE_PULL : the MSD is sent when requested by the PSAP
 *
 * It's possible to import a prepared MSD using the le_ecall_ImportMsd() function.
 * The prepared MSD must answer the requirements described in the "EN 15722:2013" publication (this
 * publication has been prepared by Technical Committee CEN/TC 278 “Intelligent Transport Systems").
 *
 * @warning The imported MSD doesn't take into account the values provided by the
 * le_ecall_SetMsdXxx() functions. It overwrites any previous imported MSD or encoded MSD.
 *
 * @warning The imported MSD overwrites the control flags (automaticActivation and testCall) set by
 * le_ecall_StartXxx() functions (Manual, Automatic, Test). The User App is in charge of their
 * correct settings.
 *
 * The encoded MSD can be retrieved with le_ecall_ExportMsd() function.
 *
 * @note The User app must perform the MSD transmission by calling le_ecall_SendMsd() when the
 * LE_ECALL_STATE_PSAP_START_IND_RECEIVED event is received.
 * The MSD can be updated before calling le_ecall_SendMsd(), using the e_ecall_ImportMsd() function
 * or the le_ecall_SetMsdXxx() functions.
 *
 * Date/time on MSD information is set automatically by fetching the time from linux system time.
 *
 * @note If linux system time is not updated automatically, update system date/time with
 * le_clk_SetAbsoluteTime() function.
 *
 * Time can be retrieved by using the following methods:
 * - le_data_GetTime()/le_data_GetDate(): retrieves the time from the network NTP server
 * - le_pos_GetTime()/le_pos_GetDate(): retrieves the time from global positioning
 *
 * @section le_ecall_eraglonass ERA-GLONASS compliancy
 *
 * To perform an emergency call following the ERA-GLONASS requirements, the 'systemStandard' entry
 * of the configuration database must be set to 'ERA-GLONASS'.
 *
 * Moreover, the User can set some specific configuration settings in accordance with the PSAP
 * configuration:
 *
 * - le_ecall_SetEraGlonassManualDialAttempts(): set the ECALL_MANUAL_DIAL_ATTEMPTS value. If a dial
 *   attempt under manual emergency call initiation failed, it should be repeated maximally
 *   ECALL_MANUAL_DIAL_ATTEMPTS-1 times within the maximal time limit of ECALL_DIAL_DURATION.
 *   The default value is 10.
 *   Redial attempts stop once the call has been cleared down correctly, or if counter / timer
 *   reached their limits.
 *   Available for both manual and test modes.
 * - le_ecall_SetEraGlonassAutoDialAttempts(): set the ECALL_AUTO_DIAL_ATTEMPTS value. If a dial
 *   attempt under automatic emergency call initiation failed, it should be repeated maximally
 *   ECALL_AUTO_DIAL_ATTEMPTS-1 times within the maximal time limit of ECALL_DIAL_DURATION.
 *   The default value is 10.
 *   Redial attempts stop once the call has been cleared down correctly, or if counter / timer
 *   reached their limits.
 * - le_ecall_SetEraGlonassDialDuration(): set the ECALL_DIAL_DURATION time. It is the maximum time
 *   the IVS have to connect the emergency call to the PSAP, including all redial attempts.
 *   If the call is not connected within this time (or ManualDialAttempts/AutoDialAttempts dial
 *   attempts), it will stop.
 * - le_ecall_SetEraGlonassFallbackTime(): set the ECALL_CCFT time. When the specified timer
 *   duration is reached and IVS NAD didn't receive a call clear-down indication then the call
 *   is automatically terminated.
 * - le_ecall_SetEraGlonassAutoAnswerTime(): sets the ECALL_AUTO_ANSWER_TIME time, the
 *   maximum time interval wherein IVDS responds to incoming calls automatically after emergency
 *   call completion.
 * - le_ecall_SetEraGlonassMSDMaxTransmissionTime(): sets the ECALL_MSD_MAX_TRANSMISSION_TIME time,
 *   the maximum time for MSD transmission.
 *
 * The corresponding getter functions let you retrieve the configuration settings values:
 *
 * - le_ecall_GetEraGlonassManualDialAttempts(): get the ECALL_MANUAL_DIAL_ATTEMPTS value.
 * - le_ecall_GetEraGlonassAutoDialAttempts(): get the ECALL_AUTO_DIAL_ATTEMPTS value.
 * - le_ecall_GetEraGlonassDialDuration(): get the ECALL_DIAL_DURATION time.
 * - le_ecall_GetEraGlonassFallbackTime(): get the ECALL_CCFT time.
 * - le_ecall_GetEraGlonassAutoAnswerTime(): get the ECALL_AUTO_ANSWER_TIME time.
 * - le_ecall_GetEraGlonassMSDMaxTransmissionTime(): get the ECALL_MSD_MAX_TRANSMISSION_TIME time.
 *
 * @section le_ecall_eraGlonassData ERA-GLONASS MSD additional data
 *
 * ERA-GLONASS additional data are optional and provided in the MSD message if any.
 * They are located in MSD data block number 12 as optional additional data.
 *
 * ERA-GLONASS MSD additional data for MSD version 1 describes:
 * - The crash severity (Accident Severity Index - ASI15)
 * - The diagnostic result
 * - The crash information
 *
 * ERA-GLONASS MSD additional data for MSD version 2 describes:
 * - The crash severity (Accident Severity Index - ASI15)
 * - The diagnostic result
 * - The crash information
 * - The coordinate system type
 *
 * ERA-GLONASS MSD additional data can be specified through the following functions:
 * - le_ecall_SetMsdEraGlonassCrashSeverity().
 * - le_ecall_ResetMsdEraGlonassCrashSeverity().
 * - le_ecall_SetMsdEraGlonassDiagnosticResult().
 * - le_ecall_ResetMsdEraGlonassDiagnosticResult().
 * - le_ecall_SetMsdEraGlonassCrashInfo().
 * - le_ecall_ResetMsdEraGlonassCrashInfo().
 * - le_ecall_SetMsdEraGlonassCoordinateSystemType().
 * - le_ecall_ResetMsdEraGlonassCoordinateSystemType().
 *
 * ERA-GLONASS additional data is encoded using the OID version "1.4.1" for
 * MSD version 1 and the OID version "1.4.2" for MSD version 2.
 * This was assigned to ERA-GLONASS optional additional data by CEN.
 *
 * Content of data block in the AdditionalData for MSD version 1 should be:
 *
 * @code
 *
 * ERAOADASN1Module
 * DEFINITIONS
 * AUTOMATIC TAGS ::=
 * BEGIN
 * ERADataFormatId::= INTEGER (1)
 *
 * ERAAdditionalData ::= SEQUENCE {
 *    crashSeverity INTEGER(0..2047) OPTIONAL,
 *    diagnosticResult DiagnosticResult OPTIONAL,
 *    crashInfo CrashInfo OPTIONAL,
 *    ...
 * }
 *
 * DiagnosticResult ::= SEQUENCE {
 *    micConnectionFailure BOOLEAN OPTIONAL,
 *    micFailure BOOLEAN OPTIONAL,
 *    rightSpeakerFailure BOOLEAN OPTIONAL,
 *    leftSpeakerFailure BOOLEAN OPTIONAL,
 *    speakersFailure BOOLEAN OPTIONAL,
 *    ignitionLineFailure BOOLEAN OPTIONAL,
 *    uimFailure BOOLEAN OPTIONAL,
 *    statusIndicatorFailure BOOLEAN OPTIONAL,
 *    batteryFailure BOOLEAN OPTIONAL,
 *    batteryVoltageLow BOOLEAN OPTIONAL,
 *    crashSensorFailure BOOLEAN OPTIONAL,
 *    firmwareImageCorruption BOOLEAN OPTIONAL,
 *    commModuleInterfaceFailure BOOLEAN OPTIONAL,
 *    gnssReceiverFailure BOOLEAN OPTIONAL,
 *    raimProblem BOOLEAN OPTIONAL,
 *    gnssAntennaFailure BOOLEAN OPTIONAL,
 *    commModuleFailure BOOLEAN OPTIONAL,
 *    eventsMemoryOverflow BOOLEAN OPTIONAL,
 *    crashProfileMemoryOverflow BOOLEAN OPTIONAL,
 *    otherCriticalFailires BOOLEAN OPTIONAL,
 *    otherNotCriticalFailures BOOLEAN OPTIONAL
 * }
 *
 * CrashInfo ::= SEQUENCE {
 *    crashFront BOOLEAN OPTIONAL,
 *    crashLeft BOOLEAN OPTIONAL,
 *    crashRight BOOLEAN OPTIONAL,
 *    crashRear BOOLEAN OPTIONAL,
 *    crashRollover BOOLEAN OPTIONAL,
 *    crashSide BOOLEAN OPTIONAL,
 *    crashFrontOrSide BOOLEAN OPTIONAL,
 *    crashAnotherType BOOLEAN OPTIONAL
 * }
 * END
 * @endcode
 *
 * Content of data block in the AdditionalData for MSD version 2 should be:
 *
 * @code
 *
 * ERAOADASN1Module
 * DEFINITIONS
 * AUTOMATIC TAGS ::=
 * BEGIN
 * ERADataFormatId::= INTEGER (1)
 *
 * ERAAdditionalData ::= SEQUENCE {
 *    crashSeverity INTEGER(0..2047) OPTIONAL,
 *    diagnosticResult DiagnosticResult OPTIONAL,
 *    crashInfo CrashInfo OPTIONAL,
 *    coordinateSystemType CoordinateSystemType DEFAULT wgs84,
 *    ...
 * }
 *
 * DiagnosticResult ::= SEQUENCE {
 *    micConnectionFailure BOOLEAN OPTIONAL,
 *    micFailure BOOLEAN OPTIONAL,
 *    rightSpeakerFailure BOOLEAN OPTIONAL,
 *    leftSpeakerFailure BOOLEAN OPTIONAL,
 *    speakersFailure BOOLEAN OPTIONAL,
 *    ignitionLineFailure BOOLEAN OPTIONAL,
 *    uimFailure BOOLEAN OPTIONAL,
 *    statusIndicatorFailure BOOLEAN OPTIONAL,
 *    batteryFailure BOOLEAN OPTIONAL,
 *    batteryVoltageLow BOOLEAN OPTIONAL,
 *    crashSensorFailure BOOLEAN OPTIONAL,
 *    firmwareImageCorruption BOOLEAN OPTIONAL,
 *    commModuleInterfaceFailure BOOLEAN OPTIONAL,
 *    gnssReceiverFailure BOOLEAN OPTIONAL,
 *    raimProblem BOOLEAN OPTIONAL,
 *    gnssAntennaFailure BOOLEAN OPTIONAL,
 *    commModuleFailure BOOLEAN OPTIONAL,
 *    eventsMemoryOverflow BOOLEAN OPTIONAL,
 *    crashProfileMemoryOverflow BOOLEAN OPTIONAL,
 *    otherCriticalFailires BOOLEAN OPTIONAL,
 *    otherNotCriticalFailures BOOLEAN OPTIONAL
 * }
 *
 * CrashInfo ::= SEQUENCE {
 *    crashFront BOOLEAN OPTIONAL,
 *    crashLeft BOOLEAN OPTIONAL,
 *    crashRight BOOLEAN OPTIONAL,
 *    crashRear BOOLEAN OPTIONAL,
 *    crashRollover BOOLEAN OPTIONAL,
 *    crashSide BOOLEAN OPTIONAL,
 *    crashFrontOrSide BOOLEAN OPTIONAL,
 *    crashAnotherType BOOLEAN OPTIONAL
 * }
 *
 * CoordinateSystemType ::= ENUMERATED {
 *    wgs64 (1),
 *    ps90 (2)
 * }
 * END
 *
 * @endcode
 *
 * @section le_ecall_redial Redial management
 *
 * In the case of PAN-EUROPEAN, the redial can be performed as many times as desired but should be
 * performed within 2 minutes. (EN 16062:2014 -7.14.3).
 *
 * In the case of ERA-GLONASS, the redial can be performed ECALL_MANUAL_DIAL_ATTEMPTS times within
 * the maximal time limit of ECALL_DIAL_DURATION (GOST 54620 2013 – Appendix A).
 *
 * The \b LE_ECALL_STATE_END_OF_REDIAL_PERIOD state event notifies the User of the redial period
 * end.
 *
 * @section le_ecall_configuration eCall configuration
 *
 * Some parameters used by the eCall service can be configured through APIs. This includes the
 * number to dial, the deregistration time, the redial interval, and some MSD information.
 *
 * @subsection le_ecall_configuration_callNumber eCall number
 *
 * By default, the number to dial is read from the FDN/SDN (Fixed Dialling Numbers/Service
 * Dialling Numbers) of the U/SIM, depending upon the eCall operating mode.
 *
 * However, the PSAP phone number can be queried and set with:
 *  - le_ecall_GetPsapNumber()
 *  - le_ecall_SetPsapNumber()
 *
 * @snippet "apps/test/modemServices/ecall/ecallIntegrationTest/eCallTest/le_ecallTest.c" PsapNumber
 *
 * @note That PSAP number is not applied to a manual or an automatically initiated eCall. For these
 *       modes, an emergency call is launched.
 *
 * @warning These two functions don't modify or read the U/SIM content.
 *
 * When modem is in @ref LE_ECALL_FORCED_PERSISTENT_ONLY_MODE or @ref LE_ECALL_ONLY_MODE,
 * le_ecall_UseUSimNumbers() API can be called to request the modem to read the number to
 * dial from the FDN/SDN of the U/SIM.
 *
 * @note If FDN directory is updated with new dial numbers, be sure that the SIM card is refreshed.
 *
 * @subsection le_ecall_configuration_nad NAD deregistration time
 *
 * The NAD (Network Access Device, i.e. the Modem) deregistration time value can be set with the
 * le_ecall_SetNadDeregistrationTime() API and retrieved with the
 * le_ecall_GetNadDeregistrationTime() API.
 *
 * @snippet "apps/test/modemServices/ecall/ecallIntegrationTest/eCallTest/le_ecallTest.c" NadTime
 *
 * 1. PAN-EUROPEAN standard (<em>EN 16062</em>) defines the T9 and T10 timers. After an eCall
 *    clear-down, an <b>eCall only</b> IVS shall:
 *    - Remain registered on the network for at least the duration of T9, which is set to
 *      one hour by default.
 *    - Deregister from the network after the expiration of T10, which duration is set to
 *      twelve hours by default.
 * 2. ERA-GLONASS standard (<em>ENG_GOST_R_54620</em>) defines the NAD_DEREGISTRATION_TIME
 *    with a minimal value of two hours and a maximum value of twelve hours. After an eCall
 *    clear-down, the IVS shall remain registered on the network and deregister from the network
 *    after the expiration of the NAD_DEREGISTRATION_TIME.
 *
 * The le_ecall_SetNadDeregistrationTime() API is used to set the duration of a
 * <em>deregistration timer</em>: after an eCall clear-down, the IVS remains registered on the
 * network for the duration of this timer and then automatically deregisters from the network upon
 * its expiration.
 *
 * @subsubsection le_ecall_configuration_nad_panEU PAN-EUROPEAN standard
 * As the T9 timer duration is fixed to one hour, the notified events depend on the
 * <em>deregistration timer</em> duration:
 *  - If its duration is set to one hour, the @ref LE_ECALL_STATE_TIMEOUT_T9 event will be reported.
 *    In this case deregistration is automatically performed when the T9 timeout indication is
 *    received.
 *  - If its duration is set to more than one hour and less than twelve hours then the
 *    @ref LE_ECALL_STATE_TIMEOUT_T10 event will be reported when the <em>deregistration timer</em>
 *    expires. The T9 timeout (@ref LE_ECALL_STATE_TIMEOUT_T9 event) will not be notified.
 *    In this case deregistration is automatically performed when the <em>deregistration timer</em>
 *    expires, provided that the application didn't already deregister from the network before.
 *
 * @subsubsection le_ecall_configuration_nad_eraGlonass ERA-GLONASS standard
 *  - The @ref LE_ECALL_STATE_TIMEOUT_T9 event is not reported as it is not defined in the
 *    ERA-GLONASS standard.
 *  - The NAD_DEREGISTRATION_TIME default value is eight hours, as defined by
 *    <em>GOST_R 54620 Table A.1</em>. It can be changed with le_ecall_SetNadDeregistrationTime()
 *    and the value must be set between two and twelve hours.
 *  - The @ref LE_ECALL_STATE_TIMEOUT_T10 event will be reported when the NAD_DEREGISTRATION_TIME
 *    expires. In this case deregistration is automatically performed when the timer expires.
 *
 * @subsubsection le_ecall_configuration_nad_timerValue Timer value
 *
 * Although the time is set in minutes the value is taken in hours, for example:
 * - from 1 to 60 minutes -> 1 hour
 * - from 61 to 120 minutes -> 2 hours, etc.
 *
 * Example: if @c deregTime parameter unit is minutes, the effective time is:
 * @code ECallConfiguration.nad_deregistration_time = (deregTime+59)/60; @endcode
 *
 * After termination of an emergency call the in-vehicle system remains registered on the network
 * for the period of time, defined by the installation parameter NAD_DEREGISTRATION_TIME.
 *
 * @warning Be sure to check the possible values of NAD_DEREGISTRATION_TIME for your specific
 * platform.
 *
 * @subsection le_ecall_configuration_redialInterval Redial interval
 *
 * The minimum interval value between dial attempts can be set or get with:
 *  - le_ecall_SetIntervalBetweenDialAttempts()
 *  - le_ecall_GetIntervalBetweenDialAttempts()
 *
 * @snippet "apps/test/modemServices/ecall/ecallIntegrationTest/eCallTest/le_ecallTest.c" DialTimer
 *
 * The default value is set to 30 seconds.
 *
 * The time is counted from the start of the first dial attempt.
 *
 * If more time has expired during the dial attempt, it will wait for 1 second to allow hangup
 * before redialing.
 *
 * If less time has expired during the dial attempt, it will wait for
 * (interval - 'dial attempt duration') seconds to allow hangup before redialing.
 *
 * In the case the call was connected, the redial will be immediate.
 *
 * le_ecall_SetIntervalBetweenDialAttempts() is available for both manual and test modes.
 *
 * @subsection le_ecall_configuration_misc Miscellaneous
 *
 * The preferred system standard defaults to PAN-EUROPEAN
 * It can be set an gotten with the following functions:
 * - le_ecall_SetSystemStandard()
 * - le_ecall_GetSystemStandard()
 *
 * @snippet "apps/test/modemServices/ecall/ecallIntegrationTest/eCallTest/le_ecallTest.c" Standard
 *
 * The MSD version can be set and and gotten with the following functions:
 * - le_ecall_SetMsdVersion()
 * - le_ecall_GetMsdVersion()
 *
 * @snippet "apps/test/modemServices/ecall/ecallIntegrationTest/eCallTest/le_ecallTest.c" MsdVersion
 *
 * The vehicle type can be set and gotten with the following functions:
 * - le_ecall_SetVehicleType()
 * - le_ecall_GetVehicleType()
 *
 * @snippet "apps/test/modemServices/ecall/ecallIntegrationTest/eCallTest/le_ecallTest.c" Vehicle
 *
 * The vehicle identifier can be set and gotten with the following functions:
 * - le_ecall_SetVIN()
 * - le_ecall_GetVIN()
 *
 * @snippet "apps/test/modemServices/ecall/ecallIntegrationTest/eCallTest/le_ecallTest.c" VIN
 *
 * The propulsion type can be set and gotten with the following functions:
 * - le_ecall_SetPropulsionType()
 * - le_ecall_GetPropulsionType()
 *
 * @snippet "apps/test/modemServices/ecall/ecallIntegrationTest/eCallTest/le_ecallTest.c" Propulsion
 *
 * @section le_ecall_samples Code samples
 *
 * A sample code that implements an eCall test session with a local voice prompt can be found in
 * \b eCallWPrompt.c file (please refer to @ref c_ecallWPromptSample page).
 *
 * A sample code that implements an eCall test session with a voice call connection can be found in
 * \b eCallWVoice.c file (please refer to @ref c_ecallWVoiceSample page).
 *
 * If you want to have a look at a more in-depth usage of these APIs, please refer to the
 * @ref le_ecall_interface.h.
 *
 * <HR>
 *
 * Copyright (C) Sierra Wireless Inc.
 */
/**
 * @file le_ecall_interface.h
 *
 * Legato @ref c_ecall include file.
 *
 * Copyright (C) Sierra Wireless Inc.
 */
/**
 * @page c_ecallWPromptSample Sample code of an eCall test session with a local voice prompt
 *
 * @include "apps/test/modemServices/ecall/ecallWPrompt/eCallWPrompt/eCallWPrompt.c"
 */
/**
 * @page c_ecallWVoiceSample Sample code an eCall test session with a voice call connection
 *
 * @include "apps/test/modemServices/ecall/ecallWVoice/eCallWVoice/eCallWVoice.c"
 */

#ifndef LE_ECALL_INTERFACE_H_INCLUDE_GUARD
#define LE_ECALL_INTERFACE_H_INCLUDE_GUARD


#include "legato.h"

// Interface specific includes
#include "le_mdmDefs_interface.h"
#include "le_mcc_interface.h"


//--------------------------------------------------------------------------------------------------
/**
 * Type for handler called when a server disconnects.
 */
//--------------------------------------------------------------------------------------------------
typedef void (*le_ecall_DisconnectHandler_t)(void *);

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
void le_ecall_ConnectService
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
le_result_t le_ecall_TryConnectService
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
void le_ecall_SetServerDisconnectHandler
(
    le_ecall_DisconnectHandler_t disconnectHandler,
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
void le_ecall_DisconnectService
(
    void
);


//--------------------------------------------------------------------------------------------------
/**
 * MSD message length in bytes.
 */
//--------------------------------------------------------------------------------------------------
#define LE_ECALL_MSD_MAX_LEN 140

//--------------------------------------------------------------------------------------------------
/**
 * Vehicle Identification Number (VIN) string LENGTH. (without nulltermination)
 */
//--------------------------------------------------------------------------------------------------
#define LE_ECALL_VIN_MAX_LEN 17

//--------------------------------------------------------------------------------------------------
/**
 * Vehicle Identification Number (VIN) string BYTES. One extra byte is added for the null
 * character.
 */
//--------------------------------------------------------------------------------------------------
#define LE_ECALL_VIN_MAX_BYTES 18

//--------------------------------------------------------------------------------------------------
/**
 * Reference returned by Create function and used by other functions
 */
//--------------------------------------------------------------------------------------------------
typedef struct le_ecall_Call* le_ecall_CallRef_t;


//--------------------------------------------------------------------------------------------------
/**
 *  Configured operation modes.
 */
//--------------------------------------------------------------------------------------------------
typedef enum
{
    LE_ECALL_NORMAL_MODE = 0,
        ///< Normal mode.
    LE_ECALL_ONLY_MODE = 1,
        ///< eCall only mode according to U/SIM operation mode or forced
        ///< by application through the le_ecall_ForceOnlyMode() function.
    LE_ECALL_FORCED_PERSISTENT_ONLY_MODE = 2
        ///< Persistent eCall only mode.
}
le_ecall_OpMode_t;


//--------------------------------------------------------------------------------------------------
/**
 *  Configure which standard to follow for the eCall, either PAN European eCall or ERA GLONASS.
 */
//--------------------------------------------------------------------------------------------------
typedef enum
{
    LE_ECALL_PAN_EUROPEAN = 0,
        ///< PAN-EUROPEAN standard
    LE_ECALL_ERA_GLONASS = 1
        ///< ERA GLONASS standard
}
le_ecall_SystemStandard_t;


//--------------------------------------------------------------------------------------------------
/**
 *  eCall session states.
 */
//--------------------------------------------------------------------------------------------------
typedef enum
{
    LE_ECALL_STATE_UNKNOWN = 0,
        ///< Unknown state.
    LE_ECALL_STATE_STARTED = 1,
        ///< eCall session started.
    LE_ECALL_STATE_CONNECTED = 2,
        ///< Emergency call is established.
    LE_ECALL_STATE_DISCONNECTED = 3,
        ///< Emergency call is disconnected.
    LE_ECALL_STATE_WAITING_PSAP_START_IND = 4,
        ///< Waiting for PSAP start indication.
    LE_ECALL_STATE_PSAP_START_IND_RECEIVED = 5,
        ///< PSAP start indication received.
    LE_ECALL_STATE_MSD_TX_STARTED = 6,
        ///< MSD transmission is started.
    LE_ECALL_STATE_LLNACK_RECEIVED = 7,
        ///< LL-NACK received.
    LE_ECALL_STATE_LLACK_RECEIVED = 8,
        ///< LL-ACK received.
    LE_ECALL_STATE_MSD_TX_COMPLETED = 9,
        ///< MSD transmission is complete.
    LE_ECALL_STATE_MSD_TX_FAILED = 10,
        ///< MSD transmission has failed.
    LE_ECALL_STATE_ALACK_RECEIVED_POSITIVE = 11,
        ///< AL-ACK received.
    LE_ECALL_STATE_ALACK_RECEIVED_CLEAR_DOWN = 12,
        ///< AL-ACK clear-down received.
    LE_ECALL_STATE_STOPPED = 13,
        ///< eCall session has been stopped by PSAP
        ///< or IVS le_ecall_End().
    LE_ECALL_STATE_RESET = 14,
        ///< eCall session has lost synchronization and starts over.
    LE_ECALL_STATE_COMPLETED = 15,
        ///< eCall session completed. The modem successfully completed
        ///< the MSD transmission and received two AL-ACKs (positive).
        ///< Can now switch to voice path.
    LE_ECALL_STATE_FAILED = 16,
        ///< Unsuccessful eCall session.
    LE_ECALL_STATE_END_OF_REDIAL_PERIOD = 17,
        ///< End of the redial period.
    LE_ECALL_STATE_TIMEOUT_T2 = 18,
        ///< Timeout for Timer T2 (see CEN-EN 16062, annex A)
    LE_ECALL_STATE_TIMEOUT_T3 = 19,
        ///< Timeout for Timer T3 (see CEN-EN 16062, annex A)
    LE_ECALL_STATE_TIMEOUT_T5 = 20,
        ///< Timeout for Timer T5 (see CEN-EN 16062, annex A)
    LE_ECALL_STATE_TIMEOUT_T6 = 21,
        ///< Timeout for Timer T6 (see CEN-EN 16062, annex A)
    LE_ECALL_STATE_TIMEOUT_T7 = 22,
        ///< Timeout for Timer T7 (see CEN-EN 16062, annex A)
    LE_ECALL_STATE_TIMEOUT_T9 = 23,
        ///< Timeout for Timer T9 (see CEN-EN 16062, annex A)
    LE_ECALL_STATE_TIMEOUT_T10 = 24
        ///< Timeout for Timer T10 (see CEN-EN 16062, annex A)
}
le_ecall_State_t;


//--------------------------------------------------------------------------------------------------
/**
 *  eCall MSD transmission mode.
 */
//--------------------------------------------------------------------------------------------------
typedef enum
{
    LE_ECALL_TX_MODE_PULL = 0,
        ///< Pull mode (modem/host waits for MSD request from PSAP to send MSD).
    LE_ECALL_TX_MODE_PUSH = 1
        ///< Push mode (modem/host sends MSD to PSAP right after eCall is connected).
}
le_ecall_MsdTxMode_t;


//--------------------------------------------------------------------------------------------------
/**
 *  eCall MSD vehicle type
 */
//--------------------------------------------------------------------------------------------------
typedef enum
{
    LE_ECALL_MSD_VEHICLE_PASSENGER_M1 = 0,
        ///< Passenger vehicle (Class M1)
    LE_ECALL_MSD_VEHICLE_BUS_M2 = 1,
        ///< Buses and coaches (Class M2)
    LE_ECALL_MSD_VEHICLE_BUS_M3 = 2,
        ///< Buses and coaches (Class M3)
    LE_ECALL_MSD_VEHICLE_COMMERCIAL_N1 = 3,
        ///< Light commercial vehicles (Class N1)
    LE_ECALL_MSD_VEHICLE_HEAVY_N2 = 4,
        ///< Heavy duty vehicles (Class N2)
    LE_ECALL_MSD_VEHICLE_HEAVY_N3 = 5,
        ///< Heavy duty vehicles (Class N3)
    LE_ECALL_MSD_VEHICLE_MOTORCYCLE_L1E = 6,
        ///< Motorcycles (Class L1e)
    LE_ECALL_MSD_VEHICLE_MOTORCYCLE_L2E = 7,
        ///< Motorcycles (Class L2e)
    LE_ECALL_MSD_VEHICLE_MOTORCYCLE_L3E = 8,
        ///< Motorcycles (Class L3e)
    LE_ECALL_MSD_VEHICLE_MOTORCYCLE_L4E = 9,
        ///< Motorcycles (Class L4e)
    LE_ECALL_MSD_VEHICLE_MOTORCYCLE_L5E = 10,
        ///< Motorcycles (Class L5e)
    LE_ECALL_MSD_VEHICLE_MOTORCYCLE_L6E = 11,
        ///< Motorcycles (Class L6e)
    LE_ECALL_MSD_VEHICLE_MOTORCYCLE_L7E = 12
        ///< Motorcycles (Class L7e)
}
le_ecall_MsdVehicleType_t;


//--------------------------------------------------------------------------------------------------
/**
 * Propulsion type Bit Mask
 */
//--------------------------------------------------------------------------------------------------
typedef enum
{
    LE_ECALL_PROPULSION_TYPE_GASOLINE = 0x1,        ///< Gasoline propulsion)
    LE_ECALL_PROPULSION_TYPE_DIESEL = 0x2,        ///< Diesel propulsion
    LE_ECALL_PROPULSION_TYPE_NATURALGAS = 0x4,        ///< Compressed natural gas propulsion
    LE_ECALL_PROPULSION_TYPE_PROPANE = 0x8,        ///< Liquid propane gas propulsion
    LE_ECALL_PROPULSION_TYPE_ELECTRIC = 0x10,        ///< Electric propulsion
    LE_ECALL_PROPULSION_TYPE_HYDROGEN = 0x20,        ///< Hydrogen propulsion
    LE_ECALL_PROPULSION_TYPE_OTHER = 0x40        ///< Other type of propulsions
}
le_ecall_PropulsionTypeBitMask_t;


//--------------------------------------------------------------------------------------------------
/**
 * Diagnostic result Bit Mask (64 bits)
 *
 */
//--------------------------------------------------------------------------------------------------
typedef enum
{
    LE_ECALL_DIAG_RESULT_PRESENT_MIC_CONNECTION_FAILURE = 0x1,        ///< Present Mic connection failure.
    LE_ECALL_DIAG_RESULT_MIC_CONNECTION_FAILURE = 0x2,        ///< Mic connection failure status.
    LE_ECALL_DIAG_RESULT_PRESENT_MIC_FAILURE = 0x4,        ///< Present Mic  failure.
    LE_ECALL_DIAG_RESULT_MIC_FAILURE = 0x8,        ///< Mic failure status.
    LE_ECALL_DIAG_RESULT_PRESENT_RIGHT_SPEAKER_FAILURE = 0x10,        ///< Present Right speaker failure.
    LE_ECALL_DIAG_RESULT_RIGHT_SPEAKER_FAILURE = 0x20,        ///< Right speaker failure status.
    LE_ECALL_DIAG_RESULT_PRESENT_LEFT_SPEAKER_FAILURE = 0x40,        ///< Present Left speaker failure.
    LE_ECALL_DIAG_RESULT_LEFT_SPEAKER_FAILURE = 0x80,        ///< Left speaker failure status.
    LE_ECALL_DIAG_RESULT_PRESENT_SPEAKERS_FAILURE = 0x100,        ///< Present Speaker failure.
    LE_ECALL_DIAG_RESULT_SPEAKERS_FAILURE = 0x200,        ///< Speaker failure status.
    LE_ECALL_DIAG_RESULT_PRESENT_IGNITION_LINE_FAILURE = 0x400,        ///< Present Ignition line failure.
    LE_ECALL_DIAG_RESULT_IGNITION_LINE_FAILURE = 0x800,        ///< Ignition line failure status.
    LE_ECALL_DIAG_RESULT_PRESENT_UIM_FAILURE = 0x1000,        ///< Present UIM failure.
    LE_ECALL_DIAG_RESULT_UIM_FAILURE = 0x2000,        ///< UIM failure status.
    LE_ECALL_DIAG_RESULT_PRESENT_STATUS_INDICATOR_FAILURE = 0x4000,        ///< Present Status indicator failure.
    LE_ECALL_DIAG_RESULT_STATUS_INDICATOR_FAILURE = 0x8000,        ///< Status indicator failure status.
    LE_ECALL_DIAG_RESULT_PRESENT_BATTERY_FAILURE = 0x10000,        ///< Present Battery failure.
    LE_ECALL_DIAG_RESULT_BATTERY_FAILURE = 0x20000,        ///< Battery failure status.
    LE_ECALL_DIAG_RESULT_PRESENT_BATTERY_VOLTAGE_LOW = 0x40000,        ///< Present Battery voltage low failure.
    LE_ECALL_DIAG_RESULT_BATTERY_VOLTAGE_LOW = 0x80000,        ///< Battery voltage low failure status.
    LE_ECALL_DIAG_RESULT_PRESENT_CRASH_SENSOR_FAILURE = 0x100000,        ///< Present Crash sensor failure.
    LE_ECALL_DIAG_RESULT_CRASH_SENSOR_FAILURE = 0x200000,        ///< Crash sensor failure status.
    LE_ECALL_DIAG_RESULT_PRESENT_FIRMWARE_IMAGE_CORRUPTION = 0x400000,        ///< Present Firmware image corruption failure.
    LE_ECALL_DIAG_RESULT_FIRMWARE_IMAGE_CORRUPTION = 0x800000,        ///< Firmware image corruption failure status.
    LE_ECALL_DIAG_RESULT_PRESENT_COMM_MODULE_INTERFACE_FAILURE = 0x1000000,        ///< Present Comm module interface failure.
    LE_ECALL_DIAG_RESULT_COMM_MODULE_INTERFACE_FAILURE = 0x2000000,        ///< Comm module interface failure status.
    LE_ECALL_DIAG_RESULT_PRESENT_GNSS_RECEIVER_FAILURE = 0x4000000,        ///< Present GNSS receiver failure.
    LE_ECALL_DIAG_RESULT_GNSS_RECEIVER_FAILURE = 0x8000000,        ///< GNSS receiver failure status.
    LE_ECALL_DIAG_RESULT_PRESENT_RAIM_PROBLEM = 0x10000000,        ///< Present RAIM problem.
    LE_ECALL_DIAG_RESULT_RAIM_PROBLEM = 0x20000000,        ///< RAIM problem status.
    LE_ECALL_DIAG_RESULT_PRESENT_GNSS_ANTENNA_FAILURE = 0x40000000,        ///< Present GNSS antenna failure.
    LE_ECALL_DIAG_RESULT_GNSS_ANTENNA_FAILURE = 0x80000000,        ///< GNSS antenna failure status.
    LE_ECALL_DIAG_RESULT_PRESENT_COMM_MODULE_FAILURE = 0x100000000,        ///< Present Comm module failure.
    LE_ECALL_DIAG_RESULT_COMM_MODULE_FAILURE = 0x200000000,        ///< Comm module failure status.
    LE_ECALL_DIAG_RESULT_PRESENT_EVENTS_MEMORY_OVERFLOW = 0x400000000,        ///< Present Events memory overflow.
    LE_ECALL_DIAG_RESULT_EVENTS_MEMORY_OVERFLOW = 0x800000000,        ///< Events memory overflow status.
    LE_ECALL_DIAG_RESULT_PRESENT_CRASH_PROFILE_MEMORY_OVERFLOW = 0x1000000000,        ///< Present Crash profile memory overflow.
    LE_ECALL_DIAG_RESULT_CRASH_PROFILE_MEMORY_OVERFLOW = 0x2000000000,        ///< Crash profile memory overflow status.
    LE_ECALL_DIAG_RESULT_PRESENT_OTHER_CRITICAL_FAILURES = 0x4000000000,        ///< Present Other critical failures.
    LE_ECALL_DIAG_RESULT_OTHER_CRITICAL_FAILURES = 0x8000000000,        ///< Other critical failures status.
    LE_ECALL_DIAG_RESULT_PRESENT_OTHER_NOT_CRITICAL_FAILURES = 0x10000000000,        ///< Present Other not critical failures.
    LE_ECALL_DIAG_RESULT_OTHER_NOT_CRITICAL_FAILURES = 0x20000000000        ///< Other not critical failures status.
}
le_ecall_DiagnosticResultBitMask_t;


//--------------------------------------------------------------------------------------------------
/**
 * Crash information Bit Mask (16 bits) indicating the type of road accident.
 *
 */
//--------------------------------------------------------------------------------------------------
typedef enum
{
    LE_ECALL_CRASH_INFO_PRESENT_CRASH_FRONT = 0x1,        ///< Present Front collision indicator.
    LE_ECALL_CRASH_INFO_CRASH_FRONT = 0x2,        ///< Front collision indicator.
    LE_ECALL_CRASH_INFO_PRESENT_CRASH_LEFT = 0x4,        ///< Present Left collision indicator.
    LE_ECALL_CRASH_INFO_CRASH_LEFT = 0x8,        ///< Left collision indicator.
    LE_ECALL_CRASH_INFO_PRESENT_CRASH_RIGHT = 0x10,        ///< Present Right collision indicator
    LE_ECALL_CRASH_INFO_CRASH_RIGHT = 0x20,        ///< Right collision indicator
    LE_ECALL_CRASH_INFO_PRESENT_CRASH_REAR = 0x40,        ///< Present indicator
    LE_ECALL_CRASH_INFO_CRASH_REAR = 0x80,        ///< Rear impact indicator.
    LE_ECALL_CRASH_INFO_PRESENT_CRASH_ROLLOVER = 0x100,        ///< Present Rollover indicator.
    LE_ECALL_CRASH_INFO_CRASH_ROLLOVER = 0x200,        ///< Rollover indicator.
    LE_ECALL_CRASH_INFO_PRESENT_CRASH_SIDE = 0x400,        ///< Present Side collision indicator.
    LE_ECALL_CRASH_INFO_CRASH_SIDE = 0x800,        ///< Side collision indicator.
    LE_ECALL_CRASH_INFO_PRESENT_CRASH_FRONT_OR_SIDE = 0x1000,        ///< Present Front or side collision indicator.
    LE_ECALL_CRASH_INFO_CRASH_FRONT_OR_SIDE = 0x2000,        ///< Front or side collision indicator.
    LE_ECALL_CRASH_INFO_PRESENT_CRASH_ANOTHER_TYPE = 0x4000,        ///< Present Other types of damage indicator.
    LE_ECALL_CRASH_INFO_CRASH_ANOTHER_TYPE = 0x8000        ///< Other types of damage indicator.
}
le_ecall_CrashInfoBitMask_t;


//--------------------------------------------------------------------------------------------------
/**
 *  eCall MSD coordinate system type
 */
//--------------------------------------------------------------------------------------------------
typedef enum
{
    LE_ECALL_MSD_COORDINATE_SYSTEM_TYPE_ABSENT = 0,
        ///< Coordinate system type is absent
    LE_ECALL_MSD_COORDINATE_SYSTEM_TYPE_WGS84 = 1,
        ///< Coordinate system type WGS-84
    LE_ECALL_MSD_COORDINATE_SYSTEM_TYPE_PZ90 = 2
        ///< Coordinate system type PZ-90
}
le_ecall_MsdCoordinateType_t;


//--------------------------------------------------------------------------------------------------
/**
 * Reference type used by Add/Remove functions for EVENT 'le_ecall_StateChange'
 */
//--------------------------------------------------------------------------------------------------
typedef struct le_ecall_StateChangeHandler* le_ecall_StateChangeHandlerRef_t;


//--------------------------------------------------------------------------------------------------
/**
 * Handler for eCall state changes.
 *
 */
//--------------------------------------------------------------------------------------------------
typedef void (*le_ecall_StateChangeHandlerFunc_t)
(
    le_ecall_CallRef_t ecallRef,
        ///< eCall reference
    le_ecall_State_t state,
        ///< eCall state
    void* contextPtr
        ///<
);

//--------------------------------------------------------------------------------------------------
/**
 * This function configures the eCall operation mode to eCall only, only emergency number can be
 * used to start an eCall session. The modem doesn't try to register on the Cellular network.
 * This function forces the modem to behave as eCall only mode whatever U/SIM operation mode.
 * The change doesn't persist over power cycles.
 * This function can be called before making an eCall.
 *
 * @return
 *      - LE_OK on success
 *      - LE_FAULT for other failures
 *      - LE_UNSUPPORTED Not supported on this platform
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_ecall_ForceOnlyMode
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * Same as le_ecall_ForceOnlyMode(), but the change persists over power cycles.
 *
 * @return
 *      - LE_OK on success
 *      - LE_FAULT for other failures
 *      - LE_UNSUPPORTED Not supported on this platform
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_ecall_ForcePersistentOnlyMode
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * This function exits from eCall Only mode. It configures the eCall operation mode to Normal mode,
 * the modem uses the default operation mode at power up (or after U/SIM hotswap). The modem behaves
 * following the U/SIM eCall operation mode; for example the U/SIM can be configured only for eCall,
 * or a combination of eCall and commercial service provision.
 *
 * @return
 *      - LE_OK on success
 *      - LE_FAULT for other failures
 *      - LE_UNSUPPORTED Not supported on this platform
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_ecall_ExitOnlyMode
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * Get the configured Operation mode.
 *
 * @return
 *      - LE_OK     on success
 *      - LE_FAULT  for other failures
 *      - LE_UNSUPPORTED Not supported on this platform
 *
 * @note If the caller is passing a bad pointer into this function, it is a fatal error, the
 *       function will not return.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_ecall_GetConfiguredOperationMode
(
    le_ecall_OpMode_t* opModePtr
        ///< [OUT] Operation mode
);

//--------------------------------------------------------------------------------------------------
/**
 * Add handler function for EVENT 'le_ecall_StateChange'
 *
 * This event provides information on eCall state changes.
 *
 */
//--------------------------------------------------------------------------------------------------
le_ecall_StateChangeHandlerRef_t le_ecall_AddStateChangeHandler
(
    le_ecall_StateChangeHandlerFunc_t handlerPtr,
        ///< [IN]
    void* contextPtr
        ///< [IN]
);

//--------------------------------------------------------------------------------------------------
/**
 * Remove handler function for EVENT 'le_ecall_StateChange'
 */
//--------------------------------------------------------------------------------------------------
void le_ecall_RemoveStateChangeHandler
(
    le_ecall_StateChangeHandlerRef_t handlerRef
        ///< [IN]
);

//--------------------------------------------------------------------------------------------------
/**
 * Create a new eCall object
 *
 * The eCall is not actually established at this point. It's still up to the caller to call
 * le_ecall_Start() when ready.
 *
 * @return
 *      - A reference to the new Call object.
 *
 * @note On failure, the process exits; you don't have to worry about checking the returned
 *       reference for validity.
 */
//--------------------------------------------------------------------------------------------------
le_ecall_CallRef_t le_ecall_Create
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * Call to free up a call reference.
 *
 * @note This will free the reference, but not necessarily stop an active eCall. If there are
 *       other holders of this reference the eCall will remain active.
 */
//--------------------------------------------------------------------------------------------------
void le_ecall_Delete
(
    le_ecall_CallRef_t ecallRef
        ///< [IN] eCall reference
);

//--------------------------------------------------------------------------------------------------
/**
 * Set the system standard.
 * Default is PAN EUROPEAN
 *
 * @return
 *  - LE_OK on success
 *  - LE_FAULT for other failures
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_ecall_SetSystemStandard
(
    le_ecall_SystemStandard_t systemStandard
        ///< [IN] System standard
);

//--------------------------------------------------------------------------------------------------
/**
 * Get the system standard.
 *
 * @return
 *  - LE_OK on success
 *  - LE_FAULT for other failures
 *  - LE_BAD_PARAMETER parameter is NULL
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_ecall_GetSystemStandard
(
    le_ecall_SystemStandard_t* systemStandardPtr
        ///< [OUT] System Standard
);

//--------------------------------------------------------------------------------------------------
/**
 * Set the MSDs version.
 * Default value is 1
 *
 * @return
 *  - LE_OK on success
 *  - LE_FAULT for other failures
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_ecall_SetMsdVersion
(
    uint32_t msdVersion
        ///< [IN] Msd version
);

//--------------------------------------------------------------------------------------------------
/**
 * Get the MSD version.
 *
 * @return
 *  - LE_OK on success
 *  - LE_FAULT for other failures
 *  - LE_BAD_PARAMETER parameter is NULL
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_ecall_GetMsdVersion
(
    uint32_t* msdVersionPtr
        ///< [OUT] Msd version
);

//--------------------------------------------------------------------------------------------------
/**
 * Set the Vehicled Type
 * Default value is 0
 *
 * @return
 *  - LE_OK on success
 *  - LE_FAULT for other failures
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_ecall_SetVehicleType
(
    le_ecall_MsdVehicleType_t vehicleType
        ///< [IN] Vehicle type
);

//--------------------------------------------------------------------------------------------------
/**
 * Get the Vehicled Type.
 *
 * @return
 *  - LE_OK on success
 *  - LE_BAD_PARAMETER parameter is NULL
 *  - LE_FAULT for other failures
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_ecall_GetVehicleType
(
    le_ecall_MsdVehicleType_t* vehicleTypePtr
        ///< [OUT] Vehicle type
);

//--------------------------------------------------------------------------------------------------
/**
 * Set the VIN (Vehicle Identification Number).
 *
 * @return
 *  - LE_OK on success
 *  - LE_BAD_PARAMETER parameter is NULL.
 *  - LE_FAULT for other failures
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_ecall_SetVIN
(
    const char* LE_NONNULL vin
        ///< [IN] VIN (Vehicle Identification Number)
);

//--------------------------------------------------------------------------------------------------
/**
 * Get the VIN (Vehicle Identification Number).
 *
 * @return
 *  - LE_OK on success
 *  - LE_NOT_FOUND if the value is not set.
 *  - LE_BAD_PARAMETER parameter is NULL or to small
 *  - LE_FAULT for other failures
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_ecall_GetVIN
(
    char* vin,
        ///< [OUT] VIN is gotten with a null termination.
    size_t vinSize
        ///< [IN]
);

//--------------------------------------------------------------------------------------------------
/**
 * Set the propulsion type.
 * Note that a vehicle may have more than one propulsion type.
 *
 * @return
 *  - LE_OK on success
 *  - LE_FAULT for other failures
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_ecall_SetPropulsionType
(
    le_ecall_PropulsionTypeBitMask_t propulsionType
        ///< [IN] bitmask
);

//--------------------------------------------------------------------------------------------------
/**
 * Get the propulsion stored.
 * Note that a vehicle may have more than one propulsion type.
 *
 * @return
 *  - LE_OK on success
 *  - LE_NOT_FOUND if the value is not set.
 *  - LE_FAULT for other failures
 *  - LE_BAD_PARAMETER parameter is NULL
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_ecall_GetPropulsionType
(
    le_ecall_PropulsionTypeBitMask_t* propulsionTypePtr
        ///< [OUT] bitmask
);

//--------------------------------------------------------------------------------------------------
/**
 * Set the push/pull transmission mode.
 *
 * @return
 *  - LE_OK on success
 *  - LE_FAULT for other failures
 *  - LE_UNSUPPORTED Not supported on this platform
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_ecall_SetMsdTxMode
(
    le_ecall_MsdTxMode_t mode
        ///< [IN] Transmission mode
);

//--------------------------------------------------------------------------------------------------
/**
 * Get the push/pull transmission mode.
 *
 * @return
 *      - LE_OK     on success
 *      - LE_FAULT  for other failures
 *      - LE_UNSUPPORTED Not supported on this platform
 *
 * @note If the caller is passing a bad pointer into this function, it is a fatal error, the
 *       function will not return.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_ecall_GetMsdTxMode
(
    le_ecall_MsdTxMode_t* modePtr
        ///< [OUT] Transmission mode
);

//--------------------------------------------------------------------------------------------------
/**
 * Set the position transmitted by the MSD.
 *
 * @return
 *      - LE_OK on success
 *      - LE_DUPLICATE an MSD has been already imported
 *      - LE_BAD_PARAMETER bad input parameter
 *      - LE_FAULT on other failures
 *
 * @note The process exits, if an invalid eCall reference is given
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_ecall_SetMsdPosition
(
    le_ecall_CallRef_t ecallRef,
        ///< [IN] eCall reference
    bool isTrusted,
        ///< [IN] true if the position is accurate, false otherwise
    int32_t latitude,
        ///< [IN] latitude in degrees with 6 decimal places, positive North.
        ///< Maximum value is +90 degrees (+90000000), minimum value is -90
        ///< degrees (-90000000).
    int32_t longitude,
        ///< [IN] longitude in degrees with 6 decimal places, positive East.
        ///< Maximum value is +180 degrees (+180000000), minimum value is
        ///< -180 degrees (-180000000).
    int32_t direction
        ///< [IN] direction of the vehicle from magnetic north (0 to 358, clockwise)
        ///< in 2-degrees unit. Valid range is 0 to 179. If direction of
        ///< travel is invalid or unknown, the value 0xFF shall be used.
);

//--------------------------------------------------------------------------------------------------
/**
 * Set the position Delta N-1 from position set in le_ecall_SetMsdPosition() transmitted by the MSD.
 *
 * @return
 *      - LE_OK on success
 *      - LE_DUPLICATE an MSD has been already imported
 *      - LE_BAD_PARAMETER bad input parameter
 *      - LE_FAULT on other failures
 *
 * @note The process exits, if an invalid eCall reference is given
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_ecall_SetMsdPositionN1
(
    le_ecall_CallRef_t ecallRef,
        ///< [IN] eCall reference
    int32_t latitudeDeltaN1,
        ///< [IN] longitude delta from position set in SetMsdPosition
        ///< 1 Unit = 100 miliarcseconds, which is approximately 3m
        ///< maximum value: 511 = 0 0'51.100'' (+- 1580m)
        ///< minimum value: -512 = -0 0'51.200'' (+- -1583m)
    int32_t longitudeDeltaN1
        ///< [IN] longitude delta from position set in SetMsdPosition
        ///< 1 Unit = 100 miliarcseconds, which is approximately 3m
        ///< maximum value: 511 = 0 0'51.100'' (+-1580m)
        ///< minimum value: -512 = -0 0'51.200'' (+- -1583m) )
);

//--------------------------------------------------------------------------------------------------
/**
 * Set the position Delta N-2 from position set in le_ecall_SetMsdPositionN1() transmitted by the MSD.
 *
 * @return
 *      - LE_OK on success
 *      - LE_DUPLICATE an MSD has been already imported
 *      - LE_BAD_PARAMETER bad input parameter
 *      - LE_FAULT on other failures
 *
 * @note The process exits, if an invalid eCall reference is given
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_ecall_SetMsdPositionN2
(
    le_ecall_CallRef_t ecallRef,
        ///< [IN] eCall reference
    int32_t latitudeDeltaN2,
        ///< [IN] longitude delta from position set in SetMsdPositionN1
        ///< 1 Unit = 100 miliarcseconds, which is approximately 3m
        ///< maximum value: 511 = 0 0'51.100'' (+-1580m)
        ///< minimum value: -512 = -0 0'51.200'' (+- -1583m)
    int32_t longitudeDeltaN2
        ///< [IN] longitude delta from position set in SetMsdPositionN1
        ///< 1 Unit = 100 miliarcseconds, which is approximately 3m
        ///< maximum value: 511 = 0 0'51.100'' (+-1580m)
        ///< minimum value: -512 = -0 0'51.200'' (+- -1583m) )
);

//--------------------------------------------------------------------------------------------------
/**
 * Set the number of passengers transmitted by the MSD.
 *
 * @return
 *      - LE_OK on success
 *      - LE_DUPLICATE an MSD has been already imported
 *      - LE_BAD_PARAMETER bad eCall reference
 *
 * @note The process exits, if an invalid eCall reference is given
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_ecall_SetMsdPassengersCount
(
    le_ecall_CallRef_t ecallRef,
        ///< [IN] eCall reference
    uint32_t paxCount
        ///< [IN] number of passengers
);

//--------------------------------------------------------------------------------------------------
/**
 * Import an already prepared MSD.
 *
 * MSD is transmitted only after an emergency call has been established.
 *
 * @return
 *      - LE_OK on success
 *      - LE_OVERFLOW The imported MSD length exceeds the MSD_MAX_LEN maximum length.
 *      - LE_BAD_PARAMETER bad eCall reference
 *      - LE_FAULT for other failures
 *
 * @note On failure, the process exits; you don't have to worry about checking the returned
 *       reference for validity.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_ecall_ImportMsd
(
    le_ecall_CallRef_t ecallRef,
        ///< [IN] eCall reference
    const uint8_t* msdPtr,
        ///< [IN] the prepared MSD
    size_t msdSize
        ///< [IN]
);

//--------------------------------------------------------------------------------------------------
/**
 * Export the encoded MSD.
 *
 * @return
 *  - LE_OK on success
 *  - LE_OVERFLOW  The encoded MSD length exceeds the user's buffer length.
 *  - LE_NOT_FOUND  No encoded MSD is available.
 *  - LE_BAD_PARAMETER bad eCall reference.
 *  - LE_FAULT for other failures.
 *
 * @note If the caller is passing a bad pointer into this function, it is a fatal error, the
 *       function will not return.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_ecall_ExportMsd
(
    le_ecall_CallRef_t ecallRef,
        ///< [IN] eCall reference
    uint8_t* msdPtr,
        ///< [OUT] the encoded MSD
    size_t* msdSizePtr
        ///< [INOUT]
);

//--------------------------------------------------------------------------------------------------
/**
 * Send the MSD.
 *
 * @return
 *      - LE_OK on success
 *      - LE_BAD_PARAMETER bad eCall reference
 *      - LE_FAULT for other failures
 *      - LE_UNSUPPORTED Not supported on this platform
 *
 * @note On failure, the process exits, so you don't have to worry about checking the returned
 *       reference for validity.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_ecall_SendMsd
(
    le_ecall_CallRef_t ecallRef
        ///< [IN] eCall reference
);

//--------------------------------------------------------------------------------------------------
/**
 * Start an automatic eCall session
 *
 * @return
 *      - LE_OK on success
 *      - LE_BUSY an eCall session is already in progress
 *      - LE_BAD_PARAMETER bad eCall reference
 *      - LE_FAULT for other failures
 *      - LE_UNSUPPORTED Not supported on this platform
 *
 * @note The process exits, if an invalid eCall reference is given
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_ecall_StartAutomatic
(
    le_ecall_CallRef_t ecallRef
        ///< [IN] eCall reference
);

//--------------------------------------------------------------------------------------------------
/**
 * Start a manual eCall session
 *
 * @return
 *      - LE_OK on success
 *      - LE_BUSY an eCall session is already in progress
 *      - LE_BAD_PARAMETER bad eCall reference
 *      - LE_FAULT for other failures
 *      - LE_UNSUPPORTED Not supported on this platform
 *
 * @note The process exits, if an invalid eCall reference is given
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_ecall_StartManual
(
    le_ecall_CallRef_t ecallRef
        ///< [IN] eCall reference
);

//--------------------------------------------------------------------------------------------------
/**
 * Start a test eCall  session
 *
 * @return
 *      - LE_OK on success
 *      - LE_BUSY an eCall session is already in progress
 *      - LE_BAD_PARAMETER bad eCall reference
 *      - LE_FAULT for other failures
 *      - LE_UNSUPPORTED Not supported on this platform
 *
 * @note The process exits, if an invalid eCall reference is given
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_ecall_StartTest
(
    le_ecall_CallRef_t ecallRef
        ///< [IN] eCall reference
);

//--------------------------------------------------------------------------------------------------
/**
 * End the current eCall session
 *
 * @return
 *      - LE_OK on success
 *      - LE_BAD_PARAMETER bad eCall reference
 *      - LE_FAULT for other failures
 *      - LE_UNSUPPORTED Not supported on this platform
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_ecall_End
(
    le_ecall_CallRef_t ecallRef
        ///< [IN] eCall reference
);

//--------------------------------------------------------------------------------------------------
/**
 * Get the current state for the given eCall
 *
 * @return
 *      - The current state for the given eCall
 *
 * @note The process exits, if an invalid eCall reference is given
 */
//--------------------------------------------------------------------------------------------------
le_ecall_State_t le_ecall_GetState
(
    le_ecall_CallRef_t ecallRef
        ///< [IN] eCall reference
);

//--------------------------------------------------------------------------------------------------
/**
 * Set the Public Safely Answering Point telephone number.
 *
 * @note That PSAP number is not applied to Manually or Automatically initiated eCall. For those
 *       modes, an emergency call is launched.
 *
 * @warning This function doesn't modified the U/SIM content.
 *
 * @return
 *  - LE_OK on success
 *  - LE_FAULT for other failures
 *
 * @note If PSAP number is too long (max LE_MDMDEFS_PHONE_NUM_MAX_LEN digits), it is a fatal error,
 *       the function will not return.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_ecall_SetPsapNumber
(
    const char* LE_NONNULL psap
        ///< [IN] Public Safely Answering Point number
);

//--------------------------------------------------------------------------------------------------
/**
 * Get the Public Safely Answering Point telephone number set with le_ecall_SetPsapNumber()
 * function.
 *
 * @note That PSAP number is not applied to Manually or Automatically initiated eCall. For those
 *       modes, an emergency call is launched.
 *
 * @warning This function doesn't read the U/SIM content.
 *
 * @return
 *  - LE_OK on success
 *  - LE_FAULT on failures or if le_ecall_SetPsapNumber() has never been called before.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_ecall_GetPsapNumber
(
    char* psap,
        ///< [OUT] Public Safely Answering Point telephone number
    size_t psapSize
        ///< [IN]
);

//--------------------------------------------------------------------------------------------------
/**
 * When modem is in ECALL_FORCED_PERSISTENT_ONLY_MODE or ECALL_ONLY_MODE, this function
 * can be called to request the modem to read the number to dial from the FDN/SDN of the U/SIM.
 *
 * @note If FDN directory is updated with new dial numbers, be sure that the SIM card is refreshed.
 *
 * @return
 *  - LE_OK on success
 *  - LE_FAULT for other failures
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_ecall_UseUSimNumbers
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * Set the NAD (network access device) deregistration time value. After termination of an emergency
 * call the in-vehicle system remains registered on the network for the period of time, defined by
 * the installation parameter NAD (network access device) deregistration time.
 *
 * @return
 *  - LE_OK on success
 *  - LE_FAULT on failure
 *
 * @note The formula to calculate NAD deregistration time for PAN_EUROPEAN is as below:
 *       ECallConfiguration.nad_deregistration_time = (deregTime+59)/60;
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_ecall_SetNadDeregistrationTime
(
    uint16_t deregTime
        ///< [IN] the NAD (network access device) deregistration time value (in minutes).
);

//--------------------------------------------------------------------------------------------------
/**
 * Get the NAD (network access device) deregistration time value.
 *
 * @return
 *  - LE_OK on success
 *  - LE_FAULT on failure
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_ecall_GetNadDeregistrationTime
(
    uint16_t* deregTimePtr
        ///< [OUT] the NAD (network access device) deregistration time value
);

//--------------------------------------------------------------------------------------------------
/**
 * Set the minimum interval value between dial attempts.
 *
 * @return
 *  - LE_OK on success
 *  - LE_FAULT for other failures
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_ecall_SetIntervalBetweenDialAttempts
(
    uint16_t pause
        ///< [IN] the minimum interval value in seconds
);

//--------------------------------------------------------------------------------------------------
/**
 * Get the minimum interval value between dial attempts.
 *
 * @return
 *  - LE_OK on success
 *  - LE_FAULT for other failures
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_ecall_GetIntervalBetweenDialAttempts
(
    uint16_t* pausePtr
        ///< [OUT] the minimum interval value in seconds
);

//--------------------------------------------------------------------------------------------------
/**
 * Set the ECALL_MANUAL_DIAL_ATTEMPTS value. If a dial attempt under manual emergency call
 * initiation failed, it should be repeated maximally ECALL_MANUAL_DIAL_ATTEMPTS-1 times within
 * the maximal time limit of ECALL_DIAL_DURATION. The default value is 10.
 * Redial attempts stop once the call has been cleared down correctly, or if counter/timer reached
 * their limits.
 *
 * @return
 *  - LE_OK on success
 *  - LE_FAULT on failure
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_ecall_SetEraGlonassManualDialAttempts
(
    uint16_t attempts
        ///< [IN] the ECALL_MANUAL_DIAL_ATTEMPTS value
);

//--------------------------------------------------------------------------------------------------
/**
 * Set the ECALL_AUTO_DIAL_ATTEMPTS value. If a dial attempt under automatic emergency call
 * initiation failed, it should be repeated maximally ECALL_AUTO_DIAL_ATTEMPTS-1 times within
 * the maximal time limit of ECALL_DIAL_DURATION. The default value is 10.
 * Redial attempts stop once the call has been cleared down correctly, or if counter/timer reached
 * their limits.
 *
 * @return
 *  - LE_OK on success
 *  - LE_FAULT on failure
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_ecall_SetEraGlonassAutoDialAttempts
(
    uint16_t attempts
        ///< [IN] the ECALL_AUTO_DIAL_ATTEMPTS value
);

//--------------------------------------------------------------------------------------------------
/**
 * Set the ECALL_DIAL_DURATION time. It's the maximum time the IVS have to connect the emergency
 * call to the PSAP, including all redial attempts.
 * If the call is not connected within this time (or ManualDialAttempts/AutoDialAttempts dial
 * attempts), it will stop.
 *
 * @return
 *  - LE_OK on success
 *  - LE_FAULT on failure
 *  - LE_UNSUPPORTED if the function is not supported by the target
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_ecall_SetEraGlonassDialDuration
(
    uint16_t duration
        ///< [IN] the ECALL_DIAL_DURATION time value (in seconds)
);

//--------------------------------------------------------------------------------------------------
/**
 * Set the ECALL_CCFT time. It's the maximum delay before initiating an an automatic call
 * termination.
 * When the delay is reached and IVS NAD didn't receive a call clear-down indication then the call
 * is immediatly terminated.
 *
 * @note Allowed range of values is 1 to 720 minutes.
 *
 * @return
 *  - LE_OK on success
 *  - LE_FAULT on failure
 *  - LE_UNSUPPORTED if the function is not supported by the target
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_ecall_SetEraGlonassFallbackTime
(
    uint16_t duration
        ///< [IN] the ECALL_CCFT time value (in minutes)
);

//--------------------------------------------------------------------------------------------------
/**
 * Set the ECALL_AUTO_ANSWER_TIME time, the time interval wherein IVDS responds to incoming calls
 * automatically after emergency call completion.
 *
 * @note Default value of auto answer time is 20 minutes. Maximum value is 720 minutes.
 *
 * @return
 *  - LE_OK on success
 *  - LE_FAULT on failure
 *  - LE_UNSUPPORTED if the function is not supported by the target
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_ecall_SetEraGlonassAutoAnswerTime
(
    uint16_t autoAnswerTime
        ///< [IN] The ECALL_AUTO_ANSWER_TIME time value (in minutes)
);

//--------------------------------------------------------------------------------------------------
/**
 * Set the ECALL_MSD_MAX_TRANSMISSION_TIME time, the time period for MSD transmission.
 *
 * @note Default value of MSD transmission time is 20 seconds.
 *
 * @return
 *  - LE_OK on success
 *  - LE_FAULT on failure
 *  - LE_UNSUPPORTED if the function is not supported by the target
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_ecall_SetEraGlonassMSDMaxTransmissionTime
(
    uint16_t msdMaxTransTime
        ///< [IN] The ECALL_MSD_MAX_TRANSMISSION_TIME time value (in seconds)
);

//--------------------------------------------------------------------------------------------------
/**
 * Get the ECALL_MANUAL_DIAL_ATTEMPTS value.
 *
 * @return
 *  - LE_OK on success
 *  - LE_FAULT on failure
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_ecall_GetEraGlonassManualDialAttempts
(
    uint16_t* attemptsPtr
        ///< [OUT] the ECALL_MANUAL_DIAL_ATTEMPTS value
);

//--------------------------------------------------------------------------------------------------
/**
 * Get the ECALL_AUTO_DIAL_ATTEMPTS value.
 *
 * @return
 *  - LE_OK on success
 *  - LE_FAULT on failure
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_ecall_GetEraGlonassAutoDialAttempts
(
    uint16_t* attemptsPtr
        ///< [OUT] the ECALL_AUTO_DIAL_ATTEMPTS value
);

//--------------------------------------------------------------------------------------------------
/**
 * Get the ECALL_DIAL_DURATION time.
 *
 * @return
 *  - LE_OK on success
 *  - LE_FAULT on failure
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_ecall_GetEraGlonassDialDuration
(
    uint16_t* durationPtr
        ///< [OUT] the ECALL_DIAL_DURATION time value (in seconds)
);

//--------------------------------------------------------------------------------------------------
/**
 * Get the ECALL_CCFT time.
 *
 * @return
 *  - LE_OK on success
 *  - LE_FAULT on failure
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_ecall_GetEraGlonassFallbackTime
(
    uint16_t* durationPtr
        ///< [OUT] the ECALL_CCFT time value (in minutes)
);

//--------------------------------------------------------------------------------------------------
/**
 * Get the ECALL_AUTO_ANSWER_TIME time.
 *
 * @return
 *  - LE_OK on success
 *  - LE_FAULT on execution failure
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_ecall_GetEraGlonassAutoAnswerTime
(
    uint16_t* autoAnswerTimePtr
        ///< [OUT] The ECALL_AUTO_ANSWER_TIME time value (in minutes)
);

//--------------------------------------------------------------------------------------------------
/**
 * Get the ECALL_MSD_MAX_TRANSMISSION_TIME time.
 *
 * @return
 *  - LE_OK on success
 *  - LE_FAULT on failure
 *  - LE_UNSUPPORTED if the function is not supported by the target
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_ecall_GetEraGlonassMSDMaxTransmissionTime
(
    uint16_t* msdMaxTransTimePtr
        ///< [OUT] The ECALL_MSD_MAX_TRANSMISSION_TIME time value (in seconds)
);

//--------------------------------------------------------------------------------------------------
/**
 * Set the ERA-GLONASS crash severity parameter.
 *
 * @return
 *  - LE_OK on success
 *  - LE_DUPLICATE an MSD has been already imported
 *  - LE_BAD_PARAMETER bad eCall reference
 *  - LE_FAULT on other failures
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_ecall_SetMsdEraGlonassCrashSeverity
(
    le_ecall_CallRef_t ecallRef,
        ///< [IN] eCall reference
    uint32_t crashSeverity
        ///< [IN] the ERA-GLONASS crash severity parameter
);

//--------------------------------------------------------------------------------------------------
/**
 * Reset the ERA-GLONASS crash severity parameter. Therefore that optional parameter is not included
 * in the MSD message.
 *
 * @return
 *  - LE_OK on success
 *  - LE_DUPLICATE an MSD has been already imported
 *  - LE_BAD_PARAMETER bad eCall reference
 *  - LE_FAULT on other failures
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_ecall_ResetMsdEraGlonassCrashSeverity
(
    le_ecall_CallRef_t ecallRef
        ///< [IN] eCall reference
);

//--------------------------------------------------------------------------------------------------
/**
 * Set the ERA-GLONASS diagnostic result using a bit mask.
 *
 * @return
 *  - LE_OK on success
 *  - LE_DUPLICATE an MSD has been already imported
 *  - LE_BAD_PARAMETER bad eCall reference
 *  - LE_FAULT on other failures
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_ecall_SetMsdEraGlonassDiagnosticResult
(
    le_ecall_CallRef_t ecallRef,
        ///< [IN] eCall reference
    le_ecall_DiagnosticResultBitMask_t diagnosticResultMask
        ///< [IN] ERA-GLONASS diagnostic result bit mask.
);

//--------------------------------------------------------------------------------------------------
/**
 * Reset the ERA-GLONASS diagnostic result bit mask. Optional parameter is not
 * included in the MSD message.
 *
 * @return
 *  - LE_OK on success
 *  - LE_DUPLICATE an MSD has been already imported
 *  - LE_BAD_PARAMETER bad eCall reference
 *  - LE_FAULT on other failures
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_ecall_ResetMsdEraGlonassDiagnosticResult
(
    le_ecall_CallRef_t ecallRef
        ///< [IN] eCall reference
);

//--------------------------------------------------------------------------------------------------
/**
 * Set the ERA-GLONASS crash type bit mask
 *
 * @return
 *  - LE_OK on success
 *  - LE_DUPLICATE an MSD has been already imported
 *  - LE_BAD_PARAMETER bad eCall reference
 *  - LE_FAULT on other failures
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_ecall_SetMsdEraGlonassCrashInfo
(
    le_ecall_CallRef_t ecallRef,
        ///< [IN] eCall reference
    le_ecall_CrashInfoBitMask_t crashInfoMask
        ///< [IN] ERA-GLONASS crash type bit mask.
);

//--------------------------------------------------------------------------------------------------
/**
 * Reset the ERA-GLONASS crash type bit mask. Optional parameter is not included
 * in the MSD message.
 *
 * @return
 *  - LE_OK on success
 *  - LE_DUPLICATE an MSD has been already imported
 *  - LE_BAD_PARAMETER bad eCall reference
 *  - LE_FAULT on other failures
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_ecall_ResetMsdEraGlonassCrashInfo
(
    le_ecall_CallRef_t ecallRef
        ///< [IN] eCall reference
);

//--------------------------------------------------------------------------------------------------
/**
 * Set the ERA-GLONASS coordinate system type.
 *
 * @return
 *  - LE_OK on success
 *  - LE_DUPLICATE an MSD has been already imported
 *  - LE_BAD_PARAMETER bad eCall reference
 *  - LE_FAULT on other failures
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_ecall_SetMsdEraGlonassCoordinateSystemType
(
    le_ecall_CallRef_t ecallRef,
        ///< [IN] eCall reference
    le_ecall_MsdCoordinateType_t coordinateSystemType
        ///< [IN] the ERA-GLONASS coordinate system type
);

//--------------------------------------------------------------------------------------------------
/**
 * Reset the ERA-GLONASS coordinate system type. Therefore that optional parameter is not included
 * in the MSD message.
 *
 * @return
 *  - LE_OK on success
 *  - LE_DUPLICATE an MSD has been already imported
 *  - LE_BAD_PARAMETER bad eCall reference
 *  - LE_FAULT on other failures
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_ecall_ResetMsdEraGlonassCoordinateSystemType
(
    le_ecall_CallRef_t ecallRef
        ///< [IN] eCall reference
);

//--------------------------------------------------------------------------------------------------
/**
 * Called to get the termination reason.
 *
 * @return The termination reason.
 *
 * @note If the caller is passing a bad pointer into this function, it is a fatal error, the
 *       function will not return.
 */
//--------------------------------------------------------------------------------------------------
le_mcc_TerminationReason_t le_ecall_GetTerminationReason
(
    le_ecall_CallRef_t ecallRef
        ///< [IN] eCall reference.
);

//--------------------------------------------------------------------------------------------------
/**
 * Called to get the platform specific termination code.
 *
 * @return The platform specific termination code.
 *
 * @note If the caller is passing a bad pointer into this function, it is a fatal error, the
 *       function will not return.
 */
//--------------------------------------------------------------------------------------------------
int32_t le_ecall_GetPlatformSpecificTerminationCode
(
    le_ecall_CallRef_t ecallRef
        ///< [IN] eCall reference.
);

#endif // LE_ECALL_INTERFACE_H_INCLUDE_GUARD