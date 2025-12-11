#include <stdio.h>

int main()
{
    FILE* pf = fopen("test.txt", "r"); // rwx r模型，文件必须存在的

    if(pf == NULL)
    {
        perror("fopen failed");
        return 1;
    }

    char arr[10] = {0};
    while((fgets(arr, 10, pf)) != NULL)
    {
        printf("%s", arr);
    }
    fclose(pf);
    pf = NULL;
    return 0;
}