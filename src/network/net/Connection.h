#pragma once

#include"InetAddress.h"
#include "Event.h"
#include "EventLoop.h"
#include <functional>
#include <unordered_map>
#include <memory>
#include <atomic>

namespace tmms{
    namespace network{
        //私有数据类型
        enum{
            kNormalContext = 0,
            kRtmpContext,
            kHttpContext,
            kUserContext,
            kFlvContext,
        };
        
        class Connection;

        using ContextPtr = std::shared_ptr<void>;
        using ConnectionPtr = std::shared_ptr<Connection>;
        using ActiveCallback = std::function<void(const ConnectionPtr &)>;    

        class Connection: public Event{
        public:
            Connection(EventLoop * loop,
                        int fd,
                        const InetAddress & localAddr,
                        const InetAddress & peerAddr);
                    
            virtual ~Connection() = default;

            //地址相关函数
            void SetLocalAddr(const InetAddress & local);
            void SetPeerAddr(const InetAddress & peer);
            const InetAddress& LocalAddr() const;
            const InetAddress& PeerAddr() const;

            //私有数据相关函数
            void SetContext(int type,const ContextPtr & context);
            void SetContext(int type,ContextPtr && context);
            template <typename T> 
            std::shared_ptr<T> GetContext(int type) const{
                auto it = contexts_.find(type);
                if(it != contexts_.end()){
                    return std::dynamic_pointer_cast<T>(it->second);
                }
                return std::shared_ptr<T>();
            }

            void ClearContext(int type);
            void ClearContext();

            //激活相关函数
            void SetActiveCallback(const ActiveCallback & cb);
            void SetActiveCallback(ActiveCallback && cb);
            void Active();
            void Deactive();

            //关闭函数
            virtual void ForceClose() = 0;

        private:
            std::unordered_map<int,ContextPtr> contexts_;   
            ActiveCallback active_cb_;
            std::atomic<bool> active_{false};
        
        protected:
            InetAddress local_addr_;
            InetAddress peer_addr_;
        };
    }
}