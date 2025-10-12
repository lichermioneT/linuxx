#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main()
{



if(0)
{

  pid_t id = fork();
  
  if(id == 0)
  {
    printf("hello exit  ");
    exit(0);
  }
  else if(id > 0)
  {
  
    printf("hello _exit  ");
    _exit(0);
  }


}

  pid_t id = fork();
  
  if(id  < 0)
  {
    exit(EXIT_FAILURE);
  }
  else if(id == 0)
  {
    printf("hell \n");
    exit(EXIT_SUCCESS);
  }
  else 
  {
    printf("asdf \n");
    exit(EXIT_SUCCESS);
  }


  return 0;
}
