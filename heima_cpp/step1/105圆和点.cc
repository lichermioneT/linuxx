#include <iostream>
using namespace std;

class Point
{
public:
    void set_x(int x) {m_X = x;}
    void set_Y(int y) {m_Y = y;}
    int  get_Y()      {return m_Y;}
    int  get_X()      {return m_X;}
private:
    int m_X;
    int m_Y;
};

class circle
{
public:
    void setR(int r)
    {
        m_R = r;
    }

    void setCenter(Point center)
    {
        m_Center = center;
    }

    int  getr()
    {
        return m_R;
    }

    Point getCenter()
    {
        return m_Center;
    }

private:
    int m_R;
    Point m_Center;
};

void 

int main()
{

// 点到圆心的坐标；








    return  0;
}