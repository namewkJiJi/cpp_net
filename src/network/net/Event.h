#pragma once

#include <string>
#include <memory>
#include <sys/epoll.h>

namespace tmms{
    namespace network{
        //避免循环依赖
        class EventLoop;//前置声明

        const int kEventRead = (EPOLLIN | EPOLLPRI | EPOLLET);
        const int kEventWrite = EPOLLOUT | EPOLLET;

        class Event : public std::enable_shared_from_this<Event>
        {
            friend class EventLoop;
        public:
            Event(EventLoop *loop);
            Event(EventLoop *loop,int fd);
            ~Event();

            virtual void OnRead() {};
            virtual void OnWrite() {};
            virtual void OnClose() {};
            virtual void OnError(const std::string & msg) {};
            
            bool EnableWriting(bool enable);
            bool EnableReading(bool enable);
            int Fd() const;

        protected:
            EventLoop *loop_{nullptr};
            int fd_{-1};
            int event_{0};
        };

    }
}