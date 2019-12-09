

/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */

/**
 * @page c_spi SPI
 *
 * @ref le_spi_interface.h "API Reference" <br>
 * @subpage howToSPI "How To Setup SPI" <br>
 * @ref yoctoOutofTreeKernelModule
 *
 * <HR>
 *
 * @section spi_overview Overview
 *
 * This API is used by apps to control a Serial Peripheral Interface (SPI).
 *
 * The SPI API is configured as a four wire interface:
 * - Clock - Serial Clock
 * - MOSI - Master Output
 * - MISO - Master Input
 * - SS or CS - Chip select
 *
 * @section spi_usage Usage
 *
 * The sample code in this section shows how to use the SPI API for a user space app.
 *
 * Handle for passing to related functions to access the SPI device:
 * @code
 * le_spi_DeviceHandleRef_t spiHandle;
 * @endcode
 *
 * le_spi_Open() opens the @c spi file handle:
 * @code
 * le_result_t res;
 * res = le_spi_Open("spidev0.0", &spiHandle);
 * LE_FATAL_IF(res != LE_OK, "le_spi_Open failed with result=%s", LE_RESULT_TXT(res));
 * @endcode
 *
 * le_spi_Configure() initializes all the parameters on the master side based on the
 * implementation below uses mode = 0, 8 bit data, 960kbps and MSB first:
 * @code
 * le_spi_Configure(spiHandle, 0, 8, 960000, 0);
 * @endcode
 *
 * le_spi_WriteHD() writes data to slave device in half-duplex mode:
 * write_buffer_tx is an array. Example defined below:
 * @code
 * uinti8_t write_buffer_tx[] ={0xC7, 0x94, 0x80, 0x9A};
 * le_result_t res;
 * res = le_spi_WriteHD(spiHandle, write_buffer_tx, NUM_ARRAY_MEMBERS(write_buffer_tx));
 * LE_FATAL_IF(res != LE_OK, "le_spi_WriteHD failed with result=%s", LE_RESULT_TXT(res));
 * @endcode
 *
 * le_spi_WriteReadHD() writes and reads to/from slave device in half-duplex mode:
 * @code
 * le_result_t res;
 * le_spi_WriteReadHD(spiHandle, read_ID_tx, NUM_ARRAY_MEMBERS(read_ID_tx), ID_rx,
 *                    &readBufferSize);
 * LE_FATAL_IF(res != LE_OK, "le_spi_WriteReadHD failed with result=%s", LE_RESULT_TXT(res));
 * @endcode
 * read_ID_tx is an array that is transmitted to the device. ID_rx is a buffer reserved for
 * data received from  the device.
 *
 * le_spi_WriteReadFD() writes and reads to/from slave in full-duplex mode:
 * @code
 * le_result_t res;
 * res = le_spi_WriteReadHD(spiHandle, read_buffer_tx, read_rx, NUM_ARRAY_MEMBERS(read_buffer_tx);
 * LE_FATAL_IF(res != LE_OK, "le_spi_WriteReadHD failed with result=%s", LE_RESULT_TXT(res));
 * @endcode
 * read_buffer_tx is an array transmitted to the device. read_rx is a buffer reserved for
 * data received from the device. Buffer size for tx and rx must be the same.
 *
 * le_spi_Close() closes the spi handle:
 * @code
 * le_spi_Close(spiHandle);
 * @endcode
 *
 * Copyright (C) Sierra Wireless Inc.
 */
/**
 * @file le_spi_interface.h
 *
 * Legato @ref c_le_spi include file.
 *
 * Copyright (C) Sierra Wireless Inc.
 */

#ifndef LE_SPI_INTERFACE_H_INCLUDE_GUARD
#define LE_SPI_INTERFACE_H_INCLUDE_GUARD


#include "legato.h"


//--------------------------------------------------------------------------------------------------
/**
 * Type for handler called when a server disconnects.
 */
