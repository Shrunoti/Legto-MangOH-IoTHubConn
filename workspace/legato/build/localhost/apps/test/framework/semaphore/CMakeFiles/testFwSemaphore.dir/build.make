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

# Utility rule file for testFwSemaphore.

# Include the progress variables for this target.
include apps/test/framework/semaphore/CMakeFiles/testFwSemaphore.dir/progress.make

apps/test/framework/semaphore/CMakeFiles/testFwSemaphore:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/shrunoti/Downloads/workspace/legato/build/localhost/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "mkexe 'testFwSemaphore': /home/shrunoti/Downloads/workspace/legato/build/localhost/tests/bin/testFwSemaphore"
	cd /home/shrunoti/Downloads/workspace/legato/apps/test/framework/semaphore && /home/shrunoti/Downloads/workspace/legato/bin/mkexe -o /home/shrunoti/Downloads/workspace/legato/build/localhost/tests/bin/testFwSemaphore -w /home/shrunoti/Downloads/workspace/legato/build/localhost/apps/test/framework/semaphore/_build_testFwSemaphore -i /home/shrunoti/Downloads/workspace/legato/apps/test/framework/semaphore -l /home/shrunoti/Downloads/workspace/legato/build/localhost/tests/lib -t localhost test_le_semaphore.c -i /home/shrunoti/Downloads/workspace/legato/build/localhost/CUnit/include /home/shrunoti/Downloads/workspace/legato/build/localhost/CUnit/lib/libcunit.a

testFwSemaphore: apps/test/framework/semaphore/CMakeFiles/testFwSemaphore
testFwSemaphore: apps/test/framework/semaphore/CMakeFiles/testFwSemaphore.dir/build.make

.PHONY : testFwSemaphore

# Rule to build all files generated by this target.
apps/test/framework/semaphore/CMakeFiles/testFwSemaphore.dir/build: testFwSemaphore

.PHONY : apps/test/framework/semaphore/CMakeFiles/testFwSemaphore.dir/build

apps/test/framework/semaphore/CMakeFiles/testFwSemaphore.dir/clean:
	cd /home/shrunoti/Downloads/workspace/legato/build/localhost/apps/test/framework/semaphore && $(CMAKE_COMMAND) -P CMakeFiles/testFwSemaphore.dir/cmake_clean.cmake
.PHONY : apps/test/framework/semaphore/CMakeFiles/testFwSemaphore.dir/clean

apps/test/framework/semaphore/CMakeFiles/testFwSemaphore.dir/depend:
	cd /home/shrunoti/Downloads/workspace/legato/build/localhost && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/shrunoti/Downloads/workspace/legato /home/shrunoti/Downloads/workspace/legato/apps/test/framework/semaphore /home/shrunoti/Downloads/workspace/legato/build/localhost /home/shrunoti/Downloads/workspace/legato/build/localhost/apps/test/framework/semaphore /home/shrunoti/Downloads/workspace/legato/build/localhost/apps/test/framework/semaphore/CMakeFiles/testFwSemaphore.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : apps/test/framework/semaphore/CMakeFiles/testFwSemaphore.dir/depend

