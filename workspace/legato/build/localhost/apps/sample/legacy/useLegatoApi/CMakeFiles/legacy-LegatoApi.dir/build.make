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

# Utility rule file for legacy-LegatoApi.

# Include the progress variables for this target.
include apps/sample/legacy/useLegatoApi/CMakeFiles/legacy-LegatoApi.dir/progress.make

apps/sample/legacy/useLegatoApi/CMakeFiles/legacy-LegatoApi:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/shrunoti/Downloads/workspace/legato/build/localhost/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "mkapp 'legacy-LegatoApi': /home/shrunoti/Downloads/workspace/legato/build/localhost/samples/legacy-LegatoApi.localhost"
	cd /home/shrunoti/Downloads/workspace/legato/apps/sample/legacy/useLegatoApi && PATH=/home/shrunoti/Downloads/workspace/legato/bin:/usr/bin/:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/snap/bin:/home/shrunoti/Downloads/workspace/legato/bin:/home/shrunoti/Downloads/workspace/legato/bin /home/shrunoti/Downloads/workspace/legato/bin/mkapp legacy-LegatoApi.adef -t localhost -w /home/shrunoti/Downloads/workspace/legato/build/localhost/apps/sample/legacy/useLegatoApi/_build_legacy-LegatoApi.localhost -i /home/shrunoti/Downloads/workspace/legato/apps/sample/legacy/useLegatoApi -c /home/shrunoti/Downloads/workspace/legato/apps/sample/legacy/useLegatoApi -o /home/shrunoti/Downloads/workspace/legato/build/localhost/samples

legacy-LegatoApi: apps/sample/legacy/useLegatoApi/CMakeFiles/legacy-LegatoApi
legacy-LegatoApi: apps/sample/legacy/useLegatoApi/CMakeFiles/legacy-LegatoApi.dir/build.make

.PHONY : legacy-LegatoApi

# Rule to build all files generated by this target.
apps/sample/legacy/useLegatoApi/CMakeFiles/legacy-LegatoApi.dir/build: legacy-LegatoApi

.PHONY : apps/sample/legacy/useLegatoApi/CMakeFiles/legacy-LegatoApi.dir/build

apps/sample/legacy/useLegatoApi/CMakeFiles/legacy-LegatoApi.dir/clean:
	cd /home/shrunoti/Downloads/workspace/legato/build/localhost/apps/sample/legacy/useLegatoApi && $(CMAKE_COMMAND) -P CMakeFiles/legacy-LegatoApi.dir/cmake_clean.cmake
.PHONY : apps/sample/legacy/useLegatoApi/CMakeFiles/legacy-LegatoApi.dir/clean

apps/sample/legacy/useLegatoApi/CMakeFiles/legacy-LegatoApi.dir/depend:
	cd /home/shrunoti/Downloads/workspace/legato/build/localhost && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/shrunoti/Downloads/workspace/legato /home/shrunoti/Downloads/workspace/legato/apps/sample/legacy/useLegatoApi /home/shrunoti/Downloads/workspace/legato/build/localhost /home/shrunoti/Downloads/workspace/legato/build/localhost/apps/sample/legacy/useLegatoApi /home/shrunoti/Downloads/workspace/legato/build/localhost/apps/sample/legacy/useLegatoApi/CMakeFiles/legacy-LegatoApi.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : apps/sample/legacy/useLegatoApi/CMakeFiles/legacy-LegatoApi.dir/depend

