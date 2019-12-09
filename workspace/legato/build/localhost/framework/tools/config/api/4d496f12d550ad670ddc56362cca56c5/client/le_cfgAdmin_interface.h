

/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */

/**
 * @page c_configAdmin Config Tree Admin API
 *
 * @ref le_cfgAdmin_interface.h "API Reference"
 *
 * The Config Tree Admin API is intended to provide tools help with administration of Trees.  These
 * administration functions are provided as a separate API for security reasons. The Admin API needs
 * to be explicitly included to use these admin functions keeping access separate from the main app
 * permissions.
 *
 * The API includes the following functions:
 * - an iterator function to walk the current list of trees.
 * - an import function to bulk load the data (full or partial) into a tree.
 * - an export function to save the contents of a tree.
 * - a delete function to remove a tree and all its objects.
 *
 * Example of @b Iterating the List of Trees:
 *
 * @code
 * void ListTrees(void)
 * {
 *     // Open a tree iterator.  Note that this isn't the same iterator that you would be using for
 *     // read and write transactions.
 *     le_cfgAdmin_IteratorRef_t iteratorRef = le_cfgAdmin_CreateTreeIterator();
 *
 *     LE_INFO("Listing configuration Trees in the current system...");
 *
 *     // The iteratorRef starts at item -1, so we need to start with calling NextTree in our loop.
 *     while (le_cfgAdmin_NextTree(iteratorRef) == LE_OK)
 *     {
 *         // Simply extract the name of the tree and dump it to the device log.
 *         char treeName[MAX_TREE_NAME_BYTES] = "";
 *
 *         if (le_cfgAdmin_GetTreeName(iteratorRef, treeName, sizeof(treeName)) == LE_OK)
 *         {
 *             LE_INFO("Tree: '%s'", treeName);
 *         }
 *     }
 *
 *     // Again, this isn't a regular config node iterator, so you need to use the cfgAdmin
 *     // release on the tree iterator.
 *     le_cfgAdmin_ReleaseTreeIterator(iteratorRef);
 * }
 * @endcode
 *
 * Example of @b Importing a Tree:
 *
 * @code
 * void ImportMyTree(const char* filePath)
 * {
 *     static char resolvedPath[PATH_MAX] = "";
 *
 *     // Because the configTree is a seperate process, we need to make sure that the path we
 *     // received is an absolute path.
 *     LE_FATAL_IF(realpath(filePath, resolvedPath) == NULL, "Could not resolve filePath: %m");
 *
 *     // Open a write transaction on /myData, as we will be writing to the configTree.
 *     le_cfg_IteratorRef_t iteratorRef = le_cfg_CreateReadTxn("/myData");
 *
 *     // Our iterator is currently on /myData, so everything under that node is replaced.  If we
 *     // want to replace the whole tree we could supply a "/" here instead of using the iterator's
 *     // current location.  Alternativly, we could have opened or moved the iterator to "/" in the
 *     // first place.
 *     LE_FATAL_IF(le_cfgAdmin_ExportTree(iteratorRef, resolvedPath, "") != LE_OK,
 *                 "Error occured while writing config data.");
 *
 *     // Close up the iterator,free it's resources, and commit the new data to the configTree.
 *     le_cfg_CommitTxn(iteratorRef);
 * }
 *
 * // To import the config data back from ./myData.cfg:
 * ImportMyData("./myData.cfg");
 * @endcode
 *
 * Example of @b Exporting a Tree
 *
 * @code
 * void ExportMyData(const char* filePath)
 * {
 *     static char resolvedPath[PATH_MAX] = "";
 *
 *     // Because the configTree is a seperate process, we need to make sure that the path we
 *     // received is an absolute path.
 *     LE_FATAL_IF(realpath(filePath, resolvedPath) == NULL, "Could not resolve filePath: %m");
 *
 *     // Open a read transaction on /myData.
 *     le_cfg_IteratorRef_t iteratorRef = le_cfg_CreateReadTxn("/myData");
 *
 *     // Our iterator is currently on /myData, so everything under that node is exported.  If we
 *     // want to export the whole tree we could supply a "/" here instead of using the iterator's
 *     // current location.  Alternativly, we could have opened or moved the iterator to "/" in the
 *     // first place.
 *     LE_FATAL_IF(le_cfgAdmin_ExportTree(iteratorRef, resolvedPath, "") != LE_OK,
 *                 "Error occured while writing config data.");
 *
 *     // Close up the iterator and free it's resources.
 *     le_cfg_CancelTxn(iteratorRef);
 * }
 *
 * // To export the config tree to ./myData.cfg:
 * ExportMyData("./myData.cfg");
 * @endcode
 *
 * Example of @b Deleting a Tree
 *
 * @code
 * // To delete a tree simply call le_cfgAdmin_DeleteTree.  For example to delete the tree foo,
 * // call as follows:
 * le_cfgAdmin_DeleteTree("foo");
 * @endcode
 *
 * Copyright (C) Sierra Wireless Inc.
 */
/**
 * @file le_cfgAdmin_interface.h
 *
 * Legato @ref c_configAdmin include file.
 *
 * Copyright (C) Sierra Wireless Inc.
 */

