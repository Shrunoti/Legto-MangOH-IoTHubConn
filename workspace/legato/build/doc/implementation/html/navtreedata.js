var NAVTREE =
[
  [ "LEGATO", "index.html", [
    [ "Docs", "index.html", "index" ],
    [ "Legato Service Directory Protocol", "serviceDirectoryProtocol.html", [
      [ "Introduction", "serviceDirectoryProtocol.html#serviceDirectoryProtocol_Intro", null ],
      [ "Sockets and Credentials", "serviceDirectoryProtocol.html#serviceDirectoryProtocol_SocketsAndCredentials", null ],
      [ "Server-to-Directory Communication", "serviceDirectoryProtocol.html#serviceDirectoryProtocol_Servers", null ],
      [ "Client-to-Directory Communication", "serviceDirectoryProtocol.html#serviceDirectoryProtocol_Clients", null ],
      [ "Byte Ordering and Packing", "serviceDirectoryProtocol.html#serviceDirectoryProtocol_Packing", null ]
    ] ],
    [ "mangOH Developers", "mangohDevelopers.html", [
      [ "What was installed", "mangohDevelopers.html#mangohDevelopers_installed", null ],
      [ "What helloWorld used", "mangohDevelopers.html#mangohDevelopers_helloWorld", null ],
      [ "Next Steps", "mangohDevelopers.html#mangohDevelopers_nextSteps", null ]
    ] ],
    [ "Software Design Documents", "c_sdd.html", "c_sdd" ],
    [ "Unix Domain Sockets API", "c_unixSockets.html", [
      [ "Table of Contents", "c_unixSockets.html#toc", null ],
      [ "Introduction", "c_unixSockets.html#c_unixSocketsIntro", null ],
      [ "Creating a Single Socket", "c_unixSockets.html#c_unixSocketsCreatingSingle", null ],
      [ "Creating a Pair of Connected Sockets", "c_unixSockets.html#c_unixSocketsCreatingPair", null ],
      [ "Listening and Connecting", "c_unixSockets.html#c_unixSocketsConnecting", null ],
      [ "Sending and Receiving", "c_unixSockets.html#c_unixSocketsSendingAndReceiving", null ],
      [ "Getting Credentials Directly from a Connected Socket", "c_unixSockets.html#c_unixSocketsGettingCredentialsDirect", null ],
      [ "Deleting a Socket", "c_unixSockets.html#c_unixSocketsDeleting", null ]
    ] ],
    [ "delta patch overview.", "Delta_Patch.html", [
      [ "mkPatch Presensation", "Delta_Patch.html#mkPatch_Presentation", [
        [ "mkPatch", "Delta_Patch.html#mkPatch_tool", null ],
        [ "Examples", "Delta_Patch.html#mkPatch_Examples", null ]
      ] ],
      [ "Apply a delta patch", "Delta_Patch.html#ApplyDeltaPatch", null ]
    ] ],
    [ "Ninja Script Generator", "mkTools_NinjaScriptGenerator.html", null ],
    [ "Conceptual Model", "mkTools_ConceptualModel.html", null ],
    [ "Design of the mk tools (mksys, mkapp, mkcomp, and mkexe)", "mkToolsDesign.html", [
      [ "Design Decomposition", "mkToolsDesign.html#mkToolsDesign_Decomposition", [
        [ "Parser", "mkToolsDesign.html#mkToolsDesign_parser", null ],
        [ "Parse Trees", "mkToolsDesign.html#mkToolsDesign_parseTrees", null ],
        [ "Modellers", "mkToolsDesign.html#mkToolsDesign_modellers", null ],
        [ "Conceptual Model", "mkToolsDesign.html#mkToolsDesign_conceptualModel", null ],
        [ "Build Script Generator", "mkToolsDesign.html#mkToolsDesign_buildScriptGenerator", null ],
        [ "App Configuration Generator", "mkToolsDesign.html#mkToolsDesign_appConfigGenerator", null ],
        [ "Code Generator", "mkToolsDesign.html#mkToolsDesign_codeGenerator", null ],
        [ "Command-Line Interpreter", "mkToolsDesign.html#mkToolsDesign_commandLineInterpreter", null ],
        [ "Ninja Build Tool", "mkToolsDesign.html#mkToolsDesign_ninjaBuildTool", null ]
      ] ],
      [ "Working Directory Structures", "mkToolsDesign.html#mkToolsDesign_workingDirs", [
        [ "Systems' Working Directories", "mkToolsDesign.html#mkToolsDesign_workingDirs_systems", null ],
        [ "app Directory", "mkToolsDesign.html#mkToolsDesign_workingDirs_app", [
          [ "obj Directory", "mkToolsDesign.html#mkToolsDesign_workingDirs_app_obj", null ],
          [ "src Directory", "mkToolsDesign.html#mkToolsDesign_workingDirs_app_src", null ],
          [ "staging Directory", "mkToolsDesign.html#mkToolsDesign_workingDirs_app_staging", null ]
        ] ],
        [ "api Directory", "mkToolsDesign.html#mkToolsDesign_workingDirs_api", null ],
        [ "component Directory", "mkToolsDesign.html#mkToolsDesign_workingDirs_component", null ],
        [ "Working Directories for mkapp, mkexe and mkcomp", "mkToolsDesign.html#mkToolsDesign_workingDirs_app_exe_comp", null ]
      ] ],
      [ "Memory Management", "mkToolsDesign.html#mkToolsDesign_MemoryManagement", null ],
      [ "Future Enhancements", "mkToolsDesign.html#mkToolsDesign_FutureEnhancements", null ]
    ] ],
    [ "Parsers", "mkTools_parsers.html", null ],
    [ "Sample code of audio settings for a dialing call", "c_audioMcc.html", null ],
    [ "Sample code of dtmf playback and decoding", "c_audiodtmf.html", null ],
    [ "Sample code of audio playback and capture", "c_audioCapturePlayback.html", null ],
    [ "Sample code for data connection service establishment", "c_dataConnectionServiceSample.html", null ],
    [ "Sample code of an eCall test session with a local voice prompt", "c_ecallWPromptSample.html", null ],
    [ "Sample code an eCall test session with a voice call connection", "c_ecallWVoiceSample.html", null ],
    [ "Sample code for GNSS device", "c_gnssSampleCode.html", null ],
    [ "Limit Definitions", "c_limit.html", null ],
    [ "Sample code of Modem Call control", "c_mccTest.html", null ],
    [ "Sample code for SMS Cell Broadcast reception", "c_smsCbSample.html", null ],
    [ "ADC Platform Adapter API", "c_pa_adc.html", null ],
    [ "Antenna Platform Adapter API", "c_pa_antenna.html", null ],
    [ "Modem eCall Platform Adapter API", "c_pa_ecall.html", [
      [ "Table of Contents", "c_pa_ecall.html#pa_ecall_toc", null ],
      [ "Introduction", "c_pa_ecall.html#pa_ecall_intro", null ],
      [ "Rational", "c_pa_ecall.html#pa_ecall_rational", null ]
    ] ],
    [ "Modem Information Platform Adapter API", "c_pa_info.html", [
      [ "Table of Contents", "c_pa_info.html#pa_info_toc", null ],
      [ "Introduction", "c_pa_info.html#pa_info_intro", null ],
      [ "Rational", "c_pa_info.html#pa_info_rational", null ]
    ] ],
    [ "Input Power Supply Monitoring Platform Adapter API", "c_pa_ips.html", null ],
    [ "Low Power Technologies Platform Adapter API", "c_pa_lpt.html", null ],
    [ "Modem Call Control Platform Adapter API", "c_pa_mcc.html", [
      [ "Table of Contents", "c_pa_mcc.html#pa_mcc_toc", null ],
      [ "Introduction", "c_pa_mcc.html#pa_mcc_intro", null ],
      [ "Rational", "c_pa_mcc.html#pa_mcc_rational", null ]
    ] ],
    [ "Modem Data Control Platform Adapter API", "c_pa_mdc.html", [
      [ "Table of Contents", "c_pa_mdc.html#pa_mdc_toc", null ],
      [ "Introduction", "c_pa_mdc.html#pa_mdc_intro", null ],
      [ "Rational", "c_pa_mdc.html#pa_mdc_rational", null ]
    ] ],
    [ "modem configuration Platform Adapter API", "c_pa_mdmCfg.html", null ],
    [ "Modem Radio Control Platform Adapter API", "c_pa_mrc.html", null ],
    [ "Remote SIM service Platform Adapter API", "c_pa_rsim.html", [
      [ "Table of Contents", "c_pa_rsim.html#pa_rsim_toc", null ],
      [ "Introduction", "c_pa_rsim.html#pa_rsim_intro", null ],
      [ "Rationale", "c_pa_rsim.html#pa_rsim_rational", null ]
    ] ],
    [ "Read and Write time to the Real Time Clock", "c_pa_rtc.html", null ],
    [ "Modem SIM Platform Adapter API", "c_pa_sim.html", [
      [ "Table of Contents", "c_pa_sim.html#pa_sim_toc", null ],
      [ "Introduction", "c_pa_sim.html#pa_sim_intro", null ],
      [ "Rational", "c_pa_sim.html#pa_sim_rational", null ]
    ] ],
    [ "Modem SMS Platform Adapter API", "c_pa_sms.html", [
      [ "Table of Contents", "c_pa_sms.html#pa_sms_toc", null ],
      [ "Introduction", "c_pa_sms.html#pa_sms_intro", null ],
      [ "Rational", "c_pa_sms.html#pa_sms_rational", null ]
    ] ],
    [ "Temperature Monitoring Platform Adapter API", "c_pa_temp.html", null ],
    [ "Platform Adapter Global Navigation Satellite System API", "c_pa_gnss.html", null ],
    [ "smsInbox SDD", "c_SDD_smsInbox.html", null ],
    [ "WiFi Plugin", "external_proj_mainpage.html", "external_proj_mainpage" ],
    [ "Todo List", "todo.html", null ],
    [ "Deprecated List", "deprecated.html", null ],
    [ "Namespaces", null, [
      [ "Namespace List", "namespaces.html", "namespaces" ],
      [ "Namespace Members", "namespacemembers.html", [
        [ "All", "namespacemembers.html", "namespacemembers_dup" ],
        [ "Functions", "namespacemembers_func.html", "namespacemembers_func" ],
        [ "Variables", "namespacemembers_vars.html", null ],
        [ "Typedefs", "namespacemembers_type.html", null ],
        [ "Enumerations", "namespacemembers_enum.html", null ],
        [ "Enumerator", "namespacemembers_eval.html", null ]
      ] ]
    ] ],
    [ "Data Structures", "annotated.html", [
      [ "Data Structures", "annotated.html", "annotated_dup" ],
      [ "Data Structure Index", "classes.html", null ],
      [ "Class Hierarchy", "hierarchy.html", "hierarchy" ],
      [ "Data Fields", "functions.html", [
        [ "All", "functions.html", "functions_dup" ],
        [ "Functions", "functions_func.html", "functions_func" ],
        [ "Variables", "functions_vars.html", "functions_vars" ],
        [ "Typedefs", "functions_type.html", null ],
        [ "Enumerations", "functions_enum.html", null ],
        [ "Enumerator", "functions_eval.html", null ],
        [ "Related Functions", "functions_rela.html", null ]
      ] ]
    ] ],
    [ "Files", null, [
      [ "File List", "files.html", "files" ],
      [ "Globals", "globals.html", [
        [ "All", "globals.html", "globals_dup" ],
        [ "Functions", "globals_func.html", "globals_func" ],
        [ "Variables", "globals_vars.html", "globals_vars" ],
        [ "Typedefs", "globals_type.html", "globals_type" ],
        [ "Enumerations", "globals_enum.html", null ],
        [ "Enumerator", "globals_eval.html", "globals_eval" ],
        [ "Macros", "globals_defs.html", "globals_defs" ]
      ] ]
    ] ]
  ] ]
];

