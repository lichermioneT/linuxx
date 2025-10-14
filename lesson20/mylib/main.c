#include "myStdio.h"

int main()
{

  FILE_* fp = fopen_("./log.txt","w");

  if(fp == NULL)
  {
    return 1;
  }

  const char* msg = "helllo \n";
  
  int cnt = 18;
  while(cnt)
  {
    fwrite_(msg, strlen(msg), fp);
    sleep(1);
    if(cnt % 3 == 0) fflush_(fp);
    cnt--;
  }
  
  fclose_(fp);




  return 0;
}
