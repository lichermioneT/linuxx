#include "blockqueue.hpp"
#include "tast.hpp"
#include <ctime>
#include <sys/types.h>
#include <unistd.h>
#include <cstdlib>

const std::string oper = "+-*/%";

template<class C, class S>
class blockqueues
{
public:
  blockqueue<C>* c_bp;
  blockqueue<S>* s_bp;
};

// 保存函数
void* saver(void* bqs_)
{
  blockqueue<savetask>* save_bq = static_cast<blockqueues<task, savetask>*>(bqs_)->s_bp;
  while(true)
  {
    savetask t;
    save_bq->pop(&t);
    t();
    std::cout<< "保存任务完成....."<<std::endl;
  }
  return nullptr;
}

// 消费者函数
void* consumer(void* bqs_)
{
  blockqueue<task>* bp = static_cast<blockqueues<task, savetask>*>(bqs_)->c_bp;
  blockqueue<savetask>* save_bq = static_cast<blockqueues<task, savetask>*>(bqs_)->s_bp;
  
  while(true)
  {
    task t;
    bp->pop(&t);                        // 取数据，后面处理
    std::string result = t();
    std::cout<<"消费数据："<< result <<std::endl;

    // 存储任务 
    savetask save(result,Save);
    save_bq->push(save);
    std::cout<< "推送 保存任务完成..." <<std::endl;
  }
  return nullptr;
}

// 生产者函数
void* productor(void* bqs_)
{
  blockqueue<task>* bp = static_cast<blockqueues<task, savetask>*>(bqs_)->c_bp;
  while(true)
  {
    // 
    int x = rand() % 10 + 1; // 随机数构建一个数据[1,10]
    int y = rand() % 5;      // [0, 4]
    int opreCode = rand() % oper.size();  // 生产数据

    std::cout<< "生产数据"<< x << " : " << y << " : "<< oper[opreCode] <<std::endl;
    task t(x, y,oper[opreCode], myadd);   // 放的数据对象
    bp->push(t);                          // 放数据,任务
    sleep(2);
  }
  return nullptr;
}


int main()
{
  srand((unsigned long)time(nullptr)^getpid());
  blockqueues<task, savetask> bqs;              // 里面存放生产者和消费者模型 
  
  bqs.c_bp = new blockqueue<task>();            // 生产者
  bqs.s_bp = new blockqueue<savetask>();        // 消费者
  
  pthread_t c, p, s;
  pthread_create(&c, nullptr, consumer, &bqs);    // 生产者
  pthread_create(&p, nullptr, productor, &bqs);   // 消费者
  pthread_create(&s, nullptr, saver, &bqs);      // 保存着

  pthread_t c1, p1, s1;
  pthread_create(&c1, nullptr, consumer, &bqs);    // 生产者
  pthread_create(&p1, nullptr, productor, &bqs);   // 消费者
  pthread_create(&s1, nullptr, saver, &bqs);      // 保存着

  pthread_join(c, nullptr);
  pthread_join(p, nullptr);
  pthread_join(s, nullptr);

  pthread_join(c1, nullptr);
  pthread_join(p1, nullptr);
  pthread_join(s1, nullptr);

  delete bqs.c_bp;
  delete bqs.s_bp;
  return 0;
}
