#include <iostream>
#include "mutex.hpp"
#include <vector>
#include <memory>
#include "thread.hpp"
#include <cstring>
#include <string>
#include <pthread.h>
#include <cstdio>
#include <unistd.h>
using namespace std;

// 间接猪跑
// pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER; // 定义锁 全局锁
// 需要多个线程交叉执行，
// 交叉执行的本质，调度器尽可能频繁发生线程调度与切换
// 线程切换：时间片到了，来了优先级跟高的线程。线程等待的时候。
// 线程是在什么时候检查上面的问题呢？内核态--》用户态。线程对调度状态进行检查，如果可以，就直接发生线程切换。

// 全局锁，只需要加锁和解释，不需要初始化和销毁

class threadData
{
public:
  string _threadName;
  pthread_mutex_t*  _mutex_t;

public:
  threadData(const string& threadName, pthread_mutex_t* mutex_t)
    :_threadName(threadName)
    ,_mutex_t(mutex_t)
  {}
};

int tickets = 100000;

void* getTickets(void* args)
{
  threadData* td = static_cast<threadData*>(args);
  while(true)
  {
#if 0
    pthread_mutex_lock(td->_mutex_t);
    if(tickets > 0)
    {
      cout<< td->_threadName << ":" << tickets << endl;
      --tickets;
      pthread_mutex_unlock(td->_mutex_t);
    }
    else 
    {
      pthread_mutex_unlock(td->_mutex_t);
      break;
    }
#else 
      LockGuard lockguard(td->_mutex_t);
      if(tickets > 0)
      {
        cout<< td->_threadName << ":" << tickets << endl;
        --tickets;
      }
      else 
      {
        break;
      }
#endif
  }
  return nullptr;
}


int main()
{
  pthread_mutex_t lock;
  pthread_mutex_init(&lock, nullptr);
  vector<pthread_t>  tids(4);
  for(int i = 0; i < 4; ++i)
  {
    char buffer[64] = {0};
    snprintf(buffer, sizeof buffer, "thread->%d", i + 1);
    
    threadData* td = new  threadData(buffer, &lock);

    int n = pthread_create(&tids[i], nullptr, getTickets, td);
    if(n != 0)
    {
      cerr<< "pthread_create error" << strerror(n) << endl;
      return 1;
    }
  }

  for(auto tid :  tids)
  {
    pthread_join(tid, nullptr);
    cout<< tid << "：线程等待成功" << endl;
  }
  return 0;
}
