#include "SocketOpt.h"
#include "NetWork.h"

using namespace tmms::network;

SocketOpt::SocketOpt(int sock,bool is_v6)
:sock_(sock),is_v6_(is_v6){

}

int SocketOpt::CreateNonblockingTcpSocket(int family){
    int sock = socket(family,SOCK_STREAM|SOCK_NONBLOCK|SOCK_CLOEXEC,IPPROTO_TCP);
    if(sock < 0){
        NETWORK_ERR << "create tcp socket failed.";
    }
    return sock;
}

int SocketOpt::CreateNonblockingUdpSocket(int family){
    int sock = socket(family,SOCK_DGRAM|SOCK_NONBLOCK|SOCK_CLOEXEC,IPPROTO_UDP);
    if(sock < 0){
        NETWORK_ERR << "create udp socket failed.";
    }
    return sock;
}


int SocketOpt::BindAddress(const InetAddress & localaddr){
    if(localaddr.IsIpv6()){
        struct sockaddr_in6 addr;
        localaddr.GetSockAddr((struct sockaddr*)&addr);
        socklen_t size = sizeof(struct sockaddr_in6);
        return bind(sock_,(struct sockaddr*)&addr,size);
    }else{
        struct sockaddr_in addr;
        localaddr.GetSockAddr((struct sockaddr *)&addr);

        socklen_t size = sizeof(struct sockaddr_in);
        return bind(sock_,(struct sockaddr*)&addr,size);
    }
}

int SocketOpt::Listen(){
    return listen(sock_,SOMAXCONN);
}

int SocketOpt::Accept(InetAddress * peeraddr){
    struct sockaddr_in6 addr;//不知道是v6还是v4，用大的
    socklen_t size = sizeof(struct sockaddr_in6);
    int acc = accept4(sock_,(struct sockaddr*)&addr,&size,SOCK_CLOEXEC|SOCK_NONBLOCK);

    if(acc > 0){
        if(addr.sin6_family == AF_INET){
            char ip[16] = {0,};
            struct sockaddr_in * addr4 = (struct sockaddr_in*)&addr;
            
            inet_ntop(AF_INET,&addr4->sin_addr.s_addr,ip,sizeof(ip));
            
            peeraddr->SetAddr(ip);
            peeraddr->SetPort(ntohs(addr4->sin_port));
        }else if(addr.sin6_family == AF_INET6){
            char ip[INET6_ADDRSTRLEN] = {0,};
            struct sockaddr_in6 * addr6 = (struct sockaddr_in6*)&addr;

            inet_ntop(AF_INET6,&addr6->sin6_addr.s6_addr,ip,sizeof(ip));

            peeraddr->SetAddr(ip);
            peeraddr->SetPort(ntohs(addr6->sin6_port));
            peeraddr->SetIsV6(true);
        }
    }

    return acc;
}

int SocketOpt::Connet(const InetAddress& addr){
    struct sockaddr_in6 addr_in;
    addr.GetSockAddr((struct sockaddr *)&addr_in);

    return connect(sock_,(struct sockaddr*)&addr_in,sizeof(struct sockaddr_in6));
}

InetAddressPtr SocketOpt::GetLocalAddr(){
    struct sockaddr_in6 addr;
    socklen_t len = sizeof(struct sockaddr_in6);

    getsockname(sock_,(struct sockaddr*)&addr,&len);
    InetAddressPtr local_addr = std::make_shared<InetAddress>();

    if(addr.sin6_family == AF_INET){
        char ip[16] = {0,};
        struct sockaddr_in * addr4 = (struct sockaddr_in*)&addr;
        
        inet_ntop(AF_INET,&addr4->sin_addr.s_addr,ip,sizeof(ip));
        
        local_addr->SetAddr(ip);
        local_addr->SetPort(ntohs(addr4->sin_port));
    }else if(addr.sin6_family == AF_INET6){
        char ip[INET6_ADDRSTRLEN] = {0,};
        struct sockaddr_in6 * addr6 = (struct sockaddr_in6*)&addr;

        inet_ntop(AF_INET6,&addr6->sin6_addr.s6_addr,ip,sizeof(ip));

        local_addr->SetAddr(ip);
        local_addr->SetPort(ntohs(addr6->sin6_port));
        local_addr->SetIsV6(true);
    }

    return local_addr;
}

InetAddressPtr SocketOpt::GetPeerAddr(){
    struct sockaddr_in6 addr;
    socklen_t len = sizeof(struct sockaddr_in6);

    getpeername(sock_,(struct sockaddr*)&addr,&len);
    InetAddressPtr peer_addr = std::make_shared<InetAddress>();

    if(addr.sin6_family == AF_INET){
        char ip[16] = {0,};
        struct sockaddr_in * addr4 = (struct sockaddr_in*)&addr;
        
        inet_ntop(AF_INET,&addr4->sin_addr.s_addr,ip,sizeof(ip));
        
        peer_addr->SetAddr(ip);
        peer_addr->SetPort(ntohs(addr4->sin_port));
    }else if(addr.sin6_family == AF_INET6){
        char ip[INET6_ADDRSTRLEN] = {0,};
        struct sockaddr_in6 * addr6 = (struct sockaddr_in6*)&addr;

        inet_ntop(AF_INET6,&addr6->sin6_addr.s6_addr,ip,sizeof(ip));

        peer_addr->SetAddr(ip);
        peer_addr->SetPort(ntohs(addr6->sin6_port));
        peer_addr->SetIsV6(true);
    }

    return peer_addr;
}

void SocketOpt::SetTcpNoDelay(bool on){
    int optvalue = on ? 1 : 0;
    setsockopt(sock_,IPPROTO_TCP,TCP_NODELAY,&optvalue,sizeof(optvalue));
}

void SocketOpt::SetReuseAddr(bool on){
    int optvalue = on ? 1 : 0;
    setsockopt(sock_,SOL_SOCKET,SO_REUSEADDR,&optvalue,sizeof(optvalue));
}

void SocketOpt::SetReusePort(bool on){
    int optvalue = on ? 1 : 0;
    setsockopt(sock_,SOL_SOCKET,SO_REUSEPORT,&optvalue,sizeof(optvalue));
}

void SocketOpt::SetKeepAlive(bool on){
    int optvalue = on ? 1 : 0;
    setsockopt(sock_,SOL_SOCKET,SO_KEEPALIVE,&optvalue,sizeof(optvalue));
}

void SocketOpt::SetNonBlocking(bool on){
    int flag = fcntl(sock_,F_GETFL,0);
    if(on){
        flag |= O_NONBLOCK;
    }else{
        flag &= ~O_NONBLOCK;
    }
    fcntl(sock_,F_SETFL,flag);
}