#include <iostream>
#include <cstring>
#include <string>
#include <pthread.h>
#include <cstdio>
#include <unistd.h>
using namespace std;

string changeId(const pthread_t& pthread_id)
{
  char tid[128];
  snprintf(tid, sizeof(tid), "0x%zx", pthread_id);

  return tid;
}

void* start_routine(void* arg)
{
  string threadName = static_cast<const char*>(arg);
  int cnt = 5;
  while(cnt)
  {
    char tid[128];
    snprintf(tid, sizeof tid, "0x%zx", pthread_self());
    cout<< threadName << "running.." << changeId(pthread_self());
    cout<< cnt <<endl;
    --cnt;
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





  return 0;
}
