#include "Acceptor.h"
#include "NetWork.h"

using namespace tmms::network;

Acceptor::Acceptor(EventLoop * loop,const InetAddress & addr)
:Event(loop),addr_(addr){
    
}

Acceptor::~Acceptor(){
    Stop();

    if(socket_opt_){
        delete socket_opt_;
        socket_opt_ = nullptr;
    }
}

void Acceptor::SetAcceptCallback(const AcceptCallback & cb){
    accept_cb_ = cb;
}

void Acceptor::SetAcceptCallback(AcceptCallback && cb){
    accept_cb_ = std::move(cb);
}

void Acceptor::Open(){
    //若原先存在文件，先关闭
    if(fd_ > 0){
        close(fd_);
        fd_ = -1;
    }

    if(addr_.IsIpv6()){
        fd_ = SocketOpt::CreateNonblockingTcpSocket(AF_INET6);

    }else{
        fd_ = SocketOpt::CreateNonblockingTcpSocket(AF_INET);
    }

    //检查是否成功打开
    if(fd_ < 0){
        NETWORK_ERR<<"socket failed . errno:"<<errno;
        exit(-1);
    }

    //清空原先存在的
    if(socket_opt_){
        delete socket_opt_;
        socket_opt_ = nullptr;
    }

    //添加到事件循环中
    loop_->AddEvent(std::dynamic_pointer_cast<Acceptor>(shared_from_this()));

    //socketopt 的设置
    socket_opt_ = new SocketOpt(fd_);
    socket_opt_->SetReuseAddr(true);
    socket_opt_->SetReusePort(true);
    socket_opt_->BindAddress(addr_);
    socket_opt_->Listen();
}

void Acceptor::Start(){
    loop_->RunInLoop([this](){
        this->Open();
    });
}

void Acceptor::Stop(){
    loop_->DelEvent(std::dynamic_pointer_cast<Acceptor>(shared_from_this()));
}

void Acceptor::OnRead(){
    if(!socket_opt_){
        return;
    }

    while(true){
        InetAddress addr;
        auto sock = socket_opt_->Accept(&addr);

        if(sock>=0){
            if(accept_cb_){
                accept_cb_(sock, addr);
            }
        }else{
            if(errno != EINTR && errno != EAGAIN){
                NETWORK_ERR<<"accept failed . errno:"<<errno;
                OnClose();
            }
            break;
        }
    }
}

void Acceptor::OnError(const std::string & msg){
    NETWORK_ERR<<"accept error:"<<msg;
    OnClose();
}

void Acceptor::OnClose(){
    Stop();
    Open();
}