#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>


int main()
{
  umask(0);
  int fd = open("myfile", O_WRONLY | O_CREAT, 0644);

  if(fd < 0)
  {
    perror("open failed");
    exit(1);
  }
  
  int cnt = 5;
  const char* msg = "hello lic \n";
  size_t len = strlen(msg);

  while(cnt != 0)
  {
    write(fd, msg, len);
    cnt--;
  }
  

  close(fd);








  return 0;
}
