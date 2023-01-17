# 设置cmake的最低版本
cmake_minimum_required(VERSION 3.10)

# 指定为C++11 版本
set(CMAKE_CXX_STANDARD 11)

project(zftp_server VERSION 1.0)

include_directories("${CMAKE_BINARY_DIR}/include")
include_directories("${CMAKE_BINARY_DIR}/../../lib/log/include")
link_libraries("${CMAKE_BINARY_DIR}/lib")

file(GLOB_RECURSE CURRENT_HEADERS  ${PROJECT_SOURCE_DIR}/include/*.h )
file(GLOB_RECURSE SRC  ${PROJECT_SOURCE_DIR}/src/server/*.cc )
file(GLOB_RECURSE COMSRC  ${PROJECT_SOURCE_DIR}/src/common/*.cc )

add_executable(${PROJECT_NAME}  ${CURRENT_HEADERS} ${SRC} ${COMSRC})
target_link_libraries(${PROJECT_NAME}  "lua" "dl" "zlk_log" "pthread" "boost_system" "boost_thread" "protobuf") 


project(zftp_client VERSION 1.0)

include_directories("${CMAKE_BINARY_DIR}/include")
include_directories("${CMAKE_BINARY_DIR}/../../lib/log/include")
link_libraries("${CMAKE_BINARY_DIR}/lib")

file(GLOB_RECURSE CURRENT_HEADERS  ${PROJECT_SOURCE_DIR}/include/*.h )
file(GLOB_RECURSE SRC  ${PROJECT_SOURCE_DIR}/src/client/*.cc )
file(GLOB_RECURSE COMSRC  ${PROJECT_SOURCE_DIR}/src/common/*.cc )

add_executable(${PROJECT_NAME}  ${CURRENT_HEADERS} ${SRC} ${COMSRC})
target_link_libraries(${PROJECT_NAME}  "lua" "dl" "zlk_log" "pthread" "boost_system" "boost_thread" "protobuf") 
