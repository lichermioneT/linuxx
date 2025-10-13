#include <stdio.h>
#include <string.h>



int main()
{

  char arr1[] = "abcdef";
  char arr2[10] = "\0";
  memcpy(arr2, arr1, strlen(arr1));
  
  for(int i = 0; i < 10; i++)
  {
    printf("%c \n", arr2[i]);
  }




  return 0;
}

