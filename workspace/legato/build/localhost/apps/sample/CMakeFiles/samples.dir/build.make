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

# Utility rule file for samples.

# Include the progress variables for this target.
include apps/sample/CMakeFiles/samples.dir/progress.make

apps/sample/CMakeFiles/samples:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/shrunoti/Downloads/workspace/legato/build/localhost/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Generated all samples in /home/shrunoti/Downloads/workspace/legato/build/localhost/samples/bin"

samples: apps/sample/CMakeFiles/samples
samples: apps/sample/CMakeFiles/samples.dir/build.make

.PHONY : samples

# Rule to build all files generated by this target.
apps/sample/CMakeFiles/samples.dir/build: samples

.PHONY : apps/sample/CMakeFiles/samples.dir/build

apps/sample/CMakeFiles/samples.dir/clean:
	cd /home/shrunoti/Downloads/workspace/legato/build/localhost/apps/sample && $(CMAKE_COMMAND) -P CMakeFiles/samples.dir/cmake_clean.cmake
.PHONY : apps/sample/CMakeFiles/samples.dir/clean

apps/sample/CMakeFiles/samples.dir/depend:
	cd /home/shrunoti/Downloads/workspace/legato/build/localhost && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/shrunoti/Downloads/workspace/legato /home/shrunoti/Downloads/workspace/legato/apps/sample /home/shrunoti/Downloads/workspace/legato/build/localhost /home/shrunoti/Downloads/workspace/legato/build/localhost/apps/sample /home/shrunoti/Downloads/workspace/legato/build/localhost/apps/sample/CMakeFiles/samples.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : apps/sample/CMakeFiles/samples.dir/depend

