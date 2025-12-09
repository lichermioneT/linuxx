#include <iostream>
using namespace std;

class C1
{
    int m_a;
};


struct C2
{
    int m_a;
};

int main()
{
// 默认的权限不同的
    C1 c1;
    C2 c2;
    c2.m_a = 10;
    return  0;
}