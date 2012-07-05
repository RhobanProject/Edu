#ifndef _CONDITION_H
#define _CONDITION_H

#include "Mutex.h"

class Condition
{
    public:

        Condition();
        ~Condition();

        // wait for the condition to be brodcasted (optional timeout in ms)
        int wait(Mutex *mutex, unsigned int timeout);

        // broadcast a condition
        void broadcast();

    private:

        pthread_cond_t condition;
};

#endif // _CONDITION_H
