#include "threadpool.hpp"
#include "tast.hpp"
#include <memory>

int main()
{
  
 // std::unique_ptr<threadpool<task>> tp(new threadpool<task>());
  
  threadpool<task>::getInstance()->run();
  int x, y;
  char op;
  while(true)
  {
    std::cout<< "请输入数据1#";
    std::cin>>x;
    std::cout<< "请输入数据2#";
    std::cin>>y;
    std::cout<< "请输入您要进行的运算#";
    std::cin>>op;
    
    task t(x, y, op, myadd);
    /*
    std::cout<< "您刚录入一个任务 ：" << t.totaskstring() << " 确认提交吗？[y/n]"<<std::endl;
    char confirm = 0;
    std::cin>>confirm;

    if(confirm == 'y')
    {
      tp->push(t);
    }
    */ 
    threadpool<task>::getInstance()->push(t);
    sleep(1);
  }



  return 0;
}




















/*
 *
#include "thread.hpp"
#include "threadpool.hpp"
#include <unistd.h>

void* run(void* args)
{
  std::string message = (const char*)args;
  while(true)
  {
    std::cout<< message <<std::endl;
    sleep(1);
  }
  return nullptr;
}

int main()
{

  ThreasNs::Thread t1(run, (void*)"thread 1 run");
  ThreasNs::Thread t2(run, (void*)"thread 2 run" );

  t1.start();
  t2.start();


  t1.join();
  t2.join();









  return 0;
}
*/
