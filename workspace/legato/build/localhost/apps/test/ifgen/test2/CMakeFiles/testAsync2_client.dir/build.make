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
include apps/test/ifgen/test2/CMakeFiles/testAsync2_client.dir/depend.make

# Include the progress variables for this target.
include apps/test/ifgen/test2/CMakeFiles/testAsync2_client.dir/progress.make

# Include the compile flags for this target's objects.
include apps/test/ifgen/test2/CMakeFiles/testAsync2_client.dir/flags.make

apps/test/ifgen/test2/example_client.c: ../../apps/test/ifgen/test2/example.api
apps/test/ifgen/test2/example_client.c: apps/test/ifgen/test2/common_interface.h
apps/test/ifgen/test2/example_client.c: apps/test/ifgen/test2/common_server.h
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/shrunoti/Downloads/workspace/legato/build/localhost/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Generating example_client.c, example_server.c"
	cd /home/shrunoti/Downloads/workspace/legato/build/localhost/apps/test/ifgen/test2 && ../../../../../../bin/ifgen /home/shrunoti/Downloads/workspace/legato/apps/test/ifgen/test2/example.api --gen-all --name-prefix=example

apps/test/ifgen/test2/example_server.c: apps/test/ifgen/test2/example_client.c
	@$(CMAKE_COMMAND) -E touch_nocreate apps/test/ifgen/test2/example_server.c

apps/test/ifgen/test2/common_interface.h: ../../apps/test/ifgen/test2/common.api
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/shrunoti/Downloads/workspace/legato/build/localhost/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Generating common_interface.h, common_server.h"
	cd /home/shrunoti/Downloads/workspace/legato/build/localhost/apps/test/ifgen/test2 && ../../../../../../bin/ifgen /home/shrunoti/Downloads/workspace/legato/apps/test/ifgen/test2/common.api --gen-interface --gen-server-interface

apps/test/ifgen/test2/common_server.h: apps/test/ifgen/test2/common_interface.h
	@$(CMAKE_COMMAND) -E touch_nocreate apps/test/ifgen/test2/common_server.h

apps/test/ifgen/test2/CMakeFiles/testAsync2_client.dir/__/__/__/__/framework/liblegato/codegen/_le_main.c.o: apps/test/ifgen/test2/CMakeFiles/testAsync2_client.dir/flags.make
apps/test/ifgen/test2/CMakeFiles/testAsync2_client.dir/__/__/__/__/framework/liblegato/codegen/_le_main.c.o: ../../framework/liblegato/codegen/_le_main.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/shrunoti/Downloads/workspace/legato/build/localhost/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object apps/test/ifgen/test2/CMakeFiles/testAsync2_client.dir/__/__/__/__/framework/liblegato/codegen/_le_main.c.o"
	cd /home/shrunoti/Downloads/workspace/legato/build/localhost/apps/test/ifgen/test2 && /usr/bin//gcc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/testAsync2_client.dir/__/__/__/__/framework/liblegato/codegen/_le_main.c.o   -c /home/shrunoti/Downloads/workspace/legato/framework/liblegato/codegen/_le_main.c

apps/test/ifgen/test2/CMakeFiles/testAsync2_client.dir/__/__/__/__/framework/liblegato/codegen/_le_main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/testAsync2_client.dir/__/__/__/__/framework/liblegato/codegen/_le_main.c.i"
	cd /home/shrunoti/Downloads/workspace/legato/build/localhost/apps/test/ifgen/test2 && /usr/bin//gcc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/shrunoti/Downloads/workspace/legato/framework/liblegato/codegen/_le_main.c > CMakeFiles/testAsync2_client.dir/__/__/__/__/framework/liblegato/codegen/_le_main.c.i

apps/test/ifgen/test2/CMakeFiles/testAsync2_client.dir/__/__/__/__/framework/liblegato/codegen/_le_main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/testAsync2_client.dir/__/__/__/__/framework/liblegato/codegen/_le_main.c.s"
	cd /home/shrunoti/Downloads/workspace/legato/build/localhost/apps/test/ifgen/test2 && /usr/bin//gcc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/shrunoti/Downloads/workspace/legato/framework/liblegato/codegen/_le_main.c -o CMakeFiles/testAsync2_client.dir/__/__/__/__/framework/liblegato/codegen/_le_main.c.s

apps/test/ifgen/test2/CMakeFiles/testAsync2_client.dir/__/__/__/__/framework/liblegato/codegen/_le_main.c.o.requires:

.PHONY : apps/test/ifgen/test2/CMakeFiles/testAsync2_client.dir/__/__/__/__/framework/liblegato/codegen/_le_main.c.o.requires

apps/test/ifgen/test2/CMakeFiles/testAsync2_client.dir/__/__/__/__/framework/liblegato/codegen/_le_main.c.o.provides: apps/test/ifgen/test2/CMakeFiles/testAsync2_client.dir/__/__/__/__/framework/liblegato/codegen/_le_main.c.o.requires
	$(MAKE) -f apps/test/ifgen/test2/CMakeFiles/testAsync2_client.dir/build.make apps/test/ifgen/test2/CMakeFiles/testAsync2_client.dir/__/__/__/__/framework/liblegato/codegen/_le_main.c.o.provides.build
