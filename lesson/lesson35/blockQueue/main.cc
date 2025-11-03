#include "blockqueue.hpp"
#include <ctime>
#include <sys/types.h>
#include <unistd.h>
#include <cstdlib>

void* consumer(void*bq_)
{
  blockqueue<int>* bp = static_cast<blockqueue<int>*>(bq_);
  while(true)
  {
    int data;
    bp->pop(&data);
    std::cout<<"消费数据："<< data <<std::endl;
  }
  return nullptr;
}

void* productor(void*bq_)
{
  blockqueue<int>* bp = static_cast<blockqueue<int>*>(bq_);
  while(true)
  {
    int data = rand() % 10 + 1; // 随机数构建一个数据
    bp->push(data);
    std::cout<< "生产数据：" << data <<std::endl;
    sleep(2);
  }
  return nullptr;
}

int main()
{
    
  srand((unsigned long)time(nullptr)^getpid());

  blockqueue<int>* bq = new blockqueue<int>();
  pthread_t c, p;

  pthread_create(&c,nullptr,consumer, bq);
  pthread_create(&p,nullptr,productor, bq);
 

  
  pthread_join(c, nullptr);
  pthread_join(p, nullptr);

  delete bq;


  return 0;
}
