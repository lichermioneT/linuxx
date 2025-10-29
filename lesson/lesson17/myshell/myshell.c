#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <assert.h>

#define NUM 1024
#define OPT_NUM 64


char linecommand[NUM];
char* myagrv[OPT_NUM];

int main()
{

  
  while(1)
  {

  printf("用户名@主机名 当前路径# ");
  fflush(stdout);
  sleep(10);

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
  while(myagrv[i++] = strtok(NULL, " "))
  {
    ;
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

   waitpid(id, NULL, 0);

  }

  return 0;
}
