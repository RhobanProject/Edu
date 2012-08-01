#include <pthread.h>
#include <string>

#include "Mutex.h"

using namespace std;

Mutex::Mutex(void)
{
  pthread_mutex_init(&_mutex, NULL);
}

Mutex::~Mutex(void)
{
  pthread_mutex_destroy(&_mutex);
}

void Mutex::lock(void)
{
  pthread_mutex_lock(&_mutex);
}

void Mutex::unlock(void)
{
  pthread_mutex_unlock(&_mutex);
}
