#include <iostream>
#include <functional>
using namespace std;

int add(int a, int b, int c)
{
  cout<< "a : " << a <<endl;
  cout<< "b : " << b <<endl;
  cout<< "c : " << c <<endl;
  return  a + b + c;
}

auto f1 = std::bind(add, -1, -2, -3);
auto f2 = std::bind(add, 100, -1, -2);

int main()
{
  
  /*
   *cout<< f1(1,2,3) << endl;
   */
  cout<< f2(-2,-3) << endl;


  return 0;
}
