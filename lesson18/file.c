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


  int fd = open(FILE_NAME, O_WRONLY | O_CREAT, 0666);
  
  if(fd < 0)
  {
    perror("open");
    return 1;
  }

  close(fd);
  return 0;
}
