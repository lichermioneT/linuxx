#include <stdio.h>

int main()
{
    FILE* pf = fopen("test.txt", "a"); // rwx r模型，文件必须存在的

    if(pf == NULL)
    {
        perror("fopen failed");
        return 1;
    }
    fputs("hell lic\n", pf);

    fclose(pf);
    pf = NULL;
    return 0;
}