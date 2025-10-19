#include <stdio.h>
#include <string.h>

int main()
{

  const char* msg = "hello lic\n";
  printf("%s",msg);
  fwrite(msg, strlen(msg), 1, stdout);
  fprintf(stdout, "%s", msg);




  return 0;
}
