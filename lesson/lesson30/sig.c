#include <stdio.h>
#include <signal.h>

// volatile:保持内存可见性，不是从寄存器读取数据
volatile int quit = 0;

void hander(int signo)
{
  printf("%d 号信号已经被捕捉了\n", signo);
  printf("quit : %d", quit);
  quit = 1;
  printf("->%d\n", quit);
}

int main()
{

  signal(2, hander);
  while(!quit); // 只是在寄存里面读取数据，眼里只有寄存器数据没有内存的数据
  printf("注意我是正常退出的\n");

  return 0;
}
