#include<iostream>
#include<string>
using namespace std;

class student
{
public:
    string m_mame;
    int  m_id;

    void show_info()
    {
        cout<< "姓名： " << m_mame << "-" << "id: " << m_id <<endl;
    }

    void set_name(string name)
    {
        m_mame = name;
    }

    void set_id(int id)
    {
        m_id = id;
    }

// 成员属性
// 成员方法
};

int main()
{
    student s;
    s.set_id(11);
    s.set_name("lichermionex");
    s.show_info();
    return 0;
}