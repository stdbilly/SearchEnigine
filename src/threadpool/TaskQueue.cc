#include "threadpool/TaskQueue.h"

namespace wd {
TaskQueue::TaskQueue(size_t queSize)
    : _queSize(queSize),
      _mutex(),
      _notFull(_mutex),
      _notEmpty(_mutex),
      _flag(true) {}

bool TaskQueue::empty() const { return _que.size() == 0; }

bool TaskQueue::full() const { return _que.size() == _queSize; }

void TaskQueue::push(ElemType elem) {
    MutexGuard autolock(_mutex);
    while (full()) {  //使用while是为了防止虚假（异常）唤醒
        _notFull.wait();
    }
    /*如果使用if，如果有N线程等待在_notfull条件变量上，执行_notFull.notify()会唤醒多个线程，
     * 唤醒的过程中会加锁，有一个线程会拿到锁，继续往下执行，其他N-1个线程在等待加锁，拿到锁的线程执行结束后，
     * 锁被释放，此时full()还是true，因为push()了任务又被pop()了，其他N-1个线程会有一个线程拿到锁，加锁后继续往下执行，
     * 根本不会判断full()是否为true,而此时队列是满的，所以要使用while
     */
    _que.push(elem);
    _notEmpty.notify();
}

ElemType TaskQueue::pop() {
    MutexGuard autolock(_mutex);
    while (_flag && empty()) {
        _notEmpty.wait();
    }
    if (_flag) {
        ElemType elem = _que.front();
        _que.pop();
        _notFull.notify();
        return elem;
    } else {
        return nullptr;
    }
}

void TaskQueue::wakeup() {
    _flag = false;
    _notEmpty.notifyall();
}
}  // namespace wd
