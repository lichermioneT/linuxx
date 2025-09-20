#include "process.h"

   
void Proncess()
{
  int cnt = 0;
  char bar[NUM];
  memset(bar, '\0', sizeof(bar));
  const char* cursor = "|/-\\"; // 旋转光标符号



  while(cnt <= 100)
  {
    printf("[%-100s][%-3d%%][%c] \r", bar, cnt, cursor[cnt % 5]);
    fflush(stdout);
    bar[cnt] = STYLE;
    cnt++;
    // sleep(1);
    usleep(50000);
  }

  printf("\n");
}

