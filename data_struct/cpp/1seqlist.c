#include <iostream>
#include <cstring>
#include <cassert>

using namespace std;

template<class T>
class seqlist
{
public:
  seqlist()
    :_data(new T[4])
    ,_size(0)
    ,_capacity(4)
  {}

  seqlist(size_t n)
    :_data(new T[n])
    ,_size(0)
    ,_capacity(n)
  {}

  ~seqlist()
  {
    delete[] _data;
    _data = nullptr;
    _size = _capacity = 0;
  }

  seqlist(const seqlist<T>& that)
  {
    _data = new T[that._capacity];
  
    for(size_t i = 0; i < that._size; i++)
    {
      _data[i] = that._data[i];
    }

    _size = that._size;
    _capacity = that._capacity;
  }

/*
  seqlist<T>& operator=(const seqlist<T>& that)
  {
    if(this != &that)
    {
      delete[] _data;
      _data = new T[that._capacity];

      for(size_t i = 0; i < that._size; i++)
      {
        _data[i] = that._data[i];
      }

      _size = that._size;
      _capacity = that._capacity;
    }

    return *this;
  }
*/
  seqlist<T>& operator=(const seqlist<T>& that)
  { 
    if(this != &that)
    {
      seqlist<T> temp(that);
      _swap(temp);
    }
    return *this;
  }

  void Insert(size_t pos, T x)
  {
    assert(pos <= _size);
    CheckCapacity();
    for(size_t i = _size; i > pos; i--)
    {
      _data[i] = _data[i-1];
    }

    _data[pos] = x;
    _size++;
  }

  void Erase(size_t pos)
  {
    assert(pos < _size);
    assert(_size > 0);
    for(size_t i = pos; i < _size - 1; i++)
    {
      _data[i] = _data[i + 1];
    }

    _size--;
  }

  void PushBack(T x)
  {
    Insert(_size, x);
  }
  
  void PopBack()
  {
    Erase(_size - 1);
  }

  void PushFront(T x)
  {
    Insert(0, x);
  }

  void PopFront()
  {
    Erase(0);
  }
private:
  T* _data;
  size_t _size;
  size_t _capacity;
  
  void _swap(seqlist<T>& that)
  {
    std::swap(_data, that._data);
    std::swap(_size, that._size);
    std::swap(_capacity, that._capacity);
  }

  void CheckCapacity()
  {
    if(_size == _capacity)
    {
      size_t newcapacity = _capacity == 0 ? 4 : _capacity * 2;
      T* temp = new T[newcapacity];
      
      for(size_t i = 0; i < _size; i++)
      {
        temp[i] = _data[i];
      }
      
      delete[] _data;

      _data = temp;

      _capacity = newcapacity;
    }
  }
};

int main()
{














  return 0;
}
