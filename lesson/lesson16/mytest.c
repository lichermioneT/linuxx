#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>


int addto(int from, int to)
{
  int sum = 0;
  for(int i = from; i <= to; i++)
  {
    sum += i;
  }

//  exit(21);
  _exit(21);
}

int main()
{
 
  printf("hello lichermionex ");
  addto(1, 100);
  // 先写入stdout的缓冲区里面
  // 数据满了
  // \n
  // 刷新
  // 调用write函数
  
/*
 *  sleep(2);
 *  exit(1);
 *
 *  int ret = addto(0, 100);
 *  (void)ret;
 *
 *  exit(12);
 *
 *  while(1) sleep(1);
 */

  for(int i = 0; i < 200 ; i++)
  {
    printf("%s \n", strerror(i));
  }


  // 我怎么知道它运行的结果怎么样？进程退出码
/*
 *  int num = addto(1, 100);
 *
 *  if(num == 5050)
 *    return 0;
 *  else 
 *    return 1;
 */

  // 不关心直接return0；
  // 关系进程退出码的时候，要返回特定的数据表明特定的错误。
  
  // 推测码的意义0是成功，非零表示失败 !0表示不同的错误。
  // 退出码，都必须有对应的文字描述
  //

  // 进程退出的时候，对应的退出码
  // 标定我们进程执行的结果是否正确
  return 0;
}
