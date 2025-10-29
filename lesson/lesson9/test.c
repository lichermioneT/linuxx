#include<stdio.h>

int main()
{

 const char* ss[] = {"a", "b", "c"};
  
  int i = 0;
  for(i = 0; i < 3; i++ )
  {
    printf("%c \n",*ss[i]);
  }

  for(i = 0; i < 3; i++ )
  {
    printf("%c \n", *ss[i]);
  }

  return 0;
}
