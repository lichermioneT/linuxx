#pragma once 
#include <string>
#include <iostream>
#include <pthread.h>
#include <functional>
#include <cassert>
#include <cstring>

class Thread;

class context
{
public:
  Thread* this_;
  void* args_;
public:
  context():this_(nullptr), args_(nullptr){}
  ~context(){}
};

class Thread
{
public:
  typedef std::function<void*(void*)> func_t;  // function pointer
  const int num = 1024;
public: 
  Thread(func_t func, void* args = nullptr, int number = 0):func_(func), args_(args)
  {
    char buffer[num];
    snprintf(buffer, sizeof(buffer), "thread-%d", number);         // thread-1,2,,n;
    name_ = buffer;                                                // thread name

    context* ctx = new context();     // store class address and agrs
    ctx->this_ = this;
    ctx->args_ = args_;

    int n = pthread_create(&tid_, nullptr, start_routine, ctx);     // TODO
    assert(n == 0);                                                 // assert 意料只中， if意料之外
    (void)n;
  }

  // 类内创建线程，执行对应的方法，方法static this pointer
  static void* start_routine(void* agrs)   // 缺省参数
  {
    context* ctx = static_cast<context*>(agrs);
    
    void* ret = ctx->this_->run(ctx->args_);
    delete ctx;
    return ret;
    // 静态不能调用成员方法，成员变量。
  }

  void join()
  {
    int n = pthread_join(tid_, nullptr);
    assert(n == 0);
    (void)n;
  }

  void* run(void* args)
  {
    return func_(args);
  }

  ~Thread()
  {
    // do nothing
  }

private:
  std::string name_;  // thread name
  func_t func_;       // thread function
  void* args_;        // thread args
  pthread_t tid_;     // thread tid
};
