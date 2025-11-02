#include <unistd.h>
#include <string.h>
#include <stdio.h>


int main()
{

  char buffer[1024];
  snprintf(buffer, sizeof(buffer), "%s", "hello licherimionex");
  write(1,buffer, strlen(buffer));



  return 0;
}
