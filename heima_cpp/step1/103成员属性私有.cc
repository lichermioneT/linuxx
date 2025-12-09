#include <iostream>
#include <string>
using namespace std;

// 成员属性设置私有
class Person
{
public:
    void set_name(string name)
    {
        m_name = name;
    }

    string get_nmae()
    {
        return m_name;
    }

    int get_age()
    {
        
        return m_age;
    }

    int set_idol(string idol)
    {
        m_idol = idol;
    }

private:
    string m_name;      // 可读可写
    int m_age = 10;     // 只读
    string m_idol;      // 只写
};

int main()
{

// get
// set
// 自己控制权限：读写
// 然后控制是否合理的
    return  0;
}