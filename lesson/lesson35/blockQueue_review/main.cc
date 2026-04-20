#include "blockqueue.hpp"
#include "task.hpp"
#include <unistd.h>

#if 0
void* consum(void* arg)
{
  blockqueue<int>* bq = static_cast<blockqueue<int>*>(arg);

  while(true)
  {
    sleep(1);
    int x = 0;

    bq->pop(&x);
    std::cout<< "消费数据"<< x << std::endl;
  }

  return nullptr;
}

void* product(void* arg)
{
  blockqueue<int>* bq = static_cast<blockqueue<int>*>(arg);
  
  while(true)
  {
    sleep(1);
    int x =rand() % 10 + 1;
    bq->push(x);
    
    std::cout<< "生产数据" << x << std::endl;
  }
  return nullptr;
}

#else 
int add(int x, int y)
{
  return x + y;
}

void* consum(void* arg)
{
  blockqueue<task>* bq = static_cast<blockqueue<task>*>(arg);

  while(true)
  {
    sleep(1);
    task t;
    bq->pop(&t);
    std::cout<< "消费数据"<< t() << std::endl;
  }

  return nullptr;
}

void* product(void* arg)
{
  blockqueue<task>* bq = static_cast<blockqueue<task>*>(arg);
  
  while(true)
  {
    sleep(1);
    int x = rand() % 10;
    int y = rand() % 10;
    task t(x, y, add);
    bq->push(t);
    
    std::cout<< "生产数据" << "x " << x << ":" << "y " << y <<std::endl;
  }
  return nullptr;
}


#endif

int main()
{
#if 0
  srand(getpid());
  blockqueue<int>* bq = new  blockqueue<int>();
  pthread_t consumer;
  pthread_t producter;

  pthread_create(&consumer, nullptr, consum, bq);
  pthread_create(&producter, nullptr, product, bq);

  pthread_join(consumer, nullptr);
  pthread_join(producter,  nullptr);

  delete bq;

#else 
  srand(getpid());
  blockqueue<task>* bq = new  blockqueue<task>();
  pthread_t consumer;
  pthread_t producter;

  pthread_create(&consumer, nullptr, consum, bq);
  pthread_create(&producter, nullptr, product, bq);

  pthread_join(consumer, nullptr);
  pthread_join(producter,  nullptr);

  delete bq;
#endif

  return 0;
}
