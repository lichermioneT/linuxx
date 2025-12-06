#include <iostream>
using namespace std;

void func()
{
    cout<< "func()" <<endl;
}

void func(int a)
{
    cout<< "func(int a)" <<endl;
}

void func(double a)
{
    cout<< "func(double a)" <<endl;
}

void func(double a, int b)
{
    cout<< "func(double a, int b)" <<endl;
}

void func(int a, double b)
{
    cout<< "func(int a, double)" <<endl;
}

int main()
{
// 作用域相同
// 函数参数类型不同的
// 个数，类型，顺序不同的
// 返回值不能作为函数重载的
    func();
    func(10);
    func(10, 8.33);

    
    return 0;
}