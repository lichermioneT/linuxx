#include <pthread.h>
#include <cstring>
#include <cstdlib>
#include <vector>
#include <unistd.h>
#include <string>
#include <iostream>
using namespace std;

// 总结线程 pthread_exit也需要带出来参数信息的 
// pthread_exit(void*)
void* start_routine(void* arg)
{
  const char* lic = static_cast<const char*>(arg);
  cout<< "线程参数:" << lic <<endl;
  
  pthread_exit((void*)lic);
}


int main()
{

  const char* lic = "hello licherminonex";
  pthread_t tid;

  int n = pthread_create(&tid, nullptr, start_routine, (void*)lic);
  if(n != 0)
  {
    cerr<< "pthread_create  error: " << strerror(n) <<endl;
    return 1;
  }

  void* ret = nullptr;
  n = pthread_join(tid, &ret);
  const char* retNew = static_cast<const char*>(ret);
  cout<< "返回参数:" << retNew <<endl;

  return 0;
}

