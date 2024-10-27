#include "InetAddress.h"
#include <string>
#include <iostream>

using namespace std;
using namespace tmms::network;

int main(int argc,const char** argv){
    std::string host;

    while(cin >> host){
        InetAddress addr(host);
        cout << "host: "<<host << endl
             << "ip: " << addr.IP() << endl
             << "port: " << addr.Port() << endl
             << "lan: "<<addr.IsLanIp() <<endl
             << "wan: "<<addr.IsWanIp() <<endl
             << "loop: "<<addr.IsLoopbackIp() << endl;
    }
    
    return 0;
}