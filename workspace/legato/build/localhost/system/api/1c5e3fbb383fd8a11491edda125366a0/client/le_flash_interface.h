

/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */

/**
 * @page c_flash Flash API
 *
 * @ref le_flash_interface.h "API Reference"
 *
 * <HR>
 *
 * This file contains data structures and prototypes definitions for high level Flash APIs.
 *
 * The goal is to provide an API to update an image recorded:
 * - in a flash partition in RAW mode at the block layer, using a Memory Technology Device (MTD).
 * - in an Unsorted Block Images (UBI) volume, using a MTD and updating UBI information and headers.
 *
 *
 * @section le_flash_binding IPC interfaces binding
 *
 * All the functions of this API are provided by the @b fwupdateService.
 *
 * Here's a code sample binding to the flash service:
 * @verbatim
   bindings:
   {
      clientExe.clientComponent.le_flash -> fwupdateService.le_flash
   }
   @endverbatim
 *
 * @warning These APIs are not available for all platforms. Please refer to the Product Technical
 * Specification document of your platform for further details.
 * Please refer to @subpage platformConstraintsFlash for details.
 *
 * @section le_flash_BadImageDetection Bad image detection
 *
 * This functionality allows the user to be notified when an image becomes bad.
 *
 * - le_flash_AddBadImageDetectionHandler() API adds a handler to notify when an image becomes bad
 * - le_flash_RemoveBadImageDetectionHandler() API removes the bad image handler
 *
 * @section flash_Protection Flash access protection
 * To address race conditions, le_flash_RequestAccess() has to be called first before trying to
 * access to the flash. When all flash modifications are done, le_flash_ReleaseAccess() is required.
 *
 * @section le_flash_Open Open a partition
 * Depending on how the read/write operations have to be done, the correct open API has to be used:
 * - le_flash_OpenMtd() when the flash image is reachable at the block layer.
 * - le_flash_OpenUbi() when the image is reachable through a UBI. Then le_flash_OpenUbiVolume()
 * needs to be called to set the volume to be accessed. Volume can be set several time
 * successively. le_flash_CloseUbiVolume() has to be called before changing the volume.
 * Please refer to @subpage platformConstraintsFlash for details.
 *
 * A sample code showing how to open, retrieve information and close can be seen below:
 * @snippet "apps/test/fwupdate/fwupdateIntegrationTest/flashApiTest/main.c" Info
 *
 * A sample code showing how to open UBI, retrieve UBI information and close UBI can be seen below:
 * @snippet "apps/test/fwupdate/fwupdateIntegrationTest/flashApiTest/main.c" UbiInfo
 *
 * @section le_flash_Close Close a partition
 * When the read/write operation are over, the application has to call le_flash_Close() API to
 * release resources and make the partition usable.
 *
 * @section le_flash_EraseBlock Erase a block inside a partition
 * When a block needs to be erased; the application has to call le_flash_EraseBlock() API to perform
 * the erase operation on the given block index. If the erase fails, the block is marked "bad".
 *
 * @section le_flash_Read Read a data chunk
 * To read data, le_flash_Read() has to be called. The data are read at the specified block index.
 * Note that this index is not the index of the block into the partition, but a logical block index:
 * - the bad blocks are skipped.
 * - the blocks are not into a consecutive order for UBI volume.
 *
 * The data length to be read can't be higher than:
 *      - an erase block size for MTD usage partition
 *      - an erase block size minus 2 pages for UBI partitions (UBI uses 2 pages for its internal
 * need)
 *
 * A sample code showing how to read a whole partition can be seen below:
 * @snippet "apps/test/fwupdate/fwupdateIntegrationTest/flashApiTest/main.c" Dump
 *
 * A sample code showing how to read a whole UBI volume inside an UBI partition can be seen below:
 * @snippet "apps/test/fwupdate/fwupdateIntegrationTest/flashApiTest/main.c" UbiDump
 *
 * @section le_flash_Write Write a data chunk
 * To write some data, le_flash_Write() can be used. The data are written at a specified block
 * index. This index is a logical block index (see @ref le_flash_Read).
 * The block is firstly erased, so no call to le_flash_EraseBlock() is needed.
 * If the erase or the write reports an error, the block is marked "bad" and the write starts
 * again at the next physical block.
 * The data length to be written can't be higher than:
 *      - an erase block size for MTD usage partition
 *      - an erase block size minus 2 pages for UBI partitions (UBI uses 2 pages for its internal
 * need)
 *
 * A sample code showing how to write a whole partition can be seen below:
 * @snippet "apps/test/fwupdate/fwupdateIntegrationTest/flashApiTest/main.c" FlashErase
 *
 * A sample code showing how to write a whole UBI volume inside an UBI partition can be seen below:
 * @snippet "apps/test/fwupdate/fwupdateIntegrationTest/flashApiTest/main.c" UbiFlash
 *
 * @section le_flash_GetBlockInformation Retrieve information about blocks and pages for a
 * partition.
 * To get information about blocks and pages, call le_flash_GetBlockInformation(). The API
 * will return:
 *      - The number of bad blocks belonging to this partition
 *      - The number of erase blocks belonging to this partition
 *      - The erase block size
 *      - The page size
 * The whole partition size in bytes can be computed by:
 *      number of erase blocks * erase block size
 * The whole partition available size for read and write purpose can be computed by:
 *      (number of erase blocks - number of bad blocks) * erase block size
 *
 * A sample code showing how to open, retrieve information and close a partition can be seen below:
 * @snippet "apps/test/fwupdate/fwupdateIntegrationTest/flashApiTest/main.c" Info
 *
 * @section le_flash_GetUbiVolumeInformation Retrieve UBI information.
 * To get information about an UBI volume, call le_flash_GetUbiVolumeInformation(). The API
 * will return:
 *      - The number of free blocks in the whole UBI partition
 *      - The currently number of blocks allocated to the UBI volume
 *      - The real size of the UBI volume
 * This API must be called after the UBI volume has been open by le_flash_OpenUbiVolume().
 *
 * A sample code showing how to open UBI, retrieve UBI information and close UBI can be seen below:
 * @snippet "apps/test/fwupdate/fwupdateIntegrationTest/flashApiTest/main.c" UbiInfo
 *
 * @section le_flash_CreateUbi Create an UBI partition and volume
 * le_flash_CreateUbi() is used to create an UBI partition. If the partition is already an UBI,
 * an error LE_DUPLICATE will be reported. The overwrite of the partition can be forced by setting
 * the flag isForcedCreate to true (all previous content will be lost).
 *
 * When the creation succeeded, the UBI device is opened using le_flash_OpenUbi() with
 * LE_FLASH_READWRITE mode.
 *
 * To create a new volume in the created UBI partition, le_flash_CreateUbiVolume() has to be
 * called. If a volume already exists with the same name or same ID, an error LE_DUPLICATE will
 * be reported. The flag isForcedCreate permits to re-create the volume, but the previous content
 * of the volume will be lost.
 *
 * The created volume can be opened using the le_flash_OpenUbiVolume() API.
 *
 * An existing volume can be deleted thanks to le_flash_DeleteUbiVolume().
 *
 * Please refer to @subpage platformConstraintsFlash for details.
 *
 * A sample code showing how to create an UBI partition can be seen below:
 * @snippet "apps/test/fwupdate/fwupdateIntegrationTest/flashApiTest/main.c" UbiCreate
 *
 * A sample code showing how to create an UBI volume inside an UBI partition can be seen below:
 * @snippet "apps/test/fwupdate/fwupdateIntegrationTest/flashApiTest/main.c" UbiCreateVol
 *
 * A sample code showing how to delete an UBI volume from an UBI partition can be seen below:
 * @snippet "apps/test/fwupdate/fwupdateIntegrationTest/flashApiTest/main.c" UbiDeleteVol
 *
 * This API is synchronous: it is blocking until the write is over. After the end of the write, the
 * data are read to ensure the data has been saved correctly.
 *
 * <HR>
 *
 * Copyright (C) Sierra Wireless Inc.
 */
