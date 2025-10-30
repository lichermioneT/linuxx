#include <stdio.h>


int main()
{
  FILE* pf = fopen("test.txt", "r");

// 读取sizeof(buffer) - 1个字符，末尾添加\0
// 成功返回*str  失败返回 NULLL
  char buffer[1024] = {0};
  while(fgets(buffer, sizeof(buffer), pf) != NULL)
  {
    fputs(buffer, stdout);
  }

  fclose(pf);
  return 0;
}
