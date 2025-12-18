#include <iostream>
using namespace std;

class person
{
public:
    person(int a, int b, int c)
    :_a(a)
    ,_b(b)
    ,_c(c)
    {}

    int add()
    {
        return _a + _b + _c;
    }

    person& adda(const person& that)
    {
        _a += that._a;
        return *this;
    }

    void printa()
    {
        cout<< " a " << _a <<endl;
    }

private:
    int _a;
    int _b;
    int _c;
};



int main()
{

// 隐含的this指针

    // person p1(1,3,4);
    // cout<< p1.add() <<endl;;

    // person p2(33,4444,56);
    // cout<< p2.add() <<endl;;

    person p1(1,34,5);
    person p2(1,34,5);
    
    p1.adda(p2).adda(p2).adda(p2);
    p1.printa();
    return  0;
}