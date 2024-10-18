#include "EventLoop.h"
#include "NetWork.h"
#include <cstring>
#include <fcntl.h>
#include <sys/socket.h>
#include <unistd.h>

using namespace tmms::network;

static thread_local EventLoop * t_local_eventLoop = nullptr;

EventLoop::EventLoop()
:epoll_fd_(::epoll_create(1024)),
epoll_events_(1024)//初始化vector的大小，当不超过1024时不重新分配内存
{
    if(t_local_eventLoop){
        NETWORK_ERR <<" there already has a EventLoop";
        exit(-1);
    }
    t_local_eventLoop = this;
}

EventLoop::~EventLoop(){
    Quit();
}

void EventLoop::Loop(){
    looping_ = true;

    while(looping_){
        memset(&epoll_events_[0],0x00,sizeof(struct epoll_event)*epoll_events_.size());

        auto ret = epoll_wait(epoll_fd_,
        (struct epoll_event*)&epoll_events_[0],
        static_cast<int>(epoll_events_.size()),//类型转换
        -1);

        if(ret >= 0){
            for(int i = 0; i < ret; ++i){
                struct epoll_event& ev = epoll_events_[i];
                //判断有效性
                if(ev.data.fd <= 0){
                    continue;
                }

                auto iter = events_.find(ev.data.fd);
                if(iter == events_.end()){
                    continue;
                }

                EventPtr & event = iter->second;

                if(ev.events&EPOLLERR){
                    int error = 0;
                    socklen_t len = sizeof(error);
                    getsockopt(event->Fd(),SOL_SOCKET,SO_ERROR,&error,&len);
                    event->OnError(strerror(error));
                }else if((ev.events&EPOLLHUP)&&!(ev.events&EPOLLIN)){
                    event->OnClose();
                }else if(ev.events & (EPOLLIN|EPOLLPRI)){
                    event->OnRead();
                }else if(ev.events & EPOLLOUT){
                    event->OnWrite();
                }
            }
            
            if(ret == epoll_events_.size()){
                epoll_events_.resize(epoll_events_.size()*2);
            }

            RunFunctions();

        }else if(ret < 0){
            NETWORK_ERR << "epoll_wait error.error:"<< errno ;
        }
    }
}

void EventLoop::Quit(){
    looping_ = false;
}


void EventLoop::AddEvent(const EventPtr& event){
    auto it = events_.find(event->Fd());
    if(it != events_.end()){
        return ;
    }
    event->event_ |= kEventRead;
    events_[event->Fd()] = event;

    struct epoll_event ev;
    memset(&ev,0x00,sizeof(struct epoll_event));
    ev.events = event->event_;
    ev.data.fd = event->fd_;
    epoll_ctl(epoll_fd_,EPOLL_CTL_ADD,event->fd_,&ev);
}

void EventLoop::DelEvent(const EventPtr& event){
    auto it = events_.find(event->Fd());
    if(it == events_.end()){
        return ;
    }
    events_.erase(it);

    struct epoll_event ev;
    memset(&ev,0x00,sizeof(struct epoll_event));
    ev.events = event->event_;
    ev.data.fd = event->fd_;
    epoll_ctl(epoll_fd_,EPOLL_CTL_DEL,event->fd_,&ev);

}

bool EventLoop::EnableEventWriting(const EventPtr& event,bool enable){
    auto it = events_.find(event->Fd());
    if(it == events_.end()){
        NETWORK_ERR << "cannt find event fd:"<<event->Fd();
        return false;
    }
    if(enable){
        event->event_ |= kEventWrite;
    }else{
        event->event_ &= ~kEventWrite;
    }

    struct epoll_event ev;
    memset(&ev,0x00,sizeof(struct epoll_event));
    ev.events = event->event_;
    ev.data.fd = event->fd_;
    epoll_ctl(epoll_fd_,EPOLL_CTL_MOD,event->fd_,&ev);

    return true;
}

bool EventLoop::EnableEventReading(const EventPtr& event,bool enable){
    auto it = events_.find(event->Fd());
    if(it == events_.end()){
        NETWORK_ERR << "cannt find event fd:"<<event->Fd();
        return false;
    }
    if(enable){
        event->event_ |= kEventRead;
    }else{
        event->event_ &= ~kEventRead;
    }

    struct epoll_event ev;
    memset(&ev,0x00,sizeof(struct epoll_event));
    ev.events = event->event_;
    ev.data.fd = event->fd_;
    epoll_ctl(epoll_fd_,EPOLL_CTL_MOD,event->fd_,&ev);

    return true;
}


void EventLoop::AssertInLoopThread(){
    if(!IsInLoopThread()){
        NETWORK_ERR << "not in loop thread";
        exit(-1);
    }
}

bool EventLoop::IsInLoopThread() const{
    return t_local_eventLoop == this;
}

void EventLoop::RunInLoop(const Func& f){
    if(IsInLoopThread()){
        f();
    }else{
        std::lock_guard<std::mutex> lk(lock_);
        fucntions_.push(f);

        WakeUp();
    }
}

void EventLoop::RunInLoop(Func&& f){
    if(IsInLoopThread()){
        f();
    }else{
        std::lock_guard<std::mutex> lk(lock_);
        fucntions_.push(std::move(f));

        WakeUp();
    }
}

void EventLoop::RunFunctions(){
    std::lock_guard<std::mutex> lk(lock_);
    while(!fucntions_.empty()){
        auto &f = fucntions_.front();
        f();
        fucntions_.pop();
    }
}

void EventLoop::WakeUp(){
    if(!pipe_event_){
        pipe_event_ = std::make_shared<PipeEvent>(this);
        AddEvent(pipe_event_);
    }   

    int64_t tmp = 1;
    pipe_event_->Write((const char*)&tmp,sizeof(tmp));
}