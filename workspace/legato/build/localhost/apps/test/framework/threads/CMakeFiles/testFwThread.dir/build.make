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
include apps/test/framework/threads/CMakeFiles/testFwThread.dir/depend.make

# Include the progress variables for this target.
include apps/test/framework/threads/CMakeFiles/testFwThread.dir/progress.make

# Include the compile flags for this target's objects.
include apps/test/framework/threads/CMakeFiles/testFwThread.dir/flags.make

apps/test/framework/threads/CMakeFiles/testFwThread.dir/__/__/__/__/framework/liblegato/codegen/_le_main.c.o: apps/test/framework/threads/CMakeFiles/testFwThread.dir/flags.make
apps/test/framework/threads/CMakeFiles/testFwThread.dir/__/__/__/__/framework/liblegato/codegen/_le_main.c.o: ../../framework/liblegato/codegen/_le_main.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/shrunoti/Downloads/workspace/legato/build/localhost/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object apps/test/framework/threads/CMakeFiles/testFwThread.dir/__/__/__/__/framework/liblegato/codegen/_le_main.c.o"
	cd /home/shrunoti/Downloads/workspace/legato/build/localhost/apps/test/framework/threads && /usr/bin//gcc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/testFwThread.dir/__/__/__/__/framework/liblegato/codegen/_le_main.c.o   -c /home/shrunoti/Downloads/workspace/legato/framework/liblegato/codegen/_le_main.c

apps/test/framework/threads/CMakeFiles/testFwThread.dir/__/__/__/__/framework/liblegato/codegen/_le_main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/testFwThread.dir/__/__/__/__/framework/liblegato/codegen/_le_main.c.i"
	cd /home/shrunoti/Downloads/workspace/legato/build/localhost/apps/test/framework/threads && /usr/bin//gcc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/shrunoti/Downloads/workspace/legato/framework/liblegato/codegen/_le_main.c > CMakeFiles/testFwThread.dir/__/__/__/__/framework/liblegato/codegen/_le_main.c.i

apps/test/framework/threads/CMakeFiles/testFwThread.dir/__/__/__/__/framework/liblegato/codegen/_le_main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/testFwThread.dir/__/__/__/__/framework/liblegato/codegen/_le_main.c.s"
	cd /home/shrunoti/Downloads/workspace/legato/build/localhost/apps/test/framework/threads && /usr/bin//gcc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/shrunoti/Downloads/workspace/legato/framework/liblegato/codegen/_le_main.c -o CMakeFiles/testFwThread.dir/__/__/__/__/framework/liblegato/codegen/_le_main.c.s

apps/test/framework/threads/CMakeFiles/testFwThread.dir/__/__/__/__/framework/liblegato/codegen/_le_main.c.o.requires:

.PHONY : apps/test/framework/threads/CMakeFiles/testFwThread.dir/__/__/__/__/framework/liblegato/codegen/_le_main.c.o.requires

apps/test/framework/threads/CMakeFiles/testFwThread.dir/__/__/__/__/framework/liblegato/codegen/_le_main.c.o.provides: apps/test/framework/threads/CMakeFiles/testFwThread.dir/__/__/__/__/framework/liblegato/codegen/_le_main.c.o.requires
	$(MAKE) -f apps/test/framework/threads/CMakeFiles/testFwThread.dir/build.make apps/test/framework/threads/CMakeFiles/testFwThread.dir/__/__/__/__/framework/liblegato/codegen/_le_main.c.o.provides.build
.PHONY : apps/test/framework/threads/CMakeFiles/testFwThread.dir/__/__/__/__/framework/liblegato/codegen/_le_main.c.o.provides

apps/test/framework/threads/CMakeFiles/testFwThread.dir/__/__/__/__/framework/liblegato/codegen/_le_main.c.o.provides.build: apps/test/framework/threads/CMakeFiles/testFwThread.dir/__/__/__/__/framework/liblegato/codegen/_le_main.c.o


apps/test/framework/threads/CMakeFiles/testFwThread.dir/main.c.o: apps/test/framework/threads/CMakeFiles/testFwThread.dir/flags.make
apps/test/framework/threads/CMakeFiles/testFwThread.dir/main.c.o: ../../apps/test/framework/threads/main.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/shrunoti/Downloads/workspace/legato/build/localhost/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object apps/test/framework/threads/CMakeFiles/testFwThread.dir/main.c.o"
	cd /home/shrunoti/Downloads/workspace/legato/build/localhost/apps/test/framework/threads && /usr/bin//gcc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/testFwThread.dir/main.c.o   -c /home/shrunoti/Downloads/workspace/legato/apps/test/framework/threads/main.c

