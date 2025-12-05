#include <iostream>
using namespace std;

int& func()
{
    static int a = 10; // 全局区域的数据，由系统释放的
    return a;
}

int main()
{

// 1.不返回局部变量的引用
// 2.函数的调用可以作为左值

// 引用的返回值作为左值

    int& c = func();
    c += 10;
    c += 10;
    cout<< c <<endl;

    func() = 1000;
    int d = func();


    cout<< d <<endl;
    return 0;
}