#pragma once
class Point
{
public:
    void set_x(int x);
    void set_Y(int y);
    int  get_Y();     
    int  get_X();      
private:
    int m_X;
    int m_Y;
};