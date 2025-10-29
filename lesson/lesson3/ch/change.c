#include<stdio.h>
#include<ctype.h>

int main()
{

   // 从键盘读取数据，转换成大写，写入到显示中 
    FILE* pf = fopen("f","r"); 
    int c = 0;
    while((c = fgetc(pf)) != EOF)
    {   
       // c =  toupper(c);
       fputc(c, stdout);
    }

    return 0;
}


