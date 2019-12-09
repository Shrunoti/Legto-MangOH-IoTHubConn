

/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */


#ifndef LE_ATDEFS_INTERFACE_H_INCLUDE_GUARD
#define LE_ATDEFS_INTERFACE_H_INCLUDE_GUARD


#include "legato.h"





//--------------------------------------------------------------------------------------------------
/**
 * Command string maximum length.
 *
 * @warning Special characters cost more than one byte
 *
 */
//--------------------------------------------------------------------------------------------------
#define LE_ATDEFS_COMMAND_MAX_LEN 511

//--------------------------------------------------------------------------------------------------
/**
 * Command string maximum length.
 * One extra byte is added for the null character.
 */
//--------------------------------------------------------------------------------------------------
#define LE_ATDEFS_COMMAND_MAX_BYTES 512

//--------------------------------------------------------------------------------------------------
/**
 * Parameter string maximum length.
 *
 * @warning Special characters cost more than one byte
 *
 */
//--------------------------------------------------------------------------------------------------
#define LE_ATDEFS_PARAMETER_MAX_LEN 127

//--------------------------------------------------------------------------------------------------
/**
 * Parameter string maximum length.
 * One extra byte is added for the null character.
 */
//--------------------------------------------------------------------------------------------------
#define LE_ATDEFS_PARAMETER_MAX_BYTES 128

//--------------------------------------------------------------------------------------------------
/**
 * AT command response maximum length.
 *
 * @warning Special characters cost more than one byte
 *
 */
//--------------------------------------------------------------------------------------------------
#define LE_ATDEFS_RESPONSE_MAX_LEN 512

//--------------------------------------------------------------------------------------------------
/**
 * AT command response maximum length.
 * One extra byte is added for the null character.
 */
//--------------------------------------------------------------------------------------------------
#define LE_ATDEFS_RESPONSE_MAX_BYTES 513

//--------------------------------------------------------------------------------------------------
/**
 * Maximum number of bytes in an unsolicited response (not including the null-terminator).
 *
 * @warning Special characters cost more than one byte
 *
 */
//--------------------------------------------------------------------------------------------------
#define LE_ATDEFS_UNSOLICITED_MAX_LEN 256

//--------------------------------------------------------------------------------------------------
/**
 * Maximum number of bytes in an unsolicited response (including the null-terminator).
 */
//--------------------------------------------------------------------------------------------------
#define LE_ATDEFS_UNSOLICITED_MAX_BYTES 257

//--------------------------------------------------------------------------------------------------
/**
 * Maximum number of bytes in a text message (not including the null-terminator).
 *
 * @warning Special characters cost more than one byte
 *
 */
//--------------------------------------------------------------------------------------------------
#define LE_ATDEFS_TEXT_MAX_LEN 4095

//--------------------------------------------------------------------------------------------------
/**
 * Maximum number of bytes in an text message (including the null-terminator).
 */
//--------------------------------------------------------------------------------------------------
#define LE_ATDEFS_TEXT_MAX_BYTES 4096

//--------------------------------------------------------------------------------------------------
/**
 * Command default timeout (in ms)
 */
//--------------------------------------------------------------------------------------------------
#define LE_ATDEFS_COMMAND_DEFAULT_TIMEOUT 30000

//--------------------------------------------------------------------------------------------------
/**
 *  CME error pattern
 */
//--------------------------------------------------------------------------------------------------
#define LE_ATDEFS_CME_ERROR "+CME ERROR: "

//--------------------------------------------------------------------------------------------------
/**
 *  CMS error pattern
 */
//--------------------------------------------------------------------------------------------------
#define LE_ATDEFS_CMS_ERROR "+CMS ERROR: "

#endif // LE_ATDEFS_INTERFACE_H_INCLUDE_GUARD