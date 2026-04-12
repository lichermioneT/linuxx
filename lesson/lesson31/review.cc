#include <pthread.h>
#include <iostream>
#include <cstring>
using namespace std;

// 总结
// 1.创建线程的本质就是：进程内的一个新执行流，执行的代码，
//   这个代码是函数，函数建立新的栈帧在共享区域。tid就是共享区的地址。
// 2.pthread_create的执行的函数 (void*)(*start_routine)(void*);
// 3.pthread_join(tid, void**), 因为返回值是void*, 所以需要二级指针进行回收。
// 两个函数都是成功返回0，失败返回error

void* start_routine(void* arg)
{
  string* info = (string*)arg;
  cout<< "传入参数：" << *info <<endl;

  info->insert(info->begin(), ' ');
  info->insert(info->begin(), ' ');
  info->insert(info->begin(), 'c');
  info->insert(info->begin(), 'i');
  info->insert(info->begin(), 'L');

  return info;
}

int main()
{
  string lic = "lichermionex";
  pthread_t tid;
  int n = pthread_create(&tid, nullptr, start_routine, &lic);
  if(n != 0)
  {
    cerr<< "pthread_create erron : " << strerror(errno) <<endl;
    return 1;
  }
  
  void* ret = nullptr;
  n = pthread_join(tid, &ret);
  if(n != 0)
  {
    cerr<< "pthread_join errno : " << strerror(errno) <<endl;
    return 1;
  }

  string* retNew = (string*)ret;
  cout<< "传出参数：" << *retNew <<endl;
  return 0;
}
