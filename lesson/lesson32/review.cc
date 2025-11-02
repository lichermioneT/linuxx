#include <vector>
#include <unistd.h>
#include <iostream>
#include <pthread.h>
using namespace std;

#define MAX 10
class ThreadData
{
public:
  int number;
  pthread_t tid;
  char namebuffer[63];
};


class ThreadReturn
{
public: 
  int exit_code;
  int exit_result;
};

void* start_routine(void* args)
{
  ThreadData* td = static_cast<ThreadData*> (args);
  int n = 1;
  while(n)
  {
    sleep(1);
/*
    cout<< "----------------------" <<endl;
    cout<< "n : " << n << " : " << "&n : " << &n <<endl;
    cout<< "namebuffer : " << td->namebuffer <<endl;
    cout<< "number : " << td->number <<endl;
    cout<< "tid : "<< td->tid <<endl;
    cout<< "----------------------" <<endl;
*/ 
    cout << "n:" << n << " &n:" << &n << " namebuffer:" << td->namebuffer << " number:" << td->number << " tid:" << td->tid << endl;
    n--;
  }

  ThreadReturn* tr = new ThreadReturn();
  tr->exit_code = 1;
  tr->exit_result = 1101;

  return (void*)tr;
}


int main()
{

  vector<ThreadData*> threads;
  
  for(int i = 0; i < MAX; i++)
  {
    ThreadData* td = new ThreadData();
    td->number = i + 1;
    snprintf(td->namebuffer, sizeof(td->namebuffer), "%s:%d", "thread", i+1);
    pthread_create(&td->tid, nullptr, start_routine, td);
    threads.push_back(td);
  }

  sleep(1);

/*
  for(auto& iter: threads)
  {
    ThreadReturn* ret = nullptr;
    pthread_join(iter->tid, (void**)&ret);
    cout<< "join success : " << "success exit_code : " << ret->exit_code << " success exit_result : " << ret->exit_result <<endl;
    delete  iter;
  }
*/

  for(size_t i = 0; i < threads.size()/2; i++)
  {
    pthread_cancel(threads[i]->tid);
  }


  for(auto& iter: threads)
  {
    void* ret_val = nullptr;  // 先定义一个void*指针
    pthread_join(iter->tid, &ret_val);  // 传递这个指针的地址
    ThreadReturn* ret = static_cast<ThreadReturn*>(ret_val);
    cout << "join success : " << "success exit_code : " << ret->exit_code 
       << " success exit_result : " << ret->exit_result << endl;
    delete ret;     // 删除线程返回的对象
    delete iter;    // 删除线程数据对象
  }






  return 0;
}
