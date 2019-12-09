

/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */


#ifndef SECSTOREADMIN_INTERFACE_H_INCLUDE_GUARD
#define SECSTOREADMIN_INTERFACE_H_INCLUDE_GUARD


#include "legato.h"

// Interface specific includes
#include "le_secStore_server.h"


//--------------------------------------------------------------------------------------------------
/**
 * Get the server service reference
 */
//--------------------------------------------------------------------------------------------------
le_msg_ServiceRef_t secStoreAdmin_GetServiceRef
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * Get the client session reference for the current message
 */
//--------------------------------------------------------------------------------------------------
le_msg_SessionRef_t secStoreAdmin_GetClientSessionRef
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * Initialize the server and advertise the service.
 */
//--------------------------------------------------------------------------------------------------
void secStoreAdmin_AdvertiseService
(
    void
);


//--------------------------------------------------------------------------------------------------
/**
 * Maximum number of characters and byte storage size permitted for a path.
 */
//--------------------------------------------------------------------------------------------------
#define SECSTOREADMIN_MAX_PATH_SIZE 511

//--------------------------------------------------------------------------------------------------
/**
 */
//--------------------------------------------------------------------------------------------------
#define SECSTOREADMIN_MAX_PATH_BYTES 512

//--------------------------------------------------------------------------------------------------
/**
 * Iterator to list entries in secure storage.
 */
//--------------------------------------------------------------------------------------------------
typedef struct secStoreAdmin_Iter* secStoreAdmin_IterRef_t;



//--------------------------------------------------------------------------------------------------
/**
 * Create an iterator for listing entries in secure storage under the specified path.
 *
 * @return
 *      An iterator reference if successful.
 *      NULL if the secure storage is currently unavailable.
 */
//--------------------------------------------------------------------------------------------------
secStoreAdmin_IterRef_t secStoreAdmin_CreateIter
(
    const char* LE_NONNULL path
        ///< [IN] Path to iterate over.
);



//--------------------------------------------------------------------------------------------------
/**
 * Deletes an iterator.
 */
//--------------------------------------------------------------------------------------------------
void secStoreAdmin_DeleteIter
(
    secStoreAdmin_IterRef_t iterRef
        ///< [IN] Iterator reference.
);



//--------------------------------------------------------------------------------------------------
/**
 * Go to the next entry in the iterator.  This should be called at least once before accessing the
 * entry.  After the first time this function is called successfully on an iterator the first entry
 * will be available.
 *
 * @return
 *      LE_OK if successful.
 *      LE_NOT_FOUND if there are no more entries available.
 */
//--------------------------------------------------------------------------------------------------
le_result_t secStoreAdmin_Next
(
    secStoreAdmin_IterRef_t iterRef
        ///< [IN] Iterator reference.
);



//--------------------------------------------------------------------------------------------------
/**
 * Get the current entry's name.
 *
 * @return
 *      LE_OK if successful.
 *      LE_OVERFLOW if the buffer is too small to hold the entry name.
 *      LE_UNAVAILABLE if the secure storage is currently unavailable.
 */
//--------------------------------------------------------------------------------------------------
le_result_t secStoreAdmin_GetEntry
(
    secStoreAdmin_IterRef_t iterRef,
        ///< [IN] Iterator reference.
    char* name,
        ///< [OUT] Buffer to store the entry name.
    size_t nameSize,
        ///< [IN]
    bool* isDirPtr
        ///< [OUT] True if the entry is a directory, false otherwise.
);



//--------------------------------------------------------------------------------------------------
/**
 * Writes a buffer of data into the specified path in secure storage.  If the item already exists,
 * it'll be overwritten with the new value. If the item doesn't already exist, it'll be created.
 *
 * @note
 *      The specified path must be an absolute path.
 *
 * @return
 *      LE_OK if successful.
 *      LE_NO_MEMORY if there isn't enough memory to store the item.
 *      LE_UNAVAILABLE if the secure storage is currently unavailable.
 *      LE_FAULT if there was some other error.
 */
//--------------------------------------------------------------------------------------------------
le_result_t secStoreAdmin_Write
(
    const char* LE_NONNULL path,
        ///< [IN] Path of the secure storage item.
    const uint8_t* bufPtr,
        ///< [IN] Buffer containing the data to store.
    size_t bufSize
        ///< [IN]
);



//--------------------------------------------------------------------------------------------------
/**
 * Reads an item from secure storage.
 *
 * @note
 *      The specified path must be an absolute path.
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
le_result_t secStoreAdmin_Read
(
    const char* LE_NONNULL path,
        ///< [IN] Path of the secure storage item.
    uint8_t* bufPtr,
        ///< [OUT] Buffer to store the data in.
    size_t* bufSizePtr
        ///< [INOUT]
);



//--------------------------------------------------------------------------------------------------
/**
 * Copy the meta file to the specified path.
 *
 * @return
 *      LE_OK if successful.
 *      LE_NOT_FOUND if the meta file does not exist.
 *      LE_UNAVAILABLE if the sfs is currently unavailable.
 *      LE_FAULT if there was some other error.
 */
//--------------------------------------------------------------------------------------------------
le_result_t secStoreAdmin_CopyMetaTo
(
    const char* LE_NONNULL path
        ///< [IN] Destination path of meta file copy.
);



//--------------------------------------------------------------------------------------------------
/**
 * Recursively deletes all items under the specified path and the specified path from secure
 * storage.
 *
 * @note
 *      The specified path must be an absolute path.
 *
 * @return
 *      LE_OK if successful.
 *      LE_NOT_FOUND if the path doesn't exist.
 *      LE_UNAVAILABLE if the secure storage is currently unavailable.
 *      LE_FAULT if there was some other error.
 */
//--------------------------------------------------------------------------------------------------
le_result_t secStoreAdmin_Delete
(
    const char* LE_NONNULL path
        ///< [IN] Path of the secure storage item.
);



//--------------------------------------------------------------------------------------------------
/**
 * Gets the size, in bytes, of all items under the specified path.
 *
 * @note
 *      The specified path must be an absolute path.
 *
 * @return
 *      LE_OK if successful.
 *      LE_NOT_FOUND if the path doesn't exist.
 *      LE_UNAVAILABLE if the secure storage is currently unavailable.
 *      LE_FAULT if there was some other error.
 */
//--------------------------------------------------------------------------------------------------
le_result_t secStoreAdmin_GetSize
(
    const char* LE_NONNULL path,
        ///< [IN] Path of the secure storage item.
    uint64_t* sizePtr
        ///< [OUT] Size in bytes of all items in the path.
);



//--------------------------------------------------------------------------------------------------
/**
 * Gets the total space and the available free space in secure storage.
 *
 * @return
 *      LE_OK if successful.
 *      LE_UNAVAILABLE if the secure storage is currently unavailable.
 *      LE_FAULT if there was some other error.
 */
//--------------------------------------------------------------------------------------------------
le_result_t secStoreAdmin_GetTotalSpace
(
    uint64_t* totalSizePtr,
        ///< [OUT] Total size, in bytes, of secure storage.
    uint64_t* freeSizePtr
        ///< [OUT] Free space, in bytes, in secure storage.
);


#endif // SECSTOREADMIN_INTERFACE_H_INCLUDE_GUARD