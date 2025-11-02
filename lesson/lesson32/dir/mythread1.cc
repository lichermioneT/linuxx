#include <pthread.h>
#include <unistd.h>
#include <string>
#include <iostream>
using namespace std;

void* start_routine(void* args)
{
  // 一个线程出现异常，会影响其它线程的
  // 进程信号，信号是发给整体发给进程的！
    string name = static_cast<const char*>(args);
    while(true)
    {
      cout<< "new thread  create success : " << name <<endl;
      sleep(1);

      int* p = nullptr;
      p = nullptr;
      *p = 0;
    }
}

int main()
{

  pthread_t id;
  pthread_create(&id, nullptr, start_routine, (void*) "pthread_one");

  while(true)
  {
      cout<< "main thread" <<endl;
      sleep(1);
  }


  return 0;
}
