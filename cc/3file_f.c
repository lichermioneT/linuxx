#include <stdio.h>


int main()
{

  FILE* fp = fopen("test.txt", "a");

  fprintf(fp, "%s %d \n", "lichermionex", 222);
  
  return 0;
}
