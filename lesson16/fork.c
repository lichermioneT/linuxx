#include<stdio.h>
#include<unistd.h>

int main()
{


  pid_t id = fork();
  //printf("%d \n", a);
  printf("%p \n", &id);
  return 0;
}
