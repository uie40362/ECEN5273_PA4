# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.10

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
CMAKE_SOURCE_DIR = "/mnt/c/Users/goshe/Personal/NUS Matters/Y3S2/ECEN5273/Assignment/Assignment 4"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "/mnt/c/Users/goshe/Personal/NUS Matters/Y3S2/ECEN5273/Assignment/Assignment 4/cmake-build-debug"

# Include any dependencies generated for this target.
include CMakeFiles/dfc.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/dfc.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/dfc.dir/flags.make

CMakeFiles/dfc.dir/DFC/dfc.c.o: CMakeFiles/dfc.dir/flags.make
CMakeFiles/dfc.dir/DFC/dfc.c.o: ../DFC/dfc.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/mnt/c/Users/goshe/Personal/NUS Matters/Y3S2/ECEN5273/Assignment/Assignment 4/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/dfc.dir/DFC/dfc.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/dfc.dir/DFC/dfc.c.o   -c "/mnt/c/Users/goshe/Personal/NUS Matters/Y3S2/ECEN5273/Assignment/Assignment 4/DFC/dfc.c"

CMakeFiles/dfc.dir/DFC/dfc.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/dfc.dir/DFC/dfc.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E "/mnt/c/Users/goshe/Personal/NUS Matters/Y3S2/ECEN5273/Assignment/Assignment 4/DFC/dfc.c" > CMakeFiles/dfc.dir/DFC/dfc.c.i

CMakeFiles/dfc.dir/DFC/dfc.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/dfc.dir/DFC/dfc.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S "/mnt/c/Users/goshe/Personal/NUS Matters/Y3S2/ECEN5273/Assignment/Assignment 4/DFC/dfc.c" -o CMakeFiles/dfc.dir/DFC/dfc.c.s

CMakeFiles/dfc.dir/DFC/dfc.c.o.requires:

.PHONY : CMakeFiles/dfc.dir/DFC/dfc.c.o.requires

CMakeFiles/dfc.dir/DFC/dfc.c.o.provides: CMakeFiles/dfc.dir/DFC/dfc.c.o.requires
	$(MAKE) -f CMakeFiles/dfc.dir/build.make CMakeFiles/dfc.dir/DFC/dfc.c.o.provides.build
.PHONY : CMakeFiles/dfc.dir/DFC/dfc.c.o.provides

CMakeFiles/dfc.dir/DFC/dfc.c.o.provides.build: CMakeFiles/dfc.dir/DFC/dfc.c.o


# Object files for target dfc
dfc_OBJECTS = \
"CMakeFiles/dfc.dir/DFC/dfc.c.o"

# External object files for target dfc
dfc_EXTERNAL_OBJECTS =

dfc: CMakeFiles/dfc.dir/DFC/dfc.c.o
dfc: CMakeFiles/dfc.dir/build.make
dfc: CMakeFiles/dfc.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir="/mnt/c/Users/goshe/Personal/NUS Matters/Y3S2/ECEN5273/Assignment/Assignment 4/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable dfc"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/dfc.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/dfc.dir/build: dfc

.PHONY : CMakeFiles/dfc.dir/build

CMakeFiles/dfc.dir/requires: CMakeFiles/dfc.dir/DFC/dfc.c.o.requires

.PHONY : CMakeFiles/dfc.dir/requires

CMakeFiles/dfc.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/dfc.dir/cmake_clean.cmake
.PHONY : CMakeFiles/dfc.dir/clean

CMakeFiles/dfc.dir/depend:
	cd "/mnt/c/Users/goshe/Personal/NUS Matters/Y3S2/ECEN5273/Assignment/Assignment 4/cmake-build-debug" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "/mnt/c/Users/goshe/Personal/NUS Matters/Y3S2/ECEN5273/Assignment/Assignment 4" "/mnt/c/Users/goshe/Personal/NUS Matters/Y3S2/ECEN5273/Assignment/Assignment 4" "/mnt/c/Users/goshe/Personal/NUS Matters/Y3S2/ECEN5273/Assignment/Assignment 4/cmake-build-debug" "/mnt/c/Users/goshe/Personal/NUS Matters/Y3S2/ECEN5273/Assignment/Assignment 4/cmake-build-debug" "/mnt/c/Users/goshe/Personal/NUS Matters/Y3S2/ECEN5273/Assignment/Assignment 4/cmake-build-debug/CMakeFiles/dfc.dir/DependInfo.cmake" --color=$(COLOR)
.PHONY : CMakeFiles/dfc.dir/depend
