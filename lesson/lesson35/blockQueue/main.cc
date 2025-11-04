#include "blockqueue.hpp"
#include "tast.hpp"
#include <ctime>
#include <sys/types.h>
#include <unistd.h>
#include <cstdlib>


int myadd(int x, int y)
{
  return x + y;
}

// 消费者函数
void* consumer(void*bq_)
{
  blockqueue<task>* bp = static_cast<blockqueue<task> *>(bq_);
  while(true)
  {
    task t;
    bp->pop(&t);
    std::cout<<"消费数据："<< t() <<std::endl;
  }
  return nullptr;
}

// 生产者函数
void* productor(void*bq_)
{
  blockqueue<task>* bp = static_cast<blockqueue<task>*>(bq_);
  while(true)
  {
    int x = rand() % 10 + 1; // 随机数构建一个数据[1,10]
    int y = rand() % 5  + 1;
    std::cout<< "生产数据"<< x << " : " << y <<std::endl;
    task t(x, y, myadd);
    bp->push(t);
    //std::cout<< "生产数据：" << data <<std::endl;
    sleep(2);
  }
  return nullptr;
}

int main()
{
  srand((unsigned long)time(nullptr)^getpid());

  blockqueue<task>* bq = new blockqueue<task>();
  pthread_t c, p;

  pthread_create(&c,nullptr,consumer, bq);
  pthread_create(&p,nullptr,productor, bq);

  pthread_join(c, nullptr);
  pthread_join(p, nullptr);

  delete bq;
  return 0;
}
