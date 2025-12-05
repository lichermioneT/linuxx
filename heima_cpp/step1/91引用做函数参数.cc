#include <iostream>
using namespace std;

void swap1(int a, int b)
{
    int c = a;
    a = b;
    b = c;
}

void swap2(int* p1, int* p2)
{
    int temp = *p1;
    *p1 = *p2;
    *p2 = temp;
}

void swap3(int& p1, int& p2)
{
    int temp = p1;
    p1 = p2;
    p2 = temp;
}

int main()
{
// 1.值传递
// 2.地址传递
// 3.引用传递

    int a = 10;
    int b = 20;
    swap1(a ,b);
    cout<< "a = " << a <<endl;
    cout<< "b = " << b <<endl;

    swap2(&a, &b);
    cout<< "a = " << a <<endl;
    cout<< "b = " << b <<endl;
    swap3(a, b);
    
    cout<< "a = " << a <<endl;
    cout<< "b = " << b <<endl;
    return 0;
}