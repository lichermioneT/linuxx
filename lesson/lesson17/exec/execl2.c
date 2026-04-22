#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/wait.h>

int main()
{
  printf("process is running ...\n");
  /*
   *execl("/usr/bin/ls","ls", "-a", "-l","-h", "/usr/bin/", NULL);
   */
  execl("/usr/bin/top","top", NULL);
  // 第一个参数，可执行文件的位置
  // 第二个参数，终端是如何执行的
  // 第三个参数，NULL结尾
  return 0;
}
