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

# Include any dependencies generated for this target.
include apps/sample/legacy/original-app/CMakeFiles/legacy-app-bin.dir/depend.make

# Include the progress variables for this target.
include apps/sample/legacy/original-app/CMakeFiles/legacy-app-bin.dir/progress.make

# Include the compile flags for this target's objects.
include apps/sample/legacy/original-app/CMakeFiles/legacy-app-bin.dir/flags.make

apps/sample/legacy/original-app/CMakeFiles/legacy-app-bin.dir/main.c.o: apps/sample/legacy/original-app/CMakeFiles/legacy-app-bin.dir/flags.make
apps/sample/legacy/original-app/CMakeFiles/legacy-app-bin.dir/main.c.o: ../../apps/sample/legacy/original-app/main.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/shrunoti/Downloads/workspace/legato/build/localhost/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object apps/sample/legacy/original-app/CMakeFiles/legacy-app-bin.dir/main.c.o"
	cd /home/shrunoti/Downloads/workspace/legato/build/localhost/apps/sample/legacy/original-app && /usr/bin//gcc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/legacy-app-bin.dir/main.c.o   -c /home/shrunoti/Downloads/workspace/legato/apps/sample/legacy/original-app/main.c

apps/sample/legacy/original-app/CMakeFiles/legacy-app-bin.dir/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/legacy-app-bin.dir/main.c.i"
	cd /home/shrunoti/Downloads/workspace/legato/build/localhost/apps/sample/legacy/original-app && /usr/bin//gcc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/shrunoti/Downloads/workspace/legato/apps/sample/legacy/original-app/main.c > CMakeFiles/legacy-app-bin.dir/main.c.i

apps/sample/legacy/original-app/CMakeFiles/legacy-app-bin.dir/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/legacy-app-bin.dir/main.c.s"
	cd /home/shrunoti/Downloads/workspace/legato/build/localhost/apps/sample/legacy/original-app && /usr/bin//gcc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/shrunoti/Downloads/workspace/legato/apps/sample/legacy/original-app/main.c -o CMakeFiles/legacy-app-bin.dir/main.c.s

apps/sample/legacy/original-app/CMakeFiles/legacy-app-bin.dir/main.c.o.requires:

.PHONY : apps/sample/legacy/original-app/CMakeFiles/legacy-app-bin.dir/main.c.o.requires

apps/sample/legacy/original-app/CMakeFiles/legacy-app-bin.dir/main.c.o.provides: apps/sample/legacy/original-app/CMakeFiles/legacy-app-bin.dir/main.c.o.requires
	$(MAKE) -f apps/sample/legacy/original-app/CMakeFiles/legacy-app-bin.dir/build.make apps/sample/legacy/original-app/CMakeFiles/legacy-app-bin.dir/main.c.o.provides.build
.PHONY : apps/sample/legacy/original-app/CMakeFiles/legacy-app-bin.dir/main.c.o.provides

apps/sample/legacy/original-app/CMakeFiles/legacy-app-bin.dir/main.c.o.provides.build: apps/sample/legacy/original-app/CMakeFiles/legacy-app-bin.dir/main.c.o


# Object files for target legacy-app-bin
legacy__app__bin_OBJECTS = \
"CMakeFiles/legacy-app-bin.dir/main.c.o"

# External object files for target legacy-app-bin
legacy__app__bin_EXTERNAL_OBJECTS =

samples/bin/legacy-app-bin: apps/sample/legacy/original-app/CMakeFiles/legacy-app-bin.dir/main.c.o
samples/bin/legacy-app-bin: apps/sample/legacy/original-app/CMakeFiles/legacy-app-bin.dir/build.make
samples/bin/legacy-app-bin: apps/sample/legacy/original-app/CMakeFiles/legacy-app-bin.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/shrunoti/Downloads/workspace/legato/build/localhost/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable ../../../../samples/bin/legacy-app-bin"
	cd /home/shrunoti/Downloads/workspace/legato/build/localhost/apps/sample/legacy/original-app && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/legacy-app-bin.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
apps/sample/legacy/original-app/CMakeFiles/legacy-app-bin.dir/build: samples/bin/legacy-app-bin

.PHONY : apps/sample/legacy/original-app/CMakeFiles/legacy-app-bin.dir/build

apps/sample/legacy/original-app/CMakeFiles/legacy-app-bin.dir/requires: apps/sample/legacy/original-app/CMakeFiles/legacy-app-bin.dir/main.c.o.requires

.PHONY : apps/sample/legacy/original-app/CMakeFiles/legacy-app-bin.dir/requires

apps/sample/legacy/original-app/CMakeFiles/legacy-app-bin.dir/clean:
	cd /home/shrunoti/Downloads/workspace/legato/build/localhost/apps/sample/legacy/original-app && $(CMAKE_COMMAND) -P CMakeFiles/legacy-app-bin.dir/cmake_clean.cmake
.PHONY : apps/sample/legacy/original-app/CMakeFiles/legacy-app-bin.dir/clean

apps/sample/legacy/original-app/CMakeFiles/legacy-app-bin.dir/depend:
	cd /home/shrunoti/Downloads/workspace/legato/build/localhost && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/shrunoti/Downloads/workspace/legato /home/shrunoti/Downloads/workspace/legato/apps/sample/legacy/original-app /home/shrunoti/Downloads/workspace/legato/build/localhost /home/shrunoti/Downloads/workspace/legato/build/localhost/apps/sample/legacy/original-app /home/shrunoti/Downloads/workspace/legato/build/localhost/apps/sample/legacy/original-app/CMakeFiles/legacy-app-bin.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : apps/sample/legacy/original-app/CMakeFiles/legacy-app-bin.dir/depend
