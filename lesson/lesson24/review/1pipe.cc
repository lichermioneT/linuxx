#include <iostream>
#include <cstring>
#include <unistd.h>
using namespace std;

int main()
{
  int fds[2];
  int n = pipe(fds);
  if(n == -1)
  {
    cerr<< "pipe:" << strerror(errno) <<endl;
    return 1;
  }

  cout<< fds[0] << endl; // read 
  cout<< fds[1] << endl; // write

  return 0;
}
