#include <stdio.h>
#include <unistd.h>

int main()
{


  char arr[100] = {0};
  for(int i = 0; i < 100; i++)
  {
    arr[i] = 'x';
    printf("\r[%-100s ]", arr);
    fflush(stdout);
    usleep(100000);
  }
  
  printf("\n");






  
  return 0;
}
