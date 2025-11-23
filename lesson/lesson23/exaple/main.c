#include "myadd.h"
#include "mysub.h"



int main()
{

  int a = 10;
  int b = 33;

  int ret = add(a, b);
  printf("ret : %d\n", ret);
  ret = sub(a, b);
  printf("ret : %d\n", ret);




  return 0;
}
