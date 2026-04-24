#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#define FINE_NAME  "log.txt"

int main()
{ 
  // r:read读方式打开文件必须存在的，w:write,a:append，写和追加文件可以不存在的。 w:write文件存在会清空文件的内容的
  // r+:read读写追加，文件必须存在的。w+:write,文件不存在则创建，会覆盖文件的内容。a+:append,文件不存在则创建，文件的内容是追加到文件的后面的。
  // rb:二进制读，wb:二进制写，ab:二进制追加的

  FILE* fp = fopen(FINE_NAME, "rb"); 

  if(fp == NULL)
  {
    perror("fopen");
    return 1;
  }

#if 0
  int arr[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

  int cnt = 10;
  while(cnt)
  {
    fwrite(arr, sizeof(int), sizeof(arr) / sizeof(int), fp);
    --cnt;
  }
  
#else 
 int arr[10] = {0};
 
 size_t ret = fread(arr, sizeof(int), sizeof(arr) / sizeof(int), fp);
 if(ret < 0)
 {
  perror("fread");
  return 1;
 }

 for(int i = 0; i < 10; ++i)
 {
   printf("%d\n", *(arr + i));
 }

#endif

  fclose(fp);
  return 0;
}
