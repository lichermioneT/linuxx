#include <iostream>
#include <unistd.h>
#include <string>
#include <pthread.h>

int tickets = 1000;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

void* start_routine(void* args)
{
  std::string name = static_cast<const char*>(args);
  while(true)
  {
    pthread_mutex_lock(&mutex);
    pthread_cond_wait(&cond,&mutex); // 为什么要有mutex？
    // 判断暂时省略
    std::cout<< name << " - " << tickets << std::endl;
    tickets--;
    pthread_mutex_unlock(&mutex);
  }
}

int main()
{
  // 同环境变量控制线程的执行
  pthread_t t[5];
  for(int i = 0; i < 5; i++)
  {
    char* name = new char[64];
    snprintf(name, 64, "thread %d", i+1);
    pthread_create(t+i, nullptr, start_routine, name);
  }

  while(true)
  {
    sleep(1);
   // pthread_cond_signal(&cond); // 唤醒一个线程 _broadcast唤醒一批线程
    pthread_cond_broadcast(&cond); // 唤醒一批线程
    std::cout<< "main thread wake up ..." <<std::endl;
  }

  for(int i = 0; i < 5; i++)
  {
    pthread_join(t[i],nullptr);
  }
  return 0;
}
