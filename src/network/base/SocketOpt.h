#pragma once

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <unistd.h>
#include <fcntl.h>
#include <memory>
#include "InetAddress.h"

namespace tmms{
    namespace network{
        using InetAddressPtr = std::shared_ptr<InetAddress>;
        class SocketOpt{
        public:
            SocketOpt(int sock,bool is_v6=false);
            ~SocketOpt() = default;

            //创建socket
            static int CreateNonblockingTcpSocket(int family);
            static int CreateNonblockingUdpSocket(int family);

            //服务器流程
            int BindAddress(const InetAddress & localaddr);
            int Listen();
            int Accept(InetAddress * peeraddr);

            //客户端流程
            int Connet(const InetAddress& addr);

            //获取地址
            InetAddressPtr GetLocalAddr();
            InetAddressPtr GetPeerAddr();

            //使能函数
            void SetTcpNoDelay(bool on);
            void SetReuseAddr(bool on);
            void SetReusePort(bool on);
            void SetKeepAlive(bool on);
            void SetNonBlocking(bool on);

        private:
            int sock_{-1};
            bool is_v6_{false};
        };
    }
}