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

# Utility rule file for fwupdateUnitTest.

# Include the progress variables for this target.
include apps/test/fwupdate/fwupdateUnitTest/CMakeFiles/fwupdateUnitTest.dir/progress.make

apps/test/fwupdate/fwupdateUnitTest/CMakeFiles/fwupdateUnitTest:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/shrunoti/Downloads/workspace/legato/build/localhost/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "mkexe 'fwupdateUnitTest': /home/shrunoti/Downloads/workspace/legato/build/localhost/tests/bin/fwupdateUnitTest"
	cd /home/shrunoti/Downloads/workspace/legato/apps/test/fwupdate/fwupdateUnitTest && /home/shrunoti/Downloads/workspace/legato/bin/mkexe -o /home/shrunoti/Downloads/workspace/legato/build/localhost/tests/bin/fwupdateUnitTest -w /home/shrunoti/Downloads/workspace/legato/build/localhost/apps/test/fwupdate/fwupdateUnitTest/_build_fwupdateUnitTest -i /home/shrunoti/Downloads/workspace/legato/apps/test/fwupdate/fwupdateUnitTest -l /home/shrunoti/Downloads/workspace/legato/build/localhost/tests/lib -t localhost /home/shrunoti/Downloads/workspace/legato/platformAdaptor/simu/components/le_pa_fwupdate fwupdateComp . -i /home/shrunoti/Downloads/workspace/legato/components/fwupdate/fwupdateDaemon -i /home/shrunoti/Downloads/workspace/legato/framework/liblegato -i /home/shrunoti/Downloads/workspace/legato/components/fwupdate/platformAdaptor/inc -i /home/shrunoti/Downloads/workspace/legato/platformAdaptor/simu/components/le_pa_fwupdate -i /home/shrunoti/Downloads/workspace/legato/platformAdaptor/simu/components/le_pa_fwupdate -i /home/shrunoti/Downloads/workspace/legato/ -C -fvisibility=default -C -g

fwupdateUnitTest: apps/test/fwupdate/fwupdateUnitTest/CMakeFiles/fwupdateUnitTest
fwupdateUnitTest: apps/test/fwupdate/fwupdateUnitTest/CMakeFiles/fwupdateUnitTest.dir/build.make

.PHONY : fwupdateUnitTest

# Rule to build all files generated by this target.
apps/test/fwupdate/fwupdateUnitTest/CMakeFiles/fwupdateUnitTest.dir/build: fwupdateUnitTest

.PHONY : apps/test/fwupdate/fwupdateUnitTest/CMakeFiles/fwupdateUnitTest.dir/build

apps/test/fwupdate/fwupdateUnitTest/CMakeFiles/fwupdateUnitTest.dir/clean:
	cd /home/shrunoti/Downloads/workspace/legato/build/localhost/apps/test/fwupdate/fwupdateUnitTest && $(CMAKE_COMMAND) -P CMakeFiles/fwupdateUnitTest.dir/cmake_clean.cmake
.PHONY : apps/test/fwupdate/fwupdateUnitTest/CMakeFiles/fwupdateUnitTest.dir/clean

apps/test/fwupdate/fwupdateUnitTest/CMakeFiles/fwupdateUnitTest.dir/depend:
	cd /home/shrunoti/Downloads/workspace/legato/build/localhost && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/shrunoti/Downloads/workspace/legato /home/shrunoti/Downloads/workspace/legato/apps/test/fwupdate/fwupdateUnitTest /home/shrunoti/Downloads/workspace/legato/build/localhost /home/shrunoti/Downloads/workspace/legato/build/localhost/apps/test/fwupdate/fwupdateUnitTest /home/shrunoti/Downloads/workspace/legato/build/localhost/apps/test/fwupdate/fwupdateUnitTest/CMakeFiles/fwupdateUnitTest.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : apps/test/fwupdate/fwupdateUnitTest/CMakeFiles/fwupdateUnitTest.dir/depend
