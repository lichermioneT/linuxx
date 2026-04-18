#include <iostream>
#include <memory>
#include <memory>
#include "thread.hpp"
#include <cstring>
#include <string>
#include <pthread.h>
#include <cstdio>
#include <unistd.h>
using namespace std;

// 间接猪跑
// pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER; // 定义锁
// 需要多个线程交叉执行，
// 交叉执行的本质，调度器尽可能频繁发生线程调度与切换
// 线程切换：时间片到了，来了优先级跟高的线程。线程等待的时候。
// 线程是在什么时候检查上面的问题呢？内核态--》用户态。线程对调度状态进行检查，如果可以，就直接发生线程切换。

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; // 初始化全局的锁
int tickets = 100000;

void* getTickets(void* args)
{
  std::string user_name =  static_cast<const char*>(args);
#if 0 
  while(true)
  {
    if(tickets >  0)
    {
      std::cout << user_name << tickets << std::endl;
      tickets--;
    }
    else 
    {
      break;
    }
  }
  return nullptr;
#else 
    
  while(true)
  {
    pthread_mutex_lock(&mutex);
    if(tickets > 0)
    {
      std::cout << user_name << ":" << tickets << std::endl;
      tickets--;
      pthread_mutex_unlock(&mutex);
    }
    else 
    {
      pthread_mutex_unlock(&mutex);
      break;
    }
  }
  return nullptr;
#endif 
}

int main()
{
  std::unique_ptr<Thread>  thread1(new Thread(getTickets, (void*)"lic 1", 1));
  std::unique_ptr<Thread>  thread2(new Thread(getTickets, (void*)"lic 2", 2));
  std::unique_ptr<Thread>  thread3(new Thread(getTickets, (void*)"lic 3", 3));
  std::unique_ptr<Thread>  thread4(new Thread(getTickets, (void*)"lic 4", 4));

  thread1->start();
  thread2->start();
  thread3->start();
  thread4->start();

  thread1->join();
  thread2->join();
  thread3->join();
  thread4->join();

  return 0;
}
