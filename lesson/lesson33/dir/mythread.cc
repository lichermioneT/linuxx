#include <iostream>
#include <memory>
#include "Thread.hpp"
#include <cstring>
#include <string>
#include <pthread.h>
#include <cstdio>
#include <unistd.h>
using namespace std;


// 添加__thread,可以将一个内置类型设置为线程局部存储
string changeId(const pthread_t& thread_id)
{
  char tid[128];
  snprintf(tid, sizeof(tid), "0x%zx", thread_id);
  return tid;
}

void* start_routine(void* args)
{
  string threadname = static_cast<const char*>(args);
  // pthread_detach(pthread_self()); // 设置自己为分离状态
  while(true)
  {
    char tid[28];
    snprintf(tid, sizeof(tid), "0x%zx", pthread_self());
    cout<< threadname << "running..." << changeId(pthread_self()) <<endl;
    sleep(1);
  }
  return nullptr;
}

// 间接猪跑
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER; // 定义锁
// 需要多个线程交叉执行，
// 交叉执行的本质，调度器尽可能频繁发生线程调度与切换
// 线程切换：时间片到了，来了优先级跟高的线程。线程等待的时候。
// 线程是在什么时候检查上面的问题呢？内核态--》用户态。线程对调度状态进行检查，如果可以，就直接发生线程切换。
int tickets = 10000;

void* getTickets(void* args)
{
  std::string user_name = static_cast<const char*>(args);
  while(true)
  {
    pthread_mutex_lock(&lock); // 加锁
    if(tickets > 0)
    {
      // 进来睡觉了，前面的已经修改了ticket了
      usleep(1000); // 1=1000=1000 000
      std::cout<< user_name << "真正抢票 " << tickets<<std::endl;
      tickets--; // 修改数据
      pthread_mutex_unlock(&lock); // 解锁
    }
    else 
    {
      pthread_mutex_unlock(&lock); // 解锁
      break;
    }
  
  }
  return nullptr;
}





int main()
{

/*
  pthread_t tid;
  pthread_create(&tid, nullptr, start_routine, (void*)"thread 1");
  string main_id = changeId(pthread_self());
  pthread_detach(tid);

  while(true)
  {
    cout<< "main thread run ...." << "new thread id : "  << changeId(tid) << " main_id : " << main_id <<endl;
    sleep(1);
  }
  sleep(2);

// join和detach不能共存
  int n = pthread_join(tid, nullptr);
  cout<< "result " << n << " n " << strerror(n) <<endl;

*/ 

  

  std::unique_ptr<Thread> thread1(new Thread(getTickets, (void*)"hello lic1", 1));
  std::unique_ptr<Thread> thread2(new Thread(getTickets, (void*)"hello lic2", 2));
  std::unique_ptr<Thread> thread3(new Thread(getTickets, (void*)"hello lic3", 3));
  std::unique_ptr<Thread> thread4(new Thread(getTickets, (void*)"hello lic4", 4));


  thread1->join();
  thread2->join();
  thread3->join();
  thread4->join();










  return 0;
}
