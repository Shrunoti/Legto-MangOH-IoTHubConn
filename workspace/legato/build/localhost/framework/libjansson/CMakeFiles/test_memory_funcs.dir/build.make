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
CMAKE_SOURCE_DIR = /home/shrunoti/Downloads/workspace/legato/3rdParty/jansson

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/shrunoti/Downloads/workspace/legato/build/localhost/framework/libjansson

# Include any dependencies generated for this target.
include CMakeFiles/test_memory_funcs.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/test_memory_funcs.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/test_memory_funcs.dir/flags.make

CMakeFiles/test_memory_funcs.dir/test/suites/api/test_memory_funcs.c.o: CMakeFiles/test_memory_funcs.dir/flags.make
CMakeFiles/test_memory_funcs.dir/test/suites/api/test_memory_funcs.c.o: /home/shrunoti/Downloads/workspace/legato/3rdParty/jansson/test/suites/api/test_memory_funcs.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/shrunoti/Downloads/workspace/legato/build/localhost/framework/libjansson/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/test_memory_funcs.dir/test/suites/api/test_memory_funcs.c.o"
	/usr/bin//gcc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/test_memory_funcs.dir/test/suites/api/test_memory_funcs.c.o   -c /home/shrunoti/Downloads/workspace/legato/3rdParty/jansson/test/suites/api/test_memory_funcs.c

CMakeFiles/test_memory_funcs.dir/test/suites/api/test_memory_funcs.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/test_memory_funcs.dir/test/suites/api/test_memory_funcs.c.i"
	/usr/bin//gcc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/shrunoti/Downloads/workspace/legato/3rdParty/jansson/test/suites/api/test_memory_funcs.c > CMakeFiles/test_memory_funcs.dir/test/suites/api/test_memory_funcs.c.i

CMakeFiles/test_memory_funcs.dir/test/suites/api/test_memory_funcs.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/test_memory_funcs.dir/test/suites/api/test_memory_funcs.c.s"
	/usr/bin//gcc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/shrunoti/Downloads/workspace/legato/3rdParty/jansson/test/suites/api/test_memory_funcs.c -o CMakeFiles/test_memory_funcs.dir/test/suites/api/test_memory_funcs.c.s

CMakeFiles/test_memory_funcs.dir/test/suites/api/test_memory_funcs.c.o.requires:

.PHONY : CMakeFiles/test_memory_funcs.dir/test/suites/api/test_memory_funcs.c.o.requires

CMakeFiles/test_memory_funcs.dir/test/suites/api/test_memory_funcs.c.o.provides: CMakeFiles/test_memory_funcs.dir/test/suites/api/test_memory_funcs.c.o.requires
	$(MAKE) -f CMakeFiles/test_memory_funcs.dir/build.make CMakeFiles/test_memory_funcs.dir/test/suites/api/test_memory_funcs.c.o.provides.build
.PHONY : CMakeFiles/test_memory_funcs.dir/test/suites/api/test_memory_funcs.c.o.provides

CMakeFiles/test_memory_funcs.dir/test/suites/api/test_memory_funcs.c.o.provides.build: CMakeFiles/test_memory_funcs.dir/test/suites/api/test_memory_funcs.c.o


# Object files for target test_memory_funcs
test_memory_funcs_OBJECTS = \
"CMakeFiles/test_memory_funcs.dir/test/suites/api/test_memory_funcs.c.o"

# External object files for target test_memory_funcs
test_memory_funcs_EXTERNAL_OBJECTS =

bin/test_memory_funcs: CMakeFiles/test_memory_funcs.dir/test/suites/api/test_memory_funcs.c.o
bin/test_memory_funcs: CMakeFiles/test_memory_funcs.dir/build.make
bin/test_memory_funcs: lib/libjansson.so
bin/test_memory_funcs: CMakeFiles/test_memory_funcs.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/shrunoti/Downloads/workspace/legato/build/localhost/framework/libjansson/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable bin/test_memory_funcs"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/test_memory_funcs.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/test_memory_funcs.dir/build: bin/test_memory_funcs

.PHONY : CMakeFiles/test_memory_funcs.dir/build

CMakeFiles/test_memory_funcs.dir/requires: CMakeFiles/test_memory_funcs.dir/test/suites/api/test_memory_funcs.c.o.requires

.PHONY : CMakeFiles/test_memory_funcs.dir/requires

CMakeFiles/test_memory_funcs.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/test_memory_funcs.dir/cmake_clean.cmake
.PHONY : CMakeFiles/test_memory_funcs.dir/clean

CMakeFiles/test_memory_funcs.dir/depend:
	cd /home/shrunoti/Downloads/workspace/legato/build/localhost/framework/libjansson && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/shrunoti/Downloads/workspace/legato/3rdParty/jansson /home/shrunoti/Downloads/workspace/legato/3rdParty/jansson /home/shrunoti/Downloads/workspace/legato/build/localhost/framework/libjansson /home/shrunoti/Downloads/workspace/legato/build/localhost/framework/libjansson /home/shrunoti/Downloads/workspace/legato/build/localhost/framework/libjansson/CMakeFiles/test_memory_funcs.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/test_memory_funcs.dir/depend

