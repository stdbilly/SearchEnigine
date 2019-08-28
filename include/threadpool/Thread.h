#pragma once
#include <pthread.h>
#include <functional>
#include <iostream>
#include <string>
#include "Nocopyble.h"
using std::cout;
using std::endl;
using std::string;

namespace wd {
// namespace current_thread {
// extern __thread int threadID;
// }

using ThreadCallback = std::function<void()>;

class Thread : Nocopyble {
   public:
    Thread(ThreadCallback&& cb, const int id)
        : _pthid(0), _isRunning(false), _cb(std::move(cb)), _id(id) {}

    ~Thread();
    void start();
    void join();
    int threadID() const { return _id; }
    static void* threadFunc(void*);

   private:
    pthread_t _pthid;
    bool _isRunning;
    ThreadCallback _cb;
    int _id;
};
}  // namespace wd
