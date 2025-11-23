#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main()
{
  
   /*
    *close(0);
    */

    close(1); //,标准输出的，显示器 

   /*
    *close(2);
    */

  
  umask(0);
  int fd = open("log.txt",O_WRONLY | O_CREAT | O_TRUNC, 0666);
  if(fd < 0)
  {
    perror("open failed");
    return 1;
  }
  
  printf("open fd %d \n", fd); 
  fprintf(stdout, "open fd %d \n", fd); 
  fflush(stdout);
  
  close(fd);
  return 0;
}
