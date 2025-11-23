#include <iostream>
using namespace std;

//1.类的引入
  // 类的成员属性  变量
  // 类的成员方法  函数

//2.访问限定符

class Person
{
public:
  double _scoer;

protected:
  int _weight;

private:
  char _name[20];
  int _age;
  int _stuid;
};

class Stack
{
public:
  void Push(){}
  void Pop(){}
  bool Empty();

private:
  int* _a;
  size_t _size;
  size_t _capacity;
};

bool Stack::Empty()
{
  return false;
}

// c/c++ struct
struct ListNode_c
{
  int val;
  struct ListNode_c* _next;
  struct ListNode_c* _prev;
};

struct ListNode_cpp
{
  int val;
  ListNode_cpp* _next;
  ListNode_cpp* _prev;
  ListNode_cpp* create(int val);
};

class A
{
private:
  char _ch;
  int _a;
};


class Date
{
public:
  void Init(int year, int month, int day) // void Init(Date* this, int year, int month, int day)
  {
    _year = year;
    _month = month;
    _day = day;
  }

  void print() // print(Date* this)
  {
    cout<< _year << "-" << _month << "-" << _day <<endl;
  }


private:
  int _year;
  int _month;
  int _day;
};


int main()
{

//声明和实例化
//访问限定符，限制外面的访问  
  
// 类的对象只是存储成员属性，不存储成员方法的。
// 成员属性存放在对象里面的。
// 成员属性在代码段的。
  /*
   *cout<< sizeof(A) <<endl;
   */

// 成员方法都是一样的，处理不同对象的数据。
// 空类和只存在成员方法的类，一个字节。 站位表示对象的存在。

// 隐含的this指针
// 谁调用成员函数，就执行谁。
  Date d;
  d.Init(22,22,22); // d.Init(&d, 22, 22, 22);
  d.print();        // d.print(&d);

// this指针在栈区





  return 0;
}
