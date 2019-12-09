

/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */


#ifndef LE_FWUPDATE_INTERFACE_H_INCLUDE_GUARD
#define LE_FWUPDATE_INTERFACE_H_INCLUDE_GUARD


#include "legato.h"


//--------------------------------------------------------------------------------------------------
/**
 * Get the server service reference
 */
//--------------------------------------------------------------------------------------------------
le_msg_ServiceRef_t le_fwupdate_GetServiceRef
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * Get the client session reference for the current message
 */
//--------------------------------------------------------------------------------------------------
le_msg_SessionRef_t le_fwupdate_GetClientSessionRef
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * Initialize the server and advertise the service.
 */
//--------------------------------------------------------------------------------------------------
void le_fwupdate_AdvertiseService
(
    void
);


//--------------------------------------------------------------------------------------------------
/**
 * Maximum length of a version string, excluding any termination character.
 */
//--------------------------------------------------------------------------------------------------
#define LE_FWUPDATE_MAX_VERS_LEN 256

//--------------------------------------------------------------------------------------------------
/**
 * Maximum length of the update status label
 */
//--------------------------------------------------------------------------------------------------
#define LE_FWUPDATE_STATUS_LABEL_LENGTH_MAX 32

//--------------------------------------------------------------------------------------------------
/**
 * Update status
 * Indicates either a success or the defective image partition.
 */
//--------------------------------------------------------------------------------------------------
typedef enum
{
    LE_FWUPDATE_UPDATE_STATUS_OK = 0,
        ///< Last update succeeded
    LE_FWUPDATE_UPDATE_STATUS_PARTITION_ERROR = 1,
        ///< At least, a partition is corrupted
    LE_FWUPDATE_UPDATE_STATUS_DWL_ONGOING = 2,
        ///< Downloading in progress
    LE_FWUPDATE_UPDATE_STATUS_DWL_FAILED = 3,
        ///< Last downloading failed
    LE_FWUPDATE_UPDATE_STATUS_DWL_TIMEOUT = 4,
        ///< Last downloading ended with timeout
    LE_FWUPDATE_UPDATE_STATUS_UNKNOWN = 5
        ///< Unknown status. It has to be the last one.
}
le_fwupdate_UpdateStatus_t;



