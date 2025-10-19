#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

int main()
{
  char buffer[1024]={0};

  ssize_t s = read(0, buffer, sizeof(buffer));
  if(s > 0)
  {
    buffer[s] = 0;
    write(1,buffer,sizeof(buffer));
    write(2,buffer,sizeof(buffer));
  }










  return 0;
}
