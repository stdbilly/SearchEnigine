#pragma once
#include "threadpool/Threadpool.h"
#include "net/TCPServer.h"
#include "WordQuery.h"

namespace wd {
class WordQueryServer {
   public:
    WordQueryServer(int threadNum, int queSize, const string& ip,
               unsigned short port);

    void start();

    void onConnection(const TCPConnectionPtr& conn);
    void onMessage(const TCPConnectionPtr& conn);
    void onClose(const TCPConnectionPtr& conn);

    void process(const TCPConnectionPtr& conn, const string& msg);

   private:
    Threadpool _threadpool;
    TCPServer _server;
    WordQuery _wordquery;
};

}  // namespace wd
