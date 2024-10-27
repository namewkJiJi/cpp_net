#pragma once


#include "EventLoop.h"
#include "Event.h"
#include "InetAddress.h"
#include "SocketOpt.h"
#include <string>
#include <functional>

namespace tmms{
    namespace network{
        using AcceptCallback = std::function<void(int sock,const InetAddress & addr)>;

        class Acceptor: public Event{
        public:
            Acceptor(EventLoop * loop,const InetAddress & addr);
            ~Acceptor();

            void SetAcceptCallback(const AcceptCallback & cb);
            void SetAcceptCallback(AcceptCallback && cb);
            void Start();
            void Stop();
            void OnRead() override;
            void OnError(const std::string & msg) override;
            void OnClose() override;

        private:
            void Open();

            InetAddress addr_;
            AcceptCallback accept_cb_;
            SocketOpt * socket_opt_{nullptr};
        };
    }
}