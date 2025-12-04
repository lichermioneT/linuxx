#include <iostream>
using namespace std;

// 栈区的数据，编译管理和开辟和释放
// 局部变量的地址
void func(int c)  // 形式参数也是在栈区的
{   c = 10;
    int a = 10; // 函数里面的局部变量 
}

int main()
{
    func(1); // 建立栈帧，

    return 0;
}