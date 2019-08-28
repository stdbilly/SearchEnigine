#include "net/SocketIO.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

namespace wd {
SocketIO::SocketIO(int fd) : _fd(fd) {}

int SocketIO::readn(char* buf, int len) {
    int left = len;
    char* p = buf;
    while (left > 0) {
        int ret = ::read(_fd, p, left);
        if (ret == -1 && errno == EINTR) {
            continue;
        } else if (ret == -1) {
            perror("read");
            return len - left;
        } else if (ret == 0) {
            return len - left;
        } else {
            left -= ret;
            p += ret;
        }
    }
    return len - left;
}

//读取一行数据
int SocketIO::readLine(char* buf, int maxLen) {
    int left = maxLen - 1;
    char* p = buf;
    int ret;
    int total = 0;
    while (left > 0) {
        ret = recvPeek(p, left);
        //查找'\n'
        for (int idx = 0; idx != ret; ++idx) {
            if (p[idx] == '\n') {
                int sz = idx + 1;
                readn(p, sz);
                total += sz;
                p += sz;
                *p = '\0';
                return total;
            }
        }
        //如果没有找到'\n'
        readn(p, ret);
        left -= ret;
        p += ret;
        total += ret;
    }
    //最终没有发现'\n'
    *p = '\0';
    return total;
}

int SocketIO::recvPeek(char* buf, int len) {
    int ret;
    do {
        ret = ::recv(_fd, buf, len, MSG_PEEK);
    } while (ret == -1 && errno == EINTR);
    return ret;
}

int SocketIO::writen(const char* buf, int len) {
    int left = len;
    const char* p = buf;
    while (left > 0) {
        int ret = ::write(_fd, p, left);
        if (ret == -1 && errno == EINTR) {
            continue;
        } else if (ret == -1) {
            perror("write");
            return len - left;
        } else {
            left -= ret;
            p += ret;
        }
    }
    return len - left;
}
}  // namespace wd
