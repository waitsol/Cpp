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
CMAKE_SOURCE_DIR = /root/wm/git/Cpp/exe/zftp

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /root/wm/git/Cpp/exe/zftp

# Include any dependencies generated for this target.
include CMakeFiles/zftp_server.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/zftp_server.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/zftp_server.dir/flags.make

CMakeFiles/zftp_server.dir/src/server/zftp_server_handmessage.cc.o: CMakeFiles/zftp_server.dir/flags.make
CMakeFiles/zftp_server.dir/src/server/zftp_server_handmessage.cc.o: src/server/zftp_server_handmessage.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/wm/git/Cpp/exe/zftp/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/zftp_server.dir/src/server/zftp_server_handmessage.cc.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/zftp_server.dir/src/server/zftp_server_handmessage.cc.o -c /root/wm/git/Cpp/exe/zftp/src/server/zftp_server_handmessage.cc

CMakeFiles/zftp_server.dir/src/server/zftp_server_handmessage.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/zftp_server.dir/src/server/zftp_server_handmessage.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/wm/git/Cpp/exe/zftp/src/server/zftp_server_handmessage.cc > CMakeFiles/zftp_server.dir/src/server/zftp_server_handmessage.cc.i

CMakeFiles/zftp_server.dir/src/server/zftp_server_handmessage.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/zftp_server.dir/src/server/zftp_server_handmessage.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/wm/git/Cpp/exe/zftp/src/server/zftp_server_handmessage.cc -o CMakeFiles/zftp_server.dir/src/server/zftp_server_handmessage.cc.s

CMakeFiles/zftp_server.dir/src/server/zlk_ftp_server.cc.o: CMakeFiles/zftp_server.dir/flags.make
CMakeFiles/zftp_server.dir/src/server/zlk_ftp_server.cc.o: src/server/zlk_ftp_server.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/wm/git/Cpp/exe/zftp/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/zftp_server.dir/src/server/zlk_ftp_server.cc.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/zftp_server.dir/src/server/zlk_ftp_server.cc.o -c /root/wm/git/Cpp/exe/zftp/src/server/zlk_ftp_server.cc

CMakeFiles/zftp_server.dir/src/server/zlk_ftp_server.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/zftp_server.dir/src/server/zlk_ftp_server.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/wm/git/Cpp/exe/zftp/src/server/zlk_ftp_server.cc > CMakeFiles/zftp_server.dir/src/server/zlk_ftp_server.cc.i

CMakeFiles/zftp_server.dir/src/server/zlk_ftp_server.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/zftp_server.dir/src/server/zlk_ftp_server.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/wm/git/Cpp/exe/zftp/src/server/zlk_ftp_server.cc -o CMakeFiles/zftp_server.dir/src/server/zlk_ftp_server.cc.s

CMakeFiles/zftp_server.dir/src/server/zlk_zftp_server_connect.cc.o: CMakeFiles/zftp_server.dir/flags.make
CMakeFiles/zftp_server.dir/src/server/zlk_zftp_server_connect.cc.o: src/server/zlk_zftp_server_connect.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/wm/git/Cpp/exe/zftp/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/zftp_server.dir/src/server/zlk_zftp_server_connect.cc.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/zftp_server.dir/src/server/zlk_zftp_server_connect.cc.o -c /root/wm/git/Cpp/exe/zftp/src/server/zlk_zftp_server_connect.cc

CMakeFiles/zftp_server.dir/src/server/zlk_zftp_server_connect.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/zftp_server.dir/src/server/zlk_zftp_server_connect.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/wm/git/Cpp/exe/zftp/src/server/zlk_zftp_server_connect.cc > CMakeFiles/zftp_server.dir/src/server/zlk_zftp_server_connect.cc.i

CMakeFiles/zftp_server.dir/src/server/zlk_zftp_server_connect.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/zftp_server.dir/src/server/zlk_zftp_server_connect.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/wm/git/Cpp/exe/zftp/src/server/zlk_zftp_server_connect.cc -o CMakeFiles/zftp_server.dir/src/server/zlk_zftp_server_connect.cc.s

