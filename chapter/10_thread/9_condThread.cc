#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <string>
#include <cstring>
#include <vector>

int tickets = 1000;
pthread_mutex_t mutex;
pthread_cond_t cond;

void* start_routine(void* arg)
{
  (void)arg;
  while(true)
  {
    pthread_mutex_lock(&mutex);
// 1.进入wait的时候拿到mutex, 
// 2.然后mutex释放了，然后放到cond队列里面去
// 3.等待队列里面，重新竞争mutex， 然后真正的返回
    pthread_cond_wait(&cond, &mutex);
    if(tickets > 0)
    {
      std::cout<< "正在抢票:" << tickets << std::endl;
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
  pthread_mutex_init(&mutex, nullptr);
  pthread_cond_init(&cond, nullptr);

  std::vector<pthread_t> td;
  for(int i = 0; i <5; ++i)
  {
    pthread_t tid;
    int n = pthread_create(&tid, nullptr, start_routine, nullptr);
    if(n != 0)
    {
      std::cerr<< "pthread_create error" << strerror(errno) << std::endl;
      return -1;
    }

    td.push_back(tid);
  }
  
  while(true)
  {
#if 0
    sleep(1);
    pthread_cond_signal(&cond);

#else 

    pthread_cond_broadcast(&cond);
#endif
  }

  for(auto e : td)
  {
    int n = pthread_join(e, nullptr);
    if(n != 0)
    {
      std::cerr<< "pthread_join" << strerror(errno) <<std::endl;
      continue;
    }
  }
  return 0;
}

