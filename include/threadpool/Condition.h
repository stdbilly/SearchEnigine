#pragma once
#include "Nocopyble.h"
#include <pthread.h>

namespace wd {
class MutexLock;

class Condition : Nocopyble {
public:
    Condition(MutexLock& mutex);
    ~Condition();
    void wait();
    void notify();
    void notifyall();
private:
    pthread_cond_t _cond;
    MutexLock& _mutex;    
};
}
