#include <iostream>
using namespace std;

int* func()
{
    int* p = new int(100);  // p还是一个局部变量，放在栈区域的
    cout<< "p在函数里面的地址 : " << p <<endl;
    return p;               // 还是会存放地址，函数返回放在寄存器里面了的，
    
}

int main()
{
// 堆区程序员管理的
    int* p = func();
    cout<< "p在函数外面的地址 : " << p <<endl;
    cout<< *p <<endl;
    return 0;
}