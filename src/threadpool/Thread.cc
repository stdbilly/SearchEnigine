#include "threadpool/Thread.h"
#include <iostream>
#include <string>
using std::string;

namespace wd {
namespace current_thread {
__thread int threadID;
}

void Thread::start() {
    pthread_create(&_pthid, nullptr, threadFunc, this);
    _isRunning = true;
}

void* Thread::threadFunc(void* arg) {
    Thread* pThread = static_cast<Thread*>(arg);
    current_thread::threadID = pThread->_id; 
    if (pThread) {
        pThread->_cb();  //执行任务
    }
    
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
