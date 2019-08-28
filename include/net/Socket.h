#pragma once

namespace wd {
class Socket {
   public:
    Socket();
    explicit Socket(int fd);
    ~Socket();

    int fd() const;
    void shutdownonWrite();

   private:
    int _fd;
};

}  // namespace wd
