#pragma once
#include <iostream>
#include <queue>
#include <pthread.h>
static const int gmaxcap = 10;

template<class T>
class blockqueue
{
private:
  std::queue<T> _q; // 定义一个队列，先进先出的
  int _maxcap;      // 最大一个容量的
  pthread_mutex_t _mutex; // 注意这里定义一个锁，是为了同时只有一个能够在队列写入或者拿走
  pthread_cond_t _ccond;  // c消费者的条件变量
  pthread_cond_t _pcond;  // p生产者的条件变量

public:
  blockqueue(int maxcap = gmaxcap)
    :_maxcap(maxcap)
  {
    pthread_mutex_init(&_mutex, nullptr); // 锁的初始化
    pthread_cond_init(&_ccond, nullptr);  // 消费者的条件变量初始化
    pthread_cond_init(&_pcond, nullptr);  // 生产者的条件变量初始化
  }

  ~blockqueue()
  {
    pthread_mutex_destroy(&_mutex); // 锁的销毁
    pthread_cond_destroy(&_ccond);  // 消费者的锁
    pthread_cond_destroy(&_pcond);  // 生产者的锁
  }

public:
  void push(const T& in)
  {
    pthread_mutex_lock(&_mutex); // 抢到锁才能够下去
    while(is_full())
    {
      pthread_cond_wait(&_pcond, &_mutex); // 抢到锁，没有被唤醒，放入等待队列里面去的。被唤醒的时候，抢着锁的 执行下面的代码。
    }

    _q.push(in); // 放数据到对里面的

    pthread_cond_signal(&_ccond); // 生成了数据，唤醒消费者的
    pthread_mutex_unlock(&_mutex); // 释放锁。 一次完整的，生产者放数据的流程。 锁返还给消费者了
  }

  void pop(T* out)
  {
    pthread_mutex_lock(&_mutex); // 抢到锁才能够下去的
    while(is_empty())
    {
      pthread_cond_wait(&_ccond, &_mutex); // 抢到锁，没有被唤醒，放入等待队列里面去的。被唤醒的时候，抢着锁的，执行下面的代码
    }

    *out = _q.front(); // 取数据
    _q.pop();
    
    pthread_cond_signal(&_pcond); // 已经有空间了，唤醒生产者。
    pthread_mutex_unlock(&_mutex); // 释放锁，生产者开始竞争锁，放数据的。
  }

private:
  bool is_full()
  {
    return _q.size() == _maxcap;
  }

  bool is_empty()
  {
    return _q.empty();    
  }
};
