#include <iostream>
#include <pthread.h>
#include <unistd.h>
using namespace std;

class Person
{
public:
  Person(int age, int height, int weight)
    :age_(age)
    ,height_(height)
    ,weight_(weight)
    {}
public:
  int age_;
  int height_;
  int weight_;
};

void* start_routine(void* args)
{
  sleep(2);
  Person* p = static_cast<Person*>(args); // static_cast<>() 相关类型的相互转换
  cout<< "子线程--" <<endl;
  cout<< "age:" << p->age_ << " height:" << p->height_ << " weight:" << p->weight_ << "thread" << endl;
  
  return args;
}

int main()
{
  pthread_t tid;
  Person* p = new Person(25, 170, 125);
  pthread_create(&tid,nullptr,start_routine, p);
  
  cout<< "main thread " <<endl;
 
  Person* output = nullptr;
  pthread_join(tid,(void**)&output); // 主进程运行到这里，等待子线程
  
  cout<< "等待出来的参数" <<endl;
  cout<< "age:" << p->age_ << " height:" << p->height_ << " weight:" << p->weight_ <<endl;
  return 0;
}
