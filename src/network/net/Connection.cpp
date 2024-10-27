#include "Connection.h"

using namespace tmms::network;

Connection::Connection(EventLoop * loop,int fd,
                        const InetAddress & localAddr,
                        const InetAddress & peerAddr)
:Event(loop,fd),local_addr_(localAddr),peer_addr_(peerAddr){

}

void Connection::SetLocalAddr(const InetAddress & local){
    local_addr_ = local;
}

void Connection::SetPeerAddr(const InetAddress & peer){
    peer_addr_ = peer;
}

const InetAddress& Connection::LocalAddr() const{
    return local_addr_;
}

const InetAddress& Connection::PeerAddr() const{
    return peer_addr_;
}

void Connection::SetContext(int type,const ContextPtr & context){
    contexts_[type] = context;
}

void Connection::SetContext(int type,ContextPtr && context){
    contexts_[type] = std::move(context);
}

void Connection::ClearContext(int type){
    contexts_[type].reset();
}

void Connection::ClearContext(){
    contexts_.clear();
}

void Connection::SetActiveCallback(const ActiveCallback & cb){
    active_cb_ = cb;
}


void Connection::SetActiveCallback(ActiveCallback && cb){
    active_cb_ = std::move(cb);
}


void Connection::Active(){
    //已经激活了不需要激活第二次
    if(!active_.load()){
        loop_->RunInLoop([this](){
            active_.store(true);
            if(active_cb_){
                active_cb_(std::dynamic_pointer_cast<Connection>(shared_from_this()));
            }
        });
    }
}

void Connection::Deactive(){
    active_.store(false);
}