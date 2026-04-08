#include <iostream>
#include <pthread.h>
#include <thread>
#include <unistd.h>
#include <string>
using namespace std;

void fun()
{
  cout<< "tttttttttttttt" <<endl;
}

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
  while(true)
  {
    cout<< "子线程" <<endl;
    fun();
    sleep(1);
  }
  sleep(2);
  Person* p = static_cast<Person*>(args); // static_cast<>() 相关类型的相互转换
  cout<< "子线程--" <<endl;
  cout<< "age:" << p->age_ << " height:" << p->height_ << " weight:" << p->weight_ << "  thread" << endl;
  
  return args;
}

class test 
{
public:
  void print(const string& str)
  {
    cout<< str << endl;
  }

  int add(int x, int y)
  {
    int ret = x + y;
    cout<< "ret " << ret <<endl;
    return ret;
  }

};

#define PALTFORM 2
int main()
{
#if PALTFORM == 0
  pthread_t tid;
  Person* p = new Person(25, 170, 125);
  pthread_create(&tid, nullptr, start_routine, p);
  
  while(true)
  {
    cout<< "父亲线程我创建的是线程的tid:" << tid <<endl;
    fun();
    sleep(1);
  }
  cout<< "main thread " <<endl;
 
  Person* output = nullptr;
  pthread_join(tid,(void**)&output); // 主进程运行到这里，等待子线程
  
  cout<< "等待出来的参数" <<endl;
  cout<< "age:" << p->age_ << " height:" << p->height_ << " weight:" << p->weight_ <<endl;

#elif  PALTFORM == 1

// c++一般创建线程，执行一般的函数 
// thread t(函数名，函数参数的) 注意引用是std::ref()

  Person* p = new Person(25, 170, 125);
  thread t(start_routine, p);
  
  t.join();

#elif  PALTFORM == 2
// 对象d执行的话
// thread t(对象的声明，实例化的对象地址， 函数参数)
  string str = "lichermionex";
  test tt;
  thread t1(&test::print, &tt, std::ref(str));
  t1.join();

  thread t2(&test::add, &tt, 10, 20);
  t2.join();

#else 


#endif 
  return 0;
}
