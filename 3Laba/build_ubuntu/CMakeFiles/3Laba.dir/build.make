# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.31

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /snap/cmake/1437/bin/cmake

# The command to remove a file.
RM = /snap/cmake/1437/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/hook/Documents/GitHub/Ubuntu/3Laba

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/hook/Documents/GitHub/Ubuntu/3Laba/build_ubuntu

# Include any dependencies generated for this target.
include CMakeFiles/3Laba.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/3Laba.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/3Laba.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/3Laba.dir/flags.make

CMakeFiles/3Laba.dir/codegen:
.PHONY : CMakeFiles/3Laba.dir/codegen

CMakeFiles/3Laba.dir/3Laba.cpp.o: CMakeFiles/3Laba.dir/flags.make
CMakeFiles/3Laba.dir/3Laba.cpp.o: /home/hook/Documents/GitHub/Ubuntu/3Laba/3Laba.cpp
CMakeFiles/3Laba.dir/3Laba.cpp.o: CMakeFiles/3Laba.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/hook/Documents/GitHub/Ubuntu/3Laba/build_ubuntu/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/3Laba.dir/3Laba.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/3Laba.dir/3Laba.cpp.o -MF CMakeFiles/3Laba.dir/3Laba.cpp.o.d -o CMakeFiles/3Laba.dir/3Laba.cpp.o -c /home/hook/Documents/GitHub/Ubuntu/3Laba/3Laba.cpp

CMakeFiles/3Laba.dir/3Laba.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/3Laba.dir/3Laba.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/hook/Documents/GitHub/Ubuntu/3Laba/3Laba.cpp > CMakeFiles/3Laba.dir/3Laba.cpp.i

CMakeFiles/3Laba.dir/3Laba.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/3Laba.dir/3Laba.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/hook/Documents/GitHub/Ubuntu/3Laba/3Laba.cpp -o CMakeFiles/3Laba.dir/3Laba.cpp.s

# Object files for target 3Laba
3Laba_OBJECTS = \
"CMakeFiles/3Laba.dir/3Laba.cpp.o"

# External object files for target 3Laba
3Laba_EXTERNAL_OBJECTS =

3Laba: CMakeFiles/3Laba.dir/3Laba.cpp.o
3Laba: CMakeFiles/3Laba.dir/build.make
3Laba: CMakeFiles/3Laba.dir/compiler_depend.ts
3Laba: CMakeFiles/3Laba.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/hook/Documents/GitHub/Ubuntu/3Laba/build_ubuntu/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable 3Laba"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/3Laba.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/3Laba.dir/build: 3Laba
.PHONY : CMakeFiles/3Laba.dir/build

CMakeFiles/3Laba.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/3Laba.dir/cmake_clean.cmake
.PHONY : CMakeFiles/3Laba.dir/clean

CMakeFiles/3Laba.dir/depend:
	cd /home/hook/Documents/GitHub/Ubuntu/3Laba/build_ubuntu && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/hook/Documents/GitHub/Ubuntu/3Laba /home/hook/Documents/GitHub/Ubuntu/3Laba /home/hook/Documents/GitHub/Ubuntu/3Laba/build_ubuntu /home/hook/Documents/GitHub/Ubuntu/3Laba/build_ubuntu /home/hook/Documents/GitHub/Ubuntu/3Laba/build_ubuntu/CMakeFiles/3Laba.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/3Laba.dir/depend

