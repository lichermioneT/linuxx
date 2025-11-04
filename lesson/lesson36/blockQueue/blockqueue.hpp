#pragma once 
#include <iostream>
#include <queue>
#include <pthread.h>

static const int gmaxcap = 5;

template<class T>
class blockqueue
{
public:
  blockqueue(const int& maxcap = gmaxcap):_maxcap(maxcap)
  {
    pthread_mutex_init(&_mutex, nullptr);
    pthread_cond_init(&_pcond, nullptr);
    pthread_cond_init(&_ccond, nullptr);
  }

  void push(const T& in) // 输入型参数 const &
  {
    pthread_mutex_lock(&_mutex);
    //细节2
    //充当条件判断必须是while
    //if要换成whlie----》
    while(is_full())              // 满了
    {
      pthread_cond_wait(&_pcond, &_mutex); // 条件不足，无法生产，继续等待
                                           // 细节1
                                           // 该函数调用的时候，会以原子性的方式，将锁释放，并且将自己挂起。 
                                           // 该函数返回的时候，会自动重新获取你传入的锁
    }
    _q.push(in); // 里面绝对有数据了
    pthread_cond_signal(&_ccond); // 唤醒消费者，可以放在解锁之后的
    pthread_mutex_unlock(&_mutex); // 同时开解锁
  }

  void pop(T* out) // 输出型参数*， 输入输出型参数&
  {
    pthread_mutex_lock(&_mutex);
    //1.先判断
    while(is_empty())
    {
      pthread_cond_wait(&_ccond, &_mutex); // 等生产者，产数据
    }
    //2保证有数据的
    *out = _q.front();
    _q.pop(); // 数据拿出出去了
    
    //3这里保证至少空一个位置
    pthread_cond_signal(&_pcond); // 唤醒消费者,可以再解锁之后。
    pthread_mutex_unlock(&_mutex); // 解锁了
  }

  ~blockqueue()
  {
    pthread_mutex_destroy(&_mutex);
    pthread_cond_destroy(&_pcond);
    pthread_cond_destroy(&_ccond);
  }
private:
  bool is_empty()
  {
    return _q.empty();
  }
  bool is_full()
  {
    return  _q.size() == _maxcap;
  }
private:
  std::queue<T> _q;
  int _maxcap;           // 队列的上线
  pthread_mutex_t _mutex;// 锁
  pthread_cond_t _pcond; // 生产者对应的条件变量
  pthread_cond_t _ccond; // 消费者对于的条件变量
};
