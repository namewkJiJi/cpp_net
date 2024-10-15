cmake_minimum_required(VERSION 2.6)
project(main)

aux_source_directory(. SRCS) 
# 这个命令会在当前目录（.）中查找所有源文件，并将它们的路径存储在变量 SRCS 中。这些源文件通常是 .cpp 或 .c 文件。
add_executable(main ${SRCS})
# 使用之前定义的 SRCS 变量，创建一个名为 tmms 的可执行文件。CMake 将会编译 SRCS 中列出的所有源文件以生成该可执行文件。

target_link_libraries(main base jsoncpp)

install(TARGETS main RUNTIME DESTINATION sbin)