#ifndef LE_CFGADMIN_INTERFACE_H_INCLUDE_GUARD
#define LE_CFGADMIN_INTERFACE_H_INCLUDE_GUARD


#include "legato.h"

// Interface specific includes
#include "le_cfg_interface.h"


//--------------------------------------------------------------------------------------------------
/**
 * Type for handler called when a server disconnects.
 */
//--------------------------------------------------------------------------------------------------
typedef void (*le_cfgAdmin_DisconnectHandler_t)(void *);

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
void le_cfgAdmin_ConnectService
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
le_result_t le_cfgAdmin_TryConnectService
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
void le_cfgAdmin_SetServerDisconnectHandler
(
    le_cfgAdmin_DisconnectHandler_t disconnectHandler,
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
void le_cfgAdmin_DisconnectService
(
    void
);


//--------------------------------------------------------------------------------------------------
/**
 * Reference to an iterator object that can be used to iterate over the list of trees.
 */
//--------------------------------------------------------------------------------------------------
typedef struct le_cfgAdmin_Iterator* le_cfgAdmin_IteratorRef_t;


//--------------------------------------------------------------------------------------------------
/**
 * Read a subset of the configuration tree from the given filePath.  The tree then overwrites the
 * node at the given nodePath.
 *
 * This function will import a sub-tree as part of the iterator's current transaction.  This allows
 * you to create an iterator on a given node.  Import a sub-tree, and then examine the contents of
 * the import before deciding to commit the new data.
 *
 * @return This function will return one of the following values:
 *
 *         - LE_OK            - The commit was completed successfuly.
 *         - LE_FAULT         - An I/O error occured while reading the data.
 *         - LE_FORMAT_ERROR  - The configuration data being imported appears corrupted.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_cfgAdmin_ImportTree
(
    le_cfg_IteratorRef_t iteratorRef,
        ///< [IN] Write iterator that is being used for the import.
    const char* LE_NONNULL filePath,
        ///< [IN] Import the tree data from the this file.
    const char* LE_NONNULL nodePath
        ///< [IN] Where in the tree should this import happen?  Leave
        ///<   as an empty string to use the iterator's current
        ///<   node.
);

//--------------------------------------------------------------------------------------------------
/**
 * Take a node given from nodePath and stream it and it's children to the file given by filePath.
 *
 * This funciton uses the iterator's read transaction, and takes a snapshot of the current state of
 * the tree.  The data write happens immediately.
 *
 * @return This function will return one of the following values:
 *
 *         - LE_OK     - The commit was completed successfuly.
 *         - LE_FAULT  - An I/O error occured while writing the data.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_cfgAdmin_ExportTree
(
    le_cfg_IteratorRef_t iteratorRef,
        ///< [IN] Write iterator that is being used for the export.
    const char* LE_NONNULL filePath,
        ///< [IN] Import the tree data from the this file.
    const char* LE_NONNULL nodePath
        ///< [IN] Where in the tree should this export happen?  Leave
        ///<   as an empty string to use the iterator's current
        ///<   node.
);

//--------------------------------------------------------------------------------------------------
/**
 * Delete a tree from the system, both from the filesystem and from memory.
 */
//--------------------------------------------------------------------------------------------------
void le_cfgAdmin_DeleteTree
(
    const char* LE_NONNULL treeName
        ///< [IN] Name of the tree to delete.
);

//--------------------------------------------------------------------------------------------------
/**
 * Create a new iterator object for iterating over the list of trees currently managed by the
 * config tree daemon.
 */
//--------------------------------------------------------------------------------------------------
le_cfgAdmin_IteratorRef_t le_cfgAdmin_CreateTreeIterator
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * Release the iterator and free it's memory back to the system.
 */
//--------------------------------------------------------------------------------------------------
void le_cfgAdmin_ReleaseTreeIterator
(
    le_cfgAdmin_IteratorRef_t iteratorRef
        ///< [IN] Iterator object to release.
);

//--------------------------------------------------------------------------------------------------
/**
 * Read the name of the tree currently pointed at by the iterator.
 *
 * @return LE_OK if there is enough room to copy the name into the supplied buffer.  LE_OVERFLOW if
 *         not.  LE_NOT_FOUND is returned if the list is empty or if the iterator hasn't been moved
 *         onto the first item yet.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_cfgAdmin_GetTreeName
(
    le_cfgAdmin_IteratorRef_t iteratorRef,
        ///< [IN] Iterator object to read.
    char* name,
        ///< [OUT] Name of the currently referenced tree is
        ///<   passed in this out parameter.
    size_t nameSize
        ///< [IN]
);

//--------------------------------------------------------------------------------------------------
/**
 * Move onto the next tree in the list.  If there are no more trees this function returns false,
 * otherwise true is returned.
 *
 * @return LE_OK if there are more trees to iterate through.  LE_NOT_FOUND if not.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_cfgAdmin_NextTree
(
    le_cfgAdmin_IteratorRef_t iteratorRef
        ///< [IN] Iterator to iterate.
);

#endif // LE_CFGADMIN_INTERFACE_H_INCLUDE_GUARD