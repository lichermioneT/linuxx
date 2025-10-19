#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
  
  FILE* fp = fopen("log.txt", "r"); // r只是读模式打开，没文件会出现报错， w,a没有会创建文件的
  if(fp == NULL)
  {
    perror("fopen failed");
    exit(1);
  }
  
  int arr[1024] = {0};
  int* arr1 = arr;

  int cnt = 5;
  int i = 0;
  while(cnt)
  {

   size_t t =  fread(arr1 + i*5 , sizeof(int), 5, fp);
    printf("%zd \n", t);
    cnt--;
    i++;
  }

  for(int i = 0; i < 40; i++)
  {
    printf("%d ", arr[i]);
  }







  fclose(fp);
  return 0;
}
