#include <iostream>
using namespace std;

class Person
{
public:
    Person()
    {
        cout<< "Person的默认构造函数" <<endl;
    }

    Person(int age)
    {
        cout<< "有参数构造" <<endl;
        _age = age;
    }
    Person(const Person& that)
    {
        cout<< "拷贝构造" <<endl;
        _age = that._age;
    }

    ~Person()
    {
        cout<< "Person的析构函数" <<endl;
    }


int _age;
};

void test1()
{
    Person p1(23);
    Person p2(p1);
}

void do_work(Person p) // 实际参数给形式参数会进行拷贝的，临时拷贝的
{

}

void test2()
{
    Person p1;
    do_work(p1);
}

Person do_work2()
{
    Person p1;
    return p1;
}

void test3()
{
    Person p = do_work2();
}


int main()
{

//1.已经创建完毕的，初始化新的对象
    // test1();

//2.值传递给函数传参
    // test2(); 

//3.值方式返回局部对象
    test3();








    return  0;
}