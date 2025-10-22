#include "comm.hpp"


int main()
{

// 获取key
  key_t k = getKey();
  printf("0x%x\n", k); // key 
  
// shmget:共享内存创建
  int shmid = createShm(k); // shmid
  printf("%d \n", shmid);

  sleep(10);
// 链接内存
  char* start = (char*)attachShm(shmid);
  printf("attach success, address start: %p \n", start);
  
// 读取数据
  while(true)
  {
    printf("client say : %s ", start);
    struct shmid_ds ds;
    shmctl(shmid, IPC_STAT, &ds);
    sleep(1);
  }

  // 去关联
  detachShm(start);

  sleep(5);
// 删除共享内存
// 删除，谁创建谁删除
  delShm(shmid);
  return 0;
}
