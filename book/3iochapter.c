#include <unistd.h>
#include <string.h>
#include <stdio.h>

int main()
{
  
  char buffer[1024] = {0};
  snprintf(buffer, sizeof(buffer), "%s", "hello licherimionex \n");
  ssize_t n = write(2,buffer, strlen(buffer));
  printf("%zu \n",n);
  return 0;
}
