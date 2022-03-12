# 设置cmake的最低版本
cmake_minimum_required(VERSION 3.10)

# 指定为C++11 版本
set(CMAKE_CXX_STANDARD 11)

project(vector_stl VERSION 1.0)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib/)
set(CMAKE_DEBUG_POSTFIX "_d")    
set(CMAKE_RELEASE_POSTFIX "_r") 
include_directories("${CMAKE_BINARY_DIR}/../../common")
include_directories("${CMAKE_BINARY_DIR}/../allocator/")
include_directories("${CMAKE_BINARY_DIR}/../base/")
add_executable(vector_stl vector_stl.cpp)

#SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)