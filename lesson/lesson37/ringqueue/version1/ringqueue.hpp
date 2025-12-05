#pragma once 
#include <iostream>
#include <vector>
#include <semaphore.h>
#include <cassert>

static const int gcap = 5;

template<class T>
class RingQueue
{
private: 
// p操作
  void p(sem_t& sem)
  {
    int i = sem_wait(&sem); // 等待信号量， 信号量会减一
    assert(0 == i);
    (void)i;
  }

// v操作
  void v(sem_t& sem)
  {
     int i = sem_post(&sem); // 释放信号，信号量会加一
     assert(0 == i);
     (void)i;
  }

public:
  RingQueue(const int& cap = gcap):_queue(cap), _cap(cap)
  {
    int n = sem_init(&_spaceSem, 0, _cap);   // 信号量的大小
    assert(0 == n);

    n = sem_init(&_dataSem, 0, 0);           // 信号量的大小
    assert(0 == n);

    _productorStep = _consumerStep = 0;      // 初始化生产者，消费者一样的位置
  }

// 生产者 
  void push(const T& in)
  {
    // 判断能够生产, 判断空间信号量
    p(_spaceSem);                       // p--
    _queue[_productorStep++] = in; 

    _productorStep %= _cap;                // 生产者的步伐
    v(_dataSem);                       // v++
  }

// 消费者
  void pop(T* out)
  {
    p(_dataSem);                     // p--
    *out = _queue[_consumerStep++];

    _consumerStep %= _cap;                // 消费者的步伐
    v(_spaceSem);                   // v++
  }

// 删除信号量，
  ~RingQueue()
  {
    sem_destroy(&_spaceSem);
    sem_destroy(&_dataSem);
  }
private:
  std::vector<T> _queue;   // 模拟环形队列，
  int _cap;                // 队列容量
  sem_t _spaceSem;         // 生产者 想生产，空间资源，生产者需要空间
  sem_t _dataSem;          // 消费者 想消费，数据资源，消费者需要数据
  int _productorStep;
  int _consumerStep;
};
