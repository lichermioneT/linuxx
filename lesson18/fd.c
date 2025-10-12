#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <assert.h>

#define FILE_NAME(number) "log.txt"#number

int main()
{
  
  int fd0 = open(FILE_NAME(1), O_WRONLY | O_CREAT | O_ASYNC, 06666);
  int fd1 = open(FILE_NAME(2), O_WRONLY | O_CREAT | O_ASYNC, 06666);
  int fd2 = open(FILE_NAME(3), O_WRONLY | O_CREAT | O_ASYNC, 06666);
  int fd3 = open(FILE_NAME(4), O_WRONLY | O_CREAT | O_ASYNC, 06666);
  int fd4 = open(FILE_NAME(5), O_WRONLY | O_CREAT | O_ASYNC, 06666);
  int fd5 = open(FILE_NAME(6), O_WRONLY | O_CREAT | O_ASYNC, 06666);

  printf("fd %d \n", fd0);
  printf("fd %d \n", fd1);
  printf("fd %d \n", fd2);
  printf("fd %d \n", fd3);
  printf("fd %d \n", fd4);
  printf("fd %d \n", fd5);

  close(fd0);
  close(fd1);
  close(fd2);
  close(fd3);
  close(fd4);
  close(fd5);


  return 0;
}
