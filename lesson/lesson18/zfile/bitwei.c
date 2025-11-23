#include <stdio.h>
#include <string.h>
#include <unistd.h>

// 每一个宏，对应的数据值，只有一个比特位是1，位置不重叠
#define ONE   0x1  // (1<<0)
#define TWO   0x2  // (1<<1)
#define THREE 0x4  // (1<<2)
#define FOUR  0x8  // (1<<3)

void show(int flags)
{
  if(flags & ONE )   printf("one    \n");
  if(flags & TWO )   printf("two    \n");
  if(flags & THREE ) printf("three  \n");
  if(flags & FOUR )  printf("four   \n");
}


int main()
{
  
 show(ONE);
 show(TWO);
 show(ONE | TWO);


 return 0;
}
