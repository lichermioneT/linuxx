#include <iostream>  // 输入输出流
#include <time.h>
using namespace std;


void swap_c(int* p1, int* p2)
{
  int temp = *p1;
  *p1 = *p2;
  *p2 = temp;
}

void swap_cpp(int& r1, int& r2)
{
  int temp = r1;
  r1 = r2;
  r2 = temp;
}

int Count1()
{
  static int n = 0;
  n++;
  return n;  // 返回一个临时变量，类型就是int. 拷贝了一份数据, 传值返回会产生临时数据的拷贝。
}

int& Count2()
{
  static int n = 0;
  n++;
  return n;  // 返回的就是n
}

struct A
{
  int a[10000];
};

A a;

A& func1()
{
  return a;
}

A func2()
{
  return a;
}

inline void print()
{
  cout<< "hello inline " <<endl;
}



int main()
{

//1.引用
//类型& 别名 = 变量名 
//同一块空间的不同名字，同时还伴随着权限
  // 权限放大了
  //const int a = 10;  // 类型const int 
  //int& b = a; // 类型int 
  
  // 权限缩小
  /*
   *int a = 10;
   *const int& b = a;
   */

// 非const既可以给const, 也可以给非const
// 赋值和引用记得区分清楚
  
/*
 *  int a = 10;
 *  const int* p1 = &a;
 *  int const* p2 = &a;
 *  p1 = p2;
 *
 *  int* const p3 = &a;
 *  *p3 = 100;
 */


//2.引用做函数参数
  
  /*
   *int a = 10;
   *int b = 20;
   *cout<< "交换前 " << "a " << a << " b " << b <<endl;
   *swap_c(&a, &b);
   *cout<< "交换后 " << "a " << a << " b " << b <<endl;
   *swap_cpp(a, b);  // 直接就是操作a, b
   *cout<< "交换后 " << "a " << a << " b " << b <<endl;
   */


//3.引用做返回值
  // 临时变量具有常属性
  // 传值返回会产生 新的空间,拷贝新的数据。 

// 如果返回变量C是一个局部变量时，引用返回是不安全的。
// 一个函数使用引用做返回，必须出了函数，这个变量还在的

// static只会执行一次的

/*
 *  const int& r1 = Count1();
 *  int& r2 = Count2();
 *
 *  r2 += 10;
 *  
 *  Count2();
 *
 *  cout << r2 <<endl;
 */

// 返回的数据必须不仅仅是函数栈帧的局部变量
  
// 指针和引用的区别  
  
//4.内联函数
  // 内联函数直接在调用的地方展开，不去建立栈帧。  
  // c语言使用宏函数
  // c++inline修饰函数
  // inline声明和定义不能分开


//5auto关键字

  //int a = 10;
  //auto b = a;

//// 指针的时候，不需要管带不带*
  //auto* c = &a;
  //auto  d = &a;

//// 引用的时候必须的&
  //auto& e = a;
  
// auto就是在迭代器使用比较方便的


//6范围for
  int array[] = {1,3,4,56,6,6,7,88,0};
  for(auto& e : array)
  {
    cout<< e*2 <<endl;
  }

//7nullptr指针是 (void*)0
//8NULL是零

  int* p1 = NULL;
  int* p2 = nullptr;











  return 0;
}
