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

# Utility rule file for mkdoc_gen.

# Include the progress variables for this target.
include framework/doc/CMakeFiles/mkdoc_gen.dir/progress.make

framework/doc/CMakeFiles/mkdoc_gen:
	cd /home/shrunoti/Downloads/workspace/legato/build/localhost/framework/doc && ../../../../bin/mkdoc /home/shrunoti/Downloads/workspace/legato/build/localhost/framework/doc/generated

mkdoc_gen: framework/doc/CMakeFiles/mkdoc_gen
mkdoc_gen: framework/doc/CMakeFiles/mkdoc_gen.dir/build.make

.PHONY : mkdoc_gen

# Rule to build all files generated by this target.
framework/doc/CMakeFiles/mkdoc_gen.dir/build: mkdoc_gen

.PHONY : framework/doc/CMakeFiles/mkdoc_gen.dir/build

framework/doc/CMakeFiles/mkdoc_gen.dir/clean:
	cd /home/shrunoti/Downloads/workspace/legato/build/localhost/framework/doc && $(CMAKE_COMMAND) -P CMakeFiles/mkdoc_gen.dir/cmake_clean.cmake
.PHONY : framework/doc/CMakeFiles/mkdoc_gen.dir/clean

framework/doc/CMakeFiles/mkdoc_gen.dir/depend:
	cd /home/shrunoti/Downloads/workspace/legato/build/localhost && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/shrunoti/Downloads/workspace/legato /home/shrunoti/Downloads/workspace/legato/framework/doc /home/shrunoti/Downloads/workspace/legato/build/localhost /home/shrunoti/Downloads/workspace/legato/build/localhost/framework/doc /home/shrunoti/Downloads/workspace/legato/build/localhost/framework/doc/CMakeFiles/mkdoc_gen.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : framework/doc/CMakeFiles/mkdoc_gen.dir/depend

