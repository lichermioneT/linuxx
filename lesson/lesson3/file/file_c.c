#include<stdio.h>
#include<ctype.h>

int main()
{

// 数据流的初始
    printf("从键盘读取数据，然后输入到显示器 \n");
   // 从键盘读取数据，转换成大写，写入到显示中 
    FILE* pf = fopen("test.txt","r"); 
    int c = 0;
    while((c = fgetc(stdin)))
   //  while((c = fgetc(pf)) != EOF)
    {   
       fputc(c, stdout);
    }

    fclose(pf);

    return 0;
}


