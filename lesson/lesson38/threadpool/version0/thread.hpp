#pragma once 
#include <string>
#include <iostream>
#include <pthread.h>
#include <functional>
#include <cassert>
#include <cstring>



namespace ThreadNs
{
typedef std::function<void*(void*)> func_t;
const int num = 1024;

class Thread
{
private:
  // 类内创建线程，执行对应的方法，方法static
  static void* start_routine(void* agrs) // 缺省参数
  {
    Thread* _this = static_cast<Thread*>(agrs);
    return _this->callback();
    // 静态不能调用成员方法，成员变量。
  }

public: 
  Thread(func_t func, void* args = nullptr):func_(func), args_(args)
  {
    char namebuffer[num];
    snprintf(namebuffer,sizeof(namebuffer), "thread-%d", threadnum++);
    name_ = namebuffer;
  }

  void start()
  {
    int n = pthread_create(&tid_, nullptr, start_routine, this);  // 
    assert(n == 0);                                               // assert 意料只中， if意料之外
    (void)n;
  }

  void join()
  {
    int n = pthread_join(tid_, nullptr);
    assert(n == 0);
    (void)n;
  }

  void* callback() { return func_(args_); }

  std::string threadname()
  {
    return name_;
  }

  ~Thread()
  {
    // do nothing
  }

private:
  std::string name_;
  func_t func_;
  void* args_;
  pthread_t tid_;
  static int threadnum;
};
  int Thread::threadnum = 1;
}
