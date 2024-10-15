cmake_minimum_required(VERSION 2.6)
project(base)

# 路径已在顶层文件指定
link_libraries(jsoncpp)

aux_source_directory(. BASE_LIB_SRCS)

# 指定库的输出路径
# PROJECT_SOURCE_DIR 指的是当前cmakeLists.txt文件所在的目录
# set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/../../lib/base)

# CMAKE_SOURCE_DIR 指的是顶层cmake所在的目录
set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/lib/base)

add_library(base ${BASE_LIB_SRCS})

# 下面是base的测试代码所需的内容
# aux_source_directory(. SRCS) 
# # 这个命令会在当前目录（.）中查找所有源文件，并将它们的路径存储在变量 SRCS 中。这些源文件通常是 .cpp 或 .c 文件。
# add_executable(baseTest ${SRCS})
# # 使用之前定义的 SRCS 变量，创建一个名为 tmms 的可执行文件。CMake 将会编译 SRCS 中列出的所有源文件以生成该可执行文件。
# install(TARGETS baseTest RUNTIME DESTINATION sbin)