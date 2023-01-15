# 设置cmake的最低版本
cmake_minimum_required(VERSION 3.10)

# 指定为C++11 版本
set(CMAKE_CXX_STANDARD 11)

project(zlk_log VERSION 1.0)

include_directories("${CMAKE_BINARY_DIR}/include")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY  "${PROJECT_BINARY_DIR}/lib") #.so
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib")  #.a

file(GLOB_RECURSE CURRENT_HEADERS  ${PROJECT_SOURCE_DIR}/*.h )
file(GLOB_RECURSE SRC  ${PROJECT_SOURCE_DIR}/zlk_log.cc )

add_library(${PROJECT_NAME} SHARED ${SRC} ${CURRENT_HEADERS})

set(CMAKE_INSTALL_PREFIX /usr/local)
install(TARGETS zlk_log
        EXPORT MyLibTargets 
        LIBRARY DESTINATION lib  # 动态库安装路径 /usr/local/lib/libtcpFunc.so
        ARCHIVE DESTINATION lib  # 静态库安装路径
        )

project(main VERSION 1.0)

include_directories("${CMAKE_BINARY_DIR}/include")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY  "${PROJECT_BINARY_DIR}/lib") #.so
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib")  #.a

file(GLOB_RECURSE CURRENT_HEADERS  ${PROJECT_SOURCE_DIR}/*.h )
file(GLOB_RECURSE SRC  ${PROJECT_SOURCE_DIR}/main.cc )

add_executable(${PROJECT_NAME}  ${SRC} ${CURRENT_HEADERS})
target_link_libraries(${PROJECT_NAME} zlk_log pthread)