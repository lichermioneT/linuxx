#include <iostream>
#include <cstring>
#include <string>
#include <pthread.h>
#include <cstdio>
#include <unistd.h>
using namespace std;


string changeId(const pthread_t& thread_id)
{
  char tid[128];
  snprintf(tid, sizeof(tid), "0x%zx", thread_id);
  return tid;
}

void* start_routine(void* args)
{
  string threadname = static_cast<const char*>(args);
  pthread_detach(pthread_self()); // 设置自己为分离状态
  int cnt = 5;
  while(cnt)
  {
    char tid[28];
    snprintf(tid, sizeof(tid), "0x%zx", pthread_self());
    cout<< threadname << "running..." << changeId(pthread_self()) <<endl;
    cout<< cnt <<endl;
    sleep(1);
    cnt--;
  }
  return nullptr;
}

int main()
{

  pthread_t tid;
  pthread_create(&tid, nullptr, start_routine, (void*)"thread 1");
  string main_id = changeId(pthread_self());
  pthread_detach(tid);

  cout<< " main thread run ...." << "new thread id : "  << changeId(tid) << " main_id : " << main_id <<endl;

  while(true)
  {
    //todo main
  }

  // join和detach不能共存
  // int n = pthread_join(tid, nullptr);
  // cout<< "result " << n << " n " << strerror(n) <<endl;

  return 0;
}
