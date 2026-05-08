#include <iostream>
#include <cstdio>
#include <string>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>



int main(int argc, char* argv[])
{
  if(argc != 3)
  {
    std::cout<< "./a.out pid sig" << std::endl;
    exit(2);
  }

  int pid = atoi(argv[1]);
  int sig = atoi(argv[2]);

  int n = kill(pid, sig);
  if(n != -1)
  {
    perror("kill");
    exit(12);
  }

  return 0;
}
