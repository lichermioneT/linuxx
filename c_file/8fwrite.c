#include <stdio.h>

struct Person
{
    int age;
    char buffer[20];
    float score;
};

int main()
{
    FILE* pf = fopen("test.txt", "w"); // rwx r模型，文件必须存在的

    if(pf == NULL)
    {
        perror("fopen failed");
        return 1;
    }
    struct Person s = {22, "lic", 55.6f};
    fwrite(&s, sizeof(s), 1, pf);


    fclose(pf);
    pf = NULL;
    return 0;
}