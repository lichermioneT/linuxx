#include <unistd.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

int main()
{

  pid_t id = fork();
  assert(id >= 0);

  if(id == 0)
  {
    printf("I AM CHILD, I am running  pid : %d\n", getpid());
    sleep(3);
    exit(13);
    printf("I AM CHILD, I am ending  \n");
  }
  else 
  {

// 退出的信号
// exit退出
// termial退出
//
    printf("I am waiting child ,,,,\n");
    int status = 0;
    pid_t child_pid = wait(&status);
   
    // printf("%d \n", (status >> 8) & 0XFF );

// wait if exited
    if(WIFEXITED(status)) // wait if exited退出
    {
      printf("eixt code : %d \n",WEXITSTATUS(status)); // wait exit status
    }

// wait if signaled
    if(WIFSIGNALED(status)) // wait if signaled 退出
    {
      printf("signal code : %d \n", WTERMSIG(status)); // wait terminal signal
    }

    printf("status : %d \n", status);
    printf("child_pid : %d \n", child_pid);
    printf("I am waiting child success \n");
  }

// wait是阻塞式等待，直到子进程退出
// 成果了返回子进程的PID, 失败返回-1
// 输出的信息在，输出型参数里面

// WIFEXITED(status) 
// WEXITSTATUS(status) 

// WIFSIGNALED(status)
// WTERMSIG(status)

// 正常退出的时候退出码就是零
// 不正常退出的时候，退出码在8-15

  return 0;
}

