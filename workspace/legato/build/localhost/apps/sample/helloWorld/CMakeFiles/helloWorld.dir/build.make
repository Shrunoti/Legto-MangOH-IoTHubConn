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

# Utility rule file for helloWorld.

# Include the progress variables for this target.
include apps/sample/helloWorld/CMakeFiles/helloWorld.dir/progress.make

apps/sample/helloWorld/CMakeFiles/helloWorld:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/shrunoti/Downloads/workspace/legato/build/localhost/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "mkapp 'helloWorld': /home/shrunoti/Downloads/workspace/legato/build/localhost/samples/helloWorld.localhost"
	cd /home/shrunoti/Downloads/workspace/legato/apps/sample/helloWorld && PATH=/home/shrunoti/Downloads/workspace/legato/bin:/usr/bin/:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/snap/bin:/home/shrunoti/Downloads/workspace/legato/bin:/home/shrunoti/Downloads/workspace/legato/bin /home/shrunoti/Downloads/workspace/legato/bin/mkapp helloWorld.adef -t localhost -w /home/shrunoti/Downloads/workspace/legato/build/localhost/apps/sample/helloWorld/_build_helloWorld.localhost -i /home/shrunoti/Downloads/workspace/legato/apps/sample/helloWorld -c /home/shrunoti/Downloads/workspace/legato/apps/sample/helloWorld -o /home/shrunoti/Downloads/workspace/legato/build/localhost/samples

helloWorld: apps/sample/helloWorld/CMakeFiles/helloWorld
helloWorld: apps/sample/helloWorld/CMakeFiles/helloWorld.dir/build.make

.PHONY : helloWorld

# Rule to build all files generated by this target.
apps/sample/helloWorld/CMakeFiles/helloWorld.dir/build: helloWorld

.PHONY : apps/sample/helloWorld/CMakeFiles/helloWorld.dir/build

apps/sample/helloWorld/CMakeFiles/helloWorld.dir/clean:
	cd /home/shrunoti/Downloads/workspace/legato/build/localhost/apps/sample/helloWorld && $(CMAKE_COMMAND) -P CMakeFiles/helloWorld.dir/cmake_clean.cmake
.PHONY : apps/sample/helloWorld/CMakeFiles/helloWorld.dir/clean

apps/sample/helloWorld/CMakeFiles/helloWorld.dir/depend:
	cd /home/shrunoti/Downloads/workspace/legato/build/localhost && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/shrunoti/Downloads/workspace/legato /home/shrunoti/Downloads/workspace/legato/apps/sample/helloWorld /home/shrunoti/Downloads/workspace/legato/build/localhost /home/shrunoti/Downloads/workspace/legato/build/localhost/apps/sample/helloWorld /home/shrunoti/Downloads/workspace/legato/build/localhost/apps/sample/helloWorld/CMakeFiles/helloWorld.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : apps/sample/helloWorld/CMakeFiles/helloWorld.dir/depend

