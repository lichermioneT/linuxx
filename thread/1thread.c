#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

void* callback(void* arg)
{
  for(int i = 0; i < 5 ; i++)
  {
    printf("子线程 %d\n", i);
  }
  printf("子线程id %ld \n",pthread_self());

  return NULL;
}

int main()
{


  pthread_t tid;
  pthread_create(&tid, NULL, callback, NULL);
  
  for(int i = 0; i < 5 ; i++)
  {
    printf("主线程 %d\n", i);
  }
  printf("主线程id %ld \n",pthread_self());

  // 睡3秒 等子线程执行
  // sleep(1);
  
  pthread_exit(NULL);
  return 0;
}
