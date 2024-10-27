#include "TcpConnection.h"
#include "NetWork.h"
#include "unistd.h"

using namespace tmms::network;

TcpConnection::TcpConnection(EventLoop * loop,int sockfd,
            const InetAddress & local_addr,
            const InetAddress & peer_addr)
:Connection(loop,sockfd,local_addr,peer_addr){

}

TcpConnection::~TcpConnection(){
    OnClose();
}

void TcpConnection::SetCloseCallback(const CloseConnectionCallback& cb){
    close_cb_ = cb;
}

void TcpConnection::SetCloseCallback(CloseConnectionCallback && cb){
    close_cb_ = std::move(cb);
}

void TcpConnection::OnClose(){
    loop_->AssertInLoopThread();
    //如果没关闭，那么该函数关掉它
    if(!closed_){
        if(close_cb_){
            close_cb_(std::dynamic_pointer_cast<TcpConnection>(shared_from_this()));
        }
        closed_ = true;//标识已经关闭了
        Close();
    }
}

void TcpConnection::ForceClose(){
    loop_->RunInLoop([this](){
        OnClose();
    });
}
            
void TcpConnection::OnRead(){
    //已关闭则不处理读事件
    if(closed_){
        NETWORK_ERR<<"host:"<<peer_addr_.ToIpPort()<<" had closed the connection";
        return;
    }

    while (true)
    {
        int err = 0;
        auto ret = msg_buffer_.readFd(fd_,&err);

        if(ret>0){
            //有数据要读
            if(msg_cb_){
                msg_cb_(std::dynamic_pointer_cast<TcpConnection>(shared_from_this()),
                        msg_buffer_);
            }   
        }else if(ret == 0){
            OnClose();
            break;
        }else{
            //发生错误
            if(err != EINTR && err != EAGAIN && err != EWOULDBLOCK){
                NETWORK_ERR<<"read err:"<<err;
                OnClose();
            }
            break;
        }
    }

}

void TcpConnection::SetRecvMsgCallback(const MessageCallback& cb){
    msg_cb_ = cb;
}

void TcpConnection::SetRecvMsgCallback(MessageCallback&& cb){
    msg_cb_ = std::move(cb);    
}
            
void TcpConnection::OnError(const std::string & msg){
    NETWORK_ERR<<"host:"<<peer_addr_.ToIpPort()<<" err msg:"<<msg;
    OnClose();
}

void TcpConnection::OnWrite(){
    if(closed_){
        NETWORK_TRACE <<"host:"<<peer_addr_.ToIpPort()<<" is closed";
        return;
    }

    //发送列表不为空则开始发送
    if(!io_vec_list_.empty()){
        while(true){
            auto ret = writev(fd_,&io_vec_list_[0],io_vec_list_.size());
            if(ret >= 0){
                //发送成功，删除已发送的数据
                while(ret >0){
                    if(io_vec_list_.front().iov_len > ret){
                        io_vec_list_.front().iov_len -= ret;
                        io_vec_list_.front().iov_base = (char*)io_vec_list_.front().iov_base + ret;
                        break;
                    }else{
                        ret -= io_vec_list_.front().iov_len;
                        io_vec_list_.erase(io_vec_list_.begin());
                    }
                }
                if(io_vec_list_.empty()){
                    EnableWriting(false);
                    if(write_complete_cb_){
                        write_complete_cb_(std::dynamic_pointer_cast<TcpConnection>(shared_from_this()));
                    }
                    return;
                }
            }else{
                if(errno != EAGAIN && errno != EAGAIN && errno != EWOULDBLOCK){
                    NETWORK_ERR<<"host:"<<peer_addr_.ToIpPort()<<" write err:"<<errno;
                    OnClose();
                    return;
                }
                break;
            }
        }
    }else{
        EnableWriting(false);
        if(write_complete_cb_){
            write_complete_cb_(std::dynamic_pointer_cast<TcpConnection>(shared_from_this()));
        }
    }
}

void TcpConnection::SetWriteCompleteCallback(const WritCompleteCallback & cb){
    write_complete_cb_ = cb;
}

void TcpConnection::SetWriteCompleteCallback(WritCompleteCallback && cb){
    write_complete_cb_ = std::move(cb);
}

void TcpConnection::Send(std::list<BufferNodePtr> &list){
    loop_->RunInLoop([this,&list](){
        SendInLoop(list);
    });
}

void TcpConnection::Send(const char * buf,size_t size){
    loop_->RunInLoop([this,buf,size](){
        SendInLoop(buf,size);
    });
}

void TcpConnection::SendInLoop(std::list<BufferNodePtr>&list){
    if(closed_){
        NETWORK_TRACE <<"host:"<<peer_addr_.ToIpPort()<<" is closed";
        return;
    }

    for(auto &node:list){
        struct iovec vec;
        vec.iov_base = (void *)node->addr;
        vec.iov_len = node->size;
        io_vec_list_.push_back(vec);
    }
    if(!io_vec_list_.empty()){
        EnableWriting(true);
    }
}

void TcpConnection::SendInLoop(const char*buf,size_t size){
    if(closed_){
        NETWORK_TRACE <<"host:"<<peer_addr_.ToIpPort()<<" is closed";
        return;
    }

    size_t send_len = 0;
    if(io_vec_list_.empty()){
        send_len = write(fd_,buf,size);
        if(send_len<0){
            //出错
            if(errno != EAGAIN && errno != EAGAIN && errno != EWOULDBLOCK){
                NETWORK_ERR<<"host:"<<peer_addr_.ToIpPort()<<" write err:"<<errno;
                OnClose();
                return;
            }
            send_len = 0;
        }
        size -= send_len;
        if(size == 0){
            if(write_complete_cb_){
                write_complete_cb_(std::dynamic_pointer_cast<TcpConnection>(shared_from_this()));
            }
            return;
        }
    }
    if(size > 0){
        struct iovec vec;
        vec.iov_base = (void*)(buf + send_len);
        vec.iov_len = size;
        io_vec_list_.push_back(vec);
        EnableWriting(true);
    }
}
            
void TcpConnection::OnTimeout(){
    NETWORK_ERR<<"host:"<<peer_addr_.ToIpPort()<<" Timeout.";
    OnClose();
}

void TcpConnection::SetTimeoutCallback(int timeout,const TimeoutCallback & cb){
    auto cp = std::dynamic_pointer_cast<TcpConnection>(shared_from_this());
    loop_->RunAfter(timeout,[&cp,&cb](){
        cb(cp);
    });
}

void TcpConnection::SetTimeoutCallback(int timeout,TimeoutCallback && cb){
    auto cp = std::dynamic_pointer_cast<TcpConnection>(shared_from_this());
    loop_->RunAfter(timeout,[&cp,cb](){
        cb(cp);
    });
}

void TcpConnection::EnableCheckIdleTimeout(int32_t max_time){
    auto tp = std::make_shared<TimeoutEntry>(std::dynamic_pointer_cast<TcpConnection>(shared_from_this()));
    max_idle_time_ = max_time;
    timeout_entry_ = tp;
    loop_->InsertEntry(max_time,tp);
}

void TcpConnection::ExtendLife(){
    auto tp = timeout_entry_.lock();
    if(tp){
        loop_->InsertEntry(max_idle_time_,tp);
    }
}