//--------------------------------------------------------------------------------------------------
typedef void (*le_spi_DisconnectHandler_t)(void *);

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
void le_spi_ConnectService
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
le_result_t le_spi_TryConnectService
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
void le_spi_SetServerDisconnectHandler
(
    le_spi_DisconnectHandler_t disconnectHandler,
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
void le_spi_DisconnectService
(
    void
);


//--------------------------------------------------------------------------------------------------
/**
 * Max byte storage size for write buffer
 */
//--------------------------------------------------------------------------------------------------
#define LE_SPI_MAX_WRITE_SIZE 1024

//--------------------------------------------------------------------------------------------------
/**
 * Max byte storage size for read buffer
 */
//--------------------------------------------------------------------------------------------------
#define LE_SPI_MAX_READ_SIZE 1024

//--------------------------------------------------------------------------------------------------
/**
 * Handle for passing to related functions to access the SPI device
 */
//--------------------------------------------------------------------------------------------------
typedef struct le_spi_DeviceHandle* le_spi_DeviceHandleRef_t;


//--------------------------------------------------------------------------------------------------
/**
 * Opens an SPI device so that the attached device may be accessed.
 *
 * @return
 *      - LE_OK on success
 *      - LE_BAD_PARAMETER if the device name string is bad
 *      - LE_NOT_FOUND if the SPI device file could not be found
 *      - LE_NOT_PERMITTED if the SPI device file can't be opened for read/write
 *      - LE_DUPLICATE if the given device file is already opened by another spiService client
 *      - LE_FAULT for non-specific failures
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_spi_Open
(
    const char* LE_NONNULL deviceName,
        ///< [IN] Handle for the SPI master to  open
    le_spi_DeviceHandleRef_t* handlePtr
        ///< [OUT] Handle for passing to related functions in order to
        ///< access the SPI device
);

//--------------------------------------------------------------------------------------------------
/**
 * Closes the device associated with the given handle and frees the associated resources.
 *
 * @note
 *      Once a handle is closed, it's not permitted to use it for future SPI access without first
 *      calling Open.
 */
//--------------------------------------------------------------------------------------------------
void le_spi_Close
(
    le_spi_DeviceHandleRef_t handle
        ///< [IN] Handle for the SPI master to close
);

//--------------------------------------------------------------------------------------------------
/**
 * Configures an SPI device's data sample settings. The required values should be
 * included in your target's datasheet. Most common @c Mode values are
 * @c 0 and @c 3.
 *
 * These are the SPI Mode options:
 * |   Mode  |   Clock Polarity   |  Clock Phase   |   Clock Edge   |
 * | :-----: | :----------------: | :------------: | :------------: |
 * |   0     |       0            |   0            |    1           |
 * |   1     |       0            |   1            |    0           |
 * |   2     |       1            |   0            |    1           |
 * |   3     |       1            |   1            |    0           |
 *
 * @note
 *      This function should be called before any of the Read/Write functions to ensure
 *      the SPI bus configuration is in a known state.
 */
//--------------------------------------------------------------------------------------------------
void le_spi_Configure
(
    le_spi_DeviceHandleRef_t handle,
        ///< [IN] Handle for the SPI master to configure
    int32_t mode,
        ///< [IN] Choose mode options for the bus per above table
    uint8_t bits,
        ///< [IN] bits per word, typically 8 bits per word
    uint32_t speed,
        ///< [IN] max speed (Hz), this is slave dependant
    int32_t msb
        ///< [IN] set as 0 for MSB as first bit or 1 for LSB as first bit
);

//--------------------------------------------------------------------------------------------------
/**
 * Performs SPI WriteRead Half Duplex. You can send send Read command/ address of data to read.
 *
 * @return
 *      LE_OK on success or LE_FAULT on failure.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_spi_WriteReadHD
(
    le_spi_DeviceHandleRef_t handle,
        ///< [IN] Handle for the SPI master to perform the write-read on
    const uint8_t* writeDataPtr,
        ///< [IN] TX command/address being sent to slave with size
    size_t writeDataSize,
        ///< [IN]
    uint8_t* readDataPtr,
        ///< [OUT] RX response from slave with number of bytes reserved
        ///< on master
    size_t* readDataSizePtr
        ///< [INOUT]
);

//--------------------------------------------------------------------------------------------------
/**
 * SPI Write for Half Duplex Communication
 *
 * @return
 *      LE_OK on success or LE_FAULT on failure.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_spi_WriteHD
(
    le_spi_DeviceHandleRef_t handle,
        ///< [IN] Handle for the SPI master to perform the write on
    const uint8_t* writeDataPtr,
        ///< [IN] TX command/address being sent to slave with size
    size_t writeDataSize
        ///< [IN]
);

//--------------------------------------------------------------------------------------------------
/**
 * SPI Read for Half Duplex Communication
 *
 * @return
 *      LE_OK on success or LE_FAULT on failure.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_spi_ReadHD
(
    le_spi_DeviceHandleRef_t handle,
        ///< [IN] Handle for the SPI master to perform the read from
    uint8_t* readDataPtr,
        ///< [OUT] RX response from slave with number of bytes reserved
        ///<  on master
    size_t* readDataSizePtr
        ///< [INOUT]
);

//--------------------------------------------------------------------------------------------------
/**
 * Simultaneous SPI Write and  Read for full duplex communication
 *
 * @return
 *      LE_OK on success or LE_FAULT on failure.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_spi_WriteReadFD
(
    le_spi_DeviceHandleRef_t handle,
        ///< [IN] Handle for the SPI master to perform full duplex write-read on
    const uint8_t* writeDataPtr,
        ///< [IN] TX command/address being sent to slave with size
    size_t writeDataSize,
        ///< [IN]
    uint8_t* readDataPtr,
        ///< [OUT] RX response from slave with same buffer size as TX
    size_t* readDataSizePtr
        ///< [INOUT]
);

#endif // LE_SPI_INTERFACE_H_INCLUDE_GUARD