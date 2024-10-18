cmake_minimum_required(VERSION 2.6)
project(net_test)



add_executable(EventLoopThreadTest EventLoopThreadTest.cpp)
target_link_libraries(EventLoopThreadTest base network)

install(TARGETS EventLoopThreadTest RUNTIME DESTINATION sbin)
