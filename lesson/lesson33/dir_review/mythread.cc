#include <iostream>
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

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

#define VERSION  1

int tickets = 10000;
void* getTickets(void* args)
{
  std::string user_name = static_cast<const char*>(args);

#if VERSION == 1
  while(true)
  {
    pthread_mutex_lock(&lock);
    if(tickets > 0)
    {
      // 进来睡觉了，前面的已经修改了ticket了
      usleep(1000); // 1=1000=1000 000
      std::cout<< user_name << "真正抢票 " << tickets <<std::endl;
      tickets--; // 修改数据
    pthread_mutex_unlock(&lock);
    }
    else 
    {
      pthread_mutex_unlock(&lock);
      break;
    }
  }
  return nullptr;

#elif VERSION == 2 
  while(true)
  {
    if(tickets > 0)
    {
      usleep(1234);
      cout<< user_name << "正在抢票 " << tickets << endl;
      --tickets;
    }
    else 
    {
      break;
    }
  }

#else 
  cout<< "我是" << user_name << endl;
#endif
  
  return nullptr;
}

int main()
{
  return 0;
}
