#include <iostream>
#include <string>
using namespace std;

class phone
{
public:
    phone(const char* msg):_name(msg)
    {
        cout<< "手机的构造函数"<<endl;
    }
    void print()
    {
        cout<< "naem " << _name <<endl;
    }

    ~phone()
    {
        cout<< "手机的析构函数"<<endl;
    }
private:
    string _name;
};

class person
{
public:
    person(int age, const char* msg):_age(age),_apple(msg)
    {
        cout<< "ren的构造函数"<<endl;
    }

    void print()
    {
        cout<< "age " << _age <<endl;
        _apple.print();
    }
        
    ~person()
    {
        cout<< "ren的析构函数"<<endl;
    }
private:
    int _age;
    phone _apple;
};



int main()
{

    person p(22,"苹果");
    p.print();








    return  0;
}