/**
 * @file le_flash_interface.h
 *
 * Legato @ref c_flash include file.
 *
 * Copyright (C) Sierra Wireless Inc.
 */

#ifndef LE_FLASH_INTERFACE_H_INCLUDE_GUARD
#define LE_FLASH_INTERFACE_H_INCLUDE_GUARD


#include "legato.h"


//--------------------------------------------------------------------------------------------------
/**
 * Type for handler called when a server disconnects.
 */
//--------------------------------------------------------------------------------------------------
typedef void (*le_flash_DisconnectHandler_t)(void *);

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
void le_flash_ConnectService
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
le_result_t le_flash_TryConnectService
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
void le_flash_SetServerDisconnectHandler
(
    le_flash_DisconnectHandler_t disconnectHandler,
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
void le_flash_DisconnectService
(
    void
);


//--------------------------------------------------------------------------------------------------
/**
 * Image length maximum length.
 *
 */
//--------------------------------------------------------------------------------------------------
#define LE_FLASH_IMAGE_NAME_MAX_LEN 32

//--------------------------------------------------------------------------------------------------
/**
 * Image length maximum length.
 * One extra byte is added for the null character.
 */
//--------------------------------------------------------------------------------------------------
#define LE_FLASH_IMAGE_NAME_MAX_BYTES 33

//--------------------------------------------------------------------------------------------------
/**
 * Maximum partition name length
 */
//--------------------------------------------------------------------------------------------------
#define LE_FLASH_PARTITION_NAME_MAX_LEN 50

//--------------------------------------------------------------------------------------------------
/**
 * Maximum partition name length
 * One extra byte is added for the null character.
 */
//--------------------------------------------------------------------------------------------------
#define LE_FLASH_PARTITION_NAME_MAX_BYTES 51

//--------------------------------------------------------------------------------------------------
/**
 * Maximum volume name length
 */
//--------------------------------------------------------------------------------------------------
#define LE_FLASH_VOLUME_NAME_MAX_LEN 50

//--------------------------------------------------------------------------------------------------
/**
 * Maximum volume name length
 * One extra byte is added for the null character.
 */
//--------------------------------------------------------------------------------------------------
#define LE_FLASH_VOLUME_NAME_MAX_BYTES 51

//--------------------------------------------------------------------------------------------------
/**
 * Max byte storage size for write buffer
 */
//--------------------------------------------------------------------------------------------------
#define LE_FLASH_MAX_WRITE_SIZE 262144

//--------------------------------------------------------------------------------------------------
/**
 * Max byte storage size for read buffer
 */
//--------------------------------------------------------------------------------------------------
#define LE_FLASH_MAX_READ_SIZE 262144

//--------------------------------------------------------------------------------------------------
/**
 * Default volume size to keep the current volume size untouched
 */
//--------------------------------------------------------------------------------------------------
#define LE_FLASH_UBI_VOL_NO_SIZE -1

//--------------------------------------------------------------------------------------------------
/**
 * Default volume ID to specify the automatic allocation of volume ID
 */
//--------------------------------------------------------------------------------------------------
#define LE_FLASH_UBI_VOL_NO_ID -1

//--------------------------------------------------------------------------------------------------
/**
 * Maximum volume ID
 */
//--------------------------------------------------------------------------------------------------
#define LE_FLASH_UBI_VOL_ID_MAX 127

//--------------------------------------------------------------------------------------------------
/**
 * Open mode.
 */
//--------------------------------------------------------------------------------------------------
typedef enum
{
    LE_FLASH_READ_ONLY = 0,
        ///<
    LE_FLASH_WRITE_ONLY = 1,
        ///<
    LE_FLASH_READ_WRITE = 2
        ///<
}
le_flash_OpenMode_t;


//--------------------------------------------------------------------------------------------------
/**
 * UBI volume type mode.
 */
//--------------------------------------------------------------------------------------------------
typedef enum
{
    LE_FLASH_DYNAMIC = 0,
        ///<
    LE_FLASH_STATIC = 1
        ///<
}
le_flash_UbiVolumeType_t;


//--------------------------------------------------------------------------------------------------
/**
 * Declare a reference type for referring a partition.
 */
//--------------------------------------------------------------------------------------------------
typedef struct le_flash_Partition* le_flash_PartitionRef_t;


//--------------------------------------------------------------------------------------------------
/**
 * Reference type used by Add/Remove functions for EVENT 'le_flash_BadImageDetection'
 */
//--------------------------------------------------------------------------------------------------
typedef struct le_flash_BadImageDetectionHandler* le_flash_BadImageDetectionHandlerRef_t;


//--------------------------------------------------------------------------------------------------
/**
 * Handler for bad image detection.
 *
 * @note The image names are platform dependent.
 */
//--------------------------------------------------------------------------------------------------
typedef void (*le_flash_BadImageDetectionHandlerFunc_t)
(
    const char* LE_NONNULL imageName,
        ///< bad image name
    void* contextPtr
        ///<
);

//--------------------------------------------------------------------------------------------------
/**
 * Add handler function for EVENT 'le_flash_BadImageDetection'
 *
 * This event provides information on bad image status.
 *
 */
//--------------------------------------------------------------------------------------------------
le_flash_BadImageDetectionHandlerRef_t le_flash_AddBadImageDetectionHandler
(
    le_flash_BadImageDetectionHandlerFunc_t handlerPtr,
        ///< [IN]
    void* contextPtr
        ///< [IN]
);

//--------------------------------------------------------------------------------------------------
/**
 * Remove handler function for EVENT 'le_flash_BadImageDetection'
 */
//--------------------------------------------------------------------------------------------------
void le_flash_RemoveBadImageDetectionHandler
(
    le_flash_BadImageDetectionHandlerRef_t handlerRef
        ///< [IN]
);

//--------------------------------------------------------------------------------------------------
/**
 * Request the flash access authorization. This is required to avoid race operations.
 *
 * @return
 *      - LE_OK            On success
 *      - LE_UNAVAILABLE   The flash access is temporarily unavailable
 *      - LE_DUPLICATE     If the a request access for the client was already performed
 *      - LE_FAULT         On failure
 *
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_flash_RequestAccess
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * Release the flash access requested by le_flash_RequestAccess API.
 *
 * @return
 *      - LE_OK            On success
 *      - LE_FAULT         On failure
 *
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_flash_ReleaseAccess
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * Open a flash partition at the block layer for the given operation and return a descriptor.
 * The read and write operation will be done using MTD.
 *
 * @return
 *      - LE_OK            On success
 *      - LE_BAD_PARAMETER If a parameter is invalid
 *      - LE_NOT_FOUND     If the flash partition is not found
 *      - LE_FAULT         On failure
 *
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_flash_OpenMtd
(
    const char* LE_NONNULL partitionName,
        ///< [IN] Partition to be opened.
    le_flash_OpenMode_t mode,
        ///< [IN] Opening mode.
    le_flash_PartitionRef_t* partitionRefPtr
        ///< [OUT] Partition reference.
);

//--------------------------------------------------------------------------------------------------
/**
 * Open a UBI volume for the given operation and return a descriptor. The read and write
 * operation will be done using MTD, UBI metadata will be updated.
 *
 * @return
 *      - LE_OK            On success
 *      - LE_BAD_PARAMETER If a parameter is invalid
 *      - LE_NOT_FOUND     If the flash partition is not found
 *      - LE_FAULT         On failure
 *
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_flash_OpenUbi
(
    const char* LE_NONNULL partitionName,
        ///< [IN] Partition to be opened.
    le_flash_OpenMode_t mode,
        ///< [IN] Opening mode.
    le_flash_PartitionRef_t* partitionRefPtr
        ///< [OUT] Partition reference.
);

//--------------------------------------------------------------------------------------------------
/**
 * Open the UBI volume of an UBI image to be used for the read and write operations. When open for
 * writing and a volumeSize is given, the UBI volume will be adjusted to this size by freeing the
 * PEBs over this size. If the data inside the volume require more PEBs, they will be added
 * by the le_flash_Write() API.
 *
 * @return
 *      - LE_OK            On success
 *      - LE_BAD_PARAMETER If a parameter is invalid
 *      - LE_NOT_FOUND     If the volume name is not found
 *      - LE_FAULT         On failure
 *
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_flash_OpenUbiVolume
(
    le_flash_PartitionRef_t partitionRef,
        ///< [IN] Partition reference.
    const char* LE_NONNULL volumeName,
        ///< [IN] Volume name to be used.
    int32_t volumeSize
        ///< [IN] Volume size to set if open for writing
);

//--------------------------------------------------------------------------------------------------
/**
 * Close the used UBI volume of an UBI image.
 *
 * @return
 *      - LE_OK            On success
 *      - LE_BAD_PARAMETER If a parameter is invalid
 *      - LE_FAULT         On failure
 *
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_flash_CloseUbiVolume
(
    le_flash_PartitionRef_t partitionRef
        ///< [IN] Partition reference.
);

//--------------------------------------------------------------------------------------------------
/**
 * Close a flash partition
 *
 * @return
 *      - LE_OK            On success
 *      - LE_BAD_PARAMETER If a parameter is invalid
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_flash_Close
(
    le_flash_PartitionRef_t partitionRef
        ///< [IN] Partition reference to be closed.
);

//--------------------------------------------------------------------------------------------------
/**
 * Erase a block inside a flash partition. If the erase fails, the block is marked bad.
 *
 * @return
 *      - LE_OK            On success
 *      - LE_BAD_PARAMETER If a parameter is invalid
 *      - LE_FAULT         On other error
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_flash_EraseBlock
(
    le_flash_PartitionRef_t partitionRef,
        ///< [IN] Partition reference to be closed.
    uint32_t blockIndex
        ///< [IN] Logical block index to be erased.
);

//--------------------------------------------------------------------------------------------------
/**
 * Read data from a flash partition. The read data are:
 * - at the logical block index given by blockIndex.
 * - the maximum read data length is:
 *      - an erase block size for MTD usage partition
 *      - an erase block size minus 2 pages for UBI partitions
 *
 * @return
 *      - LE_OK            On success
 *      - LE_BAD_PARAMETER If a parameter is invalid
 *      - LE_FAULT         On other error
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_flash_Read
(
    le_flash_PartitionRef_t partitionRef,
        ///< [IN] Partition reference to be used.
    uint32_t blockIndex,
        ///< [IN] Logical block index to be read.
    uint8_t* readDataPtr,
        ///< [OUT] Data buffer to copy the read data.
    size_t* readDataSizePtr
        ///< [INOUT]
);

//--------------------------------------------------------------------------------------------------
/**
 * Write data to a flash partition.
 * - the block is firstly erased, so no call to le_flash_EraseBlock() is needed.
 * - if the erase or the write reports an error, the block is marked "bad" and the write starts
 *   again at the next physical block.
 * - the data are programmed at the logical block index given by blockIndex.
 * - the maximum written data length is:
 *      - an erase block size for MTD usage partition. This is the eraseBlockSize returned by
 *        le_flash_GetInformation().
 *      - an erase block size minus 2 pages for UBI partitions. These are the eraseBlockSize and
 *        pageSize returned by le_flash_GetInformation().
 * If the write addresses an UBI volume and more PEBs are required to write the new data, new PEBs
 * will be added into this volume.
 *
 * @note
 *      The addressed block is automatically erased before to be written.
 *
 * @return
 *      - LE_OK            On success
 *      - LE_BAD_PARAMETER If a parameter is invalid
 *      - LE_FAULT         On other error
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_flash_Write
(
    le_flash_PartitionRef_t partitionRef,
        ///< [IN] Partition reference to be used.
    uint32_t blockIndex,
        ///< [IN] Logical block index to be write.
    const uint8_t* writeDataPtr,
        ///< [IN] Data buffer to be written.
    size_t writeDataSize
        ///< [IN]
);

//--------------------------------------------------------------------------------------------------
/**
 * Retrieve information about the partition opened: the number of bad blocks found inside the
 * partition, the number of erase blocks, the size of the erase block and the size of the page.
 *
 * @return
 *      - LE_OK            On success
 *      - LE_BAD_PARAMETER If a parameter is invalid
 *      - LE_FAULT         On other error
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_flash_GetBlockInformation
(
    le_flash_PartitionRef_t partitionRef,
        ///< [IN] Partition reference to be used.
    uint32_t* badBlocksNumberPtr,
        ///< [OUT] Bad blocks number inside the partition
    uint32_t* eraseBlocksNumberPtr,
        ///< [OUT] Erase blocks number
    uint32_t* eraseBlockSizePtr,
        ///< [OUT] Erase block size
    uint32_t* pageSizePtr
        ///< [OUT] Page size
);

//--------------------------------------------------------------------------------------------------
/**
 * Retrieve information about the UBI volume opened: the number of free blocks for the UBI,
 * the number of currently allocated blocks to the volume and its real size in bytes.
 *
 * @return
 *      - LE_OK            On success
 *      - LE_BAD_PARAMETER If a parameter is invalid
 *      - LE_FAULT         On other error
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_flash_GetUbiVolumeInformation
(
    le_flash_PartitionRef_t partitionRef,
        ///< [IN] Partition reference to be used.
    uint32_t* freeBlockNumberPtr,
        ///< [OUT] Free blocks number on the UBI partition
    uint32_t* allocatedBlockNumberPtr,
        ///< [OUT] Allocated blocks number to the UBI volume
    uint32_t* sizeInBytesPtr
        ///< [OUT] Real size in bytes of the UBI volume
);

//--------------------------------------------------------------------------------------------------
/**
 * Create an UBI partition.
 * If the partition is already an UBI, an error is raised except if the flag isForcedCreate is set
 * to true. In this case, the whole UBI partition is recreated and the previous content is lost.
 * If the operation succeed, the partition is opened in write-only and this is not necessary to
 * call le_flash_OpenUbi().
 *
 * @return
 *      - LE_OK            On success
 *      - LE_BAD_PARAMETER If a parameter is invalid
 *      - LE_NOT_FOUND     If the flash partition is not found
 *      - LE_DUPLICATE     If the partition is already an UBI partition and isForcedCreate is not
 *                         set to true
 *      - LE_FAULT         On failure
 *
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_flash_CreateUbi
(
    const char* LE_NONNULL partitionName,
        ///< [IN] Partition to be opened.
    bool isForcedCreate,
        ///< [IN] Force the UBI recreation and
        ///< overwrite the previous content.
    le_flash_PartitionRef_t* partitionRefPtr
        ///< [OUT] Partition reference.
);

//--------------------------------------------------------------------------------------------------
/**
 * Create a new UBI volume into an UBI partition.
 * If the volume name or the volume ID already exists, an error is raised except if the flag
 * isForcedCreate is set to true. In this case, the whole UBI volume is recreated and the previous
 * content is lost. If the operation succeed, UBI volume is opened and this is not necessary to
 * call le_flash_OpenUbiVolume().
 * Note that the UBI partition should be opened in write-only or read-write mode, else an error is
 * raised.
 * The volumeName is the same parameter as le_flash_OpenUbiVolume().
 * A static volume cannot be extended when mounted, so it is generally used for SQUASHFS or others
 * immutables and R/O filesystems. A dynamic volume is extensible like UBIFS volumes.
 * The volume ID is the number of the UBI volume to be created. If set to NO_UBI_VOLUME_ID, the
 * first free volume ID will be used.
 *
 * @return
 *      - LE_OK            On success
 *      - LE_BAD_PARAMETER If a parameter is invalid
 *      - LE_NOT_PERMITTED If the UBI partition is not opened in write-only or read-write mode
 *      - LE_DUPLICATE     If the UBI volume already exists with a same name or a same volume ID
 *                         and isForcedCreate is not set to true
 *      - LE_FAULT         On failure
 *
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_flash_CreateUbiVolume
(
    le_flash_PartitionRef_t partitionRef,
        ///< [IN] Partition reference.
    bool isForcedCreate,
        ///< [IN] Force the UBI volume recreation and
        ///< overwrite the previous content.
    uint32_t volumeID,
        ///< [IN] Volume ID to set.
    le_flash_UbiVolumeType_t volumeType,
        ///< [IN] Volume type to set.
    const char* LE_NONNULL volumeName,
        ///< [IN] Volume name to be created.
    int32_t volumeSize
        ///< [IN] Volume size to set.
);

//--------------------------------------------------------------------------------------------------
/**
 * Delete an UBI volume from an UBI partition.
 * If the volume is currently opened by le_flash_OpenUbiVolume(), it is closed first.
 * Note that the UBI partition should be opened in write-only or read-write mode, else an error is
 * raised.
 *
 * @return
 *      - LE_OK            On success
 *      - LE_BAD_PARAMETER If a parameter is invalid
 *      - LE_NOT_PERMITTED If the UBI partition is not open in write-only or read-write mode
 *      - LE_NOT_FOUND     If the volume name is not found
 *      - LE_FAULT         On failure
 *
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_flash_DeleteUbiVolume
(
    le_flash_PartitionRef_t partitionRef,
        ///< [IN] Partition reference.
    const char* LE_NONNULL volumeName
        ///< [IN] Volume name to be deleted.
);

#endif // LE_FLASH_INTERFACE_H_INCLUDE_GUARD