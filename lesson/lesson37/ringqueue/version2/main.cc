#include "ringqueue.hpp"
#include <pthread.h>
#include <ctime>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include "tast.hpp"

std::string selfname()
{
  char name[128];
  snprintf(name, sizeof(name), "thread[0x%zx]", pthread_self());
  return name;
}

void* productorRontine(void* rq)
{
  RingQueue<task>* ringqueue = static_cast<RingQueue<task>*>(rq); 
  while(true)
  {
    int x = rand() % 10;
    int y = rand() % 10;
    char op = oper[rand()%oper.size()];

    task t(x, y, op, myadd);
    
    ringqueue->push(t);
    std::cout<< selfname() <<" 生产者派发任务"<< t.totaskstring()<< " "<<std::endl; 
  }
}

void* consumerRoutine(void* rq)
{
  RingQueue<task>* ringqueue = static_cast<RingQueue<task>*>(rq); 
  while(true)
  {
    task t;
    ringqueue->pop(&t);
    std::string result = t();
    std::cout<< selfname() <<"             消费者消费了一个任务："<< result <<std::endl; 
    sleep(3);
  }
}

int main()
{
  srand((unsigned int)time(nullptr)^getpid()^pthread_self()); // 随机数种子
  
  RingQueue<task>* rq = new RingQueue<task>();

  pthread_t p[4], c[8];
  for(int i = 0; i < 4; i++) pthread_create(p+i, nullptr, productorRontine, rq);  //rq就是同一份资源
  for(int i = 0; i < 8; i++) pthread_create(c+i, nullptr, consumerRoutine, rq);   //rq就是同一份资源
  // 单生产者 单消费者
  // 多生产者 多消费者
  // 生产者相互竞争
  // 消费者相互竞争 

  for(int i = 0; i < 4; i++) pthread_join(p[i], nullptr);
  for(int i = 0; i < 8; i++) pthread_join(c[i], nullptr);
  return 0;
}
