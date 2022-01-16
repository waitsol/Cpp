# 设置cmake的最低版本
cmake_minimum_required(VERSION 3.10)

# 指定为C++11 版本
set(CMAKE_CXX_STANDARD 11)

project(tcpFunc VERSION 1.0)
add_library(tcpFunc SHARED tcpHead.cpp)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib/)
#2.静态库 lib 输出路径：

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ../lib/)

set(CMAKE_DEBUG_POSTFIX "_d")    
set(CMAKE_RELEASE_POSTFIX "_r") 
install(TARGETS tcpFunc
        EXPORT MyLibTargets 
        LIBRARY DESTINATION lib  # 动态库安装路径 /usr/local/lib/libtcpFunc.so
        ARCHIVE DESTINATION lib  # 静态库安装路径
        )
#3.动态库输出路径（注意这个放在最后）：

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib/)
SET_TARGET_PROPERTIES(tcpFunc PROPERTIES RUNTIME_OUTPUT_DIRECTORY ../lib/)