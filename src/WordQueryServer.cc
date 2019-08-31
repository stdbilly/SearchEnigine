#include "WordQueryServer.h"
using std::to_string;
using std::placeholders::_1;

namespace wd {
WordQueryServer::WordQueryServer(int threadNum, int queSize, const string& ip,
                                 unsigned short port)
    : _threadpool(threadNum, queSize), _server(ip, port), _wordquery() {}

void WordQueryServer::start() {
    _threadpool.start();
    _server.setConnectionCallback(
        std::bind(&WordQueryServer::onConnection, this, _1));
    _server.setMessageCallback(
        std::bind(&WordQueryServer::onMessage, this, _1));
    _server.setCloseCallback(std::bind(&WordQueryServer::onClose, this, _1));
    _server.start();
}

void WordQueryServer::onConnection(const TCPConnectionPtr& conn) {
    cout << conn->toString() << " connected" << endl;
    conn->send("welcome to server");
}

void WordQueryServer::onMessage(const TCPConnectionPtr& conn) {
    string msg = conn->receive();
    if (msg.back() == '\n') {
        msg.erase(msg.size() - 1, 1);
    }
    cout << ">> receive from client: " << msg << endl;

     _threadpool.addTask(std::bind(&WordQueryServer::process, this, conn, msg));
}

void WordQueryServer::onClose(const TCPConnectionPtr& conn) {
    cout << ">> " << conn->toString() << " disconnected" << endl;
}

//运行在线程池的某一个线程
void WordQueryServer::process(const TCPConnectionPtr& conn, const string& msg) {
    string ret = _wordquery.doQuery(msg);
    int sz = ret.size();

    string message(to_string(sz));
    message.append("\n").append(ret);
    cout << message << endl;    

    conn->sendInLoop(message);
}

}  // namespace wd
