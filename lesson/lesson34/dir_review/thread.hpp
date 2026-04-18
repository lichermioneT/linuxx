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
  Thread(func_t func, void* args = nullptr, int number = 0):func_(func), args_(args),_ctx(nullptr)
  {
    char buffer[number];
    snprintf(buffer,sizeof(buffer), "thread-%d", number);
    name_ = buffer;

    _ctx = new context(); 
    _ctx->this_ = this;
    _ctx->args_ = args_;

  }

  void start()
  {
    int n = pthread_create(&tid_, nullptr, start_routine, _ctx); // 
    if(n != 0)
    {
      std::cerr<< "pthread_create erron : " << strerror(errno) << std::endl;
      return;
    }
  }

  void join()
  {
    int n = pthread_join(tid_, nullptr);
    assert(n == 0);
    (void)n;
  }

private:
  // 类内创建线程，执行对应的方法，方法static
  static void* start_routine(void* agrs) // 缺省参数
  {
    context* ctx = static_cast<context*>(agrs);
    void* ret = ctx->this_->run(ctx->args_);

    delete ctx;
    return ret;
    // 静态不能调用成员方法，成员变量。
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
  context* _ctx;
};
