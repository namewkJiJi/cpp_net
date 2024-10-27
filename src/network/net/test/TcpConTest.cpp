#include "Acceptor.h"
#include "EventLoop.h"
#include "EventLoopThread.h"
#include "TcpConnection.h"
#include <iostream>
//4-18 最后 超时事件的测试


using namespace tmms::network;

EventLoopThread eventloop_thread;
std::thread th;
const char* http_resp= "HTTP/1.0 200 OK\r\nServer: tmms\r\n\r\n";

int main(){
    eventloop_thread.Run();
    EventLoop * loop = eventloop_thread.Loop();

    if(loop){
        std::vector<TcpConnectionPtr> list;

        InetAddress server("172.16.7.128:4567");
        std::shared_ptr<Acceptor> acceptor = std::make_shared<Acceptor>(loop, server);
        
        acceptor->SetAcceptCallback([&loop,&server,&list](int fd,const InetAddress &addr){
            std::cout<< "host: "<<addr.ToIpPort() << std::endl;
            TcpConnectionPtr con = std::make_shared<TcpConnection>(loop,fd,server,addr);
            con->SetRecvMsgCallback([](const TcpConnectionPtr& con,MsgBuffer & buf){
                std::cout<<"recv msg:"<<buf.peek()<<std::endl;
                buf.retrieveAll();
                con->Send(http_resp,strlen(http_resp));
            });
            con->SetWriteCompleteCallback([&loop](const TcpConnectionPtr&con){
                std::cout<< "write complete host:"<<con->PeerAddr().ToIpPort()<<std::endl;
                loop->DelEvent(con);
                con->ForceClose();
            });

            list.push_back(con);
            loop->AddEvent(con);

        });

        acceptor->Start();

        while(1){
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }

    return 0;
}
