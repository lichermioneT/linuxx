#ifndef __COMM_HPP__
#define __COMM_HPP__ 

#include <iostream>
#include <cerrno>
#include <cstring>
#include <cstdio>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>

#define MAX_SIZE 4096
#define PATHNAME "/tmp"
#define PROJ_ID 0X66 

key_t getKey()
{
  key_t k = ftok(PATHNAME, PROJ_ID); // 同样的数据，同样的key值，整形
  if(k < 0)
  {
    // cin,cout,cerr---> 0,1,2
    std::cerr<< errno << ":" << strerror(errno) <<std::endl;
    exit(1);
  }
  return k;
}

// IPC_CREAT 
// IPC_EXCL
//
int getShmHelper(key_t k, int flags)
{
  // k是要shmget，设置共享内存的属性里面的，用来表示
  // 该共享进程在内核的唯一性的
  //
  // 用户层    内核层
  // shmid vs  key  ：key是shmind的属性 
  // fd    vs  inode：inode是fd的属性
    int shmid = shmget(k, MAX_SIZE, flags); // 唯一标识符，大小，标识符
    if(shmid  < 0)
    {
      std::cerr<< errno << " : " << strerror(errno) <<std::endl;
      exit(2);
    }
    return shmid;
}

int createShm(key_t k)
{
  return getShmHelper(k, IPC_CREAT  | 0600);
}

int getShm(key_t k)
{
  return getShmHelper(k, IPC_CREAT  /*0*/);
}



void* attachShm(int shmid)
{
  // 纯数字没有任何意义的，必须的有类型才行的
  // int a = 10;
  // 100字面量，
  // 100u;
  // 10u
  // 10L
  // 10;
  // 3.14f
  

  void* mem = shmat(shmid,nullptr,0); // 8个字节，int四字节
  if((long long)mem == 1L)
  {
    std::cerr<< errno << " : " << strerror(errno) <<std::endl;
    exit(3);
  }

  return mem;
}

void detachShm(void* start)
{
  if(shmdt(start) == -1)
  {
    std::cerr<< errno << " : " << strerror(errno) <<std::endl;
  }
}


// 删除共享内存
// shmctl(id,cmd, shmid_ds* buf);
void delShm(int shmid)
{
  if(shmctl(shmid, IPC_RMID, nullptr) == -1)
  {
    std::cerr<< errno << " : " << strerror(errno) <<std::endl;
  }
}
  








#endif 
