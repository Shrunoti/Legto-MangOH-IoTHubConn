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

# Utility rule file for le_audio_if.

# Include the progress variables for this target.
include interfaces/CMakeFiles/le_audio_if.dir/progress.make

interfaces/CMakeFiles/le_audio_if: interfaces/le_audio_interface.h


interfaces/le_audio_interface.h: ../../interfaces/le_audio.api
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/shrunoti/Downloads/workspace/legato/build/localhost/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "ifgen 'le_audio.api': /home/shrunoti/Downloads/workspace/legato/build/localhost/interfaces/le_audio_interface.h"
	cd /home/shrunoti/Downloads/workspace/legato/build/localhost/interfaces && ../../../bin/ifgen --gen-interface /home/shrunoti/Downloads/workspace/legato/interfaces/le_audio.api --import-dir /home/shrunoti/Downloads/workspace/legato/interfaces/audio --import-dir /home/shrunoti/Downloads/workspace/legato/interfaces/modemServices --import-dir /home/shrunoti/Downloads/workspace/legato/interfaces/atServices --import-dir /home/shrunoti/Downloads/workspace/legato/interfaces

le_audio_if: interfaces/CMakeFiles/le_audio_if
le_audio_if: interfaces/le_audio_interface.h
le_audio_if: interfaces/CMakeFiles/le_audio_if.dir/build.make

.PHONY : le_audio_if

# Rule to build all files generated by this target.
interfaces/CMakeFiles/le_audio_if.dir/build: le_audio_if

.PHONY : interfaces/CMakeFiles/le_audio_if.dir/build

interfaces/CMakeFiles/le_audio_if.dir/clean:
	cd /home/shrunoti/Downloads/workspace/legato/build/localhost/interfaces && $(CMAKE_COMMAND) -P CMakeFiles/le_audio_if.dir/cmake_clean.cmake
.PHONY : interfaces/CMakeFiles/le_audio_if.dir/clean

interfaces/CMakeFiles/le_audio_if.dir/depend:
	cd /home/shrunoti/Downloads/workspace/legato/build/localhost && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/shrunoti/Downloads/workspace/legato /home/shrunoti/Downloads/workspace/legato/interfaces /home/shrunoti/Downloads/workspace/legato/build/localhost /home/shrunoti/Downloads/workspace/legato/build/localhost/interfaces /home/shrunoti/Downloads/workspace/legato/build/localhost/interfaces/CMakeFiles/le_audio_if.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : interfaces/CMakeFiles/le_audio_if.dir/depend

