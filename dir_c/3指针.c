#include <stdio.h>

size_t mystrlen(const char* str)
{
  const char* end = str;
  while(*end != '\0')
  {
    end++;
  }

  return  end - str;
}


int main()
{

  printf("%zd \n", mystrlen("aa"));







  return 0;
}
