#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main()
{
  
  umask(0);
  int fd = open("log.txt",O_RDONLY);
  if(fd < 0)
  {
    perror("open failed");
    return 1;
  }

  dup2(fd, 0); // 输入重定向

  char line[64];
  while(1)
  {
    if(fgets(line, sizeof(line), stdin) == NULL) // stdin read 
    {
      break;
    }
    printf("%s", line);
  }
  
  close(fd);
  return 0;
}