var NAVTREEINDEX =
[
"Delta_Patch.html",
"app_stop_client_8c.html#a748fa753ce7d07eed585f0c09823cb76",
"avConfigure.html#avConfigure_Ping",
"c_clock.html",
"c_logging.html#c_log_basic_logging",
"c_pathIter.html",
"ccodingStdsComplexity.html#codingComplexErrorHandling",
"class_watchdog_timeout__t.html#ab60a00f4e6fa6ffc07c92ef55e1f2c6c",
"code_generator_8h.html",
"config_8c.html#a662aa3f95629803547c6082fa7b9f953",
"daemons_2linux_2update_daemon_2system_8h.html#ad844d3816d0846e21b88925d99d99eab",
"dynamic_string_8h.html#a07ba9d8b5cb5d133ff5df72fd83135fc",
"file_descriptor_8h.html#ae4cf039aa45f3c089aa1a7676d9fd057",
"globals_defs_a.html",
"inspect_8c.html#a07f1b66457647fb32c749459cf983cb6",
"json_8c.html#a6e3870d9dfb1082f92572042508271a6",
"le__at_server__interface_8h.html#a2cbdff4ae86ac055f21301aff36ec12c",
"le__avc__interface_8h.html#a8b532a08ec27ccb68f019a78fedd4dcca8c9c1138fd416868ae67e832bd8b1588",
"le__cfg__interface_8h.html#a29c8480b3310d1235487d7ea96d8b57d",
"le__ecall__interface_8h.html#a777e8d6608b5e40f73696904471519cfa2deff70e9effd8b7174f898019c05f1c",
"le__fs_8h.html#ac2dad026f875cc242f2541b57315788e",
"le__hashmap_8h.html#a8ecdbbdb5cc0773f0f9946e6e4dec89c",
"le__lpt__interface_8h.html#a74ac2323ba4e90b43fba080b09898dc3",
"le__mdc__interface_8h.html#ad9cb7d3299620d9dcde551cec45389a1",
"le__mrc__interface_8h.html#a81d705d61ee7ee56eab6aafb1af92579afc73ac735bd3bd607c20ade9d0271d45",
"le__print_8h.html",
"le__sms__interface_8h.html#a18a1937acb5878ae01a9d668fe5070b6aed2870eb00d4511f2a0c28996f8cd984",
"le__sms_inbox1__interface_8h.html#abc1acdf37c67952b627970693d208d9c",
"le__update_ctrl__interface_8h.html#a7433ec6d73597f5783f8ab5c22a8a853",
"legatoServicesModemECall.html#eCall_ClientApp_PEeCall",
"log_8h.html#a91f69de1c84b361bbdfe0d8ac5039fea",
"messaging_interface_8c.html#a3575cd784d5473363af47d33294ada21",
"mk_common_8h.html#ae67b60895501e2e3db2149ef868e283c",
"namespacemembers_vars.html",
"pa__ips_8h.html#aac14090e681a972f674805ec1eaf95be",
"pa__sms_8h.html#a0914a185712b38afe2909d72498adb57",
"parser_8h.html#a61b2e4110a793213ad894bc20e9d4b2a",
"priority_8h_source.html",
"releaseNotes17100.html#rn1710_Fixes",
"sbtrace_8c.html#ad9e973e2dbe4e0534ca426830186a14e",
"simu_2pa__wifi__ap__ti_8c.html#ae046d624690fdf4b870d9664fdd82d85",
"struct_api_ref__t.html#a42c611d9424fadbc56c1bce7bdebf708",
"struct_content__t.html#a29aa14d0f1ed425df732827ef07a8a3a",
"struct_interface_python_files__t.html#a7a637eeb540e016f140556c198f94084",
"struct_proc_timer_obj__t.html#ae55399add0cda13944b5f6e0a043593d",
"struct_token_list__t.html#ac510d9f37f49196fa990794810d4c332",
"structmodel_1_1_app__t.html#a7491950555584e5f07eb084494d58657",
"structmodel_1_1_system__t.html#a937e3fdbb57a592f26e1155660e8d095",
"structpa__rsim___apdu_ind__t.html#a34c78b1223b92313960a0b1d489ced18",
"structpath_1_1_path__t.html#a1c50adf7865f484afeacfc52edb60033",
"sys_paths_8h.html#afb0c4b215009142dfba08ad2fc66d123",
"toolsHost_av-pack.html",
"tree_db_8h.html#ad638fcbdfbfc14902b272c5f478ba2c6",
"update_daemon_8c.html#a67236a5fb1ef3cc24d54ca7d60b7e69f",
"watchdog_8c.html#acb2567ca16a7f3ade5ef7468d5b3cfb9"
];

var SYNCONMSG = 'click to disable panel synchronisation';
var SYNCOFFMSG = 'click to enable panel synchronisation';