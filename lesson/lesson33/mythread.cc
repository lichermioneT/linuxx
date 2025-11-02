#include <iostream>
#include <cstring>
#include <string>
#include <pthread.h>
#include <cstdio>
#include <unistd.h>
using namespace std;

// 添加__thread,可以将一个内置类型设置为线程局部存储
__thread int g_val = 128;

// 共享资源
int shared = 44;
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



int main()
{

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







  return 0;
}
