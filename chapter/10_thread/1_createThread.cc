#include <pthread.h>
#include <cstring>
#include <iostream>
#include <string>
using namespace std;

// 总结
// 线程创建的本质就是在共享区里面建立一个函数栈帧。
// 执行的函数是 void*(*start_routine)(void*); 记住两个void* 。一般需要static_cast同类型的数据进行强制类型的转换。
// 线程回收需要创建一个 void*的通用指针进行接收返回值，为了能够操作void*的指针，所以 pthread_join(tid, **), join是二级指针的。

void* start_routine(void* arg)
{
  const char* str = static_cast<const char*>(arg);

  cout<< "子线程" << endl; 
  for(int n = 10; n > 0; --n)
  {
    std::cout<< n << ":" << str <<std::endl;
  }
  return const_cast<char*>(str);
}

struct Info
{
  int id;
  string name;
};

void* print_struct(void* arg)
{
  struct Info* info = static_cast<struct Info*>(arg);
  std::cout<< "结构体参数1:" << info->id << "参数2:" << info->name << std::endl;

  return info;
}

int main()
{
#if 1
  const char* p = "我是一个常量字符串";
// 1.创建一个线程需要一个线程的句柄，就是线程id号。
// 2.线程创建的本质就是，让线程去建立一个新的栈帧结构，执行对应的代码
//   函数建立栈帧，所以需要传递一个函数
//   函数的类型 (void*)(*start_routine)(void*) 函数的返回值和参数类型都是void*
  pthread_t tid;

  int n = pthread_create(&tid, nullptr, start_routine, const_cast<char*>(p));
  if(n != 0)
  {
    cerr<< "pthread_create error : " << strerror(n) << std::endl;
    return -1;
  }

// 3.线程等待的时候，有一个传输参数，void*, 为了能够操作void*, 需要取地址, 所以这里的 pthread_join(tid, **retval);
//   线程函数，返回主类型可以不强制类型转换，出来了，需要强制类型转换的。
  void* ret = nullptr;
  n = pthread_join(tid, &ret);
  if(n != 0)
  {
    cerr<< "pthread_join error : " << strerror(n) << std::endl;
    return -1;
  }

  cout<< "主线程" << endl;
  const char* newRet = static_cast<const char*>(ret);
  cout<< newRet << endl;

#else 
  struct Info stu = {1, "lic1"};
  pthread_t tid;
  int n = pthread_create(&tid, nullptr, print_struct, &stu);
  if(n != 0)
  {
    std::cerr<< "pthread_create error : " << strerror(errno) << std::endl;
    return 1;
  }

  void* ret = nullptr;
  n = pthread_join(tid, &ret);
  if(n != 0)
  {
    std::cerr<< "pthread_join error : " << strerror(errno) << std::endl;
    return 1;
  }

  struct Info* info = static_cast<struct Info*>(ret);
  std::cout<< "回收子线程tid " << tid 
           << " name " << info->name <<endl;
#endif
  return 0;
}