apps/test/framework/threads/CMakeFiles/testFwThread.dir/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/testFwThread.dir/main.c.i"
	cd /home/shrunoti/Downloads/workspace/legato/build/localhost/apps/test/framework/threads && /usr/bin//gcc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/shrunoti/Downloads/workspace/legato/apps/test/framework/threads/main.c > CMakeFiles/testFwThread.dir/main.c.i

apps/test/framework/threads/CMakeFiles/testFwThread.dir/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/testFwThread.dir/main.c.s"
	cd /home/shrunoti/Downloads/workspace/legato/build/localhost/apps/test/framework/threads && /usr/bin//gcc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/shrunoti/Downloads/workspace/legato/apps/test/framework/threads/main.c -o CMakeFiles/testFwThread.dir/main.c.s

apps/test/framework/threads/CMakeFiles/testFwThread.dir/main.c.o.requires:

.PHONY : apps/test/framework/threads/CMakeFiles/testFwThread.dir/main.c.o.requires

apps/test/framework/threads/CMakeFiles/testFwThread.dir/main.c.o.provides: apps/test/framework/threads/CMakeFiles/testFwThread.dir/main.c.o.requires
	$(MAKE) -f apps/test/framework/threads/CMakeFiles/testFwThread.dir/build.make apps/test/framework/threads/CMakeFiles/testFwThread.dir/main.c.o.provides.build
.PHONY : apps/test/framework/threads/CMakeFiles/testFwThread.dir/main.c.o.provides

apps/test/framework/threads/CMakeFiles/testFwThread.dir/main.c.o.provides.build: apps/test/framework/threads/CMakeFiles/testFwThread.dir/main.c.o


apps/test/framework/threads/CMakeFiles/testFwThread.dir/forkJoinMutex.c.o: apps/test/framework/threads/CMakeFiles/testFwThread.dir/flags.make
apps/test/framework/threads/CMakeFiles/testFwThread.dir/forkJoinMutex.c.o: ../../apps/test/framework/threads/forkJoinMutex.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/shrunoti/Downloads/workspace/legato/build/localhost/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object apps/test/framework/threads/CMakeFiles/testFwThread.dir/forkJoinMutex.c.o"
	cd /home/shrunoti/Downloads/workspace/legato/build/localhost/apps/test/framework/threads && /usr/bin//gcc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/testFwThread.dir/forkJoinMutex.c.o   -c /home/shrunoti/Downloads/workspace/legato/apps/test/framework/threads/forkJoinMutex.c

apps/test/framework/threads/CMakeFiles/testFwThread.dir/forkJoinMutex.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/testFwThread.dir/forkJoinMutex.c.i"
	cd /home/shrunoti/Downloads/workspace/legato/build/localhost/apps/test/framework/threads && /usr/bin//gcc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/shrunoti/Downloads/workspace/legato/apps/test/framework/threads/forkJoinMutex.c > CMakeFiles/testFwThread.dir/forkJoinMutex.c.i

apps/test/framework/threads/CMakeFiles/testFwThread.dir/forkJoinMutex.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/testFwThread.dir/forkJoinMutex.c.s"
	cd /home/shrunoti/Downloads/workspace/legato/build/localhost/apps/test/framework/threads && /usr/bin//gcc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/shrunoti/Downloads/workspace/legato/apps/test/framework/threads/forkJoinMutex.c -o CMakeFiles/testFwThread.dir/forkJoinMutex.c.s

apps/test/framework/threads/CMakeFiles/testFwThread.dir/forkJoinMutex.c.o.requires:

.PHONY : apps/test/framework/threads/CMakeFiles/testFwThread.dir/forkJoinMutex.c.o.requires

apps/test/framework/threads/CMakeFiles/testFwThread.dir/forkJoinMutex.c.o.provides: apps/test/framework/threads/CMakeFiles/testFwThread.dir/forkJoinMutex.c.o.requires
	$(MAKE) -f apps/test/framework/threads/CMakeFiles/testFwThread.dir/build.make apps/test/framework/threads/CMakeFiles/testFwThread.dir/forkJoinMutex.c.o.provides.build
.PHONY : apps/test/framework/threads/CMakeFiles/testFwThread.dir/forkJoinMutex.c.o.provides

apps/test/framework/threads/CMakeFiles/testFwThread.dir/forkJoinMutex.c.o.provides.build: apps/test/framework/threads/CMakeFiles/testFwThread.dir/forkJoinMutex.c.o


