#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main()
{
  

  
  umask(0);
  int fd = open("log.txt",O_WRONLY | O_CREAT | O_APPEND, 0666);
  if(fd < 0)
  {
    perror("open failed");
    return 1;
  }

  // 现在1的位置是 fd了。现在输出数据是往fd里面了，而不是stdout(显示器了)
  dup2(fd, 1);


  
  printf("open fd %d \n", fd); // 往stdout里面输出数据
  fprintf(stdout, "open fd : %d \n", fd);

  const char*msg = "hello world";
  write(1,msg, strlen(msg));
  fflush(stdout);





  
  close(fd);
  return 0;
}
