#include <pthread.h>
#include <thread>
#include <string>
#include <iostream>
using namespace std;

void* print(void* str)
{
  cout<< "新线程的----" <<endl;
  cout<< *(static_cast<const string*>(str)) <<endl;
  return str;
}

void print2(const string& str)
{
  cout<< "新线程的----" <<endl;
  cout<< str << endl;
}

class test 
{
public:
  int add (int x, int y)
  {
    cout<< "新线程的----" <<endl;
    int ret = x + y;
    return  ret;
  }
};

int main()
{
#define VERSION 3
  
#if VERSION == 1
// pthread_create操作的函数必须是 (void*)(*start_route)(void*)
// 传入的参数可以，进行强制类型转换 (void*)
  pthread_t tid = 0;    
  string str = "lichermionex";
  int n = pthread_create(&tid, NULL, print,(void*)&str);
  if(n == -1)
  {
    perror("pthread_create");
    return -1;
  }

  string* out;
  pthread_join(tid, (void**)&out);
  cout<< "主线程的----" <<endl;
  cout<< *out << endl;

#elif VERSION == 2
  cout<< "2" <<endl;
  /*
   *string str = "lichermionex";
   *thread t(print, &str);
   *t.join();
   *cout<< "主线程的----" <<endl;
   */

  string str = "lichermionex";
  thread t(print2, std::ref(str));
  cout<< "主线程的----" <<endl;
  t.join();

#else
  cout<< "3" <<endl;
  test te;
  thread t(&test::add, &te, 1, 3);
  cout<< "主线程的----" <<endl;
  t.join();
#endif

  return 0;
}
