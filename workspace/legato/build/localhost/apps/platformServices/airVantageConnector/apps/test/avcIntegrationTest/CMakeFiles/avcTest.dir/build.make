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

# Utility rule file for avcTest.

# Include the progress variables for this target.
include apps/platformServices/airVantageConnector/apps/test/avcIntegrationTest/CMakeFiles/avcTest.dir/progress.make

apps/platformServices/airVantageConnector/apps/test/avcIntegrationTest/CMakeFiles/avcTest:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/shrunoti/Downloads/workspace/legato/build/localhost/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "mkapp 'avcTest': /home/shrunoti/Downloads/workspace/legato/build/localhost/tests/apps/avcTest.localhost"
	cd /home/shrunoti/Downloads/workspace/legato/apps/platformServices/airVantageConnector/apps/test/avcIntegrationTest && PATH=/home/shrunoti/Downloads/workspace/legato/bin:/usr/bin/:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/snap/bin:/home/shrunoti/Downloads/workspace/legato/bin:/home/shrunoti/Downloads/workspace/legato/bin /home/shrunoti/Downloads/workspace/legato/bin/mkapp avcTest.adef -t localhost -w /home/shrunoti/Downloads/workspace/legato/build/localhost/apps/platformServices/airVantageConnector/apps/test/avcIntegrationTest/_build_avcTest.localhost -i /home/shrunoti/Downloads/workspace/legato/apps/platformServices/airVantageConnector/apps/test/avcIntegrationTest -c /home/shrunoti/Downloads/workspace/legato/apps/platformServices/airVantageConnector/apps/test/avcIntegrationTest -o /home/shrunoti/Downloads/workspace/legato/build/localhost/tests/apps

avcTest: apps/platformServices/airVantageConnector/apps/test/avcIntegrationTest/CMakeFiles/avcTest
avcTest: apps/platformServices/airVantageConnector/apps/test/avcIntegrationTest/CMakeFiles/avcTest.dir/build.make

.PHONY : avcTest

# Rule to build all files generated by this target.
apps/platformServices/airVantageConnector/apps/test/avcIntegrationTest/CMakeFiles/avcTest.dir/build: avcTest

.PHONY : apps/platformServices/airVantageConnector/apps/test/avcIntegrationTest/CMakeFiles/avcTest.dir/build

apps/platformServices/airVantageConnector/apps/test/avcIntegrationTest/CMakeFiles/avcTest.dir/clean:
	cd /home/shrunoti/Downloads/workspace/legato/build/localhost/apps/platformServices/airVantageConnector/apps/test/avcIntegrationTest && $(CMAKE_COMMAND) -P CMakeFiles/avcTest.dir/cmake_clean.cmake
.PHONY : apps/platformServices/airVantageConnector/apps/test/avcIntegrationTest/CMakeFiles/avcTest.dir/clean

apps/platformServices/airVantageConnector/apps/test/avcIntegrationTest/CMakeFiles/avcTest.dir/depend:
	cd /home/shrunoti/Downloads/workspace/legato/build/localhost && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/shrunoti/Downloads/workspace/legato /home/shrunoti/Downloads/workspace/legato/apps/platformServices/airVantageConnector/apps/test/avcIntegrationTest /home/shrunoti/Downloads/workspace/legato/build/localhost /home/shrunoti/Downloads/workspace/legato/build/localhost/apps/platformServices/airVantageConnector/apps/test/avcIntegrationTest /home/shrunoti/Downloads/workspace/legato/build/localhost/apps/platformServices/airVantageConnector/apps/test/avcIntegrationTest/CMakeFiles/avcTest.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : apps/platformServices/airVantageConnector/apps/test/avcIntegrationTest/CMakeFiles/avcTest.dir/depend

