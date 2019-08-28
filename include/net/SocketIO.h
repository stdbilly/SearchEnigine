#pragma once

namespace wd {
class SocketIO {
   public:
    explicit SocketIO(int fd);

    int readn(char* buf, int len);
    int readLine(char* buf, int maxLen);
    int writen(const char* buf, int len);

   private:
    int recvPeek(char* buf, int len);

   private:
    int _fd;
};

}  // namespace wd
