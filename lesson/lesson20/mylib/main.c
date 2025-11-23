#include "myStdio.h"

int main()
{

  FILE_* fp = fopen_("./log.txt","w");

  if(fp == NULL)
  {
    return 1;
  }

  /*
   *const char* msg = "helllo \n";
   *fwrite_(msg, strlen(msg), fp);
   */



  int cnt = 10;
  const char* msg = "helllo \n";
  while(1)
  {
    cnt--;
    fwrite_(msg, strlen(msg), fp);
    sleep(1);
    if(cnt == 0)break;
  }
  
  fclose_(fp);
  return 0;
}
