#include <iostream>
#include "point.h"
#include "circle.h"
using namespace std;

void isInCircle(circle& c, Point& p)
{
    int distance = (c.getCenter().get_X() - p.get_X() ) *(c.getCenter().get_X() - p.get_X()) +
                    (c.getCenter().get_Y() - p.get_Y() ) *(c.getCenter().get_Y() - p.get_Y());
    int rDistance = c.getr() * c.getr();

    if(distance == rDistance)
    {
        cout<< "圆上" <<endl;
    }
    else if(distance > rDistance)
    {
        cout<< "圆外" <<endl;
    }
    else 
    {
        cout<< "圆内" <<endl;
    }

}

// g++ 105圆和点.cc circle.cc point.cc -o main
// 编译指令的
int main()
{

// 点到圆心的坐标；
    circle c;
    c.setR(10); //               

    Point center;
    center.set_x(10);
    center.set_Y(0);
    c.setCenter(center);


    Point p;
    p.set_x(10);
    p.set_Y(11);
    isInCircle(c, p);
    return  0;
}