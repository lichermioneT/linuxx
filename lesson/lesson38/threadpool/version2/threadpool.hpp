#pragma once 

#include <mutex>
#include <unistd.h>
#include <vector>
#include <queue>
#include <pthread.h>
#include "thread.hpp"
#include "lockGuard.hpp"


const int gnum = 3;
template<class T> class threadpool;

template<class T>
class ThreadData
{
public:
  threadpool<T> *threadpooL;
  std::string name;
public:
  ThreadData(threadpool<T>* tp, const std::string& n):threadpooL(tp),name(n){}
};

using namespace ThreasNs;
template<class T>
class threadpool
{
private:
  static void* handerTask(void* args)
  {
    ThreadData<T>* td = (ThreadData<T>*)args;
    // threadpool<T>* threadpooL = static_cast<threadpool<T>*>(args);
    while(true)
    {
      T t;
      {
        // td->threadpooL->lockQueue();
        LockGuard lockguard(td->threadpooL->mutex()); // RAII风格加锁
        while(td->threadpooL->isQueueEmpty())
        {
          td->threadpooL->threadWait();
        }
      
        t = td->threadpooL->pop();
        //td->threadpooL->unlockQueue();
      }
      std::cout<< td->name << " 获取任务 "  << t.totaskstring()<< " 处理完成 结果是" << t() <<std::endl;
    }
    delete td;
    return nullptr;
  }

  threadpool(const int& num = gnum):_num(num)
  {
      pthread_mutex_init(&_mutex, nullptr);
      pthread_cond_init(&_cond, nullptr);
  
      for(int i = 0; i < _num; i++)
      {
        _threads.push_back(new Thread());
      }
  }

  void operator=(const threadpool&) = delete;
  threadpool(const threadpool&) = delete;

public:
  void lockQueue()  { pthread_mutex_lock(&_mutex);}
  void unlockQueue() { pthread_mutex_unlock(&_mutex);}
  bool isQueueEmpty() { return _task_queue.empty();}
  void threadWait() { pthread_cond_wait(&_cond, &_mutex);}
  T pop()
  {
      T t = _task_queue.front();
      _task_queue.pop();
      return t;
  }
  
  pthread_mutex_t* mutex()
  {
    return &_mutex;
  }

public:
 
  void run()
  {
    for(const auto& t: _threads)
    {
      ThreadData<T>* td = new ThreadData<T>(this, t->threadname());
      t->start(handerTask, td);
      std::cout<< t->threadname() << "start...." <<std::endl;
    }
  }

  void push(const T& in)
  {
      //pthread_mutex_lock(&_mutex);
      LockGuard lockguard(&_mutex);
      _task_queue.push(in);
      pthread_cond_signal(&_cond);
      // pthread_mutex_unlock(&_mutex);
  }

  ~threadpool()
  {
    pthread_mutex_destroy(&_mutex);
    pthread_cond_destroy(&_cond);
    for(const auto& t : _threads)
    {
      delete t;
    }
  }

// 
  static threadpool<T>* getInstance()
  {
    if(nullptr == tp)
    {
       _singl_lock.lock(); 
       if(tp == nullptr)
       {
        tp = new threadpool<T>();
       }
       _singl_lock.unlock();
    }
    return tp;
  }

private:
  int _num;
  std::vector<Thread*> _threads;
  std::queue<T> _task_queue;
  pthread_mutex_t _mutex;
  pthread_cond_t _cond;

  static threadpool<T>* tp;
  static std::mutex _singl_lock;
};

template<class T>
threadpool<T>* threadpool<T>::tp = nullptr;

template<class T>
std::mutex threadpool<T>::_singl_lock;


