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
CMAKE_SOURCE_DIR = /home/jj/tmms

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/jj/tmms/build

# Include any dependencies generated for this target.
include src/network/net/test/CMakeFiles/EventLoopThreadTest.dir/depend.make

# Include the progress variables for this target.
include src/network/net/test/CMakeFiles/EventLoopThreadTest.dir/progress.make

# Include the compile flags for this target's objects.
include src/network/net/test/CMakeFiles/EventLoopThreadTest.dir/flags.make

src/network/net/test/CMakeFiles/EventLoopThreadTest.dir/EventLoopThreadTest.cpp.o: src/network/net/test/CMakeFiles/EventLoopThreadTest.dir/flags.make
src/network/net/test/CMakeFiles/EventLoopThreadTest.dir/EventLoopThreadTest.cpp.o: ../src/network/net/test/EventLoopThreadTest.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jj/tmms/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/network/net/test/CMakeFiles/EventLoopThreadTest.dir/EventLoopThreadTest.cpp.o"
	cd /home/jj/tmms/build/src/network/net/test && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/EventLoopThreadTest.dir/EventLoopThreadTest.cpp.o -c /home/jj/tmms/src/network/net/test/EventLoopThreadTest.cpp

src/network/net/test/CMakeFiles/EventLoopThreadTest.dir/EventLoopThreadTest.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/EventLoopThreadTest.dir/EventLoopThreadTest.cpp.i"
	cd /home/jj/tmms/build/src/network/net/test && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/jj/tmms/src/network/net/test/EventLoopThreadTest.cpp > CMakeFiles/EventLoopThreadTest.dir/EventLoopThreadTest.cpp.i

src/network/net/test/CMakeFiles/EventLoopThreadTest.dir/EventLoopThreadTest.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/EventLoopThreadTest.dir/EventLoopThreadTest.cpp.s"
	cd /home/jj/tmms/build/src/network/net/test && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/jj/tmms/src/network/net/test/EventLoopThreadTest.cpp -o CMakeFiles/EventLoopThreadTest.dir/EventLoopThreadTest.cpp.s

# Object files for target EventLoopThreadTest
EventLoopThreadTest_OBJECTS = \
"CMakeFiles/EventLoopThreadTest.dir/EventLoopThreadTest.cpp.o"

# External object files for target EventLoopThreadTest
EventLoopThreadTest_EXTERNAL_OBJECTS =

src/network/net/test/EventLoopThreadTest: src/network/net/test/CMakeFiles/EventLoopThreadTest.dir/EventLoopThreadTest.cpp.o
src/network/net/test/EventLoopThreadTest: src/network/net/test/CMakeFiles/EventLoopThreadTest.dir/build.make
src/network/net/test/EventLoopThreadTest: ../lib/base/libbase.a
src/network/net/test/EventLoopThreadTest: src/network/libnetwork.a
src/network/net/test/EventLoopThreadTest: ../lib/base/libbase.a
src/network/net/test/EventLoopThreadTest: src/network/net/test/CMakeFiles/EventLoopThreadTest.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/jj/tmms/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable EventLoopThreadTest"
	cd /home/jj/tmms/build/src/network/net/test && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/EventLoopThreadTest.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/network/net/test/CMakeFiles/EventLoopThreadTest.dir/build: src/network/net/test/EventLoopThreadTest

.PHONY : src/network/net/test/CMakeFiles/EventLoopThreadTest.dir/build

src/network/net/test/CMakeFiles/EventLoopThreadTest.dir/clean:
	cd /home/jj/tmms/build/src/network/net/test && $(CMAKE_COMMAND) -P CMakeFiles/EventLoopThreadTest.dir/cmake_clean.cmake
.PHONY : src/network/net/test/CMakeFiles/EventLoopThreadTest.dir/clean

src/network/net/test/CMakeFiles/EventLoopThreadTest.dir/depend:
	cd /home/jj/tmms/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/jj/tmms /home/jj/tmms/src/network/net/test /home/jj/tmms/build /home/jj/tmms/build/src/network/net/test /home/jj/tmms/build/src/network/net/test/CMakeFiles/EventLoopThreadTest.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/network/net/test/CMakeFiles/EventLoopThreadTest.dir/depend

