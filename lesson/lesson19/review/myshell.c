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

int redirType = NONE_REDIR;
char* redirFile = NULL;

int lastcode = 0;
int lastsig = 0;

char linecommand[NUM];
char* myargv[OPT_NUM];

#define trimSpace(start) do{\
  while(isspace(*start)) ++start;\
}while(0)

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
          redirType = APPEND_REDIR; // 追加重定向
          start++;
        }
        else 
        {
          redirType = OUTPUT_REDIR; // 输出重定向
        }
      
        // 重定向的文件
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
      ++start;
    }
  }
}

int main()
{
// 这里的重定向和shell的实现就是多了一个字符串解析而言
  while(1)
  {
    redirType = NONE_REDIR;
    redirFile = NULL;
    lastcode = 0;
    lastsig = 0;
    errno = 0;
    printf("newshell:");
    fflush(stdout);
   
    // 细节，它最大读取 sizeof(linecommand)-1个字符串，自动补上\0;
    char* s = fgets(linecommand, sizeof(linecommand), stdin);
    assert(s != NULL);
    (void)s;


    // ls -a -b \n, 这里清除\n的
    linecommand[strlen(linecommand) - 1] = 0;
  #if 0
    printf("%s\n", linecommand);
  #endif

    // 检查命令的定向方式
    commandCheck(linecommand);

    myargv[0] = strtok(linecommand, " ");
    if(myargv[0] == NULL)
    {
      continue;
    }

    int i = 1;

    while((myargv[i++] = strtok(NULL, " ")) != NULL)
    {
      ;
    }


    pid_t id = fork();
    assert(id != -1);
    if(id == 0)
    {
      switch(redirType)
      {
        case NONE_REDIR: break;
        case INPUT_REDIR:
        {
            int fd = open(redirFile, O_RDONLY);
            if(fd < 0)
            {
              perror("open");
              return 1;
            }
            dup2(fd, 0);
            break;
        }
        case OUTPUT_REDIR:
        case APPEND_REDIR:
        {
          umask(0);
          int flag = O_WRONLY | O_CREAT;
          
          if(redirType == APPEND_REDIR)
            flag |= O_APPEND;
          else 
            flag |= O_TRUNC;
          
          int fd = open(redirFile, flag, 0666);
          if(fd < 0)
          {
            perror("open");
            return 1;
          }
          
          dup2(fd, 1);
          break;
        }
        default: printf("BUG?\n");
          break;
      }
      
      execvp(myargv[0], myargv);
      perror("execvp");
      _exit(127);
    }
    
    int status = 0;
    pid_t ret = waitpid(id, &status, 0);
    assert(ret > 0);
    (void)ret;
    
    lastcode = (status>>8)&0xff;
    lastsig = status & 0x7f;
  }

  return 0;
}
