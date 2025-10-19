#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int main()
{

 //  close(0);
   close(1);
  //close(2);

  int fd = open("myfile", O_RDWR);
  if(fd < 0)
  {
    perror("open failed");
    exit(1);
  }
  
  const char* msg = "hello lic\n";
  write(fd, msg, strlen(msg));

  printf("%d \n", fd);
  close(fd);









  return 0;
}
