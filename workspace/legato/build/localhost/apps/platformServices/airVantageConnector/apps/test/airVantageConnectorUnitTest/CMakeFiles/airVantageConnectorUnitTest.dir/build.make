# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.5

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/shrunoti/Downloads/workspace/legato

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/shrunoti/Downloads/workspace/legato/build/localhost

# Utility rule file for airVantageConnectorUnitTest.

# Include the progress variables for this target.
include apps/platformServices/airVantageConnector/apps/test/airVantageConnectorUnitTest/CMakeFiles/airVantageConnectorUnitTest.dir/progress.make

apps/platformServices/airVantageConnector/apps/test/airVantageConnectorUnitTest/CMakeFiles/airVantageConnectorUnitTest:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/shrunoti/Downloads/workspace/legato/build/localhost/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "mkexe 'airVantageConnectorUnitTest': /home/shrunoti/Downloads/workspace/legato/build/localhost/tests/bin/airVantageConnectorUnitTest"
	cd /home/shrunoti/Downloads/workspace/legato/apps/platformServices/airVantageConnector/apps/test/airVantageConnectorUnitTest && /home/shrunoti/Downloads/workspace/legato/bin/mkexe -o /home/shrunoti/Downloads/workspace/legato/build/localhost/tests/bin/airVantageConnectorUnitTest -w /home/shrunoti/Downloads/workspace/legato/build/localhost/apps/platformServices/airVantageConnector/apps/test/airVantageConnectorUnitTest/_build_airVantageConnectorUnitTest -i /home/shrunoti/Downloads/workspace/legato/apps/platformServices/airVantageConnector/apps/test/airVantageConnectorUnitTest -l /home/shrunoti/Downloads/workspace/legato/build/localhost/tests/lib -t localhost airVantageConnectorComp . -i airVantageConnectorComp -i /home/shrunoti/Downloads/workspace/legato/apps/platformServices/airVantageConnector//apps/test/airVantageConnectorUnitTest/ -i /home/shrunoti/Downloads/workspace/legato/apps/platformServices/airVantageConnector//avcClient/ -i /home/shrunoti/Downloads/workspace/legato/apps/platformServices/airVantageConnector//avcDaemon/ -i /home/shrunoti/Downloads/workspace/legato/apps/platformServices/airVantageConnector//avcAppUpdate/ -i /home/shrunoti/Downloads/workspace/legato/apps/platformServices/airVantageConnector//packageDownloader/ -i /home/shrunoti/Downloads/workspace/legato/framework/liblegato -i /home/shrunoti/Downloads/workspace/legato/components/watchdogChain/ -i /home/shrunoti/Downloads/workspace/legato/framework/liblegato/linux/ -i /home/shrunoti/Downloads/workspace/legato/3rdParty/Lwm2mCore/include/ -i /home/shrunoti/Downloads/workspace/legato/3rdParty/Lwm2mCore/include/platform-specific/linux/ -i /home/shrunoti/Downloads/workspace/legato/3rdParty/Lwm2mCore/packageDownloader/ -i /home/shrunoti/Downloads/workspace/legato/3rdParty/Lwm2mCore/3rdParty/wakaama/core/ -i /home/shrunoti/Downloads/workspace/legato/3rdParty/Lwm2mCore/include/lwm2mcore/ -i /home/shrunoti/Downloads/workspace/legato/3rdParty/Lwm2mCore/sessionManager/ -i /home/shrunoti/Downloads/workspace/legato/interfaces/airVantage/ -i /home/shrunoti/Downloads/workspace/legato/interfaces/modemServices/ -i /home/shrunoti/Downloads/workspace/legato/interfaces/ -i /home/shrunoti/Downloads/workspace/legato/platformAdaptor/simu/components/le_pa_avc -i /home/shrunoti/Downloads/workspace/legato/components/airVantage/platformAdaptor/inc/ -s /home/shrunoti/Downloads/workspace/legato/3rdParty/Lwm2mCore/include/lwm2mcore/ -C -fvisibility=default

airVantageConnectorUnitTest: apps/platformServices/airVantageConnector/apps/test/airVantageConnectorUnitTest/CMakeFiles/airVantageConnectorUnitTest
airVantageConnectorUnitTest: apps/platformServices/airVantageConnector/apps/test/airVantageConnectorUnitTest/CMakeFiles/airVantageConnectorUnitTest.dir/build.make

.PHONY : airVantageConnectorUnitTest

# Rule to build all files generated by this target.
apps/platformServices/airVantageConnector/apps/test/airVantageConnectorUnitTest/CMakeFiles/airVantageConnectorUnitTest.dir/build: airVantageConnectorUnitTest

.PHONY : apps/platformServices/airVantageConnector/apps/test/airVantageConnectorUnitTest/CMakeFiles/airVantageConnectorUnitTest.dir/build

apps/platformServices/airVantageConnector/apps/test/airVantageConnectorUnitTest/CMakeFiles/airVantageConnectorUnitTest.dir/clean:
	cd /home/shrunoti/Downloads/workspace/legato/build/localhost/apps/platformServices/airVantageConnector/apps/test/airVantageConnectorUnitTest && $(CMAKE_COMMAND) -P CMakeFiles/airVantageConnectorUnitTest.dir/cmake_clean.cmake
.PHONY : apps/platformServices/airVantageConnector/apps/test/airVantageConnectorUnitTest/CMakeFiles/airVantageConnectorUnitTest.dir/clean

apps/platformServices/airVantageConnector/apps/test/airVantageConnectorUnitTest/CMakeFiles/airVantageConnectorUnitTest.dir/depend:
	cd /home/shrunoti/Downloads/workspace/legato/build/localhost && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/shrunoti/Downloads/workspace/legato /home/shrunoti/Downloads/workspace/legato/apps/platformServices/airVantageConnector/apps/test/airVantageConnectorUnitTest /home/shrunoti/Downloads/workspace/legato/build/localhost /home/shrunoti/Downloads/workspace/legato/build/localhost/apps/platformServices/airVantageConnector/apps/test/airVantageConnectorUnitTest /home/shrunoti/Downloads/workspace/legato/build/localhost/apps/platformServices/airVantageConnector/apps/test/airVantageConnectorUnitTest/CMakeFiles/airVantageConnectorUnitTest.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : apps/platformServices/airVantageConnector/apps/test/airVantageConnectorUnitTest/CMakeFiles/airVantageConnectorUnitTest.dir/depend