//--------------------------------------------------------------------------------------------------
/**
 * Download the firmware image file. The function can also be used to resume the download if the
 * le_fwupdate_InitDownload() function is not called before.
 *
 * @return
 *      - LE_OK              On success
 *      - LE_BAD_PARAMETER   If an input parameter is not valid
 *      - LE_TIMEOUT         After 900 seconds without data received
 *      - LE_NOT_POSSIBLE    The systems are not synced
 *      - LE_UNAVAILABLE     The flash access is not granted for SW update
 *      - LE_CLOSED          File descriptor has been closed before all data have been received
 *      - LE_FAULT           On failure
 *
 * @note
 *      The process exits, if an invalid file descriptor (e.g. negative) is given.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_fwupdate_Download
(
    int fd
        ///< [IN] File descriptor of the image, opened to the start of the image.
);



//--------------------------------------------------------------------------------------------------
/**
 * Download initialization:
 *  - for single and dual systems, it resets resume position,
 *  - for dual systems, it synchronizes the systems if needed.
 *
 * @note
 *      When invoked, resuming a previous download is not possible, a full update package has to be
 *      downloaded.
 *
 * @return
 *      - LE_OK         On success
 *      - LE_FAULT      On failure
 *      - LE_IO_ERROR   Dual systems platforms only -- The synchronization fails due to
 *                      unrecoverable ECC errors. In this case, the update without synchronization
 *                      is forced, but the whole system must be updated to ensure that the new
 *                      update system will be workable
 *                      ECC stands for Error-Correction-Code: some errors may be corrected. If this
 *                      correction fails, a unrecoverable error is registered and the data become
 *                      corrupted.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_fwupdate_InitDownload
(
    void
);



//--------------------------------------------------------------------------------------------------
/**
 * Return the downloaded update package write position.
 *
 * @return
 *      - LE_OK              On success
 *      - LE_BAD_PARAMETER   The given parameter is invalid.
 *      - LE_FAULT           On failure
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_fwupdate_GetResumePosition
(
    size_t* positionPtr
        ///< [OUT] Update package read position
);



//--------------------------------------------------------------------------------------------------
/**
 * Return the update status.
 *
 * @return
 *      - LE_OK on success
 *      - LE_BAD_PARAMETER Invalid parameter
 *      - LE_FAULT on failure
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_fwupdate_GetUpdateStatus
(
    le_fwupdate_UpdateStatus_t* statusPtr,
        ///< [OUT] Returned update status
    char* statusLabel,
        ///< [OUT] Description of the status
    size_t statusLabelSize
        ///< [IN]
);



//--------------------------------------------------------------------------------------------------
/**
 * Get the firmware version string
 *
 * @return
 *      - LE_OK          on success
 *      - LE_NOT_FOUND   if the version string is not available
 *      - LE_OVERFLOW    if version string to big to fit in provided buffer
 *      - LE_FAULT       for any other errors
 *
 * @note If the caller is passing a bad pointer into this function, it is a fatal error, the
 *       function will not return.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_fwupdate_GetFirmwareVersion
(
    char* version,
        ///< [OUT] Firmware version string
    size_t versionSize
        ///< [IN]
);



//--------------------------------------------------------------------------------------------------
/**
 * Get the bootloader version string
 *
 * @return
 *      - LE_OK          on success
 *      - LE_NOT_FOUND   if the version string is not available
 *      - LE_OVERFLOW    if version string to big to fit in provided buffer
 *      - LE_FAULT       for any other errors
 *
 * @note If the caller is passing a bad pointer into this function, it is a fatal error, the
 *       function will not return.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_fwupdate_GetBootloaderVersion
(
    char* version,
        ///< [OUT] Bootloader version string
    size_t versionSize
        ///< [IN]
);



//--------------------------------------------------------------------------------------------------
/**
 * Get the app bootloader version string
 *
 * @return
 *      - LE_OK             on success
 *      - LE_NOT_FOUND      if the version string is not available
 *      - LE_OVERFLOW       if version string to big to fit in provided buffer
 *      - LE_BAD_PARAMETER  bad parameter
 *      - LE_UNSUPPORTED    not supported
 *      - LE_FAULT          for any other errors
 *
 * @note If the caller passes in a bad pointer, it is a fatal error and the function won't return.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_fwupdate_GetAppBootloaderVersion
(
    char* version,
        ///< [OUT] Bootloader version string
    size_t versionSize
        ///< [IN]
);



//--------------------------------------------------------------------------------------------------
/**
 * Get the status of the system.
 *
 * Dual System: Indicates if Active and Update systems are synchronized
 * Single System: This api is not supported on single system.
 *
 * @return
 *      - LE_OK            On success
 *      - LE_UNSUPPORTED   The feature is not supported
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_fwupdate_IsSystemMarkedGood
(
    bool* isSystemGoodPtr
        ///< [OUT] true if the system is marked good, false otherwise
);



//--------------------------------------------------------------------------------------------------
/**
 * Request to install the package. Calling this API will result in a system reset.
 *
 * Dual System: After reset, the UPDATE and ACTIVE systems will be swapped.
 * Single System: After reset, the image in the FOTA partition will be installed on the device.
 *
 * @note On success, a device reboot is initiated without returning any value.
 *
 *
 * @return
 *      - LE_BUSY          Download is ongoing, install is not allowed
 *      - LE_UNSUPPORTED   The feature is not supported
 *      - LE_FAULT         On failure
 *
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_fwupdate_Install
(
    void
);



//--------------------------------------------------------------------------------------------------
/**
 * Mark the current system as good.
 *
 * Dual System: Requests a system Sync. The UPDATE system will be synchronised with the ACTIVE one.
 * Single System: This api is not supported on single system.
 *
 * @return
 *      - LE_OK            On success
 *      - LE_UNSUPPORTED   The feature is not supported
 *      - LE_UNAVAILABLE   The flash access is not granted for SW update
 *      - LE_FAULT         On failure
 *      - LE_IO_ERROR      Dual systems platforms only -- The synchronization fails due to
 *                         unrecoverable ECC errors
 *                         ECC stands for Error-Correction-Code: some errors may be corrected. If
 *                         this correction fails, an unrecoverable error is registered and the data
 *                         become corrupted.
 *
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_fwupdate_MarkGood
(
    void
);



//--------------------------------------------------------------------------------------------------
/**
 * Request to install the package and marks the system as good. Calling this API will result in a
 * system reset.
 *
 * Dual System: Request a full system reset with a systems SWAP and systems SYNC. After the reset,
 * the UPDATE and ACTIVE systems will be swapped and synchronized.
 * Single System: Installs the package from the FOTA partition.
 *
 *
 * @note On success, a device reboot is initiated without returning any value.
 *
 * @return
 *      - LE_FAULT         On failure
 *
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_fwupdate_InstallAndMarkGood
(
    void
);


#endif // LE_FWUPDATE_INTERFACE_H_INCLUDE_GUARD