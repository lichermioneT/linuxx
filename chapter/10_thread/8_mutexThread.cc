#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <string>
#include <cstring>
#include <vector>

int tickets = 1000;
pthread_mutex_t mutex;

void* start_routine(void* arg)
{
  (void)arg;
  while(true)
  {
    pthread_mutex_lock(&mutex);
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

