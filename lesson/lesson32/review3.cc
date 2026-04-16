#include <iostream>
#include <vector>
#include <pthread.h>
#include <unistd.h>
#include <cstdio>
#include <cstring>

using namespace std;
class threadData
{
public:
  int id;
  pthread_t tid;
  char namebuffer[64];
};

void* start_routine(void* arg)
{
  sleep(10);
  threadData* td = static_cast<threadData*>(arg);
  cout<< td->namebuffer << ":" << td->id <<endl;

  return td;
}

int main()
{

  vector<threadData*> v;
// 1.创建10个线程，线程信息放到v里面去的。
  for(int i = 0; i < 10; ++i)
  {
    threadData* td = new threadData();
    td->id = i + 1;
    snprintf(td->namebuffer, sizeof(td->namebuffer), "thread%d", i + 1);

    int n = pthread_create(&td->tid, nullptr, start_routine, td);
    if(n != 0)
    {
      cerr<< "pthread_create error " << strerror(n) <<endl;
      return 1;
    }

    v.push_back(td);
  }

// 2.取消5个线程的
  sleep(3);
  for(int i = 0; i < 5; ++i)
  {
    int n = pthread_cancel(v[i]->tid); 
    if(n != 0)
    {
      cerr<< "pthread_cancel errno : " << strerror(n) << endl;
      return 1;
    }
    
    cout<< "线程被取消了" << i + 1<< endl;
  }

// 3.回收线程
  for(auto& e : v)
  {
    void* ret = nullptr;
    int n  = pthread_join(e->tid, &ret);
    if(n != 0)
    {
      cerr << "pthread_join errno:" << strerror(n) << endl;
      continue;
    }

    if(ret == PTHREAD_CANCELED)
    {
      cout<< e->namebuffer << "被取消" <<endl;
    }
    else 
    {
      threadData* td = static_cast<threadData*>(ret);
      cout<< td->namebuffer << "正常结束id = " << td->id <<endl; 
    }
  }

  return 0;
}
