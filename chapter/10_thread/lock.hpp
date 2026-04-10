#pragma once 
#include <pthread.h>

class Mutex
{
private:
  pthread_mutex_t* _lock_p;
public:
  Mutex(pthread_mutex_t* lock_p = nullptr)
    :_lock_p(lock_p)
  {}
  
  void lock()
  {
    if(_lock_p)
    {
      pthread_mutex_lock(_lock_p);
    }
  }

  void unlock()
  {
    pthread_mutex_unlock(_lock_p);
  }
};

class LockGuard
{
private:
  Mutex _mutex;

public:
  LockGuard(pthread_mutex_t* mutex)
    : _mutex(mutex)
  {
    _mutex.lock();
  }

  ~LockGuard()
  {
    _mutex.unlock();
  }
};
