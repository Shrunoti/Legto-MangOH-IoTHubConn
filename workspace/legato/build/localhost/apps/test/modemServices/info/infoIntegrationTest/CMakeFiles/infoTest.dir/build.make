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

# Utility rule file for infoTest.

# Include the progress variables for this target.
include apps/test/modemServices/info/infoIntegrationTest/CMakeFiles/infoTest.dir/progress.make

apps/test/modemServices/info/infoIntegrationTest/CMakeFiles/infoTest:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/shrunoti/Downloads/workspace/legato/build/localhost/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "mkapp 'infoTest': /home/shrunoti/Downloads/workspace/legato/build/localhost/tests/apps/infoTest.localhost"
	cd /home/shrunoti/Downloads/workspace/legato/apps/test/modemServices/info/infoIntegrationTest && PATH=/home/shrunoti/Downloads/workspace/legato/bin:/usr/bin/:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/snap/bin:/home/shrunoti/Downloads/workspace/legato/bin:/home/shrunoti/Downloads/workspace/legato/bin /home/shrunoti/Downloads/workspace/legato/bin/mkapp infoTest.adef -t localhost -w /home/shrunoti/Downloads/workspace/legato/build/localhost/apps/test/modemServices/info/infoIntegrationTest/_build_infoTest.localhost -i /home/shrunoti/Downloads/workspace/legato/apps/test/modemServices/info/infoIntegrationTest -c /home/shrunoti/Downloads/workspace/legato/apps/test/modemServices/info/infoIntegrationTest -o /home/shrunoti/Downloads/workspace/legato/build/localhost/tests/apps -i /home/shrunoti/Downloads/workspace/legato/interfaces/modemServices

infoTest: apps/test/modemServices/info/infoIntegrationTest/CMakeFiles/infoTest
infoTest: apps/test/modemServices/info/infoIntegrationTest/CMakeFiles/infoTest.dir/build.make

.PHONY : infoTest

# Rule to build all files generated by this target.
apps/test/modemServices/info/infoIntegrationTest/CMakeFiles/infoTest.dir/build: infoTest

.PHONY : apps/test/modemServices/info/infoIntegrationTest/CMakeFiles/infoTest.dir/build

apps/test/modemServices/info/infoIntegrationTest/CMakeFiles/infoTest.dir/clean:
	cd /home/shrunoti/Downloads/workspace/legato/build/localhost/apps/test/modemServices/info/infoIntegrationTest && $(CMAKE_COMMAND) -P CMakeFiles/infoTest.dir/cmake_clean.cmake
.PHONY : apps/test/modemServices/info/infoIntegrationTest/CMakeFiles/infoTest.dir/clean

apps/test/modemServices/info/infoIntegrationTest/CMakeFiles/infoTest.dir/depend:
	cd /home/shrunoti/Downloads/workspace/legato/build/localhost && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/shrunoti/Downloads/workspace/legato /home/shrunoti/Downloads/workspace/legato/apps/test/modemServices/info/infoIntegrationTest /home/shrunoti/Downloads/workspace/legato/build/localhost /home/shrunoti/Downloads/workspace/legato/build/localhost/apps/test/modemServices/info/infoIntegrationTest /home/shrunoti/Downloads/workspace/legato/build/localhost/apps/test/modemServices/info/infoIntegrationTest/CMakeFiles/infoTest.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : apps/test/modemServices/info/infoIntegrationTest/CMakeFiles/infoTest.dir/depend

