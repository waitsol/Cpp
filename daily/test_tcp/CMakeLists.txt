# 设置cmake的最低版本
cmake_minimum_required(VERSION 3.10)

# 指定为C++11 版本
set(CMAKE_CXX_STANDARD 11)

project(server VERSION 1.0)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib/)
set(CMAKE_DEBUG_POSTFIX "_d")    
set(CMAKE_RELEASE_POSTFIX "_r") 
include_directories("${CMAKE_BINARY_DIR}/../../common")


file(GLOB_RECURSE CURRENT_HEADERS  ${PROJECT_SOURCE_DIR}/*.h )
source_group("Include" FILES ${CURRENT_HEADERS})

add_executable(${PROJECT_NAME}  ${PROJECT_SOURCE_DIR}/server.cc ${CURRENT_HEADERS})
target_link_libraries(${PROJECT_NAME} tcpFunc)

project(client VERSION 1.0)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib/)
set(CMAKE_DEBUG_POSTFIX "_d")    
set(CMAKE_RELEASE_POSTFIX "_r") 
include_directories("${CMAKE_BINARY_DIR}/../../common")


file(GLOB_RECURSE CURRENT_HEADERS  ${PROJECT_SOURCE_DIR}/*.h )
source_group("Include" FILES ${CURRENT_HEADERS})

add_executable(${PROJECT_NAME} ${PROJECT_SOURCE_DIR}/client.cc ${CURRENT_HEADERS})
target_link_libraries(${PROJECT_NAME} tcpFunc)