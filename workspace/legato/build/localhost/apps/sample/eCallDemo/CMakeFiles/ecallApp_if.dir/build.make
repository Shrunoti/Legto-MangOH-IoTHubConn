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

# Utility rule file for ecallApp_if.

# Include the progress variables for this target.
include apps/sample/eCallDemo/CMakeFiles/ecallApp_if.dir/progress.make

apps/sample/eCallDemo/CMakeFiles/ecallApp_if: apps/sample/eCallDemo/ecallApp_interface.h


apps/sample/eCallDemo/ecallApp_interface.h: ../../apps/sample/eCallDemo/eCallAppComponent/ecallApp.api
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/shrunoti/Downloads/workspace/legato/build/localhost/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "ifgen 'eCallAppComponent/ecallApp.api': /home/shrunoti/Downloads/workspace/legato/build/localhost/apps/sample/eCallDemo/ecallApp_interface.h"
	cd /home/shrunoti/Downloads/workspace/legato/build/localhost/apps/sample/eCallDemo && ../../../../../bin/ifgen --gen-interface /home/shrunoti/Downloads/workspace/legato/apps/sample/eCallDemo/eCallAppComponent/ecallApp.api --import-dir /home/shrunoti/Downloads/workspace/legato/apps/sample/eCallDemo/audio --import-dir /home/shrunoti/Downloads/workspace/legato/apps/sample/eCallDemo/modemServices --import-dir /home/shrunoti/Downloads/workspace/legato/apps/sample/eCallDemo/atServices --import-dir /home/shrunoti/Downloads/workspace/legato/apps/sample/eCallDemo

ecallApp_if: apps/sample/eCallDemo/CMakeFiles/ecallApp_if
ecallApp_if: apps/sample/eCallDemo/ecallApp_interface.h
ecallApp_if: apps/sample/eCallDemo/CMakeFiles/ecallApp_if.dir/build.make

.PHONY : ecallApp_if

# Rule to build all files generated by this target.
apps/sample/eCallDemo/CMakeFiles/ecallApp_if.dir/build: ecallApp_if

.PHONY : apps/sample/eCallDemo/CMakeFiles/ecallApp_if.dir/build

apps/sample/eCallDemo/CMakeFiles/ecallApp_if.dir/clean:
	cd /home/shrunoti/Downloads/workspace/legato/build/localhost/apps/sample/eCallDemo && $(CMAKE_COMMAND) -P CMakeFiles/ecallApp_if.dir/cmake_clean.cmake
.PHONY : apps/sample/eCallDemo/CMakeFiles/ecallApp_if.dir/clean

apps/sample/eCallDemo/CMakeFiles/ecallApp_if.dir/depend:
	cd /home/shrunoti/Downloads/workspace/legato/build/localhost && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/shrunoti/Downloads/workspace/legato /home/shrunoti/Downloads/workspace/legato/apps/sample/eCallDemo /home/shrunoti/Downloads/workspace/legato/build/localhost /home/shrunoti/Downloads/workspace/legato/build/localhost/apps/sample/eCallDemo /home/shrunoti/Downloads/workspace/legato/build/localhost/apps/sample/eCallDemo/CMakeFiles/ecallApp_if.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : apps/sample/eCallDemo/CMakeFiles/ecallApp_if.dir/depend

