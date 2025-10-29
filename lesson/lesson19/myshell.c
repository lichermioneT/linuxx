#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <assert.h>
#include <ctype.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>

#define NUM 1024
#define OPT_NUM 64

#define NONE_REDIR   0
#define INPUT_REDIR  1
#define OUTPUT_REDIR 2
#define APPEND_REDIR 3

#define trimSpace(start) do{\
    while(isspace(*start)) ++start;\
}while(0)

int lastCode = 0;
int lastSig = 0;

char linecommand[NUM];
char* myagrv[OPT_NUM];



int redirType = NONE_REDIR;
char* redirFile = NULL;

// 
void commandCheck(char* command)
{
  assert(command != NULL);
  char* start = command;
  char* end = command + strlen(command);
  while(start < end)
  {
    if(*start == '>')
    {
      *start = '\0';
      start++;
      if(*start == '>')
      {
        redirType = APPEND_REDIR;
      }
      else 
      {
        redirType = OUTPUT_REDIR;
      }

      trimSpace(start);
      
      redirFile = start;
      break;
    }
    else if(*start == '<')
    {
      *start = '\0';
      start++;
      trimSpace(start);
      redirType = INPUT_REDIR;
      redirFile = start;
      break;
    }
    else 
    {
      start++;
    }
  }
}



int main()
{

  
  while(1)
  {
  
    // 初始化
    redirType = NONE_REDIR;
    redirFile = NULL;
    errno = 0;
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

   if(0)
   {
      execvp(myagrv[0], myagrv);
      exit(1);
   }

   if(id == 0)
   {
     // 因为命令是子进程执行的，真正重定向的工作一定是子进程来完成的
     // 如何完成重定向， 是父进程要提供的信息、
     // 这里的重定向会影响父进程吗 进程具有独立性，不能影响父进程。拷贝struct_files
     
     switch(redirType)
     {
        case NONE_REDIR: 
          break;
        case INPUT_REDIR:
          {
            int fd = open(redirFile, O_RDONLY);
            if(fd < 0)
            {
              perror("open");
              exit(errno);
            }
            dup2(fd, 0);
          }
          break;
        case OUTPUT_REDIR:
        case APPEND_REDIR:
          {
            int flags = O_WRONLY | O_CREAT;
            if(redirType == APPEND_REDIR) flags |= O_APPEND;
            else flags |= O_TRUNC;

           int fd = open(redirFile, flags, 0666);
            if(fd < 0)
            {
              perror("open");
              exit(errno);
            }

            // 重定向文件已经打开了
            
            dup2(fd, 1);

          }
          break;
        default: printf("bug?\n");
          break;
     }
  
     execv(myagrv[0], myagrv); // 执行程序替换的时候，会不会影响曾经打开的重定向文件，不会的
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
