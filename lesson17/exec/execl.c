#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/wait.h>

int main()
{


/*
  printf("process is running ...\n");
  

  // 成功了，和接下来的代码毫无关系了
  // 只需处理错误就行了
  
  execl("/usr/bin/lsaaa","ls", "-a", "-l","-h", NULL);
  // execl("/usrr/bin/top", "top", NULL);

  // 为什么printf没有执行了 
  // printf也是代码，是在execl之后的
  // execl执行完毕的时候，代码已经被完全覆盖了，开始执行新的代码了，所以printf无法执行了，
  
  

  printf("process running dome ...\n");

  perror("execl");

  exit(-1);

*/


  pid_t id = fork();
  assert(id != -1);

  if(id == 0)
  {
    // 这里的替换不会影响父进程的
    sleep(1);

  
    char* const argv_[] = {"ls", "-a", "-l",  NULL };
      
    // execl("/usr/bin/lsaaa", "ls", "-a", "-l", "-h","--color=auto", NULL);
    // execlp("ls", "ls", "-a", "-l", "-h","--color=auto", NULL);
    execv("/usr/bin/ls", argv_);

    // 成功了 下面的exit(1)不会执行的
    // 失败了 下面的exit(1)会执行的
    exit(1);
  }
  
  int status = 0;
  pid_t ret = waitpid(id, &status,0);

  if(ret > 0)
  {
    printf("wait success, exit code %d , sig %d \n", (status>>8)& 0xFF, status & 0x7F);
  }

































  return 0;
}
