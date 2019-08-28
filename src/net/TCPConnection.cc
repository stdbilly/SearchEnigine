#include "net/TCPConnection.h"
#include "net/EventLoop.h"
#include <errno.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sstream>

namespace wd {
TCPConnection::TCPConnection(int fd, EventLoop* loop)
    : _sock(fd),
      _socketIO(fd),
      _localAddr(getLocalAddr()),
      _peerAddr(getPeerAddr()),
      _isShutdownonWrite(false),
      _loop(loop) {}

TCPConnection::~TCPConnection() {
    if (!_isShutdownonWrite) {
        shutdown();
    }
}

string TCPConnection::receive() {
    char buf[65536] = {0};
    _socketIO.readLine(buf, sizeof(buf));
    return string(buf);
}

void TCPConnection::send(const string& msg) {
    _socketIO.writen(msg.c_str(), msg.size());
}

void TCPConnection::sendInLoop(const string& msg) {
    if(_loop) {
        _loop->runInloop(std::bind(&TCPConnection::send, this, msg));
    }
}

void TCPConnection::shutdown() {
    if (!_isShutdownonWrite) {
        _isShutdownonWrite = true;
        _sock.shutdownonWrite();
    }
}

string TCPConnection::toString() const {
    std::ostringstream oss;
    oss << _localAddr.ip() << ":" << _localAddr.port() << " --> "
        << _peerAddr.ip() << ":" << _peerAddr.port();
    return oss.str();
}

InetAddress TCPConnection::getLocalAddr() {
    struct sockaddr_in addr;
    socklen_t len = sizeof(struct sockaddr);
    if (getsockname(_sock.fd(), (struct sockaddr*)&addr, &len) == -1) {
        perror("getsockname");
    }
    return InetAddress(addr);
}

InetAddress TCPConnection::getPeerAddr() {
    struct sockaddr_in addr;
    socklen_t len = sizeof(struct sockaddr);
    if (getsockname(_sock.fd(), (struct sockaddr*)&addr, &len) == -1) {
        perror("getsockname");
    }
    return InetAddress(addr);
}

void TCPConnection::setConnectionCallback(const TCPConnectionCallback& cb) {
    _onConnection = cb;
}

void TCPConnection::setMessageCallback(const TCPConnectionCallback& cb) {
    _onMessage = cb;
}

void TCPConnection::setCloseCallback(const TCPConnectionCallback& cb) {
    _onClose = cb;
}

void TCPConnection::handleConnectionCallback() {
    if(_onConnection) {
        _onConnection(shared_from_this());
    }
}

void TCPConnection::handleMessageCallback() {
    if(_onMessage) {
        _onMessage(shared_from_this());
    }
}

void TCPConnection::handleCloseCallback() {
    if(_onClose) {
        _onClose(shared_from_this());
    }
}

}  // namespace wd
