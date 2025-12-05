#include "ringqueue.hpp"
#include <pthread.h>
#include <ctime>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

// 生产者往队列里面放数据
void* productorRontine(void* rq)
{
  RingQueue<int>* ringqueue = static_cast<RingQueue<int>*>(rq); 
  while(true)
  {
    int data = rand() % 10 + 1;
    ringqueue->push(data);    // 生产数据

    std::cout<<"生产数据完成， 生产的数据是 ："<< data <<std::endl;
    // sleep(1);

  }
}

// 消费者往队里里面去取数据
void* consumerRoutine(void* rq)
{
  RingQueue<int>* ringqueue = static_cast<RingQueue<int>*>(rq); 
  while(true)
  {
    int data;
    ringqueue->pop(&data); // 消费数据

    std::cout<<"消费数据完成， 消费的数据是 ："<< data <<std::endl;
    sleep(1);
  }
}

int main()
{
  srand((unsigned int)time(nullptr)^getpid()^pthread_self()); // 随机数种子
  
  RingQueue<int>* rq = new RingQueue<int>();                  // 队列里面存放数据，统一份资源
  pthread_t c, p;

  pthread_create(&c, nullptr, productorRontine, rq);          //rq就是同一份资源
  pthread_create(&p, nullptr, consumerRoutine,  rq);          //rq就是同一份资源

  pthread_join(c, nullptr);
  pthread_join(p, nullptr);
  return 0;
}