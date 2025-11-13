#include <stdio.h>
#include <string.h>
#include <unistd.h>

// ./mycmd -a -b -c
// ./mycmd -ab -bc -ac
// ./mycmd -abc

extern char** environ;
int g_val = 100;

int main()
{

  pid_t id = fork();
  if(id < 0)
  {
    perror("fork failed");
    return 1;
  }
  else if(id == 0)
  {
    g_val = 1;
    while(1)
    {
      printf("我是子进程、 g_val：%d , &g_val : %p \n",  g_val, &g_val);
      sleep(1);
    }
  }
  else 
  {
    g_val = 2;
    while(1)
    {
      printf("我是父进程、 g_val：%d, &g_val  : %p \n",  g_val, &g_val);
      sleep(1);
    }
  }





/*
  for(int i = 0; environ[i]; i++)
  {
    printf("%d : %s \n", i, environ[i]);
  }

*/  





/*
  for(int i = 0; env[i]; i++)
  {
    printf("%d : %s\n", i, env[i]);
  }

  return 0;
*/ 
/*
 * 
  if(agrc != 2)
  {
    printf("Usage: \n%s [-a/-b/-c/-ab/-ac/-bc/-abc]\n", argv[0]);
    return 1;
  }

  if(strcmp("-a", argv[1]) == 0)
  {
    printf("功能a\n");
  }
  if(strcmp("-b", argv[1]) == 0)
  {
    printf("功能b\n");
  }
  
  if(strcmp("-c", argv[1]) == 0)
  {
    printf("功能c\n");
  }

  if(strcmp("-ab", argv[1]) == 0)
  {
    printf("功能ab\n");
  }

  if(strcmp("-ac", argv[1]) == 0)
  {
    printf("功能ac\n");
  }

  if(strcmp("-bc", argv[1]) == 0)
  {
    printf("功能bc\n");
  }
  if(strcmp("-abc", argv[1]) == 0)
  {
    printf("功能abc\n");
  }

*/
  return 0;
}


