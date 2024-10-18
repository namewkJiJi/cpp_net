# pragma once

#include "NonCopyable.h"
#include "EventLoop.h"
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>


namespace tmms{
    namespace network{
        class EventLoopThread{
        public:
            EventLoopThread();
            ~EventLoopThread();

            void Run();
            void StartEventLoop();
            EventLoop* Loop() const;
            std::thread & Thread();
        private:
            EventLoop* loop_{nullptr};
            std::thread thread_;
            bool running_{false};
            std::mutex lock_;
            std::condition_variable condition_;
            std::once_flag once_;
            std::promise<int> promise_loop_;
        };

    }
}