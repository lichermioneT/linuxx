#include <iostream>
using namespace std;


int main()
{
//1.引用必须初始化
//2.初始化不能修改了

// 引用单词就是用个别名，语法实际上不占空间的(实际还是占空间的)
    int a = 10;
    int& b = a;
    int& c = b;
    c = 100;
    c = b;      // 这里实际上是赋值，不是别名重新指向
    
    cout<< "a = " << a <<endl;
    cout<< "b = " << b <<endl;
    cout<< "c = " << c <<endl;


    return 0;
}