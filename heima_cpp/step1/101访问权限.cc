#include <iostream>
#include <string>
using namespace std;


// 访问权限：public,protected,private;
// public:类外类内 可以访问
// protected:类外不行，类类内可以的. 儿子可以发访问保护
// private:类外不行，类类内可以的。  儿子不可以访问私有

class person
{
public:
    string m_name;
protected:
    string m_car;
private:
    int m_Passwd;

public:
    void func()
    {
        m_name = "lic";
        m_car = "tesla";
        m_Passwd = 111;
    }

};

int main()
{
// 公开的属性和方法可以访问的
    person p1;
    p1.func();
    p1.m_name = "lic11";
    return  0;
}