# 设置cmake的最低版本
cmake_minimum_required(VERSION 3.10)

# 指定为C++11 版本
set(CMAKE_CXX_STANDARD 11)

project(main VERSION 1.0)
aux_source_directory(./ src)#将./ft目录中的文件名称赋值给变量 ft_src

add_executable(main  ${src})

#SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)