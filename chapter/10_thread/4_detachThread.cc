#include <iostream>    
#include <vector>    
#include <pthread.h>    
#include <unistd.h>    
#include <cstdio>    
#include <cstring>    
    
using namespace std;    

// 线程执行函数    
void* start_routine(void* arg)    
{    
    (void)arg;
    cout<< "子线程" <<endl;
    
    return nullptr;
} 

int main()    
{    
  pthread_t tid;
  int n = pthread_create(&tid, nullptr, start_routine, nullptr);
  if(n != 0)
  {
    cerr<< "pthread_create" << strerror(errno) << endl;
  }

// 建议分类的话，马上在主线程这里分离的
// 就不需要join了
  n = pthread_detach(tid);
  if(n != 0)
  {
    cerr<< "pthread_detach" << strerror(errno) << endl;
    return 1;
  }

  cout<< "ddd" << endl;
  return 0;
}


