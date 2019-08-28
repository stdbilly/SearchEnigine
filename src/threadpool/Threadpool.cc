#include "threadpool/Threadpool.h"
#include <unistd.h>
#include <iostream>
#include <string>
#include "threadpool/Thread.h"
using std::string;

namespace wd {
Threadpool::Threadpool(size_t threadNum, size_t queSize)
    : _threadNum(threadNum),
      _queSize(queSize),
      _taskque(_queSize),
      _isExit(false) {
    _threads.reserve(_threadNum);
}

Threadpool::~Threadpool() {
    cout << "~Threadpool()" << endl;
    if (!_isExit) {
        stop();
    }
}

void Threadpool::addTask(Task&& task) { _taskque.push(std::move(task)); }

Task Threadpool::getTask() { return _taskque.pop(); }

void Threadpool::start() {
    for (size_t idx = 0; idx != _threadNum; ++idx) {
        string threadname = "workthread " + std::to_string(idx);
        unique_ptr<Thread> thread(
            new Thread(std::bind(&Threadpool::threadFunc, this), idx));
        _threads.push_back(std::move(thread));
    }

    for (auto& thread : _threads) {
        thread->start();
    }
}
//每一个子线程都要执行的任务
void Threadpool::threadFunc() {
    while (!_isExit) {
        Task task = getTask();
        if (task) {
            task();  //执行任务
        }
    }
}

void Threadpool::stop() {
    if (!_isExit) {
        while (!_taskque.empty()) {
            ::usleep(1);
        }
        _isExit = true;
        _taskque.wakeup();
        for (auto& thread : _threads) {
            thread->join();
        }
    }
}
}  // namespace wd
