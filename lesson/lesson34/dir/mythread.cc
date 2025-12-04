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
// pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER; // 定义锁 全局锁
// 需要多个线程交叉执行，
// 交叉执行的本质，调度器尽可能频繁发生线程调度与切换
// 线程切换：时间片到了，来了优先级跟高的线程。线程等待的时候。
// 线程是在什么时候检查上面的问题呢？内核态--》用户态。线程对调度状态进行检查，如果可以，就直接发生线程切换。

// 全局锁，只需要加锁和解释，不需要初始化和销毁

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER; // 定义锁

class threadData
{
public:
  threadData(const std::string& threadname, pthread_mutex_t* mutex_p):threadName_(threadname), mutex_t_(mutex_p)
  {}
  ~threadData(){}
public:
  std::string threadName_;    // 定义一个string对象，线程的名字
  pthread_mutex_t* mutex_t_;  // 放一个锁，
};

int tickets = 10000;

void* getTickets(void* args)
{
  threadData* td = static_cast<threadData*>(args);        // 锁和线程名称
  // threadData* username = static_cast<threadData*>(args);
  while(true)
  {
// 加锁和解锁是多个线程，串行执行的，所以程序变慢了。212321312322------
// 锁只规定互斥访问，没有规定必须让谁，优先执行
// 锁就是真是的让多个执行流进行竞争的结果
    {
      LockGuard lockguard(&lock); // 直接用对象加锁,局部对象加锁 RAII风格的加锁，局部变量{大括号里面的}， 利用class的构造函数和析构函数 加锁和解锁。
      //pthread_mutex_lock(td->mutex_t_); // 加锁
      if(tickets > 0)
      {
        usleep(1000); // 1=1000=1000 000
        std::cout<< td->threadName_ << "真正抢票 " << tickets <<std::endl;
        // std::cout<< username << "真正抢票 " << tickets<<std::endl;
        tickets--; // 修改数据
      // pthread_mutex_unlock(td->mutex_t_);
      }
      else 
      {
        //pthread_mutex_unlock(td->mutex_t_);
        break;
      }
    }
    // 抢完了票，就完了吗？
    usleep(1000); // 形成一个订单
  }
  return nullptr;
}

int main()
{
#define NUM 4
  pthread_mutex_t lock;                   // 定义一个锁，数据类型的 
  pthread_mutex_init(&lock, nullptr);     // 初始化一个锁
  std::vector<pthread_t> tids(4);         // 定义一个数组，用来存放线程 id值

  for(int i = 0; i < NUM; i++)
  {
    char buffer[64];
    snprintf(buffer, sizeof(buffer), "thread %d", i + 1);
    threadData* td = new threadData(buffer, &lock);      // 创建一个对象，把名字和锁给它
    pthread_create(&tids[i], nullptr, getTickets, td);   // 创建一个线程，对象的数据给它(函数)
  }

  for(const auto &tid : tids)
  {
    pthread_join(tid, nullptr); // 等待线程，第二个参数是输出型参数，二级指针
  }

/*
  pthread_t t1, t2, t3, t4;
  pthread_create(&t1, nullptr, getTickets, (void*)"thread 1");
  pthread_create(&t2, nullptr, getTickets, (void*)"thread 2");
  pthread_create(&t3, nullptr, getTickets, (void*)"thread 3");
  pthread_create(&t4, nullptr, getTickets, (void*)"thread 4");

  pthread_join(t1, nullptr);
  pthread_join(t2, nullptr);
  pthread_join(t3, nullptr);
  pthread_join(t4, nullptr);
*/ 
  pthread_mutex_destroy(&lock); //  删除锁 之间使用锁

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
  
/*
  std::unique_ptr<Thread> thread1(new Thread(getTickets, (void*)"hello lic1", 1));
  std::unique_ptr<Thread> thread2(new Thread(getTickets, (void*)"hello lic2", 2));
  std::unique_ptr<Thread> thread3(new Thread(getTickets, (void*)"hello lic3", 3));
  std::unique_ptr<Thread> thread4(new Thread(getTickets, (void*)"hello lic4", 4));


  thread1->join();
  thread2->join();
  thread3->join();
  thread4->join();
*/
  return 0;
}
