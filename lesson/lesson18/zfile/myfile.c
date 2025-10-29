#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define FILE_NAME "log.txt"

int main()
{

/*
  printf("dafdsf");
  printf("dafdsf");
  printf("dafdsf");
  printf("dafdsf");
  printf("dafdsf");
*/ 

//  FILE* fp = fopen(FILE_NAME, "w"); // r w r+(读写， 不存在出错)，w+(读写，不存在创建) a(append 追加) a+()
//  FILE* fp = fopen(FILE_NAME, "r"); // r w r+(读写， 不存在出错)，w+(读写，不存在创建) a(append 追加) a+()
                              
  FILE* fp = fopen(FILE_NAME, "a"); // r w r+(读写， 不存在出错)，w+(读写，不存在创建) a(append 追加) a+()
  if(fp == NULL)
  {
    perror("fopen");
    return 1;
  }

/*
  char buffer[64];
  while(fgets(buffer, sizeof(buffer) - 1, fp))
  {
      buffer[strlen(buffer) - 1] = 0;
       puts(buffer); 
  }  
*/


  
  int cnt = 5;
  while(cnt)
  {
    fprintf(fp, " %s : %d \n", "hello lic", cnt--);
  } 






  fclose(fp);






  return 0;
}
