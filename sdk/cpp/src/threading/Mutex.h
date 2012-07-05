#ifndef MUTEX_H
#define MUTEX_H

#include <pthread.h>

class Mutex
{
    public:
        Mutex(void);
        ~Mutex(void);
        void lock(void);
        void unlock(void);

        pthread_mutex_t _mutex;
};

#endif

