#include "net/TCPServer.h"

namespace wd {
TCPServer::TCPServer(const string& ip, unsigned short port) 
: _acceptor(ip, port) 
, _loop(_acceptor) {}

void TCPServer::start() {
    _acceptor.ready();
    _loop.loop();
}

void TCPServer::setConnectionCallback(TCPConnectionCallback&& cb) {
    _loop.setConnectionCallback(std::move(cb));
}

void TCPServer::setMessageCallback(TCPConnectionCallback&& cb) {
    _loop.setMessageCallback(std::move(cb));
}

void TCPServer::setCloseCallback(TCPConnectionCallback&& cb) {
    _loop.setCloseCallback(std::move(cb));
}

}  // namespace wd
