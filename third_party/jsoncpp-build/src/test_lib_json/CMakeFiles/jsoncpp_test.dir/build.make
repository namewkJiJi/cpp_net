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
CMAKE_SOURCE_DIR = /home/jj/tmms/third_party/jsoncpp

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/jj/tmms/third_party/jsoncpp-build

# Include any dependencies generated for this target.
include src/test_lib_json/CMakeFiles/jsoncpp_test.dir/depend.make

# Include the progress variables for this target.
include src/test_lib_json/CMakeFiles/jsoncpp_test.dir/progress.make

# Include the compile flags for this target's objects.
include src/test_lib_json/CMakeFiles/jsoncpp_test.dir/flags.make

src/test_lib_json/CMakeFiles/jsoncpp_test.dir/jsontest.cpp.o: src/test_lib_json/CMakeFiles/jsoncpp_test.dir/flags.make
src/test_lib_json/CMakeFiles/jsoncpp_test.dir/jsontest.cpp.o: /home/jj/tmms/third_party/jsoncpp/src/test_lib_json/jsontest.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jj/tmms/third_party/jsoncpp-build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/test_lib_json/CMakeFiles/jsoncpp_test.dir/jsontest.cpp.o"
	cd /home/jj/tmms/third_party/jsoncpp-build/src/test_lib_json && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/jsoncpp_test.dir/jsontest.cpp.o -c /home/jj/tmms/third_party/jsoncpp/src/test_lib_json/jsontest.cpp

src/test_lib_json/CMakeFiles/jsoncpp_test.dir/jsontest.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/jsoncpp_test.dir/jsontest.cpp.i"
	cd /home/jj/tmms/third_party/jsoncpp-build/src/test_lib_json && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/jj/tmms/third_party/jsoncpp/src/test_lib_json/jsontest.cpp > CMakeFiles/jsoncpp_test.dir/jsontest.cpp.i

src/test_lib_json/CMakeFiles/jsoncpp_test.dir/jsontest.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/jsoncpp_test.dir/jsontest.cpp.s"
	cd /home/jj/tmms/third_party/jsoncpp-build/src/test_lib_json && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/jj/tmms/third_party/jsoncpp/src/test_lib_json/jsontest.cpp -o CMakeFiles/jsoncpp_test.dir/jsontest.cpp.s

src/test_lib_json/CMakeFiles/jsoncpp_test.dir/fuzz.cpp.o: src/test_lib_json/CMakeFiles/jsoncpp_test.dir/flags.make
src/test_lib_json/CMakeFiles/jsoncpp_test.dir/fuzz.cpp.o: /home/jj/tmms/third_party/jsoncpp/src/test_lib_json/fuzz.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jj/tmms/third_party/jsoncpp-build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object src/test_lib_json/CMakeFiles/jsoncpp_test.dir/fuzz.cpp.o"
	cd /home/jj/tmms/third_party/jsoncpp-build/src/test_lib_json && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/jsoncpp_test.dir/fuzz.cpp.o -c /home/jj/tmms/third_party/jsoncpp/src/test_lib_json/fuzz.cpp

src/test_lib_json/CMakeFiles/jsoncpp_test.dir/fuzz.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/jsoncpp_test.dir/fuzz.cpp.i"
	cd /home/jj/tmms/third_party/jsoncpp-build/src/test_lib_json && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/jj/tmms/third_party/jsoncpp/src/test_lib_json/fuzz.cpp > CMakeFiles/jsoncpp_test.dir/fuzz.cpp.i

src/test_lib_json/CMakeFiles/jsoncpp_test.dir/fuzz.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/jsoncpp_test.dir/fuzz.cpp.s"
	cd /home/jj/tmms/third_party/jsoncpp-build/src/test_lib_json && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/jj/tmms/third_party/jsoncpp/src/test_lib_json/fuzz.cpp -o CMakeFiles/jsoncpp_test.dir/fuzz.cpp.s

