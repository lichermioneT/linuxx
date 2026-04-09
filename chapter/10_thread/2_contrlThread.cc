#include <pthread.h>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

class threadData
{
public:
  pthread_t tid;
  int id;
  char buffer[64];
};

void* start_routine(void* arg)
{
    threadData* td = static_cast<threadData*>(arg);
    cout<< td->id << "---" << td->buffer << endl;

    return td;
}

int main()
{
  vector<threadData*> v;
  for(int i = 0; i < 10; ++i)
  {
    threadData* td = new threadData();
    td->id = i + 1;
    snprintf(td->buffer, sizeof(td->buffer), "%s:%d", "thread", i + 1);
    
// 注意 这里失败了返回的是错误码的
    int n = pthread_create(&td->tid, nullptr, start_routine, td);
    if(n != 0)
    {
      cerr<< "pthread_create" << strerror(n) << endl;
      return -1;
    }

    v.push_back(td);
  }

// 1.注意这里是pthread_join.
//   它是会阻塞等待的，所以这里是1到10的回收顺序的。
  for(auto& e : v)
  {
    void* ret = nullptr;
// 注意 这里失败了返回的是错误码的
    int n = pthread_join(e->tid, &ret);
    if(n != 0)
    {
      cerr<< "pthread_join" << strerror(n) << endl;
      continue;
    }

      threadData* td = static_cast<threadData*>(ret);
      cout<< td->buffer << "正常返回" <<endl;

  }

  for(auto e : v)
  {
    delete e;
  }

  v.clear();


  return 0;
}
