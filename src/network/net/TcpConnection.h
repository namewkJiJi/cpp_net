#pragma once

#include "Connection.h"
#include "InetAddress.h"
#include "MsgBuffer.h"
#include <functional>
#include <memory>
#include <list>
#include <vector>
#include <sys/uio.h>

namespace tmms{
    namespace network{
        class TcpConnection;
        using TcpConnectionPtr = std::shared_ptr<TcpConnection>;
        using CloseConnectionCallback = std::function<void(const TcpConnectionPtr&)>;
        using MessageCallback = std::function<void(const TcpConnectionPtr&,MsgBuffer & buffer)>;
        using WritCompleteCallback = std::function<void(const TcpConnectionPtr&)>;
        using TimeoutCallback = std::function<void(const TcpConnectionPtr&)>;

        struct BufferNode{
            BufferNode(void * buf,size_t s)
            :addr(buf),size(s){}
            
            void * addr{nullptr};
            size_t size{0};
        };
        using BufferNodePtr = std::shared_ptr<BufferNode>;
        
        struct TimeoutEntry;

        class TcpConnection:public Connection{
        public:
            TcpConnection(EventLoop * loop,int sockfd,
                        const InetAddress & local_addr,
                        const InetAddress & peer_addr);
            ~TcpConnection();

            void SetCloseCallback(const CloseConnectionCallback& cb);
            void SetCloseCallback(CloseConnectionCallback && cb);
            //关闭事件
            void OnClose() override;
            void ForceClose() override;

            //读
            void OnRead() override;
            void SetRecvMsgCallback(const MessageCallback& cb);
            void SetRecvMsgCallback(MessageCallback&& cb);

            //出错
            void OnError(const std::string & msg) override;

            //写
            void OnWrite() override;
            void SetWriteCompleteCallback(const WritCompleteCallback & cb);
            void SetWriteCompleteCallback(WritCompleteCallback && cb);
            void Send(std::list<BufferNodePtr> &list);
            void Send(const char * buf,size_t size);

            //超时事件
            void OnTimeout();
            void SetTimeoutCallback(int timeout,const TimeoutCallback & cb);
            void SetTimeoutCallback(int timeout,TimeoutCallback && cb);
            void EnableCheckIdleTimeout(int32_t max_time);
            
        private:
            void ExtendLife();
            void SendInLoop(std::list<BufferNodePtr>&list);
            void SendInLoop(const char*buf,size_t size);
            
            bool closed_{false};    
            CloseConnectionCallback close_cb_;
            
            MsgBuffer msg_buffer_;
            MessageCallback msg_cb_;
        
            std::vector<struct iovec> io_vec_list_;            
            WritCompleteCallback write_complete_cb_;

            //弱引用，不占用智能指针的引用
            std::weak_ptr<TimeoutEntry> timeout_entry_;
            int32_t max_idle_time_{30};
        };
        
        struct TimeoutEntry{
            TimeoutEntry(const TcpConnectionPtr& c)
            :conn(c){

            }
            ~TimeoutEntry(){
                auto c = conn.lock();
                if(c){
                    c->OnTimeout();
                }
            }

            std::weak_ptr<TcpConnection> conn;
        };
    }
}