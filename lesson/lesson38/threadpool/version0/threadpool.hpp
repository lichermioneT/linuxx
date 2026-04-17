#pragma once 
#include <unistd.h>
#include <vector>
#include <queue>
#include <pthread.h>
#include "thread.hpp"
#include "lockGuard.hpp"
#include "tast.hpp"


const int gnum = 10;
// 模板对象的声明
template<class T> 
class threadpool;

template<class T>
class ThreadData
{
public:
  threadpool<T>* threadpooL; // 线程池指针
  std::string name;          // 名称
public:
  ThreadData(threadpool<T>* tp, const std::string& name_)
    :threadpooL(tp)
    ,name(name_)
  {}
};

using namespace ThreadNs;
template<class T>
class threadpool
{
private:
  int _num;
  std::vector<Thread*> _threads;
  std::queue<T> _task_queue;
  pthread_mutex_t _mutex;
  pthread_cond_t _cond;

public:
  threadpool(const int& num = gnum)
    :_num(num)
  {
      pthread_mutex_init(&_mutex, nullptr);
      pthread_cond_init(&_cond, nullptr);
  
      for(int i = 0; i < _num; i++)
      {
        _threads.push_back(new Thread());
      }
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

  void run()
  {
    for(const auto& t: _threads)
    {
      ThreadData<T>* td = new ThreadData<T>(this, t->threadname());
      t->start(handerTask, td);
      std::cout<< t->threadname() << "start...." <<std::endl;
    }
  }

private:
  static void* handerTask(void* args)
  {
#if 1
    ThreadData<T>* td = static_cast<ThreadData<T>>(args);

    while(true)
    {
      td->threadpooL->unlockQueue(); 

      while(td->threadpooL->isQueueEmpty())
      {
        td->threadpooL->threadWait();
      }

      T t = td->threadpooL->pop();
      td->threadpooL->unlockQueue();
      std::cout<< td->name << "获取一个任务：" << t.toTaskString() << "处理结果" << t() << std::endl;

    }

    delete td;
#else 

#endif
    return nullptr;
  }

private:
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
 
  void push(const T& in)
  {
#if 1
    pthread_mutex_lock(&_mutex);
    _task_queue.push(in);
    pthread_cond_signal(&_cond);
    pthread_mutex_unlock(&_mutex);
#else 
    LockGuard lockguard(&_mutex);
    _task_queue.push(in);
    pthread_cond_signal(&_cond);
#endif
  }
};





