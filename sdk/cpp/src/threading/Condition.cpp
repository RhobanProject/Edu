#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <time.h>
#include <pthread.h>
#include <errno.h>

#include "Condition.h"

using namespace std;

Condition::Condition()
{
    int ret = pthread_cond_init(&condition, 0 );

    if(ret==-1) {
        throw string("Failed to init condition");
    }
}

Condition::~Condition()
{
    pthread_cond_destroy(&condition);
}

int Condition::wait(Mutex *mutex, unsigned int timeout = 0)
{
    int ret;
    struct timespec time;

    clock_gettime(CLOCK_REALTIME, &time);

    time.tv_sec += timeout/1000;
    time.tv_nsec += (timeout%1000) * 1000000;

    if (time.tv_nsec >= 1000000000L) {
        time.tv_sec++;
        time.tv_nsec -= 1000000000L;
    }

    mutex->lock();

    if (timeout > 0) {
        ret = pthread_cond_timedwait(&condition, &mutex->_mutex, &time);
    } else {
        ret = pthread_cond_wait(&condition, &mutex->_mutex);
    }

    mutex->unlock();

    if (ret == ETIMEDOUT) {
        return 0;
    }

    if (ret != 0) {
        throw string("Failed to wait condition");
    }

    return 1;
}

void Condition::broadcast()
{
    int ret = pthread_cond_broadcast(&condition);

    if(ret==-1) {
        throw string("Failed to broadcast condition");
    }
}

