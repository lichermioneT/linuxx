#include <iostream>
using namespace std;

void func(int a, int )
{
    cout<< "this is func" <<endl;
}

void func2(int a, int = 10)
{
    cout<< "this is func" <<endl;
}

int main()
{
// 站位参数
// 站位参数的默认参数
    func(10, 10);
    return 0;
}