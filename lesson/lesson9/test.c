#include<stdio.h>

int main()
{

  
  // 这里全部都是存放的是 "a", "b", "c"的指针
  //
  const char* ss[] = {"a", "b", "c"};
  
  int i = 0;
  for(i = 0; i < 3; i++ )
  {
    printf("%p \n",ss[i]);
  }

  for(i = 0; i < 3; i++ )
  {
    printf("%c \n", *ss[i]);
  }

  return 0;
}
