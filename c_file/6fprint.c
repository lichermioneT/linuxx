#include <stdio.h>
#include <string.h>

struct Person
{
    int age;
    char buffer[20];
};

int main()
{
    FILE* pf = fopen("test.txt", "w"); 
    // 写模式：文件不存在会自动创建；存在会被清空

    if(pf == NULL)
    {
        perror("fopen failed");
        return 1;
    }

    struct Person zs;
    zs.age = 10;
    strcpy(zs.buffer, "zhangsan");

    // 往文件写入数据
    fprintf(pf, "%d %s", zs.age, zs.buffer);

    fclose(pf);
    pf = NULL;

    return 0;
}
