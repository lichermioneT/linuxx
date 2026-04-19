#include <iostream>
#include <unistd.h>
#include <string>
#include <pthread.h>
#include <vector>
using namespace std;

int tickets = 10000;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t  cond = PTHREAD_COND_INITIALIZER;

void* getTicket(void* arg)
{
  string name = static_cast<const char*>(arg);

  while(true)
  {
    pthread_mutex_lock(&mutex);
    pthread_cond_wait(&cond, &mutex);
// 1.条件变量的等待队列
// 2.原子释放mutex：解锁 + 进入等待
// 3.唤醒后，重新竞争mutex，然后返回。
    if(tickets > 0)
    {
      cout<< name << " 抢票" << tickets << endl;
      --tickets;
      pthread_mutex_unlock(&mutex);
    }
    else 
    {
      pthread_mutex_unlock(&mutex);
      break;
    }
  }
  return nullptr;
}

int main()
{
  pthread_t tid[5];
  for(int i = 0; i < 5; ++i)
  {
    char* buffer = new char[64];
    snprintf(buffer, 64, "thread->%d ", i + 1);
    
    int n = pthread_create(&tid[i], nullptr, getTicket, buffer);
    if(n != 0)
    {
      perror("pthread_create");
      return 1;
    }
  }

  while(true)
  {
#if 0
    pthread_cond_signal(&cond); 
#else 
   pthread_cond_broadcast(&cond);
#endif
  }
  return 0;
}
