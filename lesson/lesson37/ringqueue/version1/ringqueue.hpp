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
  void p(sem_t& sem)
  {
    int i = sem_wait(&sem); // 等待信号量， 信号量会减一
    assert(0 == i);
    (void)i;
  }

  void v(sem_t& sem)
  {
     int i = sem_post(&sem); // ++
     assert(0 == i);
     (void)i;
  }

public:
  RingQueue(const int& cap = gcap):_queue(cap), _cap(cap)
  {
    int n = sem_init(&_spaceSem, 0, _cap);
    assert(0 == n);
    n = sem_init(&_dataSem, 0, 0);
    assert(0 == n);

    _productorStep = _consumerStep = 0;
  }

// 生产者 
  void push(const T& in)
  {
    // 判断能够生产, 判断空间信号量
    p(_spaceSem);
    _queue[_productorStep++] = in; 

    _productorStep %= _cap; 
    v(_dataSem);
  }

  void pop(T* out)
  {
    p(_dataSem);
    *out = _queue[_consumerStep++];

    _consumerStep %= _cap;
    v(_spaceSem);
  }

  ~RingQueue()
  {
    sem_destroy(&_spaceSem);
    sem_destroy(&_dataSem);
  }
private:
  std::vector<T> _queue;
  int _cap;
  sem_t _spaceSem; // 生产者 想生产，空间资源
  sem_t _dataSem;  // 消费者 想消费，数据资源
  int _productorStep;
  int _consumerStep;
};
