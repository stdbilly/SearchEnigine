#pragma once
#include "threadpool/MutexLock.h"
#include <sys/epoll.h>
#include <functional>
#include <map>
#include <memory>
#include <vector>
using std::function;
using std::map;
using std::shared_ptr;
using std::vector;

namespace wd {
class Acceptor;
class TCPConnection;

class EventLoop {
   public:
    using TCPConnectionPtr = shared_ptr<TCPConnection>;
    using TCPConnectionCallback = function<void(const TCPConnectionPtr&)>;
    using Functor = function<void()>;

    EventLoop(Acceptor& acceptor);
    void loop();
    void unloop();
    void runInloop(Functor&& cb);
    void setConnectionCallback(const TCPConnectionCallback&& cb) {
        _onConnection = std::move(cb);
    }

    void setMessageCallback(const TCPConnectionCallback&& cb) {
        _onMessage = std::move(cb);
    }

    void setCloseCallback(const TCPConnectionCallback&& cb) {
        _onClose = std::move(cb);
    }

   private:
    void waitEpollFd();
    void handleNewConnection();
    void handleMessage(int fd);
    int createEpollFd();

    void addEpollFdRead(int fd);
    void delEpollFdRead(int fd);
    bool isConnectionClosed(int fd);

    int createEventFd();
    void handleRead();
    void wakeup();
    void doPendingFunctors();

   private:
    int _efd;
    int _eventfd;
    Acceptor& _acceptor;
    vector<struct epoll_event> _eventList;
    map<int, TCPConnectionPtr> _conns;
    bool _isLooping;
    MutexLock _mutex;
    vector<Functor> _pendingFunctors;

    TCPConnectionCallback _onConnection;
    TCPConnectionCallback _onMessage;
    TCPConnectionCallback _onClose;
};

}  // namespace wd
