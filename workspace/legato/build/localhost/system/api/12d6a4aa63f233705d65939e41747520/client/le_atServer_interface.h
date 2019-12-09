

/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */

/**
 * @page c_atServer AT Commands Server
 *
 * @ref le_atServer_interface.h "API Reference"
 *
 * The AT commands Server handles AT commands' subscriptions on a requested serial device. The
 * server is compliant with 3GPP 27.007, paragraphs 4.0, 4.1 and 4.2, and V25 ter, paragraphs 5.3,
 * 5.4.
 *
 * @section le_atServer_binding IPC interfaces binding
 *
 * All the functions of this API are provided by the @b atService.
 *
 * Here's a code sample binding to the AT commands server:
 * @verbatim
   bindings:
   {
     atServerTest.atServerTestComp.le_atServer -> atService.le_atServer
   }
   @endverbatim
 *
 * @section atServer_syntax AT command syntax
 *
 * @subsection atServer_Syntax Syntax rules
 *
 * To be interpreted, the command line sent to the AT commands server must start by the pattern
 * "AT" (mean ATtention command).<br>
 * Lowercase characters are changed to their uppercase equivalents. Only characters between quotes
 * are not replaced.
 *
 * The supported command formats are:
 * - Basic syntax command:
 *      - using the format AT<command>[<number>]<br>
 * The command name is composed of one or several of those characters: A to Z, & and \\.
 *      - the syntax of S command is also supported, like:
 *          - ATS<parameter_number>?
 *          - ATS<parameter_number>=\<value>
 *      - D command is supported. The characters which don't belong to the following list are
 * ignore:
 *              - V.250 dialing digits: 0 1 2 3 4 5 6 7 8 9 * # + A B C D
 *              - V.250 modifier characters: , T P ! W @
 *              - V.250 semicolon character: ;
 *              - GSM/UMTS modifier characters:
 *                  - I or i for CLIR supplementary service subscription
 * (I=invocation, i=suppression)
 *                  - G or g for CUG supplementary service subscription (G=invocation,
 * g=suppression)
 *                  - Direct dialing from phonebook: > (if follow by a string, it has to be put
 * between quote)
 *              .
 * @if 0 doxygen comment: The dot above ends the sub item, and must be kept @endif
 * All characters after the "D" are considered part of The D command parameter up to a semicolon or
 * the end of command line.
 * <br>
 * - Extended command format, with the following format:
 *      - action command with no parameters: AT+<name>
 *      - parameter command: AT+<name>=<value1>[,<value2>[,<value3>[...]]]<br>
 *        Values must be separated by a coma. Some values may be optional: in that case, optional
 *        value can be omitted in the AT command (e.g. AT+ABCD=,1).<br>
 *        A value is a string composed of one or several of the following characters: 0 to 9,
 *        A to F, H, z*, #, +, -. To set a value with other characters, the value has to be set
 * between quote.
 *      - test command (determine the supported values): AT+<name>=?
 *      - read command (determine the current values, or stored values): AT+<name>?
 *
 * @note '+' AT command starting character shall be replaced by other symbol, not included into
 * a to z, A to Z, & and \\.
 *
 * @subsection atServer_concate Concatenating commands
 *
 * Basic syntax command can be concatenated without a separator:<br>
 * ATE1V1S95=47S0=0
 *
 * Additional commands (extended syntax command or basic syntax command) can be added after an
 * extended syntax command with the insertion of a semicolon at the end of the command:<br>
 * AT+CMDA=<param1>,<param2>;+CMDB?;+CMDC=?<br>
 * AT+CMDA=<param1>,<param2>;E1V1
 *
 * Extended syntax command can be concatenated after basic syntax commands without a separator:<br>
 * ATE1V1+CMD=<param1>
 *
 * @section atServer_binding Device Binding
 *
 * le_atServer_Open() must be called to bind the file descriptor of the device with the AT commands
 * server. Note that a socket can also be bound.
 * Multiple devices can be bound.
 * A file descriptor can be unbound using le_atServer_Close().
 *
 * The server can be suspended using le_atServer_Suspend() in order to use the
 * opened fd for other purposes like starting a PPP service on the opened fd.
 * For that a fd dup needs to be done before opening a server session.
 * When needed, the server can be resumed using le_atServer_Resume(). Make sure
 * to close the fd when the application exists or you may get too many open files error.
 *
 * used before opening a server session
 * @section atServer_subscription Subscription
 *
 * A new AT command can be added into the parser using le_atServer_Create(), and it can be deleted
 * using le_atServer_Delete().
 * le_atServer_EnableEcho() allows the user to enable echo on a selected device.
 * le_atServer_DisableEcho() allows the user to disable echo on a selected device.
 *
 * @section atServer_handler Handler
 *
 * To handle the AT command, the application has to subscribe a handler using
 * le_atServer_AddCommandHandler(). It can be removed with le_atServer_RemoveCommandHandler().
 *
 * The called handler (le_atServer_CommandHandlerRef_t prototype) can use
 * le_atServer_GetCommandName() to retrieve the received AT command string.
 *
 * The device used to execute the AT command can be retrieve thanks to le_atServer_GetDevice().
 *
 * It can also call le_atServer_GetParameter() to retrieve parameters of the AT command. This
 * function gets the string sending through the AT command. If the parameter was sent between
 * quotes, the quotes are removed. This API can be used for both formats:
 * - In case of a basic format command, if exists, the parameter can be retrieved at the index 0.
 *
 * For S command specific format (ATS<parameter_number>=\<value>), the <parameter_number> is
 * retrieved at the index 0, the \<value> parameter at the index 1.
 *
 * - In case of an extended format command, parameters are retrieved thanks to their indexes,
 * starting from 0. If the parameter is missed (e.g. "AT+CMD=,1"), the getting value is an empty
 * string (i.e. '\0' with null length).
 *
 * The handler receives in argument the type of the AT command (of le_atServer_Type_t type).
 * Even if these types are specific to the extended format commands according to the standards, they
 * are also applicable here to basic format commands to detect commands with parameters, or read
 * values (e.g. ATS<parameter_number>?).
 *
 * @note If the parameter is parsed with quotes, the quotes are removed when retrieving the
 * parameter value using le_atServer_GetParameter() API. If a parmeter is not parsed with quotes,
 * that parameter is converted to uppercase equivalent.
 *
 * @section atServer_responses Responses
 *
 * @subsection intermediateRsp Intermediate response
 *
 * The application has can send intermediate responses through
 * le_atServer_SendIntermediateResponse().
 *
 * @subsection finalResultCode Final result code
 *
 * The application must return a final result code using le_atServer_SendFinalResultCode().
 * The corresponding device will be locked until the final response is sent.
 *
 * If no answer is sent, the device will not accept any new AT commands (an error will be
 * returned).
 *
 * le_atServer_SendFinalResultCode() takes as arguments the AT command reference, a pattern (which
 * is the prefix of the final response), a final response type as defined in
 * @ref le_atServer_FinalRsp_t and an error code identifier.
 *
 * The final response type permits to the AT command Server to continue or stop the parsing of
 * concatenated commands: if one command is failed, next commands are not executed, the final result
 * of the concatenated AT command is the last error.
 *
 * @subsection finalRsp Final response
 *
 * @deprecated le_atServer_SendFinalResponse() should not be used anymore and will be removed soon.
 * It has been replaced by le_atServer_SendFinalResultCode().
 *
 * In all cases, the application must send a final response using
 * le_atServer_SendFinalResponse(). The corresponding device will be locked until the final response
 * is sent.
 *
 * If no answer is sent, the device will not accept any new AT commands (an error will be
 * returned).
 *
 * le_atServer_SendFinalResponse() has to inform of the result thanks to the argument of
 * le_atServer_FinalRsp_t type. This argument permits to the AT command Server to:
 * - create the result string if no custom string is provided
 * - continue or stop the parsing of concatenated commands: if one command is failed, next commands
 * are not executed, the final result of the concatenated AT command is the last error.
 *
 * @subsection unsolicitedRsp Unsolicited response
 *
 * The application can also send unsolicited responses to warn a host
 * application using le_atServer_SendUnsolicitedResponse().
 *
 * This response is sent when no AT command is being processing on the device (i.e. unsolicited
 * response is sent between the latest final response and the next reception of an AT command).
 *
 * If an unsolicited response is sent when an AT command is in progress, the unsolicited response
 * is buffered and sent as soon as the device becomes available (i.e., the processing AT command
 * sends its final response).
 *
 * @section atServer_errors Error codes
 *
 * le_atServer_EnableExtendedErrorCodes() allows the user to use extended error codes on a selected
 * device. When this mode is enabled, numerical codes are displayed when an error occurs.
 * le_atServer_EnableVerboseErrorCodes() allows the user to enable verbose error codes on a selected
 * device. Thus, instead of numerical codes, error are actually displayed as verbose messages.
 * le_atServer_DisableExtendedErrorCodes() allows the user to disable the current error mode namely
 * extended error codes or verbose error codes on a selected device.
 *
 * User can create custom error codes using le_atServer_CreateErrorCode() by providing an error
 * code identifier and a specific pattern. The pattern is a prefix of the final response string.
 * Standard error codes use the patterns "+CME ERROR: " and "+CMS ERROR: " for instance. These
 * standard patterns are defined in the following macros: CME_ERROR and CMS_ERROR.
 * The code identifier should be equal or higher than 512 as the range [0, 511] is reserved for
 * standard error codes defined in 3GPP 27.007 9.2 and TS 127.005 3.2.5.
 *
 * le_atServer_CreateErrorCode() returns a reference which can be used to attach a custom verbose
 * message to the error codes by calling le_atServer_SetVerboseErrorCode().
 * le_atServer_DeleteErrorCode() allows the user to drop an already registered error code.
 *
 * @section Text
 *
 * le_atServer_GetTextAsync() allows the user to register a le_atServer_GetTextCallback_t callback
 * to retrieve text and sends a prompt <CR><LF><greater_than><SPACE> on the current command's
 * device.
 *
 * It will receive at max LE_ATDEFS_TEXT_MAX_LEN bytes.
 * If Esc Key is hit then the command is canceled and an empty buffer is returned with result
 * set to LE_OK.
 * If Ctrl+z is hit then the received buffer is returned and the result is set to LE_OK.
 * In case of a read error, an empty buffer is returned with result set to LE_IO_ERROR;
 *
 * Example:
 *
 * AT+CMGS="+85291234567"
 * > <ESC>
 *
 * OK
 *
 * AT+CMGS="+85291234567"
 * > It is easy to send text messages. <CTRL-Z>
 * +CMGS: 5
 *
 * OK
 *
 * @section atServer_bridge Bridge
 *
 * A second file descriptor can be used thanks to le_atServer_OpenBridge() to send
 * all unknown AT commands to an alternative device (such as the modem). For all devices linked to
 * that bridge using le_atServer_AddDeviceToBridge(), unknown commands will be sent through that
 * file descriptor.
 *
 * The bridge only works with AT commands that have the following terminal responses:
 * - "OK"
 * - "NO CARRIER"
 * - "NO DIALTONE"
 * - "BUSY"
 * - "NO ANSWSER"
 * - "ERROR"
 * - "+CME ERROR"
 * - "+CMS ERROR"
 *
 * AT commands executed through the bridge do not support text mode (e.g.; +CMGS) or data mode
 * (e.g.; ATD*99***1#). Sending these commands through the bridge may lock the Legato AT commands
 * parser.
 *
 * @image html atCommandsParserBridge.png
 *
 * @note AT commands server is opened on the file descriptor fd1 using le_atServer_Open() API.
 * AT commands server is bridged on the file descriptor fd2 using le_atServer_OpenBridge() API.
 *
 * A device can be remove from a bridge thanks to le_atServer_RemoveDeviceFromBridge() API.
 * A bridge can be closed using le_atServer_CloseBridge() API.
 *
 * @warning Some modem AT commands may conflict with Legato APIs; using both may cause problems that
 * can be difficult to diagnose. The modem AT commands should be avoided whenever possible, and
 * should only be used with great care.
 *
 * The application can send an unsolicited on all opened device, or only one on a dedicated device.
 *
 *
 * Copyright (C) Sierra Wireless Inc.
 */
