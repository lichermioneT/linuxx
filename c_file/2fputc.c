#include <stdio.h>

int main()
{
    FILE* pf = fopen("test.txt", "r");
    if(pf == NULL)
    {
        perror("fopen failed");
        return 1;
    }

// 写一个字符
    for(int i = 'a'; i <= 'z'; i++)
    {
        fputc(i,pf);
    }

    char ch = 0;
    while((ch = fgetc(pf)) != EOF)
    {
        printf("%c \n", ch);
    }

    fclose(pf);
    pf = NULL;
    return 0;
}