CMakeFiles/zftp_server.dir/src/common/io_service_pool.cc.o: CMakeFiles/zftp_server.dir/flags.make
CMakeFiles/zftp_server.dir/src/common/io_service_pool.cc.o: src/common/io_service_pool.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/wm/git/Cpp/exe/zftp/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/zftp_server.dir/src/common/io_service_pool.cc.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/zftp_server.dir/src/common/io_service_pool.cc.o -c /root/wm/git/Cpp/exe/zftp/src/common/io_service_pool.cc

CMakeFiles/zftp_server.dir/src/common/io_service_pool.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/zftp_server.dir/src/common/io_service_pool.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/wm/git/Cpp/exe/zftp/src/common/io_service_pool.cc > CMakeFiles/zftp_server.dir/src/common/io_service_pool.cc.i

CMakeFiles/zftp_server.dir/src/common/io_service_pool.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/zftp_server.dir/src/common/io_service_pool.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/wm/git/Cpp/exe/zftp/src/common/io_service_pool.cc -o CMakeFiles/zftp_server.dir/src/common/io_service_pool.cc.s

CMakeFiles/zftp_server.dir/src/common/zlk_message.pb.cc.o: CMakeFiles/zftp_server.dir/flags.make
CMakeFiles/zftp_server.dir/src/common/zlk_message.pb.cc.o: src/common/zlk_message.pb.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/wm/git/Cpp/exe/zftp/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/zftp_server.dir/src/common/zlk_message.pb.cc.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/zftp_server.dir/src/common/zlk_message.pb.cc.o -c /root/wm/git/Cpp/exe/zftp/src/common/zlk_message.pb.cc

CMakeFiles/zftp_server.dir/src/common/zlk_message.pb.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/zftp_server.dir/src/common/zlk_message.pb.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/wm/git/Cpp/exe/zftp/src/common/zlk_message.pb.cc > CMakeFiles/zftp_server.dir/src/common/zlk_message.pb.cc.i

CMakeFiles/zftp_server.dir/src/common/zlk_message.pb.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/zftp_server.dir/src/common/zlk_message.pb.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/wm/git/Cpp/exe/zftp/src/common/zlk_message.pb.cc -o CMakeFiles/zftp_server.dir/src/common/zlk_message.pb.cc.s

# Object files for target zftp_server
zftp_server_OBJECTS = \
"CMakeFiles/zftp_server.dir/src/server/zftp_server_handmessage.cc.o" \
"CMakeFiles/zftp_server.dir/src/server/zlk_ftp_server.cc.o" \
"CMakeFiles/zftp_server.dir/src/server/zlk_zftp_server_connect.cc.o" \
"CMakeFiles/zftp_server.dir/src/common/io_service_pool.cc.o" \
"CMakeFiles/zftp_server.dir/src/common/zlk_message.pb.cc.o"

# External object files for target zftp_server
zftp_server_EXTERNAL_OBJECTS =

zftp_server: CMakeFiles/zftp_server.dir/src/server/zftp_server_handmessage.cc.o
zftp_server: CMakeFiles/zftp_server.dir/src/server/zlk_ftp_server.cc.o
zftp_server: CMakeFiles/zftp_server.dir/src/server/zlk_zftp_server_connect.cc.o
zftp_server: CMakeFiles/zftp_server.dir/src/common/io_service_pool.cc.o
zftp_server: CMakeFiles/zftp_server.dir/src/common/zlk_message.pb.cc.o
zftp_server: CMakeFiles/zftp_server.dir/build.make
zftp_server: CMakeFiles/zftp_server.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/root/wm/git/Cpp/exe/zftp/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Linking CXX executable zftp_server"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/zftp_server.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/zftp_server.dir/build: zftp_server

.PHONY : CMakeFiles/zftp_server.dir/build

CMakeFiles/zftp_server.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/zftp_server.dir/cmake_clean.cmake
.PHONY : CMakeFiles/zftp_server.dir/clean

CMakeFiles/zftp_server.dir/depend:
	cd /root/wm/git/Cpp/exe/zftp && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /root/wm/git/Cpp/exe/zftp /root/wm/git/Cpp/exe/zftp /root/wm/git/Cpp/exe/zftp /root/wm/git/Cpp/exe/zftp /root/wm/git/Cpp/exe/zftp/CMakeFiles/zftp_server.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/zftp_server.dir/depend
