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

# Utility rule file for testFwMessaging-Test3-client.

# Include the progress variables for this target.
include apps/test/framework/messaging/CMakeFiles/testFwMessaging-Test3-client.dir/progress.make

apps/test/framework/messaging/CMakeFiles/testFwMessaging-Test3-client:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/shrunoti/Downloads/workspace/legato/build/localhost/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "mkexe 'testFwMessaging-Test3-client': /home/shrunoti/Downloads/workspace/legato/build/localhost/tests/bin/testFwMessaging-Test3-client"
	cd /home/shrunoti/Downloads/workspace/legato/apps/test/framework/messaging && /home/shrunoti/Downloads/workspace/legato/bin/mkexe -o /home/shrunoti/Downloads/workspace/legato/build/localhost/tests/bin/testFwMessaging-Test3-client -w /home/shrunoti/Downloads/workspace/legato/build/localhost/apps/test/framework/messaging/_build_testFwMessaging-Test3-client -i /home/shrunoti/Downloads/workspace/legato/apps/test/framework/messaging -l /home/shrunoti/Downloads/workspace/legato/build/localhost/tests/lib -t localhost messagingTest3-client.c

testFwMessaging-Test3-client: apps/test/framework/messaging/CMakeFiles/testFwMessaging-Test3-client
testFwMessaging-Test3-client: apps/test/framework/messaging/CMakeFiles/testFwMessaging-Test3-client.dir/build.make

.PHONY : testFwMessaging-Test3-client

# Rule to build all files generated by this target.
apps/test/framework/messaging/CMakeFiles/testFwMessaging-Test3-client.dir/build: testFwMessaging-Test3-client

.PHONY : apps/test/framework/messaging/CMakeFiles/testFwMessaging-Test3-client.dir/build

apps/test/framework/messaging/CMakeFiles/testFwMessaging-Test3-client.dir/clean:
	cd /home/shrunoti/Downloads/workspace/legato/build/localhost/apps/test/framework/messaging && $(CMAKE_COMMAND) -P CMakeFiles/testFwMessaging-Test3-client.dir/cmake_clean.cmake
.PHONY : apps/test/framework/messaging/CMakeFiles/testFwMessaging-Test3-client.dir/clean

apps/test/framework/messaging/CMakeFiles/testFwMessaging-Test3-client.dir/depend:
	cd /home/shrunoti/Downloads/workspace/legato/build/localhost && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/shrunoti/Downloads/workspace/legato /home/shrunoti/Downloads/workspace/legato/apps/test/framework/messaging /home/shrunoti/Downloads/workspace/legato/build/localhost /home/shrunoti/Downloads/workspace/legato/build/localhost/apps/test/framework/messaging /home/shrunoti/Downloads/workspace/legato/build/localhost/apps/test/framework/messaging/CMakeFiles/testFwMessaging-Test3-client.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : apps/test/framework/messaging/CMakeFiles/testFwMessaging-Test3-client.dir/depend

