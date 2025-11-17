#include <iostream>  // 输入输出流
using namespace std;

/*
 *using std::cout;
 *using std::endl;
 */

namespace N 
{
  int a = 10;
  int b = 20;

  int add(const int& a, const int& b)
  {
    return a + b;
  }
}

// 缺省参数
int add(int a, int b = 10)
{
  return a + b;
}

//全缺省
int Func1(int a = 10, int b = 20, int c = 30)
{
  return a + b + c;
}

//半缺省
//缺省部分参数，必须从右到左
//传参数必须从左往右边
int Func2(int a, int b, int c = 44)
{
  return a + b + c;
}


// 函数重载
void print(int x, double y)
{
  cout<< "int    x  = " << x <<endl;
  cout<< "double y = " << y <<endl;
}

void print(double y, int x)
{
  cout<< "double y = " << y <<endl;
  cout<< "int    x  = " << x <<endl;
}

void print(int x, double y, char ch)
{
  cout<< "int    x  = " << x <<endl;
  cout<< "double y  = " << y <<endl;
  cout<< "char   ch = " << ch <<endl;
}




int main()
{

//1.c++关键字
//2.c++namespace
/*
 *  cout<< N::add(1, 3) << endl;
 *
 *  {
 *    using N::add;
 *    cout<< add(33, 44) <<endl;
 *  }
 *
 *  {
 *    using namespace N;
 *    cout << add(33, 55) <<endl;
 *  }
 *
 *  // c++的全部都是在std里面的
 *  std::cout<< "hello c++" <<endl; 
 */


//3.c++/cin 
//4.缺省参数
  /*
   *cout<< add(33) <<endl;
   *cout<< add(33,11) <<endl;
   *cout<< Func1() <<endl;
   *cout<< Func1(0) <<endl;
   */

//5.函数重载
// 函数名相同，参数个数，顺序，类型不同
// 函数名没有相同的
  /*
   *print(3, 44.4); 
   *cout<<endl;
   *print(44.4, 55); 
   *cout<<endl;
   *print(33, 44.5,'z');
   */
  
    /*
     *预处理：取消注释，头文件展开，宏替换，条件编译 test.i 
     *编译：  检查语法，转成汇编代码                 test.s 
     *汇编：  汇编代码转成二进制                     test.o 
     *链接：  将目标文件链接到一起                   test
     *函数调用：函数定义，声明，调用。
     *.h实现  链接：头文件声明了，在源文件调用，链接的时候去实现的源文件链接上。 
     */
  
    //_z_number_函数名_类型首字母 
    // objdump -S 源文件
    // object dump
    
    // extern "C" 修饰函数函数声明(这个函数不支持重载了)
    // c和c++都能够调用 c++实现的库
    
//6引用
//类型& 别名 = 变量

/*
 *  int a = 1;
 *  int& ra = a;
 *  int& b = a;
 *  
 *  cout<< a <<endl;
 *  cout<< ra <<endl;
 *  cout<< b <<endl;
 *  
 *
 *  //1.定义的时候初始化
 *  //2.一个变量可以多个引用
 *  //3.引用只能指向一个实体
 *  
 *  int c = 100;
 *  c = a; // 这是赋值
 */

  

/*
 *  int a = 10;
 *  int& b = a; // b的类型是int;
 *  
 *  const int c = 10;
 *  const int& d = c;
 *  //int& d = c; 不能的 权限
 *
 *  int e = 10;
 *  const int& f = e;
 */
// 权限只能缩小，不能放大的

  // 隐式类型，临时变量具有常属性
  
  //int i = 0;
  //const double& d = i; // i=temp ,temp=d; 临时变量具有常性

  // 函数返回值也是临时变量的









  return 0;
}
