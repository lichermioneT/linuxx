#include<stdio.h>


int main(int argv, char* argc[], char* env[])
{
  
  int i = 0;
  for(i = 0; *(env + 1); i++)
  {
    printf("%s \n", env[i]);
  }

   for(i = 0; i < argv; i++)
   {
      printf("%s \n", argc[i]);
   }



  return 0;
}
