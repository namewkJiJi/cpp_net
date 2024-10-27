cmake_minimum_required(VERSION 2.6)
project(net_test)

# add_executable(EventLoopThreadTest EventLoopThreadTest.cpp)
# target_link_libraries(EventLoopThreadTest base network)

# install(TARGETS EventLoopThreadTest RUNTIME DESTINATION sbin/test)

# add_executable(InetAddressTest InetAddressTest.cpp)
# target_link_libraries(InetAddressTest base network)
# install(TARGETS InetAddressTest RUNTIME DESTINATION sbin/test)

# add_executable(SockOptTest SockOptTest.cpp)
# target_link_libraries(SockOptTest base network)
# install(TARGETS SockOptTest RUNTIME DESTINATION sbin/test)

# add_executable(AcceptorTest AcceptorTest.cpp)
# target_link_libraries(AcceptorTest base network)
# install(TARGETS AcceptorTest RUNTIME DESTINATION sbin/test)

add_executable(TcpConTest TcpConTest.cpp)
target_link_libraries(TcpConTest base network)
install(TARGETS TcpConTest RUNTIME DESTINATION sbin/test)
