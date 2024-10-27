#include "EventLoop.h"
#include "NetWork.h"
#include "TTime.h"
#include <cstring>
#include <fcntl.h>
#include <sys/socket.h>
#include <unistd.h>

using namespace tmms::network;

static thread_local EventLoop * t_local_eventLoop = nullptr;

EventLoop::EventLoop()
:epoll_fd_(::epoll_create(1024)),//参数没影响？
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

bool EventLoop::HandleEvent(struct epoll_event& ev){
    //如果事件无效则继续处理下一个事件
    if(ev.data.fd <= 0){
        return false;
    }

    //在事件表中查找事件
    auto iter = events_.find(ev.data.fd);
    
    if(iter == events_.end()){
        //该事件不在时间表中
        return false;
    }

    //获取Event类的指针
    EventPtr & event = iter->second;


    //根据事件类型处理事件
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
    }else{
        return false;
    }

    return true;    
}

void EventLoop::Loop(){
    looping_ = true;
    int64_t timeout = 1000;//毫秒

    while(looping_){
        memset(&epoll_events_[0],0x00,sizeof(struct epoll_event)*epoll_events_.size());

        //waits for events on the epoll_fd_
        //第二个参数：store returned events
        //即使发生的事件数量超过了数组大小，epoll_wait 也不会试图写入越界的数据
        auto ret = epoll_wait(epoll_fd_,
        (struct epoll_event*)&epoll_events_[0],
        static_cast<int>(epoll_events_.size()),//类型转换
        timeout//The maximum time (in milliseconds) to wait for events.
        );

        //等待Event成功
        if(ret >= 0){
            for(int i = 0; i < ret; ++i){//遍历所有事件
                //取出事件流中的一个事件
                struct epoll_event& ev = epoll_events_[i];
                auto res = HandleEvent(ev);
            }
            
            if(ret >= epoll_events_.size()){
                epoll_events_.resize(ret + 1);
            }

            RunFunctions();

            int64_t now = tmms::base::TTime::NowMS();
            wheel_.OnTimer(now);

        }else if(ret < 0){
            //等待event失败
            NETWORK_ERR << "epoll_wait error.error:"<< errno ;
        }
    }
}

void EventLoop::Quit(){
    looping_ = false;
}


// 向事件循环中添加事件
/*
??设置事件为可读事件
在事件循环中的事件多为需要读取的事件
当关联的文件描述符上有数据可读时，epoll会通知我们。
*/
void EventLoop::AddEvent(const EventPtr& event){
    // 查找事件是否已经存在
    auto it = events_.find(event->Fd());
    // 如果事件已经存在，则直接返回
    if(it != events_.end()){
        return ;
    }
    // 设置事件为可读事件
    event->event_ |= kEventRead;
    // 将事件添加到事件列表中
    events_[event->Fd()] = event;

    // 创建epoll事件
    struct epoll_event ev;
    // 将epoll事件结构体清零
    memset(&ev,0x00,sizeof(struct epoll_event));
    // 设置epoll事件类型
    ev.events = event->event_;
    // 设置epoll事件对应的文件描述符
    ev.data.fd = event->fd_;
    // 将事件添加到epoll中
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

//对事件的可写性进行设置
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

void EventLoop::InsertEntry(uint32_t delay,EntryPtr entryPtr){
    if(IsInLoopThread()){
        wheel_.InsertEntry(delay,entryPtr);
    }else{
        RunInLoop([this,delay,entryPtr](){
            wheel_.InsertEntry(delay,entryPtr);
        });
    }
}

void EventLoop::RunAfter(double delay,const Func & cb){
    if(IsInLoopThread()){
        wheel_.RunAfter(delay,cb);
    }else{
        RunInLoop([this,delay,cb](){
            wheel_.RunAfter(delay,cb);
        });
    }
}

void EventLoop::RunAfter(double delay,Func && cb){
    if(IsInLoopThread()){
        wheel_.RunAfter(delay,cb);
    }else{
        RunInLoop([this,delay,cb](){
            wheel_.RunAfter(delay,cb);
        });
    }
}

void EventLoop::RunEvery(double interval, const Func & cb){
    if(IsInLoopThread()){
        wheel_.RunEvery(interval,cb);
    }else{
        RunInLoop([this,interval,cb](){
            wheel_.RunEvery(interval,cb);
        });
    }
}

void EventLoop::RunEvery(double interval, Func && cb){
    if(IsInLoopThread()){
        wheel_.RunEvery(interval,cb);
    }else{
        RunInLoop([this,interval,cb](){
            wheel_.RunEvery(interval,cb);
        });
    }
}

