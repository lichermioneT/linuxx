#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <assert.h>

#define FILE_NAME "log.txt"

int main()
{

  umask(0);

  // w -> O_WRONLY | O_CREAT | O_TRUNC
  //  int fd = open(FILE_NAME, O_WRONLY | O_CREAT | O_TRUNC, 0666);
  
  // a
  //  int fd = open(FILE_NAME, O_WRONLY | O_CREAT | O_APPEND, 0666);

  // r
  int fd = open(FILE_NAME, O_RDONLY);


  if(fd < 0)
  {
    perror("open");
    return 1;
  }

  char buffer[1024];
  ssize_t num = read(fd, buffer, sizeof(buffer) - 1);
  if(num > 0)
  {
    buffer[num] = 0; // \0 null '\0' 都是0
  }

  printf("%s", buffer);

/*
  int cnt = 5;
  
  char  outBuffer[64];

  while(cnt)
  {
    sprintf(outBuffer, "%s %d\n", "aaa ", cnt-- );

    // \0作为字符串结束的标志位，是c语言的规定，和我文件有什么关系被？
    write(fd, outBuffer, strlen(outBuffer)); // 需不需要加一  
  }

*/ 
  // printf("fd : %d \n", fd);
  close(fd);
  return 0;
}
