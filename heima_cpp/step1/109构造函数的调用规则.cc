#include <iostream>
using namespace std;

//1.创建一个类，c++编译器给每一个类添加三个函数
// 1.默认构造函数 空实现 调用其它类
// 2.默认析构函数 空实现
// 3.默认拷贝构造

class person
{
public:
    person()
    {
        _age = 10;
        cout<< "person的无参构造函数" <<endl;
    }

    person(int age)
    {   
        _age = age;
        cout<< "person的有参数构造函数" <<endl;
    }

    // person(const person& that)
    // {
    //     _age = that._age;
    //     cout<< "person的拷贝构造函数" <<endl;
    // }

    ~person()
    {
        cout<< "析构函数" <<endl;
    }

int _age;
};

void test1()
{
    person p1;
    p1._age = 44;
    person p2(p1);
    cout<< "p2的年龄" <<endl;
}


int main()
{


    test1();







    return  0;
}