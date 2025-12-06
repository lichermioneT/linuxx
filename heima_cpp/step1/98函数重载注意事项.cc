#include <iostream>
using namespace std;


void func(int& a)
{
    cout<< "func(int&)" <<endl;
}

void func(const int& a)
{
    cout<< "func(const int&)" <<endl;
}

void func2(int a, int b = 10)
{
    cout<< "func2(int a, int b)" <<endl;
}

void func2(int a)
{
    cout<< "func2(int a)" <<endl;
}

int main()
{
// 引用作为函数参数

// 默认参数作为函数重载的
    int a = 10;
    func(a);

    const int b = 10;
    func(b);

    func(10); // int temp = 10; const int& a = temp;


// 默认参数
    // func2(10); 碰到默认参数
    func2(10, 20);

// 默认参数和函数重载出现二义性的，小心。

    
    return 0;
}