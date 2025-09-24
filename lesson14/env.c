#include<stdio.h>
#include<string.h>
#include<stdlib.h>


int main()
{

/*
  printf("%s \n", getenv("USER"));
  printf("%s \n", getenv("PWD"));
  printf("%s \n", getenv("PATH"));
*/

// 程序会判断用户身份的信息

/*
  char* who = getenv("USER");
  if(strcmp(who, "root") == 0)
  {
    printf("我是root\n");
  }
  else 
  {
    printf("我不是root \n");
  }
*/
    
/*
  char* myval = getenv("myval");
  if(myval == NULL)
  {
    printf("环境变量不存 \n");
  }
  else 
  {
    printf("环境变量存在： %s\n", myval);
  }
*/

  
  printf("%s \n", getenv("PWD"));





































































































  return 0;
}
