#pragma once
#include "circle.h"
#include "point.h"

class circle
{
public:
    void setR(int r);
    void setCenter(Point center);
    int  getr();
    Point getCenter();
    
private:
    int m_R;
    Point m_Center; // 类内的成员
};