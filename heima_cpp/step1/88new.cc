#include <iostream>
using namespace std;

int* func()
{
    int* p = new int(100);
    return p;
}

int main()
{
// 1.new的语法
    int*pp = func();
    cout<< *pp <<endl;
    delete pp;
    int* p = new int(100);
    cout<< *p <<endl;

    int* pr = new int[10];
    for(int i = 0; i < 10; i++)
    {
        *(pr+i) = i;
    }

    for(int i = 0; i < 10; i++)
    {
        cout<< *(pr + i) <<endl;
    }


    delete[] pr;
    delete p;

    return 0;
}