src/test_lib_json/CMakeFiles/jsoncpp_test.dir/main.cpp.o: src/test_lib_json/CMakeFiles/jsoncpp_test.dir/flags.make
src/test_lib_json/CMakeFiles/jsoncpp_test.dir/main.cpp.o: /home/jj/tmms/third_party/jsoncpp/src/test_lib_json/main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jj/tmms/third_party/jsoncpp-build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object src/test_lib_json/CMakeFiles/jsoncpp_test.dir/main.cpp.o"
	cd /home/jj/tmms/third_party/jsoncpp-build/src/test_lib_json && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/jsoncpp_test.dir/main.cpp.o -c /home/jj/tmms/third_party/jsoncpp/src/test_lib_json/main.cpp

src/test_lib_json/CMakeFiles/jsoncpp_test.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/jsoncpp_test.dir/main.cpp.i"
	cd /home/jj/tmms/third_party/jsoncpp-build/src/test_lib_json && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/jj/tmms/third_party/jsoncpp/src/test_lib_json/main.cpp > CMakeFiles/jsoncpp_test.dir/main.cpp.i

src/test_lib_json/CMakeFiles/jsoncpp_test.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/jsoncpp_test.dir/main.cpp.s"
	cd /home/jj/tmms/third_party/jsoncpp-build/src/test_lib_json && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/jj/tmms/third_party/jsoncpp/src/test_lib_json/main.cpp -o CMakeFiles/jsoncpp_test.dir/main.cpp.s

# Object files for target jsoncpp_test
jsoncpp_test_OBJECTS = \
"CMakeFiles/jsoncpp_test.dir/jsontest.cpp.o" \
"CMakeFiles/jsoncpp_test.dir/fuzz.cpp.o" \
"CMakeFiles/jsoncpp_test.dir/main.cpp.o"

# External object files for target jsoncpp_test
jsoncpp_test_EXTERNAL_OBJECTS =

bin/jsoncpp_test: src/test_lib_json/CMakeFiles/jsoncpp_test.dir/jsontest.cpp.o
bin/jsoncpp_test: src/test_lib_json/CMakeFiles/jsoncpp_test.dir/fuzz.cpp.o
bin/jsoncpp_test: src/test_lib_json/CMakeFiles/jsoncpp_test.dir/main.cpp.o
bin/jsoncpp_test: src/test_lib_json/CMakeFiles/jsoncpp_test.dir/build.make
bin/jsoncpp_test: lib/libjsoncpp.so.1.9.7
bin/jsoncpp_test: src/test_lib_json/CMakeFiles/jsoncpp_test.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/jj/tmms/third_party/jsoncpp-build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking CXX executable ../../bin/jsoncpp_test"
	cd /home/jj/tmms/third_party/jsoncpp-build/src/test_lib_json && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/jsoncpp_test.dir/link.txt --verbose=$(VERBOSE)
	cd /home/jj/tmms/third_party/jsoncpp-build/src/test_lib_json && ../../bin/jsoncpp_test

# Rule to build all files generated by this target.
src/test_lib_json/CMakeFiles/jsoncpp_test.dir/build: bin/jsoncpp_test

.PHONY : src/test_lib_json/CMakeFiles/jsoncpp_test.dir/build

src/test_lib_json/CMakeFiles/jsoncpp_test.dir/clean:
	cd /home/jj/tmms/third_party/jsoncpp-build/src/test_lib_json && $(CMAKE_COMMAND) -P CMakeFiles/jsoncpp_test.dir/cmake_clean.cmake
.PHONY : src/test_lib_json/CMakeFiles/jsoncpp_test.dir/clean

src/test_lib_json/CMakeFiles/jsoncpp_test.dir/depend:
	cd /home/jj/tmms/third_party/jsoncpp-build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/jj/tmms/third_party/jsoncpp /home/jj/tmms/third_party/jsoncpp/src/test_lib_json /home/jj/tmms/third_party/jsoncpp-build /home/jj/tmms/third_party/jsoncpp-build/src/test_lib_json /home/jj/tmms/third_party/jsoncpp-build/src/test_lib_json/CMakeFiles/jsoncpp_test.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/test_lib_json/CMakeFiles/jsoncpp_test.dir/depend

