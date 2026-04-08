#include <vector>
#include <unistd.h>
#include <iostream>
#include <pthread.h>
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
  sleep(10);
  threadData* td = static_cast<threadData*>(arg);
  cout<< td->buffer << ":" << td->id <<endl;
  
  return  (void*)arg;
}

int main()
{
  vector<threadData*> v;

  for(int i = 0; i < 10; ++i)
  {
    threadData* td = new threadData();    

    td->id = i+1;
    snprintf(td->buffer, sizeof(td->buffer), "%s:%d", "thread", i + 1);

    int n = pthread_create(&td->tid, nullptr, start_routine, td);
    if(n == -1)
    {
      perror("pthread_create");
      return -1;
    }
    v.push_back(td);
  }

  sleep(3);
  for(int i = 0; i < 5; ++i)
  {
    cout<< "线程被取消了" << i + 1 <<endl;
    pthread_cancel(v[i]->tid);
  }
  
  for(auto& e : v)
  {
    void* ret = nullptr;
    int n = pthread_join(e->tid, &ret);
    if(n != 0)
    {
      cerr << "pthread_join" << endl;
      continue;
    }
    
    if(ret == PTHREAD_CANCELED)
    {
      cout << e->buffer << " 被取消退出" << endl;
    }
		else 
		{
				threadData* td = static_cast<threadData*>(ret);
        cout << td->buffer << " 正常返回, id = " << td->id << endl;
		}

  }


  return 0;
}
