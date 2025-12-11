#include <stdio.h>

int main()
{
    FILE* pf = fopen("test.txt", "r");
    if(pf == NULL)
    {
        perror("fopen failed");
        return 1;
    }

// 读一个字符
// 读失败和文件结尾
    char ch = 0;
    while((ch = fgetc(pf)) != EOF)
    {
        printf("%c \n", ch);
    }

    fclose(pf);
    pf = NULL;
    return 0;
}