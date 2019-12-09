

/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */


#ifndef LE_FLASH_INTERFACE_H_INCLUDE_GUARD
#define LE_FLASH_INTERFACE_H_INCLUDE_GUARD


#include "legato.h"


//--------------------------------------------------------------------------------------------------
/**
 * Get the server service reference
 */
//--------------------------------------------------------------------------------------------------
le_msg_ServiceRef_t le_flash_GetServiceRef
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * Get the client session reference for the current message
 */
//--------------------------------------------------------------------------------------------------
le_msg_SessionRef_t le_flash_GetClientSessionRef
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * Initialize the server and advertise the service.
 */
//--------------------------------------------------------------------------------------------------
void le_flash_AdvertiseService
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