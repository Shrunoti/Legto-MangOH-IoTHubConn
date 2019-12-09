

/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */

/**
 * @page c_fwupdate Firmware Update API
 *
 * @ref le_fwupdate_interface.h "API Reference"
 *
 * <HR>
 *
 * Firmware update allows the various firmware images to be updated from the application processor.
 * This may include the modem bootloader, modem firmware, and linux image, where the linux image
 * consists of the linux bootloader, linux kernel, and linux rootfs.
 *
 * Firmware update is useful when the image comes from an alternative source, rather than as an
 * over-the-air update through the AirVantage service.
 *
 * @section le_fwupdate_binding IPC interfaces binding
 *
 * All the functions of this API are provided by the @b le_fwupdate service.
 *
 * Here's a code sample binding to the le_fwupdate service:
 * @verbatim
   bindings:
   {
      clientExe.clientComponent.le_fwupdate -> fwupdateService.le_fwupdate
   }
   @endverbatim
 *
 *
 * @warning All of these APIs are not available for all platforms. Please refer to the
 * Product Technical specification document of your platform for further details.
 * Please refer to @subpage platformConstraintsDualSys for details on Dual System.
 *
 * @section le_fwupdate_image Update Firmware Image
 *
 * The firmware update process is started by calling le_fwupdate_Download().  This function takes
 * a file descriptor, rather than a file, to provide flexibility in the source of the image.  In
 * particular, this can be used to stream the image, rather than having to save it on the file
 * system before doing the update.
 *
 * On platform which does not support dual system, when the image is successfully downloaded, a
 * reset will occur in order to apply the update. This will reset all processors.  After the
 * application processor has restarted, the @ref le_info_version APIs can be used to determine
 * whether the firmware has been updated to the new version.
 *
 * On platform which supports dual system, if the image is successfully downloaded, the user needs
 * to swap the systems in order to use the updated system. This will reset all processors. After the
 * application processor has restarted, the @ref le_info_version APIs can be used to determine
 * whether the firmware has been updated to the new version.
 *
 * @section le_fwupdate_resume Update Firmware Image Download Resume
 *
 * During the download, the flash programming position is saved. Thanks to this position, fwupdate
 * service is able to resume the download without downloading the update package at the beginning.
 *
 * By default, the fwupdate service download API is resuming the previous download: new data
 * received through this API will be programmed at the resume position.
 *
 * @note
 * A download can be resumed when:
 * - le_fwupdate_Download() API has previously returned LE_CLOSED or LE_TIMEOUT
 * - A reset occured during last le_fwupdate_Download() processing (Legato/full system reset)
 * - No resume can be performed in other cases.
 *
 * A complete download can be forced by calling le_fwupdate_InitDownload(). Resume position
 * is set to 0.
 *
 * The current resume position can be retrieved by calling le_fwupdate_GetResumePosition().
 *
 * <HR>
 *
 * Copyright (C) Sierra Wireless Inc.
 */
/**
 * @file le_fwupdate_interface.h
 *
 * Legato @ref c_fwupdate include file.
 *
 * Copyright (C) Sierra Wireless Inc.
 */

#ifndef LE_FWUPDATE_INTERFACE_H_INCLUDE_GUARD
#define LE_FWUPDATE_INTERFACE_H_INCLUDE_GUARD


#include "legato.h"


//--------------------------------------------------------------------------------------------------
/**
 * Type for handler called when a server disconnects.
 */
//--------------------------------------------------------------------------------------------------
typedef void (*le_fwupdate_DisconnectHandler_t)(void *);

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
void le_fwupdate_ConnectService
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
le_result_t le_fwupdate_TryConnectService
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
void le_fwupdate_SetServerDisconnectHandler
(
    le_fwupdate_DisconnectHandler_t disconnectHandler,
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
void le_fwupdate_DisconnectService
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