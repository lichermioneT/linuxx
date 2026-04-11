#include <iostream>    
#include <vector>    
#include <pthread.h>    
#include <unistd.h>    
#include <cstdio>    
#include <cstring>    
using namespace std;    

// pthread_join默认是阻塞等待，有时候我们不需要进行等待，
// 需要进行线程的分离，
// pthread_detach(tid) 传入线程的tid就可以进行分类的
// 记住
// 1.建议在主线程里面取消
// 2.分离了就不需要join了的

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


