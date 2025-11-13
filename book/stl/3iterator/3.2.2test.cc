#include <iostream>
#include <algorithm>
#include <iterator>  // 需要包含这个头文件

using namespace std;

// 1.iterator
template<class T>
class listitem
{
public:
  // 添加 value_type 定义
  using value_type = T;
  listitem(const T& val):_value(val), _next(nullptr){}
  listitem(const T& val, listitem* next):_value(val), _next(next) {}

  void set_next(listitem* next) {_next = next; }
public:
  T& value() {return _value;}  // 返回引用
  const T& value() const {return _value;}  // const版本
  listitem* next() const {return _next;}

private:
  T _value;
  listitem* _next;
};

template<class item>
struct listiter
{
  // 迭代器特性定义
  using iterator_category = std::forward_iterator_tag;
  using value_type = typename item::value_type;
  using difference_type = std::ptrdiff_t;
  using pointer = value_type*;
  using reference = value_type&;

  item* ptr;
  listiter(item* p = 0):ptr(p){}

  reference operator*() const {return ptr->value();}
  pointer operator->() const {return &(ptr->value());}

  listiter& operator++(){ptr = ptr->next(); return *this;}
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

  // 添加 begin() 和 end() 方法
  listiter<listitem<T>> begin() { return listitem<listitem<T>>(_front); }
  listiter<listitem<T>> end() { return listitem<listitem<T>>(nullptr); }

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

int main()
{


  int arr[] = {1, 3, 4, 44,5, 5,555};
  List<int> mylist;

  for(auto & e : arr) 
  {
    mylist.insert_front(e);
  }

  for(auto it = mylist.begin(); it != mylist.end(); ++it)
  {
    std::cout<< *it <<" ";
  }
  std::cout<<std::endl;








  return 0;
}
