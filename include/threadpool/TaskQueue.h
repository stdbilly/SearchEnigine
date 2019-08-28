#pragma once
#include <queue>
#include "Condition.h"
#include "MutexLock.h"
#include <functional>
using std::queue;

namespace wd {
using ElemType = std::function<void()>;
class TaskQueue {
   public:
    TaskQueue(size_t quesize);

    bool empty() const;
    bool full() const;
    void push(ElemType);
    ElemType pop();
    void wakeup();
   private:
    size_t _queSize;
    queue<ElemType> _que;
    MutexLock _mutex;
    Condition _notFull;
    Condition _notEmpty;
    bool _flag;
};
}  // namespace wd
