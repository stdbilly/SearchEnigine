#pragma once
#include "Acceptor.h"
#include "EventLoop.h"
#include "TCPConnection.h"

namespace wd {
class TCPServer {
   public:
    TCPServer(const string& ip, unsigned short port);
    void start();

    void setConnectionCallback(TCPConnectionCallback&& cb);
    void setMessageCallback(TCPConnectionCallback&& cb);
    void setCloseCallback(TCPConnectionCallback&& cb);

   private:
    Acceptor _acceptor;
    EventLoop _loop;
};

}  // namespace wd
