

/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */


#ifndef LE_ATCLIENT_INTERFACE_H_INCLUDE_GUARD
#define LE_ATCLIENT_INTERFACE_H_INCLUDE_GUARD


#include "legato.h"

// Interface specific includes
#include "le_atDefs_server.h"


//--------------------------------------------------------------------------------------------------
/**
 * Get the server service reference
 */
//--------------------------------------------------------------------------------------------------
le_msg_ServiceRef_t le_atClient_GetServiceRef
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * Get the client session reference for the current message
 */
//--------------------------------------------------------------------------------------------------
le_msg_SessionRef_t le_atClient_GetClientSessionRef
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * Initialize the server and advertise the service.
 */
//--------------------------------------------------------------------------------------------------
void le_atClient_AdvertiseService
(
    void
);


//--------------------------------------------------------------------------------------------------
/**
 */
//--------------------------------------------------------------------------------------------------
typedef struct le_atClient_Cmd* le_atClient_CmdRef_t;


//--------------------------------------------------------------------------------------------------
/**
 */
//--------------------------------------------------------------------------------------------------
typedef struct le_atClient_Device* le_atClient_DeviceRef_t;


//--------------------------------------------------------------------------------------------------
/**
 * Reference type used by Add/Remove functions for EVENT 'le_atClient_UnsolicitedResponse'
 */
//--------------------------------------------------------------------------------------------------
typedef struct le_atClient_UnsolicitedResponseHandler* le_atClient_UnsolicitedResponseHandlerRef_t;


//--------------------------------------------------------------------------------------------------
/**
 * Handler for unsolicited response reception.
 *
 */
//--------------------------------------------------------------------------------------------------
typedef void (*le_atClient_UnsolicitedResponseHandlerFunc_t)
(
    const char* LE_NONNULL unsolicitedRsp,
        ///< The call reference.
    void* contextPtr
        ///<
);


//--------------------------------------------------------------------------------------------------
/**
 * This function must be called to start a ATClient session on a specified device.
 *
 * @return reference on a device context
 */
//--------------------------------------------------------------------------------------------------
le_atClient_DeviceRef_t le_atClient_Start
(
    int fd
        ///< [IN] File descriptor.
);



//--------------------------------------------------------------------------------------------------
/**
 * This function must be called to stop the ATClient session on the specified device.
 *
 * @return
 *      - LE_FAULT when function failed
 *      - LE_OK when function succeed
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_atClient_Stop
(
    le_atClient_DeviceRef_t device
        ///< [IN] Device reference
);



//--------------------------------------------------------------------------------------------------
/**
 * This function must be called to create a new AT command.
 *
 * @return pointer to the new AT Command reference
 */
//--------------------------------------------------------------------------------------------------
le_atClient_CmdRef_t le_atClient_Create
(
    void
);



