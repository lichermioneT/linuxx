#include <iostream>
using namespace std;

void print(const int&  a)
{
    cout<< "a = " << a <<endl;
}


int main()
{
// 常量引用，防止被误修改的
    // 这里的临时空间的别名不知道了
    // 临时变量具有常属性的
    const int&  ref = 10; // int temp = 10; const int& ref = temp;
    print(ref);

    return 0;
}