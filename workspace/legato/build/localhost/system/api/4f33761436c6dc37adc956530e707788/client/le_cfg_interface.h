

/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */

/**
 * @page c_config Config Tree API
 *
 * @ref le_cfg_interface.h "API Reference" <br>
 *
 * The Config Tree is a non-volatile noSQL database that stores configuration values for apps.
 * By default each app is given read access to a single tree that has the same name as the app.
 * Any other access permissions (read access for any other tree, or write access to any tree at all)
 * must be explicitly granted in the app's @ref defFilesAdef_requiresConfigTree ".adef file".
 *
 * Trees are created automatically the first time that they are accessed by an app or a component.
 * The apps tree will be named the same as the name of the app. It is possible for apps to be given
 * access to other apps tree's or for multiple apps to share one tree.
 *
 * @note If your app is running as root then the configuration will get added to the
 * System Tree by default. System utilities also use the Config Tree and store their
 * configurations in the @c system tree.
 *
 * Apps are able to search over the tree for data, although it's generally expected that the location
 * will be known to the app and that the app will point to the right node to retrieve the value.
 *
 * More on the @ref conceptsConfig "Config Strategy" for the Legato AF.
 *
 * @section cfg_Overview Overview
 *
 * The tree is broken down into stems and leaves.
 *
 * A stem is a node that has at least one child node. A leaf has no children and holds a value.
 *
 * @code
 *        +tree
 *            |
 *            +--stem
 *            |   |
 *            |   +--leaf (value)
 *            |
 *            +-stem
 *               |
 *               +--leaf (value)
 *               |
 *               +--leaf (value)
 * @endcode
 *
 * Paths in the tree are traditional Unix style paths and each node is separated by a /:
 *
 * @code /stem/stem/leaf @endcode
 *
 * If no app is specified as the root of the tree, then the app will search in it's own tree.
 * To get to another tree reference that tree followed by a colon.
 *
 * @verbatim
 /path/to/my/tree/value                   # references the default apps tree
 secondTree:/path/to/secondTree/value    # references the second tree
 @endverbatim
 *
 * @note It's recommended to store anything complex using stems and leaves, this enhances
 * readability and debugging. It also helps to sidestep nasty cross platform alignment issues.
 *
 * Apps must explicitly give permissions to other apps before they can access their Config Tree data.
 * This is done in the @ref defFilesAdef_requiresConfigTree ".adef file".
 * Each transaction is only able to iterate over one tree at a time, each tree that you want to read
 * or write to must be created as a separate transaction.
 *
 * The Config Tree supports storing the following types of data and each has their own get/set
 * function as well as a quick set/get function (see @ref cfg_quick):
 * -  string
 * -  signed integer
 * -  boolean
 * -  64bit floating point
 * -  empty values.
 *
 * Each transaction has a global timeout setting (default is 30s). The configuration is
 * located in the System Tree and may be configured with the @ref toolsTarget_config "config"
 * target tool.
 *
 * @verbatim
 config /configTree/transactionTimeout 10 int         #changes the timeout to 10s
 @endverbatim
 *
 * @section cfg_trans Transactions
 *
 * @subsection cfg_transConcepts Key Transaction Concepts
 *
 * -  All transactions are sent to a queue and processed in a sequence.
 * -  Only one write transaction may be active at a time and subsequent writes are queued
 *    until the first is finished processing.
 * -  Transactions may contain multiple read or write requests within a single transaction.
 * -  Multiple read transactions may be processed while a write transaction is active.
 * -  Quick(implicit) read/writes can be created and are also sequentially queued.
 *
 * @subsection cfg_createTrans Create Transactions
 *
 * To make a change to the tree, you must Create a write transaction, call one or more Set
 * functions, and Commit the transaction.  If a write transaction is canceled instead of committed,
 * then the changes will be discarded and the tree will remain unchanged.
 *
 * To read from a tree without making any changes, you should:
 * - create a read transaction,
 * - call the Get functions,
 * - cancel the transaction when finished.
 *
 * You could also:
 * - create a write transaction,
 * - perform only Get operations,
 * - cancel the transaction
 *
 * @note It's safer to use a read transaction when there is no intention to change the tree.
 *
 * Transactions must not be kept open for extended periods of time.  If a transaction is kept open
 * for longer than the transaction time limit (default is 30 seconds), then the Config Tree will
 * cancel the transaction and drop the connection to the offending client (most likely causing the
 * client process to terminate).
 *
 * | Function                  | Action                                                            |
 * | --------------------------| ------------------------------------------------------------------|
 * | @c le_cfg_CreateReadTxn() | Opens the transaction                                             |
 * | @c le_cfg_CancelTxn()     | Closes a read/write transaction and does not write it to the tree |
 * | @c le_cfg_CommitTxn()     | Closes a write transaction and queues it for commit               |
 *
 * @subsection cfg_transNavigate Navigating the Tree
 *
 * To move around within the Tree you can move directly to a specific node(leaf or stem) and then
 * do your read or write from that point.  Functions have been added to easily navigate through
 * Tree. All nodes can be referenced either by their absolute or relative paths.
 *
 * | Function                    | Action                                                                         |
 * | ----------------------------| -------------------------------------------------------------------------------|
 * | @c le_cfg_GoToNode()        | Moves to the location specified                                                |
 * | @c le_cfg_GoToParent()      | Moves to the parent of the current node (moves up the Tree)                    |
 * | @c le_cfg_GoToFirstChild()  | Moves to the first node from the current location (moves down the Tree)        |
 * | @c le_cfg_GoToNextSibling() | Moves to the next node on the same level as the current node (moves laterally) |
 *
 * @subsection cfg_transGetInfo Retrieving Node Information
 *
 * The Config tree also contains functions to help you identify your current location in the tree,
 * what node you are currently pointing at, and what type of data is contained in the current node.
 *
 * | Function                | Action                                                                          |
 * | ------------------------| ------------------------------------------------------------------------------- |
 * | @c le_cfg_GetPath()     | Gets the location of where you are in the Tree                                  |
 * | @c le_cfg_GetNodeType() | Gets the data type of the node where you are currently located                  |
 * | @c le_cfg_GetNodeName() | Gets the name of the node where you are in the Tree (does not include the path) |
 *
 * @subsection cfg_read Read Transactions
 *
 * Each data type has it's own get function to read a value from a node within the Tree.
 *
 * | Function                | Action                                   |
 * | ------------------------| -----------------------------------------|
 * | @c le_cfg_GetString()   | Reads the string's value                 |
 * | @c le_cfg_GetInt()      | Reads the integer's value                |
 * | @c le_cfg_GetFloat()    | Reads the floating point value           |
 * | @c le_cfg_GetBool()     | Reads the boolean value                  |
 *
 * To perform a read from a Tree, we need to open a transaction, move to the node that you want to
 * read from, read the node and then cancel the transaction.
 *
 * Sample read transaction (with error checking):
 *
 * @code
 * le_result_t GetIp4Static  //reads the IP address values from the Config Tree
 * (
 *     const char* interfaceNamePtr,
 *     char* ipAddrPtr,
 *     size_t ipAddrSize,
 *     char* netMaskPtr,
 *     size_t netMaskSize
 * )
 * {
 *     // Change current tree position to the base ip4 node.
 *     char nameBuffer[LE_CFG_STR_LEN_BYTES] = { 0 };
 *
 *     // Returns errors for out of bounds exceptions
 *     int r = snprintf(nameBuffer, sizeof(nameBuffer), "/system/%s/ip4", interfaceNamePtr);
 *     if (r < 0)
 *     {
 *         return LE_FAULT;
 *     }
 *     else if (r >= sizeof(nameBuffer))
 *     {
 *         return LE_OVERFLOW;
 *     }
 *
 *     // Open up a read transaction on the Config Tree.
 *     le_cfg_IteratorRef_t iteratorRef = le_cfg_CreateReadTxn(nameBuffer);
 *
 *     if (le_cfg_NodeExists(iteratorRef, "") == false)
 *     {
 *         LE_WARN("Configuration not found.");
 *         le_cfg_CancelTxn(iteratorRef);
 *         return LE_NOT_FOUND;
 *     }
 *
 *     // Returns the IP Address value stored in the Config Tree.
 *     le_result_t result = le_cfg_GetString(iteratorRef, "addr", ipAddrPtr, ipAddrSize, "");
 *     if (result != LE_OK)
 *     {
 *         le_cfg_CancelTxn(iteratorRef);
 *         return result;
 *     }
 *
 *     // Returns the NetMask value stored in the Config Tree.
 *     result = le_cfg_GetString(iteratorRef, "mask", netMaskPtr, netMaskSize, "");
 *     if (result != LE_OK)
 *     {
 *         le_cfg_CancelTxn(iteratorRef);
 *         return result;
 *     }
 *
 *     // Close the transaction and return success.
 *     le_cfg_CancelTxn(iteratorRef);
 *
 *     return LE_OK;
 * }
 * @endcode
 *
 * @note Any writes done will be discarded at the end of the read transaction.
 *
 * @subsection cfg_write Write Transactions
 *
 * Each data type has it's own set function, to write a value to a node within the Tree. Before you
 * are able to write to a tree, permissions must be set in the apps
 * @ref defFilesAdef_requiresConfigTree ".adef's requires section" or with the
 * @ref toolsTarget_config tool.
 *
 * | Function                | Action                                   |
 * | ------------------------| -----------------------------------------|
 * | @c le_cfg_SetString()   | Writes the string's value                |
 * | @c le_cfg_SetInt()      | Writes the integer's value               |
 * | @c le_cfg_SetFloat()    | Writes the floating point value          |
 * | @c le_cfg_SetBool()     | Writes the boolean value                 |
 *
 * To perform a write to a Tree, we need to open a transaction, move to the node that you want to
 * write to, write to the node and then commit the transaction.
 *
 * Sample write transaction (with error checking):
 *
 * @code
 * void SetIp4Static
 * (
 *     const char* interfaceNamePtr,
 *     const char* ipAddrPtr,
 *     const char* netMaskPtr
 * )
 * {
 *     // Change current tree position to the base ip4 node.
 *     char nameBuffer[LE_CFG_STR_LEN_BYTES] = { 0 };
 *
 *     int r = snprintf(nameBuffer, sizeof(nameBuffer), "/system/%s/ip4", interfaceNamePtr);
 *     LE_ASSERT((r >= 0) && (r < sizeof(nameBuffer));
 *
 *     // Create a write transaction so that we can update the tree.
 *     le_cfg_IteratorRef_t iteratorRef = le_cfg_CreateWriteTxn(nameBuffer);
 *
 *     le_cfg_SetString(iteratorRef, "addr", ipAddrPtr);
 *     le_cfg_SetString(iteratorRef, "mask", netMaskPtr);
 *
 *     // Commit the transaction to make sure these new settings get written to the tree.
 *     le_cfg_CommitTxn(iteratorRef);
 * }
 * @endcode
 *
 * @note Creating write transactions creates a temporary working copy of the tree for use within the
 * transaction. All read transactions running in the meantime see the committed state, without any
 * of the changes that have been made within the write transaction.
 *
 * @subsection cfg_transDelete Deleting a Node
 *
 * You can also delete a node from the tree.  A word of caution as deleting a node will
 * automatically delete all children nodes as well.
 *
 * | Function                 | Action                                   |
 * | -------------------------| -----------------------------------------|
 * | @c le_cfg_DeleteNode()   | Deletes the node and all children        |
 *
 * @section cfg_quick Quick Read/Writes
 *
 * Another option is to perform quick read/write which implicitly wraps functions with in an
 * internal transaction. This is ideal if all you need to do is read or write some simple values
 * to the default app tree.
 *
 * The quick reads and writes work almost identically to the transactional version except quick
 * reads don't explicitly take an iterator object. The quick functions internally use an
 * implicit transaction. This implicit transaction wraps one get or set, and does not protect
 * your code from other activity in the system.
 *
 * Because quick read/write functions don't get created within a transaction, there is no option to
 * traverse to a specific node.
 * All values that are read or written must be referenced from the root of the tree.
 *
 * A quick delete example:
 *
 * @code
 * void ClearIpInfo
 * (
 *     const char* interfaceNamePtr
 * )
 * {
 *     char pathBuffer[MAX_CFG_STRING] = { 0 };
 *
 *     // Removes the node from the tree.
 *     snprintf(pathBuffer, sizeof(pathBuffer), "/system/%s/ip4/", interfaceNamePtr);
 *     le_cfg_QuickDeleteNode(pathBuffer);
 * }
 * @endcode
 *
 * @warning Because each quick function is independent, there's no guarantee of consistency between
 * them.  If another process changes one of the values while you read/write the other,
 * the two values could be read out of sync.
 *
 * Copyright (C) Sierra Wireless Inc.
 */
