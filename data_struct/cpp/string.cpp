#include <iostream>
#include <cstring>
class string 
{
public:
  string()
    :_data(new char[1])
    ,_length(0)
    ,_capacity(0)
  {
    _data[0] = '\0';
  }

  string(const char* str)
    :_data(new char[strlen(str) + 1])
    ,_length(strlen(str))
    ,_capacity(strlen(str))
  {
    if(str == nullptr)
    {
      _data[0] = '\0';
    }
    else 
    {
      memcpy(_data, str, _length);
    }
  }



private:
  char* _data;
  size_t _length;
  size_t _capacity;
};


int main()
{















  return 0;
}
