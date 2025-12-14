#include <iostream>
using namespace std;

class Person
{
public:
    Person() // 无参数，默认构造函数
    {
        _age = 100;
        cout<< "person的无参构造函数" <<endl;
    }

    Person(int age) // 有参数构造
    {
        _age = age;
        cout<< "person的构造函数 int age" <<endl;
    }

    // 拷贝构造函数，不写const加引用。
    Person(const Person& that)
    {
        cout<< "Person的拷贝构造函数" <<endl;
        _age = that._age;
    }
    ~Person()
    {
        cout<< "person的析构函数" <<endl;
    }
    int _age;
};

int main()
{

//1.有参数和无参数构造
//2.普通和拷贝构造


//1.括号法
    // Person p1;
    // Person p2(10);
    // Person p3(p2);
    // cout<< p1._age <<endl;
    // cout<< p2._age <<endl;
    // cout<< p3._age <<endl;

    // Person p(); // 函数声明，注意写错了。
    // 调用类的默认构造函数不要用()()();
    // 默认构造函数：编译，全缺省，无参数

//2.显示法
    // Person p1;
    // Person p2 = Person(10); // 有参数构造
    // Person p3 = Person(p2); // 拷贝构造
    // Person(10) Person(p2) 匿名对象，当前行执行结束，立即回收了‘

    // Person(10);
    // cout<< "aa" <<endl;

    // 不要用拷贝构造函数，初始化匿名 对象
    // Person p1;
    // Person(p1); // Person(p1) = Person p1;

// 3.隐式转换法
    Person p1 = 10; // Person p1 = Person(10); 有参数构造
    Person p2 = p1; // Person p2 = Person(p1); 拷贝构造



    return  0;
}