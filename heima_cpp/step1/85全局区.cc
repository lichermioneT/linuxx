#include <iostream>
using namespace std;

int g = 10;
const int c_g = 100;

int main()
{
    static int s_a = 10;
    const int  c_l_a = 100;
    int a = 10;
    int b = 10;
    cout<< "局部常量的地址   : " << &c_l_a <<endl;
    cout<< "局部变量a的地址 : " << &a <<endl;     
    cout<< "局部变量b的地址 : " << &b <<endl;     
    cout<< "------------" <<endl;
    static int c = 10;

    cout<< "全局变量c的地址 : " << &g <<endl;     
    cout<< "静态变量c的地址 : " << &c <<endl;

    // 字符串常量
    cout<< "字符串常量的地址 : " << &"hello" <<endl;
    cout<< "全局常量的地址   : " << &c_g <<endl;
    cout<< "局部静态的地址   : " << &s_a <<endl;


// const修饰的局部变量
// 局部变量

// static修饰的变量
// 全局变量
// 常量：字符串常量， const修饰的全局常量
    return 0;
}