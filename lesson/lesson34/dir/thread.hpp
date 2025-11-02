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
  typedef std::function<void*(void*)> func_t;
public: 
  Thread(func_t func, void* args = nullptr, int number = 0):func_(func), args_(args)
  {
    char buffer[number];
    snprintf(buffer,sizeof(buffer), "thread-%d", number);
    name_ = buffer;

    context* ctx = new context(); 
    ctx->this_ = this;
    ctx->args_ = args_;
    int n = pthread_create(&tid_, nullptr, start_routine, ctx); //TODO
    assert(n == 0); // assert 意料只中， if意料之外
    (void)n;
  }

  void start()
  {
  }

  // 类内创建线程，执行对应的方法，方法static
  static void* start_routine(void* agrs) // 缺省参数
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
  std::string name_;
  func_t func_;
  void* args_;
  pthread_t tid_;
};
