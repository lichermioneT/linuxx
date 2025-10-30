#include <stdio.h>

// 二级指针解引用一次还是指针
// 二级指针解引用两次就是对应的数据了
extern char** environ;

int main()
{

  for(int i = 0; **environ; environ++, i++)
  {
    printf("%d : %s \n", i, *environ);
  }

  return 0;
}
