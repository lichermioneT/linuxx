

#include <iostream>
using namespace std;

class person
{
public:
    person()
    {
        _age = 10;
        cout<< "person的无参构造函数" <<endl;
    }

    person(int age, int height)
    {   
        _age = age;
        _height = new int(height);
        cout<< "person的有参数构造函数" <<endl;
    }

    person(const person& that)
    {
        _age = that._age;
        // _height = that._height;  默认的拷贝构造函数
        _height =  new int(*that._height); // 重新开辟空间的
        cout<< "person的拷贝构造函数" <<endl;
    }

    ~person()
    {
        cout<< "析构函数" <<endl;
        delete _height;
    }

int _age;
int* _height;
};

void test1()
{
    person p1(10, 45);
    person p2(p1);
}

int main()
{
    test1();

// 浅拷贝会释放同一块空间两次

    return  0;
}