#include <stdio.h>

struct Person
{
    int age;
    char buffer[20];
    float score;
};

int main()
{
    FILE* pf = fopen("test.txt", "r"); // rwx r模型，文件必须存在的

    if(pf == NULL)
    {
        perror("fopen failed");
        return 1;
    }

    struct Person s;

    fread(&s, sizeof(s), 1, pf);
    printf("%d %s %d \n", s.age, s.buffer, s.score);

    fclose(pf);
    pf = NULL;
    return 0;
}