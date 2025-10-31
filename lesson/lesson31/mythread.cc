#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <cassert>
#include <cstdio>
using namespace std;


void fun()
{
  cout<< "this is a new method" <<endl;
}

// 新线程，
void* thread_routine(void* agrs)
{
  const char* name = (const char*)agrs;
  while(true)
  {
     cout << "我是新线程， 我真正运行 ！";
     cout << "name : " << name;
     fun();
     sleep(2);
  }
  return nullptr;
}

int main()
{

  pthread_t tid;
  int n = pthread_create(&tid, nullptr, thread_routine, (void*)"thread_one");
  assert(0 == n);
  (void)n;

  // tid是地址
   char buffer[64];
   // 主线程
   snprintf(buffer, sizeof(buffer), "0x%x \n", tid);
   while(true)
   {
     cout << "我是主线程， 我真正运行 ！ 我创建出来的tid :" << buffer;
     fun();
     sleep(2);
   }
  




  return 0;
}
