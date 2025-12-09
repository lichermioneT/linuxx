#include <iostream>
using namespace std;

class Cube
{
public:
    void set_L(int L) { m_L = L; }
    int get_L() const  { return m_L; }
    void set_W(int W) { m_W = W; }
    int get_w() const  { return m_W; }
    void set_H(int H) { m_H = H; }
    int get_H() const  { return m_H; }

    int calculateS()
    {
        return 2 * m_L * m_W + 2 * m_W * m_H + 2 * m_H * m_L;
    }

    int calculateV()
    {
        return m_L * m_W * m_H;
    }

    bool isSame(const Cube &c2)
    {
        if (this->m_L == c2.get_L() && this->m_W == c2.get_w() && this->m_H == c2.get_H())
        {
            return true;
        }
        else
        {
            return false;
        }
    }

private:
    int m_L;
    int m_W;
    int m_H;
};

bool isSame(Cube &c1, Cube &c2)
{
    if (c1.get_L() == c2.get_L() && c1.get_w() == c2.get_w() && c1.get_H() == c2.get_H())
    {
        return true;
    }
    else
    {
        return false;
    }
}

int main()
{

    Cube c1;
    c1.set_L(10);
    c1.set_W(10);
    c1.set_H(10);

    Cube c2;
    c2.set_L(10);
    c2.set_W(10);
    c2.set_H(11);

    cout<< isSame(c1, c2) <<endl;
    cout<< c1.isSame(c2) <<endl;




    return 0;
}