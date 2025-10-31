#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <assert.h>
#include <stdlib.h>

void hander(int signo)
{
  printf("%d 号信号已经被捕捉了\n", signo);
}

int main()
{

  //  signal(SIGCHLD, hander); // 捕捉子进程的退出信号。这样就清晰了。子进程的退出了
                           // waitpid()缺点子进程太多了，同时退出  while()循环等 waitpid(-1,) -1任意进程
                           // 子进程太多了，只有一部分退出了。waitpid要非阻塞 waitpdi(-1,null, wnohong) 轮询等待子进程
  signal(SIGCHLD, SIG_IGN);
  pid_t id = fork();
  assert(id >= 0);
  if(id == 0)
  {
    sleep(5);
    printf("我是子进程\n");
    exit(1);
  }

  while(1)
    sleep(1);
  return 0;
}
