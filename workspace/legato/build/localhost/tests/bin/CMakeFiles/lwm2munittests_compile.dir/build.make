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

# Utility rule file for lwm2munittests_compile.

# Include the progress variables for this target.
include tests/bin/CMakeFiles/lwm2munittests_compile.dir/progress.make

tests/bin/CMakeFiles/lwm2munittests_compile:
	cd /home/shrunoti/Downloads/workspace/legato/build/localhost/tests/bin && make

lwm2munittests_compile: tests/bin/CMakeFiles/lwm2munittests_compile
lwm2munittests_compile: tests/bin/CMakeFiles/lwm2munittests_compile.dir/build.make

.PHONY : lwm2munittests_compile

# Rule to build all files generated by this target.
tests/bin/CMakeFiles/lwm2munittests_compile.dir/build: lwm2munittests_compile

.PHONY : tests/bin/CMakeFiles/lwm2munittests_compile.dir/build

tests/bin/CMakeFiles/lwm2munittests_compile.dir/clean:
	cd /home/shrunoti/Downloads/workspace/legato/build/localhost/tests/bin && $(CMAKE_COMMAND) -P CMakeFiles/lwm2munittests_compile.dir/cmake_clean.cmake
.PHONY : tests/bin/CMakeFiles/lwm2munittests_compile.dir/clean

tests/bin/CMakeFiles/lwm2munittests_compile.dir/depend:
	cd /home/shrunoti/Downloads/workspace/legato/build/localhost && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/shrunoti/Downloads/workspace/legato /home/shrunoti/Downloads/workspace/legato/3rdParty/Lwm2mCore/tests /home/shrunoti/Downloads/workspace/legato/build/localhost /home/shrunoti/Downloads/workspace/legato/build/localhost/tests/bin /home/shrunoti/Downloads/workspace/legato/build/localhost/tests/bin/CMakeFiles/lwm2munittests_compile.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : tests/bin/CMakeFiles/lwm2munittests_compile.dir/depend

