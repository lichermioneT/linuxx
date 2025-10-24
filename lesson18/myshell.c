#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <assert.h>

#define NUM 1024
#define OPT_NUM 64
int lastCode = 0;
int lastSig = 0;

char linecommand[NUM];
char* myagrv[OPT_NUM];

int main()
{

  
  while(1)
  {

  printf("用户名@主机名 当前路径# ");
  fflush(stdout);
  sleep(10);
  
  // 去掉0 sizeof会计算 0的
  char* s = fgets(linecommand, sizeof(linecommand)-1, stdin);
  assert(s != NULL);
  (void)s;

  // 清理最后一个\n
  //
  linecommand[strlen(linecommand) - 1] = 0;
  //  printf("test %s\n", linecommand);
  
  // " ls -a -l -h"
  myagrv[0] = strtok(linecommand, " ");
  
  int i = 1;
  if(myagrv[0] != NULL && strcmp(myagrv[0], "ls") == 0)
  {
    myagrv[i] =(char*) "--color=auto";
  }

  while((myagrv[i++] = strtok(NULL, " ")) != NULL)
  {
    ;
  }

  // 如果是cd指令，不需要创建子进程，让对应的shell执行。
  // 不需要子进程来执行，让shell自己执行----- 内建/内置命令
  if(myagrv[0] != NULL && strcmp(myagrv[0],"cd") == 0)
  {
    if(myagrv[1] != NULL) chdir(myagrv[1]);
    continue;
  }

  if(myagrv[0] != NULL && myagrv[1] != NULL && strcmp(myagrv[0], "echo") == 0 )
  {
    if( strcmp(myagrv[1],"$?") == 0) 
    {
      printf("%d, %d \n", lastCode, lastSig);
    }
    else 
    {
      printf("%s \n", myagrv[1]);
    }

    continue;
  }
  //测试 是否成功
  
#ifdef DEBUG 
  for(int i = 0; myagrv[i]; i++)
  {
    printf("myagrv[%d], %s \n", i, myagrv[i]);
  }
#endif

  // 执行命令 
  pid_t id = fork();
  assert(id != -1);

   if(id == 0)
   {
      execvp(myagrv[0], myagrv);
      exit(1);
   }
  
   int status = 0;
   pid_t ret =  waitpid(id, &status, 0);
   assert(ret > 0);
   (void)ret;
   lastCode = (status>>8)&0xff;
   lastSig = status & 0x7f;
  }
  return 0;
}
