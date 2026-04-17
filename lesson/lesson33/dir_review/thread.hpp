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
  Thread* _this;
  void* _arg;
public:
  context()
    :_this(nullptr)
    ,_arg(nullptr)
  {}
};

class Thread
{
private:
  using func_t = std::function<void*(void*)>; // 类型别名的
  const int num = 1024;

private:
  std::string _name;
  func_t _func;
  void* _arg;
  pthread_t _tid;
  context* _ctx;
public:
  Thread(func_t func, void* arg = nullptr, int number = 0)
    :_func(func)
    ,_arg(arg)
  {
    char buffer[num];
    snprintf(buffer, sizeof(buffer), "Thread%d\n", number);
    _name = buffer;

    _ctx->_this = this;
    _ctx->_arg = _arg;
  } 

  ~Thread()
  {
  }

public:
  void start()
  {
    int n = pthread_create(&_tid, nullptr, start_routine, _arg);
    if(n != 0)
    {
      std::cerr << "pthread_create error : " << strerror(n) << std::endl;
      return;
    }
  }

  void join()
  {
    int n = pthread_join(_tid,  nullptr);
    if(n != 0)
    {
      std::cerr << "pthread_join error : " << strerror(n) << std::endl;
      return;
    }
  }

private:
  static void* start_routine(void* arg)
  {
    context* ctx = static_cast<context*>(arg);
    void* ret = ctx->_this->run(ctx->_arg);
    return ret;
  }

  void* run(void* arg)
  {
    return _func(arg);
  }
};
