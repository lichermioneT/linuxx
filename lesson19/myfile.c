#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main()
{
  
  // close(0);
  close(1);
  // close(2);

  
  umask(0);
  int fd = open("log.txt",O_WRONLY | O_CREAT | O_TRUNC, 0666);
  if(fd < 0)
  {
    perror("open failed");
    return 1;
  }
  
  printf("open fd %d \n", fd); // 往stdout里面输出数据
  fprintf(stdout, "open fd : %d \n", fd);
  fflush(stdout);





  
  close(fd);
  return 0;
}
