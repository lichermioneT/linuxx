#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define FINE_NAME  "log.txt"

int main()
{ 
  // r:read读方式打开文件必须存在的，w:write,a:append，写和追加文件可以不存在的。 w:write文件存在会清空文件的内容的
  // r+:read读写追加，文件必须存在的。w+:write,文件不存在则创建，会覆盖文件的内容。a+:append,文件不存在则创建，文件的内容是追加到文件的后面的。
  // rb:二进制读，wb:二进制写，ab:二进制追加的

  //FILE* fp = fopen(FINE_NAME, "r"); 
  FILE* fp = fopen(FINE_NAME, "w+"); 

  if(fp == NULL)
  {
    perror("fopen");
    return 1;
  }

  int cnt = 10;
  while(cnt)
  {
    int ret = fputc('a', fp);
    printf("%d\n", ret);
    --cnt;
  }

// 文件指针，执行开头的位置
  rewind(fp); 
  cnt = 10;

  while(cnt)
  {
    int ch = fgetc(fp);
    int ret = putchar(ch);
    printf("%d\n", ret);
    --cnt;
  }
  printf("\n");

  fclose(fp);
  return 0;
}
