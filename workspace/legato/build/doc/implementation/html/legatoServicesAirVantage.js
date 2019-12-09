var legatoServicesAirVantage =
[
    [ "Documentation", "legatoServicesAirVantage.html#legatoServicesAirVantage_docs", null ],
    [ "APIs", "legatoServicesAirVantage.html#legatoServicesAirVantage_API", null ],
    [ "App Implementation", "legatoServicesAirVantage.html#legatoServicesAirVantage_PS", null ],
    [ "Sample Apps", "legatoServicesAirVantage.html#legatoServicesAirVantage_Sample", null ],
    [ "External Documentation", "legatoServicesAirVantage.html#legatoServicesAirVantage_ExternalDocs", null ],
    [ "AirVantage 1.0 to 2.0 Migration", "avMigration.html", [
      [ "Comparison of Changes", "avMigration.html#avMigration_Comparison", null ],
      [ "Migrating from AVC 1.0 to AVC 2.0", "avMigration.html#avMigration_Migration", null ],
      [ "Asset Data Models", "avMigration.html#avMigration_AssetData", [
        [ "Data Model", "avMigration.html#avMigration_AssetDataDataModel", null ],
        [ "Command Arguments", "avMigration.html#avMigration_AssetDataCommand", null ],
        [ "Resource Paths", "avMigration.html#avMigration_AssetDataResourcePaths", null ]
      ] ],
      [ "Time Series", "avMigration.html#avMigration_TimeSeries", null ]
    ] ],
    [ "Register a Target on AirVantage", "avRegister.html", [
      [ "Register a Target", "avRegister.html#avRegister_createSys", [
        [ "Test Connection to AirVantage", "avRegister.html#avRegister_createSys_test", null ]
      ] ],
      [ "Build Apps to connect to AirVantage", "avRegister.html#avRegister_DesignApps", null ]
    ] ],
    [ "Create and Install Bundles", "avInstallUpload.html", [
      [ "Create App or System Bundle", "avInstallUpload.html#avInstall", [
        [ "Setting an App Type", "avInstallUpload.html#avInstallAppType", null ],
        [ "App Signature Checks", "avInstallUpload.html#avInstallAppSigs", null ]
      ] ],
      [ "Create Installation job", "avInstallUpload.html#avInstallCreateInstJob", null ],
      [ "Receive App on AirVantage Agent", "avInstallUpload.html#avInstallRcvAgent", [
        [ "Check Success Status on AirVantage", "avInstallUpload.html#avInstallUploadStatus", null ],
        [ "Uploading a 17.05 Legato System Bundle", "avInstallUpload.html#avInstallUpload_CustomSystem", null ]
      ] ]
    ] ],
    [ "Asset Data", "avData.html", [
      [ "Overview", "avData.html#avData_Overview", null ],
      [ "Variables, Settings and Commands", "avData.html#avData_VariablesSettings", null ],
      [ "Asset Data Models", "avData.html#avData_DataModel", null ],
      [ "TimeSeries", "avData.html#avData_TimeSeries", null ]
    ] ],
    [ "Exchange Data", "avExchangeData.html", [
      [ "Prerequisites", "avExchangeData.html#avExchangeData_pre", null ],
      [ "Data Create the App", "avExchangeData.html#avExchangeData_define", [
        [ "One Room Scenario", "avExchangeData.html#avExchangeData_defineData_one", null ]
      ] ],
      [ "Implement Legato App", "avExchangeData.html#avExchangeData_implementLegatoApp", [
        [ "Sample Source Code", "avExchangeData.html#avExchangeData_implementLegatoApp_sampleCode", null ],
        [ "Use Legato APIs", "avExchangeData.html#avExchangeData_implementLegatoApp_useAPIs", null ],
        [ "Declare Variables", "avExchangeData.html#avExchangeData_implementLegatoApp_varSettings", null ],
        [ "Initialize App", "avExchangeData.html#avExchangeData_implementLegatoApp_initApp", [
          [ "COMPONENT_INIT", "avExchangeData.html#avExchangeData_implementLegatoApp_COMPONENT_INIT", null ],
          [ "Variable Handler Functions", "avExchangeData.html#avExchangeData_implementLegatoApp_VariableHandlers", null ],
          [ "Setting Handler Functions", "avExchangeData.html#avExchangeData_implementLegatoApp_setHandlers", null ],
          [ "Command Handler Functions", "avExchangeData.html#avExchangeData_implementLegatoApp_cmdHandlers", null ],
          [ "AVC Handler Functions", "avExchangeData.html#avExchangeData_implementLegatoApp_avcHandler", null ]
        ] ],
        [ "Push Data from Target to AirVantage", "avExchangeData.html#avExchangeData_implementLegatoApp_pushHandler", [
          [ "Exit App", "avExchangeData.html#avExchangeData_implementLegatoApp_exitApp", null ]
        ] ],
        [ "Compile and Install", "avExchangeData.html#avExchangeData_implementLegatoApp_compileInstall", null ]
      ] ],
      [ "Exchange Data with AirVantage", "avExchangeData.html#avExchangeData_exchData", [
        [ "Push Data", "avExchangeData.html#avExchangeData_exchData_pushData", null ]
      ] ],
      [ "Testing Asset Data App Results", "avExchangeData.html#avExchangeData_avAPI", [
        [ "Request Access Token", "avExchangeData.html#avExchangeData_avAPI_Authenticate", null ],
        [ "Obtain the UID of your target", "avExchangeData.html#avExchangeData_avAPI_UID", null ],
        [ "Request Data", "avExchangeData.html#avExchangeData_avAPI_Read", null ],
        [ "Apply Settings", "avExchangeData.html#avExchangeData_avAPI_Write", null ],
        [ "Send a Command", "avExchangeData.html#avExchangeData_avAPI_Command", null ]
      ] ]
    ] ],
    [ "Configuration Tips for AirVantage", "avConfigure.html", [
      [ "Check Running Services", "avConfigure.html#avConfigure_Apps", null ],
      [ "Check the Data Connection", "avConfigure.html#avConfigure_Data", null ],
      [ "Check the IP Address", "avConfigure.html#avConfigure_IP", null ],
      [ "Check Network Connections", "avConfigure.html#avConfigure_Ping", null ],
      [ "Connect to the AirVantage Server", "avConfigure.html#avConfigure_Connect", null ],
      [ "Configure Frequency of Connections", "avConfigure.html#avConfigure_Frequency", null ]
    ] ],
    [ "AirVantage Connector API", "c_le_avc.html", [
      [ "Firmware/Application Updates", "c_le_avc.html#c_le_avc_update", [
        [ "User Agreement", "c_le_avc.html#c_le_avc_user_agreement", null ],
        [ "Update Control", "c_le_avc.html#c_le_avc_update_control", [
          [ "Pending Updates", "c_le_avc.html#c_le_avc_updateControl_pending", null ],
          [ "Accepting Installs/Uninstalls", "c_le_avc.html#c_le_avc_updateControl_accepting", null ]
        ] ],
        [ "Application Installation Blocking", "c_le_avc.html#c_le_avc_update_app", null ],
        [ "Suspend and resume an update", "c_le_avc.html#c_le_avc_suspend_resume", null ]
      ] ],
      [ "Timers", "c_le_avc.html#c_le_avc_Timers", [
        [ "Polling Timer", "c_le_avc.html#c_le_avc_PollingTimer", null ],
        [ "Retry Timers", "c_le_avc.html#c_le_avc_RetryTimers", null ]
      ] ],
      [ "Device reboot", "c_le_avc.html#c_le_avc_reboot", null ],
      [ "Credential Status", "c_le_avc.html#c_le_avc_GetCredentialStatus", null ],
      [ "Connection pending", "c_le_avc.html#c_le_avc_connection", null ],
      [ "Data routing", "c_le_avc.html#c_le_avc_routing", null ],
      [ "Connection / Download timeout", "c_le_avc.html#c_le_avc_timeout", null ],
      [ "Service Configuration Tree", "c_le_avc.html#le_avcService_configdb", null ]
    ] ],
    [ "AirVantage Data API", "c_le_avdata.html", [
      [ "IPC Interfaces Binding", "c_le_avdata.html#le_avdata_binding", null ],
      [ "AirVantage Data Overview", "c_le_avdata.html#le_avdata_overview", null ],
      [ "Field Values and Activity", "c_le_avdata.html#le_avdata_field", null ],
      [ "Namespace", "c_le_avdata.html#le_avdata_namespace", null ],
      [ "Time Series", "c_le_avdata.html#le_avdata_timeseries", null ],
      [ "User App Session Management", "c_le_avdata.html#le_avdata_session", null ],
      [ "Fatal Behavior", "c_le_avdata.html#le_avdata_fatal", null ]
    ] ]
];