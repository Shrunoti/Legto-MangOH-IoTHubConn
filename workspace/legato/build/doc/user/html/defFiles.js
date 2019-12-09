var defFiles =
[
    [ ".sdef Files", "defFiles.html#defFilesOverview_sdef", null ],
    [ ".adef Files", "defFiles.html#defFilesOverview_adef", null ],
    [ ".cdef Files", "defFiles.html#defFilesOverview_cdef", null ],
    [ ".mdef Files", "defFiles.html#defFilesOverview_mdef", null ],
    [ "Build Tool Search Paths", "defFiles.html#defFilesOverview_searchPaths", null ],
    [ "Precedence and Overriding", "defFiles.html#defFilesOverview_overriding", null ],
    [ "Unsandboxed App Limits", "defFiles.html#defFilesOverview_unsandboxedApps", null ],
    [ "Common File Format", "defFilesFormat.html", [
      [ "Sections", "defFilesFormat.html#defFileFormatSections", null ],
      [ "Named Items", "defFilesFormat.html#defFileFormatNamedItems", null ],
      [ "Numerical Values", "defFilesFormat.html#defFileFormatNumberValues", null ],
      [ "Text Values", "defFilesFormat.html#defFileFormatTextValues", null ],
      [ "Environment Variables", "defFilesFormat.html#defFileFormatUsingEnvVarsIn", null ],
      [ "Include Directives", "defFilesFormat.html#defFileFormatIncludeDirectives", null ],
      [ "If Directives", "defFilesFormat.html#defFileFormatIfDirectives", null ],
      [ "Whitespace", "defFilesFormat.html#defFileFormatWhitespace", null ],
      [ "Comments", "defFilesFormat.html#defFileFormatComments", null ],
      [ "Sample", "defFilesFormat.html#defFileFormatSample", null ]
    ] ],
    [ "System Definition .sdef", "defFilesSdef.html", [
      [ "Search Paths", "defFilesSdef.html#defFilesSdef_searchPaths", null ],
      [ "interfaceSearch", "defFilesSdef.html#defFilesSdef_searchPathsInterface", null ],
      [ "appSearch", "defFilesSdef.html#defFilesSdef_searchPathsApp", null ],
      [ "componentSearch", "defFilesSdef.html#defFilesSdef_searchPathsComponent", null ],
      [ "apps", "defFilesSdef.html#defFilesSdef_apps", [
        [ "maxPriority", "defFilesSdef.html#defFilesSdef_appsMaxPriority", null ],
        [ "preloaded", "defFilesSdef.html#defFilesSdef_appsPreloaded", null ]
      ] ],
      [ "bindings", "defFilesSdef.html#defFilesSdef_bindings", null ],
      [ "buildVars", "defFilesSdef.html#defFilesSdef_buildVars", null ],
      [ "cflags", "defFilesSdef.html#defFilesSdef_cFlags", null ],
      [ "cxxflags", "defFilesSdef.html#defFilesSdef_cxxFlags", null ],
      [ "ldflags", "defFilesSdef.html#defFilesSdef_ldFlags", null ],
      [ "kernelModules", "defFilesSdef.html#defFilesSdef_kernelModules", null ],
      [ "commands", "defFilesSdef.html#defFilesSdef_commands", null ],
      [ "externalWatchdogKick", "defFilesSdef.html#defFilesSdef_externalWatchdogKick", null ]
    ] ],
    [ "Application Definition .adef", "defFilesAdef.html", [
      [ "bindings", "defFilesAdef.html#defFilesAdef_bindings", null ],
      [ "bundles", "defFilesAdef.html#defFilesAdef_bundles", null ],
      [ "components", "defFilesAdef.html#defFilesAdef_components", null ],
      [ "cpuShare", "defFilesAdef.html#defFilesAdef_cpuShare", null ],
      [ "executables", "defFilesAdef.html#defFilesAdef_executables", null ],
      [ "groups", "defFilesAdef.html#defFilesAdef_groups", null ],
      [ "maxFileSystemBytes", "defFilesAdef.html#defFilesAdef_maxFileSystemBytes", null ],
      [ "maxMemoryBytes", "defFilesAdef.html#defFilesAdef_maxMemoryBytes", null ],
      [ "maxMQueueBytes", "defFilesAdef.html#defFilesAdef_maxMQueueBytes", null ],
      [ "maxQueuedSignals", "defFilesAdef.html#defFilesAdef_maxQueuedSignals", null ],
      [ "maxThreads", "defFilesAdef.html#defFilesAdef_maxThreads", null ],
      [ "maxSecureStorage Bytes", "defFilesAdef.html#defFilesAdef_maxSecureStorageBytes", null ],
      [ "pools", "defFilesAdef.html#defFilesAdef_pools", null ],
      [ "processes", "defFilesAdef.html#defFilesAdef_process", [
        [ "run", "defFilesAdef.html#defFilesAdef_processRun", null ],
        [ "envVars", "defFilesAdef.html#defFilesAdef_processEnvVars", null ],
        [ "faultAction", "defFilesAdef.html#defFilesAdef_processFaultAction", null ],
        [ "priority", "defFilesAdef.html#defFilesAdef_processPriority", null ],
        [ "maxCoreDumpFileBytes", "defFilesAdef.html#defFilesAdef_processMaxCoreDumpFileBytes", null ],
        [ "maxFileBytes", "defFilesAdef.html#defFilesAdef_processMaxFileBytes", null ],
        [ "maxFileDescriptors", "defFilesAdef.html#defFilesAdef_processMaxFileDescriptors", null ],
        [ "maxLockedMemoryBytes", "defFilesAdef.html#defFilesAdef_processMaxLockedMemoryBytes", null ],
        [ "watchdogAction", "defFilesAdef.html#defFilesAdef_processwatchdogAction", null ],
        [ "watchdogTimeout", "defFilesAdef.html#defFilesAdef_processwatchdogTimeout", null ]
      ] ],
      [ "extern", "defFilesAdef.html#defFilesAdef_extern", [
        [ "Server-side Example", "defFilesAdef.html#defFilesAdef_externServer", null ],
        [ "Client-side Example", "defFilesAdef.html#defFilesAdef_externClient", null ],
        [ "Binding Example", "defFilesAdef.html#defFilesAdef_externBinding", null ],
        [ "requires and provides", "defFilesAdef.html#defFilesAdef_externRequiresProvides", null ]
      ] ],
      [ "requires", "defFilesAdef.html#defFilesAdef_requires", [
        [ "configTree", "defFilesAdef.html#defFilesAdef_requiresConfigTree", null ],
        [ "dir", "defFilesAdef.html#defFilesAdef_requiresDir", null ],
        [ "file", "defFilesAdef.html#defFilesAdef_requiresFile", null ],
        [ "device", "defFilesAdef.html#defFilesAdef_requiresDevice", null ]
      ] ],
      [ "sandboxed", "defFilesAdef.html#defFilesAdef_sandboxed", null ],
      [ "start", "defFilesAdef.html#defFilesAdef_start", null ],
      [ "version", "defFilesAdef.html#defFilesAdef_version", null ],
      [ "watchdogAction", "defFilesAdef.html#defFilesAdef_watchdogAction", null ],
      [ "watchdogTimeout", "defFilesAdef.html#defFilesAdef_watchdogTimeout", null ],
      [ "maxwatchdogTimeout", "defFilesAdef.html#defFilesAdef_maxwatchdogTimeout", null ]
    ] ],
    [ "Component Definition .cdef", "defFilesCdef.html", [
      [ "assets", "defFilesCdef.html#defFilesCdef_assets", null ],
      [ "bundles", "defFilesCdef.html#defFilesCdef_bundles", null ],
      [ "cflags", "defFilesCdef.html#defFilesCdef_cFlags", null ],
      [ "cxxflags", "defFilesCdef.html#defFilesCdef_cxxFlags", null ],
      [ "externalBuild", "defFilesCdef.html#defFilesCdef_externalBuild", null ],
      [ "ldflags", "defFilesCdef.html#defFilesCdef_ldFlags", null ],
      [ "pools", "defFilesCdef.html#defFilesCdef_pools", null ],
      [ "provides", "defFilesCdef.html#defFilesCdef_provides", [
        [ "api", "defFilesCdef.html#defFilesCdef_providesApi", [
          [ "[manual-start]", "defFilesCdef.html#defFilesCdef_providesApiManualStart", null ],
          [ "[async]", "defFilesCdef.html#defFilesCdef_providesApiAsync", null ]
        ] ]
      ] ],
      [ "requires", "defFilesCdef.html#defFilesCdef_requires", [
        [ "api", "defFilesCdef.html#defFilesCdef_requiresApi", [
          [ "options", "defFilesCdef.html#defFilesCdef_requiresApiOptions", null ]
        ] ],
        [ "File", "defFilesCdef.html#defFilesCdef_requiresFile", null ],
        [ "device", "defFilesCdef.html#defFilesCdef_requiresDevice", null ],
        [ "dir", "defFilesCdef.html#defFilesCdef_requiresDir", null ],
        [ "lib", "defFilesCdef.html#defFilesCdef_requiresLib", null ],
        [ "component", "defFilesCdef.html#defFilesCdef_requiresComponent", null ]
      ] ],
      [ "javaPackage", "defFilesCdef.html#defFilesCdef_requiresJavaPackage", null ],
      [ "sources", "defFilesCdef.html#defFilesCdef_sources", null ]
    ] ],
    [ "Kernel Module Definition .mdef", "defFilesMdef.html", [
      [ "sources", "defFilesMdef.html#defFilesMdef_sources", null ],
      [ "preBuilt", "defFilesMdef.html#defFilesMdef_preBuilt", null ],
      [ "params", "defFilesMdef.html#defFilesMdef_params", null ],
      [ "flags", "defFilesMdef.html#defFilesMdef_flags", null ]
    ] ]
];