#include <pthread.h>
#include <cstring>
#include <vector>
#include <iostream>
#include <unistd.h>
using namespace std;

// 总结
// 1.线程执行的函数，需要的参数可以放在对象里面进行，传参的。
// 2.vector里面有指针记得释放，然后clear,的
class threadData 
{
public:
  int id;
  pthread_t tid;
  char buffer[64];
};

void* start_routine(void* arg)
{
  sleep(1);
  threadData* td = static_cast<threadData*>(arg);
  cout<< td->buffer << endl;

  return td;
}

int main()
{
  vector<threadData*> v; 
  for(int i =  0; i  < 5; ++i)
  {
    threadData* td = new threadData();
    td->id = i + 1;
    snprintf(td->buffer, sizeof(td->buffer), "thread %d",  i + 1);

    int n = pthread_create(&td->tid, nullptr, start_routine, td);
    if(n != 0)
    {
      cerr << "pthread_create error  : " << strerror(errno) << endl;
      return 1;
    }

    v.push_back(td);
  }

  for(int i = 0; i < 5; ++i)
  {
    void* ret = nullptr;
    int n = pthread_join(v[i]->tid, &ret);
    if(n != 0)
    {
      cerr << "pthread_join error  : " << strerror(errno) << endl;
      return 1;
    }

    threadData* td = static_cast<threadData*>(ret);
    cout<< "线程" << td->id << "被回收了" << endl;
  }

  for(auto& e : v)
  {
    delete e;
  }

  v.clear();
  return 0;
}
