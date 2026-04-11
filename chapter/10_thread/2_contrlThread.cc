#include <pthread.h>
#include <unistd.h>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

// 总结：
// 创建线程需要传递参数，我们封装一个对象 threadData, 给线程传入参数信息的

// 创建一个线程需要tid,我们可以存放在结构体，然后再把结构体信息传递给
// 线程是需要回收的，就和进程一样的。 也许我们需要知道线程的返回值信息。
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
    cout<< td->id << "::" << td->buffer << endl;
    sleep(2);
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
// 成功返回的是 零
    int n = pthread_create(&td->tid, nullptr, start_routine, td);
    if(n != 0)
    {
      std::cerr<< "pthread_create" << strerror(n) << std::endl;
      return 1;
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
