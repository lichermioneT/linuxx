#include <iostream>
using namespace std;

// 求圆的半径
const double PI = 3.14;
class Circle
{
// 属性:变量
// 行为:函数

public:
    int m_r;
    double calculateZC()
    {
        return 2 * PI * m_r;
    }

};

int main()
{
// 类和对象
// 类的属性
// 类的方法

// 封装
    Circle c; // 实例化对象
    c.m_r = 10;
    cout<< "圆的周长 : " << c.calculateZC() <<endl;
    return 0;
}