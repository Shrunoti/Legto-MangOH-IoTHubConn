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

# Utility rule file for ecallUnitTest.

# Include the progress variables for this target.
include apps/test/modemServices/ecall/ecallUnitTest/CMakeFiles/ecallUnitTest.dir/progress.make

apps/test/modemServices/ecall/ecallUnitTest/CMakeFiles/ecallUnitTest:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/shrunoti/Downloads/workspace/legato/build/localhost/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "mkexe 'ecallUnitTest': /home/shrunoti/Downloads/workspace/legato/build/localhost/tests/bin/ecallUnitTest"
	cd /home/shrunoti/Downloads/workspace/legato/apps/test/modemServices/ecall/ecallUnitTest && /home/shrunoti/Downloads/workspace/legato/bin/mkexe -o /home/shrunoti/Downloads/workspace/legato/build/localhost/tests/bin/ecallUnitTest -w /home/shrunoti/Downloads/workspace/legato/build/localhost/apps/test/modemServices/ecall/ecallUnitTest/_build_ecallUnitTest -i /home/shrunoti/Downloads/workspace/legato/apps/test/modemServices/ecall/ecallUnitTest -l /home/shrunoti/Downloads/workspace/legato/build/localhost/tests/lib -t localhost . -i /home/shrunoti/Downloads/workspace/legato/components/modemServices//modemDaemon -i /home/shrunoti/Downloads/workspace/legato/components/modemServices//platformAdaptor/inc -i /home/shrunoti/Downloads/workspace/legato/framework/liblegato -i /home/shrunoti/Downloads/workspace/legato/components/cfgEntries -i /home/shrunoti/Downloads/workspace/legato/platformAdaptor/simu/components/le_pa -i /home/shrunoti/Downloads/workspace/legato/platformAdaptor/simu/components/le_pa_ecall -i /home/shrunoti/Downloads/workspace/legato/apps/test/modemServices/ecall/ecallUnitTest/simu/ -s /home/shrunoti/Downloads/workspace/legato/platformAdaptor --cflags=-DINCLUDE_ECALL=1

ecallUnitTest: apps/test/modemServices/ecall/ecallUnitTest/CMakeFiles/ecallUnitTest
ecallUnitTest: apps/test/modemServices/ecall/ecallUnitTest/CMakeFiles/ecallUnitTest.dir/build.make

.PHONY : ecallUnitTest

# Rule to build all files generated by this target.
apps/test/modemServices/ecall/ecallUnitTest/CMakeFiles/ecallUnitTest.dir/build: ecallUnitTest

.PHONY : apps/test/modemServices/ecall/ecallUnitTest/CMakeFiles/ecallUnitTest.dir/build

apps/test/modemServices/ecall/ecallUnitTest/CMakeFiles/ecallUnitTest.dir/clean:
	cd /home/shrunoti/Downloads/workspace/legato/build/localhost/apps/test/modemServices/ecall/ecallUnitTest && $(CMAKE_COMMAND) -P CMakeFiles/ecallUnitTest.dir/cmake_clean.cmake
.PHONY : apps/test/modemServices/ecall/ecallUnitTest/CMakeFiles/ecallUnitTest.dir/clean

apps/test/modemServices/ecall/ecallUnitTest/CMakeFiles/ecallUnitTest.dir/depend:
	cd /home/shrunoti/Downloads/workspace/legato/build/localhost && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/shrunoti/Downloads/workspace/legato /home/shrunoti/Downloads/workspace/legato/apps/test/modemServices/ecall/ecallUnitTest /home/shrunoti/Downloads/workspace/legato/build/localhost /home/shrunoti/Downloads/workspace/legato/build/localhost/apps/test/modemServices/ecall/ecallUnitTest /home/shrunoti/Downloads/workspace/legato/build/localhost/apps/test/modemServices/ecall/ecallUnitTest/CMakeFiles/ecallUnitTest.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : apps/test/modemServices/ecall/ecallUnitTest/CMakeFiles/ecallUnitTest.dir/depend

