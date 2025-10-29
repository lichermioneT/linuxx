#include <stdio.h>


int main()
{

  FILE* fp = fopen("test.txt","r");

  char buffer[1024];
  // while(fgets(buffer, sizeof(buffer), fp))
  while(fgets(buffer, sizeof(buffer), stdin))
  {
    printf("%s", buffer);
  }


  return 0;
}
