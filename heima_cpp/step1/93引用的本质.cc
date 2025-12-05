#include <iostream>
using namespace std;

void func(int& ref) // int* const ref 
{
    ref = 100; // *ref = 100;
}

int main()
{

// 指针常量
// 一定指向了不能进行改变的 
// int* const p = &a
    int a = 10;

    int& ref = a; // int* const ref = &a;
    ref = 20;      // *ref = 20;
    
    cout<< "a = " << a <<endl;
    cout<< "ref = " << ref <<endl;

    func(ref);
    cout<< "a = " << a <<endl;
    cout<< "ref = " << ref <<endl;

    return 0;
}