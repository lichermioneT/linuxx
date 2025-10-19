#include <iostream>
#include <string>
using namespace std;

class Person
{
public:
  void print_basic()
  {
    cout<< "_name:" << _name <<endl;
    cout<< "-age:" << _age <<endl;
  }
protected:
  string _name = "peter";
  int _age = 18;
};

class Teacher : public Person 
{
public: 
  void print_teacher()
  {
    cout<< " -jobid:" << _jobid <<endl;
  }
protected: 
  int _jobid = 1234;
};



int main()
{

/*
  cout << sizeof(Person) <<endl; // 8+4=12 16
  cout << sizeof(string) <<endl; // 8
  cout << sizeof(Teacher) <<endl;

  Person().print_basic();
  Teacher().print_teacher();
*/











  return 0;
}
