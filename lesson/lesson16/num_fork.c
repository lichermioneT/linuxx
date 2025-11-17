#include <unistd.h>
#include <stdio.h>


int main()
{

  int cnt = 0;
  while(1)
  {
    int ret = fork();
    if(ret < 0)
    {
      printf("fork failed, cnt : %d \n", cnt);
    }
    else if(ret == 0) 
    {
        while(1)sleep(1);
    }

    cnt++;
  }




  return 0;
}
