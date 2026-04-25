#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
int main()
{
  
  // 用户数据都要经过缓冲区域的
  // 用户接口
  printf("hello printf------------------c\n");
  fprintf(stdout, "hello fprintf--------c \n");
  fputs("hello fputs--------------------c \n", stdout);



  // 系统接口
  const char* msg = "hello elifc-------------linux \n";
  write(1, msg, strlen(msg));
  
  // 执行到这里的时候，还没有刷新数据
  fork();
  
  // stdout默认是行刷新 ，fork之前，三条c函数已经数据输出到显示器了，你的FILE内部，进程进程内部不存在对应的数据了
  // 进行了 > 写入文件不再是显示器了，而是普通文件了，采用的刷新策略全缓冲，c函数虽然有\n， 不足以将stdout缓冲区写满！
  // 数据并没有被刷新。
  //
  // 执行fork时，stdout属于父进程，创建子进程，紧接着就是进程退出。但是谁先退出，一定要进行缓冲区刷新(就是修改)
  // 发生写时拷贝
  // 数据终止会显示两份
  //
  // wirte没有， write没有FILE, 用的是fd， 就没有c提供的缓冲区。

  return 0;
}
