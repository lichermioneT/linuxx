#include <unistd.h>
#include <wait.h>
#include <sys/types.h>


int main()
{

  pid_t id = fork();
  
  
  waitpid(id, NULL, 0);




  return 0;
}
