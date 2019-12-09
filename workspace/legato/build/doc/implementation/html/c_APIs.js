var c_APIs =
[
    [ "Overview", "c_APIs.html#cApiOverview", [
      [ "Design", "c_APIs.html#Object-Oriented", null ],
      [ "Types", "c_APIs.html#Opaque", null ],
      [ "and Event-Driven Programs", "c_APIs.html#Handlers", null ]
    ] ],
    [ "Build Configuration", "c_le_build_cfg.html", [
      [ "LE_MEM_TRACE", "c_le_build_cfg.html#bld_cfg_mem_trace", null ],
      [ "LE_MEM_VALGRIND", "c_le_build_cfg.html#bld_cfg_mem_valgrind", null ],
      [ "LE_SMACK_DISABLE", "c_le_build_cfg.html#bld_cfg_disable_SMACK", null ],
      [ "LE_SEGV_HANDLER_DISABLE", "c_le_build_cfg.html#bld_cfg_disable_SEGV_HANDLER", null ]
    ] ],
    [ "Basic Type and Constant Definitions", "c_basics.html", null ],
    [ "Command Line Arguments API", "c_args.html", [
      [ "Argument Access By Index", "c_args.html#c_args_by_index", null ],
      [ "Options", "c_args.html#c_args_options", null ],
      [ "Positional Arguments", "c_args.html#c_args_positional", null ],
      [ "Argument Scanner", "c_args.html#c_args_scanner", [
        [ "Usage", "c_args.html#c_args_scanner_usage", null ],
        [ "Example", "c_args.html#c_args_parser_example", null ],
        [ "Error Handling", "c_args.html#c_args_parser_errorHandling", null ]
      ] ],
      [ "Writing Your Own main()?", "c_args.html#c_args_writingYourOwnMain", null ]
    ] ],
    [ "Atomic File Operation API", "c_atomFile.html", [
      [ "Atomic File Operations", "c_atomFile.html#c_atomFile_operation", null ],
      [ "Streams", "c_atomFile.html#c_atomFile_streams", null ],
      [ "Non-blocking", "c_atomFile.html#c_atomFile_nonblock", null ],
      [ "Multiple Threads", "c_atomFile.html#c_atomFile_threading", null ],
      [ "Limitations", "c_atomFile.html#c_atomFile_limitations", null ]
    ] ],
    [ "CRC32 API", "c_crc.html", [
      [ "Computing a CRC32", "c_crc.html#Crc32", null ]
    ] ],
    [ "Directory API", "c_dir.html", [
      [ "Creating Directories", "c_dir.html#c_dir_create", null ],
      [ "Removing Directories", "c_dir.html#c_dir_delete", null ],
      [ "Reading Directories", "c_dir.html#c_dir_read", null ]
    ] ],
    [ "Doubly Linked List API", "c_doublyLinkedList.html", [
      [ "Creating and Initializing Lists", "c_doublyLinkedList.html#dls_createList", null ],
      [ "Creating and Accessing Nodes", "c_doublyLinkedList.html#dls_createNode", null ],
      [ "Adding Links to a List", "c_doublyLinkedList.html#dls_add", null ],
      [ "Removing Links from a List", "c_doublyLinkedList.html#dls_remove", null ],
      [ "Accessing Links in a List", "c_doublyLinkedList.html#dls_peek", null ],
      [ "Swapping Links", "c_doublyLinkedList.html#dls_swap", null ],
      [ "Querying List Status", "c_doublyLinkedList.html#dls_query", null ],
      [ "Queues and Stacks", "c_doublyLinkedList.html#dls_fifo", null ],
      [ "Thread Safety and Re-Entrancy", "c_doublyLinkedList.html#dls_synch", null ]
    ] ],
    [ "Dynamic Memory Allocation API", "c_memory.html", [
      [ "Overview", "c_memory.html#mem_overview", null ],
      [ "Creating a Pool", "c_memory.html#mem_creating", null ],
      [ "Allocating From a Pool", "c_memory.html#mem_allocating", null ],
      [ "Releasing Back Into a Pool", "c_memory.html#mem_releasing", null ],
      [ "Reference Counting", "c_memory.html#mem_ref_counting", null ],
      [ "Destructors", "c_memory.html#mem_destructors", null ],
      [ "Statistics", "c_memory.html#mem_stats", null ],
      [ "Diagnostics", "c_memory.html#mem_diagnostics", null ],
      [ "Multi-Threading", "c_memory.html#mem_threading", null ],
      [ "Managing Pool Sizes", "c_memory.html#mem_pool_sizes", null ],
      [ "Sub-Pools", "c_memory.html#mem_sub_pools", null ]
    ] ],
    [ "Event Loop API", "c_eventLoop.html", [
      [ "Deferred Function Calls", "c_eventLoop.html#c_event_deferredFunctionCalls", null ],
      [ "Dispatching Function Execution to Other Threads", "c_eventLoop.html#c_event_dispatchingToOtherThreads", null ],
      [ "Publish-Subscribe Events", "c_eventLoop.html#c_event_publishSubscribe", null ],
      [ "Layered Publish-Subscribe Handlers", "c_eventLoop.html#c_event_layeredPublishSubscribe", null ],
      [ "Event Reports Containing Reference-Counted Objects", "c_eventLoop.html#c_event_reportingRefCountedObjects", null ],
      [ "Miscellaneous Multithreading Topics", "c_eventLoop.html#c_event_miscThreadingTopics", null ],
      [ "Integrating with Legacy POSIX Code", "c_eventLoop.html#c_event_integratingLegacyPosix", null ],
      [ "Troubleshooting", "c_eventLoop.html#c_event_troubleshooting", null ]
    ] ],
    [ "File Descriptor Monitor API", "c_fdMonitor.html", [
      [ "Start/Stop Monitoring", "c_fdMonitor.html#c_fdMonitorStartStop", null ],
      [ "Event Types", "c_fdMonitor.html#c_fdMonitorEvents", null ],
      [ "FD Types", "c_fdMonitor.html#c_fdTypes", [
        [ "Files", "c_fdMonitor.html#c_fdTypes_files", null ],
        [ "Pipes", "c_fdMonitor.html#c_fdTypes_pipes", null ],
        [ "Sockets", "c_fdMonitor.html#c_fdTypes_sockets", null ],
        [ "Terminals and Pseudo-Terminals", "c_fdMonitor.html#c_fdTypes_terminals", null ]
      ] ],
      [ "Handler Functions", "c_fdMonitor.html#c_fdMonitorHandlers", null ],
      [ "Enable/Disable Event Monitoring", "c_fdMonitor.html#c_fdMonitorEnableDisable", null ],
      [ "Handler Function Context", "c_fdMonitor.html#c_fdMonitorHandlerContext", null ],
      [ "Power Management", "c_fdMonitor.html#c_fdMonitorPowerManagement", null ],
      [ "Threading", "c_fdMonitor.html#c_fdMonitorThreading", null ],
      [ "Troubleshooting", "c_fdMonitor.html#c_fdMonitorTroubleshooting", null ]
    ] ],
    [ "File Locking API", "c_flock.html", [
      [ "Co-operative File Locking", "c_flock.html#c_flock_cooperative", null ],
      [ "Locking Files", "c_flock.html#c_flock_locks", null ],
      [ "Streams", "c_flock.html#c_flock_streams", null ],
      [ "Non-blocking", "c_flock.html#c_flock_nonblock", null ],
      [ "Multiple Threads", "c_flock.html#c_flock_threads", null ],
      [ "Replicating File Descriptors", "c_flock.html#c_flock_replicateFd", null ],
      [ "Limitations", "c_flock.html#c_flock_limitations", null ]
    ] ],
    [ "File System service", "c_fs.html", null ],
    [ "HashMap API", "c_hashmap.html", [
      [ "Creating a HashMap", "c_hashmap.html#c_hashmap_create", null ],
      [ "Adding key-value pairs", "c_hashmap.html#c_hashmap_insert", [
        [ "Tip", "c_hashmap.html#c_hashmap_tips", null ]
      ] ],
      [ "Iterating over a map", "c_hashmap.html#c_hashmap_iterating", null ],
      [ "Tracing a map", "c_hashmap.html#c_hashmap_tracing", null ]
    ] ],
    [ "Hex string API", "c_hex.html", [
      [ "Conversion", "c_hex.html#hex_conversion", null ]
    ] ],
    [ "JSON Parsing API", "c_json.html", [
      [ "Starting and Stopping Parsing", "c_json.html#c_json_start", null ],
      [ "Event Handling", "c_json.html#c_json_events", null ],
      [ "Context", "c_json.html#c_json_context", null ],
      [ "Error Handling", "c_json.html#c_json_errors", null ],
      [ "Other Functions", "c_json.html#c_json_otherFunctions", null ],
      [ "Example", "c_json.html#c_json_example", null ],
      [ "Multi-Threading", "c_json.html#c_json_threads", null ]
    ] ],
    [ "Logging API", "c_logging.html", [
      [ "Logging Basics", "c_logging.html#c_log_logging", [
        [ "Levels", "c_logging.html#c_log_levels", null ],
        [ "Standard Out and Standard Error in Syslog", "c_logging.html#c_log_basic_defaultSyslog", null ],
        [ "Basic Logging", "c_logging.html#c_log_basic_logging", null ],
        [ "Conditional Logging", "c_logging.html#c_log_conditional_logging", null ],
        [ "Fatal Errors", "c_logging.html#c_log_loging_fatals", null ],
        [ "Tracing", "c_logging.html#c_log_tracing", null ],
        [ "Result Code Text", "c_logging.html#c_log_resultTxt", null ]
      ] ],
      [ "Log Controls", "c_logging.html#c_log_controlling", [
        [ "Log Control Tool", "c_logging.html#c_log_control_tool", null ],
        [ "Log Control Configuration Settings", "c_logging.html#c_log_control_config", null ],
        [ "Environment Variables", "c_logging.html#c_log_control_environment_vars", [
          [ "LE_LOG_LEVEL", "c_logging.html#c_log_control_env_level", null ],
          [ "LE_LOG_TRACE", "c_logging.html#c_log_control_env_trace", null ]
        ] ],
        [ "Programmatic Log Control", "c_logging.html#c_log_control_functions", null ]
      ] ],
      [ "Log Formats", "c_logging.html#c_log_format", null ],
      [ "App Crash Logs", "c_logging.html#c_log_debugFiles", null ]
    ] ],
    [ "Low-level Messaging API", "c_messaging.html", [
      [ "Interaction Model", "c_messaging.html#c_messagingInteractionModel", null ],
      [ "Addressing", "c_messaging.html#c_messagingAddressing", null ],
      [ "Protocols", "c_messaging.html#c_messagingProtocols", null ],
      [ "Client Usage Model", "c_messaging.html#c_messagingClientUsage", [
        [ "Sending a Message", "c_messaging.html#c_messagingClientSending", null ],
        [ "Receiving a Non-Response Message", "c_messaging.html#c_messagingClientReceiving", null ],
        [ "Closing Sessions", "c_messaging.html#c_messagingClientClosing", null ],
        [ "Multithreading", "c_messaging.html#c_messagingClientMultithreading", null ],
        [ "Sample Code", "c_messaging.html#c_messagingClientExample", null ]
      ] ],
      [ "Server Usage Model", "c_messaging.html#c_messagingServerUsage", [
        [ "Processing Messages from Clients", "c_messaging.html#c_messagingServerProcessingMessages", null ],
        [ "Sending Non-Response Messages to Clients", "c_messaging.html#c_messagingServerSendingNonResponse", null ],
        [ "Cleaning up when Sessions Close", "c_messaging.html#c_messagingServerCleanUp", null ],
        [ "Removing Service", "c_messaging.html#c_messagingRemovingService", null ],
        [ "Multithreading", "c_messaging.html#c_messagingServerMultithreading", null ],
        [ "Sample Code", "c_messaging.html#c_messagingServerExample", null ]
      ] ],
      [ "Start Up Sequencing", "c_messaging.html#c_messagingStartUp", null ],
      [ "Memory Management", "c_messaging.html#c_messagingMemoryManagement", null ],
      [ "Security", "c_messaging.html#c_messagingSecurity", null ],
      [ "Get Client Info", "c_messaging.html#c_messagingGetClientInfo", null ],
      [ "Sending File Descriptors", "c_messaging.html#c_messagingSendingFileDescriptors", null ],
      [ "Future Enhancements", "c_messaging.html#c_messagingFutureEnhancements", null ],
      [ "Design Notes", "c_messaging.html#c_messagingDesignNotes", null ],
      [ "Troubleshooting", "c_messaging.html#c_messagingTroubleshooting", null ]
    ] ],
    [ "Mutex API", "c_mutex.html", [
      [ "Creating a Mutex", "c_mutex.html#c_mutex_create", null ],
      [ "Using a Mutex", "c_mutex.html#c_mutex_locking", null ],
      [ "Deleting a Mutex", "c_mutex.html#c_mutex_delete", null ],
      [ "Diagnostics", "c_mutex.html#c_mutex_diagnostics", null ]
    ] ],
    [ "Low-level Pack/Unpack API", "c_pack.html", null ],
    [ "Path API", "c_path.html", [
      [ "Directory and Basename", "c_path.html#c_path_dirAndBasename", null ],
      [ "Thread Safety", "c_path.html#c_path_threads", null ]
    ] ],
    [ "Path Iterator API", "c_pathIter.html", [
      [ "Create a Path Iterator", "c_pathIter.html#c_pathIter_create", null ],
      [ "Iterating a Path", "c_pathIter.html#c_pathIter_iterate", null ],
      [ "Absolute versus Relative Paths", "c_pathIter.html#c_pathIter_absoluteRelative", null ],
      [ "Modifying Paths", "c_pathIter.html#c_pathIter_modifyPath", null ]
    ] ],
    [ "Print APIs", "c_print.html", null ],
    [ "Random Number API", "c_rand.html", null ],
    [ "Safe References API", "c_safeRef.html", [
      [ "Create Safe Reference", "c_safeRef.html#c_safeRef_create", null ],
      [ "Lookup Pointer", "c_safeRef.html#c_safeRef_lookup", null ],
      [ "Delete Safe Reference", "c_safeRef.html#c_safeRef_delete", null ],
      [ "Create Referece Map", "c_safeRef.html#c_safeRef_map", null ],
      [ "Multithreading", "c_safeRef.html#c_safeRef_multithreading", null ],
      [ "Sample Code", "c_safeRef.html#c_safeRef_example", null ]
    ] ],
    [ "Semaphore API", "c_semaphore.html", [
      [ "Creating a Semaphore", "c_semaphore.html#create_semaphore", null ],
      [ "Using a Semaphore", "c_semaphore.html#use_semaphore", null ],
      [ "Deleting a Semaphore", "c_semaphore.html#delete_semaphore", null ],
      [ "Diagnostics", "c_semaphore.html#diagnostics_semaphore", null ]
    ] ],
    [ "Signals API", "c_signals.html", [
      [ "Signal Event Handlers", "c_signals.html#c_signals_eventHandlers", null ],
      [ "Mixing Asynchronous Signal Handlers with Synchronous Signal Event Handlers", "c_signals.html#c_signals_mixedHandlers", null ],
      [ "Multi-Threading Support", "c_signals.html#c_signals_multiThread", null ],
      [ "Limitations and Warnings", "c_signals.html#c_signals_limitations", null ],
      [ "Blocking signals", "c_signals.html#c_signals_blocking", null ]
    ] ],
    [ "Singly Linked List API", "c_singlyLinkedList.html", [
      [ "Creating and Initializing Lists", "c_singlyLinkedList.html#sls_createList", null ],
      [ "Creating and Accessing Nodes", "c_singlyLinkedList.html#sls_createNode", null ],
      [ "Adding Links to a List", "c_singlyLinkedList.html#sls_add", null ],
      [ "Removing Links from a List", "c_singlyLinkedList.html#sls_remove", null ],
      [ "Accessing Links in a List", "c_singlyLinkedList.html#sls_peek", null ],
      [ "Querying List Status", "c_singlyLinkedList.html#sls_query", null ],
      [ "Queues and Stacks", "c_singlyLinkedList.html#sls_fifo", null ],
      [ "Thread Safety and Re-Entrancy", "c_singlyLinkedList.html#sls_synch", null ]
    ] ],
    [ "System Clock API", "c_clock.html", [
      [ "Getting/Setting Time", "c_clock.html#clk_time", null ],
      [ "Operations on Time Values", "c_clock.html#clk_values", null ],
      [ "Converting Time to/from Other Formats", "c_clock.html#clk_convert", null ]
    ] ],
    [ "Thread Control API", "c_threading.html", [
      [ "Creating a Thread", "c_threading.html#threadCreating", null ],
      [ "Terminating a Thread", "c_threading.html#threadTerminating", null ],
      [ "Joining", "c_threading.html#threadJoining", null ],
      [ "Thread-Local Data", "c_threading.html#threadLocalData", null ],
      [ "Thread Synchronization", "c_threading.html#threadSynchronization", null ],
      [ "Thread Destructors", "c_threading.html#threadDestructors", null ],
      [ "Using Legato APIs from Non-Legato Threads", "c_threading.html#threadLegatoizing", null ]
    ] ],
    [ "Timer API", "c_timer.html", [
      [ "Creating/Deleting Timer Objects", "c_timer.html#timer_objects", null ],
      [ "Using Timers", "c_timer.html#timer_usage", null ],
      [ "Thread Support", "c_timer.html#le_timer_thread", null ],
      [ "Suspend Support", "c_timer.html#le_timer_suspend", null ],
      [ "Fatal Errors", "c_timer.html#timer_errors", null ],
      [ "Troubleshooting", "c_timer.html#timer_troubleshooting", null ]
    ] ],
    [ "Unit Testing API", "c_test.html", [
      [ "Modes of Operation", "c_test.html#c_test_modes", null ],
      [ "Setting Up the Test Framework", "c_test.html#c_test_setup", null ],
      [ "Performing Tests", "c_test.html#c_test_testing", null ],
      [ "Exiting a Test Program", "c_test.html#c_test_exit", null ],
      [ "Test Results", "c_test.html#c_test_result", null ],
      [ "Multi-Threaded Tests", "c_test.html#c_test_multiThread", null ],
      [ "Multi-Process Tests", "c_test.html#c_test_multiProcess", null ]
    ] ],
    [ "UTF-8 String Handling API", "c_utf8.html", [
      [ "UTF-8 Encoding", "c_utf8.html#utf8_encoding", null ],
      [ "Copy and Append", "c_utf8.html#utf8_copy", null ],
      [ "Truncation", "c_utf8.html#utf8_trunc", null ],
      [ "String Lengths", "c_utf8.html#utf8_stringLength", null ],
      [ "Character Lengths", "c_utf8.html#utf8_charLength", null ],
      [ "Checking UTF-8 Format", "c_utf8.html#utf8_format", null ],
      [ "String Parsing", "c_utf8.html#utf8_parsing", null ]
    ] ],
    [ "tty API", "c_tty.html", [
      [ "Open/Close serial ports", "c_tty.html#c_tty_open_close", null ],
      [ "Settings serial ports", "c_tty.html#c_tty_settings", null ]
    ] ]
];