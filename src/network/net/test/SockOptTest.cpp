#include "SocketOpt.h"
#include "InetAddress.h"
#include <iostream>

using namespace tmms::network;

void TestClient(){
    int sock = SocketOpt::CreateNonblockingTcpSocket(AF_INET);

    if (sock < 0) {
        std::cerr << "Failed to create socket" << std::endl;
        exit(-1);
    }

    //在本虚拟机上运行：
    //nc -l 4444 监听4444端口，可供程序连接
    InetAddress server("172.16.7.128:4444");

    SocketOpt opt(sock);
    opt.SetNonBlocking(false);
    auto ret = opt.Connet(server);

    std::cout<<"connet result:"<<ret << " --err: "<<errno<<std::endl
            <<"loacl :"<<opt.GetLocalAddr()->ToIpPort()<<std::endl
            <<"peer : "<<opt.GetPeerAddr()->ToIpPort()<<std::endl;

}

void TestServer(){
    std::cout<<"into server:"<<std::endl;

    int sock = SocketOpt::CreateNonblockingTcpSocket(AF_INET);

    if (sock < 0) {
        std::cerr << "Failed to create socket" << std::endl;
        exit(-1);
    }

    //在本虚拟机上运行：
    //ab -n 1 -c 1 -k 172.16.7.128:1234/ 对服务器进行测试
    // -n 数量 -c 并发数 -k keepalive
    InetAddress server("0.0.0.0:1234");

    SocketOpt opt(sock);
    opt.SetNonBlocking(false);
    std::cout<<"set nonblocking err:"<<errno<<std::endl;
    
    opt.BindAddress(server);
    std::cout<<"bind err: "<<errno<<std::endl;
    opt.Listen();
    std::cout<<"listen err: "<<errno<<std::endl;
    InetAddress addr;
    auto ns = opt.Accept(&addr);


    std::cout<<"connet result:"<<ns << " --err: "<<errno<<std::endl
            <<"addr :"<<addr.ToIpPort()<<std::endl;
}

int main() {
    // TestClient();
    TestServer();

    return 0;
}