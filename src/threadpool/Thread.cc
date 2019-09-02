#include "threadpool/Thread.h"
#include "Redis.h"
#include <iostream>
#include <string>
using std::string;

namespace wd {
namespace current_thread {
__thread Redis* predis;
}

void Thread::start() {
    pthread_create(&_pthid, nullptr, threadFunc, this);
    _isRunning = true;
}

void* Thread::threadFunc(void* arg) {
    current_thread::predis = new Redis(); 
    current_thread::predis->connect("127.0.0.1", 6379);

    Thread* pThread = static_cast<Thread*>(arg);
    if (pThread) {
        pThread->_cb();  //执行任务
    }
    delete current_thread::predis;
    return nullptr;
}

void Thread::join() {
    if (_isRunning) {
        pthread_join(_pthid, nullptr);
        _isRunning = false;
    }
}

Thread::~Thread() {
    if (_isRunning) {
        pthread_detach(_pthid);
    }
}

}  // namespace wd