apps/test/framework/threads/CMakeFiles/testFwThread.dir/externalThreadApi.c.o: apps/test/framework/threads/CMakeFiles/testFwThread.dir/flags.make
apps/test/framework/threads/CMakeFiles/testFwThread.dir/externalThreadApi.c.o: ../../apps/test/framework/threads/externalThreadApi.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/shrunoti/Downloads/workspace/legato/build/localhost/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building C object apps/test/framework/threads/CMakeFiles/testFwThread.dir/externalThreadApi.c.o"
	cd /home/shrunoti/Downloads/workspace/legato/build/localhost/apps/test/framework/threads && /usr/bin//gcc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/testFwThread.dir/externalThreadApi.c.o   -c /home/shrunoti/Downloads/workspace/legato/apps/test/framework/threads/externalThreadApi.c

apps/test/framework/threads/CMakeFiles/testFwThread.dir/externalThreadApi.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/testFwThread.dir/externalThreadApi.c.i"
	cd /home/shrunoti/Downloads/workspace/legato/build/localhost/apps/test/framework/threads && /usr/bin//gcc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/shrunoti/Downloads/workspace/legato/apps/test/framework/threads/externalThreadApi.c > CMakeFiles/testFwThread.dir/externalThreadApi.c.i

apps/test/framework/threads/CMakeFiles/testFwThread.dir/externalThreadApi.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/testFwThread.dir/externalThreadApi.c.s"
	cd /home/shrunoti/Downloads/workspace/legato/build/localhost/apps/test/framework/threads && /usr/bin//gcc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/shrunoti/Downloads/workspace/legato/apps/test/framework/threads/externalThreadApi.c -o CMakeFiles/testFwThread.dir/externalThreadApi.c.s

apps/test/framework/threads/CMakeFiles/testFwThread.dir/externalThreadApi.c.o.requires:

.PHONY : apps/test/framework/threads/CMakeFiles/testFwThread.dir/externalThreadApi.c.o.requires

apps/test/framework/threads/CMakeFiles/testFwThread.dir/externalThreadApi.c.o.provides: apps/test/framework/threads/CMakeFiles/testFwThread.dir/externalThreadApi.c.o.requires
	$(MAKE) -f apps/test/framework/threads/CMakeFiles/testFwThread.dir/build.make apps/test/framework/threads/CMakeFiles/testFwThread.dir/externalThreadApi.c.o.provides.build
.PHONY : apps/test/framework/threads/CMakeFiles/testFwThread.dir/externalThreadApi.c.o.provides

apps/test/framework/threads/CMakeFiles/testFwThread.dir/externalThreadApi.c.o.provides.build: apps/test/framework/threads/CMakeFiles/testFwThread.dir/externalThreadApi.c.o


apps/test/framework/threads/CMakeFiles/testFwThread.dir/priority.c.o: apps/test/framework/threads/CMakeFiles/testFwThread.dir/flags.make
apps/test/framework/threads/CMakeFiles/testFwThread.dir/priority.c.o: ../../apps/test/framework/threads/priority.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/shrunoti/Downloads/workspace/legato/build/localhost/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building C object apps/test/framework/threads/CMakeFiles/testFwThread.dir/priority.c.o"
	cd /home/shrunoti/Downloads/workspace/legato/build/localhost/apps/test/framework/threads && /usr/bin//gcc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/testFwThread.dir/priority.c.o   -c /home/shrunoti/Downloads/workspace/legato/apps/test/framework/threads/priority.c

apps/test/framework/threads/CMakeFiles/testFwThread.dir/priority.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/testFwThread.dir/priority.c.i"
	cd /home/shrunoti/Downloads/workspace/legato/build/localhost/apps/test/framework/threads && /usr/bin//gcc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/shrunoti/Downloads/workspace/legato/apps/test/framework/threads/priority.c > CMakeFiles/testFwThread.dir/priority.c.i

apps/test/framework/threads/CMakeFiles/testFwThread.dir/priority.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/testFwThread.dir/priority.c.s"
	cd /home/shrunoti/Downloads/workspace/legato/build/localhost/apps/test/framework/threads && /usr/bin//gcc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/shrunoti/Downloads/workspace/legato/apps/test/framework/threads/priority.c -o CMakeFiles/testFwThread.dir/priority.c.s

apps/test/framework/threads/CMakeFiles/testFwThread.dir/priority.c.o.requires:

.PHONY : apps/test/framework/threads/CMakeFiles/testFwThread.dir/priority.c.o.requires

