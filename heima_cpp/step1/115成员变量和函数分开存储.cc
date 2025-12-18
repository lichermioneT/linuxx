#include <iostream>
using namespace std;

class person
{
public:
int a;
static int b;
void func1(){}
void func2(){}
static void func3(){}

};
int person::b = 10;

void test01()
{
    person p;
    cout<< sizeof(p) <<endl;
}



int main()
{

// 空对象也有独一无二的内存
    test01();




    return  0;
}