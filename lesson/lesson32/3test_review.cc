#include <pthread.h>
#include <cstring>
#include <vector>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
using namespace std;

// 总结 
// 1.线程回收比拿到tid，注意vector的数据放进去
// 2.注意new出来的资源和delete搭配
// 3.线程申请的资源在堆上，不能直接在线程里面 定义临时变量拿出来的

class threadData
{
public:
  int id;
  pthread_t tid;
  char namebuffer[64];
};

class threadReturn
{
public:
  int exit_code;
  int exit_result;
};

void* start_routine(void* arg)
{
  sleep(1);
  threadData* td = static_cast<threadData*>(arg);
  cout<< "I  am " <<  td->namebuffer <<endl;
  
  threadReturn* tr = new threadReturn();
  tr->exit_code = 0;
  tr->exit_result = 10;

  return (void*)tr;
}

int main()
{
  vector<threadData*> threads;
  for(int i = 0; i < 10; ++i)
  {
    threadData* td  = new threadData();
    td->id = i + 1;
    snprintf(td->namebuffer, sizeof td->namebuffer, "thread%d", i + 1);

    int n = pthread_create(&td->tid, nullptr, start_routine, td);
    if(n != 0)
    {
      cerr<< "pthread_create error : " << strerror(n) << endl;
      return 1;
    }

    threads.push_back(td);
  }

  for(auto& iter : threads)
  {
    void*  ret  = nullptr;
    int n = pthread_join(iter->tid, &ret);
    if(n != 0)
    {
      cerr<< "pthread_join error : " << strerror(n) << endl;
      return 1;
    }

    threadReturn* retNew = static_cast<threadReturn*>(ret);
    cout<< iter->namebuffer << "  success  " << "exit_code:"<< retNew->exit_code << "exit_result:" << retNew->exit_result <<endl;
    delete retNew;
  }

  for(auto& iter : threads)
  {
    delete iter;
  }
  
  threads.clear();

  return 0;
}
