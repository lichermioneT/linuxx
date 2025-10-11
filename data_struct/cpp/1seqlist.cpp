#include<iostream>
#include<cassert>

using namespace std;

template<class T>
class seqlist
{
public:
  seqlist(size_t n = 4)
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

  seqlist(const seqlist& that)
    :_data(new T[that._capacity])
    ,_size(that._size)
    ,_capacity(that._capacity)
  {
    for(size_t i = 0; i < _size; i++)
    {
      _data[i] = that._data[i];
    }
  }

  seqlist<T>& operator=(const seqlist<T>& that)
  {

    // 先检查自己给自己拷贝
    if(this != &that)
    {
      // 拷贝一个临时数据，然后把自己的交换了
      seqlist<T> temp(that);
      _swap(temp); 
    }
      return *this;
  }


  size_t size() const {return  _size;}
  size_t capacity() const {return _capacity;}

  void print()
  {
    for(size_t i = 0; i < _size; i++)
    {
        cout<< _data[i] << " ";
    }
    cout<<endl;
  }

  void PushBack(T x)
  {
    Checkcapacity();
    _data[_size] = x;
    _size++;
  }

  void PopBack()
  {
    assert(_size > 0);
    _size--;
  }
  
  void PusbFront(T x)
  {
    Checkcapacity();
    for(int i = _size - 1; i >= 0; i--)
    {
      _data[i+1] = _data[i];
    }
    _data[0] = x;
    _size++;
  }
  
  void PopFront()
  {
    assert(_size > 0);
    for(size_t i = 1; i < _size; i++)
    {
      _data[i - 1] = _data[i];
    }
    
    _size--;
  }
  
  int Find(T x)
  {
    for(size_t i = 0; i < _size; i++)
    {
      if(_data[i] == x)
        return i;
    }

    return -1;
  }
  
  void Insert(size_t pos, T x)
  {
    assert(pos <= _size);
    Checkcapacity();

    for(int i = _size - 1; i >= pos; i--)
    {
      _data[i + 1] =  _data[i];
    }
    
    _data[pos] = x;
    
    _size++;
  }



  void Erase(size_t pos)
  {
    assert(pos < _size);
    for(size_t i = pos; i < _size - 1; i++)
    {
      _data[i] = _data[i + 1];
    }

    _size--;
  }


private:
  T* _data;
  size_t _size;
  size_t _capacity;
  
  void Checkcapacity()
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
  
  void _swap(seqlist<T>& that)
  {
    std::swap(_data, that._data);
    std::swap(_size, that._size);
    std::swap(_capacity, that._capacity);
  }
};



int main()
{
  seqlist<int> s1(10);
  cout<< s1.capacity() <<endl;
  cout<< s1.size() <<endl;
  return 0;
}
