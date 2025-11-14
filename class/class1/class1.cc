#include <iostream>  // 输入输出流
using namespace std;

namespace N 
{
  int a = 10;
  int b = 20;

  int add(const int& a, const int& b)
  {
    return a + b;
  }
}

int main()
{

//1.c++关键字
  cout<< N::add(1, 3) << endl;

  {
    using N::add;
    cout<< add(33, 44) <<endl;
  }

  {
    using namespace N;
    cout << add(33, 55) <<endl;
  }

  // c++的全部都是在std里面的
  std::cout<< "hello c++" <<endl; 














  return 0;
}
