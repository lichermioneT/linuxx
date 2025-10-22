#include "comm.hpp"

int main()
{

// key获取
  key_t k = getKey();
  printf("key : 0x%x\n", k);

// shmid获取
  int shmid = getShm(k);
  printf("shmid : %d\n", shmid);

// 链接内存
  char* start = (char*)attachShm(shmid);
  printf("attach success, address start: %p \n", start);

// 通信
  const char* msg = "hello server, 我是另一个进程，正在和你通信";
  pid_t id = getpid();
  int cnt = 1;
  while(true)
  {
    snprintf(start, MAX_SIZE, "%s[pid %d][消息次数 ；%d]\n", msg, id, cnt++);
  }

// 断开链接
  detachShm(start);
  
  return 0;
}