.PHONY : apps/test/ifgen/test2/CMakeFiles/testAsync2_client.dir/__/__/__/__/framework/liblegato/codegen/_le_main.c.o.provides

apps/test/ifgen/test2/CMakeFiles/testAsync2_client.dir/__/__/__/__/framework/liblegato/codegen/_le_main.c.o.provides.build: apps/test/ifgen/test2/CMakeFiles/testAsync2_client.dir/__/__/__/__/framework/liblegato/codegen/_le_main.c.o


apps/test/ifgen/test2/CMakeFiles/testAsync2_client.dir/example_client.c.o: apps/test/ifgen/test2/CMakeFiles/testAsync2_client.dir/flags.make
apps/test/ifgen/test2/CMakeFiles/testAsync2_client.dir/example_client.c.o: apps/test/ifgen/test2/example_client.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/shrunoti/Downloads/workspace/legato/build/localhost/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building C object apps/test/ifgen/test2/CMakeFiles/testAsync2_client.dir/example_client.c.o"
	cd /home/shrunoti/Downloads/workspace/legato/build/localhost/apps/test/ifgen/test2 && /usr/bin//gcc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/testAsync2_client.dir/example_client.c.o   -c /home/shrunoti/Downloads/workspace/legato/build/localhost/apps/test/ifgen/test2/example_client.c

apps/test/ifgen/test2/CMakeFiles/testAsync2_client.dir/example_client.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/testAsync2_client.dir/example_client.c.i"
	cd /home/shrunoti/Downloads/workspace/legato/build/localhost/apps/test/ifgen/test2 && /usr/bin//gcc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/shrunoti/Downloads/workspace/legato/build/localhost/apps/test/ifgen/test2/example_client.c > CMakeFiles/testAsync2_client.dir/example_client.c.i

apps/test/ifgen/test2/CMakeFiles/testAsync2_client.dir/example_client.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/testAsync2_client.dir/example_client.c.s"
	cd /home/shrunoti/Downloads/workspace/legato/build/localhost/apps/test/ifgen/test2 && /usr/bin//gcc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/shrunoti/Downloads/workspace/legato/build/localhost/apps/test/ifgen/test2/example_client.c -o CMakeFiles/testAsync2_client.dir/example_client.c.s

apps/test/ifgen/test2/CMakeFiles/testAsync2_client.dir/example_client.c.o.requires:

.PHONY : apps/test/ifgen/test2/CMakeFiles/testAsync2_client.dir/example_client.c.o.requires

apps/test/ifgen/test2/CMakeFiles/testAsync2_client.dir/example_client.c.o.provides: apps/test/ifgen/test2/CMakeFiles/testAsync2_client.dir/example_client.c.o.requires
	$(MAKE) -f apps/test/ifgen/test2/CMakeFiles/testAsync2_client.dir/build.make apps/test/ifgen/test2/CMakeFiles/testAsync2_client.dir/example_client.c.o.provides.build
.PHONY : apps/test/ifgen/test2/CMakeFiles/testAsync2_client.dir/example_client.c.o.provides

apps/test/ifgen/test2/CMakeFiles/testAsync2_client.dir/example_client.c.o.provides.build: apps/test/ifgen/test2/CMakeFiles/testAsync2_client.dir/example_client.c.o


apps/test/ifgen/test2/CMakeFiles/testAsync2_client.dir/clientMain.c.o: apps/test/ifgen/test2/CMakeFiles/testAsync2_client.dir/flags.make
apps/test/ifgen/test2/CMakeFiles/testAsync2_client.dir/clientMain.c.o: ../../apps/test/ifgen/test2/clientMain.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/shrunoti/Downloads/workspace/legato/build/localhost/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building C object apps/test/ifgen/test2/CMakeFiles/testAsync2_client.dir/clientMain.c.o"
	cd /home/shrunoti/Downloads/workspace/legato/build/localhost/apps/test/ifgen/test2 && /usr/bin//gcc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/testAsync2_client.dir/clientMain.c.o   -c /home/shrunoti/Downloads/workspace/legato/apps/test/ifgen/test2/clientMain.c

apps/test/ifgen/test2/CMakeFiles/testAsync2_client.dir/clientMain.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/testAsync2_client.dir/clientMain.c.i"
	cd /home/shrunoti/Downloads/workspace/legato/build/localhost/apps/test/ifgen/test2 && /usr/bin//gcc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/shrunoti/Downloads/workspace/legato/apps/test/ifgen/test2/clientMain.c > CMakeFiles/testAsync2_client.dir/clientMain.c.i

apps/test/ifgen/test2/CMakeFiles/testAsync2_client.dir/clientMain.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/testAsync2_client.dir/clientMain.c.s"
	cd /home/shrunoti/Downloads/workspace/legato/build/localhost/apps/test/ifgen/test2 && /usr/bin//gcc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/shrunoti/Downloads/workspace/legato/apps/test/ifgen/test2/clientMain.c -o CMakeFiles/testAsync2_client.dir/clientMain.c.s