apps/test/framework/threads/CMakeFiles/testFwThread.dir/priority.c.o.provides: apps/test/framework/threads/CMakeFiles/testFwThread.dir/priority.c.o.requires
	$(MAKE) -f apps/test/framework/threads/CMakeFiles/testFwThread.dir/build.make apps/test/framework/threads/CMakeFiles/testFwThread.dir/priority.c.o.provides.build
.PHONY : apps/test/framework/threads/CMakeFiles/testFwThread.dir/priority.c.o.provides

apps/test/framework/threads/CMakeFiles/testFwThread.dir/priority.c.o.provides.build: apps/test/framework/threads/CMakeFiles/testFwThread.dir/priority.c.o


# Object files for target testFwThread
testFwThread_OBJECTS = \
"CMakeFiles/testFwThread.dir/__/__/__/__/framework/liblegato/codegen/_le_main.c.o" \
"CMakeFiles/testFwThread.dir/main.c.o" \
"CMakeFiles/testFwThread.dir/forkJoinMutex.c.o" \
"CMakeFiles/testFwThread.dir/externalThreadApi.c.o" \
"CMakeFiles/testFwThread.dir/priority.c.o"

# External object files for target testFwThread
testFwThread_EXTERNAL_OBJECTS =

tests/bin/testFwThread: apps/test/framework/threads/CMakeFiles/testFwThread.dir/__/__/__/__/framework/liblegato/codegen/_le_main.c.o
tests/bin/testFwThread: apps/test/framework/threads/CMakeFiles/testFwThread.dir/main.c.o
tests/bin/testFwThread: apps/test/framework/threads/CMakeFiles/testFwThread.dir/forkJoinMutex.c.o
tests/bin/testFwThread: apps/test/framework/threads/CMakeFiles/testFwThread.dir/externalThreadApi.c.o
tests/bin/testFwThread: apps/test/framework/threads/CMakeFiles/testFwThread.dir/priority.c.o
tests/bin/testFwThread: apps/test/framework/threads/CMakeFiles/testFwThread.dir/build.make
tests/bin/testFwThread: framework/lib/liblegato.so
tests/bin/testFwThread: apps/test/framework/threads/CMakeFiles/testFwThread.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/shrunoti/Downloads/workspace/legato/build/localhost/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Linking C executable ../../../../tests/bin/testFwThread"
	cd /home/shrunoti/Downloads/workspace/legato/build/localhost/apps/test/framework/threads && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/testFwThread.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
apps/test/framework/threads/CMakeFiles/testFwThread.dir/build: tests/bin/testFwThread

.PHONY : apps/test/framework/threads/CMakeFiles/testFwThread.dir/build

apps/test/framework/threads/CMakeFiles/testFwThread.dir/requires: apps/test/framework/threads/CMakeFiles/testFwThread.dir/__/__/__/__/framework/liblegato/codegen/_le_main.c.o.requires
apps/test/framework/threads/CMakeFiles/testFwThread.dir/requires: apps/test/framework/threads/CMakeFiles/testFwThread.dir/main.c.o.requires
apps/test/framework/threads/CMakeFiles/testFwThread.dir/requires: apps/test/framework/threads/CMakeFiles/testFwThread.dir/forkJoinMutex.c.o.requires
apps/test/framework/threads/CMakeFiles/testFwThread.dir/requires: apps/test/framework/threads/CMakeFiles/testFwThread.dir/externalThreadApi.c.o.requires
apps/test/framework/threads/CMakeFiles/testFwThread.dir/requires: apps/test/framework/threads/CMakeFiles/testFwThread.dir/priority.c.o.requires

.PHONY : apps/test/framework/threads/CMakeFiles/testFwThread.dir/requires

apps/test/framework/threads/CMakeFiles/testFwThread.dir/clean:
	cd /home/shrunoti/Downloads/workspace/legato/build/localhost/apps/test/framework/threads && $(CMAKE_COMMAND) -P CMakeFiles/testFwThread.dir/cmake_clean.cmake
.PHONY : apps/test/framework/threads/CMakeFiles/testFwThread.dir/clean

apps/test/framework/threads/CMakeFiles/testFwThread.dir/depend:
	cd /home/shrunoti/Downloads/workspace/legato/build/localhost && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/shrunoti/Downloads/workspace/legato /home/shrunoti/Downloads/workspace/legato/apps/test/framework/threads /home/shrunoti/Downloads/workspace/legato/build/localhost /home/shrunoti/Downloads/workspace/legato/build/localhost/apps/test/framework/threads /home/shrunoti/Downloads/workspace/legato/build/localhost/apps/test/framework/threads/CMakeFiles/testFwThread.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : apps/test/framework/threads/CMakeFiles/testFwThread.dir/depend

