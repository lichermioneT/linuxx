#include <iostream>    
#include <memory>
#include <assert.h>
#include <functional>
#include <string>
#include <vector>    
#include <pthread.h>    
#include <unistd.h>    
#include <cstdio>    
#include <cstring>    
#include "lock.hpp"

    
class Thread;
class context
{
public:
  Thread* _this;
  void* _arg;
};

class Thread 
{
private:
  typedef std::function<void*(void*)> func_t;
  static const int num = 1024;
private:
  std::string _name;
  func_t _func;
  void* _arg;
  pthread_t _tid;
  
public:
  Thread(func_t func, void* arg = nullptr, int number = 0)
  {
    char buffer[num];
    snprintf(buffer, sizeof buffer, "Thread->%d", number);

    _name = buffer;
    _func = func;
    _arg = arg;

    context* ctx =  new context();
    ctx->_this = this;
    ctx->_arg = arg;

    int n = pthread_create(&_tid, nullptr, start_routine, ctx);
    assert(n == 0); 
    (void)n;
  }
  
  static void* start_routine(void* arg)
  {
    context* ctx = static_cast<context*>(arg);
    void* ret = ctx->_this->run(ctx->_arg);
    
    delete ctx;

    return  ret;
  }

  void* run(void* arg)
  {
    return _func(arg);
  }

  void join()
  {
    int n = pthread_join(_tid, nullptr);
    assert(n == 0);
    (void)n;
  }
};

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

int tickets = 100000;
void* getTicket(void* arg)
{
  std::string  user_name = static_cast<const char*>(arg);
  while(tickets)
  {
    LockGuard lck(&lock);

    /*
     *sleep(1);
     */
    if(tickets > 0)
    {
      std::cout<< user_name << "正在抢票" << tickets << std::endl;
      tickets--;
    }
    else 
    {
      break;
    }

  }
  return nullptr;
}

int main()    
{ 
    std::unique_ptr<Thread> Thread1(new Thread(getTicket,(void*)"hello lic1", 1));
    std::unique_ptr<Thread> Thread2(new Thread(getTicket,(void*)"hello lic2", 2));
    std::unique_ptr<Thread> Thread3(new Thread(getTicket,(void*)"hello lic3", 3));
    std::unique_ptr<Thread> Thread4(new Thread(getTicket,(void*)"hello lic4", 4));

    Thread1->join();
    Thread2->join();
    Thread3->join();
    Thread4->join();

    return 0;
}


