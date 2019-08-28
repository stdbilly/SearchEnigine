#include "net/Socket.h"
#include <errno.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

namespace wd {
Socket::Socket() {
    _fd = socket(AF_INET, SOCK_STREAM, 0);
    if (_fd == -1) {
        perror("socket");
    }
}

Socket::Socket(int fd) : _fd(fd) {}

int Socket::fd() const { return _fd; }

void Socket::shutdownonWrite() { ::shutdown(_fd, SHUT_WR); }

Socket::~Socket() { ::close(_fd); }
}  // namespace wd
