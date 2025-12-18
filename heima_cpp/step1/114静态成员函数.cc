#include <iostream>
using namespace std;

class person
{
public:
    static void func()
    {
        cout<< a <<endl;
        cout<< "static void func()" <<endl;
    }

    static int a;
    int b;
};

int person::a = 10;


int main()
{

// 共享一个
// 类名和对象调用
    person p;
    p.func();

    person::func();

    return  0;
}