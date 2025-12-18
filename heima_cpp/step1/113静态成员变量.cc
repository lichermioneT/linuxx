#include <iostream>
using namespace std;

class person
{
public:
    void add()
    {
        a+=1;
    }
    void print()
    {
        cout<< "a " << a <<endl;
    }
public:
    static int a;
};

int person::a = 0;
void test1()
{
    person p1;
    p1.add();
    p1.add();
    p1.add();
    p1.add();

    person p2;
    p2.add();
    p2.add();
    p2.add();
    p2.add();

    p2.print();
    p1.print();
}

void  test2()
{
    person p1;
    p1.add();
    p1.add();
    p1.add();
    cout<< person::a <<endl;

}

int main()
{

//1全部类共享
//2编译的时候就分配空间了
//3类外初始化
    // test1();


// 两种访问方式 
    test2();




    return  0;
}