/**
 * @file le_atServer_interface.h
 *
 * Legato @ref c_atServer include file.
 *
 * Copyright (C) Sierra Wireless Inc.
 */

#ifndef LE_ATSERVER_INTERFACE_H_INCLUDE_GUARD
#define LE_ATSERVER_INTERFACE_H_INCLUDE_GUARD


#include "legato.h"

// Interface specific includes
#include "le_atDefs_interface.h"


//--------------------------------------------------------------------------------------------------
/**
 * Type for handler called when a server disconnects.
 */
//--------------------------------------------------------------------------------------------------
typedef void (*le_atServer_DisconnectHandler_t)(void *);

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
void le_atServer_ConnectService
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
le_result_t le_atServer_TryConnectService
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
void le_atServer_SetServerDisconnectHandler
(
    le_atServer_DisconnectHandler_t disconnectHandler,
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
void le_atServer_DisconnectService
(
    void
);


//--------------------------------------------------------------------------------------------------
/**
 *  Maximum text length.
 */
//--------------------------------------------------------------------------------------------------
#define LE_ATSERVER_TEXT_MAX_LEN 4096

//--------------------------------------------------------------------------------------------------
/**
 *  CME error pattern
 */
//--------------------------------------------------------------------------------------------------
#define LE_ATSERVER_CME_ERROR "+CME ERROR: "

//--------------------------------------------------------------------------------------------------
/**
 *  CMS error pattern
 */
//--------------------------------------------------------------------------------------------------
#define LE_ATSERVER_CMS_ERROR "+CMS ERROR: "

//--------------------------------------------------------------------------------------------------
/**
 *  Reference type for an AT command.
 */
//--------------------------------------------------------------------------------------------------
typedef struct le_atServer_Cmd* le_atServer_CmdRef_t;


//--------------------------------------------------------------------------------------------------
/**
 *  Reference type for a AT command device.
 */
//--------------------------------------------------------------------------------------------------
typedef struct le_atServer_Device* le_atServer_DeviceRef_t;


//--------------------------------------------------------------------------------------------------
/**
 *  Reference type for a AT commands server / AT commands client bridge.
 */
//--------------------------------------------------------------------------------------------------
typedef struct le_atServer_Bridge* le_atServer_BridgeRef_t;


//--------------------------------------------------------------------------------------------------
/**
 *  Reference type for an error code.
 */
//--------------------------------------------------------------------------------------------------
typedef struct le_atServer_ErrorCode* le_atServer_ErrorCodeRef_t;


//--------------------------------------------------------------------------------------------------
/**
 * Received AT command type.
 *
 */
//--------------------------------------------------------------------------------------------------
typedef enum
{
    LE_ATSERVER_TYPE_ACT = 0,
        ///< Action AT command (e.g. 'AT+CMD')
    LE_ATSERVER_TYPE_PARA = 1,
        ///< AT command received with parameters (e.g. 'AT+CMD=x,y')
    LE_ATSERVER_TYPE_TEST = 2,
        ///< Test AT command (e.g. 'AT+CMD=?')
    LE_ATSERVER_TYPE_READ = 3
        ///< Read AT command (e.g. 'AT+CMD?')
}
le_atServer_Type_t;


//--------------------------------------------------------------------------------------------------
/**
 * Final response.
 */
//--------------------------------------------------------------------------------------------------
typedef enum
{
    LE_ATSERVER_OK = 0,
        ///< AT command successfully treated
    LE_ATSERVER_NO_CARRIER = 1,
        ///< Connection terminated
    LE_ATSERVER_ERROR = 2,
        ///< Error in AT command treatement
    LE_ATSERVER_NO_DIALTONE = 3,
        ///< No dialtone detected
    LE_ATSERVER_BUSY = 4
        ///< Busy signal detected
}
le_atServer_FinalRsp_t;


//--------------------------------------------------------------------------------------------------
/**
 * Device availability.
 */
//--------------------------------------------------------------------------------------------------
typedef enum
{
    LE_ATSERVER_ALL_DEVICES = 0,
        ///< action will be done on all bound devices
    LE_ATSERVER_SPECIFIC_DEVICE = 1
        ///< action will be done on a specified device
}
le_atServer_AvailableDevice_t;


//--------------------------------------------------------------------------------------------------
/**
 * Reference type used by Add/Remove functions for EVENT 'le_atServer_Command'
 */
//--------------------------------------------------------------------------------------------------
typedef struct le_atServer_CommandHandler* le_atServer_CommandHandlerRef_t;


//--------------------------------------------------------------------------------------------------
/**
 * Handler for the AT command processing.
 *
 * @note The argument "parametersNumber" is set only when "type" parameter value is
 * LE_AT_SERVER_TYPE_PARA
 */
//--------------------------------------------------------------------------------------------------
typedef void (*le_atServer_CommandHandlerFunc_t)
(
    le_atServer_CmdRef_t commandRef,
        ///< Received AT command reference
    le_atServer_Type_t type,
        ///< Received AT command type
    uint32_t parametersNumber,
        ///< Parameters number
    void* contextPtr
        ///<
);

//--------------------------------------------------------------------------------------------------
/**
 * Get text callback
 *
 * @return
 *      - LE_OK: The function succeeded
 *      - LE_IO_ERROR: An io error happened and the function couldn't read from the device
 *      - LE_FORMAT_ERROR: Received an invalid character or an invalid sequence
 *      - LE_FAULT: Failed to remove backspaces
 */
//--------------------------------------------------------------------------------------------------
typedef void (*le_atServer_GetTextCallbackFunc_t)
(
    le_atServer_CmdRef_t cmdRef,
        ///< Received AT command reference
    le_result_t result,
        ///< Result
    const char* LE_NONNULL text,
        ///< Received text
    uint32_t len,
        ///< Text length
    void* contextPtr
        ///<
);

//--------------------------------------------------------------------------------------------------
/**
 * Suspend server / enter data mode
 *
 * When this function is called the server stops monitoring the fd for events
 * hence no more I/O operations are done on the fd by the server.
 *
 * @return
 *      - LE_OK             Success.
 *      - LE_BAD_PARAMETER  Invalid device reference.
 *      - LE_FAULT          Device not monitored
 *
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_atServer_Suspend
(
    le_atServer_DeviceRef_t device
        ///< [IN] device to be suspended
);

//--------------------------------------------------------------------------------------------------
/**
 * Resume server / enter command mode
 *
 * When this function is called the server resumes monitoring the fd for events
 * and is able to interpret AT commands again.
 *
 * @return
 *      - LE_OK             Success.
 *      - LE_BAD_PARAMETER  Invalid device reference.
 *      - LE_FAULT          Device not monitored
 *
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_atServer_Resume
(
    le_atServer_DeviceRef_t device
        ///< [IN] device to be resumed
);

//--------------------------------------------------------------------------------------------------
/**
 * This function opens an AT server session on the requested device.
 *
 * @return
 *      - Reference to the requested device.
 *      - NULL if the device is not available or fd is a BAD FILE DESCRIPTOR.
 *
 * @note Make sure to duplicate (man dup) your file descriptor before opening
 *       a server session to be able to use the suspend/resume feature
 *
 */
//--------------------------------------------------------------------------------------------------
le_atServer_DeviceRef_t le_atServer_Open
(
    int fd
        ///< [IN] File descriptor.
);

//--------------------------------------------------------------------------------------------------
/**
 * This function closes the AT server session on the requested device.
 *
 * @return
 *      - LE_OK             The function succeeded.
 *      - LE_BAD_PARAMETER  Invalid device reference.
 *      - LE_BUSY           The requested device is busy.
 *      - LE_FAULT          Failed to stop the server, check logs
 *                              for more information.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_atServer_Close
(
    le_atServer_DeviceRef_t device
        ///< [IN] device to be unbound
);

//--------------------------------------------------------------------------------------------------
/**
 * This function created an AT command and register it into the AT parser.
 *
 * @return
 *      - Reference to the AT command.
 *      - NULL if an error occurs.
 */
//--------------------------------------------------------------------------------------------------
le_atServer_CmdRef_t le_atServer_Create
(
    const char* LE_NONNULL name
        ///< [IN] AT command name string
);

//--------------------------------------------------------------------------------------------------
/**
 * This function deletes an AT command (i.e. unregister from the AT parser).
 *
 * @return
 *      - LE_OK            The function succeeded.
 *      - LE_FAULT         The function failed to delete the command.
 *      - LE_BUSY          Command is in progress.
 *
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_atServer_Delete
(
    le_atServer_CmdRef_t commandRef
        ///< [IN] AT command reference
);

//--------------------------------------------------------------------------------------------------
/**
 * Add handler function for EVENT 'le_atServer_Command'
 *
 * This event provides information when the AT command is detected.
 *
 */
//--------------------------------------------------------------------------------------------------
le_atServer_CommandHandlerRef_t le_atServer_AddCommandHandler
(
    le_atServer_CmdRef_t commandRef,
        ///< [IN] AT command reference
    le_atServer_CommandHandlerFunc_t handlerPtr,
        ///< [IN] Handler to called when the AT command is detected
    void* contextPtr
        ///< [IN]
);

//--------------------------------------------------------------------------------------------------
/**
 * Remove handler function for EVENT 'le_atServer_Command'
 */
//--------------------------------------------------------------------------------------------------
void le_atServer_RemoveCommandHandler
(
    le_atServer_CommandHandlerRef_t handlerRef
        ///< [IN]
);

//--------------------------------------------------------------------------------------------------
/**
 * This function can be used to get the parameters of a received AT command.
 *
 * @return
 *      - LE_OK            The function succeeded.
 *      - LE_FAULT         The function failed to get the requested parameter.
 *
 * @note If the parameter is parsed with quotes, the quotes are removed when retrieving the
 * parameter value using this API. If a parmeter is not parsed with quotes, that parameter is
 * converted to uppercase equivalent.
 *
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_atServer_GetParameter
(
    le_atServer_CmdRef_t commandRef,
        ///< [IN] AT command reference
    uint32_t index,
        ///< [IN] agument index
    char* parameter,
        ///< [OUT] parameter value
    size_t parameterSize
        ///< [IN]
);

//--------------------------------------------------------------------------------------------------
/**
 * This function can be used to get the AT command string.
 *
 * @return
 *      - LE_OK            The function succeeded.
 *      - LE_FAULT         The function failed to get the AT command string.
 *
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_atServer_GetCommandName
(
    le_atServer_CmdRef_t commandRef,
        ///< [IN] AT command reference
    char* name,
        ///< [OUT] AT command string
    size_t nameSize
        ///< [IN]
);

//--------------------------------------------------------------------------------------------------
/**
 * This function can be used to get the device reference in use for an AT command specified with
 * its reference.
 *
 * @return
 *      - LE_OK            The function succeeded.
 *      - LE_FAULT         The function failed to get the AT command string.
 *
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_atServer_GetDevice
(
    le_atServer_CmdRef_t commandRef,
        ///< [IN] AT command reference
    le_atServer_DeviceRef_t* deviceRefPtr
        ///< [OUT] Device reference
);

//--------------------------------------------------------------------------------------------------
/**
 * This function can be used to send an intermediate response.
 *
 * @return
 *      - LE_OK            The function succeeded.
 *      - LE_FAULT         The function failed to send the intermediate response.
 *
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_atServer_SendIntermediateResponse
(
    le_atServer_CmdRef_t commandRef,
        ///< [IN] AT command reference
    const char* LE_NONNULL intermediateRsp
        ///< [IN] Intermediate response to be
        ///< sent
);

//--------------------------------------------------------------------------------------------------
/**
 * This function can be used to send the final response.
 *
 * @deprecated le_atServer_SendFinalResponse() should not be used anymore and will be removed soon.
 * It has been replaced by le_atServer_SendFinalResultCode()
 *
 * @return
 *      - LE_OK            The function succeeded.
 *      - LE_FAULT         The function failed to send the final response.
 *
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_atServer_SendFinalResponse
(
    le_atServer_CmdRef_t commandRef,
        ///< [IN] AT command reference
    le_atServer_FinalRsp_t finalResponse,
        ///< [IN] Final response to be sent
    bool customStringAvailable,
        ///< [IN] Custom final response has to be sent
        ///< instead of the default one.
    const char* LE_NONNULL finalRsp
        ///< [IN] custom final response string
);

//--------------------------------------------------------------------------------------------------
/**
 * This function can be used to send the final result code.
 *
 * @return
 *      - LE_OK            The function succeeded.
 *      - LE_FAULT         The function failed to send the final result code.
 *
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_atServer_SendFinalResultCode
(
    le_atServer_CmdRef_t commandRef,
        ///< [IN] AT command reference
    le_atServer_FinalRsp_t finalResult,
        ///< [IN] Final result code to be sent
    const char* LE_NONNULL pattern,
        ///< [IN] Prefix of the return message
    uint32_t errorCode
        ///< [IN] Numeric error code
);

//--------------------------------------------------------------------------------------------------
/**
 * This function can be used to send the unsolicited response.
 *
 * @return
 *      - LE_OK            The function succeeded.
 *      - LE_FAULT         The function failed to send the unsolicited response.
 *
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_atServer_SendUnsolicitedResponse
(
    const char* LE_NONNULL unsolRsp,
        ///< [IN] Unsolicited response to be
        ///< sent
    le_atServer_AvailableDevice_t availableDevice,
        ///< [IN] device to send the
        ///< unsolicited response
    le_atServer_DeviceRef_t device
        ///< [IN] device reference where the
        ///< unsolicited response has to
        ///< be sent
);

//--------------------------------------------------------------------------------------------------
/**
 * This function enables echo on the selected device.
 *
 * @return
 *      - LE_OK             The function succeeded.
 *      - LE_BAD_PARAMETER  Invalid device reference.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_atServer_EnableEcho
(
    le_atServer_DeviceRef_t device
        ///< [IN] device reference
);

//--------------------------------------------------------------------------------------------------
/**
 * This function disables echo on the selected device.
 *
 * @return
 *      - LE_OK             The function succeeded.
 *      - LE_BAD_PARAMETER  Invalid device reference.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_atServer_DisableEcho
(
    le_atServer_DeviceRef_t device
        ///< [IN] device reference
);

//--------------------------------------------------------------------------------------------------
/**
 * This function opens a AT commands server bridge.
 * All unknown AT commands will be sent on this alternative file descriptor thanks to the AT client
 * Service.
 *
 * @return
 *      - Reference to the requested bridge.
 *      - NULL if the device can't be bridged
 */
//--------------------------------------------------------------------------------------------------
le_atServer_BridgeRef_t le_atServer_OpenBridge
(
    int fd
        ///< [IN] File descriptor.
);

//--------------------------------------------------------------------------------------------------
/**
 * This function closes an opened bridge.
 *
 * @return
 *      - LE_OK            The function succeeded.
 *      - LE_FAULT         The function failed to close the bridge.
 *      - LE_BUSY          The bridge is in use (devices references have to be removed first).
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_atServer_CloseBridge
(
    le_atServer_BridgeRef_t bridgeRef
        ///< [IN] Bridge reference
);

//--------------------------------------------------------------------------------------------------
/**
 * This function adds a device to an opened bridge.
 *
 * @return
 *      - LE_OK            The function succeeded.
 *      - LE_BUSY          The device is already used by the bridge.
 *      - LE_FAULT         The function failed to add the device to the bridge.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_atServer_AddDeviceToBridge
(
    le_atServer_DeviceRef_t deviceRef,
        ///< [IN] Device reference to add to the bridge
    le_atServer_BridgeRef_t bridgeRef
        ///< [IN] Bridge refence
);

//--------------------------------------------------------------------------------------------------
/**
 * This function removes a device from a bridge
 *
 * @return
 *      - LE_OK            The function succeeded.
 *      - LE_NOT_FOUND     The device is not isued by the specified bridge
 *      - LE_BUSY          The device is currently in use
 *      - LE_FAULT         The function failed to add the device to the bridge.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_atServer_RemoveDeviceFromBridge
(
    le_atServer_DeviceRef_t deviceRef,
        ///< [IN] Device reference to add to the bridge
    le_atServer_BridgeRef_t bridgeRef
        ///< [IN] Bridge refence
);

//--------------------------------------------------------------------------------------------------
/**
 * This function enables verbose error codes on the selected device.
 */
//--------------------------------------------------------------------------------------------------
void le_atServer_EnableVerboseErrorCodes
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * This function enables extended error codes on the selected device.
 */
//--------------------------------------------------------------------------------------------------
void le_atServer_EnableExtendedErrorCodes
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * This function disables extended error codes on the selected device.
 */
//--------------------------------------------------------------------------------------------------
void le_atServer_DisableExtendedErrorCodes
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * This function creates a custom error code.
 * @return
 *      - ErrorCode    Reference to the created error code
 *      - NULL         Function failed to create the error code
 *
 * @note This function fails to create the error code if the combinaison (errorCode, pattern)
 * already exists or if the errorCode number is lower than 512.
 *
 */
//--------------------------------------------------------------------------------------------------
le_atServer_ErrorCodeRef_t le_atServer_CreateErrorCode
(
    uint32_t errorCode,
        ///< [IN] Numerical error code
    const char* LE_NONNULL pattern
        ///< [IN] Prefix of the response message
);

//--------------------------------------------------------------------------------------------------
/**
 * This function deletes a custom error code.
 *
 * @return
 *      - LE_OK            The function succeeded
 *      - LE_FAULT         The function failed to delete the error code
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_atServer_DeleteErrorCode
(
    le_atServer_ErrorCodeRef_t errorCodeRef
        ///< [IN] Error code reference
);

//--------------------------------------------------------------------------------------------------
/**
 * This function adds a verbose message to a specified error code
 *
 * @return
 *      - LE_OK            The function succeeded
 *      - LE_FAULT         The function failed to set the verbose message
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_atServer_SetVerboseErrorCode
(
    le_atServer_ErrorCodeRef_t errorCodeRef,
        ///< [IN] Error code reference
    const char* LE_NONNULL verboseCode
        ///< [IN] Verbose message
);

//--------------------------------------------------------------------------------------------------
/**
 * This function allows the user to register a le_atServer_GetTextCallback_t callback
 * to retrieve text and sends a prompt <CR><LF><greater_than><SPACE> on the current command's
 * device.
 *
 * @return
 *      - LE_OK             The function succeeded.
 *      - LE_BAD_PARAMETER  Invalid device or command reference.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_atServer_GetTextAsync
(
    le_atServer_CmdRef_t cmdRef,
        ///< [IN] AT command reference
    le_atServer_GetTextCallbackFunc_t callbackPtr,
        ///< [IN] Get text callback
    void* contextPtr
        ///< [IN]
);

#endif // LE_ATSERVER_INTERFACE_H_INCLUDE_GUARD