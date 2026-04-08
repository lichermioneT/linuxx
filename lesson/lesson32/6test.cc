#include <pthread.h>
#include <cstdlib>
#include <vector>
#include <unistd.h>
#include <string>
#include <iostream>
using namespace std;

void* start_routine(void* args)
{
    cout<< "新线程" <<endl;
    const char* td = static_cast<const char*>(args);
    cout<< td <<endl;
  
    pthread_exit((void*)td);
    /*
     *return (void*)td;
     */
}

int main()
{
  pthread_t tid;
  pthread_create(&tid, nullptr, start_routine, (void*)"licherminonxe");

  void* str;
  pthread_join(tid, &str);
  cout<< "主线程" <<endl;
  const char* str2 = static_cast<const char*>(str);
  cout<< str2 << endl;

  return 0;
}
