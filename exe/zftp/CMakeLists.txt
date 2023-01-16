# 设置cmake的最低版本
cmake_minimum_required(VERSION 3.10)

# 指定为C++11 版本
set(CMAKE_CXX_STANDARD 11)

project(zlk_zftp_server VERSION 1.0)

include_directories("${CMAKE_BINARY_DIR}/include")
include_directories("${CMAKE_BINARY_DIR}/../../lib/log/include")


file(GLOB_RECURSE CURRENT_HEADERS  ${PROJECT_SOURCE_DIR}/include/*.h )
file(GLOB_RECURSE SRC  ${PROJECT_SOURCE_DIR}/src/server/z.cpp )

add_executable(${PROJECT_NAME}  ${SRC} ${CURRENT_HEADERS})
target_link_libraries(${PROJECT_NAME}  "lua" "dl" "zlk_log" "pthread")
