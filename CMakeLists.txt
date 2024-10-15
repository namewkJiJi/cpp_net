cmake_minimum_required(VERSION 2.6)
project(tmms)

set(CMAKE_INSTALL_PREFIX ../bin)


# 此为cmake自动安装jsoncpp库的方法，但未成功，采用手动安装
# include(ExternalProject)
# ExternalProject_Add(jsoncpp
#     SOURCE_DIR ${PROJECT_SOURCE_DIR}/third_party/jsoncpp
#     CONFIGURE_COMMAND mkdir -p ../jsoncpp-build &&cd ../jsoncpp-build && cmake ../jsoncpp -DCMAKE_INSTALL_PREFIX=${PROJECT_SOURCE_DIR}/lib/jsoncpp
#     BUILD_COMMAND cd ../jsoncpp-build && make
#     BUILD_IN_SOURCE 1
#     INSTALL_COMMAND cd ../jsoncpp-build && make install
# )

# 指定引用头文件路径
include_directories(${PROJECT_SOURCE_DIR}/lib/json/include/)
include_directories(${PROJECT_SOURCE_DIR}/src/base/)


# 指定链接库路径
# link_directories(${PROJECT_SOURCE_DIR}/lib/jsoncpp/lib64/)
link_directories(${PROJECT_SOURCE_DIR}/lib/json/lib/)
link_directories(${PROJECT_SOURCE_DIR}/lib/base)

# 设置运行时链接的路径
set(CMAKE_INSTALL_RPATH "${CMAKE_SOURCE_DIR}/lib/json/lib")

if (CMAKE_BUILD_TYPE STREQUAL Debug)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O0 -g -pipe -Wall -Werror -Wno-unused-variable -lrt -pthread")
elseif (NOT CMAKE_BUILD_TYPE STREQUAL Debug)
    message(STATUS "Release Version.")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O3 -g -pipe -Wall -Werror -Wno-unused-variable -lrt -pthread")
endif ()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-strict-aliasing -Wno-error=shadow -Wno-sign-compare")

message(STATUS "USING GCC VERSION: ${CMAKE_CXX_COMPILER_VERSION}")

if(CMAKE_CXX_COMPILER_VERSION VERSION_GREATER 7.2.9)
    message(STATUS "GCC Version > 7.3.0, USING -Wno-error=unused-result -Werror=return-type")
    set(CMAKE_CXX_FLAGS "-Wno-error=unused-result -Werror=return-type ${CMAKE_CXX_FLAGS}")
endif()


add_subdirectory(src)