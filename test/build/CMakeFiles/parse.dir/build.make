# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

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
CMAKE_SOURCE_DIR = /home/joe2/workspace1/jsonhelper/test

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/joe2/workspace1/jsonhelper/test/build

# Include any dependencies generated for this target.
include CMakeFiles/parse.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/parse.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/parse.dir/flags.make

CMakeFiles/parse.dir/home/joe2/workspace1/jsonhelper/src/parse.cpp.o: CMakeFiles/parse.dir/flags.make
CMakeFiles/parse.dir/home/joe2/workspace1/jsonhelper/src/parse.cpp.o: /home/joe2/workspace1/jsonhelper/src/parse.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/joe2/workspace1/jsonhelper/test/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/parse.dir/home/joe2/workspace1/jsonhelper/src/parse.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/parse.dir/home/joe2/workspace1/jsonhelper/src/parse.cpp.o -c /home/joe2/workspace1/jsonhelper/src/parse.cpp

CMakeFiles/parse.dir/home/joe2/workspace1/jsonhelper/src/parse.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/parse.dir/home/joe2/workspace1/jsonhelper/src/parse.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/joe2/workspace1/jsonhelper/src/parse.cpp > CMakeFiles/parse.dir/home/joe2/workspace1/jsonhelper/src/parse.cpp.i

CMakeFiles/parse.dir/home/joe2/workspace1/jsonhelper/src/parse.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/parse.dir/home/joe2/workspace1/jsonhelper/src/parse.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/joe2/workspace1/jsonhelper/src/parse.cpp -o CMakeFiles/parse.dir/home/joe2/workspace1/jsonhelper/src/parse.cpp.s

# Object files for target parse
parse_OBJECTS = \
"CMakeFiles/parse.dir/home/joe2/workspace1/jsonhelper/src/parse.cpp.o"

# External object files for target parse
parse_EXTERNAL_OBJECTS =

libparse.a: CMakeFiles/parse.dir/home/joe2/workspace1/jsonhelper/src/parse.cpp.o
libparse.a: CMakeFiles/parse.dir/build.make
libparse.a: CMakeFiles/parse.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/joe2/workspace1/jsonhelper/test/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX static library libparse.a"
	$(CMAKE_COMMAND) -P CMakeFiles/parse.dir/cmake_clean_target.cmake
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/parse.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/parse.dir/build: libparse.a

.PHONY : CMakeFiles/parse.dir/build

CMakeFiles/parse.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/parse.dir/cmake_clean.cmake
.PHONY : CMakeFiles/parse.dir/clean

CMakeFiles/parse.dir/depend:
	cd /home/joe2/workspace1/jsonhelper/test/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/joe2/workspace1/jsonhelper/test /home/joe2/workspace1/jsonhelper/test /home/joe2/workspace1/jsonhelper/test/build /home/joe2/workspace1/jsonhelper/test/build /home/joe2/workspace1/jsonhelper/test/build/CMakeFiles/parse.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/parse.dir/depend
