#include <iostream>
#include <iterator>
#include <memory>
#include <string>
#include <list>
#include <deque>
#include <vector>
#include <algorithm>
using namespace std;

/*
void test()
{

 auto_ptr<string> ps(new string("lichermionex"));
 cout<< *ps <<endl;
 cout<< ps->size() <<endl;
}
*/ 

template<class T>
class Auto_ptr
{
  public:
explicit Auto_ptr(T* p = 0):pointee(p){}

  template<class U>
  Auto_ptr(Auto_ptr& rhs):pointee(rhs.release()){}
  ~Auto_ptr(){delete pointee;}
  
  template<class U>
  Auto_ptr<T>& operator=(Auto_ptr<U>& rhs)
  {
    if(this != &rhs)
    {
      reset(rhs.release());
      return *this;
    }
  }

  T* release() // 释放空间
  {
    T* old_ptr = pointee;
    pointee = nullptr;
    return old_ptr;
  }

  void reset(T* p = nullptr) // 指针重新指
  {
    if(pointee != p)
    {
      delete pointee;
      pointee = p;
    }
  }

  T& operator*() const {return *pointee;}
  T* operator->() const {return pointee;}
  T* get() const {return pointee;}
private:
  T* pointee;  
};

// 1.iterator
template<class T>
class listitem
{
public:
  using value_type = T;

  listitem(const T& val):_value(val), _next(nullptr){}
  listitem(const T& val, listitem* next):_value(val), _next(next) {}
  void set_next(listitem* next) {_next = next; }
public:
  T value()const {return _value;}
  listitem* next() const {return _next;}

private:
  T _value;
  listitem* _next;
};


template<class T>
class List 
{
public:
  void insert_front(T value)
  {
    listitem<T>* newNode = new listitem<T>(value, _front);

    if(_front == nullptr)
    {
      _front = _end = newNode;
    }
    else 
    {
      _front = newNode;
    }
    
    _size++;
  }

  void insert_end(T value)
  {
    listitem<T>* newNode  = new listitem<T>(value);
    if(_front == nullptr)
    {
      _front = _end = newNode;
    }
    else 
    {
      _end->set_next(newNode);
      _end = newNode;
    }
  }

  void display(std::ostream& os = std::cout) const 
  {
      listitem<T>* cur = _front;
      while(cur != nullptr)
      {
        os<< cur->value() <<endl;
        cur = cur->next();
      }
  }
  
  listitem<T>* front()
  {
    return _front;
  }

public:
  List(long size = 0, listitem<T>* end = nullptr, listitem<T>* front = nullptr)
    :_size(size)
    ,_end(end)
    ,_front(front)
    {}
private:
  long _size;
  listitem<T>* _end;
  listitem<T>* _front;
};

template<class item>
struct listiter 
{
    // 添加迭代器特性定义
  using iterator_category = std::forward_iterator_tag;
  using value_type = typename item::value_type;  // 假设item有value_type定义
  using difference_type = std::ptrdiff_t;
  using pointer = value_type*;
  using reference = value_type&;

  item* ptr;
  listiter(item* p = 0):ptr(p){}

  item& operator*() const {return *ptr;}
  item* operator->() const {return ptr; }

  listiter& operator++(){ptr = ptr->next; return *this;}
  listiter operator++(int) {listiter temp = *this; ++*this; return temp;}

  bool operator==(const listiter& i) const 
  {
    return ptr == i.ptr;
  }
  
  bool operator!=(const listiter& i) const 
  {
    return ptr != i.ptr;
  }
};



int main()
{

// 迭代器最重要的就是重载
// operator* operator->

  //test();

  List<int> mylist;
  for(int i = 0; i < 10; i++)
  {
    mylist.insert_end(i);
  }

  mylist.display();
  
  listiter<listitem<int>> begin(mylist.front());
  listiter<listitem<int>> end;
  listiter<listitem<int>> iter;

  iter = find(begin, end, 3);
  if(iter == end)
  {
    cout<< " not found "<<endl;
  }
  else 
  {
    cout<< " found." << iter->value() <<endl;
  }


  return 0;
}
