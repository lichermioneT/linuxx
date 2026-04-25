#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main()
{
  umask(022);
  int fd = open("log.txt", O_RDWR | O_CREAT | O_APPEND, 0666);
  if(fd == -1)
  {
    perror("open");
    return 1;
  }

  const char* name = "lichermionex-----------linuxxxx\n";
  int cnt = 10;
  while(cnt)
  {
    ssize_t s = write(fd, name, strlen(name));
    if(s < 0)
    {
      perror("write");
      return 1;
    }
    --cnt;
  }

  lseek(fd, 0, SEEK_SET);

  char buferr[64] = {0};
  for(int i = 0; i < 10; ++i)
  {
   ssize_t n = read(fd, buferr, sizeof buferr);
   if(n > 0)
   {
     buferr[n] = 0;
     printf("%s\n", buferr);
   }
  }

  close(fd);
  return 0;
}
