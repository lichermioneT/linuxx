#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
                                     // 写数字得用*b 
  FILE* fp = fopen("log.txt", "wb"); // r只是读模式打开，没文件会出现报错， w,a没有会创建文件的
  if(fp == NULL)
  {
    perror("fopen failed");
    exit(2);
  }
  
  const int arr[5] = {1,2,3,4,5};
  int cnt = 5;
  while(cnt)
  {
    size_t i = fwrite(arr, sizeof(int), 5, fp);
    printf("%zd \n", i);
    cnt--;
  }
  









  fclose(fp);
  return 0;
}
