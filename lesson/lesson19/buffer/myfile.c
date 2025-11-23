#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
int main()
{
  
  // 用户数据都要经过缓冲区域的
  // c接口
  printf("hello printf \n");
  fprintf(stdout, "hello fprintf \n");
  fputs("hello fputs \n", stdout);


  // 系统调用接口
  const char* msg = "hello elifc \n";
  write(1, msg, strlen(msg));
  
  // 执行到这里的时候，还没有刷新数据
  fork();


  return 0;
}
