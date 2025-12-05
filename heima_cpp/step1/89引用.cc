#include <iostream>
using namespace std;


int main()
{
// 类型& 别名 = 原名
    int a = 10;
    int& b = a;
    int& c = b;
    
    b = 100;
    cout<< "a = " << a <<endl;
    cout<< "b = " << b <<endl;
    cout<< "c = " << c <<endl;
    cout<< "&a = "<< &a <<endl;
    cout<< "&b = "<< &b <<endl;
    cout<< "&c = "<< &c <<endl;


    return 0;
}