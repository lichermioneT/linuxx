#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <assert.h>




int main(int argc, char* agrv[])
{
  printf("process is runing ...\n");
  
  pid_t id = fork();
  assert(id != -1);

  if(id == 0)
  {
    /*
     *execl("/usr/bin/ls", "ls", "-a", "-l", "-h", NULL); // 写时拷贝，新数据和代码执行。新的进程加载新的数据和代码。写时拷贝了。
     */

    /*
     *execlp("ls", "ls", "-a", "-l", "-h", NULL);
     */

/*
 *    char* const argv[] = {
 *      "ls",
 *      "-a",
 *      "-l",
 *      "-h",
 *      NULL
 *    };
 *
 *    execv("/usr/bin/ls", argv);
 */

/*
 *     char* const argv[] = {
 *       "ls",
 *       "-a",
 *       "-l",
 *       "-h",
 *       NULL
 *     };
 *
 *    execvp("ls", argv);
 */


  /*
   *execl("./mybin", "./mybin", NULL);
   */

  // 自己的环境变量
/*
 *    char* const envp_[]={
 *      (char*)"MYENV=1222111111111",
 *      NULL
 *    };
 *
 *  execle("./mybin", "mybin", NULL, envp_); 
 */
  


/*
 *     extern char** environ;
 *     execle("./mybin", "mybin", NULL, environ);
 */

    /*
     *extern char** environ;
     *putenv((char*)"MYENV=1111111111111"); // 添加到环境变量里面
     *execle("./mybin", "mybin", NULL, environ);
     */


  sleep(1);
  execvp(agrv[1], &agrv[1]);

    




    exit(1);
  }

  int status = 0;
  int ret = waitpid(id, &status, 0);
  
  if(WIFEXITED(status))
  {
    printf("exit code : %d \n", WEXITSTATUS(status));
  }
  
  if(WIFSIGNALED(status))
  {
    printf("signal code : %d \n", WTERMSIG(status));
  }

  printf("process is runing ...\n");




  return 0;
}
