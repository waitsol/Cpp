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
CMAKE_SOURCE_DIR = /root/git/Cpp/common

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /root/git/Cpp/common

# Include any dependencies generated for this target.
include CMakeFiles/tcpFunc.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/tcpFunc.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/tcpFunc.dir/flags.make

CMakeFiles/tcpFunc.dir/tcpHead.cpp.o: CMakeFiles/tcpFunc.dir/flags.make
CMakeFiles/tcpFunc.dir/tcpHead.cpp.o: tcpHead.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/git/Cpp/common/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/tcpFunc.dir/tcpHead.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/tcpFunc.dir/tcpHead.cpp.o -c /root/git/Cpp/common/tcpHead.cpp

CMakeFiles/tcpFunc.dir/tcpHead.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/tcpFunc.dir/tcpHead.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/git/Cpp/common/tcpHead.cpp > CMakeFiles/tcpFunc.dir/tcpHead.cpp.i

CMakeFiles/tcpFunc.dir/tcpHead.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/tcpFunc.dir/tcpHead.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/git/Cpp/common/tcpHead.cpp -o CMakeFiles/tcpFunc.dir/tcpHead.cpp.s

# Object files for target tcpFunc
tcpFunc_OBJECTS = \
"CMakeFiles/tcpFunc.dir/tcpHead.cpp.o"

# External object files for target tcpFunc
tcpFunc_EXTERNAL_OBJECTS =

libtcpFunc.so: CMakeFiles/tcpFunc.dir/tcpHead.cpp.o
libtcpFunc.so: CMakeFiles/tcpFunc.dir/build.make
libtcpFunc.so: CMakeFiles/tcpFunc.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/root/git/Cpp/common/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX shared library libtcpFunc.so"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/tcpFunc.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/tcpFunc.dir/build: libtcpFunc.so

.PHONY : CMakeFiles/tcpFunc.dir/build

CMakeFiles/tcpFunc.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/tcpFunc.dir/cmake_clean.cmake
.PHONY : CMakeFiles/tcpFunc.dir/clean

CMakeFiles/tcpFunc.dir/depend:
	cd /root/git/Cpp/common && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /root/git/Cpp/common /root/git/Cpp/common /root/git/Cpp/common /root/git/Cpp/common /root/git/Cpp/common/CMakeFiles/tcpFunc.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/tcpFunc.dir/depend

