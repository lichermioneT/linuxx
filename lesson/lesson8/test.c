#include<stdio.h>
#include<unistd.h>

int main()
{
  
  // 毫无怀疑的是，一定执行的是printf
  // 这是一个顺序结构的
  //
  // 只不过该数据没有被显示出来
  // sleep2才显示出来的
  // sleep2期间 printf已经完成打印了，那么对应的数据在哪里呢？
  // 缓冲区里面
  // 现在无法解释系统级别的缓冲区
  // \n行缓冲区域的
  // printf("you can see me..........\n");
  

  int cnt = 12;
  while(cnt)
  {
    //printf("%d \r",cnt);
    printf("剩余时间：%2d\r",cnt); // 一次刷新两个字符
    fflush(stdout);
    cnt--;
    sleep(1);
  }
  return 0;
}


