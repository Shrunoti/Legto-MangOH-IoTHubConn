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

# Utility rule file for testAssetData.

# Include the progress variables for this target.
include apps/test/avcService/assetData/CMakeFiles/testAssetData.dir/progress.make

apps/test/avcService/assetData/CMakeFiles/testAssetData:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/shrunoti/Downloads/workspace/legato/build/localhost/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "mkexe 'testAssetData': /home/shrunoti/Downloads/workspace/legato/build/localhost/tests/bin/testAssetData"
	cd /home/shrunoti/Downloads/workspace/legato/apps/test/avcService/assetData && /home/shrunoti/Downloads/workspace/legato/bin/mkexe -o /home/shrunoti/Downloads/workspace/legato/build/localhost/tests/bin/testAssetData -w /home/shrunoti/Downloads/workspace/legato/build/localhost/apps/test/avcService/assetData/_build_testAssetData -i /home/shrunoti/Downloads/workspace/legato/apps/test/avcService/assetData -l /home/shrunoti/Downloads/workspace/legato/build/localhost/tests/lib -t localhost assetDataTest -i /home/shrunoti/Downloads/workspace/legato/interfaces -i /home/shrunoti/Downloads/workspace/legato/components/airVantage/avcDaemon/ -i /home/shrunoti/Downloads/workspace/legato/framework/liblegato -i /home/shrunoti/Downloads/workspace/legato/components/airVantage/platformAdaptor/inc

testAssetData: apps/test/avcService/assetData/CMakeFiles/testAssetData
testAssetData: apps/test/avcService/assetData/CMakeFiles/testAssetData.dir/build.make

.PHONY : testAssetData

# Rule to build all files generated by this target.
apps/test/avcService/assetData/CMakeFiles/testAssetData.dir/build: testAssetData

.PHONY : apps/test/avcService/assetData/CMakeFiles/testAssetData.dir/build

apps/test/avcService/assetData/CMakeFiles/testAssetData.dir/clean:
	cd /home/shrunoti/Downloads/workspace/legato/build/localhost/apps/test/avcService/assetData && $(CMAKE_COMMAND) -P CMakeFiles/testAssetData.dir/cmake_clean.cmake
.PHONY : apps/test/avcService/assetData/CMakeFiles/testAssetData.dir/clean

apps/test/avcService/assetData/CMakeFiles/testAssetData.dir/depend:
	cd /home/shrunoti/Downloads/workspace/legato/build/localhost && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/shrunoti/Downloads/workspace/legato /home/shrunoti/Downloads/workspace/legato/apps/test/avcService/assetData /home/shrunoti/Downloads/workspace/legato/build/localhost /home/shrunoti/Downloads/workspace/legato/build/localhost/apps/test/avcService/assetData /home/shrunoti/Downloads/workspace/legato/build/localhost/apps/test/avcService/assetData/CMakeFiles/testAssetData.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : apps/test/avcService/assetData/CMakeFiles/testAssetData.dir/depend

