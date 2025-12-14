#include <iostream>
using namespace std;

class Person
{
public:
    Person()
    {
        cout<< "Person的构造函数" <<endl;
    }

    ~Person()
    {
        cout<< "Person的析构函数" <<endl;
    }


};

void test1()
{
    Person p;
}

int main()
{
    test1();
    Person p;








    return  0;
}