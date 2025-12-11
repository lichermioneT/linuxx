#include <stdio.h>

int main()
{
    FILE* pf = fopen("test.txt", "r"); // rwx r模型，文件必须存在的
    // FILE* pf = fopen("test.txt", "w"); // rwx
    // FILE* pf = fopen("test.txt", "x"); // rwx
    if(pf == NULL)
    {
        perror("fopen failed");
        return 1;
    }

    fclose(pf);
    pf = NULL;
    return 0;
}

// 任何一个c程序默认打开三个流，
// stdin
// stdout
// stderr
// 不需要打开，我们就可直接使用