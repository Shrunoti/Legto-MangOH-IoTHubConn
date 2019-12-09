

/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */

/**
 * @page c_secStore Secure Storage
 *
 * @ref le_secStore_interface.h "API Reference" <br>
 * @ref c_secStoreAdmin API <br>
 * @ref platformConstraintsSecStorage Constraints
 *
 * <HR>
 *
 * This API for accessing secure storage.
 *
 * Secure storage can be used to store sensitive information like passwords, keys, certificates,
 * etc. All data in the secure storage is in an encrypted format. Each app using this API only has
 * access to its own secure storage data.
 *
 * App's items in secure storage have a name and a value. The item name is used to access the item's
 * value and can be maximum 255 characters.  The item name can contain path separators, '/', to
 * help organize an app's data.  Item names cannot contain a trailing separator.
 *
 * To create or update an item, use le_secStore_Write() to specify the item's name and value. If the
 * item doesn't exist, it'll be created. Each item can be a maximum of 8192 bytes.  If it's larger,
 * le_secStore_Write() will fail.
 *
 * Additionally, an app's total secure storage usage is limited by the maxSecureStorageBytes setting
 * that may be specified in the xdef files.  The maxSecureStorageBytes default is 8192 bytes.
 *
 * Writing to secure storage may also fail if the system-wide secure storage is out of memory.  The
 * system-wide secure storage memory amount is platform dependent
 * (see @ref platformConstraintsSecStorage).
 *
 * To read an item, use le_secStore_Read(), and specify the item's name. To delete an item, use
 * le_secStore_Delete().
 *
 * All the functions in this API are provided by the @b secStore service.
 *
 * Here's a code sample binding to this service:
 * @verbatim
   bindings:
   {
      clientExe.clientComponent.le_secStore -> secStore.le_secStore
   }
   @endverbatim
 *
 * Whenever the secure storage is modified, a timer of 300 seconds is started. When the timer
 * expires, a backup is performed to capture all changes since the previous backup. If the secure
 * storage is not modified, then the backup is not performed.
 * If corruption in the secure storage is detected, a restore is performed and the target device is
 * rebooted.
 *
 * @section c_secStoreGlobal Global Secure Storage
 *
 * This same API also provides access to a global area that can be shared across the system.
 * This interface is called @c secStoreGlobal.
 *
 * Here's a code sample binding to this service:
 * @verbatim
   bindings:
   {
      clientExe.clientComponent.secStoreGlobal -> secStore.secStoreGlobal
   }
   @endverbatim
 *
 * And the following code should be used to use the API from your .cdef file:
 * @verbatim
   requires:
   {
     api:

     {
       secStoreGlobal = le_secStore.api
    }
   }
   @endverbatim
 *
 * <HR>
 *
 * Copyright (C) Sierra Wireless Inc.
 */
/**
 * @file le_secStore_interface.h
 *
 * Legato @ref c_secStore API
 *
 * Copyright (C) Sierra Wireless Inc.
 */

#ifndef LE_SECSTORE_INTERFACE_H_INCLUDE_GUARD
#define LE_SECSTORE_INTERFACE_H_INCLUDE_GUARD


#include "legato.h"


//--------------------------------------------------------------------------------------------------
/**
 * Type for handler called when a server disconnects.
 */
//--------------------------------------------------------------------------------------------------
typedef void (*le_secStore_DisconnectHandler_t)(void *);

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
void le_secStore_ConnectService
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
le_result_t le_secStore_TryConnectService
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
void le_secStore_SetServerDisconnectHandler
(
    le_secStore_DisconnectHandler_t disconnectHandler,
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
void le_secStore_DisconnectService
(
    void
);


//--------------------------------------------------------------------------------------------------
/**
 * Maximum number of characters and byte storage size permitted for a secure storage item name.
 */
//--------------------------------------------------------------------------------------------------
#define LE_SECSTORE_MAX_NAME_SIZE 255

//--------------------------------------------------------------------------------------------------
/**
 */
//--------------------------------------------------------------------------------------------------
#define LE_SECSTORE_MAX_NAME_BYTES 256

//--------------------------------------------------------------------------------------------------
/**
 * Maximum number of bytes for each item in secure storage.
 */
//--------------------------------------------------------------------------------------------------
#define LE_SECSTORE_MAX_ITEM_SIZE 8192

//--------------------------------------------------------------------------------------------------
/**
 * Writes an item to secure storage. If the item already exists, it'll be overwritten with
 * the new value. If the item doesn't already exist, it'll be created.
 * If the item name is not valid or the buffer is NULL, this function will kill the calling client.
 *
 * @return
 *      LE_OK if successful.
 *      LE_NO_MEMORY if there isn't enough memory to store the item.
 *      LE_UNAVAILABLE if the secure storage is currently unavailable.
 *      LE_FAULT if there was some other error.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_secStore_Write
(
    const char* LE_NONNULL name,
        ///< [IN] Name of the secure storage item.
    const uint8_t* bufPtr,
        ///< [IN] Buffer containing the data to store.
    size_t bufSize
        ///< [IN]
);

//--------------------------------------------------------------------------------------------------
/**
 * Reads an item from secure storage.
 * If the item name is not valid or the buffer is NULL, this function will kill the calling client.
 *
 * @return
 *      LE_OK if successful.
 *      LE_OVERFLOW if the buffer is too small to hold the entire item. No data will be written to
 *                  the buffer in this case.
 *      LE_NOT_FOUND if the item doesn't exist.
 *      LE_UNAVAILABLE if the secure storage is currently unavailable.
 *      LE_FAULT if there was some other error.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_secStore_Read
(
    const char* LE_NONNULL name,
        ///< [IN] Name of the secure storage item.
    uint8_t* bufPtr,
        ///< [OUT] Buffer to store the data in.
    size_t* bufSizePtr
        ///< [INOUT]
);

//--------------------------------------------------------------------------------------------------
/**
 * Deletes an item from secure storage.
 * If the item name is not valid, this function will kill the calling client.
 *
 * @return
 *      LE_OK if successful.
 *      LE_NOT_FOUND if the item doesn't exist.
 *      LE_UNAVAILABLE if the secure storage is currently unavailable.
 *      LE_FAULT if there was some other error.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_secStore_Delete
(
    const char* LE_NONNULL name
        ///< [IN] Name of the secure storage item.
);

#endif // LE_SECSTORE_INTERFACE_H_INCLUDE_GUARD