/**
 * @file le_cfg_interface.h
 *
 * Legato @ref c_config include file.
 *
 * Copyright (C) Sierra Wireless Inc.
 */

#ifndef LE_CFG_INTERFACE_H_INCLUDE_GUARD
#define LE_CFG_INTERFACE_H_INCLUDE_GUARD


#include "legato.h"


//--------------------------------------------------------------------------------------------------
/**
 * Type for handler called when a server disconnects.
 */
//--------------------------------------------------------------------------------------------------
typedef void (*le_cfg_DisconnectHandler_t)(void *);

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
void le_cfg_ConnectService
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
le_result_t le_cfg_TryConnectService
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
void le_cfg_SetServerDisconnectHandler
(
    le_cfg_DisconnectHandler_t disconnectHandler,
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
void le_cfg_DisconnectService
(
    void
);


//--------------------------------------------------------------------------------------------------
/**
 * Length of the strings used by this API.
 */
//--------------------------------------------------------------------------------------------------
#define LE_CFG_STR_LEN 511

//--------------------------------------------------------------------------------------------------
/**
 * Length of the strings used by this API, including the trailing NULL.
 */
//--------------------------------------------------------------------------------------------------
#define LE_CFG_STR_LEN_BYTES 512

//--------------------------------------------------------------------------------------------------
/**
 * Allowed length of a node name.
 */
//--------------------------------------------------------------------------------------------------
#define LE_CFG_NAME_LEN 127

//--------------------------------------------------------------------------------------------------
/**
 * The node name length, including a trailing NULL.
 */
//--------------------------------------------------------------------------------------------------
#define LE_CFG_NAME_LEN_BYTES 128

//--------------------------------------------------------------------------------------------------
/**
 * Reference to a tree iterator object.
 */
//--------------------------------------------------------------------------------------------------
typedef struct le_cfg_Iterator* le_cfg_IteratorRef_t;


//--------------------------------------------------------------------------------------------------
/**
 * Identifies the data type of node.
 */
//--------------------------------------------------------------------------------------------------
typedef enum
{
    LE_CFG_TYPE_EMPTY = 0,
        ///< A node with no value.
    LE_CFG_TYPE_STRING = 1,
        ///< A string encoded as utf8.
    LE_CFG_TYPE_BOOL = 2,
        ///< Boolean value.
    LE_CFG_TYPE_INT = 3,
        ///< Signed 32-bit.
    LE_CFG_TYPE_FLOAT = 4,
        ///< 64-bit floating point value.
    LE_CFG_TYPE_STEM = 5,
        ///< Non-leaf node, this node is the parent of other nodes.
    LE_CFG_TYPE_DOESNT_EXIST = 6
        ///< Node doesn't exist.
}
le_cfg_nodeType_t;


//--------------------------------------------------------------------------------------------------
/**
 * Reference type used by Add/Remove functions for EVENT 'le_cfg_Change'
 */
//--------------------------------------------------------------------------------------------------
typedef struct le_cfg_ChangeHandler* le_cfg_ChangeHandlerRef_t;


//--------------------------------------------------------------------------------------------------
/**
 * Handler for node change notifications.
 */
//--------------------------------------------------------------------------------------------------
typedef void (*le_cfg_ChangeHandlerFunc_t)
(
    void* contextPtr
        ///<
);

//--------------------------------------------------------------------------------------------------
/**
 * Create a read transaction and open a new iterator for traversing the config tree.
 *
 * This action creates a read lock on the given tree, which will start a read-timeout.
 * Once the read timeout expires, all active read iterators on that tree will be
 * expired and their clients will be killed.
 *
 * @note A tree transaction is global to that tree; a long-held read transaction will block other
 *        user's write transactions from being committed.
 *
 * @return This will return the newly created iterator reference.
 */
//--------------------------------------------------------------------------------------------------
le_cfg_IteratorRef_t le_cfg_CreateReadTxn
(
    const char* LE_NONNULL basePath
        ///< [IN] Path to the location to create the new iterator.
);

//--------------------------------------------------------------------------------------------------
/**
 * Create a write transaction and open a new iterator for both reading and writing.
 *
 * This action creates a write transaction. If the app holds the iterator for
 * longer than the configured write transaction timeout, the iterator will cancel the
 * transaction. Other reads will fail to return data, and all writes will be thrown away.
 *
 * @note A tree transaction is global to that tree; a long-held write transaction will block
 *       other user's write transactions from being started. Other trees in the system
 *       won't be affected.
 *
 * @return This will return a newly created iterator reference.
 */
//--------------------------------------------------------------------------------------------------
le_cfg_IteratorRef_t le_cfg_CreateWriteTxn
(
    const char* LE_NONNULL basePath
        ///< [IN] Path to the location to create the new iterator.
);

//--------------------------------------------------------------------------------------------------
/**
 * Closes the write iterator and commits the write transaction. This updates the config tree
 * with all of the writes that occurred within the iterator.
 *
 * @note This operation will also delete the iterator object.
 */
//--------------------------------------------------------------------------------------------------
void le_cfg_CommitTxn
(
    le_cfg_IteratorRef_t iteratorRef
        ///< [IN] Iterator object to commit.
);

//--------------------------------------------------------------------------------------------------
/**
 * Closes and frees the given iterator object. If the iterator is a write iterator, the transaction
 * will be canceled. If the iterator is a read iterator, the transaction will be closed. No data is
 * written to the tree
 *
 * @note This operation will also delete the iterator object.
 */
//--------------------------------------------------------------------------------------------------
void le_cfg_CancelTxn
(
    le_cfg_IteratorRef_t iteratorRef
        ///< [IN] Iterator object to close.
);

//--------------------------------------------------------------------------------------------------
/**
 * Changes the location of iterator. The path passed can be an absolute or a
 * relative path from the iterators current location.
 *
 * The target node does not need to exist. Writing a value to a non-existent node will
 * automatically create that node and any ancestor nodes (parent, parent's parent, etc.) that
 * also don't exist.
 */
//--------------------------------------------------------------------------------------------------
void le_cfg_GoToNode
(
    le_cfg_IteratorRef_t iteratorRef,
        ///< [IN] Iterator to move.
    const char* LE_NONNULL newPath
        ///< [IN] Absolute or relative path from the current location.
);

//--------------------------------------------------------------------------------------------------
/**
 * Move the iterator to the parent of the current node (moves up the tree).
 *
 * @return Return code will be one of the following values:
 *
 *         - LE_OK        - Commit was completed successfully.
 *         - LE_NOT_FOUND - Current node is the root node: has no parent.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_cfg_GoToParent
(
    le_cfg_IteratorRef_t iteratorRef
        ///< [IN] Iterator to move.
);

//--------------------------------------------------------------------------------------------------
/**
 * Moves the iterator to the the first child of the node from the current location.
 *
 * For read iterators without children, this function will fail. If the iterator is a write
 * iterator, then a new node is automatically created. If this node or newly created
 * children of this node are not written to, then this node will not persist even if the iterator is
 * committed.
 *
 * @return Return code will be one of the following values:
 *
 *         - LE_OK        - Move was completed successfully.
 *         - LE_NOT_FOUND - The given node has no children.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_cfg_GoToFirstChild
(
    le_cfg_IteratorRef_t iteratorRef
        ///< [IN] Iterator object to move.
);

//--------------------------------------------------------------------------------------------------
/**
 * Jumps the iterator to the next child node of the current node. Assuming the following tree:
 *
 * @code
 * baseNode
 *      |
 *      +childA
 *          |
 *          +valueA
 *          |
 *          +valueB
 * @endcode
 *
 * If the iterator is moved to the path, "/baseNode/childA/valueA". After the first
 * GoToNextSibling the iterator will be pointing at valueB. A second call to GoToNextSibling
 * will cause the function to return LE_NOT_FOUND.
 *
 * @return Returns one of the following values:
 *
 *         - LE_OK            - Commit was completed successfully.
 *         - LE_NOT_FOUND     - Iterator has reached the end of the current list of siblings.
 *                              Also returned if the the current node has no siblings.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_cfg_GoToNextSibling
(
    le_cfg_IteratorRef_t iteratorRef
        ///< [IN] Iterator to iterate.
);

//--------------------------------------------------------------------------------------------------
/**
 * Get path to the node where the iterator is currently pointed.
 *
 * Assuming the following tree:
 *
 * @code
 * baseNode
 *      |
 *      +childA
 *          |
 *          +valueA
 *          |
 *          +valueB
 * @endcode
 *
 * If the iterator was currently pointing at valueA, GetPath would return the following path:
 *
 * @code
 * /baseNode/childA/valueA
 * @endcode
 *
 * Optionally, a path to another node can be supplied to this function. So, if the iterator is
 * again on valueA and the relative path ".." is supplied then this function will return the
 * the path relative to the node given:
 *
 * @code
 * /baseNode/childA/
 * @endcode
 *
 * @return - LE_OK            - The write was completed successfully.
 *         - LE_OVERFLOW      - The supplied string buffer was not large enough to hold the value.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_cfg_GetPath
(
    le_cfg_IteratorRef_t iteratorRef,
        ///< [IN] Iterator to move.
    const char* LE_NONNULL path,
        ///< [IN] Path to the target node. Can be an absolute path, or
        ///< a path relative from the iterator's current position.
    char* pathBuffer,
        ///< [OUT] Absolute path to the iterator's current node.
    size_t pathBufferSize
        ///< [IN]
);

//--------------------------------------------------------------------------------------------------
/**
 * Get the data type of node where the iterator is currently pointing.
 *
 * @return le_cfg_nodeType_t value indicating the stored value.
 */
//--------------------------------------------------------------------------------------------------
le_cfg_nodeType_t le_cfg_GetNodeType
(
    le_cfg_IteratorRef_t iteratorRef,
        ///< [IN] Iterator object to use to read from the tree.
    const char* LE_NONNULL path
        ///< [IN] Path to the target node. Can be an absolute path, or
        ///< a path relative from the iterator's current position.
);

//--------------------------------------------------------------------------------------------------
/**
 * Get the name of the node where the iterator is currently pointing.
 *
 * @return - LE_OK       Read was completed successfully.
 *         - LE_OVERFLOW Supplied string buffer was not large enough to hold the value.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_cfg_GetNodeName
(
    le_cfg_IteratorRef_t iteratorRef,
        ///< [IN] Iterator object to use to read from the tree.
    const char* LE_NONNULL path,
        ///< [IN] Path to the target node. Can be an absolute path, or
        ///< a path relative from the iterator's current position.
    char* name,
        ///< [OUT] Read the name of the node object.
    size_t nameSize
        ///< [IN]
);

//--------------------------------------------------------------------------------------------------
/**
 * Add handler function for EVENT 'le_cfg_Change'
 *
 * This event provides information on changes to the given node object, or any of it's children,
 * where a change could be either a read, write, create or delete operation.
 */
//--------------------------------------------------------------------------------------------------
le_cfg_ChangeHandlerRef_t le_cfg_AddChangeHandler
(
    const char* LE_NONNULL newPath,
        ///< [IN] Path to the object to watch.
    le_cfg_ChangeHandlerFunc_t handlerPtr,
        ///< [IN] Handler to receive change notification
    void* contextPtr
        ///< [IN]
);

//--------------------------------------------------------------------------------------------------
/**
 * Remove handler function for EVENT 'le_cfg_Change'
 */
//--------------------------------------------------------------------------------------------------
void le_cfg_RemoveChangeHandler
(
    le_cfg_ChangeHandlerRef_t handlerRef
        ///< [IN]
);

//--------------------------------------------------------------------------------------------------
/**
 * Deletes the node specified by the path. If the node doesn't exist, nothing happens. All child
 * nodes are also deleted.
 *
 * If the path is empty, the iterator's current node is deleted.
 *
 * This function is only valid during a write transaction.
 */
//--------------------------------------------------------------------------------------------------
void le_cfg_DeleteNode
(
    le_cfg_IteratorRef_t iteratorRef,
        ///< [IN] Iterator to use as a basis for the transaction.
    const char* LE_NONNULL path
        ///< [IN] Path to the target node. Can be an absolute path, or
        ///< a path relative from the iterator's current position.
);

//--------------------------------------------------------------------------------------------------
/**
 * Check if the given node is empty. A node is also considered empty if it doesn't yet exist. A
 * node is also considered empty if it has no value or is a stem with no children.
 *
 * If the path is empty, the iterator's current node is queried for emptiness.
 *
 * Valid for both read and write transactions.
 *
 * @return A true if the node is considered empty, false if not.
 */
//--------------------------------------------------------------------------------------------------
bool le_cfg_IsEmpty
(
    le_cfg_IteratorRef_t iteratorRef,
        ///< [IN] Iterator to use as a basis for the transaction.
    const char* LE_NONNULL path
        ///< [IN] Path to the target node. Can be an absolute path, or
        ///< a path relative from the iterator's current position.
);

//--------------------------------------------------------------------------------------------------
/**
 * Clears out the node's value. If the node doesn't exist it will be created, and have no value.
 *
 * If the path is empty, the iterator's current node will be cleared. If the node is a stem
 * then all children will be removed from the tree.
 *
 * Only valid during a write transaction.
 */
//--------------------------------------------------------------------------------------------------
void le_cfg_SetEmpty
(
    le_cfg_IteratorRef_t iteratorRef,
        ///< [IN] Iterator to use as a basis for the transaction.
    const char* LE_NONNULL path
        ///< [IN] Path to the target node. Can be an absolute path, or
        ///< a path relative from the iterator's current position.
);

//--------------------------------------------------------------------------------------------------
/**
 * Checks to see if a given node in the config tree exists.
 *
 * @return True if the specified node exists in the tree. False if not.
 */
//--------------------------------------------------------------------------------------------------
bool le_cfg_NodeExists
(
    le_cfg_IteratorRef_t iteratorRef,
        ///< [IN] Iterator to use as a basis for the transaction.
    const char* LE_NONNULL path
        ///< [IN] Path to the target node. Can be an absolute path, or
        ///< a path relative from the iterator's current position.
);

//--------------------------------------------------------------------------------------------------
/**
 * Reads a string value from the config tree. If the value isn't a string, or if the node is
 * empty or doesn't exist, the default value will be returned.
 *
 * Valid for both read and write transactions.
 *
 * If the path is empty, the iterator's current node will be read.
 *
 * @return - LE_OK       - Read was completed successfully.
 *         - LE_OVERFLOW - Supplied string buffer was not large enough to hold the value.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_cfg_GetString
(
    le_cfg_IteratorRef_t iteratorRef,
        ///< [IN] Iterator to use as a basis for the transaction.
    const char* LE_NONNULL path,
        ///< [IN] Path to the target node. Can be an absolute path,
        ///< or a path relative from the iterator's current
        ///< position.
    char* value,
        ///< [OUT] Buffer to write the value into.
    size_t valueSize,
        ///< [IN]
    const char* LE_NONNULL defaultValue
        ///< [IN] Default value to use if the original can't be
        ///<   read.
);

//--------------------------------------------------------------------------------------------------
/**
 * Writes a string value to the config tree. Only valid during a write
 * transaction.
 *
 * If the path is empty, the iterator's current node will be set.
 */
//--------------------------------------------------------------------------------------------------
void le_cfg_SetString
(
    le_cfg_IteratorRef_t iteratorRef,
        ///< [IN] Iterator to use as a basis for the transaction.
    const char* LE_NONNULL path,
        ///< [IN] Path to the target node. Can be an absolute path, or
        ///< a path relative from the iterator's current position.
    const char* LE_NONNULL value
        ///< [IN] Value to write.
);

//--------------------------------------------------------------------------------------------------
/**
 * Reads a signed integer value from the config tree.
 *
 * If the underlying value is not an integer, the default value will be returned instead. The
 * default value is also returned if the node does not exist or if it's empty.
 *
 * If the value is a floating point value, then it will be rounded and returned as an integer.
 *
 * Valid for both read and write transactions.
 *
 * If the path is empty, the iterator's current node will be read.
 */
//--------------------------------------------------------------------------------------------------
int32_t le_cfg_GetInt
(
    le_cfg_IteratorRef_t iteratorRef,
        ///< [IN] Iterator to use as a basis for the transaction.
    const char* LE_NONNULL path,
        ///< [IN] Path to the target node. Can be an absolute path, or
        ///< a path relative from the iterator's current position.
    int32_t defaultValue
        ///< [IN] Default value to use if the original can't be
        ///<   read.
);

//--------------------------------------------------------------------------------------------------
/**
 * Writes a signed integer value to the config tree. Only valid during a
 * write transaction.
 *
 * If the path is empty, the iterator's current node will be set.
 */
//--------------------------------------------------------------------------------------------------
void le_cfg_SetInt
(
    le_cfg_IteratorRef_t iteratorRef,
        ///< [IN] Iterator to use as a basis for the transaction.
    const char* LE_NONNULL path,
        ///< [IN] Path to the target node. Can be an absolute path, or
        ///< a path relative from the iterator's current position.
    int32_t value
        ///< [IN] Value to write.
);

//--------------------------------------------------------------------------------------------------
/**
 * Reads a 64-bit floating point value from the config tree.
 *
 * If the value is an integer then the value will be promoted to a float. Otherwise, if the
 * underlying value is not a float or integer, the default value will be returned.
 *
 * If the path is empty, the iterator's current node will be read.
 *
 * @note Floating point values will only be stored up to 6 digits of precision.
 */
//--------------------------------------------------------------------------------------------------
double le_cfg_GetFloat
(
    le_cfg_IteratorRef_t iteratorRef,
        ///< [IN] Iterator to use as a basis for the transaction.
    const char* LE_NONNULL path,
        ///< [IN] Path to the target node. Can be an absolute path, or
        ///< a path relative from the iterator's current position.
    double defaultValue
        ///< [IN] Default value to use if the original can't be
        ///<   read.
);

//--------------------------------------------------------------------------------------------------
/**
 * Writes a 64-bit floating point value to the config tree. Only valid
 * during a write transaction.
 *
 * If the path is empty, the iterator's current node will be set.
 *
 * @note Floating point values will only be stored up to 6 digits of precision.
 */
//--------------------------------------------------------------------------------------------------
void le_cfg_SetFloat
(
    le_cfg_IteratorRef_t iteratorRef,
        ///< [IN] Iterator to use as a basis for the transaction.
    const char* LE_NONNULL path,
        ///< [IN] Path to the target node. Can be an absolute path, or
        ///< a path relative from the iterator's current position.
    double value
        ///< [IN] Value to write.
);

//--------------------------------------------------------------------------------------------------
/**
 * Reads a value from the tree as a boolean. If the node is empty or doesn't exist, the default
 * value is returned. Default value is also returned if the node is a different type than
 * expected.
 *
 * Valid for both read and write transactions.
 *
 * If the path is empty, the iterator's current node will be read.
 */
//--------------------------------------------------------------------------------------------------
bool le_cfg_GetBool
(
    le_cfg_IteratorRef_t iteratorRef,
        ///< [IN] Iterator to use as a basis for the transaction.
    const char* LE_NONNULL path,
        ///< [IN] Path to the target node. Can be an absolute path, or
        ///< a path relative from the iterator's current position.
    bool defaultValue
        ///< [IN] Default value to use if the original can't be
        ///<   read.
);

//--------------------------------------------------------------------------------------------------
/**
 * Writes a boolean value to the config tree. Only valid during a write
 * transaction.
 *
 * If the path is empty, the iterator's current node will be set.
 */
//--------------------------------------------------------------------------------------------------
void le_cfg_SetBool
(
    le_cfg_IteratorRef_t iteratorRef,
        ///< [IN] Iterator to use as a basis for the transaction.
    const char* LE_NONNULL path,
        ///< [IN] Path to the target node. Can be an absolute path, or
        ///< a path relative from the iterator's current position.
    bool value
        ///< [IN] Value to write.
);

//--------------------------------------------------------------------------------------------------
/**
 * Deletes the node specified by the path. If the node doesn't exist, nothing happens. All child
 * nodes are also deleted.
 */
//--------------------------------------------------------------------------------------------------
void le_cfg_QuickDeleteNode
(
    const char* LE_NONNULL path
        ///< [IN] Path to the node to delete.
);

//--------------------------------------------------------------------------------------------------
/**
 * Clears the current value of a node. If the node doesn't currently exist then it is created as a
 * new empty node.
 */
//--------------------------------------------------------------------------------------------------
void le_cfg_QuickSetEmpty
(
    const char* LE_NONNULL path
        ///< [IN] Absolute or relative path to read from.
);

//--------------------------------------------------------------------------------------------------
/**
 * Reads a string value from the config tree. If the value isn't a string, or if the node is
 * empty or doesn't exist, the default value will be returned.
 *
 * @return - LE_OK       - Commit was completed successfully.
 *         - LE_OVERFLOW - Supplied string buffer was not large enough to hold the value.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_cfg_QuickGetString
(
    const char* LE_NONNULL path,
        ///< [IN] Path to read from.
    char* value,
        ///< [OUT] Value read from the requested node.
    size_t valueSize,
        ///< [IN]
    const char* LE_NONNULL defaultValue
        ///< [IN] Default value to use if the original can't be read.
);

//--------------------------------------------------------------------------------------------------
/**
 * Writes a string value to the config tree.
 */
//--------------------------------------------------------------------------------------------------
void le_cfg_QuickSetString
(
    const char* LE_NONNULL path,
        ///< [IN] Path to the value to write.
    const char* LE_NONNULL value
        ///< [IN] Value to write.
);

//--------------------------------------------------------------------------------------------------
/**
 * Reads a signed integer value from the config tree. If the value is a floating point
 * value, then it will be rounded and returned as an integer. Otherwise If the underlying value is
 * not an integer or a float, the default value will be returned instead.
 *
 * If the value is empty or the node doesn't exist, the default value is returned instead.
 */
//--------------------------------------------------------------------------------------------------
int32_t le_cfg_QuickGetInt
(
    const char* LE_NONNULL path,
        ///< [IN] Path to the value to write.
    int32_t defaultValue
        ///< [IN] Default value to use if the original can't be read.
);

//--------------------------------------------------------------------------------------------------
/**
 * Writes a signed integer value to the config tree.
 */
//--------------------------------------------------------------------------------------------------
void le_cfg_QuickSetInt
(
    const char* LE_NONNULL path,
        ///< [IN] Path to the value to write.
    int32_t value
        ///< [IN] Value to write.
);

//--------------------------------------------------------------------------------------------------
/**
 * Reads a 64-bit floating point value from the config tree. If the value is an integer,
 * then it is promoted to a float. Otherwise, if the underlying value is not a float, or an
 * integer the default value will be returned.
 *
 * If the value is empty or the node doesn't exist, the default value is returned.
 *
 * @note Floating point values will only be stored up to 6 digits of precision.
 */
//--------------------------------------------------------------------------------------------------
double le_cfg_QuickGetFloat
(
    const char* LE_NONNULL path,
        ///< [IN] Path to the value to write.
    double defaultValue
        ///< [IN] Default value to use if the original can't be read.
);

//--------------------------------------------------------------------------------------------------
/**
 * Writes a 64-bit floating point value to the config tree.
 *
 * @note Floating point values will only be stored up to 6 digits of precision.
 */
//--------------------------------------------------------------------------------------------------
void le_cfg_QuickSetFloat
(
    const char* LE_NONNULL path,
        ///< [IN] Path to the value to write.
    double value
        ///< [IN] Value to write.
);

//--------------------------------------------------------------------------------------------------
/**
 * Reads a value from the tree as a boolean. If the node is empty or doesn't exist, the default
 * value is returned. This is also true if the node is a different type than expected.
 *
 * If the value is empty or the node doesn't exist, the default value is returned instead.
 */
//--------------------------------------------------------------------------------------------------
bool le_cfg_QuickGetBool
(
    const char* LE_NONNULL path,
        ///< [IN] Path to the value to write.
    bool defaultValue
        ///< [IN] Default value to use if the original can't be read.
);

//--------------------------------------------------------------------------------------------------
/**
 * Writes a boolean value to the config tree.
 */
//--------------------------------------------------------------------------------------------------
void le_cfg_QuickSetBool
(
    const char* LE_NONNULL path,
        ///< [IN] Path to the value to write.
    bool value
        ///< [IN] Value to write.
);

#endif // LE_CFG_INTERFACE_H_INCLUDE_GUARD