//--------------------------------------------------------------------------------------------------
/**
 * This function must be called to delete an AT command reference.
 *
 * @return
 *      - LE_OK when function succeed
 *
 * @note If the AT Command reference is invalid, a fatal error occurs,
 *       the function won't return.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_atClient_Delete
(
    le_atClient_CmdRef_t cmdRef
        ///< [IN] AT Command
);



//--------------------------------------------------------------------------------------------------
/**
 * This function must be called to set the AT command string to be sent.
 *
 * @return
 *      - LE_OK when function succeed
 *
 * @note If the AT Command reference is invalid, a fatal error occurs,
 *       the function won't return.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_atClient_SetCommand
(
    le_atClient_CmdRef_t cmdRef,
        ///< [IN] AT Command
    const char* LE_NONNULL command
        ///< [IN] Set Command
);



//--------------------------------------------------------------------------------------------------
/**
 * This function must be called to set the waiting intermediate responses.
 * Several intermediate responses can be specified separated by a '|' character into the string
 * given in parameter.
 *
 * @return
 *      - LE_FAULT when function failed
 *      - LE_OK when function succeed
 *
 * @note If the AT Command reference or set intermediate response is invalid, a fatal error occurs,
 *       the function won't return.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_atClient_SetIntermediateResponse
(
    le_atClient_CmdRef_t cmdRef,
        ///< [IN] AT Command
    const char* LE_NONNULL intermediate
        ///< [IN] Set Intermediate
);



//--------------------------------------------------------------------------------------------------
/**
 * This function must be called to set the final response(s) of the AT command execution.
 * Several final responses can be specified separated by a '|' character into the string given in
 * parameter.
 *
 * @return
 *      - LE_FAULT when function failed
 *      - LE_OK when function succeed
 *
 * @note If the AT Command reference or set response is invalid, a fatal error occurs,
 *       the function won't return.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_atClient_SetFinalResponse
(
    le_atClient_CmdRef_t cmdRef,
        ///< [IN] AT Command
    const char* LE_NONNULL response
        ///< [IN] Set Response
);



//--------------------------------------------------------------------------------------------------
/**
 * This function must be called to set the text when the prompt is expected.
 *
 * @return
 *      - LE_FAULT when function failed
 *      - LE_OK when function succeed
 *
 * @note If the AT Command reference is invalid, a fatal error occurs,
 *       the function won't return.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_atClient_SetText
(
    le_atClient_CmdRef_t cmdRef,
        ///< [IN] AT Command
    const char* LE_NONNULL text
        ///< [IN] The AT Data to send
);



//--------------------------------------------------------------------------------------------------
/**
 * This function must be called to set the timeout of the AT command execution.
 *
 * @return
 *      - LE_OK when function succeed
 *
 * @note If the AT Command reference is invalid, a fatal error occurs,
 *       the function won't return.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_atClient_SetTimeout
(
    le_atClient_CmdRef_t cmdRef,
        ///< [IN] AT Command
    uint32_t timer
        ///< [IN] The timeout value in milliseconds.
);



//--------------------------------------------------------------------------------------------------
/**
 * This function must be called to set the device where the AT command will be sent.
 *
 * @return
 *      - LE_FAULT when function failed
 *      - LE_OK when function succeed
 *
 * @note If the AT Command reference is invalid, a fatal error occurs,
 *       the function won't return.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_atClient_SetDevice
(
    le_atClient_CmdRef_t cmdRef,
        ///< [IN] AT Command
    le_atClient_DeviceRef_t devRef
        ///< [IN] Device where the AT command has to be sent
);



//--------------------------------------------------------------------------------------------------
/**
 * This function must be called to send an AT Command and wait for response.
 *
 * @return
 *      - LE_FAULT when function failed
 *      - LE_TIMEOUT when a timeout occur
 *      - LE_OK when function succeed
 *
 * @note If the AT Command reference is invalid, a fatal error occurs,
 *       the function won't return.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_atClient_Send
(
    le_atClient_CmdRef_t cmdRef
        ///< [IN] AT Command
);



//--------------------------------------------------------------------------------------------------
/**
 * This function is used to get the first intermediate response.
 *
 * @return
 *      - LE_FAULT when function failed
 *      - LE_OK when function succeed
 *
 * @note If the AT Command reference is invalid, a fatal error occurs,
 *       the function won't return.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_atClient_GetFirstIntermediateResponse
(
    le_atClient_CmdRef_t cmdRef,
        ///< [IN] AT Command
    char* intermediateRsp,
        ///< [OUT] First intermediate result code
    size_t intermediateRspSize
        ///< [IN]
);



//--------------------------------------------------------------------------------------------------
/**
 * This function is used to get the next intermediate response.
 *
 * @return
 *      - LE_NOT_FOUND when there are no further results
 *      - LE_OK when function succeed
 *
 * @note If the AT Command reference is invalid, a fatal error occurs,
 *       the function won't return.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_atClient_GetNextIntermediateResponse
(
    le_atClient_CmdRef_t cmdRef,
        ///< [IN] AT Command
    char* intermediateRsp,
        ///< [OUT] Get Next intermediate result
        ///< code.
    size_t intermediateRspSize
        ///< [IN]
);



//--------------------------------------------------------------------------------------------------
/**
 * This function is used to get the final response
 *
 * @return
 *      - LE_FAULT when function failed
 *      - LE_OK when function succeed
 *
 * @note If the AT Command reference is invalid, a fatal error occurs,
 *       the function won't return.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_atClient_GetFinalResponse
(
    le_atClient_CmdRef_t cmdRef,
        ///< [IN] AT Command
    char* finalRsp,
        ///< [OUT] Get Final Line
    size_t finalRspSize
        ///< [IN]
);



//--------------------------------------------------------------------------------------------------
/**
 * This function must be called to automatically set and send an AT Command.
 *
 * @return
 *      - LE_FAULT when function failed
 *      - LE_TIMEOUT when a timeout occur
 *      - LE_OK when function succeed
 *
 * @note This command creates a command reference when called
 *
 * @note In case of an Error the command reference will be deleted and though
 *       not usable. Make sure to test the return code and not use the reference
 *       in other functions.
 *
 * @note If the AT command is invalid, a fatal error occurs,
 *        the function won't return.
 *
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_atClient_SetCommandAndSend
(
    le_atClient_CmdRef_t* cmdRefPtr,
        ///< [OUT] Cmd reference
    le_atClient_DeviceRef_t devRef,
        ///< [IN] Dev reference
    const char* LE_NONNULL command,
        ///< [IN] AT Command
    const char* LE_NONNULL interResp,
        ///< [IN] Expected intermediate response
    const char* LE_NONNULL finalResp,
        ///< [IN] Expected final response
    uint32_t timeout
        ///< [IN] Timeout value in milliseconds.
);



//--------------------------------------------------------------------------------------------------
/**
 * Add handler function for EVENT 'le_atClient_UnsolicitedResponse'
 *
 * This event provides information on a subscribed unsolicited response when this unsolicited
 * response is received.
 *
 */
//--------------------------------------------------------------------------------------------------
le_atClient_UnsolicitedResponseHandlerRef_t le_atClient_AddUnsolicitedResponseHandler
(
    const char* LE_NONNULL unsolRsp,
        ///< [IN] Pattern to match
    le_atClient_DeviceRef_t devRef,
        ///< [IN] Device to listen
    le_atClient_UnsolicitedResponseHandlerFunc_t handlerPtr,
        ///< [IN] unsolicited handler
    void* contextPtr,
        ///< [IN]
    uint32_t lineCount
        ///< [IN] Indicate the number of line of
        ///< the unsolicited
);



//--------------------------------------------------------------------------------------------------
/**
 * Remove handler function for EVENT 'le_atClient_UnsolicitedResponse'
 */
//--------------------------------------------------------------------------------------------------
void le_atClient_RemoveUnsolicitedResponseHandler
(
    le_atClient_UnsolicitedResponseHandlerRef_t handlerRef
        ///< [IN]
);


#endif // LE_ATCLIENT_INTERFACE_H_INCLUDE_GUARD