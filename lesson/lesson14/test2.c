#include <unistd.h>
#include <stdio.h>

int g_val = 100;

int main()
{

  pid_t id = fork();
  if(id == -1)
  {
    perror("fork");
    return 1;
  }
  else if (id == 0)
  {
    while(1)
    {
      printf("ppid : %d\t, pid : \t%d, %d : %p\n", getppid(), getpid(), g_val, &g_val);
      --g_val;
      sleep(2);
    }
  }
  else 
  {
    while(1)
    {
      printf("ppid : %d\t, pid : \t%d, %d : %p\n", getppid(), getpid(), g_val, &g_val);
      g_val -= 2;
      sleep(2);
    }
  }

  return 0;
}
