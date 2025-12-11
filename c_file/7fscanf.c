#include <stdio.h>
#include <string.h>

struct Person
{
    int age;
    char buffer[20];
};

int main()
{
    FILE* pf = fopen("test.txt", "r"); 
    // 写模式：文件不存在会自动创建；存在会被清空

    if(pf == NULL)
    {
        perror("fopen failed");
        return 1;
    }

    struct Person s = {0};
    fscanf(pf, "%d %s\n", &(s.age), &(s.buffer));
    printf("%d%s\n", s.age, s.buffer);
    

    fclose(pf);
    pf = NULL;

    return 0;
}