apps/test/ifgen/test2/CMakeFiles/testAsync2_client.dir/clientMain.c.o.requires:

.PHONY : apps/test/ifgen/test2/CMakeFiles/testAsync2_client.dir/clientMain.c.o.requires

apps/test/ifgen/test2/CMakeFiles/testAsync2_client.dir/clientMain.c.o.provides: apps/test/ifgen/test2/CMakeFiles/testAsync2_client.dir/clientMain.c.o.requires
	$(MAKE) -f apps/test/ifgen/test2/CMakeFiles/testAsync2_client.dir/build.make apps/test/ifgen/test2/CMakeFiles/testAsync2_client.dir/clientMain.c.o.provides.build
.PHONY : apps/test/ifgen/test2/CMakeFiles/testAsync2_client.dir/clientMain.c.o.provides

apps/test/ifgen/test2/CMakeFiles/testAsync2_client.dir/clientMain.c.o.provides.build: apps/test/ifgen/test2/CMakeFiles/testAsync2_client.dir/clientMain.c.o


# Object files for target testAsync2_client
testAsync2_client_OBJECTS = \
"CMakeFiles/testAsync2_client.dir/__/__/__/__/framework/liblegato/codegen/_le_main.c.o" \
"CMakeFiles/testAsync2_client.dir/example_client.c.o" \
"CMakeFiles/testAsync2_client.dir/clientMain.c.o"

# External object files for target testAsync2_client
testAsync2_client_EXTERNAL_OBJECTS =

tests/bin/testAsync2_client: apps/test/ifgen/test2/CMakeFiles/testAsync2_client.dir/__/__/__/__/framework/liblegato/codegen/_le_main.c.o
tests/bin/testAsync2_client: apps/test/ifgen/test2/CMakeFiles/testAsync2_client.dir/example_client.c.o
tests/bin/testAsync2_client: apps/test/ifgen/test2/CMakeFiles/testAsync2_client.dir/clientMain.c.o
tests/bin/testAsync2_client: apps/test/ifgen/test2/CMakeFiles/testAsync2_client.dir/build.make
tests/bin/testAsync2_client: framework/lib/liblegato.so
tests/bin/testAsync2_client: apps/test/ifgen/test2/CMakeFiles/testAsync2_client.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/shrunoti/Downloads/workspace/legato/build/localhost/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Linking C executable ../../../../tests/bin/testAsync2_client"
	cd /home/shrunoti/Downloads/workspace/legato/build/localhost/apps/test/ifgen/test2 && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/testAsync2_client.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
apps/test/ifgen/test2/CMakeFiles/testAsync2_client.dir/build: tests/bin/testAsync2_client

.PHONY : apps/test/ifgen/test2/CMakeFiles/testAsync2_client.dir/build

apps/test/ifgen/test2/CMakeFiles/testAsync2_client.dir/requires: apps/test/ifgen/test2/CMakeFiles/testAsync2_client.dir/__/__/__/__/framework/liblegato/codegen/_le_main.c.o.requires
apps/test/ifgen/test2/CMakeFiles/testAsync2_client.dir/requires: apps/test/ifgen/test2/CMakeFiles/testAsync2_client.dir/example_client.c.o.requires
apps/test/ifgen/test2/CMakeFiles/testAsync2_client.dir/requires: apps/test/ifgen/test2/CMakeFiles/testAsync2_client.dir/clientMain.c.o.requires

.PHONY : apps/test/ifgen/test2/CMakeFiles/testAsync2_client.dir/requires

apps/test/ifgen/test2/CMakeFiles/testAsync2_client.dir/clean:
	cd /home/shrunoti/Downloads/workspace/legato/build/localhost/apps/test/ifgen/test2 && $(CMAKE_COMMAND) -P CMakeFiles/testAsync2_client.dir/cmake_clean.cmake
.PHONY : apps/test/ifgen/test2/CMakeFiles/testAsync2_client.dir/clean

apps/test/ifgen/test2/CMakeFiles/testAsync2_client.dir/depend: apps/test/ifgen/test2/example_client.c
apps/test/ifgen/test2/CMakeFiles/testAsync2_client.dir/depend: apps/test/ifgen/test2/example_server.c
apps/test/ifgen/test2/CMakeFiles/testAsync2_client.dir/depend: apps/test/ifgen/test2/common_interface.h
apps/test/ifgen/test2/CMakeFiles/testAsync2_client.dir/depend: apps/test/ifgen/test2/common_server.h
	cd /home/shrunoti/Downloads/workspace/legato/build/localhost && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/shrunoti/Downloads/workspace/legato /home/shrunoti/Downloads/workspace/legato/apps/test/ifgen/test2 /home/shrunoti/Downloads/workspace/legato/build/localhost /home/shrunoti/Downloads/workspace/legato/build/localhost/apps/test/ifgen/test2 /home/shrunoti/Downloads/workspace/legato/build/localhost/apps/test/ifgen/test2/CMakeFiles/testAsync2_client.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : apps/test/ifgen/test2/CMakeFiles/testAsync2_client.dir/depend
