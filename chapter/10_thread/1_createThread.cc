#include <pthread.h>
#include <iostream>
#include <string>
using namespace std;

void* start_routine(void* arg)
{
  cout<< "子线程" << endl;
  const char* str = static_cast<const char*>(arg);
  cout<< str << endl;

  return arg;
}

struct info 
{
  int id;
  string name;
};

int main()
{

  const char* p = "我是一个常量字符串";
  struct info stu = {1, "lic1"};
// 1.创建一个线程需要一个线程的句柄，就是线程id号。
  pthread_t tid;
// 2.线程创建的本质就是，让线程去建立一个新的栈帧结构，执行对应的代码
//   函数建立栈帧，所以需要传递一个函数
//   函数的类型 (void*)(*start_routine)(void*) 函数的返回值和参数类型都是void*

  // int n = pthread_create(&tid, nullptr, start_routine, (void*)p);
  int n = pthread_create(&tid, nullptr, start_routine, &stu);
  if(n == -1)
  {
    perror("pthread_create");
    return -1;
  }

// 3.线程等待的时候，有一个传输参数，void*, 为了能够操作void*, 需要取地址, 所以这里的 pthread_join(tid, **retval);
//   线程函数，返回主类型可以不强制类型转换，出来了，需要强制类型转换的。
  void* ret = nullptr;
  pthread_join(tid, &ret);

  cout<< "主线程" << endl;
  const char* newRet = static_cast<const char*>(ret);
  cout<< newRet << endl;
  return 0;
}
