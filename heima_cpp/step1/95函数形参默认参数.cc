#include <iostream>
using namespace std;

int func(int a = 8, int b = 44, int c = 10)
{
    return a + b + c;
}

int func1(int a, int b = 44, int c = 10)
{
    return a + b + c;
}

int func2(int a, int b = 44, int c = 10);

int func2(int a, int b, int c)
{
    return a + b + c;
}


int main()
{
// 自己传入用自己的，不传入用默认的
    cout<< func() <<endl;;
    cout<< func(10) <<endl;;
    cout<< func(10, 88) <<endl;;
    cout<< func(10, 20, 30) <<endl;;

// 1.某个位置有了默认参数，从此位置到结束必须有默认参数。 从右往左
// 函数声明有了默认参数，实现就不需要带参数了。          声明和实现只能有一个有默认参数



    return 0;
}