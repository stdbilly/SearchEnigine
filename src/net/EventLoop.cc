#include "net/EventLoop.h"
#include <assert.h>
#include <sys/eventfd.h>
#include <unistd.h>
#include <iostream>
#include "net/Acceptor.h"
#include "net/TCPConnection.h"
using std::cout;
using std::endl;

namespace wd {
EventLoop::EventLoop(Acceptor& acceptor)
    : _efd(createEpollFd()),
      _eventfd(createEventFd()),
      _acceptor(acceptor),
      _eventList(1024),
      _isLooping(false) {
    addEpollFdRead(_acceptor.fd());
    addEpollFdRead(_eventfd);
}

void EventLoop::loop() {
    _isLooping = true;
    while (_isLooping) {
        waitEpollFd();
    }
}

//运行在另一个线程
void EventLoop::unloop() {
    if (_isLooping) {
        _isLooping = false;
    }
}

void EventLoop::runInloop(Functor&& cb) {
    {
        MutexGuard autolock(_mutex);
        _pendingFunctors.push_back(std::move(cb));
    }
    wakeup();
}

void EventLoop::waitEpollFd() {
    int nready;
    do {
        nready =
            epoll_wait(_efd, &*_eventList.begin(), _eventList.size(), 15000);
    } while (nready == -1 && errno == EINTR);

    if (nready == -1) {
        perror("epoll_wait");
        return;
    } else if (nready == 0) {
        cout << ">> epoll_wait timeout!" << endl;
    } else {
        if (nready == _eventList.size()) {
            _eventList.resize(2 * nready);
        }

        for (int idx = 0; idx != nready; ++idx) {
            int fd = _eventList[idx].data.fd;
            if (fd == _acceptor.fd()) {
                //处理新连接
                if (_eventList[idx].events & EPOLLIN) {
                    handleNewConnection();
                }
            } else if (fd == _eventfd) {
                if (_eventList[idx].events & EPOLLIN) {
                    handleRead();
                    cout << ">> do pending functors" << endl;
                    doPendingFunctors();
                }
            } else {
                //处理消息
                if (_eventList[idx].events & EPOLLIN) {
                    handleMessage(fd);
                }
            }
        }
    }
}

void EventLoop::handleNewConnection() {
    int peerFd = _acceptor.accept();
    addEpollFdRead(peerFd);
    TCPConnectionPtr conn(new TCPConnection(peerFd, this));
    conn->setConnectionCallback(_onConnection);
    conn->setMessageCallback(_onMessage);
    conn->setCloseCallback(_onClose);
    _conns.insert(std::make_pair(peerFd, conn));
    conn->handleConnectionCallback();
}

void EventLoop::handleMessage(int fd) {
    bool isClosed = isConnectionClosed(fd);
    auto it = _conns.find(fd);
    assert(it != _conns.end());
    if (!isClosed) {
        it->second->handleMessageCallback();
    } else {
        delEpollFdRead(fd);
        it->second->handleCloseCallback();
        _conns.erase(it);
    }
}

bool EventLoop::isConnectionClosed(int fd) {
    int ret;
    do {
        char buf[1024];
        ret = recv(fd, buf, sizeof(buf), MSG_PEEK);
    } while (ret == -1 && errno == EINTR);
    return (ret == 0);
}

int EventLoop::createEpollFd() {
    int ret = ::epoll_create1(0);
    if (ret == -1) {
        perror("epoll_create1");
    }
    return ret;
}

void EventLoop::addEpollFdRead(int fd) {
    struct epoll_event evt;
    evt.data.fd = fd;
    evt.events = EPOLLIN;
    int ret = epoll_ctl(_efd, EPOLL_CTL_ADD, fd, &evt);
    if (ret == -1) {
        perror("epoll_ctl");
    }
}

void EventLoop::delEpollFdRead(int fd) {
    struct epoll_event evt;
    evt.data.fd = fd;
    int ret = epoll_ctl(_efd, EPOLL_CTL_DEL, fd, &evt);
    if (ret == -1) {
        perror("epoll_ctl");
    }
}

int EventLoop::createEventFd() {
    int fd = ::eventfd(0, 0);
    if (fd == -1) {
        perror("eventfd");
    }
    return fd;
}

void EventLoop::handleRead() {
    uint64_t count;
    int ret = ::read(_eventfd, &count, sizeof(count));
    if (ret == -1) {
        perror("read");
    }
}

void EventLoop::wakeup() {
    uint64_t one = 1;
    int ret = ::write(_eventfd, &one, sizeof(one));
    if (ret != sizeof(one)) {
        perror("write");
    }
}

void EventLoop::doPendingFunctors() {
    vector<Functor> temp;
    {
        MutexGuard autolock(_mutex);
        _pendingFunctors.swap(temp);
    }

    for (auto& functor : temp) {
        functor();
    }
}

}  // namespace wd
