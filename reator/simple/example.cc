#include <unistd.h>
#include <stdio.h>


int main()
{
  char buffer[1024] = {0};
  chdir("./test");
  
  getcwd(buffer, sizeof(buffer));
  printf("%s \n", buffer);




  return 0;
}
