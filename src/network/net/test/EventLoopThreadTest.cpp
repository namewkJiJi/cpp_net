#include<iostream>
#include"EventLoopThread.h"
#include "EventLoopThreadPool.h"
#include"EventLoop.h"
#include "PipeEvent.h"
#include "TTime.h"
#include <thread>

using namespace tmms::network;

EventLoopThread eventloop_thread;
std::thread th;

void TestEventLoopThread(){
    eventloop_thread.Run();
    EventLoop * loop = eventloop_thread.Loop();
    if(loop){
        std::cout << "loop :"<< loop << std::endl;
        PipeEventPtr pipe = std::make_shared<PipeEvent>(loop);
        loop->AddEvent(pipe);
        int64_t test = 12345;
        pipe->Write((const char*)&test,sizeof(test));

        th = std::thread([&pipe](){
            int64_t now = tmms::base::TTime::NowMS();
            pipe->Write((const char*)&now,sizeof(now));
            while(1){
                int64_t now = tmms::base::TTime::NowMS();
                pipe->Write((const char*)&now,sizeof(now));
 
                std::this_thread::sleep_for(std::chrono::seconds(1));
            }
        });
        while(1){
            std::this_thread::sleep_for(std::chrono::seconds(50));
        }
    }
}

void TestEventLoopThreadPool(){
    EventLoopThreadPool pool(2,0,2);

    pool.Start();
    // std::cout<<"thread id : "<<std::this_thread::get_id()<<std::endl;

    // std::vector<EventLoop *> list = pool.GetLoops();
    // for(auto & loop : list){
    //     loop->RunInLoop([&loop](){
    //         std::cout << "loop :"<< loop <<" thread id : "<<std::this_thread::get_id()<< std::endl;
    //     });
        
    // }

    // EventLoop * loop = pool.GetNextLoop();
    // std::cout << "loop :"<< loop << std::endl;

    EventLoop * loop = pool.GetNextLoop();
    std::cout << "loop :"<< loop << std::endl;
    loop->RunAfter(1,[](){
        std::cout << "run after 1s now : "<< tmms::base::TTime::Now() << std::endl;
    });

    loop->RunAfter(5,[](){
        std::cout << "run after 5s now : "<< tmms::base::TTime::Now() << std::endl;
    });

    loop->RunEvery(1,[](){
        std::cout << "run every 1s now : "<< tmms::base::TTime::Now() << std::endl;
    });

    loop->RunEvery(5,[](){
        std::cout << "run every 5s now : "<< tmms::base::TTime::Now() << std::endl;
    });

    //不退出这个函数
    //一旦退出会析构掉线程池
    //析构后会出现问题？？
    while(1){
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

}

int main(){
    TestEventLoopThreadPool();
    while(1){
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    return 0;
}