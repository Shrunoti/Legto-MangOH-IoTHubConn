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

# Utility rule file for rsimUnitTest.

# Include the progress variables for this target.
include apps/test/modemServices/rsim/rsimUnitTest/CMakeFiles/rsimUnitTest.dir/progress.make

apps/test/modemServices/rsim/rsimUnitTest/CMakeFiles/rsimUnitTest:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/shrunoti/Downloads/workspace/legato/build/localhost/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "mkexe 'rsimUnitTest': /home/shrunoti/Downloads/workspace/legato/build/localhost/tests/bin/rsimUnitTest"
	cd /home/shrunoti/Downloads/workspace/legato/apps/test/modemServices/rsim/rsimUnitTest && /home/shrunoti/Downloads/workspace/legato/bin/mkexe -o /home/shrunoti/Downloads/workspace/legato/build/localhost/tests/bin/rsimUnitTest -w /home/shrunoti/Downloads/workspace/legato/build/localhost/apps/test/modemServices/rsim/rsimUnitTest/_build_rsimUnitTest -i /home/shrunoti/Downloads/workspace/legato/apps/test/modemServices/rsim/rsimUnitTest -l /home/shrunoti/Downloads/workspace/legato/build/localhost/tests/lib -t localhost . -i /home/shrunoti/Downloads/workspace/legato/components/modemServices//rSimDaemon -i /home/shrunoti/Downloads/workspace/legato/components/modemServices//platformAdaptor/inc -i /home/shrunoti/Downloads/workspace/legato/framework/liblegato -i /home/shrunoti/Downloads/workspace/legato/platformAdaptor/simu/components/le_pa_remotesim -s /home/shrunoti/Downloads/workspace/legato/platformAdaptor

rsimUnitTest: apps/test/modemServices/rsim/rsimUnitTest/CMakeFiles/rsimUnitTest
rsimUnitTest: apps/test/modemServices/rsim/rsimUnitTest/CMakeFiles/rsimUnitTest.dir/build.make

.PHONY : rsimUnitTest

# Rule to build all files generated by this target.
apps/test/modemServices/rsim/rsimUnitTest/CMakeFiles/rsimUnitTest.dir/build: rsimUnitTest

.PHONY : apps/test/modemServices/rsim/rsimUnitTest/CMakeFiles/rsimUnitTest.dir/build

apps/test/modemServices/rsim/rsimUnitTest/CMakeFiles/rsimUnitTest.dir/clean:
	cd /home/shrunoti/Downloads/workspace/legato/build/localhost/apps/test/modemServices/rsim/rsimUnitTest && $(CMAKE_COMMAND) -P CMakeFiles/rsimUnitTest.dir/cmake_clean.cmake
.PHONY : apps/test/modemServices/rsim/rsimUnitTest/CMakeFiles/rsimUnitTest.dir/clean

apps/test/modemServices/rsim/rsimUnitTest/CMakeFiles/rsimUnitTest.dir/depend:
	cd /home/shrunoti/Downloads/workspace/legato/build/localhost && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/shrunoti/Downloads/workspace/legato /home/shrunoti/Downloads/workspace/legato/apps/test/modemServices/rsim/rsimUnitTest /home/shrunoti/Downloads/workspace/legato/build/localhost /home/shrunoti/Downloads/workspace/legato/build/localhost/apps/test/modemServices/rsim/rsimUnitTest /home/shrunoti/Downloads/workspace/legato/build/localhost/apps/test/modemServices/rsim/rsimUnitTest/CMakeFiles/rsimUnitTest.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : apps/test/modemServices/rsim/rsimUnitTest/CMakeFiles/rsimUnitTest.dir/depend

