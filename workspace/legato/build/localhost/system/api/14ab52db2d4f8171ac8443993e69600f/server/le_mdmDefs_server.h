

/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */


#ifndef LE_MDMDEFS_INTERFACE_H_INCLUDE_GUARD
#define LE_MDMDEFS_INTERFACE_H_INCLUDE_GUARD


#include "legato.h"





//--------------------------------------------------------------------------------------------------
/**
 * Cf. ITU-T recommendations E.164/E.163. E.164 numbers can have a maximum of 15 digits except the
 * international prefix ('+' or '00'). One extra byte is added for the null character.
 */
//--------------------------------------------------------------------------------------------------
#define LE_MDMDEFS_PHONE_NUM_MAX_LEN 17

//--------------------------------------------------------------------------------------------------
/**
 * Cf. ITU-T recommendations E.164/E.163. E.164 numbers can have a maximum of 15 digits except the
 * international prefix ('+' or '00'). One extra byte is added for the null character.
 * One extra byte is added for the null character.
 */
//--------------------------------------------------------------------------------------------------
#define LE_MDMDEFS_PHONE_NUM_MAX_BYTES 18

//--------------------------------------------------------------------------------------------------
/**
 **  IP Version
 */
//--------------------------------------------------------------------------------------------------
typedef enum
{
    LE_MDMDEFS_IPV4 = 0,
        ///< IPv4 technology
    LE_MDMDEFS_IPV6 = 1,
        ///< IPv6 technology
    LE_MDMDEFS_IPMAX = 2
        ///<
}
le_mdmDefs_IpVersion_t;


#endif // LE_MDMDEFS_INTERFACE_H_INCLUDE_GUARD